#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glview.h"
#include "game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<Game> game,QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void displayGameOver(int score);
private:
    GLView * m_GLView;
    std::shared_ptr<Game> m_game;
};
#endif // MAINWINDOW_H
