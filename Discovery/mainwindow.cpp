#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_GLWidget=new GLWidget(this);
    setCentralWidget(m_GLWidget);
}

MainWindow::~MainWindow()
{
}

