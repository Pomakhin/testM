//
//  GameView.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__GameView__
#define __MidasMiner__GameView__

#include <stdio.h>
#include "GenericSingleton.h"
#include <list>

class SDL_Window;
class SDL_Renderer;
class GameObject;

class GameView : public Singleton<GameView>
{
    friend class Singleton<GameView>;
private:
    bool m_running = false;
    SDL_Window* m_pWindow = 0;
    SDL_Renderer* m_pRenderer = 0;
    std::list<GameObject *> m_objects;
protected:
    GameView(){};
public:
    void init();
    void render();
    void update();
    void handleEvents();
    void clean();
    bool isRunning() {return m_running;}
};

#endif /* defined(__MidasMiner__GameView__) */
