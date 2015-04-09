//
//  GameView.cpp
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "GameView.h"
#include <iostream>
#include "TextureManager.h"
#include "GameObject.h"
#include "Consts.h"
#include <cassert>
#include <SDL2_ttf/SDL_ttf.h>

GameView::GameView()
{
}

bool GameView::init()
{
    bool result = true;
    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
    {
        Game::getInstance()->registerObserver(this);
        m_window.reset(SDL_CreateWindow("MidasMiner Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 755, 600, 0));
        if(m_window != 0)
        {
            std::cout << "window creation success\n";
            m_renderer.reset(SDL_CreateRenderer(m_window.get(), -1, 0));
            if (m_renderer)
            {
                std::cout << "renderer creation success\n";
                SDL_SetRenderDrawColor(m_renderer.get(),
                                       255,255,255,255);
            }
            else
            {
                result = false;
            }
            
            if (result)
            {
                m_clipRect.x = C_BOARD_TOP_LEFT.x - 2;
                m_clipRect.y = C_BOARD_TOP_LEFT.y - 2;
                m_clipRect.w = 320;
                m_clipRect.h = 320;
                
                TextureManager::getInstance()->load("BackGround.jpg", "BG", m_renderer.get());
                TextureManager::getInstance()->load("Blue.png", "Blue", m_renderer.get());
                TextureManager::getInstance()->load("Green.png", "Green", m_renderer.get());
                TextureManager::getInstance()->load("Purple.png", "Purple", m_renderer.get());
                TextureManager::getInstance()->load("Red.png", "Red", m_renderer.get());
                TextureManager::getInstance()->load("Yellow.png", "Yellow", m_renderer.get());

                if (TTF_Init() != 0)
                {
                    std::cout << "Cannot init TTF\n";
                    result = false;
                }
                else
                {
                    m_timeLabel.reset(new TextLabel(m_renderer.get(), "Arial.ttf", Point(95, 440), 50));
                    //m_gameOverLabel.reset(new TextLabel(m_renderer.get(), "Arial.ttf", Point(50, 215), 30));
                    m_gameOverLabel.reset(new TextLabel(m_renderer.get(), "Arial.ttf", Point(400, 50), 30));
                    m_replayLabel.reset(new TextLabel(m_renderer.get(), "Arial.ttf", Point(10, 10), 25));
                    m_scoreLabel.reset(new TextLabel(m_renderer.get(), "Arial.ttf", Point(420, 10), 25));
                    
                    m_replayLabel->setText("Press R to replay");
                    m_scoreLabel->setText("Score: 0");
                }
            }
        }
    }
    else
    {
        result = false;
    }
    return result;
}

void GameView::render()
{
    SDL_RenderClear(m_renderer.get()); // clear the renderer to the draw color
    TextureManager::getInstance()->draw("BG", 0, 0, m_renderer.get());
    
    // render game objects in a clip rect to implement falling objects
    SDL_RenderSetClipRect(m_renderer.get(), &m_clipRect);
    for (const auto &pair : m_objects)
    {
        pair.second->draw(m_renderer.get());
    }
    SDL_RenderSetClipRect(m_renderer.get(), nullptr);
    
    if (m_timeLabel)
    {
        m_timeLabel->draw();
    }
    if (m_gameOverLabel)
    {
        m_gameOverLabel->draw();
    }
    if (m_replayLabel)
    {
        m_replayLabel->draw();
    }
    if (m_scoreLabel)
    {
        m_scoreLabel->draw();
    }
    
    SDL_RenderPresent(m_renderer.get()); // draw to the screen
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
    else if (m_removeInProgress && !hasMotion)
    {
        m_removeInProgress = false;
        Game::getInstance()->onEndRemoving();
    }
}

void GameView::clean()
{
    SDL_Quit();
}

void GameView::onInitBoard()
{
    if (m_gameOverLabel)
    {
        m_gameOverLabel->setText("");
    }
    if (m_scoreLabel)
    {
        m_scoreLabel->setText("Score: 0");
    }
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
    removeHint();
    auto firstIt = m_objects.find(firstPos);
    auto secondIt = m_objects.find(secondPos);
    assert(firstIt != m_objects.end());
    assert(secondIt != m_objects.end());
    
    firstIt->second->moveTo(secondIt->second->getPos());
    secondIt->second->moveTo(firstIt->second->getPos());
    
    firstIt->second.swap(secondIt->second);
    
    m_swapInProgress = true;
}

void GameView::onRemoveObjects(const ObjectsMovesList &dropsLis)
{
    // remove objects
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
    
    // drop objects
    for (const auto &pair : dropsLis)
    {
        auto it = m_objects.find(pair.first);
        if (it == m_objects.end())
        {
            // drop objects from virtual board
            m_objects[pair.second].reset(new GameObject());
            m_objects[pair.second]->load(boardPosToPixelPos(pair.first), C_TYPES_MAP.at(board(pair.first)));
            m_objects[pair.second]->moveTo(boardPosToPixelPos(pair.second), C_GRAVITATIONAL_ACCELERATION);
        }
        else
        {
            // drop objects from real board
            it->second->moveTo(boardPosToPixelPos(pair.second), C_GRAVITATIONAL_ACCELERATION);
            m_objects[pair.second] = std::move(it->second);
        }
    }
    
    if (dropsLis.size())
    {
        m_removeInProgress = true;
    }
    
    if (m_scoreLabel)
    {
        m_scoreLabel->setText("Score: " + std::to_string(Game::getInstance()->getScore()));
    }
}

Point GameView::boardPosToPixelPos(const Point &boardPos)
{
    Point result;
    result.x = C_BOARD_TOP_LEFT.x + boardPos.x*C_OBJECT_SIZE;
    result.y = C_BOARD_TOP_LEFT.y + boardPos.y*C_OBJECT_SIZE;
    return result;
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

void GameView::updateGameTime(int t)
{
    if (m_timeLabel)
    {
        m_timeLabel->setText(std::to_string(t));
    }
}

void GameView::gameOver()
{
    removeHint();
    m_timeLabel->setText("");
    m_gameOverLabel->setText("GAME OVER");
}

void GameView::setHint()
{
    removeHint();
    Game::getInstance()->getPossibleMove(&m_hintList);
    for (auto pos : m_hintList)
    {
        auto it = m_objects.find(pos);
        if (it != m_objects.end())
        {
            it->second.reset(new HintDecorator(std::move(it->second)));
        }
    }
}

void GameView::removeHint()
{
    for (auto pos : m_hintList)
    {
        m_objects[pos].reset(new GameObject());
        const Board &board = Game::getInstance()->getBoard();
        m_objects[pos]->load(boardPosToPixelPos(pos), C_TYPES_MAP.at(board(pos)));
    }
    m_hintList.clear();
}


