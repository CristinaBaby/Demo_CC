#ifndef __GLOBAL_HH__
#define __GLOBAL_HH__
#include "cocos2d.h"
#include <string>
#include <Map>

using namespace std;

USING_NS_CC;

//design resolution
extern cocos2d::Size design_size;

static const string IAP_ALL="com.macygonecrazy.firemananimal.unlockall";
static const string IAP_VEHICLES="com.macygonecrazy.firemananimal.allvehicles";
static const string IAP_CLOTHES="com.macygonecrazy.firemananimal.allclothes";
//scene change time
static const float SCENE_MOVING_TIME=0.25;
static const float WakeUpDistance=150.0f;
static const float RoleSpeed=3.5f;
static const float EnemySpeed=1.2f;

//menu type
static const int MENU_TYPE_1=1;
static const int MENU_TYPE_2=2;
static const int MENU_TYPE_3=3;
static const int MENU_TYPE_4=4;
static const int MENU_TYPE_5=5;
static const int MENU_TYPE_6=6;

extern float blankArea_width;
extern float blankArea_height;
extern  float contentScale;
extern cocos2d::Size screenSize;
extern int resourceType;
extern bool soundOn;
extern bool closeAds;


#define SF_RESOURCES_1024_768            0
#define SF_RESOURCES_2048_1536           1

#define ccp2(X,Y)(resourceType==SF_RESOURCES_2048_1536? (cocos2d::Point((float)(X)*2, (float)(Y)*2)):(cocos2d::Point((float)(X), (float)(Y))))
#define CCSize2(width, height)( resourceType==SF_RESOURCES_2048_1536?cocos2d::Size((float)((width)*2), (float)((height)*2)):cocos2d::Size((float)(width), (float)(height)))


//fonts
static const std::string font_felt =
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
"dialog/Gotham-Ultra.otf";
#else
"dialog/Gotham-Ultra.otf";
#endif
#define fontsize(size) (resourceType==SF_RESOURCES_2048_1536) ? (float)(size) * 2 : size

typedef enum
{
    eColorTypeRGB = 0,
    eColorTypeHSV
}ColorType;


typedef enum
{
    eXNone=0,
    eXLeft,
    eXRight,
    eXCenter
}PaddingTypeX;

typedef enum
{
    eYNone=0,
    eYTop,
    eYBottom,
    eYCenter
}PaddingTypeY;

struct Color{
    int r;
    int g;
    int b;
};



struct ComponentTexture
{
	string imageName;
	int zOrder;
	cocos2d::Point position;
    
} ;


struct StringBean
{
    
	int id;
	string Description;
	string Comment;
	string Language;
};

struct IAPBean
{
    
	int id;
	string key;
	int buyNumber;
};

struct GameData{
    int personIndex;
    int carIndex;
    int cureIndex;
};

extern GameData gameData;

extern map<int,IAPBean> IAPs;

typedef void (Ref::*SEL_CALLBACK)(Ref*);
#define  callbackselector(_SELECTOR) (SEL_CALLBACK)(&_SELECTOR)

typedef void (Ref::*SEL_CALLBACK1)(Ref*,void* );
#define  callbackselector1(_SELECTOR) (SEL_CALLBACK1)(&_SELECTOR)

inline float scribbleTargetScaleRate();

inline float scribbleTargetScaleRate(){
    return (SF_RESOURCES_1024_768 == resourceType) ? 2.0 : 4.0;
}

inline float contentSizeScaleRate();

inline float contentSizeScaleRate(){
    return (SF_RESOURCES_1024_768 == resourceType) ? 1.0 : 2.0;
}

inline cocos2d::Size renderTextureSize(cocos2d::Sprite *pSprite){
    cocos2d::Size lSpriteSize = pSprite->getContentSize();
    float lWidth = lSpriteSize.width;
    float lHeight = lSpriteSize.height;
    
    
    const float lFitSize = 370;//min 353
    
    if (lWidth < lFitSize) {
        lWidth = lFitSize;
    }
    if (lHeight < lFitSize) {
        lHeight = lFitSize;
    }
    
    return cocos2d::Size(lWidth, lHeight);
}

inline cocos2d::Point ccpUI(float _x ,float _y,PaddingTypeX X_padding,PaddingTypeY Y_padding){
    cocos2d::Point position;
    float x=_x;
    float y=_y;
    switch (X_padding) {
        case eXLeft:
            
            break;
        case eXRight:
            if(resourceType==SF_RESOURCES_2048_1536)
            {
                x=screenSize.width/2-(design_size.width-x);
            }else{
                x=screenSize.width-(design_size.width-x);
            }
            break;
        case eXCenter:
            if(resourceType==SF_RESOURCES_2048_1536)
            {
                x=screenSize.width/4;
            }else{
                x=screenSize.width/2;
            }
            break;
        case eXNone:
            
            break;
    }
    
    
    switch (Y_padding) {
        case eYTop:
            if(resourceType==SF_RESOURCES_2048_1536)
            {
                y=screenSize.height/2-(design_size.height-y);
            }else{
                y=screenSize.height-(design_size.height-y);
            }
            break;
        case eYBottom:
            
            break;
        case eYCenter:
            if(resourceType==SF_RESOURCES_2048_1536)
            {
                y=screenSize.height/4;
            }else{
                y=screenSize.height/2;
            }
            break;
        case eXNone:
            
            break;
    }
    
    position=ccp2(x,y);
    return position;
}

#endif