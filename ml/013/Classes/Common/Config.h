//
//  MakeConfig.h
//  Chups006
//
//  Created by QAMAC01 on 14-9-16.
//
//

#ifndef Chups006_Config_h
#define Chups006_Config_h

enum
{
    kTagFlour = 100,
    kTagMilk,
    kTagOil1,
    kTagOil2,
    kTagEgg1,
    kTagEgg2,
    kTagSpoon,
    kTagPan,
    kTagBouquet,
    kTagStraw,
    kTagHint,
    kTagBox,
    kTagCocoa,
    kTagSalt,
    kTagBakingSoda,
    kTagShortening,
    kTagButter,
    kTagSugar,
    kTagBowl,
    kTagUmbrella,
    kTagKnife,
    
    kTagLeft,
    kTagRight,
    kTagDown,
    kTagReady,
    kTagTongue,
    kTagNone,
    kTagCancel,
};


enum
{
    kBtnPlay = 12000,
    kBtnMore,
    kBtnPhoto,
    kBtnPre,
    kBtnSound,
    kBtnShop,
    kBtnRate,
    kBtnPolicy,
    kBtnFav,
    kBtnBack,
    kBtnTrash,
    kBtnClose,
    kBtnNext,
    kBtnReset,
    kBtnResetGame,
    kBtnHome,
    kBtnRestore,
    kBtnLock,
    kBtnDelete,
    kBtnMail,
    kBtnBlend,
    kBtnInject,
    kBtnTypeCell,
    kBtnIap,
    kBtnEatAgain,
    kBtnCancel,
    kBtnConfirm,
    kBtnTakePhoto,
    kBtnSwip,
    kBtnConfirmBase,
    kBtnCancelBase,
    kBtnReEat,

};

enum
{
    kStepSwipUp =300,
    kStepSwipDown,
    kStepNone,

};




#include "cocos2d.h"
USING_NS_CC;

class Config : public Ref
{
public:
        
    static int getShape();
    static const char* getShapeScribbleName();
    static std::string getShapeName();
    static Texture2D* getShapeScribbleTexture(int h, int s, int v);
    static Texture2D* getShapeScribbleTexture(int h, int s, int v, int idx);
    static const char* getChoosenName(const char* pName);

    //action
    static void flashAction(Sprite* sp, bool isRepeat, int times);
    static void zoomAction(Sprite* sp, bool isRepeat, int times);
    
    static Vec2 getSnowconePos();

};

#endif
