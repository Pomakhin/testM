//
//  TextLabel.cpp
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 09.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "TextLabel.h"
#include <SDL2_ttf/SDL_ttf.h>
#include <iostream>

TextLabel::TextLabel(SDL_Renderer* renderer, const std::string &font, const Point &pos, int size):
    m_renderer(renderer),
    m_font(font),
    m_pos(pos),
    m_size(size)
{}

void TextLabel::draw()
{
    if (m_texture)
    {
        int width = 0;
        int height = 0;
        SDL_QueryTexture(m_texture.get(), nullptr, nullptr, &width, &height);
        SDL_Rect srcRect;
        SDL_Rect destRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = destRect.w = width;
        srcRect.h = destRect.h = height;
        destRect.x = m_pos.x;
        destRect.y = m_pos.y;
        SDL_RenderCopyEx(m_renderer, m_texture.get(), &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
    }
}
void TextLabel::setText(const std::string &text)
{
    if (m_texture)
    {
        SDL_DestroyTexture(m_texture.get());
        m_texture = nullptr;
    }
    if (text.size())
    {
        //Open the font
        TTF_Font *font = TTF_OpenFont(m_font.c_str(), m_size);
        if (font)
        {
            SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), {0,0,0});
            m_texture.reset(SDL_CreateTextureFromSurface(m_renderer, surf));
            if (!m_texture)
            {
                std::cout << "SDL renderText error\n";
            }
            SDL_FreeSurface(surf);
            TTF_CloseFont(font);
        }
        else
        {
            std::cout << "Cannot open font " << m_font << '\n';
        }
    }
}
TextLabel::~TextLabel()
{
}