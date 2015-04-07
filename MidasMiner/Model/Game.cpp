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

Game::Game():
    m_board(C_BOARD_WIDTH, C_BOARD_HEIGHT)
{
    
}

void Game::init()
{
    generateInitialBoard();    
}

void Game::generateInitialBoard()
{
    srand(static_cast<unsigned>(time(nullptr)));
    for (int y = 0; y < m_board.height(); y++)
    {
        for (int x = 0; x < m_board.width(); x++)
        {
            m_board(x,y) = rand() % C_OBJECTS_TYPES_COUNT + 1; // 0 value means no item on this position
        }
    }
    
    printObjects();
    
    ObjectsPositionSet l_matchesSet = getObjectsToDelete(true);
    
    if (l_matchesSet.size())
    {
        replaceWithUnique(l_matchesSet);
    }
    
    printObjects();
    
    // even though probability is negligible
    if (!getPossibleMove(nullptr))
    {
        generateInitialBoard();
    }
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

Game::ObjectsPositionSet Game::getObjectsToDelete(bool needCalcScore)
{
    auto getMatchesByPosition = [this](int x, int y, bool horizontal,  ObjectsPositionSet &matches, std::map<int, int> *sequencesMap)
    {
        int matchCount = 0;
        bool condition = true;
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
    
    ObjectsPositionSet &result = horizontalObjects;
    
    result.insert(verticalObjects.begin(), verticalObjects.end());
    
    return result;
}

bool Game::getPossibleMove(Game::ObjectsPositionList *moveContents)
{
    auto getMoveByTwoPoints = [this](const Point &firstPos, const Point &secondPos, Game::ObjectsPositionList *moveContents)->bool
    {
        auto getMoveByThreePoints = [this](const Point &firstPos, const Point &secondPos, const Point &candidatePos, Game::ObjectsPositionList *moveContents)->bool
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
    for (int y = 0; y < m_board.height(); y++)
    {
        for (int x = 0; x < m_board.width(); x++)
        {
            std::cout << m_board(x,y) << " ";
        }
        std::cout << '\n';
    }
    std::cout<<'\n';
}

