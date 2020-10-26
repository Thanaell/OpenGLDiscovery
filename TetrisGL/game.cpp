#include "game.h"
#include <QDebug>
#include "tshape.h"
#include <set>

Game * Game::m_instance=nullptr;


void Game::reset(){
    m_score=0;
    emit scoreChanged();
    MovableShape::updateRandomShapesVec();
    m_currentShape=MovableShape::createMovableShape();
    m_currentShapePos={(m_gridWidth-m_currentShape->getSize())/2,m_gridHeight-m_currentShape->getVerticalSize()};
    putCurrentShape();
    for (int x=0; x<m_gridWidth; x++){
        for (int y =0; y<m_gridHeight; y++){
            m_grid[{x,y}].first=ShapeType::EMPTY;
            m_grid[{x,y}].second=false;
        }
    }
    for (int i =0; i<m_upcomingGridWidth; i++){
        for (int j=0; j<m_upcomingGridHeight;j++){
            m_upcomingGrid[{i,j}]=ShapeType::EMPTY;
        }
    }
    for (int i=0; i<nbUpcomingShapes;i++){
        m_upcomingShapes.push_back(MovableShape::createMovableShape());
    }
    updateUpcomingShapesGrid();
    m_timer.start(1000);
    m_timer.blockSignals(false);
}

Game::Game() : nbUpcomingShapes(3),m_timer(new QTimer)
{
    srand (static_cast <unsigned> (time(0)));
    m_gridWidth=10;
    m_gridHeight=20;
    m_upcomingGridWidth=MovableShape::getMaxShapeSize()+2;
    m_upcomingGridHeight=nbUpcomingShapes*MovableShape::getMaxShapeSize();
}

void Game::updateUpcomingShapesGrid(){
    m_upcomingGrid.clear();
    QPoint shapePos;
    for (int i=0; i<nbUpcomingShapes; i++){
        shapePos.setY(i*MovableShape::getMaxShapeSize());
        auto &shape=m_upcomingShapes[nbUpcomingShapes-i-1];
        ShapeType type=shape->getType();
        auto squares=shape->getAbsoluteSquares(shapePos);
        for (auto square : squares){
            m_upcomingGrid[{square.x(),square.y()}]=type;
        }
    }
}

void Game::clearCurrentShape(){
    //removes shape from grid
    for (auto point : m_currentShape->getSquares()){
        m_grid[{m_currentShapePos.x()+point.x(),m_currentShapePos.y()+point.y()}].first=ShapeType::EMPTY;
    }
}

void Game::putCurrentShape(){
    //replaces shape in grid
    for (auto point : m_currentShape->getSquares()){
        m_grid[{m_currentShapePos.x()+point.x(),m_currentShapePos.y()+point.y()}].first=m_currentShape->getType();
        m_grid[{m_currentShapePos.x()+point.x(),m_currentShapePos.y()+point.y()}].second=true;
    }
}

void Game::run(){
    reset();
    QObject::connect(&m_timer,&QTimer::timeout, this, &Game::tick);
}

void Game::moveCurrentShapeRight(){
    auto squares=m_currentShape->getAbsoluteSquares(m_currentShapePos);
    bool hasCollided=false;
    for (auto square : squares){
        if (square.x()==m_gridWidth-1 || (m_grid[{square.x()+1,square.y()}].first!=ShapeType::EMPTY && !m_grid[{square.x()+1,square.y()}].second)){
            hasCollided=true;
            break;
        }
    }
    if(!hasCollided){
       clearCurrentShape();
       m_currentShapePos.setX(m_currentShapePos.x()+1);
       putCurrentShape();
    }
}
void Game::moveCurrentShapeLeft(){
    auto squares=m_currentShape->getAbsoluteSquares(m_currentShapePos);
    bool hasCollided=false;
    for (auto square : squares){
        if (square.x()==0 || (m_grid[{square.x()-1,square.y()}].first!=ShapeType::EMPTY && !m_grid[{square.x()-1,square.y()}].second)){
            hasCollided=true;
            break;
        }
    }
    if(!hasCollided){
       clearCurrentShape();
       m_currentShapePos.setX(m_currentShapePos.x()-1);
       putCurrentShape();
    }
}

