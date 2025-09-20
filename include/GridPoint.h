// -*- C++ -*-

#pragma once

struct Translation {
    int x;
    int y;
};

struct GridPoint {
    int x;
    int y;

    GridPoint& operator+=(const GridPoint& rhs);
    friend GridPoint operator+(GridPoint lhs, const GridPoint& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    GridPoint& operator+=(const Translation& rhs);
    friend GridPoint operator+(GridPoint lhs, const Translation& rhs)
    {
        lhs += rhs;
        return lhs;
    }
};
