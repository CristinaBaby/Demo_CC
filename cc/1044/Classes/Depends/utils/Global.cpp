//
//  Global.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-4.
//
//

#include "Global.h"

float volume_sound=1;
float volume_music=1;

bool IS_2048 = false;

float UI_SCALE = 1.f;


bool IAP_DEBUG = true;
bool GAME_DEBUG = true;

bool IS_ICON_LOADFINISH = false;

Layer* decorationLayer = nullptr;
Layer* iceDecorationLayer = nullptr;
Layer* foodLayer = nullptr;
Layer* fillDecorationLayer = nullptr;

//游戏选择界面的资源名字
std::string CHOOSE_ITEM_PATH[CHOOSE_ITEM_NUMS] = {"snow_cone","ice_cream","ice_pop"};

//游戏类型
int CHOOSE_TYPE = -1;

//IceCream
std::string G_IceCream_MenuItem[6] = {"syrup","candy","sprinkle","cookie","topper","extras"};
Sprite* G_IceCream_CupSprite = nullptr;
int     G_IceCream_CupNum = -1;
int     G_IceCream_DecNum[6] = {33,31,32,32,30,33};


//IcePop
std::string G_IcePop_MenuItem[5] = {"coating","sprinkle","candy","extras","sticks"};
Sprite* G_IcePop_MouleSprite = nullptr;
int G_IcePop_MouleNum = -1;
int G_IcePop_DecNum[5] = {20,32,31,33,22};
int G_IcePop_SticksNum = -1;

//Snow
std::string G_Snow_MenuItem[6] = {"sprinkle","spoons","candy","extras","topper","bg"};
int G_Snow_CupNum = -1;
int G_Snow_ShapeNum = -1;
int G_Snow_BGNum = -1;
int G_Snow_SpoonNum = -1;
int G_Snow_DecNum[6] = {32,35,31,33,30,24};

Sprite* G_Snow_ShapeIce = nullptr;

float bannerHeight = 0.0;





