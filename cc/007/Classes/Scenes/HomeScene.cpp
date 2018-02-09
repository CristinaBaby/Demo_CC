
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"

HomeScene::HomeScene()
{
    
    m_bShowMoreGame = false;
    m_bCanTouch = false;
}

HomeScene::~HomeScene()
{
    MemoryManager::getInstance()->clean(ClassString(HomeScene));
    Director::getInstance()->purgeCachedData();
    ArmatureDataManager::getInstance()->destroyInstance();
    
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
    Sprite* pBg = Sprite::create(localPath("start.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pLogo = Sprite::create(localPath("logo.png"));
    this->addChildToUILayer(pLogo);
    pLogo->setVisible(false);
    pLogo->setScale(0.2);
    CMVisibleRect::setPositionAdapted(pLogo, 320, 750+1000,kBorderNone,kBorderTop);
    
    Sprite* pGrass = Sprite::create(localPath("grass.png"));
    this->addChildToUILayer(pGrass);
    CMVisibleRect::setPositionAdapted(pGrass, 320, 20,kBorderNone,kBorderBottom);
    
    Armature* pRole = Armature::create("model1");
    this->addChildToContentLayer(pRole);
    CMVisibleRect::setPositionAdapted(pRole, 480+visibleSize.width, 300,kBorderNone,kBorderBottom);
//    ArmatureAnimation* lArmation = pRole->getAnimation();
//    m_pRole->setScale(1.5);
    
    
    
    pRole->runAction(Sequence::create(DelayTime::create(1),
                                      CallFunc::create([=](){
        ArmatureAnimation* lArmation = pRole->getAnimation();
        lArmation->play("6");
        lArmation->gotoAndPause(291);
    }),
                                      MoveBy::create(2,Vec2(-visibleSize.width,0)),
                                      CallFunc::create([=](){
        ArmatureAnimation* lArmation = pRole->getAnimation();
        lArmation->play("6",0,-1);
        AudioHelp::getInstance()->playEffect("vo_cookie_maker.mp3");
    }),
                                      DelayTime::create(0.5),
                                      CallFunc::create([=](){
        for(int i = 0;i<3;i++){
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/home_star.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(5);
            pParticle->setPosition(CMVisibleRect::getPosition(420, 300));
            pParticle->setTexture(Sprite::create(localPath("star"+std::to_string(i+1)+".png"))->getTexture());
        }
        pLogo->runAction(Sequence::create(DelayTime::create(1.5),
                                          Show::create(),
                                          CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_so_many_yummy_cookies.mp3");
        }),
                                          Spawn::create(EaseBounceOut::create(MoveBy::create(1, Vec2(0, -1000))),
                                                        EaseElasticInOut::create(ScaleTo::create(1, 1), 1.0f), NULL),
                                          ActionHelper::getJellyAction(),
                                          CallFunc::create([=](){
            ArmatureAnimation* lArmation = pRole->getAnimation();
            lArmation->play("6",0,-1);
            lArmation->gotoAndPause(305);
        }), NULL));
        m_pGameUI->showHomeUILayout();
    }), NULL));
    

    m_bShowAds = false;
//
    m_moreGameButton = SSCMoreGameButton::create();
//    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
    m_moreGameButton->showButton(m_pUILayer,true);
    m_moreGameButton->setClickCall(CC_CALLBACK_0(HomeScene::clickedMoreGameButton,this));
    CMVisibleRect::setPosition(m_moreGameButton,70,150,kBorderRight,kBorderBottom);
    //    m_moreGameButton->setClickCall([=](){
    //        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    //    });
    AudioHelp::getInstance()->playBackGroundMusic("music_home.mp3");
    
    GameDataManager::getInstance()->m_bShowAds = false;
    return true;
}

void HomeScene::onEnter()
{
    m_bShowAds = true;
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_HOME);
    
}
void HomeScene::onEnterTransitionDidFinish()
{
    ExtensionScene::onEnterTransitionDidFinish();
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        
    }
}

void HomeScene::clickedMoreGameButton()
{
    SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
}

void HomeScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    btn->setEnabled(false);
    this->runAction(Sequence::create(DelayTime::create(0.3),
                                     CallFunc::create([=]()
                                                      {
                                                          btn->setEnabled(true);
                                                      }),
                                     NULL));
    switch (tag) {
        case GameUILayoutLayer::eUIButtonTagHomeStart:
        {
            AudioHelp::getInstance()->playStartEffect();
            
//            ArmatureDataManager::getInstance()->removeArmatureFileInfo("model1/model1.ExportJson");
//            MemoryManager::getInstance()->clean("HomeScene");
            SceneManager::replaceTheScene<AnimationScene>();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagPolicy:
        {
            Application::getInstance()->openURL("https://www.crazycatsmedia.com/privacy-policy/");
            break;
        }
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
            STSystemFunction st;
            st.showMoreGame();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagHomeShop:
        {
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
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("model1/model10.png", "model1/model10.plist", "model1/model1.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("model1/model11.png", "model1/model11.plist", "model1/model1.ExportJson");
    MemoryManager::getInstance()->addArmaturePath("model1/model1.ExportJson", ClassString(HomeScene));
    
    MemoryManager::getInstance()->addTexturePath("model1/model10.png", ClassString(HomeScene));
    MemoryManager::getInstance()->addTexturePath("model1/model11.png", ClassString(HomeScene));
}

void HomeScene::onPermissionGrantedResult(int requestCode,bool bGranted){
    if (requestCode == 1) {
        if (bGranted) {
            //add your code....
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }else{
        log("-------->anroid runtime permisson was not granted ,%d,requestcode = %d",bGranted,requestCode);
    }
}
void HomeScene::_onStart()
{
    
    m_pGameUI->showNextLayout();
}

void HomeScene::_showParticle()
{
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/powerStar.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setLocalZOrder(5);
    float poY = 20.0f+30*(arc4random()%20);
    float poX = 100.0f+50*(arc4random()%20);
    pParticle->setPosition(CMVisibleRect::getPosition(poX, poY));
    
}

void HomeScene::onKeyBackClicked()
{
    if (Director::getInstance()->getRunningScene()->getChildByTag(9999)) {
        Director::getInstance()->getRunningScene()->getChildByTag(9999)->removeFromParent();
        return;
    }
    Dialog* dialog=Dialog::create(CCSize2(481,537), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
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