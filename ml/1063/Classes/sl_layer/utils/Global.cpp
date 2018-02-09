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
Layer* fillToolLayer = nullptr;
Layer* notEatLayer = nullptr;

std::string DEC_ITEM[DEC_ITEM_NUMS] = {"cake","icing","caketopper","candy","plates"};
int DEC_ITEM_Num[DEC_ITEM_NUMS] = {34,38,30,15,20};
Rect R_Rect = Rect::ZERO;
int MiniGame_Num=-1;
int MiniGame_Num_1=-1;
vector<string> _vDidLockItem;
string g_choose_waste="strawberry";
int g_dec_bg = -1;
int g_dec_plates = -1;
bool g_eat_cake = false;
bool g_from_eat = false;

//路径，方便修改
std::string g_path_dec = "cocostudio/png/cake/dec/";
std::string g_path_ui  = "cocostudio/ui/public/";
std::string g_path = "cocostudio/png/cake/";




