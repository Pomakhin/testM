//
//  Utils.cpp
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "Utils.h"
#include <cstdlib>

Board::Board(int width, int height):
m_width(width),
m_height(height)
{
    m_objects.resize(m_height * 2);
    for (auto &row : m_objects)
    {
        row.resize(m_width);
    }
}
int& Board::operator() (int x, int y)
{
    if (y >= m_height ||
        x >= m_width ||
        x < 0)
    {
        return m_outOfBoardValue;
    }
    return m_objects[y+m_height][x];
}
int Board::operator() (int x, int y) const
{
    if (y >= m_height ||
        x >= m_width ||
        x < 0)
    {
        return int{};
    }
    return m_objects[y+m_height][x];
}
int& Board::operator() (const Point &pos)
{
    return operator()(pos.x, pos.y);
}
int  Board::operator() (const Point &pos) const
{
    return operator()(pos.x, pos.y);
}
int Board::width() const
{
    return m_width;
}
int Board::height() const
{
    return m_height;
}
std::multimap<int, Point> Board::getNeighbors(const Point &pos)
{
    std::multimap<int, Point> result;
    if (pos.x > 0)
    {
        result.insert(std::make_pair((*this)(pos.x-1, pos.y), Point(pos.x-1, pos.y)));
    }
    if (pos.x < m_width - 1)
    {
        result.insert(std::make_pair((*this)(pos.x+1, pos.y), Point(pos.x+1, pos.y)));
    }
    if (pos.y > 0)
    {
        result.insert(std::make_pair((*this)(pos.x, pos.y-1), Point(pos.x, pos.y-1)));
    }
    if (pos.y < m_height - 1)
    {
        result.insert(std::make_pair((*this)(pos.x, pos.y+1), Point(pos.x, pos.y+1)));
    }
    return result;
}
bool Board::isInside(const Point &pos)
{
    return (pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height);
}

bool Board::areNeighbors(const Point &first, const Point &second)
{
    if (first.y < 0 || second.y < 0)
    {
        return false;
    }
    if (first.x == second.x &&
        std::abs(first.y - second.y) == 1)
    {
        return true;
    }
    if (first.y == second.y &&
        std::abs(first.x - second.x) == 1)
    {
        return true;
    }
    return false;
}