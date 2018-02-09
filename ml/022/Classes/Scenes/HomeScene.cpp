
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"
#include "SSCMoreGameManager.h"
class HomeCart:public Node {
    
    
public:
    CREATE_FUNC(HomeCart);
    bool init();
    
    void showAnimation();
private:
    Sprite* m_pCart;
    Sprite* m_pFrame;
    Sprite* m_pLogo;
    Sprite* m_pColor;
    ClippingNode* m_pLightNode;
};

bool HomeCart::init()
{
    if (!Node::init()) {
        return false;
    }
    Node* pCocosCart = CSLoader::createNode("HomeCartNode.csb");
    m_pCart = Sprite::create("content/start/cart.png");
    this->addChild(m_pCart);
    m_pCart->setPosition(pCocosCart->getChildByName("cart")->getPosition());
    
    m_pFrame = Sprite::create("content/start/frame.png");
    this->addChild(m_pFrame);
    m_pFrame->setPosition(pCocosCart->getChildByName("frame")->getPosition());
    
    m_pLogo = Sprite::create("content/start/logo.png");
    this->addChild(m_pLogo);
    m_pLogo->setPosition(pCocosCart->getChildByName("logo")->getPosition());
    
    m_pLightNode = ClippingNode::create(Sprite::create("content/start/light.png"));
    this->addChild(m_pLightNode);
    m_pLightNode->setPosition(pCocosCart->getChildByName("light")->getPosition());
    m_pLightNode->setAlphaThreshold(0.5);
    
    m_pColor = Sprite::create("content/start/color.png");
    m_pLightNode->addChild(m_pColor);
    m_pColor->setScale(2);
    m_pColor->runAction(RepeatForever::create(Sequence::create(Spawn::create(ScaleTo::create(2, 3),
                                                                                 RotateBy::create(2, 180), NULL),
                                                                   Spawn::create(ScaleTo::create(2, 2),
                                                                                 RotateBy::create(2, 180), NULL), NULL)));
    return true;
}

void HomeCart::showAnimation(){
    m_pLogo->setVisible(false);
    m_pLogo->setPosition(m_pLogo->getPosition()+Vec2(0, -visibleSize.height*.8));
    m_pLogo->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=](){
        m_pLogo->setVisible(true);
    }),
                                        Spawn::create(EaseBackOut::create(ScaleTo::create(1, 1)),
                                                      EaseBounceOut::create(MoveBy::create(0.8, Vec2(0, visibleSize.height*0.8))), NULL), NULL));
    
}

class HomeGirl:public Node {
    
public:
    CREATE_FUNC(HomeGirl);
    bool init();
    Vec2 getArmWorldPos(){
        return m_pArm->getParent()->convertToWorldSpace(m_pArm->getPosition());
    }
private:
    Sprite* m_pBody;
    Sprite* m_pHead;
    Sprite* m_pArm;
};

bool HomeGirl::init()
{
    if (!Node::init()) {
        return false;
    }
    Node* pCocosGirl = CSLoader::createNode("HomeGirlNode.csb");
    m_pBody = Sprite::create("content/start/body.png");
    this->addChild(m_pBody);
    m_pBody->setPosition(pCocosGirl->getChildByName("body")->getPosition());
    
    m_pHead = Sprite::create("content/start/head0.png");
    this->addChild(m_pHead);
    m_pHead->setPosition(pCocosGirl->getChildByName("head")->getPosition());
    
    m_pArm = Sprite::create("content/start/drink.png");
    this->addChild(m_pArm);
    m_pArm->setPosition(pCocosGirl->getChildByName("drink")->getPosition());
    m_pArm->setScale(0.5);
    
    m_pHead->runAction(RepeatForever::create(Sequence::create(AnimationHelp::createAnimate("content/start/head", 0, 1,true,true,0.2),
                                                              DelayTime::create(1.5), NULL)));
    return true;
}
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
    Sprite* pBg = Sprite::create(localPath("start1.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    
    HomeCart* pCart = HomeCart::create();
    this->addChildToContentLayer(pCart);
    CMVisibleRect::setPositionAdapted(pCart, 320+visibleSize.width, 330);
    m_pCart = pCart;
    pCart->setLocalZOrder(1);
    pCart->showAnimation();
    pCart->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    
    HomeGirl* pGirl = HomeGirl::create();
    this->addChildToContentLayer(pGirl);
    CMVisibleRect::setPositionAdapted(pGirl, 260, 300-300);
    pGirl->setScale(0.3);
    
    Sprite* pDrink = Sprite::create(localPath("drink.png"));
    this->addChildToContentLayer(pDrink);
    pDrink->setLocalZOrder(5);
    pDrink->setVisible(false);
    pDrink->setScale(0.5);
    
    pGirl->runAction(Sequence::create(DelayTime::create(0.5),
                                      Spawn::create(MoveBy::create(0.5, Vec2(0, 300)),
                                                    ScaleTo::create(0.5, 1), NULL),
                                      CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_home_logo.mp3");
        pDrink->setPosition(m_pContentLayer->convertToNodeSpace(pGirl->getArmWorldPos()));
        pDrink->setVisible(true);
        pDrink->runAction(Sequence::create(DelayTime::create(1),
                                           EaseBackOut::create(ScaleTo::create(0.5, 1)),
                                           CallFunc::create([=](){
            pDrink->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.3, Vec2(0, 30)),
                                                                     MoveBy::create(0.3, Vec2(0, -30)),
                                                                     DelayTime::create(1), NULL)));
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
    CMVisibleRect::setPosition(m_moreGameButton, 70, 160,kBorderRight,kBorderBottom);
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
    this->runAction(Sequence::create(DelayTime::create(0.3),
                                     CallFunc::create([=](){
        
        if (!UserDefault::getInstance() -> getBoolForKey("removeAds")){
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        }
    }), NULL));
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
            
            SceneManager::replaceTheScene<ChooseIceShapeScene>();
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
            SSCMoreGameManager::getInstance()->show();
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