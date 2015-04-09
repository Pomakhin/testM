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

static const int C_FPS = 60;
static const int C_DELAY_TIME = 1000.0f / C_FPS;

static const double C_GAME_TIME = 60.0f;
static const double C_HINT_TIME = 5.0f;
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
    {5, "Yellow"}
};

static const std::map<int,int> C_SEQ_SCORE_MAP =
{
    {3, 50},
    {4, 100},
    {5, 150},
    {6, 250},
    {7, 500},
    {8, 1000}
};

static const std::map<int,int> C_MATCHES_SCORE_MAP =
{
    {2, 50},
    {3, 100},
    {4, 150},
    {5, 250},
    {6, 350},
    {7, 500},
    {8, 750},
    {9, 1000}
};


#endif /* defined(__MidasMiner__Consts__) */
