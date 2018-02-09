
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"
#include "DecorateLogoLayer.h"
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
    
    Sprite* pShop = Sprite::create(localPath("bg_house.png"));
    pShop->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pShop);
    pShop->setLocalZOrder(1);
    
    
    Sprite* pBalloon = Sprite::create(localPath("flowerpot.png"));
    pShop->addChild(pBalloon);
    pBalloon->setPosition(Vec2(950, 190));
    
    Sprite* pFlower = Sprite::create(localPath("flowerpot_1.png"));
    pShop->addChild(pFlower);
    pFlower->setPosition(Vec2(920, 120));
    
    pBalloon->runAction(RepeatForever::create(Sequence::create(RotateBy::create(1, -20),
                                                              RotateBy::create(2, 40),
                                                              RotateBy::create(1, -20),
                                                              DelayTime::create(0.5),
                                                              NULL)));
    pFlower->runAction(RepeatForever::create(Sequence::create(JumpBy::create(0.5,Vec2::ZERO,10,1),
                                                              ScaleTo::create(0.3,1,0.9),
                                                              ScaleTo::create(0.3,1),
                                                              DelayTime::create(2),
                                                              NULL)));
    
    Sprite* pCarShadow = Sprite::create(localPath("car_shadow.png"));
    this->addChildToContentLayer(pCarShadow);
    pCarShadow->setAnchorPoint(Vec2(0.5, 0));
    CMVisibleRect::setPositionAdapted(pCarShadow, 200, 120-pCarShadow->getContentSize().height/2);
    
    Sprite* pCar = Sprite::create(localPath("car.png"));
    this->addChildToContentLayer(pCar);
    CMVisibleRect::setPositionAdapted(pCar, 200, 120);
    
//    Sprite* pGirl = Sprite::create(localPath("model.png"));
//    this->addChildToContentLayer(pGirl);
//    pGirl->setLocalZOrder(1);
//    CMVisibleRect::setPositionAdapted(pGirl, 200, 100);
    
    pCarShadow->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1,1,0.95),
                                                           ScaleTo::create(0.5,1),
                                                           DelayTime::create(3), NULL)));
    
    pCar->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1,1,0.9),
                                                           ScaleTo::create(0.5,1),
                                                           DelayTime::create(3), NULL)));
    
    Node* pLogoNode = CSLoader::createNode("res/LogoNode.csb");
    ui::Helper::doLayout(pLogoNode);
    CMVisibleRect::setPositionAdapted(pLogoNode, 480, 520);
    this->addChildToBGLayer(pLogoNode);
    m_pLogoNode = pLogoNode;
    
    m_pLogoSprite = (Sprite*)pLogoNode->getChildByName("logo");
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        auto action = CSLoader::createTimeline("LogoNode.csb",pLogoNode);
        action->play("all", false);
        pLogoNode->runAction(action);
        
        Node* pDesign = pLogoNode->getChildByName("selfdesign");
        
        m_pLogoClipping = ClippingNode::create(Sprite::create("content/logo_dec/logo_mask.png"));
        pDesign->addChild(m_pLogoClipping);
        m_pLogoClipping->setPosition(Vec2(-50, 0));
        m_pLogoClipping->setAlphaThreshold(0.5);
        _showLogo();
        AudioHelp::getInstance()->playEffect("vo_start.mp3");
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        
        Sprite* pSprite = Sprite::create(localPath("start_bg.png"));
        this->addChildToUILayer(pSprite);
        CMVisibleRect::setPositionAdapted(pSprite, 475, 235);
        pSprite->setScale(0.5);
        pSprite->runAction(ScaleTo::create(0.5, 1));
        pSprite->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.1),
                                                                  ScaleTo::create(0.5, 1),
                                                                  NULL)));
        m_pPlaySprite = pSprite;
        m_pGameUI->showHomeUILayout();
    }), NULL));
    
    m_pGirl = RoleModel::create();
    m_pGirl->setRole(localCocosPath("cooker1/"), "cooker1", 4);
    this->addChildToContentLayer(m_pGirl);
    m_pGirl->setScale(0.4);
    CMVisibleRect::setPositionAdapted(m_pGirl, 200, 100);
    
    m_pGirl->playArmation(0, false);
    m_pGirl->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (0==std::strcmp(movementID.c_str(), "Animation1") ){
                m_pGirl->playArmation(1, true);
            }
        }
    };
    m_bShowAds = true;
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){
        return m_bCanTouch;
    };
    listener->onTouchEnded = [=](Touch *pTouch, Event *pEvent){
        Vec2 pos = pTouch->getLocation();
        Rect rect = Rect(0, 0, 680, 215);
        rect.origin = m_pLogoClipping->getParent()->convertToWorldSpace(m_pLogoClipping->getPosition());
        if (rect.containsPoint(pos)) {
            _showDecLogoLayer();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    m_moreGameButton = SSCMoreGameButton::create();
    
    
    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
    m_moreGameButton->showButton(this,true);
    m_moreGameButton->setClickCall(CC_CALLBACK_0(HomeScene::clickedMoreGameButton,this));
//    m_moreGameButton->setClickCall([=](){
//        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
//    });
    AudioHelp::getInstance()->playBackGroundMusic("bg.mp3");
    GameDataManager::getInstance()->m_nDecorateStep= 0;
    if (!UserDefault::getInstance() -> getBoolForKey("removeAds")) {
        AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
    }
    return true;
}

void HomeScene::onEnter()
{
    ExtensionScene::onEnter();
    m_bShowAds = true;
    
//    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
//        AdsManager::getInstance()->removeAds(kTypeBannerAds);
//        
//    }
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_HOME);
}
void HomeScene::onExit()
{
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_HOME);
    ExtensionScene::onExit();
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
//            SceneManager::replaceTheScene<ChoosePackageScene>();
            
            if (!UserDefault::getInstance() -> getBoolForKey("removeAds")) {
                AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
            }
            m_moreGameButton->removeFromParent();
            Button* pShop = m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagHomeShop);
            pShop->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, -500))));
            btn->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, -1000))));
            m_pPlaySprite->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, -1000))));
            
            m_pLogoSprite->runAction(FadeOut::create(0.5));
            Node* pCar = m_pLogoNode->getChildByName("car");
            Node* pPizza = m_pLogoNode->getChildByName("logo_pizza1");
            pCar->setVisible(false);
            pPizza->setVisible(false);
            
            m_pGameUI->showNextLayout();
            
            _showDecLogoLayer();
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
//            STSystemFunction st;
//            st.showMoreGame();
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
    setExCocosPath("res/");
    
    m_nNextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagChoosePackage;
}

void HomeScene::_showLogo()
{
    m_pLogoClipping->removeAllChildren();
    std::string name;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    name = FileUtility::getStoragePath()+ "logo.png";
#else
    name = FileUtility::getStoragePath()+ "/logo.png";
#endif
    Sprite* pSprite = Sprite::create(name);
    if (pSprite) {
        pSprite->setScale(1.2);
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
        name = FileUtility::getStoragePath()+ "logo.png";
#else
        name = FileUtility::getStoragePath()+ "/logo.png";
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
    if (m_bShowMoreGame) {
        m_bShowMoreGame = false;
        return;
    }
    if (Director::getInstance()->getRunningScene()->getChildByTag(9999)) {
        Director::getInstance()->getRunningScene()->getChildByTag(9999)->removeFromParent();
        return;
    }
    Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
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