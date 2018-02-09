//
//  MakeScene.h
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/7.
//
//

#ifndef __MLABS030_IceCreamKids_CookingGames__MakeScene__
#define __MLABS030_IceCreamKids_CookingGames__MakeScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class MakeScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    MakeScene();
    ~MakeScene();
public:
    virtual bool init();
    CREATE_FUNC(MakeScene);
    CREATE_SCENE(MakeScene);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
    
   void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
   void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
   void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
    
    //各个材料进入动画
    void MaterialComing1(float dt);
    void MaterialComing2(float dt);
    void MaterialComing3(float dt);
    void MaterialComing4(float dt);
    
    //重力感应
    virtual void didAccelerate(Acceleration* acc, Event* event);
public:
    //下一步
    void nextStep();
    
    void milkAction(float dt);
    void sugarAction(float dt);
    void eggAction(float dt);
    void checkStep(float dt);
    void makeOver();
public:
    Sprite* m_pBG;
    Sprite* m_pBowl;
    Sprite* m_pBowlMix;
    Vector<ToolSprite*> m_vTool;
    Vec2 oldPos;
    int m_iToolZorder;
    
    //需要现实的工具
    std::vector<string> m_vToolPath;
    
    double m_dAccX;
    int m_iEggNum;
    ToolSprite* m_pWhisk;
    
    int m_iStepNum;
    int m_iWhiskNum;
    bool m_bIsSounds;
    bool isTouch;
    
    bool m_bCloseDevice;
};

#endif /* defined(__MLABS030_IceCreamKids_CookingGames__MakeScene__) */
