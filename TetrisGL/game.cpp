#include "game.h"
#include <QDebug>
#include "tshape.h"
#include <set>

Game * Game::m_instance=nullptr;


Game::Game() : m_timer(new QTimer)
{
    srand (static_cast <unsigned> (time(0)));
    m_gridWidth=10;
    m_gridHeight=20;
    m_currentShape=MovableShape::createMovableShape();
    m_currentShapePos={(m_gridWidth-m_currentShape->getSize())/2,m_gridHeight-m_currentShape->getVerticalSize()};
    putCurrentShape();
    for (int x=0; x<m_gridWidth; x++){
        for (int y =0; y<m_gridHeight; y++){
            m_grid[{x,y}].first=ShapeType::EMPTY;
            m_grid[{x,y}].second=false;
        }
    }
    m_bottomShape=std::unique_ptr<BottomShape>(new BottomShape(m_gridWidth));
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
    m_timer.start(1000);
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

void Game::checkLinesAndUpdate(std::set<int> lines){
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
}
void Game::moveCurrentShapeDown(){
    bool hasCollided=false;
    QPoint previousShapePos=m_currentShapePos;
    std::vector<QPoint> previousShapeSquares=m_currentShape->getAbsoluteSquares(previousShapePos);
    for (auto square : m_currentShape->getAbsoluteSquares(m_currentShapePos)){
        if (square.y()==0 || (!m_grid[{square.x(),square.y()-1}].second && m_grid[{square.x(),square.y()-1}].first!=ShapeType::EMPTY)){
            m_currentShape=MovableShape::createMovableShape();
            m_currentShapePos={(m_gridWidth-m_currentShape->getSize())/2,m_gridHeight-m_currentShape->getVerticalSize()};
            putCurrentShape();
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
        checkLinesAndUpdate(lines);
    }
    else{
        for (auto square : previousShapeSquares){
            m_grid[{square.x(),square.y()}].second=false;
        }
        clearCurrentShape();
        m_currentShapePos.setY(m_currentShapePos.y()-1);
        putCurrentShape();
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

void Game::reactToKey(int key){
    if (key==Qt::Key::Key_E){
        clearCurrentShape();
        m_currentShape->rotateClockwise();
        putCurrentShape();
    }
    else if (key==Qt::Key::Key_A){
        clearCurrentShape();
        m_currentShape->rotateAntiClockwise();
        putCurrentShape();
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
}
