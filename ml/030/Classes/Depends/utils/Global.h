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
extern Layer* notEatLayer;
extern Layer* fillToolLayer;

extern bool IAP_DEBUG;
extern bool GAME_DEBUG;
extern bool isLockPackage ;


static const std::string font_felt =
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
"images/dialog/HELVETICA_ROUNDED_LT_BLACK.TTF";
#else
"images/dialog/HELVETICA_ROUNDED_LT_BLACK.TTF";
#endif

const bool PLATFORM_IOS=
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
true;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
false;
#endif
#define FAVORITE_DIR  "RollUp"
#define MAKERNAME "Italian Food"


#define     FAVOR_TYPE_NUMS       6
#define     DEC_ITEM_NUMS      6

extern std::string FAVOR_TYPES[FAVOR_TYPE_NUMS];
extern std::string DEC_ITEM[DEC_ITEM_NUMS];
extern int DEC_ITEM_Num[DEC_ITEM_NUMS];

extern Rect R_Rect;
//已经解锁的物品
extern std::vector<std::string> _vDidLockItem;

extern std::string g_choose_waste;
extern int g_dec_bg;
extern int g_dec_cone;
extern bool g_bFromHome;
#endif /* defined(__OreoMaker__Global__) */

