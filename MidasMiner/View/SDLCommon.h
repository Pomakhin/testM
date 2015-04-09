//
//  SDLCommon.h
//  MidasMiner
//
//  Created by Михаил Помахин on 10.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__SDLCommon__
#define __MidasMiner__SDLCommon__

#include <SDL2/SDL.h>

struct SDLDeleter
{
    void operator()(SDL_Window *p) const
    {
        SDL_DestroyWindow(p);
    }
    void operator()(SDL_Renderer *p) const
    {
        SDL_DestroyRenderer(p);
    }
    void operator()(SDL_Texture *p) const
    {
        SDL_DestroyTexture(p);
    }
};


#endif /* defined(__MidasMiner__SDLCommon__) */
