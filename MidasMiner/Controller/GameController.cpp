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

GameController::~GameController()
{
    GameView::destroyInstance();
    Game::destroyInstance();
}

void GameController::init()
{
    m_running = GameView::getInstance()->init();
    newGame();
    while (m_running)
    {
        m_frameStart = SDL_GetTicks();
        
        handleEvents();

        GameView::getInstance()->update();
        m_timer.update();
        if (m_timer.isActive())
        {
            m_hintTimer.update();
        }
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
    initHint();
}

void GameController::initHint()
{
    m_hintTimer.init(static_cast<double>(SDL_GetTicks()) / 1000.0f + C_HINT_TIME);
    m_hintTimer.setOnCompleteTimerFunction([]{GameView::getInstance()->setHint();});
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
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    selectObject(Point(event.button.x, event.button.y));
                }
            }
                break;
            case SDL_MOUSEBUTTONUP:
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    selectObject(Point(event.button.x, event.button.y));
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

void GameController::selectObject(const Point &pos)
{
    if (!Game::getInstance()->getBlockControls() && m_timer.isActive())
    {
        Point boardPos;
        if (GameView::getInstance()->pixelPosToBoardPos(pos, boardPos))
        {
            Game::getInstance()->setSelected(boardPos);
            // reset hint timer after every user select
            initHint();
        }
    }
}