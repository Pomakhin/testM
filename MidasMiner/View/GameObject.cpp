//
//  GameObject.cpp
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "GameObject.h"
#include "TextureManager.h"
#include "Consts.h"

#include <SDL2/SDL.h>

void GameObject::draw(SDL_Renderer* renderer)
{
    TextureManager::getInstance()->draw(m_textureId, m_pos.x, m_pos.y, renderer, m_alpha);
}

bool GameObject::update()
{
//    if (m_alphaIncrease)
//    {
//        if (m_alpha < 255)
//        {
//            m_alpha+= 5;
//        }
//        else
//        {
//            m_alphaIncrease = false;
//        }
//    }
//    else
//    {
//        if (m_alpha > 100)
//        {
//            m_alpha-= 5;
//        }
//        else
//        {
//            m_alphaIncrease = true;
//        }
//    }
    
//    m_y+=m_yVelocity;
//    m_yVelocity += C_GRAVITATIONAL_ACCELERATION;

    bool result = false;
    if (m_xVelocity)
    {
        if (m_acceleration > 0.0f)
        {
            m_xVelocity += Utils::sign(m_xVelocity) * m_acceleration;
        }
        if (abs(m_pos.x + m_xVelocity - m_destination.x) < abs(m_xVelocity))
        {
            m_pos.x = m_destination.x;
            m_xVelocity = 0.0f;
        }
        else
        {
            m_pos.x += m_xVelocity;
            result = true;
        }
    }
    if (m_yVelocity)
    {
        if (m_acceleration > 0.0f)
        {
            m_yVelocity += Utils::sign(m_yVelocity) * m_acceleration;
        }
        if (abs(m_pos.y + m_yVelocity - m_destination.y) < abs(m_yVelocity))
        {
            m_pos.y = m_destination.y;
            m_yVelocity = 0.0f;
        }
        else
        {
            m_pos.y += m_yVelocity;
            result = true;
        }
    }
    return result;
}

bool GameObject::isIntersect(const Point &point)
{
    return
        (point.x > m_pos.x &&
        point.x < m_pos.x + C_OBJECT_SIZE &&
        point.y > m_pos.y &&
        point.y < m_pos.y + C_OBJECT_SIZE);
}

void GameObject::load(const Point &pos, const std::string &textureId)
{
    m_pos = pos;
    m_textureId = textureId;
}

void GameObject::moveTo(const Point &toPos, const float &acceleration)
{
    if (m_pos != toPos)
    {
        m_destination = toPos;
        if (m_destination.x < m_pos.x)
        {
            m_xVelocity = -C_BASE_SPEED;
        }
        else if (m_destination.x > m_pos.x)
        {
            m_xVelocity = C_BASE_SPEED;
        }
        if (m_destination.y < m_pos.y)
        {
            m_yVelocity = -C_BASE_SPEED;
        }
        else if (m_destination.y > m_pos.y)
        {
            m_yVelocity = C_BASE_SPEED;
        }
        m_acceleration = acceleration;
    }
}

void GameObjectDecorator::draw(SDL_Renderer* renderer)
{
    m_gameObject->draw(renderer);
}
bool GameObjectDecorator::update()
{
    return m_gameObject->update();
}
void GameObjectDecorator::load(const Point &pos, const std::string &textureId)
{
    m_gameObject->load(pos, textureId);
}
bool GameObjectDecorator::isIntersect(const Point &point)
{
    return m_gameObject->isIntersect(point);
}
void GameObjectDecorator::moveTo(const Point &toPos, const float &acceleration)
{
    return m_gameObject->moveTo(toPos, acceleration);
}
Point GameObjectDecorator::getPos()
{
    return m_gameObject->getPos();
}
void SelectedDecorator::draw(SDL_Renderer *renderer)
{
    GameObjectDecorator::draw(renderer);
    SDL_Rect rectToDraw = {getPos().x - 2 ,getPos().y - 2,C_OBJECT_SIZE - 2,C_OBJECT_SIZE - 2};

    //Set Color of Rect with SDL_SetRenderDrawColor if needed

    SDL_RenderDrawRect(renderer, &rectToDraw);
}