//
//  ShopLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#include "ShopLayer.h"
#include "SceneManager.h"
#include "IAPManager.h"
#include "AudioHelp.h"
#include "Global.h"
#include "ActionUtils.h"

#include "MySpriteButton.h"
static vector<string> IAPTypes={"shop_btn_christmas pack@2x.png","shop_btn_decoration@2x.png","shop_btn_cone@2x.png","shop_btn_fruit@2x.png",
    "shop_btn_restore@2x.png"};
string shopItemBg[4]={"alldecorations.png","allmolds.png","unlockall0.png","noads.png"};
Vec2 shopItemPos[4]={Vec2(238,399 + 50),Vec2(900,399 + 50),Vec2(566,214 + 150),Vec2(928,193)};
ShopLayer::ShopLayer()
{
    btnIndex = -1;
}
ShopLayer::~ShopLayer()
{
    
}

void ShopLayer::onEnter()
{
    BaseLayer::onEnter();
    showUI();
    showFont();
    _eventDispatcher->addCustomEventListener("closeAds", CC_CALLBACK_0(ShopLayer::shopCloaeAds, this));
    if (kIAPManager->isShowAds()) {
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
    }
}
void ShopLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}

void ShopLayer::onExit()
{
    BaseLayer::onExit();
    if (kIAPManager->isShowAds()) {
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, true);
    }
    _eventDispatcher->removeCustomEventListeners("closeAds");
