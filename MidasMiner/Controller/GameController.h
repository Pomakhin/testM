//
//  GameController.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__GameController__
#define __MidasMiner__GameController__

#include <stdio.h>
#include "GenericSingleton.h"

class GameController : public Singleton<GameController>
{
    friend class Singleton<GameController>;
protected:
    GameController(){};
public:
    void init();
};

#endif /* defined(__MidasMiner__GameController__) */
