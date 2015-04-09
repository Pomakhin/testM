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
#include "TextLabel.h"
#include "GameObject.h"
#include "SDLCommon.h"

class GameView : public Singleton<GameView>, Observer
{
    friend class Singleton<GameView>;
private:
    bool m_swapInProgress = false;
    bool m_removeInProgress = false;
    std::unique_ptr<SDL_Window, SDLDeleter> m_window = 0;
    std::unique_ptr<SDL_Renderer, SDLDeleter> m_renderer = 0;
    SDL_Rect m_clipRect;
    std::map<Point, std::unique_ptr<BaseGameObject> > m_objects;
    std::pair<bool, Point> m_selected;
    std::unique_ptr<TextLabel> m_timeLabel;
    std::unique_ptr<TextLabel> m_gameOverLabel;
    std::unique_ptr<TextLabel> m_replayLabel;
    std::unique_ptr<TextLabel> m_scoreLabel;
    
    ObjectsPositionList m_hintList;
    
    void clearSelection();
    void removeHint();
protected:
    GameView();
public:
    ~GameView(){}
    void onInitBoard() override;
    void onSelectObject(const Point &pos) override;
    void onSwapObjects(const Point &firstPos, const Point &secondPos) override;
    void onRemoveObjects(const ObjectsMovesList &dropsLis) override;
    
    bool init();
    void render();
    void update();
    void clean();
    bool pixelPosToBoardPos(const Point &pixelPos, Point &boardPos);
    Point boardPosToPixelPos(const Point &boardPos);
    
    void updateGameTime(int t);
    void gameOver();
    void setHint();
};

#endif /* defined(__MidasMiner__GameView__) */
