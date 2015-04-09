//
//  Game.cpp
//  MidasMiner
//
//  Created by Михаил Помахин on 04.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "Game.h"

#include <iostream>
#include "Consts.h"
#include <cassert>

Game::Game():
    m_board(C_BOARD_WIDTH, C_BOARD_HEIGHT)
{
    srand(static_cast<unsigned>(time(nullptr)));
}

void Game::init()
{
    m_score = 0;
    generateInitialBoard();    
}

void Game::registerObserver(Observer *observer)
{
    m_observers.push_back(observer);
}

void Game::notify(NotifyFunc func)
{
    for (auto observer : m_observers)
    {
        func(observer);
    }
}

int Game::getRandomObjType()
{
    return rand() % C_OBJECTS_TYPES_COUNT + 1;
}

void Game::generateInitialBoard()
{
    for (int y = 0; y < m_board.height(); y++)
    {
        for (int x = 0; x < m_board.width(); x++)
        {
            m_board(x,y) = getRandomObjType(); // 0 value means no item on this position
        }
    }
    
    ObjectsPositionSet matchesSet = getObjectsToRemove(false);
    
    if (matchesSet.size())
    {
        replaceWithUnique(matchesSet);
    }
    
    // even though probability is negligible
    if (!getPossibleMove(nullptr))
    {
        generateInitialBoard();
    }
    notify([](Observer* o){o->onInitBoard();});
}

void Game::replaceWithUnique(const ObjectsPositionSet &objectsIds)
{
    for (const auto &pos : objectsIds)
    {
        std::set<int> forbiddenIds;
        std::multimap<int, Point> neighbors = m_board.getNeighbors(pos);
       
        for (int i = 1; i <= C_OBJECTS_TYPES_COUNT; i++)
        {
            if (neighbors.find(i) == neighbors.end())
            {
                m_board(pos.x, pos.y) = i;
                break;
            }
        }
    }
}

ObjectsPositionSet Game::getObjectsToRemove(bool needCalcScore)
{
    auto getMatchesByPosition = [this](int x, int y, bool horizontal,  ObjectsPositionSet &matches, std::map<int, int> *sequencesMap)
    {
        int matchCount = 0;
        bool condition = (m_board(x,y) > 0);
        while (condition)
        {
            matchCount++;
            if (horizontal)
            {
                condition = (x + matchCount < m_board.width()) && (m_board(x + matchCount, y) == m_board(x,y));
            }
            else
            {
                condition = (y + matchCount < m_board.height()) && (m_board(x, y + matchCount) == m_board(x,y));
            }
        }
        if (matchCount >= C_MIN_MATCH_COUNT)
        {
            if (sequencesMap)
            {
                (*sequencesMap)[matchCount]++;
            }
            
            int curId = horizontal ? x : y;
            for (int i = curId; i < curId + matchCount; i++)
            {
                matches.insert((horizontal ? Point(i, y) : Point(x, i)));
            }
        }
    };
   
    // key - sequence length; value - number of sequences
    std::map<int, int> sequencesMap;
    
    ObjectsPositionSet horizontalObjects;
    ObjectsPositionSet verticalObjects;
    for (int y = 0; y < m_board.height(); y++)
    {
        for (int x = 0; x < m_board.width(); x++)
        {
            if (horizontalObjects.find(Point(x, y)) == horizontalObjects.end())
            {
                getMatchesByPosition(x, y, true, horizontalObjects, needCalcScore ? &sequencesMap : nullptr);
            }
            if (verticalObjects.find(Point(x, y)) == verticalObjects.end())
            {
                getMatchesByPosition(x, y, false, verticalObjects, needCalcScore ? &sequencesMap : nullptr);
            }
        }
    }
    
    // Increase score
    // 1. by sequences length
    int matchesCount = 0;
    for (auto pair : sequencesMap)
    {
        matchesCount += pair.second;
        m_score += C_SEQ_SCORE_MAP.at(pair.first) * pair.second;
    }
    // 2. by matches count
    if (matchesCount > 1)
    {
        auto it = C_MATCHES_SCORE_MAP.find(matchesCount);
        m_score += (it == C_MATCHES_SCORE_MAP.end() ? C_MATCHES_SCORE_MAP.rbegin()->second : it->second);
    }
    
    ObjectsPositionSet &result = horizontalObjects;
    
    result.insert(verticalObjects.begin(), verticalObjects.end());
    
    return result;
}

