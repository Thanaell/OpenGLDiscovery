#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glview.h"
#include "game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::unique_ptr<Game> game,QWidget *parent = nullptr);
    ~MainWindow();
private:
    GLView * m_GLView;
};
#endif // MAINWINDOW_H
