//
//  SL_EatLayer.h
//  OreoMaker
//
//  Created by wusonglin1// on 14-11-10.
//
//

#ifndef __OreoMaker__SL_EatLayer__
#define __OreoMaker__SL_EatLayer__

#include <iostream>
#include "cocos2d.h"
#include "BaseLayer.h"


#include "Global.h"
#include "SL_ScribbleTouchNode.h"
#include "ToolSprite.h"
#include "ShadeSprite.h"
#include "FileUtility.h"
USING_NS_CC;
using namespace std;
class SL_EatLayer:public BaseLayer,public ToolSpriteDelegate,public SL_ScribbleTouchDelegate{
    
private:
    SL_EatLayer();
    ~SL_EatLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(SL_EatLayer);
    
public:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
public:
    void initUI();
    void initMenu();
    void onBtnCallBack(Ref* sender);
    virtual void showTip();
    void _showTip();
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    void setRenderTexture(RenderTexture* rt){m_pRenderTexture = rt; m_pRenderTexture->setPosition(Vec2(m_pRenderTexture->getSprite()->getContentSize().width*0.5, m_pRenderTexture->getSprite()->getContentSize().height*0.5)); addChild(m_pRenderTexture);};
    //    virtual void onExit();
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    void getScribbleCallback(float ft);
    
    void _getScribbleCallback(float ft);
    
    
    void saveToPhoto(float ft);
    void _saveToPhoto(float ft);
    void savePic(float ft);
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    
    
    void addFavoriteCallback(bool isSuc);
    void _addFavoriteCallback(bool isSuc);
    void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    
    void initCup();
    void ProgressTimerFunc(float dt);
    void DrinkMove(float dt);
    
//    void eatOver(Ref*);
    void eatAgain();
    void onScribbleTouchOver();
    void onScribbleTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
   
    
    
    void showPhotoBg();
    void hidePhotoBg();
    
    void startTimeAction(float f);
    void readyAction();
    void goAction();
    void timeAction();
    void showNomorTip();
    void showTips(bool isGood);
    void drinkWater(float f);
private:
    Sprite* m_pBackGround;
    //Sprite* m_pFavor;
    Sprite* m_pPlate;
    Sprite* m_pSpriteTip;
    Sprite* m_pCup1;
    Sprite* _cup;
    Sprite* _cupSprite;
    ToolSprite* m_pCup2;
    
    Sprite* m_pDrink;
    
    Menu* m_pUIMenu;
    MenuItemSprite *m_pHomeItemSprite;
    MenuItemSprite *m_pBackItemSprite;
    MenuItemSprite *m_pFavItemSprite;
    
    RenderTexture* m_pRenderTexture;
    RenderTexture* m_pFavoriteTexture;
    RenderTexture* _m_pFavoriteTexture;
    
    SL_ScribbleTouchNode* node;
    SL_ScribbleTouchNode* nodeBom;
    
    ProgressTimer* m_pdrink;
    ToolSprite* m_pEatAgain;
    
 
    bool m_bTouchOver;
    bool m_bIsTouch;
    string m_sPath;
};
#endif /* defined(__OreoMaker__SL_EatLayer__) */
