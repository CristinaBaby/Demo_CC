//
//  HomeScene.cpp
//  Test
//
//  Created by liqiang on 16/4/22.
//
//

//#def 

#include "HomeScene.h"
#include  "AdapterScreen.h"
#include "MakeSceneOne.h"
#include "MakeOtherScene.h"
#include "SelectLayer.h"
#include "AudioHelp.h"
#include "ShopScene.hpp"
#include "AdsLoadingLayer.h"
#include "SSCInternalLibManager.h"

//REGIST_SECENE(HomeScene)

Scene* HomeScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HomeScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void HomeScene::onExit()
{
    BaseLayer::onExit();
    GameDataInstance->setIsMoreGame(false);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(AppResume);
}

void HomeScene::onEnter()
{
    BaseLayer::onEnter();
    
    
    log("HomeScene::onEnter()");
    
    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("homeBg.mp3");

    
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(AppResume, CC_CALLBACK_0(HomeScene::appResume, this));
    
    
    if(!m_bIsFirst){
    
        m_bIsFirst = true;
        CallFunc* next = CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            AppearManager::LeftAppear(m_pBtnMore);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            m_pBtnShop->setVisible(true);
#endif
            
            Vec2 pos = m_pBtnShop->convertToWorldSpace(Vec2::ZERO);
            
            float m_foffSetX = Director::getInstance()->getVisibleSize().width-pos.x+m_pBtnShop->getContentSize().width/2;
            
            
            MoveBy* move = MoveBy::create(1.0, Vec2(-m_foffSetX, 0));
            
            m_pBtnShop->runAction(Sequence::create(MoveBy::create(0, Vec2(m_foffSetX,0)),EaseBackInOut::create(move),CallFunc::create([=](){
                
                m_pBtnPlay->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.35, Vec2(0,10)), MoveBy::create(0.35, Vec2(0,-20)),MoveBy::create(0.35, Vec2(0,20)),MoveBy::create(0.35, Vec2(0,-10)),DelayTime::create(2),NULL)));
                
            }), NULL));
            
            
//            AppearManager::RightAppear(m_pBtnShop,1,CallFunc::create([=](){
//                
//                m_pBtnPlay->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.35, Vec2(0,10)), MoveBy::create(0.35, Vec2(0,-20)),MoveBy::create(0.35, Vec2(0,20)),MoveBy::create(0.35, Vec2(0,-10)),DelayTime::create(2),NULL)));
//                
//            }));
        });
        
        CallFunc* play = CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            AppearManager::BottomAppear(m_pBtnPlay);
            
        });
        
        CallFunc* call = CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            AppearManager::LeftAppear(m_pSpriteCookie);
            
            
        });
        
        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
        AppearManager::TopAppear(m_pSpriteLogo,1,call);
        runAction(Sequence::create(DelayTime::create(2),play,DelayTime::create(1),next,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("VoiceOver/so many yummy cookies.mp3");
            
        }), NULL));
    }
}
void HomeScene::appResume()
{
    log("HomeScene::appResume()");
    if (GameDataInstance->getIsMoreGame()) {
        
        ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
        
            log("setIsMoreGame");
            GameDataInstance->setIsMoreGame(false);
            
        }));
    
    }
}
HomeScene::HomeScene():m_pBtnPlay(nullptr),m_pBtnMore(nullptr),m_pBtnRate(nullptr),m_pBtnShop(nullptr),m_pSpriteCookie(nullptr),m_pSpriteLogo(nullptr),m_pSpriteTapeYellow(nullptr),m_pSpriteTapeGreen(nullptr),m_bIsFirst(false)
{
    
}
HomeScene::~HomeScene()
{
    
}
bool HomeScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    setAndroidKeyType(DialogFuncType::kDialogExit);
    
//  RewardLayer* layer = RewardLayer::create(RewardType::typeDec);
//  this->addChild(layer,100);
    
