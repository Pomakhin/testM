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

class Game : public Singleton<Game>
{
    friend class Singleton<Game>;
private:
    typedef std::set<Point > ObjectsPositionSet;
    typedef std::list<Point > ObjectsPositionList;
    
    Board m_board;
    
    void generateInitialBoard();
    ObjectsPositionSet getObjectsToDelete(bool needCalcScore);
    bool getPossibleMove(ObjectsPositionList *moveContents);
    void replaceWithUnique(const ObjectsPositionSet &objectsIds);
protected:
    Game();
public:
    void init();
    void printObjects();
    const Board &getBoard() {return m_board;}
};

#endif /* defined(__MidasMiner__Game__) */
