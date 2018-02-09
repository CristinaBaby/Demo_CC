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
//游戏选择界面的资源名字

//选择香肠的资源图名称
std::string CHOOSE_HOTDOG_PATH[7] = {"regular-dog","beef-dog","chicken-dog","beef-spicy-dog","bratwurst-dog","vegetarian-dog","red-snapper-dog"};

//选择香肠的类型
int DogHot_Num = 1;
Layer* decorationLayer = nullptr;
Layer* fillToolLayer = nullptr;
Layer* notEatLayer = nullptr;
//Vector<ToolSprite*> V_TOOL;
int Mac_Num= -1;//机器
//装饰界面的Item
std::string DEC_ITEM[DEC_ITEM_NUMS] = {"cone","icing","sprinkle","candy","fruit","backgrounds"};
int DEC_ITEM_Num[DEC_ITEM_NUMS] = {30,29,43,43,38,30};
Rect R_Rect = Rect::ZERO;
int MiniGame_Num=-1;
int MiniGame_Num_1=-1;
vector<string> _vDidLockItem;
string g_choose_waste="";
int g_dec_bg = -1;
int g_dec_cone = -1;
bool g_bFromHome = false;