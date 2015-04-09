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

namespace Utils
{
    template <typename T> int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

};

class Timer
{
public:
    typedef std::function<void(int)> OnTimerFunction;
    typedef std::function<void()> OnCompleteTimerFunction;
private:
    OnTimerFunction m_onTimer;
    OnCompleteTimerFunction m_onCompleteTimerFunction;
    double m_timeToStop;
    int m_currentRest = 0;
public:
    void init(double a_timeToStop);
    void setOnTimerFunction(OnTimerFunction a_onTimer);
    void setOnCompleteTimerFunction(OnCompleteTimerFunction a_onComplete);
    Timer();
    bool isActive();
    int getCurrentRest() {return m_currentRest;}
    void update();
    void stop();
};


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


// Board class also contains virtual part of game board that uses to create new game objects after matches removing
class Board
{
private:
    int m_width {};
    int m_height {};
    std::vector<std::vector<int> > m_objects;
    
    // to uniformity consider out of board points as points with 0 value
    int m_outOfBoardValue = 0;
public:
    Board(int width, int height);
    int& operator() (int x, int y);
    int  operator() (int x, int y) const;
    int& operator() (const Point &pos);
    int  operator() (const Point &pos) const;
    int width() const;
    int height() const ;
    
    // return map of objects surrounding given point (max 4)
    //  key - object type
    //  value - point
    std::multimap<int, Point> getNeighbors(const Point &pos);
    
    // return true if point is inside actual board (not virtual)
    bool isInside(const Point &pos);
    
    // return true is points have own side
    static bool areNeighbors(const Point &first, const Point &second);
};

#endif /* defined(__MidasMiner__Utils__) */
