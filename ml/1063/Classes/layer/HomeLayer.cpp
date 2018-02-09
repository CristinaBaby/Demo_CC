//
//  HomeLayer.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/3/18.
//
//
#include "HomeLayer.h"
#include "MainScene.h"
#include "AdapterScreen.h"
#include "ChooseFood.h"
#include "LQ_adLoadingLayer.h"
#include "SoundPlayer.h"
#include "STSystemFunction.h"
#include "ShopLayer.h"
#include "SoundPlayer.h"
#define ZorderBg       1000
#define ZorderContent  1500
#define ZorderUI       2000

#define TAGBtnPlay     1
#define TAGBtnMore     2
#define TAGBtnShop     3
IMPLEMENT_COCOS2DX_CLASS(HomeLayer);
REGIST_SECENE(HomeLayer);
bool HomeLayer::init(){
    if(BaseHomeLayer::init()) {
        
        initView();
        initUI();
        return true;
    }
    return false;
}
void HomeLayer::onEnterTransitionDidFinish(){
    BaseHomeLayer::onEnterTransitionDidFinish();
}
void HomeLayer::onEnter()
{
    BaseHomeLayer::onEnter();
    SoundPlayer::getInstance()->playBackGroundMusic("sound/start page.mp3");
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
}
void HomeLayer::onExit()
{
    BaseHomeLayer::onExit();
}
void HomeLayer::initView()
{
    m_pBg = Sprite::create("cocostudio/png/Home page/bg.jpg");
    kAdapterScreen->setExactPosition(m_pBg,320,480);
    this->addChild(m_pBg,ZorderBg);
    
    m_pTable = Sprite::create("cocostudio/png/Home page/table.png");
    kAdapterScreen->setExactPosition(m_pTable,0,0,Vec2::ZERO,kBorderLeft,kBorderBottom);
    this->addChild(m_pTable,ZorderContent);
    
    m_pCake = Sprite::create("cocostudio/png/Home page/cake.png");
    m_pCake->setPosition(Vec2(0 + 150 - 40,312 - 50 - 50));
    m_pCake->setAnchorPoint(Vec2::ZERO);
    m_pTable->addChild(m_pCake);
    
    m_pCookies = Sprite::create("cocostudio/png/Home page/cookies.png");
    m_pCookies->setPosition(Vec2(307 - 40,237 - 50));
    m_pTable->addChild(m_pCookies);
    m_pCookies->setAnchorPoint(Vec2::ZERO);
    
    m_pFruitTart = Sprite::create("cocostudio/png/Home page/fruit-tart.png");
    m_pFruitTart->setPosition(Vec2(167 - 120 - 40,221 - 50));
    m_pTable->addChild(m_pFruitTart);
    m_pFruitTart->setAnchorPoint(Vec2::ZERO);
    
    cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("cocostudio/png/Home page/ACG/wedding/wedding/wedding.ExportJson");
    cocostudio::Armature *armature = cocostudio::Armature::create("wedding");
    kAdapterScreen->setExactPosition(armature,-150,150 - 100,Vec2(1,0),kBorderRight,kBorderBottom);
    //    armature->getAnimation()->play("Animation3");
    this->addChild(armature,ZorderContent-1);
    
    this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.0),CallFunc::create([=](){
        
        armature->getAnimation()->play("Animation3");
        
    }),DelayTime::create(2.0),NULL)));
    
    
    m_pTable->setPositionX(m_pTable->getPositionX()-1000);
    
}
void HomeLayer::initUI()
{
    
    kAdapterScreen->setUpScreenAdapter(640,960);
    
    m_pUiBar = Sprite::create("cocostudio/png/Home page/home_bg.png");
    kAdapterScreen->setExactPosition(m_pUiBar,320,150 - 100,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(m_pUiBar,ZorderUI);
    
    m_pBtnPlay = MySpriteButton::create("cocostudio/png/Home page/home_btn_play.png",CC_CALLBACK_1(HomeLayer::clickBtn, this));
    m_pBtnShop = MySpriteButton::create("cocostudio/png/Home page/home_btn_shop.png",CC_CALLBACK_1(HomeLayer::clickBtn, this));
    m_pBtnMore = MySpriteButton::create("cocostudio/png/Home page/home_btn_more.png",CC_CALLBACK_1(HomeLayer::clickBtn, this));
    
    
    m_pBtnPlay->setPosition(m_pUiBar->getContentSize()/2);
    m_pBtnShop->setPosition(m_pBtnPlay->getPosition()+Vec2(-200,0) + Vec2(-500,0));
    m_pBtnMore->setPosition(m_pBtnPlay->getPosition()+Vec2( 200,0) + Vec2( 500,0));
    //    m_pUiBar;
    
    m_pUiBar->addChild(m_pBtnPlay);
    m_pUiBar->addChild(m_pBtnMore);
    m_pUiBar->addChild(m_pBtnShop);
    
    m_pBtnShop->setVisible(false);
    m_pBtnMore->setVisible(false);
    m_pBtnPlay->setScale(0);
    
    m_pBtnPlay->setEnable(false);
    m_pBtnMore->setEnable(false);
    m_pBtnShop->setEnable(false);
    
    m_pUiBar->setPositionY(m_pUiBar->getPositionY()-1000);
    
    m_pBtnShop->setTag(TAGBtnShop);
    m_pBtnMore->setTag(TAGBtnMore);
    m_pBtnPlay->setTag(TAGBtnPlay);
    
    showTable();
    
}
void HomeLayer::showTable()
{
    m_pTable->runAction(Sequence::create(MoveBy::create(1.0,Vec2(1000,0)),DelayTime::create(0.5),CallFunc::create([=](){
        
        showUIBar();
        showLogo();
        
    }),nullptr));
}
void HomeLayer::showUIBar()
{
    
    m_pUiBar->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(0,1000)),1.0),CallFunc::create([=](){
        
        //        showUIBar();
        SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
        showUI();
        
    }),nullptr));
}
void HomeLayer::showLogo()
{
    m_pLogo = Sprite::create("cocostudio/png/Home page/home_logo.png");
    kAdapterScreen->setExactPosition(m_pLogo,320,0 - 1000,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(m_pLogo,ZorderUI);
    SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
    m_pLogo->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(0,-1000)),1.0),CallFunc::create([=](){
        
        //        showUIBar();
        
        
    }),nullptr));
    
    
}
void HomeLayer::showUI()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    m_pBtnShop->setVisible(true);
#endif
    m_pBtnMore->setVisible(true);
    
    m_pBtnShop->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(2.0,Vec2(500,0)),1.0),nullptr));
    m_pBtnMore->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(2.0,Vec2(-500,0)),1.0),nullptr));
    m_pBtnPlay->runAction(Sequence::create(DelayTime::create(0.8),Spawn::create(ScaleTo::create(1.0,1.0),RotateBy::create(1.0,360*3), NULL),CallFunc::create([=](){
        
        m_pBtnPlay->setEnable(true);
        m_pBtnMore->setEnable(true);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
        m_pBtnShop->setEnable(true);
#endif
        
        m_pBtnPlay->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1.5,1.1),ScaleTo::create(1.5,1.0), NULL)));
        //        m_pLogo->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.5,Vec2(0,40)),MoveBy::create(1.5,Vec2(0,-40)), NULL)));
        
    }), NULL));
    
    
}
void HomeLayer::clickBtn(Ref* pSender)
{
    auto btn = dynamic_cast<MySpriteButton*>(pSender);
    switch (btn->getTag()) {
        case TAGBtnPlay:
        {
            m_pBtnPlay->stopAllActions();
            m_pBtnPlay->setEnable(false);
            
            SoundPlayer::getInstance()->playEffect("sound/play button_spin magnify.mp3");
            m_pBtnPlay->runAction(Sequence::create(Spawn::create(ScaleTo::create(1.0,0),RotateBy::create(1.0,360*4), NULL),CallFunc::create([=](){
                
                m_pBtnShop->setEnable(false);
                m_pBtnMore->setEnable(false);
                m_pBtnShop->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(-500,0)),1.0));
                m_pBtnMore->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(0.5,Vec2(500,0)),1.0),CallFunc::create([=](){
                    SoundPlayer::getInstance()->playEffect("sound/ingredients_out.mp3");
                    m_pUiBar->runAction(EaseElasticIn::create(MoveBy::create(1.0,Vec2(0,-1000)),1.0));
                    
                    
                }),nullptr));
                
            }), DelayTime::create(4.0),CallFunc::create([=](){
                
                
                //next
                //                Director::getInstance()->replaceScene(ChooseFood::createScene());
                
                auto  _endFunc = [this](){
                    LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                    Director::getInstance()->pushScene(TransitionFade::create(0.8,ChooseFood::createScene(),Color3B::WHITE));
                    
                };
                if(IAPManager::getInstance()->isShowAds()){
                    LQ_adLoadingLayer::loadingDoneCallback = _endFunc;
                    LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(false);
                    AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
                }
                else
                    _endFunc();
                
                
                
                
            }),NULL));
            
        }
            break;
        case TAGBtnMore:
        {
            
            SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
            STSystemFunction stf;
            stf.showMoreGame();
        }
            break;
        case TAGBtnShop:
        {
            
            SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
            addChild(ShopLayer::create(),3000);
        }
            break;
        default:
            break;
    }
    
}