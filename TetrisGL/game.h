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
public slots:
    void tick();
private:
    Game();
    static Game * m_instance;
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
