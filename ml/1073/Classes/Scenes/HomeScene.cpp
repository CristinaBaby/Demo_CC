
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"
#include "HomeCart.h"

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
    Sprite* pBg = Sprite::create(localPath("start_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    
    Vec2 cartPos[3] = {
        Vec2(500, 150),
        Vec2(100, 350),
        Vec2(600, 430),
    };
    
    for (int i = 2; i>=0; i--) {
        HomeCart* pCart = HomeCart::create();
        pCart->loadCartType(i+1,(i+1)%2);
        this->addChildToContentLayer(pCart);
        pCart->setLocalZOrder(1);
        pCart->runCart();
        int dir = i%2==0?1:-1;
        
        CMVisibleRect::setPositionAdapted(pCart, cartPos[i].x+dir*visibleSize.width, cartPos[i].y);
        
        pCart->runAction(Sequence::create(DelayTime::create(0.5+0.5*i),
                                          EaseBackOut::create(MoveBy::create(2, Vec2(-dir*visibleSize.width, 0))),
                                          CallFunc::create([=](){
            pCart->stopRunCart();
        }), NULL));
    }
    
    Node* pLogoNode = Node::create();
    this->addChildToUILayer(pLogoNode);
    CMVisibleRect::setPositionAdapted(pLogoNode, 320, 740,kBorderNone,kBorderTop);
    
    Sprite* pLogoBg = Sprite::create(localPath("logo/logo1.png"));
    pLogoNode->addChild(pLogoBg);
//    CMVisibleRect::setPosition(pLogoBg, 320, 730);
    pLogoBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    pLogoBg->setPosition(Vec2(0, 30));
    pLogoBg->setScale(0.6,0);
    
    Sprite* pLogoText = Sprite::create(localPath("logo/logo0.png"));
    pLogoNode->addChild(pLogoText);
    CMVisibleRect::setPosition(pLogoText, 320, 700);
    pLogoText->setPosition(Vec2(0, 0));
    pLogoText->setScale(0,1);
    
    Sprite* pLogoText2 = Sprite::create(localPath("logo/logo2.png"));
    pLogoNode->addChild(pLogoText2);
    CMVisibleRect::setPosition(pLogoText2, 320, 700-20);
    pLogoText2->setPosition(Vec2(0, -20));
    pLogoText2->setScale(0);
    
    Sprite* pLogoStar = Sprite::create(localPath("logo/star.png"));
    pLogoNode->addChild(pLogoStar);
    pLogoStar->setPosition(Vec2(0, 190));
    pLogoStar->setScale(0);
    
    pLogoText->runAction(Sequence::create(DelayTime::create(3),
                                          ScaleTo::create(0.5, 1),
                                          CallFunc::create([=](){
        pLogoBg->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5, 1)),
                                            CallFunc::create([=](){
            pLogoStar->runAction(Sequence::create(ScaleTo::create(0.3, 1),
                                                  FadeTo::create(1, 180),
                                                  FadeTo::create(1, 255),
                                                  FadeTo::create(1, 180),
                                                  FadeTo::create(1, 255),  NULL));
            pLogoText2->runAction(Sequence::create(DelayTime::create(1),
                                                   Spawn::create(ScaleTo::create(0.5, 1),
                                                                 JumpBy::create(0.6, Vec2(0, 100), 120, 1), NULL),
                                                   CallFunc::create([=](){
                ccBezierConfig cfg;
                cfg.controlPoint_1 = cfg.controlPoint_2 = pLogoStar->getPosition()+Vec2(30, 20);
                cfg.endPosition = pLogoText2->getPosition()+Vec2(20, 26);
                pLogoStar->runAction(Spawn::create(ScaleTo::create(0.5, 0.5),
                                                   BezierTo::create(0.5, cfg), NULL));
                
                m_pGameUI->showHomeUILayout();
            }), NULL));
        }), NULL));
    }), NULL));
    
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_start_page_0.mp3");
    }),
                                     DelayTime::create(1.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_start_page_1.mp3");
    }),
                                     DelayTime::create(2.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_start_page_2.mp3");
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
    AudioHelp::getInstance()->playBackGroundMusic("background.mp3");
    
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
            
            SceneManager::replaceTheScene<ChooseScene>();
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