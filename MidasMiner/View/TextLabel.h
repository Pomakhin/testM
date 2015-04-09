//
//  TextLabel.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 09.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__TextLabel__
#define __MidasMiner__TextLabel__

#include <stdio.h>
#include "Utils.h"
#include <string>

class SDL_Renderer;
class SDL_Texture;

class TextLabel
{
private:
    SDL_Renderer* m_renderer = nullptr;
    std::string m_font;
    Point m_pos;
    int m_size = 0;
    SDL_Texture *m_texture = nullptr;
public:
    TextLabel(SDL_Renderer* renderer, const std::string &font, const Point &pos, int size);
    ~TextLabel();
    void draw();
    void setText(const std::string &text);
};

#endif /* defined(__MidasMiner__TextLabel__) */
