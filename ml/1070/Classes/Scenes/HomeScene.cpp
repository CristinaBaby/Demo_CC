
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"
#include "CottonMachine.h"
#include "DecorateLogoLayer.h"

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
    Sprite* pBg = Sprite::create(localPath("bg.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    
    Node* pCart = CSLoader::createNode("HomeCartNode.csb");
    this->addChildToContentLayer(pCart);
    CMVisibleRect::setPositionAdapted(pCart, 480+visibleSize.width, 300);
    m_pCart = pCart;
    
    Node* pGirl = CSLoader::createNode("HomeGirl.csb");
    this->addChildToContentLayer(pGirl);
    CMVisibleRect::setPositionAdapted(pGirl, 800+visibleSize.width, 210);
    m_pGirl = pGirl;
    
    pGirl->runAction(Sequence::create(DelayTime::create(4),
                                      MoveBy::create(2, Vec2(-visibleSize.width, 0)),
                                      CallFunc::create([=](){
        auto action = CSLoader::createTimeline("HomeGirl.csb",pGirl);
        action->play("wave", true);
        pGirl->runAction(action);
        
        Sprite* pSprite = Sprite::create(localPath("btn_start1.png"));
        this->addChildToUILayer(pSprite);
        CMVisibleRect::setPositionAdapted(pSprite, 475, 235);
        pSprite->setScale(0.5);
        pSprite->runAction(ScaleTo::create(0.5, 1));
        m_pPlaySprite = pSprite;
        pSprite->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.1),
                                                                  ScaleTo::create(0.5, 1),
                                                                  NULL)));
        m_pGameUI->showHomeUILayout();
    }), NULL));
    pCart->runAction(Sequence::create(DelayTime::create(1),
                                      MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                      CallFunc::create([=]()
                                                       {
                                                           AudioHelp::getInstance()->playEffect("vo_start_page.mp3");
                                                           auto action = CSLoader::createTimeline("HomeCartNode.csb",pCart);
                                                           action->play("animation0", false);
                                                           pCart->runAction(action);
                                                       }), NULL));
    AudioHelp::getInstance()->playBackGroundMusic("bg_start.mp3");
    
    m_bShowAds = false;
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){
        return m_bCanTouch;
    };
    listener->onTouchEnded = [=](Touch *pTouch, Event *pEvent){
        Vec2 pos = pTouch->getLocation();
        Rect rect = Rect(0, 0, 780, 170);
        rect.origin = m_pLogoClipping->getParent()->convertToWorldSpace(m_pLogoClipping->getPosition()-Vec2(380, 75));
        if (rect.containsPoint(pos)) {
            _showDecLogoLayer();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    m_moreGameButton = SSCMoreGameButton::create();
    
    
    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
//    m_moreGameButton->showButton(this,false);
    auto pos = Vec2(getContentSize().width - 18 - m_moreGameButton->getButtonSize().width/2,
                    80 + m_moreGameButton->getButtonSize().height/2);
    m_moreGameButton->setPosition(pos);
    this->addChild(m_moreGameButton,200);
    m_moreGameButton->setClickCall(CC_CALLBACK_0(HomeScene::clickedMoreGameButton,this));
    CMVisibleRect::setPosition(m_moreGameButton, 70, 150,kBorderRight,kBorderBottom);
    //    m_moreGameButton->setClickCall([=](){
    //        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    //    });
    AudioHelp::getInstance()->playBackGroundMusic("background.mp3");
    GameDataManager::getInstance()->m_nDecorateStep= 0;
    
    if (!UserDefault::getInstance() -> getBoolForKey("removeAds")) {
        log("========================  ads");
        //AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
        
        AdsManager::getInstance()->showAds(kTypeInterstitialAds);
    }
    
    GameDataManager::getInstance()->m_nDecorateStep= 0;
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
            
            _onStart();
            btn->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, -1000))));
        }
            break;
        case GameUILayoutLayer::eUIButtonTagNext:
        {
            SceneManager::replaceTheScene<MakeCottonScene>();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagPolicy:
        {
            Application::getInstance()->openURL("https://www.makerlabs.net/privacy/");
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
//            STSystemFunction st;
//            st.showMoreGame();
            SSCInternalLibManager::getInstance()->showMoreGames();
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
    _showLogo();
    _showDecLogoLayer();
}
void HomeScene::_showLogo()
{
    m_pLogoClipping->removeAllChildren();
    std::string name;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    name = SSCFileUtility::getStoragePath()+ "logo.png";
#else
    name = FileUtils::getInstance()->getWritablePath()+ "logo.png";
#endif
    Sprite* pSprite = Sprite::create(name);
    if (pSprite) {
        pSprite->setScale(1.2);
        pSprite->setPosition(Vec2(10, -50));
        m_pLogoClipping->addChild(pSprite);
    }
}

void HomeScene::_showDecLogoLayer()
{
    m_bCanTouch = false;
    DecorateLogoLayer* pLayer = DecorateLogoLayer::create();
    this->addChildToUILayer(pLayer);
    pLayer->setLocalZOrder(200);
    pLayer->onLogoDecoratedCallback = [=](){
        std::string name;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        name = SSCFileUtility::getStoragePath()+ "logo.png";
#else
        name = FileUtils::getInstance()->getWritablePath()+ "logo.png";
#endif
        TextureCache::getInstance()->removeTextureForKey(name);
        Sprite* pSprite = Sprite::create(name);
        if (pSprite) {
            this->addChildToUILayer(pSprite);
            pSprite->setPosition(visibleSize*0.5);
            Vec2 pos = m_pLogoClipping->getParent()->convertToWorldSpace(m_pLogoClipping->getPosition());
            pSprite->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                CallFunc::create([=](){
                _showLogo();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/logo.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setLocalZOrder(5);
                pParticle->setPosition(pos);
                pSprite->removeFromParent();
                m_bCanTouch = true;
            }), NULL));
        }else{
            m_bCanTouch = true;
        }
        
        AudioHelp::getInstance()->playEffect("cheer_star.mp3");
        this->runAction(Repeat::create(Sequence::create(DelayTime::create(0.5),
                                                        CallFunc::create([=](){
            _showParticle();
        }), NULL), 6));
    };
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