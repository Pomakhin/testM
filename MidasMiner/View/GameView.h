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
#include "Game.h"
#include <SDL2/SDL.h>

class BaseGameObject;

class GameView : public Singleton<GameView>, Observer
{
    friend class Singleton<GameView>;
private:
    bool m_running = false;
    bool m_swapInProgress = false;
    bool m_removeInProgress = false;
    SDL_Window* m_pWindow = 0;
    SDL_Renderer* m_pRenderer = 0;
    SDL_Rect m_clipRect;
    std::map<Point, std::unique_ptr<BaseGameObject> > m_objects;
    std::pair<bool, Point> m_selected;
    
    void clearSelection();
protected:
    GameView();
public:
    void onInitBoard() override;
    void onSelectObject(const Point &pos) override;
    void onSwapObjects(const Point &firstPos, const Point &secondPos) override;
    void onRemoveObjects(const ObjectsMovesList &dropsLis) override;
    
    void init();
    void render();
    void update();
    void clean();
    bool isRunning() {return m_running;}
    bool pixelPosToBoardPos(const Point &pixelPos, Point &boardPos);
    Point boardPosToPixelPos(const Point &boardPos);
};

#endif /* defined(__MidasMiner__GameView__) */
