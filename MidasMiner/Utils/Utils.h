//
//  Utils.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__Utils__
#define __MidasMiner__Utils__

#include <vector>
#include <map>

struct Point
{
    int x{};
    int y{};
    Point(int a_x, int a_y): x(a_x), y(a_y){}
    Point(){}
    bool operator<(const Point &rhs) const
    {
        if((x < rhs.x))
        {
            return true;
        }
        if(x == rhs.x && y < rhs.y)
        {
            return true;
        }
        return false;
    }
    bool operator==(const Point &rhs) const
    {
        return (x==rhs.x && y==rhs.y);
    }
    bool operator!=(const Point &rhs) const
    {
        return !(*this == rhs);
    }
};

class Board
{
private:
    int m_width {};
    int m_height {};
    std::vector<std::vector<int> > m_objects;
    int m_outOfBoardValue = 0;
public:
    Board(int width, int height);
    int& operator() (int x, int y);
    int  operator() (int x, int y) const;
    int& operator() (const Point &pos);
    int  operator() (const Point &pos) const;
    int width() const;
    int height() const ;
    std::multimap<int, Point> getNeighbors(const Point &pos);
    bool isInside(const Point &pos);
};

#endif /* defined(__MidasMiner__Utils__) */
