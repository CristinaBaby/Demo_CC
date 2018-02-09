
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
    CMVisibleRect::setPositionAdapted(pLogo, 320, 800+visibleSize.height);
    
    pLogo->runAction(Sequence::create(DelayTime::create(0.5),
                                      CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_home_logo.mp3");
    }),
                                      EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                      CallFunc::create([=](){
        Sprite* pStartBg = Sprite::create(localPath("btn_start0.png"));
        this->addChildToUILayer(pStartBg);
        CMVisibleRect::setPositionAdapted(pStartBg, 320, 300);
        pStartBg->setScale(0.2);
//        pStartBg->setOpacity(0);
        pStartBg->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5, 1)),
//                                             FadeIn::create(0.5),
                                             CallFunc::create([=](){
            m_pGameUI->showHomeUILayout();
        }), NULL));
                                      }), NULL));
    

    m_bShowAds = false;
//
    m_moreGameButton = SSCMoreGameButton::create();
//    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
    m_moreGameButton->showButton(m_pContentLayer,true);
    m_moreGameButton->setClickCall(CC_CALLBACK_0(HomeScene::clickedMoreGameButton,this));
    //    m_moreGameButton->setClickCall([=](){
    //        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    //    });
    
    CMVisibleRect::setPosition(m_moreGameButton, 80, 160,kBorderRight,kBorderBottom);
    AudioHelp::getInstance()->playBackGroundMusic("music.mp3");
    
    GameDataManager::getInstance()->m_bShowAds = false;
    GameDataManager::getInstance()->m_bFirst = true;
    return true;
}

void HomeScene::onEnter()
{
    m_bShowAds = true;
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_HOME);
    
//    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
//        AdsManager::getInstance()->removeAds(kTypeBannerAds);
//        
//    }
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
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setPosition(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagHomeStart)->getPosition());
            pParticle->setLocalZOrder(10);
            this->runAction(Sequence::create(DelayTime::create(0.2),
                                             CallFunc::create([=]()
                                                              {
                                                                  SceneManager::replaceTheScene<ChooseScene>();
                                                              }),
                                             NULL));
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
    
//#if __cplusplus > 201100L
//    RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
//        onPermissionGrantedResult(requestcode, bgranted);
//    };
//#else
//    RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
//#endif
//    //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
//    int requestCode = 1;
//    //调用权限申请的方法,根据需要申请敏感权限
//    RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kCamera | PERMISSION::kWriteExternalStorage);
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
    m_moreGameButton->removeFromParent();
    Button* pShop = m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagHomeShop);
    pShop->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, -500))));
    m_pPlaySprite->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, -1000))));
    
    m_pCart->setVisible(false);
    m_pGirl->setVisible(false);
    
    m_pCartDec = CSLoader::createNode("LogoCartNode.csb");
    this->addChildToContentLayer(m_pCartDec);
    CMVisibleRect::setPositionAdapted(m_pCartDec, 480, 360);
    m_pCartDec->setScale(1.4);
    m_pCartDec->runAction(ScaleTo::create(0.5, 1));
    
    Node* pDecoNode = m_pCartDec->getChildByName("dec");
    m_pLogoClipping = ClippingNode::create(Sprite::create("content/logo_dec/mask.png"));
    pDecoNode->addChild(m_pLogoClipping);
    m_pLogoClipping->setAlphaThreshold(0.5);
    
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