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
#include "GenericSingleton.h"
#include "Utils.h"

class Observer
{
public:
    virtual void onInitBoard(){};
    virtual void onSelectObject(const Point &pos){};
    virtual void onSwapObjects(const Point &firstPos, const Point &secondPos){};
    virtual void onRemoveObjects(){};
};

class Game : public Singleton<Game>
{
    friend class Singleton<Game>;
public:
    typedef std::set<Point > ObjectsPositionSet;
private:
    typedef std::list<Point > ObjectsPositionList;
    typedef std::function<void(Observer*)> NotifyFunc;
    struct SwapData
    {
        enum SwapState
        {
            None,
            Direct,
            Reverse
        };
        SwapState State = SwapState::None;
        Point FirstPos;
        Point SecondPos;
    };
    
    SwapData m_swap;
    Board m_board;
    std::pair<bool, Point> m_selected;
    bool m_blockControls = false;
    
    std::list<Observer *> m_observers;
    
    void generateInitialBoard();
    ObjectsPositionSet getObjectsToRemove(bool needCalcScore);
    bool getPossibleMove(ObjectsPositionList *moveContents);
    void replaceWithUnique(const ObjectsPositionSet &objectsIds);
    void notify(NotifyFunc func);
    void doSwap(const Point &firstPos, const Point &secondPos, const SwapData::SwapState &state);
    void removeObjects(const ObjectsPositionSet &objects);
protected:
    Game();
public:
    void setSelected(const Point &point);
    bool getSelected(Point &point);
    void init();
    const Board &getBoard() {return m_board;}
    bool getBlockControls() {return m_blockControls;}
    void onEndSwapping();
    
    void registerObserver(Observer *observer);
    
    // for debug
    void printObjects();
};

#endif /* defined(__MidasMiner__Game__) */
