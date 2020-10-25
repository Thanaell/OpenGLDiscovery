#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <map>
#include <queue>
#include <memory>
#include "movableshape.h"
#include "QTimer"
#include <set>


class Game : public QObject
{
    Q_OBJECT
public:
    static Game * instance();
    void run();
    void reactToKey(int key);
    void moveCurrentShapeDown();
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
    int checkLinesAndUpdate(std::set<int> lines);
    void removeLine(int line);
    void updateUpcomingShapesGrid();
    void generateNewMovableShape();
    void reset();
public slots:
    void tick();
private:
    Game();
    static Game * m_instance;
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
    void gameUpdated();
};

#endif // GAME_H
