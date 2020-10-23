#include "mainwindow.h"

MainWindow::MainWindow(std::unique_ptr<Game> game,QWidget *parent)
    : QMainWindow(parent)
{

    m_GLView=new GLView(std::move(game),this);
    setCentralWidget(m_GLView);
}

MainWindow::~MainWindow()
{
}

