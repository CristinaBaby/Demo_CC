//
//  HomeLayer.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-22.
//
//

#ifndef __OreoMaker__HomeLayer__
#define __OreoMaker__HomeLayer__

#include <iostream>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "BaseLayer.h"
#include "Utils.h"
#include "SceneManager.h"
#include "ToolSprite.h"

USING_NS_CC;

enum{
    eTagStart,
    eTagFavorite,
    eTagShop,
    eTagSetting,
    eTagRateus,
    eTagPrivacy,
    eTagMoreGame,
    eMiniGame,
};
class HomeLayer:public BaseLayer,public ToolSpriteDelegate{
    
private:
    HomeLayer();
    ~HomeLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(HomeLayer);
    
public:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
public:
    void initUI();
    void onBtnCallBack(Ref* sender);
    
   void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
   void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    
    //展示全屏广告的回调
    virtual void showAdsLaod(EventCustom*);
    //关闭全屏广告的回调
    virtual void closeAdsLaod(EventCustom*);
    void ShowUIMenu();
    void ShowLogo();
    void ShowUIPlay();
    void showCar(float dt);
    
    void carAction();
    void logoAction();
    
    void nextAction();
    void carGoOut();
public:
    Menu* m_pUIMenu;
private:
    MenuItemSprite *m_pStartItemSprite;
    MenuItemSprite *m_pShopItemSprite;
    MenuItemSprite *m_pMoreItemSprite;
    MenuItemSprite *m_pFavItemSprite;
    MenuItemSprite *m_pRateusItemSprite;
    MenuItemSprite *m_pPrivacyItemSprite;
//    MenuItemSprite *m_pMiniGameItemSprite;
    MenuItemSprite *m_pSettingItemSprite;
    
    ToolSprite* m_pLogo;
    Sprite* m_pBgSprite;
    Sprite* m_pBgSpriteFront;
    ToolSprite* m_pCar;
    
    bool m_bFirst;
    bool m_bShowNextScene;
};

#endif /* defined(__OreoMaker__HomeLayer__) */