//  PopDialogLayer* layer = PopDialogLayer::create(newFlavor);
//  layer->setAnchorPoint(Vec2::ZERO);
//  this->addChild(layer,100);
    
    m_pBgLayer = Layer::create();
    m_pBgLayer->setPosition(Vec2::ZERO);
    this->addChild(m_pBgLayer,0);
    
    m_pContentLayer = Layer::create();
    m_pContentLayer->setPosition(Vec2::ZERO);
    this->addChild(m_pContentLayer,50);
    
    m_pUILayer = Layer::create();
    m_pUILayer->setPosition(Vec2::ZERO);
    this->addChild(m_pUILayer,100);
    
    
    Sprite* m_pBg = Sprite::create("home/home_bg.jpg");
    this->addToBgLayer(m_pBg);
    kAdapterScreen->setExactPosition(m_pBg, 480, 320);
    
    m_pBtnPlay = KSButton::create("home/play-btn.png");
    m_pBtnPlay->setVisible(false);
    m_pBtnPlay->setClickListener(CC_CALLBACK_1(HomeScene::btnClick, this));
    this->addToUILayer(m_pBtnPlay);
    m_pBtnPlay->setTag(1);
    kAdapterScreen->setExactPosition(m_pBtnPlay, 480, 120);
    
    
    m_pBtnMore = KSButton::create("home/more-btn.png");
    m_pBtnMore->setVisible(false);
    m_pBtnMore->setClickListener(CC_CALLBACK_1(HomeScene::btnClick, this));
    this->addToUILayer(m_pBtnMore);
    m_pBtnMore->setTag(2);
    kAdapterScreen->setExactPosition(m_pBtnMore, 100, 100,Vec2(0.5,0.5),kBorderLeft,kBorderBottom);
    
    
    m_pBtnRate = KSButton::create("home/rateus-btn.png");
    m_pBtnRate->setClickListener(CC_CALLBACK_1(HomeScene::btnClick, this));
    this->addToUILayer(m_pBtnRate);
    m_pBtnRate->setTag(3);
    kAdapterScreen->setExactPosition(m_pBtnRate, 110, 50,Vec2(0.5,0.5),kBorderLeft,kBorderTop);
    
    auto btn_privacy = KSButton::create("cocostudio/UI/startpage/pp.png");
    this->addToUILayer(btn_privacy);
    kAdapterScreen->setExactPosition(btn_privacy, 140, 50,Vec2(0.5,0.5),kBorderRight,kBorderTop);
    btn_privacy->setTag(1000);
    btn_privacy->setClickListener(CC_CALLBACK_1(HomeScene::btnClick, this));
    
    m_pBtnShop = KSButton::create("home/shop-btn.png");
    m_pBtnShop->setVisible(false);
    m_pBtnShop->setClickListener(CC_CALLBACK_1(HomeScene::btnClick, this));
    this->addToUILayer(m_pBtnShop);
    m_pBtnShop->setTag(4);
    kAdapterScreen->setExactPosition(m_pBtnShop, 100, 90,Vec2(0.5,0.5),kBorderRight,kBorderBottom);

    Sprite* m_pSpriteTable = Sprite::create("home/table.png");
    this->addToContentLayer(m_pSpriteTable);
    kAdapterScreen->setExactPosition(m_pSpriteTable, 480, 320);
    
    m_pSpriteCookie = Sprite::create("home/cookie.png");
    m_pSpriteCookie->setVisible(false);
    this->addToContentLayer(m_pSpriteCookie);
    kAdapterScreen->setExactPosition(m_pSpriteCookie, 430, 240);
    
    Sprite* m_pSpriteTape  = Sprite::create("home/daizi0.png");
    this->addToContentLayer(m_pSpriteTape);
    kAdapterScreen->setExactPosition(m_pSpriteTape, 480, 600+30);
    
    m_pSpriteTapeYellow = Sprite::create("home/daizi2.png");
    this->addToContentLayer(m_pSpriteTapeYellow);
    kAdapterScreen->setExactPosition(m_pSpriteTapeYellow, 250, 670+30,Vec2(0.5,1));
    m_pSpriteTapeYellow->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(2, 1, 0.7), ScaleTo::create(1, 1, 1),NULL)));
    
    m_pSpriteTapeGreen = Sprite::create("home/daizi1.png");
    this->addToContentLayer(m_pSpriteTapeGreen);
    kAdapterScreen->setExactPosition(m_pSpriteTapeGreen, 800, 670+30,Vec2(0.5,1));
    m_pSpriteTapeGreen->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1, 0.7), ScaleTo::create(2, 1, 1),NULL)));
    
    m_pSpriteLogo = Sprite::create("home/logo.png");
    m_pSpriteLogo->setVisible(false);
    this->addToContentLayer(m_pSpriteLogo);
    kAdapterScreen->setExactPosition(m_pSpriteLogo, 480, 500);
    
    
    
    Sprite* jie = Sprite::create("home/balloon0_.png");
    this->addToBgLayer(jie);
    kAdapterScreen->setExactPosition(jie, 50, 200,Vec2(0.5, 0));
    
    Sprite* b = Sprite::create(__String::createWithFormat("home/balloon%d.png",0)->getCString());
    BalloonShake(b, 1, 15);
    this->addToBgLayer(b);
    kAdapterScreen->setExactPosition(b, 50, 200,Vec2(0.5, 0));
    
    Sprite* b1 = Sprite::create(__String::createWithFormat("home/balloon%d.png",1)->getCString());
    BalloonShake(b1, 0.7, 10);
    this->addToBgLayer(b1);
    kAdapterScreen->setExactPosition(b1, 50, 200,Vec2(0.5, 0));
    
    Sprite* b2 = Sprite::create(__String::createWithFormat("home/balloon%d.png",2)->getCString());
    BalloonShake(b2, 0.5, 9);
    this->addToBgLayer(b2);
    kAdapterScreen->setExactPosition(b2, 50, 200,Vec2(0.5, 0));
    
    ///
    
    Sprite* jie1 = Sprite::create("home/balloon3_.png");
    this->addToBgLayer(jie1);
    kAdapterScreen->setExactPosition(jie1, 940, 250,Vec2(0.5, 0));
    
    Sprite* b3 = Sprite::create(__String::createWithFormat("home/balloon%d.png",3)->getCString());
    BalloonShake(b3, 1, 15);
    this->addToBgLayer(b3);
    kAdapterScreen->setExactPosition(b3, 940, 250,Vec2(0.5, 0));
    
    
    Sprite* b4 = Sprite::create(__String::createWithFormat("home/balloon%d.png",4)->getCString());
    BalloonShake(b4, 0.7, 10);
    this->addToBgLayer(b4);
    kAdapterScreen->setExactPosition(b4, 940, 250,Vec2(0.5, 0));
    
    
    Sprite* b5 = Sprite::create(__String::createWithFormat("home/balloon%d.png",5)->getCString());
    BalloonShake(b5, 0.5, 9);
    this->addToBgLayer(b5);
    kAdapterScreen->setExactPosition(b5, 940, 250,Vec2(0.5, 0));

    
    
    return  true;
}
void HomeScene::addToBgLayer(Node* node,int zorder ,int tag)
{
    m_pBgLayer->addChild(node,zorder,tag);
}
void HomeScene::addToContentLayer(Node* node,int zorder,int tag)
{
    m_pContentLayer->addChild(node,zorder,tag);
}
void HomeScene::addToUILayer(Node* node,int zorder,int tag)
{
    m_pUILayer->addChild(node,zorder,tag);
}
void HomeScene::BalloonShake(Node* n ,float time ,float angle)
{
       n->runAction(RepeatForever::create(Sequence::create(RotateBy::create(time, -angle),RotateBy::create(2*time, 2*angle),RotateBy::create(time, -angle), NULL)));
}
void HomeScene::btnClick(KSButton* btn)
{
    log("btn--%d",btn->getTag());
    
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    
    switch (btn->getTag()) {
        case 1000:
            Application::getInstance()->openURL("http://www.crazycatsmedia.com/privacy-policy/");
            break;
        case 1:{
        
            log("Play");
            
            AudioHelp::getInstance()->playEffect("Homebutton.mp3");
            
            btn->setEnabled(false);
            AppearManager::LeftDisAppear(m_pBtnMore);
            AppearManager::RightDisAppear(m_pBtnShop,1.0,CallFunc::create([=](){
                
                if(IAPManager::getInstance()->isShowAds()){
                
                    AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
                    AdLoadingLayerBase::loadingDoneCallback=[=](){
                        
                        goToNext();
                        
                    };
                    
                    
                }else{
                
                    goToNext();
                        
                }
            }));
            
        }
            break;
        case 2:
        {
//            m_bIsShowReturn = true;
            log("More");
//            CFSystemFunction sys;
//            sys.showMoreGame();
            SSCInternalLibManager::getInstance()->showMoreGames();
            GameDataInstance->setIsMoreGame(true);
            
            
         }
            break;
        case 3:
        {
            log("Rate");
//            CFSystemFunction sys;
//            sys.rateUs();
            SSCInternalLibManager::getInstance()->rateUs();
            
            
        }
            break;
        case 4:
        {
            log("Shop");
//            ShopScne* shop = ;
            btn->setEnabled(false);
            Director::getInstance()->pushScene(ShopScne::createScene());
            
            ActionUtils::delayTimeAction(this,2.0,CallFunc::create([=](){
            
                btn->setEnabled(true);
            
            }));
            
        }
            break;
        default:
            break;
    }

    
}
void HomeScene::goToNext()
{
    
    string gameState = UserDefault::getInstance()->getStringForKey(GameState,GameStateOne);
    
    //第一种游戏状态
    if (gameState == GameStateOne) {
        
        UserDefault::getInstance()->setStringForKey(GameState,GameStateOne);
        UserDefault::getInstance()->flush();
        
        GameDataInstance->setGameType(GameTypeOne);
        GameDataInstance->setFlavor(FlavorTypeChololateChip);
        GameDataInstance->setMold(MoldRound);
        KitchenLayer::loading(KitchenType::kitchen_material,[=](){
            
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,MakeSceneOne::createScene()));
            
        });
        
    }
    //第二种游戏状态
    else if(gameState == GameStateTwo){
        
        GameDataInstance->setPopDialogType(PopDialogTypeNewFlavor);
        GameDataInstance->setFlavor(FlavorTypeVanilla);
        GameDataInstance->setGameType(GameTypeOther);
        KitchenLayer::loading(KitchenType::kitchen_material,[=](){
            
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,MakeOtherScene::createScene()));
            
        });
        
    }else if(gameState == GameStateThree){
        
        //GameDataInstance->setPopDialogType(PopDialogTypeAllFlavor);
        KitchenLayer::loading(KitchenType::kitchen_choose,[=](){
            
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0,SecletLayer::createScene()));
            
        });
    }
    

}
