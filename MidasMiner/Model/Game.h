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

typedef std::set<Point > ObjectsPositionSet;
typedef std::list<std::pair<Point, Point > > ObjectsMovesList;
typedef std::list<Point > ObjectsPositionList;

class Observer
{
public:
    virtual void onInitBoard(){};
    virtual void onSelectObject(const Point &pos){};
    virtual void onSwapObjects(const Point &firstPos, const Point &secondPos){};
    virtual void onRemoveObjects(const ObjectsMovesList &dropsLis){};
};

// Game model
class Game : public Singleton<Game>
{
    friend class Singleton<Game>;
private:
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
    int m_score = 0;
    
    std::list<Observer *> m_observers;
    
    int getRandomObjType();
    void generateInitialBoard();
    ObjectsPositionSet getObjectsToRemove(bool needCalcScore);
    void replaceWithUnique(const ObjectsPositionSet &objectsIds);
    void notify(NotifyFunc func);
    void doSwap(const Point &firstPos, const Point &secondPos, const SwapData::SwapState &state);
    bool removeObjects();
protected:
    Game();
public:
    void registerObserver(Observer *observer);
    void init();    
    
    bool getPossibleMove(ObjectsPositionList *moveContents);
    void setSelected(const Point &point);
    const Board &getBoard() {return m_board;}
    
    bool getBlockControls() {return m_blockControls;}
    
    // Views notify Game directly
    void onEndSwapping();
    void onEndRemoving();
    
    int getScore() {return m_score;}
    
    // for debug
    void printObjects();
    
    //for tests
    void setTestBoard(const Board &board) {m_board = board;};
};

#endif /* defined(__MidasMiner__Game__) */
