
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"
#include "SSCMoreGameButton.h"

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
    Sprite* pBg = Sprite::create(localPath("start_bg0.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pBg2 = Sprite::create(localPath("start_bg1.jpg"));
    pBg2->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg2);
    pBg2->setOpacity(0);
    
    Sprite* pLogo = Sprite::create(localPath("logo.png"));
    this->addChildToUILayer(pLogo);
    pLogo->setOpacity(0);
    pLogo->setScale(0.1);
    pLogo->setLocalZOrder(1);
    
    Sprite* pSnowCone = Sprite::create(localPath("snowcone.png"));
    this->addChildToUILayer(pSnowCone);
    CMVisibleRect::setPosition(pSnowCone, 320, 340-visibleSize.height);
    pSnowCone->setScale(0.3);
    pSnowCone->setLocalZOrder(2);
    
    for (int i = 0; i<5; i++) {
        std::stringstream ostr;
        ostr<<"cocos/girl/icegirl"<<i;
        ArmatureDataManager::getInstance()->addArmatureFileInfo(ostr.str()+".png", ostr.str()+".plist", "cocos/girl/icegirl.ExportJson");
    }
    Armature* lArmature = Armature::create("icegirl");
    this->addChildToUILayer(lArmature);
    lArmature->setScale(0.3);
    lArmature->setLocalZOrder(1);
    
    ArmatureAnimation* lArmation = lArmature->getAnimation();
    lArmation->play("fly",-1,1);
    CMVisibleRect::setPositionAdapted(lArmature, 130-visibleSize.width, 600+300,kBorderLeft);
    lArmature->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, -300))),
                                          DelayTime::create(1),
                                          CallFunc::create([=]()
                                                           {
                                                               lArmation->stop();
                                                               lArmation->play("magic1", -1, 0);
                                                               AudioHelp::getInstance()->playEffect("abcerbra.mp3");
                                                           }),
                                          DelayTime::create(1),
                                          CallFunc::create([=]()
                                                           {
                                                               ParticleSystemQuad* pParticleSnow = ParticleSystemQuad::create("particle/snow0.plist");
                                                               this->addChildToUILayer(pParticleSnow);
                                                               pParticleSnow->setPosition(Vec2(visibleSize.width/2, visibleSize.height+10));
                                                               
                                                               ParticleSystemQuad* pParticleSnow2 = ParticleSystemQuad::create("particle/snow1.plist");
                                                               this->addChildToUILayer(pParticleSnow2);
                                                               pParticleSnow2->setPosition(Vec2(visibleSize.width/2, visibleSize.height+10));

                                                               ParticleSystemQuad* pParticleSnow3 = ParticleSystemQuad::create("particle/homesnow.plist");
                                                               this->addChildToUILayer(pParticleSnow3);
                                                               pParticleSnow3->setPosition(Vec2(visibleSize.width/2, visibleSize.height+10));
                                                           }), NULL));
    
    lArmation->setMovementEventCallFunc([=](Armature * arm, MovementEventType type, const std::string &movementID){
        if (type==MovementEventType::COMPLETE) {
            if (type == MovementEventType::COMPLETE) {
                if(movementID=="magic1"){
                    AudioHelp::getInstance()->playEffect("genie_magic.mp3");
                    
                    Vec2 point = CocoStudioHelper::getBoneWorlPoint(arm, "effect3");
                    point = pLogo->getParent()->convertToNodeSpace(point);
                    pLogo->setPosition(point);
                    
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/home_light0.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setPosition(point);
                    
                    ParticleSystemQuad* pParticle2 = ParticleSystemQuad::create("particle/home_light1.plist");
                    this->addChildToUILayer(pParticle2);
                    pParticle2->setPosition(point);
                    
                    lArmation->play("magic2", -1, 1);
                    pBg2->runAction(Sequence::create(DelayTime::create(0.5),
                                                     FadeIn::create(3),
                                                     CallFunc::create([=]()
                                                                      {
                                                                          m_pGameUI->showHomeUILayout();
                                                                          AudioHelp::getInstance()->playEffect("snowcone_out.mp3");
                                                                          pSnowCone->runAction(Spawn::create(EaseBackOut::create(MoveBy::create(0.5, Vec2(0, visibleSize.height))),
                                                                                                             ScaleTo::create(0.4, 1), NULL));
                                                                          
                                                                          Vec2 point = CocoStudioHelper::getBoneWorlPoint(arm, "effect3");
                                                                          point = pLogo->getParent()->convertToNodeSpace(point);
                                                                          pLogo->setPosition(point);
                                                                          pLogo->runAction(Sequence::create(DelayTime::create(1),
                                                                                                            FadeIn::create(0.3),
                                                                                                            CallFunc::create([=](){
                                                                              AudioHelp::getInstance()->playEffect("tools_fall.mp3");
                                                                          }),
                                                                                                            Spawn::create( MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(320, 850))),
                                                                                                                          ScaleTo::create(0.3, 1), NULL),
                                                                                                            CallFunc::create([=](){
                                                                              
                                                                              AudioHelp::getInstance()->playEffect("logo.mp3");
                                                                          }), NULL));
                                                                          
                                                                          SSCMoreGameButton* m_moreGameButton = SSCMoreGameButton::create();
                                                                          //    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
                                                                          m_moreGameButton->showButton(m_pContentLayer,true);
                                                                          m_moreGameButton->setClickCall(CC_CALLBACK_0(HomeScene::clickedMoreGameButton,this));
                                                                          CMVisibleRect::setPosition(m_moreGameButton, 80, 155,kBorderRight,kBorderBottom);
                                                                      }), NULL));
                }
            }
        }
    });
    
    
    AudioHelp::getInstance()->playBackGroundMusic("bg_home_map.mp3");
    
    m_bShowAds = false;
    GameDataManager::getInstance()->m_nDecorateStep= 0;
    if (!UserDefault::getInstance() -> getBoolForKey("removeAds")) {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
    }
    return true;
}
void HomeScene::clickedMoreGameButton()
{
    SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
}

void HomeScene::onEnter()
{
    ExtensionScene::onEnter();
    m_bShowAds = false;
    
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->removeAds(kTypeBannerAds);
        
    }
}
void HomeScene::onExit()
{
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
//            AudioHelp::getInstance()->playStartEffect();
            AudioHelp::getInstance()->playEffect("have_fun.mp3");
            SceneManager::replaceTheScene<MapScene>();
            
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
        case GameUILayoutLayer::eUIButtonTagPolicy:
        {
            Application::getInstance()->openURL("https://www.makerlabs.net/privacy/");
            break;
        }
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