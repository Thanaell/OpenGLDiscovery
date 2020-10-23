#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <map>
#include <queue>
#include <memory>
#include "movableshape.h"
#include "bottomshape.h"
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
    int getGridHeight(){return m_gridHeight;};
    int getGridWidth(){return m_gridWidth;};
    void moveCurrentShapeRight();
    void moveCurrentShapeLeft();
    void checkLinesAndUpdate(std::set<int> lines);
    void removeLine(int line);
public slots:
    void tick();
private:
    Game();
    static Game * m_instance;
    int m_gridHeight;
    int m_gridWidth;
    std::map<std::pair<int,int>,std::pair<ShapeType, bool>> m_grid;
    std::unique_ptr<MovableShape> m_currentShape;
    QPoint m_currentShapePos;
    std::queue<std::unique_ptr<MovableShape>> m_upcomingShapes;
    std::unique_ptr<BottomShape> m_bottomShape;
    QTimer m_timer;


signals:
    void gameUpdated();
};

#endif // GAME_H
