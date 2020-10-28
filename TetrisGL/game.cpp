#include "game.h"
#include <QDebug>
#include "Shape/tshape.h"
#include <set>

Game * Game::m_instance=nullptr;

//---------------------------------------------------
Game * Game::instance(){
    if (m_instance){
        return m_instance;
    }
    else{
        m_instance=new Game();
        return m_instance;
    }
}

//---------------------------------------------------
Game::Game() : nbUpcomingShapes(3),m_timer(new QTimer)
{
    srand (static_cast <unsigned> (time(0)));
    m_gridWidth=10;
    m_gridHeight=20;
    m_upcomingGridWidth=MovableShape::getMaxShapeSize()+2;
    m_upcomingGridHeight=nbUpcomingShapes*MovableShape::getMaxShapeSize();
}

//---------------------------------------------------
void Game::reset(){
    m_score=0;
    emit scoreChanged();
    MovableShape::updateRandomShapesVec();
    m_currentShape=MovableShape::createMovableShape();
    m_currentShapePos={(m_gridWidth-m_currentShape->getSize())/2,m_gridHeight-m_currentShape->getVerticalSize()-m_currentShape->getLowestYShape()};
    mPutCurrentShape();
    //Fills grid with empty shapes
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
    mUpdateUpcomingShapesGrid();
    m_timer.start(1000);
    m_timer.blockSignals(false);
}

//---------------------------------------------------
void Game::run(){
    reset();
    QObject::connect(&m_timer,&QTimer::timeout, this, &Game::tick);
}

//---------------------------------------------------
void Game::tick(){
    mMoveCurrentShapeDown();
}

//---------------------------------------------------
void Game::setGameSpeed(){
    if (m_msBetweenTicks>200){
        m_msBetweenTicks=1000-100*m_score;
    }
}

