//
//  CookMakeScene.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/22.
//
//

#ifndef CookMakeScene_hpp
#define CookMakeScene_hpp

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"
#include "MySpriteButton.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class CookMakeScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    CookMakeScene();
    ~CookMakeScene();
    
    virtual bool init();
    CREATE_FUNC(CookMakeScene);
public:
    CREATE_SCENE(CookMakeScene);
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();

    void initUI();
    void nextStep();
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
    
    void showStoven(float dt);
    void showPot(float dt);
    void showPlate(float dt);
    void showWaterCup(float dt);
    void showHandAddWater(float dt);
    void showHandAddGelatin(float dt);
    void showGelatin(float dt);
    void showPotLid(float dt);
    void showPotStir(int index);
    void showPotMix(float dt);
    void showHandFruitToPot(float dt);
    
    void addWaterToPot(int index);
    void addGelatinToPot(int index);
    void switchBtn(Ref *sender);
    void potLidAction(float dt);
    void potLidBtn(Ref *sender);
    
private:
    Sprite* m_pBG;
    Sprite* m_pStoven;
    ToolSprite* m_pCup;
    ToolSprite* m_pGelatin;
    MySpriteButton* m_pSwitch;
    Vector<ToolSprite*> m_vTool;
    Vector<Sprite*> m_vPot;
    Vector<Sprite*> m_vPotCover;
    Vector<MySpriteButton*> m_vPotLid;
    Vector<Sprite*> m_vFire;
    Sprite* m_pHand;
    Vec2 oldPos;
    int m_iZorder;
};


#endif /* CookMakeScene_hpp */
