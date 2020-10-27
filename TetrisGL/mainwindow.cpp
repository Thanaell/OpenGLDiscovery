#include "mainwindow.h"
#include "glview.h"

#include <QMessageBox>
#include <QAbstractButton>

//-------------------------------------------
MainWindow::MainWindow(std::shared_ptr<Game> game,QWidget *parent)
    : QMainWindow(parent), m_game(game)
{

    QObject::connect(game.get(),&Game::gameOver,this,&MainWindow::mDisplayGameOver);
    m_GLView=new GLView(m_game,this);
    setCentralWidget(m_GLView);
}

//-------------------------------------------
MainWindow::~MainWindow()
{

}

//-------------------------------------------
void MainWindow::mDisplayGameOver(int score){
    QMessageBox *box= new QMessageBox();
    box->setText("Game Over : "+QString::number(score));
    box->exec();
    QObject::connect(box, &QMessageBox::buttonClicked, m_game.get(), &Game::reset );
}
