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

Layer* decorationLayer;
bool IAP_DEBUG = true;
bool GAME_DEBUG = true;

bool IS_ICON_LOADFINISH = false;
//游戏选择界面的资源名字

//选择香肠的资源图名称
std::string CHOOSE_HOTDOG_PATH[7] = {"regular-dog","beef-dog","chicken-dog","beef-spicy-dog","bratwurst-dog","vegetarian-dog","red-snapper-dog"};

//装饰界面的Item
std::string DEC_ITEM[DEC_ITEM_NUMS] = {"cheese","meat","others","vegetable","drink","sidedish","plate"};
int DEC_ITEM_Num[DEC_ITEM_NUMS] = {6,6,6,6,6,6,6};



std::string g_cook_select[6]={"bagel","bananabread","brownbread","chocolateswirlbread","raisinbread","whitebread"};

int g_cook_index = -1;

int g_dec_plate_index=-1;
int g_dec_side_index= -1;
int g_dec_drink_index= -1;