int Game::checkLinesAndUpdate(std::set<int> lines){
    std::map<int,int> elementsPerLine;
    std::vector<int> linesToUpdate;
    for (int y : lines){
        elementsPerLine[y]=0;
        for (int x=0; x<m_gridWidth; x++){
            if (m_grid[{x,y}].first!=ShapeType::EMPTY){
                elementsPerLine[y]++;
            }
        }
    }
    for (auto element : elementsPerLine){
        if (element.second==m_gridWidth){
            linesToUpdate.push_back(element.first);
        }
    }
    std::sort(linesToUpdate.begin(),linesToUpdate.end());
    std::reverse(linesToUpdate.begin(),linesToUpdate.end());

    for (auto line : linesToUpdate){
        for (int y = line; y<m_gridHeight-1; y++){
            for (int x=0; x<m_gridWidth; x++){
                if (!m_grid[{x,y+1}].second){
                    m_grid[{x,y}].first=m_grid[{x,y+1}].first;
                    m_grid[{x,y}].second=false;

                }
            }
        }
    }

    m_score+=linesToUpdate.size();
    if(linesToUpdate.size()!=0){
        emit scoreChanged();
    }
    return linesToUpdate.size();
}

void Game::generateNewMovableShape(){
    m_currentShape=std::move(m_upcomingShapes.front());
    m_upcomingShapes.pop_front();
    m_upcomingShapes.push_back(MovableShape::createMovableShape());
    updateUpcomingShapesGrid();
}

bool Game::moveCurrentShapeDown(){
    bool hasCollided=false;
    QPoint previousShapePos=m_currentShapePos;
    std::vector<QPoint> previousShapeSquares=m_currentShape->getAbsoluteSquares(previousShapePos);
    for (auto square : m_currentShape->getAbsoluteSquares(m_currentShapePos)){
        if (square.y()==0 || (!m_grid[{square.x(),square.y()-1}].second && m_grid[{square.x(),square.y()-1}].first!=ShapeType::EMPTY)){
            generateNewMovableShape();
            m_currentShapePos={(m_gridWidth-m_currentShape->getSize())/2,m_gridHeight-m_currentShape->getVerticalSize()};
            hasCollided=true;
            break;
        }
    }
    if (hasCollided){
        std::set<int> lines;
        //set previous shape not moving
        for (auto square : previousShapeSquares){
            m_grid[{square.x(),square.y()}].second=false;
            lines.insert(square.y());
        }
        int nbLinesDeleted=checkLinesAndUpdate(lines);
        bool isGameOver=false;
        for (auto square : previousShapeSquares){
            if (square.y()-nbLinesDeleted>=m_gridHeight-1){
                m_timer.blockSignals(true);
                emit gameOver(m_score);
                isGameOver=true;
                reset();
                break;
            }
        }
        if(!isGameOver){
            putCurrentShape();
        }
    }
    else{
        for (auto square : previousShapeSquares){
            m_grid[{square.x(),square.y()}].second=false;
        }
        clearCurrentShape();
        m_currentShapePos.setY(m_currentShapePos.y()-1);
        putCurrentShape();
    }   
    return hasCollided;
}

void Game::moveAllWayDown(){
    bool isDown=false;
    while (!isDown){
        isDown=moveCurrentShapeDown();
    }
}

void Game::tick(){
    moveCurrentShapeDown();
}

Game * Game::instance(){
    if (m_instance){
        return m_instance;
    }
    else{
        m_instance=new Game();
        return m_instance;
    }
}

void Game::rotateCurrentShapeClockwise(){
    clearCurrentShape();
    m_currentShape->rotateClockwise();
    for (auto square:m_currentShape->getAbsoluteSquares(m_currentShapePos)){
        if (!isSquareInGrid(square)){
            m_currentShape->rotateAntiClockwise();
            break;
        }
    }
    putCurrentShape();
}

bool Game::isSquareInGrid(QPoint square){
    if (square.x()>=m_gridWidth){
        return false;
    }
    if (square.x()<=0){
        return false;
    }
    if (square.y()>=m_gridHeight){
        return false;
    }
    if (square.y()<=0){
        return false;
    }
    return true;
}

void Game::rotateCurrentShapeAntiClockwise(){
    clearCurrentShape();
    m_currentShape->rotateAntiClockwise();
    for (auto square:m_currentShape->getAbsoluteSquares(m_currentShapePos)){
        if (!isSquareInGrid(square)){
            m_currentShape->rotateClockwise();
            break;
        }
    }
    putCurrentShape();
}

void Game::reactToKey(int key){
    if (key==Qt::Key::Key_E){
        rotateCurrentShapeClockwise();
    }
    else if (key==Qt::Key::Key_A){
        rotateCurrentShapeAntiClockwise();
    }
    else if (key==Qt::Key::Key_D){
        moveCurrentShapeRight();
    }
    else if (key==Qt::Key::Key_Q){
        moveCurrentShapeLeft();
    }
    else if (key==Qt::Key::Key_S){
        moveCurrentShapeDown();
    }
    else if (key==Qt::Key::Key_Z){
        moveAllWayDown();
    }
}
