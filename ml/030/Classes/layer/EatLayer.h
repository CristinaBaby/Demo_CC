//
//  EatLayer.h
//  OreoMaker
//
//  Created by wusonglin1// on 14-11-10.
//
//

#ifndef __OreoMaker__EatLayer__
#define __OreoMaker__EatLayer__

#include <iostream>
#include "cocos2d.h"
#include "BaseLayer.h"


#include "Global.h"
#include "ScribbleTouchNode.h"
#include "ToolSprite.h"
#include "ShadeSprite.h"
USING_NS_CC;
using namespace std;
class EatLayer:public BaseLayer,public ToolSpriteDelegate,public ScribbleTouchDelegate{
    
private:
    EatLayer();
    ~EatLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(EatLayer);
    
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
    void savePic(float ft);
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    
    
    void addFavoriteCallback(bool isSuc);
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
    
    
    ScribbleTouchNode* node;
    ScribbleTouchNode* nodeBom;
    
    ProgressTimer* m_pdrink;
    ToolSprite* m_pEatAgain;
    
 
    bool m_bTouchOver;
    bool m_bIsTouch;
   
};
#endif /* defined(__OreoMaker__EatLayer__) */
