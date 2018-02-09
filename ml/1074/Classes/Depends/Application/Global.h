#ifndef __GLOBAL_HH__
#define __GLOBAL_HH__
#include "cocos2d.h"
#include <string>
#include <Map>

using namespace std;

USING_NS_CC;
//design resolution
extern cocos2d::Size design_size;
extern cocos2d::Size frame_size;

extern float blankArea_width;
extern float blankArea_height;
extern  float contentScale;
extern cocos2d::Size screenSize;
extern int resourceType;
extern bool soundOn;
extern bool closeAds;
extern int gToolType;


//menu type
static const int MENU_TYPE_1=1;
static const int MENU_TYPE_2=2;
static const int MENU_TYPE_3=3;
static const int MENU_TYPE_4=4;
static const int MENU_TYPE_5=5;
static const int MENU_TYPE_6=6;

static const int MENU_TYPE_FAV=14;
static const int MENU_TYPE_RESET=13;
static const int MENU_TYPE_HOME=12;
static const int MENU_TYPE_BACK=11;
static const int MENU_TYPE_EXIT=10;


//personal add
/******************/

extern bool gIsHomeVedio;
extern bool gIsNewgame;
extern bool gNoneIap;

extern cocos2d::Size visibleSize;
extern float adsDystent;

/*********************/

#define SF_RESOURCES_1024_768            0
#define SF_RESOURCES_2048_1536           1

#define ccp2(X,Y)(resourceType==SF_RESOURCES_2048_1536? (Vec2((float)(X)*2, (float)(Y)*2)):(Vec2((float)(X), (float)(Y))))
#define CCSize2(width, height)( resourceType==SF_RESOURCES_2048_1536?cocos2d::Size((float)((width)*2), (float)((height)*2)):cocos2d::Size((float)(width), (float)(height)))

#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,##__VA_ARGS__)


/************************************************************************/
/* just for UILayer                                                     */
/************************************************************************/
//#define pUI(X,Y) (CCPointMake((X)/(design_size.width/screenSize.width),(Y)/(design_size.height/screenSize.height)))
#define pUI(X,Y) (ccp((X)/(design_size.width/screenSize.width),(Y)/(design_size.height/screenSize.height)))

//fonts
static const std::string font_felt =
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    "fonts/Chewy.ttf";
#else
    "fonts/Chewy.ttf";
#endif
#define fontsize(size) (resourceType==SF_RESOURCES_2048_1536) ? (float)(size) * 2 : size

inline float contentSizeScaleRate();

inline float contentSizeScaleRate(){
    return (SF_RESOURCES_1024_768 == resourceType) ? 1.0 : 2.0;
}

#define PIXEL_TO_METER 32
#endif