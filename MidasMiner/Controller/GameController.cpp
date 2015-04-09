//
//  GameController.cpp
//  MidasMiner
//
//  Created by Mikhail ; on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "GameController.h"
#include "Game.h"
#include "GameView.h"
#include "Consts.h"

void GameController::init()
{
    m_running = GameView::getInstance()->init();
    
    m_beginOfGame = SDL_GetTicks();
    newGame();
    while (m_running)
    {
        m_frameStart = SDL_GetTicks();
        
        handleEvents();

        GameView::getInstance()->update();
        m_timer.update();
        GameView::getInstance()->render();
        
        m_frameTime = SDL_GetTicks() - m_frameStart;
        
        if(m_frameTime < C_DELAY_TIME)
        {
            SDL_Delay(C_DELAY_TIME - m_frameTime);
        }
    }
    GameView::getInstance()->clean();
}

void GameController::newGame()
{
    m_timer.init(static_cast<double>(SDL_GetTicks()) / 1000.0f + C_GAME_TIME);
    m_timer.setOnTimerFunction([](int t){GameView::getInstance()->updateGameTime(t);});
    m_timer.setOnCompleteTimerFunction([]{GameView::getInstance()->gameOver();});
    Game::getInstance()->init();    
}

void GameController::handleEvents()
{
    SDL_Event event;
    if(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
            {
                if (!Game::getInstance()->getBlockControls() && m_timer.isActive())
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
                if (!Game::getInstance()->getBlockControls() && m_timer.isActive())
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
            case SDL_KEYUP:
            {
                if (event.key.keysym.sym == SDLK_r)
                {
                    newGame();
                }
            }
                break;
            default:
                break;
        }
    }
}