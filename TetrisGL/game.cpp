#include "game.h"
#include <QDebug>
#include "tshape.h"

Game * Game::m_instance=nullptr;


Game::Game() : m_timer(new QTimer)
{
    srand (static_cast <unsigned> (time(0)));
    m_gridWidth=10;
    m_gridHeight=50;
    m_currentShape=MovableShape::createMovableShape();
    m_currentShapePos={(m_gridWidth-m_currentShape->getSize())/2,m_gridHeight-m_currentShape->getSize()-3};
    putCurrentShape();
    for (int i =0; i<m_gridHeight; i++){
        for (int j=0; j<m_gridWidth; j++){
            m_grid[{i,j}]=ShapeType::EMPTY;
        }
    }
}

void Game::clearCurrentShape(){
    //removes shape from grid
    for (auto point : m_currentShape->getSquares()){
        m_grid[{m_currentShapePos.y()+point.y(),m_currentShapePos.x()+point.x()}]=ShapeType::EMPTY;
    }
}

void Game::putCurrentShape(){
    //replaces shape in grid
    for (auto point : m_currentShape->getSquares()){
        m_grid[{m_currentShapePos.y()+point.y(),m_currentShapePos.x()+point.x()}]=m_currentShape->getType();
    }
}

void Game::run(){
    m_timer.start(1000);
    QObject::connect(&m_timer,&QTimer::timeout, this, &Game::tick);
}

void Game::moveCurrentShapeRight(){
    //TODO : check collisions with bottom shape

    //if no collisions

    //moves shape right
    clearCurrentShape();  
    int rightMargin=m_gridWidth-m_currentShape->getSize()-1;
    int test=m_currentShapePos.x()-m_currentShape->getRightSpace();
    if(test<rightMargin){
       m_currentShapePos.setX(m_currentShapePos.x()+1);
    }
    putCurrentShape();
}
void Game::moveCurrentShapeLeft(){
    //TODO : check collisions with bottom shape

    //if no collisions

    //moves shape down
    clearCurrentShape();
    int leftMargin=0;
    int test=m_currentShapePos.x()+m_currentShape->getLeftSpace();
    if(test>leftMargin){
       m_currentShapePos.setX(m_currentShapePos.x()-1);
    }
    putCurrentShape();

}
void Game::moveCurrentShapeDown(){
    //TODO : check collisions with bottom shape

    //if no collisions

    //moves shape down
    clearCurrentShape();
    m_currentShapePos.setY(m_currentShapePos.y()-1);
    if(m_currentShapePos.y()==0){
        m_currentShape=MovableShape::createMovableShape();
        m_currentShapePos={(m_gridWidth-m_currentShape->getSize())/2,m_gridHeight-m_currentShape->getSize()-m_currentShape->getSize()-3};
    }
    putCurrentShape();


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