//    _eventDispatcher->remo
}
void ShopLayer::shopCloaeAds()
{
    if(kIAPManager->isShowAds()){
        
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
    }
}
bool ShopLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        isFirstIn=true;
        
        initUI();
        isInit=true;
        
    }while(0);
    
    return true;
    
}
void ShopLayer::initData()
{
  
    for(auto i = IAPTypes.begin();i<IAPTypes.end();i++)
    {
        string buypath = (*i).c_str();
        Sprite* data = Sprite::create("images/shop_fav/"+buypath);
        m_vTypes.pushBack(data);
    }
}
void ShopLayer::initUI()
{
    kIAPManager->m_fnAfterPurchase = CC_CALLBACK_1(ShopLayer::purchaseFinishCallback, this);
    kIAPManager->m_fnAfterRestore = CC_CALLBACK_2(ShopLayer::AfterRestoreCallBack, this);

    Sprite* m_pBgSprite=Sprite::create("images/shop_fav/shop_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBgSprite, 480, 320);
    this->addToBackGroundLayer(m_pBgSprite, 3);
    
    for (int i = 0;i<4;i++) {
        
        auto btn = MySpriteButton::create(__String::createWithFormat("images/shop_fav/%s",shopItemBg[i].c_str())->getCString(),std::bind(&ShopLayer::btnClick, this,std::placeholders::_1));
        btn->setPosition(shopItemPos[i]);
        m_pBgSprite->addChild(btn);
        btn->setTag(i);
        m_vBtn.pushBack(btn);
        if (i==0) {
           btn->setPositionX(btn->getPositionX()-500);
        }else if(i==1){
            btn->setPositionX(btn->getPositionX()+500);
        }else if(i == 2){
        
            btn->setPositionY(btn->getPositionY()+100);
            btn->setScale(0);
        }else if(i == 3){
            btn->setOpacity(0);
        }
        
    }
    
    auto restore = MySpriteButton::create("images/shop_fav/restore.png",std::bind(&ShopLayer::restoreClick, this,std::placeholders::_1));
    kAdapterScreen->setExactPosition(restore, 0,0,Vec2(0,0),kBorderLeft,kBorderBottom);
    m_pBgSprite->addChild(restore);

    auto close = MySpriteButton::create("images/button/public_btn_back.png",std::bind(&ShopLayer::closeClick, this,std::placeholders::_1));
    kAdapterScreen->setExactPosition(close, 10,10,Vec2(0,1),kBorderLeft,kBorderTop);
    this->addChild(close,10);
    
    auto bear = Sprite::create("images/shop_fav/unlockall1.png");
    bear->setAnchorPoint(Vec2(0.5,0));
    bear->setPosition(m_vBtn.at(2)->getContentSize()/2);
    bear->setPosition(bear->getPosition()-Vec2(0,bear->getContentSize().height/2));
    m_vBtn.at(2)->addChild(bear);
    
    bear->runAction(RepeatForever::create(Sequence::create(RotateTo::create(1.0,10),CallFunc::create([=](){
    
    }),RotateTo::create(1.0,-10),CallFunc::create([=](){
    
    }),NULL)));
    
    auto unlockall = Sprite::create("images/shop_fav/unlockall2.png");
    unlockall->setAnchorPoint(Vec2::ZERO);
    m_vBtn.at(2)->addChild(unlockall);
    
    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/knifeParticl.plist");
    emitter1->setPosition(bear->getContentSize()/2);
    emitter1->setAutoRemoveOnFinish(true);
    emitter1->setDuration(-1);
    bear->addChild(emitter1, 30);
}
void ShopLayer::showFont()
{
    auto font = Sprite::create("images/shop_fav/shop.png");
    kAdapterScreen->setExactPosition(font,480,10 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(font);
    
    ActionUtils::moveByElasticOut(font,Vec2(0,-500));
    
}
void ShopLayer::showUI()
{
    ActionUtils::moveByElasticOut(m_vBtn.at(0),Vec2(500,0));
    ActionUtils::moveByElasticOut(m_vBtn.at(1),Vec2(-500,0));
    
    m_vBtn.at(2)->runAction(Sequence::create(Spawn::create(MoveBy::create(1.0,Vec2(0,-100)),ScaleTo::create(1.0,1.0), NULL), NULL));
    m_vBtn.at(3)->runAction(FadeIn::create(0.5));
}
void ShopLayer::btnClick(Ref* pSender)
{
    auto btn = dynamic_cast<MySpriteButton*>(pSender);
    int index = btn->getTag();
    if(!kIAPManager->isPackagePurchased(index))
    {
        
        kIAPManager->purchase(index);
        
    }
    else
    {
        
        Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
        kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
        
        dialog->setContentText("You have already purchased this package!");
        
        dialog->setPositiveBtnText("");
        dialog->setNegativeBtnText("");
        dialog->setCallback(this);
        this->addToUILayer(dialog, 9999);
    }


}
void ShopLayer::restoreClick(Ref* pSender)
{
    kIAPManager->restore();

}
void ShopLayer::closeClick(Ref* pSender)
{

    Director::getInstance()->popScene();

}
void ShopLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
//    btn->getTag()
    log("---%d",btn->getTag());
    if(type == ui::Widget::TouchEventType::ENDED&&btn->getTag() == 30)
    {
        SceneManager::getInstance()->popScene();
        AudioHelp::getInstance()->playDeleteEffect();
        
    }else if (type == ui::Widget::TouchEventType::ENDED&&btn->getTag()>90&& btn->getTag()<110)
    {
        log("---%d",btn->getTag()-100);
        int tag = btn->getTag()-100;
        btnIndex = tag;
        
        if(tag == 5){
        
            kIAPManager->purchase(5);
        
        }else{
            if(!kIAPManager->isPackagePurchased(tag))
            {
                
                kIAPManager->purchase(tag);
                
            }
            else
            {
                
                Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
                kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
                dialog->setContentText("You have already purchased this package!");
                dialog->setPositiveBtnText("");
                dialog->setNegativeBtnText("");
                dialog->setCallback(this);
                this->addChild(dialog, 9999);
            }
        
        }
    }
}
bool ShopLayer::checkOneUnLock()
{
    
    if(IAPManager::getInstance()->isPackagePurchased(2)&&IAPManager::getInstance()->isPackagePurchased(3)&&IAPManager::getInstance()->isPackagePurchased(4))
    {
    
        IAPManager::getInstance()->freePackge(1);
        
    
    }
    
    return false;

}
bool ShopLayer::checkAllUnLock()
{

    return false;
}
void ShopLayer::onScrolling(float offsetX,float offsetY)
{

    log("%f %f",offsetX,offsetY);
}
void ShopLayer::onScrollEnd(float offsetX,float offsetY)
{

}
void ShopLayer::onScrollDidZoom(cocos2d::extension::ScrollView* view)
{

}
void ShopLayer::onBuyCallback(int tag,MenuItem* menuItem,int index)
{
//    log("-----------IAP %d",index);
//
//    if(index == 4){
//        
//        kIAPManager->restore();
//        return;
//    }
//    
//

}
void ShopLayer::AfterRestoreCallBack(bool isRestore,bool isPurchase)
{
    
    Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    
    if(isRestore)
    {
        if(isPurchase)
        {
            dialog->setContentText("Your content has been restored!");
            
            //解锁mold
            if(kIAPManager->isPackagePurchased(1)){
                
                UserDefault::getInstance()->setBoolForKey(GummyMoldLock,false);
                UserDefault::getInstance()->flush();
            }
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
    this->addChild(dialog, 9999);


}
void ShopLayer::onNegativeClick(void* type)
{
    
}

void ShopLayer::onPositiveClick(void* type)
{
    
}

void ShopLayer::purchaseFinishCallback(bool isPurchase)
{
    log("Purchase is successful");
    if (isPurchase) {
        
        //解锁mold
        if(kIAPManager->isPackagePurchased(1)){
        
            UserDefault::getInstance()->setBoolForKey(GummyMoldLock,false);
            UserDefault::getInstance()->flush();
        }
        
        
        
    }
}

