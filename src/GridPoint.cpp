#include "GridPoint.h"

GridPoint& GridPoint::operator+=(const GridPoint& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

GridPoint& GridPoint::operator+=(const Translation& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}
