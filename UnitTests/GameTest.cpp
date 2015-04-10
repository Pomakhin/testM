//
//  GameTest.cpp
//  MidasMiner
//
//  Created by Михаил Помахин on 10.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include <stdio.h>

#include "Utils.h"
#include "gtest/gtest.h"
#include "Game.h"
#include "Consts.h"
#include <vector>

void setBoardWithVec(const std::vector<std::vector<int> > vec, Board &board)
{
    for (int y = 0; y < board.height(); y++)
    {
        for (int x = 0; x < board.width(); x++)
        {
            board(x,y) = vec[y][x];
        }
    }
}

void initTestGame()
{
    Board board(3,3);
    std::vector<std::vector<int> > v =
    {
        {1, 2, 1},
        {2, 3, 2},
        {3, 2, 3}
    };
    setBoardWithVec(v, board);
    Game::getInstance()->init();
    Game::getInstance()->setTestBoard(board);
}

TEST (Board, getNeighbors)
{
    Board board(4, 4);

    for (int y = 0; y < board.height(); y++)
    {
        for (int x = 0; x < board.width(); x++)
        {
            board(x,y) = 1;
        }
    }

    auto res = board.getNeighbors(Point(0, 0));
    EXPECT_EQ(res.size(), 2);
    EXPECT_EQ(res.count(1), 2);
}

TEST(GameTest, PossibleMove)
{
    initTestGame();
    ObjectsPositionList moveContents;
    EXPECT_TRUE(Game::getInstance()->getPossibleMove(&moveContents));
    EXPECT_EQ(3, moveContents.size());
}

TEST(GameTest, NoPossibleMove)
{
    Board board(3,3);
    std::vector<std::vector<int> > v =
    {
        {2, 3, 1},
        {2, 1, 2},
        {3, 3, 2}
    };
    setBoardWithVec(v, board);
    Game::getInstance()->init();
    Game::getInstance()->setTestBoard(board);
    EXPECT_FALSE(Game::getInstance()->getPossibleMove(nullptr));
}

TEST (GameTest, SwapAndReverse)
{
    initTestGame();
    Game::getInstance()->setSelected(Point(0,0));
    Game::getInstance()->setSelected(Point(0,1));
    
    EXPECT_EQ(2, Game::getInstance()->getBoard()(0,0));
    EXPECT_EQ(1, Game::getInstance()->getBoard()(0,1));
    
    Game::getInstance()->onEndSwapping();

    EXPECT_EQ(1, Game::getInstance()->getBoard()(0,0));
    EXPECT_EQ(2, Game::getInstance()->getBoard()(0,1));
}


TEST (GameTest, SwapAndRemove)
{
    initTestGame();
    Game::getInstance()->setSelected(Point(1,1));
    Game::getInstance()->setSelected(Point(1,2));
    
    EXPECT_EQ(2, Game::getInstance()->getBoard()(1,1));
    EXPECT_EQ(3, Game::getInstance()->getBoard()(1,2));
    
    Game::getInstance()->onEndSwapping();
    
    for (int y = 0; y < Game::getInstance()->getBoard().height(); y++)
    {
        for (int x = 0; x < Game::getInstance()->getBoard().width(); x++)
        {
            EXPECT_TRUE(Game::getInstance()->getBoard()(x,y) != 0);
        }
    }
}

TEST (GameTest, IncreaseScore)
{
    initTestGame();
    Game::getInstance()->setSelected(Point(1,1));
    Game::getInstance()->setSelected(Point(1,2));
    
    Game::getInstance()->onEndSwapping();
    
    EXPECT_EQ(Game::getInstance()->getScore(), 150);
    
}