//---------------------------------------------------
void Game::mUpdateUpcomingShapesGrid(){
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

//---------------------------------------------------
void Game::mClearCurrentShape(){
    //removes shape from grid
    for (auto point : m_currentShape->getSquares()){
        m_grid[{m_currentShapePos.x()+point.x(),m_currentShapePos.y()+point.y()}].first=ShapeType::EMPTY;
    }
}

//---------------------------------------------------
void Game::mPutCurrentShape(){
    //replaces shape in grid
    for (auto point : m_currentShape->getSquares()){
        m_grid[{m_currentShapePos.x()+point.x(),m_currentShapePos.y()+point.y()}].first=m_currentShape->getType();
        m_grid[{m_currentShapePos.x()+point.x(),m_currentShapePos.y()+point.y()}].second=true;
    }
}

//---------------------------------------------------
void Game::reactToKey(int key){
    switch (key) {
    case Qt::Key::Key_E:
        mRotateCurrentShapeClockwise();
        break;
    case Qt::Key::Key_A:
        mRotateCurrentShapeAntiClockwise();
        break;
    case Qt::Key::Key_Q:
        mMoveCurrentShapeLeft();
        break;
    case Qt::Key::Key_D:
        mMoveCurrentShapeRight();
        break;
    case Qt::Key::Key_S:
        mMoveCurrentShapeDown();
        break;
    case Qt::Key::Key_Z:
        mMoveAllWayDown();
        break;
    default:
        break;
    }
}

//---------------------------------------------------
void Game::mRotateCurrentShapeClockwise(){
    mClearCurrentShape();
    m_currentShape->rotateClockwise();
    for (auto square:m_currentShape->getAbsoluteSquares(m_currentShapePos)){
        if (!mIsSquareInGrid(square)){
            m_currentShape->rotateAntiClockwise();
            break;
        }
    }
    mPutCurrentShape();
}

//---------------------------------------------------
void Game::mRotateCurrentShapeAntiClockwise(){
    mClearCurrentShape();
    m_currentShape->rotateAntiClockwise();
    for (auto square:m_currentShape->getAbsoluteSquares(m_currentShapePos)){
        if (!mIsSquareInGrid(square)){
            m_currentShape->rotateClockwise();
            break;
        }
    }
    mPutCurrentShape();
}

//---------------------------------------------------
void Game::mMoveCurrentShapeRight(){
    auto squares=m_currentShape->getAbsoluteSquares(m_currentShapePos);
    bool hasCollided=false;
    for (auto square : squares){
        if (square.x()==m_gridWidth-1 || (m_grid[{square.x()+1,square.y()}].first!=ShapeType::EMPTY && !m_grid[{square.x()+1,square.y()}].second)){
            hasCollided=true;
            break;
        }
    }
    if(!hasCollided){
       mClearCurrentShape();
       m_currentShapePos.setX(m_currentShapePos.x()+1);
       mPutCurrentShape();
    }
}

//---------------------------------------------------
void Game::mMoveCurrentShapeLeft(){
    auto squares=m_currentShape->getAbsoluteSquares(m_currentShapePos);
    bool hasCollided=false;
    for (auto square : squares){
        if (square.x()==0 || (m_grid[{square.x()-1,square.y()}].first!=ShapeType::EMPTY && !m_grid[{square.x()-1,square.y()}].second)){
            hasCollided=true;
            break;
        }
    }
    if(!hasCollided){
       mClearCurrentShape();
       m_currentShapePos.setX(m_currentShapePos.x()-1);
       mPutCurrentShape();
    }
}

//---------------------------------------------------
bool Game::mMoveCurrentShapeDown(){
    bool hasCollided=false;
    QPoint previousShapePos=m_currentShapePos;
    std::vector<QPoint> previousShapeSquares=m_currentShape->getAbsoluteSquares(previousShapePos);
    //check for collisions
    for (auto square : m_currentShape->getAbsoluteSquares(m_currentShapePos)){
        if (square.y()==0 || (!m_grid[{square.x(),square.y()-1}].second && m_grid[{square.x(),square.y()-1}].first!=ShapeType::EMPTY)){
            mGenerateNewMovableShape();
            m_currentShapePos={(m_gridWidth-m_currentShape->getSize())/2,m_gridHeight-m_currentShape->getVerticalSize()-m_currentShape->getLowestYShape()};
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
        //deletes lines if needeed
        int nbLinesDeleted=mCheckLinesAndUpdate(lines);
        bool isGameOver=false;

        //checks for game over
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
            mPutCurrentShape();
        }
    }
    //if no collisions, resets previous position in grid to not moving
    else{
        for (auto square : previousShapeSquares){
            m_grid[{square.x(),square.y()}].second=false;
        }
        mClearCurrentShape();
        m_currentShapePos.setY(m_currentShapePos.y()-1);
        mPutCurrentShape();
    }
    return hasCollided;
}

//---------------------------------------------------
void Game::mMoveAllWayDown(){
    bool isDown=false;
    while (!isDown){
        isDown=mMoveCurrentShapeDown();
    }
}

//---------------------------------------------------
int Game::mCheckLinesAndUpdate(std::set<int> lines){
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
    //lines for highest to lowest
    std::sort(linesToUpdate.begin(),linesToUpdate.end());
    std::reverse(linesToUpdate.begin(),linesToUpdate.end());

    //shifts lines down
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
        m_timer.stop();
        setGameSpeed();
        m_timer.start(m_msBetweenTicks);
        emit scoreChanged();
    }
    return linesToUpdate.size();
}

//---------------------------------------------------
void Game::mGenerateNewMovableShape(){
    m_currentShape=std::move(m_upcomingShapes.front());
    m_upcomingShapes.pop_front();
    m_upcomingShapes.push_back(MovableShape::createMovableShape());
    mUpdateUpcomingShapesGrid();
}

//---------------------------------------------------
bool Game::mIsSquareInGrid(QPoint square){
    if (square.x()>=m_gridWidth){
        return false;
    }
    if (square.x()<0){
        return false;
    }
    if (square.y()>=m_gridHeight){
        return false;
    }
    if (square.y()<0){
        return false;
    }
    return true;
}



