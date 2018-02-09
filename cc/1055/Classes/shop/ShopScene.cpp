//
//  ShopScene.cpp
//  Test
//
//  Created by liqiang on 16/4/20.
//
//
#include "AdapterScreen.h"
#include "ShopScene.hpp"
#include "IAPManager.h"
#include "AdsManager.h"
#include "GameData.h"
Scene* ShopScne::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ShopScne::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


ShopScne::ShopScne():m_pSpriteAllCookie(nullptr),m_pSpriteFullVersion(nullptr),m_pSpriteDec(nullptr),noAds(nullptr)
{
}
ShopScne::~ShopScne()
{

}
bool ShopScne::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CFLayer::init() )
    {
        return false;
    }
    
    IAPManager::getInstance()->m_fnAfterPurchase = CC_CALLBACK_1(ShopScne::purchaseFinishCallback, this);
    IAPManager::getInstance()->m_fnAfterRestore = CC_CALLBACK_2(ShopScne::AfterRestoreCallBack, this);
    
    Sprite* m_pSpriteBg = Sprite::create("shop/shop-bg.png");
    this->addToBgLayer(m_pSpriteBg);
    kAdapterScreen->setExactPosition(m_pSpriteBg, 480, 320);
    
    m_pSpriteAllCookie = Sprite::create("shop/allcookie.png");
    this->addToContentLayer(m_pSpriteAllCookie);
    kAdapterScreen->setExactPosition(m_pSpriteAllCookie, 190, 300);
    m_vSprite.pushBack(m_pSpriteAllCookie);
    m_pSpriteAllCookie->setVisible(false);
    
    Sprite* shadow = Sprite::create("shop/allcookie-shadow.png");
    m_pSpriteAllCookie->addChild(shadow,-1,1);
    shadow->setPosition(175, 30);
    shadow->setVisible(false);
    
    m_pSpriteFullVersion = Sprite::create("shop/fullversion.png");
    this->addToContentLayer(m_pSpriteFullVersion);
    kAdapterScreen->setExactPosition(m_pSpriteFullVersion, 490, 340);
    m_vSprite.pushBack(m_pSpriteFullVersion);
    
    Sprite* shadow1= Sprite::create("shop/fullversion-shadow.png");
    m_pSpriteFullVersion->addChild(shadow1,-1,1);
    shadow1->setPosition(170, 55);
    shadow1->setVisible(false);
    
    
    m_pSpriteDec = Sprite::create("shop/alldecortions.png");
    m_pSpriteDec->setVisible(false);
    this->addToContentLayer(m_pSpriteDec);
    kAdapterScreen->setExactPosition(m_pSpriteDec, 780, 340);
    m_vSprite.pushBack(m_pSpriteDec);
    Sprite* shadow2= Sprite::create("shop/alldecortions-shadow.png");
    m_pSpriteDec->addChild(shadow2,-1,1);
    shadow2->setPosition(130, 20);
    shadow2->setVisible(false);
    
    
    
    for (auto p: m_vSprite)
    {
        KSButton* btn = KSButton::create("shop/buy_btn.png");
        p->addChild(btn);
        btn->setTag(int(m_vSprite.getIndex(p)));
        btn->setPosition(Vec2(p->getContentSize().width/2-80,15));
        btn->setClickListener(CC_CALLBACK_1(ShopScne::btnClick, this));
        
    }

    
    noAds = KSButton::create("shop/noads-btn.png");
    noAds->setVisible(false);
    this->addToUILayer(noAds);
    kAdapterScreen->setExactPosition(noAds, 315, 100);
    noAds->setClickListener(CC_CALLBACK_1(ShopScne::btnClick, this));
    noAds->setTag(3);
    
    KSButton* restore = KSButton::create("shop/restore_btn.png");
    this->addToBgLayer(restore);
    kAdapterScreen->setExactPosition(restore, 100, 35 + 30,Vec2(0.5, 0.5),kBorderRight,kBorderBottom);
    restore->setClickListener(CC_CALLBACK_1(ShopScne::btnClick, this));
    restore->setTag(4);
    
    
    KSButton* back = KSButton::create("shop/back.png");
    this->addToUILayer(back);
    kAdapterScreen->setExactPosition(back, 80, 70,Vec2(0.5, 0.5),kBorderLeft,kBorderTop);
    back->setClickListener(CC_CALLBACK_1(ShopScne::btnClick, this));
    back->setTag(5);

    
    
    
    return  true;
}

