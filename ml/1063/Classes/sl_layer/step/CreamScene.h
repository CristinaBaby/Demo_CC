//
//  CreamScene.h
//  CCATS016_CakeMaker
//
//  Created by wusonglin1 on 15/9/21.
//
//

#ifndef __CCATS016_CakeMaker__CreamScene__
#define __CCATS016_CakeMaker__CreamScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"
#include "MyPaintSprite.h"


using namespace ui;
USING_NS_CC;

//步骤的基本模板
class CreamScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    CreamScene();
    ~CreamScene();
public:
    virtual bool init();
    CREATE_FUNC(CreamScene);
    CREATE_SCENE(CreamScene);
    
    void showHandOne();
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
    
    void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
    
    void MaterialComing();
    void MaterialComing1();
    void showHand();
public:
    //下一步
    void nextStep();
public:
    Sprite* m_pBG;
    Sprite* m_pStands;
    ToolSprite* m_pWhisk;
    MyPaintSprite* m_pCakePaint;
    Vec2 oldPos;
    bool m_bPaint;
    bool m_bShowHand;
    bool m_bMakeOver;
    Sprite* m_pWhiskCover;
    Sprite* m_pHand;
};

#endif /* defined(__CCATS016_CakeMaker__CreamScene__) */