bool Game::getPossibleMove(ObjectsPositionList *moveContents)
{
    auto getMoveByTwoPoints = [this](const Point &firstPos, const Point &secondPos, ObjectsPositionList *moveContents)->bool
    {
        auto getMoveByThreePoints = [this](const Point &firstPos, const Point &secondPos, const Point &candidatePos, ObjectsPositionList *moveContents)->bool
        {
            bool result = false;
            if (m_board.isInside(candidatePos))
            {
                int typeId = m_board(firstPos);
                auto neighbors = m_board.getNeighbors(candidatePos);
                auto range = neighbors.equal_range(typeId);
                for (auto it = range.first; it != range.second; ++it)
                {
                    if (it->second != firstPos && it->second != secondPos)
                    {
                        if (moveContents)
                        {
                            moveContents->push_back(firstPos);
                            moveContents->push_back(secondPos);
                            moveContents->push_back(it->second);
                        }
                        result = true;
                        break;
                    }
                }
            }
            return result;
        };
        
        bool result = false;
        bool horizontal = (firstPos.y == secondPos.y);
        bool isNeighbors = (abs(firstPos.x-secondPos.x) + abs(firstPos.y-secondPos.y) == 1);

        if (isNeighbors)
        {
            Point beforeFirstPos(horizontal ? firstPos.x-1 : firstPos.x, horizontal ? firstPos.y : firstPos.y-1);
            result = getMoveByThreePoints(firstPos, secondPos, beforeFirstPos, moveContents);
            if (!result)
            {
                Point afterSecondPos(horizontal ? secondPos.x+1 : secondPos.x, horizontal ? secondPos.y : secondPos.y+1);
                result = getMoveByThreePoints(firstPos, secondPos, afterSecondPos, moveContents);
            }
        }
        else
        {
            Point middlePos(horizontal ? firstPos.x+1 : firstPos.x, horizontal ? firstPos.y : firstPos.y+1);
            result = getMoveByThreePoints(firstPos, secondPos, middlePos, moveContents);
        }
        
        return result;
    };
    
    bool result = false;
    for (int y = 0; y < m_board.height() - 1; y++)
    {
        for (int x = 0; x < m_board.width() - 1; x++)
        {
            Point curPoint(x,y);
            for (bool horizontal : {false, true})
            {
                for (int i = 1; i <= 2; i++)
                {
                    Point nextPoint(horizontal ? x+i : x, horizontal ? y : y+i);
                    if (m_board(curPoint) == m_board(nextPoint))
                    {
                        if (getMoveByTwoPoints(curPoint, nextPoint, moveContents))
                        {
                            return true;
                        }
                    }
                }

            }
        }
    }
    return result;
}

void Game::printObjects()
{
    for (int y = -m_board.height(); y < m_board.height(); y++)
    {
        for (int x = 0; x < m_board.width(); x++)
        {
            std::cout << m_board(x,y) << " ";
        }
        std::cout << '\n';
    }
    std::cout<<'\n';
}

void Game::doSwap(const Point &firstPos, const Point &secondPos, const SwapData::SwapState &state)
{
    m_selected.first = false;
    notify([firstPos, secondPos](Observer *o){o->onSwapObjects(firstPos, secondPos);});
    m_swap.State = state;
    m_swap.FirstPos = firstPos;
    m_swap.SecondPos = secondPos;
    int tempType = m_board(secondPos);
    m_board(secondPos) = m_board(firstPos);
    m_board(firstPos) = tempType;
    m_blockControls = true;
}

void Game::setSelected(const Point &point)
{
    bool needNotify = false;
    if (m_board.isInside(point))
    {
        if (m_selected.first)
        {
            if (Board::areNeighbors(m_selected.second, point))
            {
                // if already have one object selected and objects are neighbors - perform swap
                doSwap(m_selected.second, point, SwapData::SwapState::Direct);
            }
            else
            {
                m_selected.second = point;
                needNotify = true;
            }
        }
        else
        {
            m_selected.first = true;
            m_selected.second = point;
            needNotify = true;
        }
    }
    if (needNotify)
    {
        notify([point](Observer *o){o->onSelectObject(point);});
    }
}

void Game::onEndSwapping()
{
    assert(m_swap.State != SwapData::SwapState::None);
    m_blockControls = false;
    if (m_swap.State == SwapData::SwapState::Direct)
    {
        if (!removeObjects())
        {
            doSwap(m_swap.FirstPos, m_swap.SecondPos, SwapData::SwapState::Reverse);
        }
    }
}

// Calculate drop vectors and send them to Observers
// WARNING! dropList is order dependent: the bottom objects must be first
bool Game::removeObjects()
{
    ObjectsPositionSet objectsToRemove = getObjectsToRemove(true);
    if (objectsToRemove.size())
    {
        printObjects();
        
        
        m_blockControls = true;
        for (const Point &pos : objectsToRemove)
        {
            m_board(pos) = 0;
        }
        
        printObjects();
        ObjectsMovesList dropsList;
        for (int x = 0; x < m_board.width(); x++)
        {
            // get blank objects count in every column
            unsigned blankItemsCount = 0;
            for (int y = 0; y < m_board.height(); y++)
            {
                if (m_board(x,y) == 0)
                {
                    blankItemsCount++;
                }
            }
            
            // generate virtual objects that'll be dropped
            if (blankItemsCount)
            {
                for (unsigned i = -1; i >= -blankItemsCount; i--)
                {
                    m_board(x, i) = getRandomObjType();
                }
                
                // calc destination points for falling objects
                int dropHeight = 0;
                for (int y = m_board.height()-1; y >= -m_board.height() ; y--)
                {
                    if (m_board(x,y) == 0)
                    {
                        dropHeight++;
                    }
                    else if (dropHeight)
                    {
                        dropsList.emplace_back(Point(x,y), Point(x,y+dropHeight));
                    }
                }
            }
        }
        
        printObjects();
        
        notify([dropsList](Observer *o){o->onRemoveObjects(dropsList);});

        for (auto pair : dropsList)
        {
            m_board(pair.second) = m_board(pair.first);
            m_board(pair.first) = 0;
        }
        
        printObjects();
        
        // for debug!!!
        for (int x = 0; x < m_board.width() - 1; x++)
        {
            for (int y = 0; y < m_board.height() - 1; y++)
            {
                if (m_board(x,y) == 0)
                {
                    int i;
                    i = 0;
                }
                assert(m_board(x,y) != 0);
            }
        }
        
        return true;
            
    }
    else
    {
        return false;
    }
}

void Game::onEndRemoving()
{
    m_blockControls = false;
    
    removeObjects();
}