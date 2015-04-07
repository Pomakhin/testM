//
//  GameObject.cpp
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "GameObject.h"
#include "TextureManager.h"

void GameObject::draw(SDL_Renderer* renderer)
{
    TextureManager::getInstance()->draw(m_textureId, m_x, m_y, m_width, m_height, renderer);
}

void GameObject::update()
{
    
}

void GameObject::load(int x, int y, const std::string &textureId)
{
    m_x = x;
    m_y = y;
    m_textureId = textureId;
}