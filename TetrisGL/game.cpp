#include "game.h"
#include <QDebug>
#include "tshape.h"

Game * Game::m_instance=nullptr;

Game::Game() : m_timer(new QTimer)
{
    m_gridWidth=10;
    m_gridHeight=50;
    m_currentShape=std::unique_ptr<MovableShape>(new TShape());
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

void Game::currentShapeDown(){
    //TODO : check collisions with bottom shape

    //if no collisions

    //moves shape down
    clearCurrentShape();
    m_currentShapePos.setY(m_currentShapePos.y()-1);
    putCurrentShape();

}

void Game::tick(){
    currentShapeDown();
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
    if (key==Qt::Key::Key_D){
        clearCurrentShape();
        m_currentShape->rotateClockwise();
        putCurrentShape();
    }
    else if (key==Qt::Key::Key_Q){
        clearCurrentShape();
        m_currentShape->rotateAntiClockwise();
        putCurrentShape();
    }
}
