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
#include "MySpriteButton.h"
#include "ToolSprite.h"

USING_NS_CC;

enum{
    eTagStart,
    eTagFavorite,
    eTagShop,
    eTagSetting,
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
    
private:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    
    void btnClick(Ref *sender);
    
     virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
private:
    void initUI();
    void showBear();
    void showLogo();
    void showStartBtn();
    void showUIBtn();
    void btnAction();
private:
    Sprite* m_pBg;
    
    ToolSprite* m_pLogo;
    ToolSprite* m_pBear1;
    ToolSprite* m_pBear2;
    
    MySpriteButton* m_pStartBtn;
    MySpriteButton* m_pMoreGameBtn;
    MySpriteButton* m_pShopBtn;
    bool m_bIsFirst;
};

#endif /* defined(__OreoMaker__HomeLayer__) */
