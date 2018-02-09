//
//  OvenScene.h
//  CCATS016_CakeMaker
//
//  Created by wusonglin1 on 15/9/18.
//
//

#ifndef __CCATS016_CakeMaker__OvenScene__
#define __CCATS016_CakeMaker__OvenScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class OvenScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    OvenScene();
    ~OvenScene();
public:
    virtual bool init();
    CREATE_FUNC(OvenScene);
    CREATE_SCENE(OvenScene);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
    
    void showHand(Vec2 pos);
    void showHand1();
    void showHand2();
    void showHand3();
    void showHand4();
    
    void showCake();
    void startOven(float dt);
    
public:
    //下一步
    void nextStep();
    
    
public:
    Sprite* m_pBG;
    Sprite* m_pHand;
    
    ToolSprite* m_pOvenCover;
    ToolSprite* m_pSwitch;
    
    Sprite* m_pCake;
    Rect m_rOverRect;
    
    bool m_bOvenClose;
    bool m_bSitchClose;
    bool m_bOvenOver;
    int m_iOvenNum;
  
};
#endif /* defined(__CCATS016_CakeMaker__OvenScene__) */
