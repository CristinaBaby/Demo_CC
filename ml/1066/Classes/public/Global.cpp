//
//  Global.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-4.
//
//

#include "Global.h"

bool g_isDebugMode = true;

// public
bool g_bEatSoundPlaying = false;
bool g_bIsNeedAds = true;
bool g_bIsFromShop = false;


// project
int g_iIceCreamType = -1;
int g_iStickerIndex = -1;
int g_iSyrupIndex = -1;
int g_iIceCreamCount = -1;
// false表示首页背景音乐，true表示其它页面背景音乐
bool g_bBgMusicType = false;

bool g_bIsFlag = false;

int  CATEGORY_NUMS[TOOL_TYPE_NUMS];
std::string CATEGORY_NAME[TOOL_TYPE_NUMS];

