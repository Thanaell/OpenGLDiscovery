#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <QPoint>

class Shape
{
public:
    Shape();
    void addSquare(QPoint p);
    void removeSquare(QPoint p);
    std::vector<QPoint> getSquares(){return m_squares;};

protected:
    std::vector<QPoint> m_squares;
};

#endif // SHAPE_H
