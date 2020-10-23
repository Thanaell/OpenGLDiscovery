#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <map>
#include <queue>
#include <memory>
#include "movableshape.h"
#include "bottomshape.h"
#include "QTimer"


class Game : public QObject
{
    Q_OBJECT
public:
    static Game * instance();
    void run();
    void reactToKey(int key);
    void currentShapeDown();
    void clearCurrentShape();
    void putCurrentShape();
    std::map<std::pair<int,int>,ShapeType> getGrid(){return m_grid;};
    int getGridHeight(){return m_gridHeight;};
    int getGridWidth(){return m_gridWidth;};
public slots:
    void tick();
private:
    Game();
    static Game * m_instance;
    int m_gridHeight;
    int m_gridWidth;
    std::map<std::pair<int,int>,ShapeType> m_grid;
    std::shared_ptr<MovableShape> m_currentShape;
    QPoint m_currentShapePos;
    std::queue<std::shared_ptr<MovableShape>> m_upcomingShapes;
    std::shared_ptr<BottomShape> m_bottomShape;
    QTimer m_timer;

signals:
    void gameUpdated();
};

#endif // GAME_H
