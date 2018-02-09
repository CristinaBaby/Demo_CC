
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
    LockUIManager::getInstance()->onRewardSuccess = nullptr;
    
    DecorateManager::getInstance()->reset();
    //    bg
    Sprite* pBg = Sprite::create(localPath("home_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Node* pLogo = Node::create();
    this->addChildToUILayer(pLogo);
    CMVisibleRect::setPositionAdapted(pLogo, 320, 840+visibleSize.height);
    
    Sprite* pTitle = Sprite::create(localPath("logo1.png"));
    pLogo->addChild(pTitle,5);
    
    Sprite* pRainbow = Sprite::create(localPath("logo2.png"));
    pLogo->addChild(pRainbow);
//    pRainbow->setAnchorPoint(Vec2(0.5,0));
    pRainbow->setScale(1,0.1);
    
    for(int i = 0;i<3;i++){
        Sprite* pFruit = Sprite::create(localPath("logo_fruit"+std::to_string(i+1)+".png"));
        pLogo->addChild(pFruit);
        Vec2 pos[] ={Vec2(-160,-50),Vec2(100,-90),Vec2(150,-40)};
        pFruit->setPosition(pos[i]);
        pFruit->setScale(0.1);
        pFruit->runAction(Sequence::create(DelayTime::create(3),
                                           Spawn::create(ScaleTo::create(0.5,1),
                                                         RotateBy::create(0.6,360),
                                                         JumpBy::create(1,Vec2(0,80),200,1),NULL),
                                           CallFunc::create([=](){
            pFruit->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2+0.4*i),
                                                                     Spawn::create(JumpBy::create(0.5,Vec2(0,0),100,1),
                                                                                   RotateBy::create(0.4,360),NULL),NULL)));
        }),NULL));
    }
    pLogo->runAction(Sequence::create(DelayTime::create(0.5),
                                      EaseBackOut::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height))),
                                      CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_rainbow_cake_bakery.mp3");
        pRainbow->runAction(Sequence::create(DelayTime::create(0.5),
                                           EaseBackOut::create(ScaleTo::create(0.5, 1)),
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
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setLocalZOrder(100);
            pParticle->setPosition(btn->getPosition());
            
            SceneManager::replaceTheScene<ChooseScene>();
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