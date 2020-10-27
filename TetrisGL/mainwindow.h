#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "game.h"

class GLView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<Game> game,QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void mDisplayGameOver(int score);
private:
    GLView * m_GLView;
    std::shared_ptr<Game> m_game;
};
#endif // MAINWINDOW_H