void ShopScne::btnClick(KSButton* btn)
{
    log("btn--%d",btn->getTag());
    int tag = btn->getTag();
    
    btn->setEnabled(false);
    this->runAction(Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
    
        btn->setEnabled(true);
    
    }), NULL));
    
    switch (tag) {
        case 0://log("AllCookie");
        case 1://log("FullVersion");
        case 2://log("AllDec");
        case 3://log("NoAds");
        {
            if(!kIAPManager->isPackagePurchased(tag))
            {
                
                kIAPManager->purchase(tag);
                
            }
            else
            {
                Dialog* dialog= Dialog::create((void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL);
                dialog -> setAnchorPoint(Vec2(0,0));
                kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
                dialog->setContentText("You have already purchased this package!");
                dialog->setPositiveBtnText("");
                dialog->setNegativeBtnText("");
                dialog->setCallback(this);
                this->addChild(dialog, 9999);
            }
        }
            break;
        case 4:
        {
            log("Restore");
            kIAPManager->restore();
        }
            
            break;
        case 5:
        {
            log("Back");
            Director::getInstance()->popScene();
        }
            
            break;
            
        default:
            break;
    }
}
void ShopScne::purchaseFinishCallback(bool isPurchase)
{
    if(isPurchase){
        
        if (kIAPManager->isPackagePurchased("com.crazycatsmedia.cookiemaker.noads")) {
        
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        
        }
        
    }
}

void ShopScne::AfterRestoreCallBack(bool isRestore,bool isPurchase)
{
    
    
//    dialog -> setAnchorPoint(Vec2(0,0));
//    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
//    dialog->setContentText("You have already purchased this package!");
//    dialog->setPositiveBtnText("");
//    dialog->setNegativeBtnText("");
//    dialog->setCallback(this);
//    this->addChild(dialog, 9999);
    
   Dialog* dialog= Dialog::create((void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL);
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    
    
    if(isRestore){
        
        if (kIAPManager->isPackagePurchased("com.crazycatsmedia.cookiemaker.noads")) {
            
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
            
        }
        
    }
    
    if(isRestore)
    {
        if(isPurchase)
        {
            dialog->setContentText("Your content has been restored!");
        }
        else
        {
            dialog->setContentText("Sorry! It looks like you haven't purchased anything yet.");
            
        }
    }
    else
    {
        dialog->setContentText("Sorry, restore transaction failed !");
    }
    this->addToUILayer(dialog, 9999);


}
void ShopScne::onNegativeClick(void* type)
{
    
}
void ShopScne::onPositiveClick(void* type)
{
    
}
void ShopScne::closeBanner()
{
    if(kIAPManager->isShowAds()){
        
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
    }
}
void ShopScne::onEnter()
{
    CFLayer::onEnter();
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(AppResume, CC_CALLBACK_0(ShopScne::closeBanner, this));
    
    AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
//    CallFunc* rightappear = CallFunc::create([=]()
//    {
//        AppearManager::RightAppear(m_pSpriteDec,1,CallFunc::create([=](){m_pSpriteDec->getChildByTag(1)->setVisible(true);}));
//    });
    
    CallFunc* leftappear = CallFunc::create([=]()
    {
        m_pSpriteFullVersion->getChildByTag(1)->setVisible(true);
        AppearManager::RightAppear(m_pSpriteDec,1,CallFunc::create([=](){m_pSpriteDec->getChildByTag(1)->setVisible(true);}));
        AppearManager::LeftAppear(m_pSpriteAllCookie,1,CallFunc::create([=](){m_pSpriteAllCookie->getChildByTag(1)->setVisible(true);}));
    });
    
    CallFunc* bottomappear = CallFunc::create([=]()
    {
        AppearManager::BottomAppear(noAds);
    });
    
    AppearManager::TopAppear(m_pSpriteFullVersion,1,leftappear);
    
    runAction(Sequence::create(DelayTime::create(2),bottomappear,NULL));
}
void ShopScne::onExit()
{
    CFLayer::onExit();
    
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(AppResume);
    
    AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, true);
}