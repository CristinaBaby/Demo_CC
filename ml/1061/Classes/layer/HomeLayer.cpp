//
//  HomeLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#include "HomeLayer.h"
#include "SettingLayer.h"
#include "../Depends/modules/CFSystemFunction.h"
#include "../Depends/managers/IAPManager.h"
#include "../Depends/utils/AudioHelp.h"
#include "ActionUtils.h"
#include "Global.h"
#include "MoregameBtn.h"
#include "SelectScene.h"
#include "ParabolaTo.h"
#include "ChangeLoading.h"
#include "AdLoadingLayerBase.h"
#include "PopDialogLayer.h"

HomeLayer::HomeLayer()
{
    m_bIsFirst = true;
}
HomeLayer::~HomeLayer()
{

}
bool HomeLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initUI();
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void HomeLayer::initUI()
{
    
//    ChangeLoading::loading(nullptr);
    
//    ChangeLoading* pop = ChangeLoading::create();
//    pop->setAnchorPoint(Vec2::ZERO);
//    this->addChild(pop,100);

//    ToolSprite* m_pLogo;

    m_pBg = Sprite::create("images/home/mainpage_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBg,480,320);
    this->addToBackGroundLayer(m_pBg,10);

    m_pStartBtn    = MySpriteButton::create(("images/home/play.png"), std::bind(&HomeLayer::btnClick, this,std::placeholders::_1));
    m_pMoreGameBtn = MySpriteButton::create(("images/home/mg.png"), std::bind(&HomeLayer::btnClick, this,std::placeholders::_1));
    m_pShopBtn     = MySpriteButton::create(("images/home/shop.png"), std::bind(&HomeLayer::btnClick, this,std::placeholders::_1));
    
    m_pStartBtn->setTag(eTagStart);
    m_pMoreGameBtn->setTag(eTagMoreGame);
    m_pShopBtn->setTag(eTagShop);
    
    kAdapterScreen->setExactPosition(m_pStartBtn, 480,320 - 50);
    kAdapterScreen->setExactPosition(m_pMoreGameBtn,10 -500,10,Vec2(1,0),kBorderRight,kBorderBottom);
    kAdapterScreen->setExactPosition(m_pShopBtn, 10-500 + m_pShopBtn->getContentSize().width,10,Vec2(1,0),kBorderRight,kBorderBottom);
    
    this->addToUILayer(m_pStartBtn,0);
    this->addToUILayer(m_pMoreGameBtn,0);
    this->addToUILayer(m_pShopBtn,0);
    
    m_pStartBtn->setScale(0);
    
    m_pLogo = ToolSprite::create("images/home/logo.png");
    m_pLogo->setDelegate(this);
    kAdapterScreen->setExactPosition(m_pLogo, 10,10 + 50,Vec2(0.5,0.5),kBorderLeft,kBorderTop);
    this->addToUILayer(m_pLogo,0);
    m_pLogo->setIsMove(false);
    m_pLogo->setPosition(m_pLogo->getPosition()+Vec2(m_pLogo->getContentSize().width/2,-m_pLogo->getContentSize().height/2));
    m_pLogo->setPositionY(m_pLogo->getPositionY()+1000);
    
    m_pBear1 = ToolSprite::create("images/home/bear0.png");
    m_pBear1->setDelegate(this);
    m_pBear1->setPosition(Vec2(982 - 50,166 - m_pBear1->getContentSize().height/2) + Vec2(0,m_pBear1->getContentSize().width*0.5));
    m_pBg->addChild(m_pBear1);
    m_pBear1->setIsMove(false);
    m_pBear1->setRotation(90);
    m_pBear1->setPosition(Vec2(1500,1500));
    m_pBear1->setAnchorPoint(Vec2(0.33,0.06));
    
    
    m_pBear2 = ToolSprite::create("images/home/bear1.png");
    m_pBear2->setDelegate(this);
    m_pBear2->setPosition(Vec2(764,166) + Vec2(0,m_pBear2->getContentSize().width*0.5));
    m_pBg->addChild(m_pBear2);
    m_pBear2->setIsMove(false);
    m_pBear2->setTag(1);
    m_pBear2->setRotation(90);
    m_pBear2->setPosition(Vec2(1500,1500));
    
    auto chooseParticle = ParticleSystemQuad::create("particle/bakeToolParticle.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->setVisible(true);
    chooseParticle->setPosition(m_pLogo->getContentSize()*.5);
    chooseParticle->setScale(1.5);
    m_pLogo->addChild(chooseParticle,-1);
}
void HomeLayer::onEnter()
{
    BaseLayer::onEnter();
    
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    
    if(m_bIsFirst){
    
        showBear();
        m_bIsFirst = false;
    }
    
    
    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("bg_music2.mp3");
    
    //首页不显示广告
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    
}
void HomeLayer::onEnterTransitionDidFinish()
{
    
    BaseLayer::onEnterTransitionDidFinish();
    
}
void HomeLayer::onExit()
{
    BaseLayer::onExit();

}
void HomeLayer::showBear()
{
    MoveTo* move1 = MoveTo::create(1.0,Vec2(982 - 50,166));
    RotateTo* rota1 = RotateTo::create(1.0,0);
    Spawn* sp1 = Spawn::create(move1,rota1, NULL);
    
    MoveTo* move2 = MoveTo::create(1.0,Vec2(764,166) + Vec2(0,m_pBear2->getContentSize().width*0.5));
    RotateTo* rota2 = RotateTo::create(1.0,0);
    Spawn* sp2 = Spawn::create(move2,rota2, NULL);
    
    
    ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("candy in.mp3");
        
    }));
    
    m_pBear1->runAction(Sequence::create(sp1, NULL));
    m_pBear2->runAction(Sequence::create(DelayTime::create(0.5),CallFunc::create([=](){
    
        ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("candy in.mp3");
            
        }));
        
    
    }),sp2,DelayTime::create(0.5),CallFunc::create([=](){
    
        showLogo();
    
    }),NULL));
}
void HomeLayer::showLogo()
{
   
    
    ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
    
         AudioHelp::getInstance()->playEffect("candy in.mp3");
    
    }));
    
    ActionUtils::moveByElasticOut(m_pLogo, Vec2(0,-1000),1.0,CallFunc::create([=](){
        
        
        showStartBtn();
    }));
}
void HomeLayer::showStartBtn()
{
    RotateBy* rota = RotateBy::create(1.0,360*3);
    ScaleTo*  scale = ScaleTo::create(1.0,1.0);
    Spawn* sp = Spawn::create(rota,scale, NULL);
    
    AudioHelp::getInstance()->playEffect("play_in.mp3");
    
    m_pStartBtn->runAction(Sequence::create(sp,DelayTime::create(0.5),CallFunc::create([=](){
    
       
        showUIBtn();
    
    }), NULL));
    
    m_pBear1->runAction(RepeatForever::create(Sequence::create(RotateTo::create(1.5,6),RotateTo::create(2.5,-9), NULL)));
}
void HomeLayer::showUIBtn()
{
    ActionUtils::moveByElasticOut(m_pMoreGameBtn, Vec2(-500,0),1.0,CallFunc::create([=](){
        
        
        
    }));

    ActionUtils::moveByElasticOut(m_pShopBtn, Vec2(-500,0),1.0,CallFunc::create([=](){
        
         btnAction();
        
    }));

}
void HomeLayer::btnAction()
{
    
    m_pStartBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1.0,1.0),ScaleTo::create(1.0,1.1), NULL)));
    
    m_pLogo->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.0,Vec2(0,20)),MoveBy::create(1.0,Vec2(0,-20)), NULL)));
    
}
void HomeLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    
    if(toolSprite->getTag() == 1){
    
        toolSprite->cancelEvent(true);
        
        AudioHelp::getInstance()->playEffect("choose.mp3");
        
        toolSprite->runAction(Sequence::create(JumpBy::create(1.0, Vec2(0,0),50,2),CallFunc::create([=](){
        
            toolSprite->cancelEvent(false);
        
        }),NULL));
    
        return;
    }
    
    toolSprite->cancelEvent(true);
    
    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    AudioHelp::getInstance()->playEffect("choose.mp3");
    toolSprite->runAction(Sequence::create(s1,s2,s3,CallFunc::create([=](){
        
        toolSprite->cancelEvent(false);
        
    }),nullptr));
}
void HomeLayer::btnClick(Ref *sender)
{
    MySpriteButton* btn = dynamic_cast<MySpriteButton*>(sender);
    
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    
    int tag = btn->getTag();
    switch (tag) {
        case eTagStart:
        {
            m_pStartBtn->stopAllActions();
            
            float timeScale = 1.2;
            ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
            ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
//            ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
//            ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
//            ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
            ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
//            ret = Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);

            
            m_pStartBtn->runAction(Sequence::create(lScale1, lScale2, lScale6,FadeOut::create(0.5),CallFunc::create([=](){
            
                m_pShopBtn->setEnable(false);
                m_pMoreGameBtn->setEnable(false);
                m_pShopBtn->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,-300)),1.0));
                m_pMoreGameBtn->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,-300)),1.0));
            
                m_pLogo->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,500))),CallFunc::create([=](){
                
                    m_pBear2->cancelEvent(true);
                    AudioHelp::getInstance()->playEffect("loading_decoration out.mp3");
                    
                    for (int i = 0;i<2;i++) {
                        
                        
                        ActionUtils::delayTimeAction(this,1.0*i,CallFunc::create([=](){
                        
                            AudioHelp::getInstance()->playEffect("choose.mp3");
                        
                        }));
                        
                    }
                    
                    
                    JumpTo* jump = JumpTo::create(1.5, m_pBg->convertToWorldSpace(Vec2(580 + 60,150)) + Vec2(0,m_pBear2->getContentSize().height/2),50,2);
                    
                    m_pBear2->runAction(Sequence::create(jump, ScaleTo::create(0.5,1.1),DelayTime::create(1.0),CallFunc::create([=](){
                    
                        
                        if(kIAPManager->isShowAds())
                        {
                            AdsLoadingLayer::m_bIsFrom = false;
                            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
                            AdLoadingLayerBase::loadingDoneCallback = []()
                            {
                                SceneChangeManager->enterPopFruitScene();
                            };
                        }
                        else
                        {
                            
                            ChangeLoading::loading([](){
                                
                                SceneChangeManager->enterPopFruitScene();
                                
                            });
//
//                            SceneChangeManager->enterPopFruitScene();
                        }
                        
                        
                        
                        
                    
                    }),NULL));
                
                
                }),nullptr));
                
            }), NULL));
        }
            break;
        case eTagMoreGame:
        {
            CFSystemFunction sys;
            sys.showMoreGame();
            
        }
            break;
        case eTagShop:
        {
            SceneChangeManager->enterShopScene();

        }
            break;
        default:
            break;
    }
}
//void HomeLayer::onBtnCallBack(Ref* sender)
//{
//    Node* node = dynamic_cast<Node*>(sender);
//    switch (node->getTag()) {
//        case eTagStart:
//        {
//            
//            m_pUIMenu->setEnabled(false);
//            JumpBy* jump = JumpBy::create(1.0, Vec2(0,0), 50, 2);
//            RotateBy* rota = RotateBy::create(1.0, 360*4);
//            ScaleTo* scale = ScaleTo::create(1.0, 0);
//            Spawn* sp = Spawn::create(rota,scale, NULL);
//            
//            AudioHelp::getInstance()->playEffect("jump.mp3");
//            
//            ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
//            
//            
//                 AudioHelp::getInstance()->playEffect("jump.mp3");
//            
//            }));
//            
//            m_pStartItemSprite->runAction(Sequence::create(jump,CallFunc::create([=](){
//                
//                AudioHelp::getInstance()->playEffect("spinmagnify.mp3");
//               
//                
//            }),sp,CallFunc::create([=](){
//            
//                ActionUtils::moveByElasticIn(m_pFavItemSprite, Vec2(1000,0));
//                ActionUtils::moveByElasticIn(m_pShopItemSprite, Vec2(-1000,0));
//                m_pStartItemSprite->setVisible(false);
//            
//            }), DelayTime::create(1.0),CallFunc::create([=](){
//            
//                if(kIAPManager->isShowAds())
//                {
//                    AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
//                    AdLoadingLayerBase::loadingDoneCallback = []()
//                    {
//                        SceneChangeManager->enterSelectScene();
//                    };
//                }
//                else
//                {
//                    SceneChangeManager->enterSelectScene();
//                }
//
//            
//            }),NULL));
// 
//                         CCLOG("startGame");
//            
//            AudioHelp::getInstance()->playStartEffect();
//            
//        }
//            break;
//        case eTagFavorite:
//        {
//                CCLOG("Favorite");
////            ShowIcon::hide();
//            

