//
//  TextureManager.cpp
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include "TextureManager.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* pRenderer)
{
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
    if(pTempSurface == 0)
    {
        return false;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
    SDL_FreeSurface(pTempSurface);
    // everything went ok, add the texture to our list
    if(texture != 0)
    {
        int width = 0;
        int height = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        TextureData data;
        data.SDLTexture = texture;
        data.Width = width;
        data.Height = height;
        m_textureMap[id] = data;
        return true;
    }
    // reaching here means something went wrong
    return false;
}

void TextureManager::draw(std::string id, int x, int y, SDL_Renderer* pRenderer, int alpha)
{
    const TextureData &texData = m_textureMap[id];
    SDL_Rect srcRect;
    SDL_Rect destRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = destRect.w = texData.Width;
    srcRect.h = destRect.h = texData.Height;
    destRect.x = x;
    destRect.y = y;
    SDL_SetTextureAlphaMod( texData.SDLTexture, alpha );
    SDL_RenderCopyEx(pRenderer, texData.SDLTexture, &srcRect,
                     &destRect, 0, 0, SDL_FLIP_NONE);
    SDL_SetTextureAlphaMod( texData.SDLTexture, 255 );
}