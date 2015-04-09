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
#include "SDLCommon.h"

class SDL_Renderer;
class SDL_Texture;

struct TextureData
{
    std::unique_ptr<SDL_Texture, SDLDeleter> SDLTexture;
    int Width = 0;
    int Height = 0;
    TextureData(SDL_Texture *sdlTexture, int width, int height):
        Width(width),
        Height(height)
    {
        SDLTexture.reset(sdlTexture);
    }
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
    ~TextureManager();
    bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
    void draw(std::string id, int x, int y, SDL_Renderer* pRenderer, int alpha = 255);
};

#endif /* defined(__MidasMiner__TextureManager__) */
