//
//  HomeScene.hpp
//  Test
//
//  Created by liqiang on 16/4/22.
//
//

#ifndef HomeScene_hpp
#define HomeScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "CFLayer.h"
#include "AppearManager.h"
#include "KSButton.h"
#include "MainScene.h"
#include "GameData.h"
#include "KitchenLayer.h"
#include "AdsManager.h"
#include "BaseLayer.h"
class HomeScene:public BaseLayer
{
    
public:
    static Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(HomeScene);
    
    HomeScene();
    ~HomeScene();
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    Sprite* m_pSpriteCookie;
    Sprite* m_pSpriteLogo;
    Sprite* m_pSpriteTapeYellow;
    Sprite* m_pSpriteTapeGreen;
    
    void BalloonShake(Node* n ,float time ,float angle);
    
    
    KSButton* m_pBtnPlay;
    KSButton* m_pBtnMore;
    KSButton* m_pBtnRate;
    KSButton* m_pBtnShop;
    void btnClick(KSButton* btn);
    
    void goToNext();
    
    bool m_bIsFirst;
    
    void appResume();
private:
    /*background layer*/
    Layer*       m_pBgLayer;
    /*content layer*/
    Layer*       m_pContentLayer;
    /*UI layer*/
    Layer*       m_pUILayer;
    
public:
    void addToBgLayer(Node*,int zorder = 0,int tag = 0);
    void addToContentLayer(Node*,int zorder = 0,int tag = 0);
    void addToUILayer(Node*,int zorder = 0,int tag = 0);
};


#endif /* HomeScene_hpp */
