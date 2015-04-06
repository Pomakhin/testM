//
//  Game.h
//  MidasMiner
//
//  Created by Михаил Помахин on 04.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__Game__
#define __MidasMiner__Game__

#include <stdio.h>

#include <set>
#include <list>
#include <vector>
#include <map>

struct Point
{
    long x{};
    long y{};
    Point(long a_x, long a_y): x(a_x), y(a_y){}
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
    long m_width {};
    long m_height {};
    std::vector<std::vector<long> > m_objects;
    long m_outOfBoardValue{};
public:
    Board(long width, long height);
    long& operator() (long x, long y);
    long  operator() (long x, long y) const;
    long& operator() (const Point &pos);
    long  operator() (const Point &pos) const;
    long width();
    long height();
    std::multimap<long, Point> getNeighbors(const Point &pos);
    bool isInside(const Point &pos);
};

class Game
{
private:
    typedef std::set<Point > ObjectsPositionSet;
    typedef std::list<Point > ObjectsPositionList;
    static Game *m_instance;
    
    Board m_board;
    
    void generateInitialBoard();
    ObjectsPositionSet getObjectsToDelete(bool needCalcScore);
    bool getPossibleMove(ObjectsPositionList *moveContents);
    void replaceWithUnique(const ObjectsPositionSet &objectsIds);
protected:
    Game();
public:
    static Game *getInstanse();
    
    void printObjects();
};

#endif /* defined(__MidasMiner__Game__) */
