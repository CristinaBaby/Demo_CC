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

USING_NS_CC;

static const float DESIGN_WIDTH = 640;
static const float DESIGN_HEIGHT= 960;
extern Layer* decorationLayer;
extern Layer* foodLayer;
extern Layer* fillDecorationLayer;
extern Layer* iceDecorationLayer;

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

#define IceCream "IceCream"
#define IcePop "IcePop"

#define     FAVOR_TYPE_NUMS       6
#define     CHOOSE_ITEM_NUMS      3


extern std::string FAVOR_TYPES[FAVOR_TYPE_NUMS];
extern std::string CHOOSE_ITEM_PATH[CHOOSE_ITEM_NUMS];

extern std::string G_IceCream_MenuItem[6];
extern std::string G_IcePop_MenuItem[5];
extern std::string G_Snow_MenuItem[6];
extern int     G_IceCream_DecNum[6];
extern int     G_IcePop_DecNum[5];
 extern int G_Snow_DecNum[6];
//选择游戏类型
extern int CHOOSE_TYPE ;


extern Sprite* G_IcePop_MouleSprite;
extern Sprite* G_IceCream_CupSprite;
extern Sprite* G_Snow_ShapeIce;

extern int G_IceCream_CupNum;
extern int G_IcePop_MouleNum;
extern int G_IcePop_SticksNum;
extern int G_Snow_BGNum ;
extern int G_Snow_SpoonNum ;
extern int G_Snow_CupNum;
extern int G_Snow_ShapeNum ;

extern float bannerHeight;




#endif /* defined(__OreoMaker__Global__) */
