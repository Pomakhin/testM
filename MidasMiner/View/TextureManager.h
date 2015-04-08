//
//  TextureManager.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__TextureManager__
#define __MidasMiner__TextureManager__

#include <stdio.h>
#include <string>
#include <map>
#include "GenericSingleton.h"

class SDL_Renderer;
class SDL_Texture;

struct TextureData
{
    SDL_Texture *SDLTexture = nullptr;
    int Width = 0;
    int Height = 0;
    TextureData(SDL_Texture *sdlTexture, int width, int height):
        SDLTexture(sdlTexture),
        Width(width),
        Height(height)
    {}
    TextureData(){}
};

class TextureManager : public Singleton<TextureManager>
{
    friend class Singleton<TextureManager>;
private:  
    std::map<std::string, TextureData> m_textureMap;
protected:
    TextureManager(){};
public:
    
    bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
    void draw(std::string id, int x, int y, SDL_Renderer* pRenderer, int alpha = 255);
};

#endif /* defined(__MidasMiner__TextureManager__) */
