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
#include "../Depends/utils/AdapterScreen.h"
#include "../Depends/base/BaseLayer.h"
#include "../Depends/utils/Utils.h"
#include "../Depends/managers/SceneManager.h"

USING_NS_CC;

enum{
    eTagStart,
    eTagFavorite,
    eTagShop,
    eTagSetting,
    eTagPrivacy,
    eTagMoreGame,
};
class HomeLayer:public BaseLayer{
    
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
    
public:
    void initUI();
    void onBtnCallBack(Ref* sender);
    
    // 监听Android返回键事件
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    
    
    //展示全屏广告的回调
    virtual void showAdsLaod(Ref*);
    //关闭全屏广告的回调
    virtual void closeAdsLaod(Ref*);
    void ShowUIMenu();
    void ShowLogo();
    void ShowUIPlay();
    void createQiqiu(float);
public:
    Menu* m_pUIMenu;
private:
    MenuItemSprite *m_pStartItemSprite;
    MenuItemSprite *m_pShopItemSprite;
    MenuItemSprite *m_pMoreItemSprite;
    MenuItemSprite *m_pFavItemSprite;
    MenuItemSprite *m_pPrivacyItemSprite;
    MenuItemSprite *m_pSettingItemSprite;
    
    Sprite* m_pLogo;
    Sprite* m_pBgSprite;
    Sprite* m_pCrab;
    Sprite* m_pBear;
};

#endif /* defined(__OreoMaker__HomeLayer__) */
