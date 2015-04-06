//
//  Game.cpp
//  MidasMiner
//
//  Created by Михаил Помахин on 04.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "Game.h"

#include <iostream>

Game *Game::m_instance = nullptr;

static const long C_BOARD_WIDTH = 8;
static const long C_BOARD_HEIGHT = 8;
static const long C_MIN_MATCH_COUNT = 3;
static const long C_OBJECTS_TYPES_COUNT = 6;

Board::Board(long width, long height):
    m_width(width),
    m_height(height)
{
    m_objects.resize(m_height);
    for (auto &row : m_objects)
    {
        row.resize(m_width);
    }
}
long& Board::operator() (long x, long y)
{
    if (y >= m_height ||
        x >= m_width)
    {
        return m_outOfBoardValue;
    }
    return m_objects[y][x];
}
long Board::operator() (long x, long y) const
{
    if (y >= m_height ||
        x >= m_width)
    {
        return long{};
    }
    return m_objects[y][x];
}
long& Board::operator() (const Point &pos)
{
    return operator()(pos.x, pos.y);
}
long  Board::operator() (const Point &pos) const
{
    return operator()(pos.x, pos.y);
}
long Board::width()
{
    return m_width;
}
long Board::height()
{
    return m_height;
}
std::multimap<long, Point> Board::getNeighbors(const Point &pos)
{
    std::multimap<long, Point> result;
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

Game *Game::getInstanse()
{
    if (!m_instance)
    {
        m_instance = new Game();
    }
    return m_instance;
}

Game::Game():
    m_board(C_BOARD_WIDTH, C_BOARD_HEIGHT)
{
    generateInitialBoard();
}

void Game::generateInitialBoard()
{
    srand(static_cast<unsigned>(time(nullptr)));
    for (long y = 0; y < m_board.height(); y++)
    {
        for (long x = 0; x < m_board.width(); x++)
        {
            m_board(x,y) = rand() % C_OBJECTS_TYPES_COUNT + 1; // 0 value means no item on this position
        }
    }
    
    ObjectsPositionSet l_matchesSet = getObjectsToDelete(true);
    
    if (l_matchesSet.size())
    {
        replaceWithUnique(l_matchesSet);
    }
    ObjectsPositionList moveContents;
    if (!getPossibleMove(&moveContents))
    {
        int i;
        i = 0;
    }
    int i;
    i = 0;
}

void Game::replaceWithUnique(const ObjectsPositionSet &objectsIds)
{
    for (const auto &pos : objectsIds)
    {
        std::set<long> forbiddenIds;
        std::multimap<long, Point> neighbors = m_board.getNeighbors(pos);
       
        for (long i = 1; i <= C_OBJECTS_TYPES_COUNT; i++)
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
    auto getMatchesByPosition = [this](long x, long y, bool horizontal,  ObjectsPositionSet &matches, std::map<long, long> *sequencesMap)
    {
        long matchCount = 0;
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
            
            long curId = horizontal ? x : y;
            for (long i = curId; i < curId + matchCount; i++)
            {
                matches.insert((horizontal ? Point(i, y) : Point(x, i)));
            }
        }
    };
   
    // key - sequence length; value - number of sequences
    std::map<long, long> sequencesMap;
    
    ObjectsPositionSet horizontalObjects;
    ObjectsPositionSet verticalObjects;
    for (long y = 0; y < m_board.height(); y++)
    {
        for (long x = 0; x < m_board.width(); x++)
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
    auto getPossibleMoveForNeighbors = [this](Game::ObjectsPositionList *moveContents)->bool
    {
        auto getMoveForNeighbor = [this](const Point &firstPos, const Point &secondPos, Game::ObjectsPositionList *moveContents)->bool
        {
            bool result = false;
            long typeId = m_board(firstPos);
            bool horizontal = (firstPos.y == secondPos.y);

            Point beforeFirstPos;
            beforeFirstPos.x = horizontal ? firstPos.x-1 : firstPos.x;
            beforeFirstPos.y = horizontal ? firstPos.y : firstPos.y -1;
            if (m_board.isInside(beforeFirstPos))
            {
                auto neighbors = m_board.getNeighbors(beforeFirstPos);
                if (neighbors.count(typeId) > 1)
                {
                    if (moveContents)
                    {
                        moveContents->push_back(firstPos);
                        moveContents->push_back(secondPos);
                        auto range = neighbors.equal_range(typeId);
                        for (auto it = range.first; it != range.second; ++it)
                        {
                            if (it->second != firstPos)
                            {
                                moveContents->push_back(it->second);
                                break;
                            }
                        }
                    }
                    result = true;
                }
            }
            
            Point afterSecondPos;
            afterSecondPos.x = horizontal ? secondPos.x+1 : secondPos.x;
            afterSecondPos.y = horizontal ? secondPos.y : secondPos.y +1;
            if (m_board.isInside(afterSecondPos))
            {
                auto neighbors = m_board.getNeighbors(afterSecondPos);
                if (neighbors.count(typeId) > 1)
                {
                    if (moveContents)
                    {
                        moveContents->push_back(firstPos);
                        moveContents->push_back(secondPos);
                        auto range = neighbors.equal_range(typeId);
                        for (auto it = range.first; it != range.second; ++it)
                        {
                            if (it->second != secondPos)
                            {
                                moveContents->push_back(it->second);
                                break;
                            }
                        }
                    }
                    result = true;
                }
            }
            
            return result;
        };
        
//        
        bool result = false;
        for (long y = 0; y < m_board.height() - 1; y++)
        {
            for (long x = 0; x < m_board.width() - 1; x++)
            {
                if (m_board(x,y) == m_board(x+1,y))
                {
                    result = getMoveForNeighbor(Point(x,y), Point(x+1,y), moveContents);
                    if (result)
                    {
                        return result;
                    }
                }
                
                if (m_board(x,y) == m_board(x,y+1))
                {
                    result = getMoveForNeighbor(Point(x,y), Point(x,y+1), moveContents);
                    if (result)
                    {
                        return result;
                    }
                }
            }
        }
        return result;
    };
    
    bool result = getPossibleMoveForNeighbors(moveContents);
    if (!result)
    {
        //result = getPossibleMoveForDivided(moveContents);
    }
    return result;
}

void Game::printObjects()
{
    for (long y = 0; y < m_board.height(); y++)
    {
        for (long x = 0; x < m_board.width(); x++)
        {
            std::cout << m_board(x,y) << " ";
        }
        std::cout << '\n';
    }
}

