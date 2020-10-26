#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <map>
#include <queue>
#include <memory>
#include "Shape/movableshape.h"
#include "QTimer"
#include <set>


class Game : public QObject
{
    Q_OBJECT
public:
    static Game * instance();
    void run();
    void reactToKey(int key);
    bool moveCurrentShapeDown();
    void moveAllWayDown();
    void clearCurrentShape();
    void putCurrentShape();
    std::map<std::pair<int,int>,std::pair<ShapeType,bool>> getGrid(){return m_grid;};
    std::map<std::pair<int,int>,ShapeType> getUpcomingGrid(){return m_upcomingGrid;};
    int getGridHeight(){return m_gridHeight;};
    int getGridWidth(){return m_gridWidth;};
    int getUpcomingGridHeight(){return m_upcomingGridHeight;};
    int getUpcomingGridWidth(){return m_upcomingGridWidth;};
    void moveCurrentShapeRight();
    void moveCurrentShapeLeft();
    void rotateCurrentShapeClockwise();
    void rotateCurrentShapeAntiClockwise();
    int checkLinesAndUpdate(std::set<int> lines);
    void removeLine(int line);
    void updateUpcomingShapesGrid();
    void generateNewMovableShape();

    bool isSquareInGrid(QPoint square);
    int getScore(){return m_score;};
    void setGameSpeed();
public slots:
    void tick();
    void reset();
private:
    Game();
    static Game * m_instance;
    int m_msBetweenTicks;
    int m_score;
    int m_gridHeight;
    int m_gridWidth;
    int m_upcomingGridHeight;
    int m_upcomingGridWidth;
    std::map<std::pair<int,int>,std::pair<ShapeType, bool>> m_grid;
    std::map<std::pair<int,int>,ShapeType> m_upcomingGrid;
    std::unique_ptr<MovableShape> m_currentShape;
    QPoint m_currentShapePos;
    std::deque<std::unique_ptr<MovableShape>> m_upcomingShapes;
    int nbUpcomingShapes;
    QTimer m_timer;


signals:
    void scoreChanged();
    void gameOver(int score);
};

#endif // GAME_H
