//
//  GameController.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__GameController__
#define __MidasMiner__GameController__

#include <stdio.h>
#include "GenericSingleton.h"
#include <SDL2/SDL.h>
#include "Utils.h"

class GameController : public Singleton<GameController>
{
    friend class Singleton<GameController>;
private:
    Timer m_timer;
    
    bool m_running = false;
    Uint32 m_frameStart = 0;
    Uint32 m_frameTime = 0;
    Uint32 m_beginOfGame = 0;
    
    void handleEvents();
    void newGame();
protected:
    GameController(){};
public:
    void init();
};

#endif /* defined(__MidasMiner__GameController__) */
