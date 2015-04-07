//
//  GameObject.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__GameObject__
#define __MidasMiner__GameObject__

#include <stdio.h>
#include <string>

class SDL_Renderer;

class GameObject
{
protected:
    std::string m_textureId;
    int m_width = 0;
    int m_height = 0;
    int m_x = 0;
    int m_y = 0;
public:
    void draw(SDL_Renderer* renderer);
    void update();
    void load(int x, int y, const std::string &textureId);
};

#endif /* defined(__MidasMiner__GameObject__) */
