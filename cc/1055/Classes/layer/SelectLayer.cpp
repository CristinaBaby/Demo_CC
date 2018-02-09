//
//  SeceltLayer.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/15.
//
//

#include "SelectLayer.h"
REGIST_SECENE(SecletLayer)
#include "MakeSceneOne.h"
#include "MakeOtherScene.h"
#include "ShopScene.hpp"

string toolPath[6]={"chocolatechip","peanutbutter","original","chocolate","mint","strawberry"};

string toolFlavor[6]={"chocolatechips","peanutbutter","vanilla","chocolate","mint","strawberry"};

SecletLayer::SecletLayer()
{
    
}
SecletLayer::~SecletLayer()
{
    
}
bool SecletLayer::init()
{
    
    if(!BaseLayer::init())
        return false;
    //加载_studioLayer
    _studioLayer = StudioLayer::create("SeclecScene.csb");
    addChild(_studioLayer);
    
    //加载碗，盘子一类的工具
//    auto bowl = _studioLayer->getNodeByName<Sprite*>("bowl");
//    ActionHelper::showBackInOut(bowl, 0.5, ActionHelper::ShowDirection::show_from_left);
    
    for (int i = 0;i<6; i++) {
        
        auto tool = _studioLayer->getNodeByName<Sprite*>(toolPath[i].c_str());
        
        if (i < 3) {
//            ActionHelper::showBezier(tool, tool->getPosition(), ActionHelper::ShowDirection::show_from_left);
              ActionHelper::showBackInOut(tool,0.2*i, ActionHelper::ShowDirection::show_from_left);
        }else{
              ActionHelper::showBackInOut(tool,0.2*i, ActionHelper::ShowDirection::show_from_right);
//            ActionHelper::showBezier(tool, tool->getPosition(), ActionHelper::ShowDirection::show_from_right);
        
        }
        tool->setTag(i);
        m_vFlavor.pushBack(tool);
        auto lisenter = MoveLisenter::create();
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, tool);
        lisenter->setOrginOffset();
        
        
        lisenter->onTouchDown = CC_CALLBACK_2(SecletLayer::onTouchDown, this);
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)        
        if(i>3 && !kIAPManager->isPackagePurchased(FLAVOR_IAP_ID)){
        
            auto lock = Sprite::create("cocostudio/UI/decorate/lock.png");
            lock->setPosition(tool->getContentSize()/2);
            lock->setScale(0.7);
            tool->addChild(lock);
        
            m_vLock.pushBack(lock);
            
        }
#endif
        
        
    }
    
    
    auto title = Sprite::create("cocostudio/UI/tips/chooseflavor.png");
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(title);
    title->setTag(1001);
    
    
    return true;
}
void SecletLayer::onEnter()
{
    BaseLayer::onEnter();
    
    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("homeBg.mp3");
    
    if (IAPManager::getInstance()->isShowAds()) {
        
        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
        
    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(kIAPManager->isPackagePurchased(FLAVOR_IAP_ID)){
        for (auto lock:m_vLock) {
            lock->removeFromParent();
        }
    }
#endif
    string gameState = UserDefault::getInstance()->getStringForKey(GameState);
    if(gameState == GameStateThree){
    
        bool isShow = UserDefault::getInstance()->getBoolForKey("isShow", false);
        if (!isShow) {
            UserDefault::getInstance()->setBoolForKey("isShow", true);
            UserDefault::getInstance()->flush();
            
            PopDialogLayer* layer = PopDialogLayer::create(PopDialogType::allFlavor);
            layer->setAnchorPoint(Vec2::ZERO);
            this->addChild(layer);
            layer->setListener([=](KSPopLayer*, KSPopLayer::TouchEventType touch){
                if(touch == KSPopLayer::TouchEventType::ENDED){
                    
                    layer->removeFromParent();
                    if (this->getChildByTag(1001)) {
                        
                        this->getChildByTag(1001)->runAction(MoveBy::create(1.0,Vec2(0,-500)));
                        AudioHelp::getInstance()->playEffect("VoiceOver/choose a flavor.mp3");
                        
                    }
                }
            });
            layer->btnClose=[=](){
                
                layer->removeFromParent();
                if (this->getChildByTag(1001)) {
                    
                    this->getChildByTag(1001)->runAction(MoveBy::create(1.0,Vec2(0,-500)));
                    AudioHelp::getInstance()->playEffect("VoiceOver/choose a flavor.mp3");
                    
                }
            };
        }else{
        
            if (this->getChildByTag(1001)) {
                
                this->getChildByTag(1001)->runAction(MoveBy::create(1.0,Vec2(0,-500)));
                AudioHelp::getInstance()->playEffect("VoiceOver/choose a flavor.mp3");
                
            }
        
        }
    }
}
void SecletLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void SecletLayer::onExit()
{
    BaseLayer::onExit();
    
}
void SecletLayer::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    log("%s", FileUtils::getInstance()->getWritablePath().c_str());
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(MoveBy::create(1.0,Vec2(0,500)));
        
    }
    node->setLocalZOrder(100);
    int tag = node->getTag();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (!kIAPManager->isPackagePurchased(FLAVOR_IAP_ID)) {
        
        if (tag > 3) {
            
            Director::getInstance()->pushScene(ShopScne::createScene());
            return;
        }
        
    }
#endif
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    
    for (auto tool : m_vFlavor) {
        
        if (tool->getTag() == tag) {
            continue;
        }
        log("%d",tag);
        float detal = 1000;
        
        if (tool->getTag() < 3) {
            detal = -1000;
        }
        
        ActionUtils::moveByElasticIn(tool,Vec2(detal,0),1.0);
        
    }
    
    node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,node->getParent()->getContentSize()/2),ScaleTo::create(1.0,1.5),nullptr),DelayTime::create(2.0),CallFunc::create([=](){
    
         Director::getInstance()->getEventDispatcher()->setEnabled(true);
        
        if (tag < 2) {
            
            GameDataInstance->setFlavor(toolFlavor[tag].c_str());
            GameDataInstance->setGameType(GameTypeOne);
            GameDataInstance->setMold(MoldRound);
            //第一种游戏
            
            KitchenLayer::loading(KitchenType::kitchen_material,[=](){
                
                 Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,MakeSceneOne::createScene()));
                
            });

            
        }
        if(tag > 1){
        
            GameDataInstance->setFlavor(toolFlavor[tag].c_str());
            GameDataInstance->setGameType(GameTypeOther);
        
            KitchenLayer::loading(KitchenType::kitchen_material,[=](){
                
                Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,MakeOtherScene::createScene()));
                            
            });
        
        }
        
        
    }), NULL));
    
}
void SecletLayer::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    
}
void SecletLayer::onTouchUp(Node* node,MoveLisenter* lisenter)
{
}
void SecletLayer::touchEnd(ui::Widget* widget)
{
    
    
}