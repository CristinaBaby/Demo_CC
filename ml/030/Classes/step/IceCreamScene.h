//
//  IceCreamScene.h
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/9.
//
//

#ifndef __MLABS030_IceCreamKids_CookingGames__IceCreamScene__
#define __MLABS030_IceCreamKids_CookingGames__IceCreamScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class IceCreamScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    IceCreamScene();
    ~IceCreamScene();
public:
    virtual bool init();
    CREATE_FUNC(IceCreamScene);
    CREATE_SCENE(IceCreamScene);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
    
    void showHand1();
    void showHand2(float dt);
    void showHand3();
    
    void coneAction(float dt);
    void crushAction(float dt);
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
public:
    //下一步
    void nextStep();
public:
    Sprite* m_pBG;
    Sprite* m_pMachine;
    Vec2 oldPos;
    int m_iCrushNum;
    Sprite* m_pCrush;
};


#endif /* defined(__MLABS030_IceCreamKids_CookingGames__IceCreamScene__) */
