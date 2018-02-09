//
//  IceCreamIceCreamEatLayer.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/13.
//
//

#ifndef __CCATS1044_BeachFoodParty__IceCreamIceCreamEatLayer__
#define __CCATS1044_BeachFoodParty__IceCreamIceCreamEatLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"


#include "Global.h"
#include "ScribbleTouchNode.h"
#include "ToolSprite.h"

USING_NS_CC;
using namespace std;
class IceCreamEatLayer:public BaseLayer,public ToolSpriteDelegate{
    
private:
    IceCreamEatLayer();
    ~IceCreamEatLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(IceCreamEatLayer);
    
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
    void saveToPhoto(float ft);
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    
    void addFavoriteCallback(bool isSuc);
    void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    
    void initCup();
    void ProgressTimerFunc(float dt);
    void DrinkMove(float dt);
    
    void eatOver(Ref*);
    void eatAgain();
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
    
    Sprite* m_pShapeSprite;
    ScribbleTouchNode* node;
    ProgressTimer* m_pdrink;
    ToolSprite* m_pEatAgain;
};
#endif /* defined(__CCATS1044_BeachFoodParty__IceCreamIceCreamEatLayer__) */
