//
//  GameController.cpp
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "GameController.h"
#include "Game.h"
#include "GameView.h"
#include <SDL2/SDL.h>

void GameController::init()
{
    GameView::getInstance()->init();    
    Game::getInstance()->init();
    
    while (GameView::getInstance()->isRunning())
    {
        handleEvents();
        GameView::getInstance()->update();
        GameView::getInstance()->render();
    }
    GameView::getInstance()->clean();
}

void GameController::handleEvents()
{
    SDL_Event event;
    if(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                //m_running = false; TODO::
                break;
            case SDL_MOUSEBUTTONDOWN:
            {
                if (!Game::getInstance()->getBlockControls())
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        Point boardPos;
                        if (GameView::getInstance()->pixelPosToBoardPos(Point(event.button.x, event.button.y), boardPos))
                        {
                            Game::getInstance()->setSelected(boardPos);
                        }
                    }
                }
            }
                break;
            case SDL_MOUSEBUTTONUP:
            {
                if (!Game::getInstance()->getBlockControls())
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        Point boardPos;
                        if (GameView::getInstance()->pixelPosToBoardPos(Point(event.button.x, event.button.y), boardPos))
                        {
                            Game::getInstance()->setSelected(boardPos);
                        }
                    }
                }
            }
                break;
            default:
                break;
        }
    }
}