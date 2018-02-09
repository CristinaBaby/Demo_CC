//
//  ShopScene.hpp
//  Test
//
//  Created by liqiang on 16/4/20.
//
//

#ifndef ShopScene_hpp
#define ShopScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "CFLayer.h"
#include "AdapterScreen.h"
#include "KSButton.h"
#include "AppearManager.h"
#include "Dialog.h"
USING_NS_CC;

class ShopScne : public CFLayer,public DialogCallback
{
public:
   
    static Scene* createScene();
 
    virtual bool init();
   
    CREATE_FUNC(ShopScne);
    
    ShopScne();
    ~ShopScne();
    
private:
    void onNegativeClick(void*);
    void onPositiveClick(void*);
    
    void purchaseFinishCallback(bool isPurchase);
    void AfterRestoreCallBack(bool isRestore,bool isPurchase);
    
    virtual void onEnter();
    virtual void onExit();
    
    void closeBanner();
    
    Sprite* m_pSpriteAllCookie;
    Sprite* m_pSpriteFullVersion;
    Sprite* m_pSpriteDec;
    
    KSButton* noAds;
    
    Vector<Sprite*> m_vSprite;
    
    void btnClick(KSButton* btn);
};

#endif /* ShopScene_hpp */
