
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"

static  Vec2 gKidPos[] = {
    Vec2(170,225),
    Vec2(810,180),
    Vec2(700,260),
    Vec2(600,280),
};
HomeScene::HomeScene()
{
    m_bShowMoreGame = false;
}

HomeScene::~HomeScene()
{
    
}
bool HomeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    DecorateManager::getInstance()->reset();
    //    bg
    Sprite* pBg = Sprite::create(localPath("start_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    
    Sprite* pLogo = Sprite::create(localPath("logo.png"));
    this->addChildToUILayer(pLogo);
    pLogo->setLocalZOrder(1);
    CMVisibleRect::setPositionAdapted(pLogo, 320, 700+visibleSize.height/2);
    
    
    Sprite* pCart0 = Sprite::create(localPath("cart2.png"));
    this->addChildToContentLayer(pCart0);
    CMVisibleRect::setPositionAdapted(pCart0, -30-visibleSize.width, 100-200);
    
    Sprite* pCart1 = Sprite::create(localPath("cart1.png"));
    this->addChildToContentLayer(pCart1);
    
    AudioHelp::getInstance()->playBackGroundMusic("bg.mp3");
    
    CMVisibleRect::setPositionAdapted(pCart1, 650+visibleSize.width, 100-200);
    
    pCart0->runAction(Sequence::create(DelayTime::create(1),
                                       EaseBackOut::create(MoveBy::create(0.8, Vec2(visibleSize.width, 200))), NULL));
    
    pCart1->runAction(Sequence::create(DelayTime::create(1),
                                       EaseBackOut::create(MoveBy::create(0.8, Vec2(-visibleSize.width, 200))), NULL));
    
    pLogo->runAction(Sequence::create(DelayTime::create(2),
                                       EaseBounceOut::create(MoveBy::create(0.8, Vec2(0, -visibleSize.height/2))),
                                      CallFunc::create([=](){
        pLogo->runAction(RepeatForever::create(Spawn::create(JumpBy::create(6, Vec2::ZERO, 20, 1),
                                                             Sequence::create(ScaleTo::create(3, 1,0.95),
                                                                              ScaleTo::create(3, 1), NULL), NULL)));
        m_pGameUI->showHomeUILayout();
        
        
        Sprite* pPlayBg = Sprite::create(localPath("btn_play0.png"));
        this->addChildToContentLayer(pPlayBg);
        CMVisibleRect::setPositionAdapted(pPlayBg, 320, 390);
        pPlayBg->setScale(0.2);
        pPlayBg->runAction(Spawn::create(ScaleTo::create(0.5, 1),
                                         RotateBy::create(0.5, 360),
                                         CallFunc::create([=](){
            pPlayBg->runAction(RepeatForever::create(RotateBy::create(3, 360)));
        }), NULL));
    }), NULL));
    
    m_bShowAds = false;
    GameDataManager::getInstance()->m_nDecorateStep= 0;
    if (!UserDefault::getInstance() -> getBoolForKey("removeAds")) {
        AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
    }
    return true;
}

void HomeScene::onEnter()
{
    ExtensionScene::onEnter();
    m_bShowAds = false;
    
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->removeAds(kTypeBannerAds);
        
    }
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_HOME);
}
void HomeScene::onExit()
{
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_HOME);
    ExtensionScene::onExit();
}
void HomeScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    btn->setEnabled(false);
    if (tag!=GameUILayoutLayer::eUIButtonTagHomeStart) {
        this->runAction(Sequence::create(DelayTime::create(0.3),
                                         CallFunc::create([=]()
                                                          {
                                                              btn->setEnabled(true);
                                                          }),
                                         NULL));
    }
    switch (tag) {
        case GameUILayoutLayer::eUIButtonTagHomeStart:
        {
            AudioHelp::getInstance()->playStartEffect();
            SceneManager::replaceTheScene<ChoosePackageScene>();
            
        }
            break;
        case GameUILayoutLayer::eUIButtonTagHomeRestore:
        {
            ShopAdapter::getInstance()->requestRestore();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagHomeFav:
        {
        }
            break;
        case GameUILayoutLayer::eUIButtonTagHomeMore:
        {
            m_bShowMoreGame = true;
            STSystemFunction st;
            st.showMoreGame();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagHomeShop:
        {
//            SceneManager::pushTheScene<ShopScene>();
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss(false);
        }
            break;
            
        default:
            break;
    }
}

#pragma mark - initData
void HomeScene::_initData()
{
    setExPath("content/start/");
    
}

void HomeScene::_showParticle()
{
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dress.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setLocalZOrder(5);
    pParticle->setPosition(Vec2(0, visibleSize.height*0.8));
    
    AudioHelp::getInstance()->playEffect("particles_sparkle.mp3");
    pParticle->runAction(RepeatForever::create(Sequence::create(Spawn::create(MoveBy::create(10, Vec2(visibleSize.width+100, 0)),
                                                                              Sequence::create(EaseBackOut::create(MoveBy::create(1, Vec2(0, 30))),
                                                                                               DelayTime::create(1),
                                                                                               EaseBackOut::create(MoveBy::create(2, Vec2(0, -60))),
                                                                                               DelayTime::create(1),
                                                                                               EaseBackOut::create(MoveBy::create(2, Vec2(0, 60))),
                                                                                               DelayTime::create(1),
                                                                                               EaseBackOut::create( MoveBy::create(1, Vec2(0, -30))),
                                                                                               NULL),
                                                                              NULL),
                                                                DelayTime::create(2),
                                                                CallFunc::create([=]()
                                                                                 {
                                                                                     pParticle->setPosition(Vec2(0, visibleSize.height*0.8));
                                                                                     AudioHelp::getInstance()->playEffect("particles_sparkle.mp3");
                                                                                 }),
                                                                NULL)));
    
}

void HomeScene::onKeyBackClicked()
{
    if (m_bShowMoreGame) {
        m_bShowMoreGame = false;
        return;
    }
    if (Director::getInstance()->getRunningScene()->getChildByTag(9999)) {
        Director::getInstance()->getRunningScene()->getChildByTag(9999)->removeFromParent();
        return;
    }
    Dialog* dialog=Dialog::create(Size(497,355), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
    dialog->setContentText("Are you sure you want to quit the game?");
    Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
    dialog->setPosition(Director::getInstance()->getVisibleOrigin());
    dialog->onPositiveClick = [=](void*){
//        STSystemFunction st;
//        st.endAnalytics();
        Analytics::getInstance()->endSession();
        Director::getInstance()->end();
    };
    
}