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
#include "Game.h"

void GameView::init()
{
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
            m_running = true;
        }
    }
    
    TextureManager::getInstance()->load("BackGround.jpg", "BG", m_pRenderer);
    TextureManager::getInstance()->load("Blue.png", "Blue", m_pRenderer);
    TextureManager::getInstance()->load("Green.png", "Green", m_pRenderer);
    TextureManager::getInstance()->load("Purple.png", "Purple", m_pRenderer);
    TextureManager::getInstance()->load("Red.png", "Red", m_pRenderer);
    TextureManager::getInstance()->load("Yellow.png", "Yellow", m_pRenderer);
    
    const Board &board = Game::getInstance()->getBoard();
    
    int y = C_BOARD_TOP_LEFT.y;
    for (int i = 0; i < board.height(); i++)
    {
        int x = C_BOARD_TOP_LEFT.x;
        for (int j = 0; j < board.height(); j++)
        {
            GameObject *obj = new GameObject();
            obj->load(x, y, C_TYPES_MAP.at(board(j,i)));
            m_objects.push_back(obj);
            x += C_OBJECT_SIZE;
        }
        y += C_OBJECT_SIZE;
    }
}


void GameView::render()
{
    SDL_RenderClear(m_pRenderer); // clear the renderer to the draw color
    TextureManager::getInstance()->draw("BG", 0, 0, 755, 600, m_pRenderer);
    for (auto obj : m_objects)
    {
        obj->draw(m_pRenderer);
    }
    SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void GameView::update()
{
    for (auto obj : m_objects)
    {
        obj->update();
    }
}

void GameView::clean()
{
    std::cout << "cleaning game\n";
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}

void GameView::handleEvents()
{
    SDL_Event event;
    if(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_running = false;
                break;
            default:
                break;
        }
    }
}
