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

void GameController::init()
{
    Game::getInstance()->init();
    GameView::getInstance()->init();
    
    while (GameView::getInstance()->isRunning())
    {
        GameView::getInstance()->handleEvents();
        GameView::getInstance()->update();
        GameView::getInstance()->render();
    }
    GameView::getInstance()->clean();
}