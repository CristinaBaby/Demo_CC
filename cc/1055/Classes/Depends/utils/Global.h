//
//  Global.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-4.
//
//

#ifndef __OreoMaker__Global__
#define __OreoMaker__Global__

#include "cocos2d.h"
#include <iostream>

#include "ToolSprite.h"

USING_NS_CC;

static const float DESIGN_WIDTH = 960;
static const float DESIGN_HEIGHT= 640;
extern Layer* decorationLayer;
//extern Layer* notEatLayer;
extern Layer* fillToolLayer;

extern bool IAP_DEBUG;
extern bool GAME_DEBUG;
extern bool isLockPackage ;

static const std::string font_felt =
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
"fonts/Chewy.ttf";
#else
"fonts/Chewy.ttf";
#endif

const bool PLATFORM_IOS=
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
true;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
false;
#endif
#define FAVORITE_DIR  "RollUp"
#define MAKERNAME "Italian Food"

#define     DEC_IAP_ID           "com.crazycatsmedia.cookiemaker.decoration"
#define     FLAVOR_IAP_ID        "com.crazycatsmedia.cookiemaker.flavor"

#define     FAVOR_TYPE_NUMS       6
#define     DEC_ITEM_NUMS      7

#define     CoinNum      "coinNum"
#define     SandwichCaheDir      "sandwich.png"
#define     GummyMoldLock "gummyMake"

extern std::string FAVOR_TYPES[FAVOR_TYPE_NUMS];
extern std::string DEC_ITEM[DEC_ITEM_NUMS];
extern int DEC_ITEM_Num[DEC_ITEM_NUMS];

extern std::string g_cook_select[6];
extern int g_cook_index;

extern int g_dec_side_index;
extern int g_dec_plate_index;
extern int g_dec_drink_index;

#endif /* defined(__OreoMaker__Global__) */