//            AudioHelp::getInstance()->playSelectEffect();
//            auto func = CallFunc::create([&](){
//                
//                m_pUIMenu->setEnabled(true);
//                
//            });
//            ActionUtils::delayTimeAction(this,0.5,func);
//        }
//            break;
//        case eTagMoreGame:
//        {
//            CCLOG("MoreGame");
//            AudioHelp::getInstance()->playSelectEffect();
//
//        }
//            break;
//        case eTagSetting:
//        {
//               CCLOG("Setting");
//           
//            AudioHelp::getInstance()->playSelectEffect();
//            SettingLayer* layer = SettingLayer::create();
//            layer->showInParent(this);
//            
//        }
//            break;
//        case eTagShop:
//        {
//            CCLOG("shop");
////            ShowIcon::hide();
//           
//            AudioHelp::getInstance()->playSelectEffect();
//
//            auto func = CallFunc::create([&](){
//                
//                
//                
//            });
//            ActionUtils::delayTimeAction(this,0.5,func);
//        }
//            break;
//        case eTagPrivacy:
//        {
//
//            CCLOG("eTagPrivacy");
//            CFSystemFunction sys;
//            sys.rateUs();
//          
//        }
//            break;
//        case eMiniGame:
//        {
//            b_IsFromHome = true;
//            m_pUIMenu->setEnabled(false);
//            if(TaskItemCtrInstance->isAllTaskItemUnlock()){
//            
//                //所有任务解锁  进入选择小游戏界面
//                 SceneChangeManager->enterSelectMiniGameScene();
//                
//            }else{
//                //选择礼物界面
//                SceneChangeManager->enterSelectGifiScene();
//            
//            }
//        }
//            break;
//        default:
//            break;
//    }

//}
// 监听Android返回键事件
void HomeLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText(ExitGame);
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addChild(dialog, 9999);
        }
    }
}
///
void HomeLayer::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void HomeLayer::onPositiveClick(void* type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CFSystemFunction sys;
    sys.endSession();
    Director::getInstance()->end();
#endif

}
