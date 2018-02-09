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
#define  BTN_MARGIN             18
#define  SWISH_SOUND_TIME       0.9f

USING_NS_CC;

static const float DESIGN_WIDTH = 960;
static const float DESIGN_HEIGHT= 640;

extern bool g_isDebugMode;

// global
extern bool g_bEatSoundPlaying;
extern bool g_bIsNeedAds;
extern bool g_bIsFromShop;

// project
extern int g_iIceCreamType;
extern int g_iStickerIndex;
extern int g_iSyrupIndex;
extern int g_iIceCreamCount;
extern bool g_bBgMusicType;

extern bool g_bIsFlag;

//fonts
static const std::string font_felt =
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
"fonts/COOPBL.TTF";
#else
"fonts/COOPBL.TTF";
#endif

const bool PLATFORM_IOS=
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
true;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
false;
#endif

#define     TOOL_TYPE_NUMS        4
extern int          CATEGORY_NUMS[];
extern std::string  CATEGORY_NAME[];

#endif /* defined(__OreoMaker__Global__) */
