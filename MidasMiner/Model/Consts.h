//
//  Consts.h
//  MidasMiner
//
//  Created by Mikhail Pomakhin on 07.04.15.
//  Copyright (c) 2015 Mikhail Pomakhin. All rights reserved.
//

#ifndef __MidasMiner__Consts__
#define __MidasMiner__Consts__

#include <map>
#include <string>
#include "Utils.h"

static const int C_BOARD_WIDTH = 8;
static const int C_BOARD_HEIGHT = 8;
static const int C_MIN_MATCH_COUNT = 3;
static const int C_OBJECTS_TYPES_COUNT = 5;
static const Point C_BOARD_TOP_LEFT(335, 110);
static const int C_OBJECT_SIZE = 40;
static const float C_GRAVITATIONAL_ACCELERATION = 0.4f;
static const float C_BASE_SPEED = 2.0f;

static const std::map<int,std::string> C_TYPES_MAP =
{
    {1, "Blue"},
    {2, "Green"},
    {3, "Purple"},
    {4, "Red"},
    {5, "Yellow"},
};

#endif /* defined(__MidasMiner__Consts__) */
