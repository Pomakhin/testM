//
//  main.cpp
//  MidasMiner
//
//  Created by Михаил Помахин on 04.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#include <iostream>
#include "GameController.h"

int main(int argc, const char * argv[])
{
    GameController::getInstance()->init();
    return 0;
}
