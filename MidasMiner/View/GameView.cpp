//
//  GameView.cpp
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "GameView.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "TextureManager.h"
#include "GameObject.h"
#include "Consts.h"
#include <cassert>

GameView::GameView()
{
}

void GameView::init()
{
    Game::getInstance()->registerObserver(this);
    // initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) >= 0)
    {
        // if succeeded create our window
        m_pWindow = SDL_CreateWindow("MidasMiner Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 755, 600, 0);
        // if the window creation succeeded create our renderer
        if(m_pWindow != 0)
        {
            std::cout << "window creation success\n";
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
            if (m_pRenderer)
            {
                std::cout << "renderer creation success\n";
                SDL_SetRenderDrawColor(m_pRenderer,
                                       255,255,255,255);
            }
            
            SDL_Rect rect;
            rect.x = C_BOARD_TOP_LEFT.x - 2;
            rect.y = C_BOARD_TOP_LEFT.y - 2;
            rect.w = 320;
            rect.h = 320;
            SDL_RenderSetClipRect(m_pRenderer, &rect);
            
            m_running = true;
        }
    }
    
    TextureManager::getInstance()->load("BackGround.jpg", "BG", m_pRenderer);
    TextureManager::getInstance()->load("Blue.png", "Blue", m_pRenderer);
    TextureManager::getInstance()->load("Green.png", "Green", m_pRenderer);
    TextureManager::getInstance()->load("Purple.png", "Purple", m_pRenderer);
    TextureManager::getInstance()->load("Red.png", "Red", m_pRenderer);
    TextureManager::getInstance()->load("Yellow.png", "Yellow", m_pRenderer);
    
}

void GameView::render()
{
    SDL_RenderClear(m_pRenderer); // clear the renderer to the draw color
    TextureManager::getInstance()->draw("BG", 0, 0, m_pRenderer);
    for (const auto &pair : m_objects)
    {
        pair.second->draw(m_pRenderer);
    }
    SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void GameView::update()
{
    bool hasMotion = false;
    for (const auto &pair : m_objects)
    {
        if (pair.second->update())
        {
            hasMotion = true;
        }
    }
    
    if (m_swapInProgress && !hasMotion)
    {
        m_swapInProgress = false;
        Game::getInstance()->onEndSwapping();
    }
}

void GameView::clean()
{
    std::cout << "cleaning game\n";
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}

void GameView::onInitBoard()
{
    m_objects.clear();
    const Board &board = Game::getInstance()->getBoard();
    
    int y = C_BOARD_TOP_LEFT.y;
    for (int i = 0; i < board.height(); i++)
    {
        int x = C_BOARD_TOP_LEFT.x;
        for (int j = 0; j < board.height(); j++)
        {
            m_objects[Point(j,i)].reset(new GameObject());
            m_objects[Point(j,i)]->load(Point(x, y), C_TYPES_MAP.at(board(j,i)));
            x += C_OBJECT_SIZE;
        }
        y += C_OBJECT_SIZE;
    }
}

void GameView::clearSelection()
{
    if (m_selected.first)
    {
        auto it = m_objects.find(m_selected.second);
        if (it != m_objects.end())
        {
            Point oldPos = it->second->getPos();
            it->second.reset(new GameObject());
            const Board &board = Game::getInstance()->getBoard();
            it->second->load(oldPos, C_TYPES_MAP.at(board(it->first)));
            m_selected.first = false;
        }
    }
}

void GameView::onSelectObject(const Point &pos)
{
    clearSelection();
    auto it = m_objects.find(pos);
    if (it != m_objects.end())
    {
        it->second.reset(new SelectedDecorator(std::move(it->second)));
        m_selected.first = true;
        m_selected.second = pos;
    }
}

void GameView::onSwapObjects(const Point &firstPos, const Point &secondPos)
{
    clearSelection();
    auto firstIt = m_objects.find(firstPos);
    auto secondIt = m_objects.find(secondPos);
    assert(firstIt != m_objects.end());
    assert(secondIt != m_objects.end());
    
    firstIt->second->moveTo(secondIt->second->getPos());
    secondIt->second->moveTo(firstIt->second->getPos());
    
    firstIt->second.swap(secondIt->second);
    
    m_swapInProgress = true;
}

void GameView::onRemoveObjects()
{
    const Board &board = Game::getInstance()->getBoard();
    for (auto it = m_objects.begin(); it != m_objects.end();)
    {
        if (board(it->first) == 0)
        {
            it = m_objects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool GameView::pixelPosToBoardPos(const Point &pixelPos, Point &boardPos)
{
    bool result = false;
    for (const auto &pair : m_objects)
    {
        if (pair.second->isIntersect(pixelPos))
        {
            boardPos = pair.first;
            result = true;
            break;
        }
    }
    return result;
}


