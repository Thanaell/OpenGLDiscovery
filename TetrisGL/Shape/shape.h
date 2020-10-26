#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <QPoint>

class Shape
{
public:
    Shape(int size);
    void addSquare(QPoint p);
    void removeSquare(QPoint p);
    std::vector<QPoint> getSquares(){return m_squares;};
    virtual int getSize() const {return m_shapeSize;};

protected:
    std::vector<QPoint> m_squares;
    int m_shapeSize;
    int m_gridSize;
};

#endif // SHAPE_H
