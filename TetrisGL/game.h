#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <map>
#include <queue>
#include <memory>
#include "Shape/movableshape.h"
#include "QTimer"
#include <set>

//Singleton handling game grid and shapes
class Game : public QObject
{
    Q_OBJECT
public:
    static Game * instance();
    void run();
    void reactToKey(int key);
    void setGameSpeed();

    std::map<std::pair<int,int>,std::pair<ShapeType,bool>> getGrid(){return m_grid;};
    std::map<std::pair<int,int>,ShapeType> getUpcomingGrid(){return m_upcomingGrid;};
    int getGridHeight() const {return m_gridHeight;};
    int getGridWidth() const {return m_gridWidth;};
    int getUpcomingGridHeight() const{return m_upcomingGridHeight;};
    int getUpcomingGridWidth() const {return m_upcomingGridWidth;};
    int getScore() const {return m_score;};

public slots:
    void tick();
    void reset();

private:

    Game();
    bool mMoveCurrentShapeDown();
    void mMoveCurrentShapeRight();
    void mMoveCurrentShapeLeft();
    void mUpdateUpcomingShapesGrid();
    void mGenerateNewMovableShape();
    void mMoveAllWayDown();
    void mClearCurrentShape();
    void mPutCurrentShape();
    bool mIsSquareInGrid(QPoint square);
    void mRotateCurrentShapeClockwise();
    void mRotateCurrentShapeAntiClockwise();
    int mCheckLinesAndUpdate(std::set<int> lines);

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
