
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"

class HomeGirl:public Node {
    
public:
    CREATE_FUNC(HomeGirl);
    bool init();
    void waveArm();
private:
    Sprite* m_pBody;
    Sprite* m_pHead;
};

bool HomeGirl::init()
{
    if (!Node::init()) {
        return false;
    }
    m_pBody = Sprite::create("content/start/1.png");
    this->addChild(m_pBody);
    
    m_pHead = Sprite::create("content/start/head1.png");
    this->addChild(m_pHead);
    
    
    m_pHead->runAction(RepeatForever::create(Sequence::create(AnimationHelp::createAnimate("content/start/head", 1, 2,true,true,0.2),
                                                              DelayTime::create(1.5), NULL)));
    return true;
}



void HomeGirl::waveArm(){
    m_pBody->runAction(AnimationHelp::createAnimate("content/start/", 1, 2,false,true,0.5));
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
    Sprite* pBg = Sprite::create(localPath("start_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    m_pBg = pBg;
    
    HomeCart* pCart = HomeCart::create();
    this->addChildToContentLayer(pCart);
    pCart->setPosition(Vec2(visibleSize.width/2+50+visibleSize.width, visibleSize.height/2+500));
    m_pCart = pCart;
    pCart->setLocalZOrder(1);
    pCart->runCar();
    pCart->setName("cart");
    
    HomeGirl* pGirl = HomeGirl::create();
    this->addChildToContentLayer(pGirl);
    pGirl->setLocalZOrder(2);
    CMVisibleRect::setPositionAdapted(pGirl, 490, 370);
    pGirl->setVisible(false);
    pGirl->setName("girl");
    
    Sprite* pLogo = Sprite::create(localPath("logo.png"));
    this->addChildToUILayer(pLogo);
    pLogo->setPosition(Vec2(pGirl->getBoundingBox().getMinX(), pGirl->getBoundingBox().getMaxY()));
    pLogo->setVisible(false);
    pLogo->setScale(0.1);
    pLogo->setName("logo");
    m_pLogo = pLogo;
    m_pCart = pCart;
    m_pGirl = pGirl;
    
    pCart->runAction(Sequence::create(DelayTime::create(0.5),
                                      CallFunc::create([=](){
        AudioHelp::getInstance()->playLoopEffect("driving.mp3");
    }),
                                      EaseBackOut::create(MoveBy::create(3, Vec2(-visibleSize.width, -500))),
                                      CallFunc::create([=](){
        AudioHelp::getInstance()->stopLoopEffect();
        AudioHelp::getInstance()->playEffect("car_stop.mp3");
        pCart->stopRunCar();
    }),
                                      DelayTime::create(2),
                                      CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_start_page.mp3");
        pBg->runAction(ScaleTo::create(1, 1.9));
        pCart->stopCartAction();
        pCart->runAction(Sequence::create(ScaleTo::create(1, 1.9),
                                          DelayTime::create(0.5),
                                          CallFunc::create([=](){
            pGirl->setVisible(true);
        }),
                                          DelayTime::create(1),
                                          CallFunc::create([=](){
            pGirl->waveArm();
            pLogo->setVisible(true);
//            pLogo->runAction(RotateBy::create(1, 360));
//            pLogo->runAction(ScaleTo::create(1, 1));
            Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(320, 830));
            pLogo->runAction(Sequence::create(DelayTime::create(0.5),
                                              Spawn::create(RotateBy::create(1, 360),
                                                            ScaleTo::create(1, 1),
                                                            EaseBackOut::create(MoveTo::create(1, pos)), NULL),
                                              CallFunc::create([=](){
                m_pGameUI->showHomeUILayout();
            }), NULL));
        }), NULL));
        
    }), NULL));
    
    
    

    m_bShowAds = false;
//
    m_moreGameButton = SSCMoreGameButton::create();
//    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
    m_moreGameButton->showButton(m_pContentLayer,true);
    m_moreGameButton->setClickCall(CC_CALLBACK_0(HomeScene::clickedMoreGameButton,this));
    CMVisibleRect::setPosition(m_moreGameButton, 80, 155,kBorderRight,kBorderBottom);
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
    
//    static bool firstTime = true;
//    if(firstTime) {
//        AdsManager::getInstance()->showAds(kTypeCrosspromoAds);
//        firstTime = false;
//    }
}
void HomeScene::onEnterTransitionDidFinish()
{
    ExtensionScene::onEnterTransitionDidFinish();
    AdsManager::getInstance()->showAds(kTypeCrosspromoAds);
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
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=]()
                                                      {
                                                          btn->setEnabled(true);
                                                      }),
                                     NULL));
    switch (tag) {
        case GameUILayoutLayer::eUIButtonTagHomeStart:
        {
            AudioHelp::getInstance()->playStartEffect();
//            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeCrosspromoAds);
            _onStart();
//            SceneManager::replaceTheScene<ChooseIceShapeScene>();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagPolicy:
        {
            Application::getInstance()->openURL("https://www.makerlabs.net/privacy/");
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
    m_pLogo->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, 1000))));
    Vector<Node*> pUIChildren = m_pGameUI->getChildren();
    for_each(pUIChildren.begin(), pUIChildren.end(), [=](Node* pNode){
        Vec2 pos = pNode->getPosition();
        if (pos.y<visibleSize.height/2) {
            pNode->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, -1000))));
        }else{
            pNode->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, 500))));
        }
    });
    
    Sprite* pMask = Sprite::create(localPath("mask.png"));
    this->addChildToUILayer(pMask);
    pMask->setPosition(visibleSize*0.5);
    pMask->setScale(1.2, 1.5);
    pMask->setVisible(false);
    m_pGirl->setVisible(false);
    
    
    Sprite* pFrame = Sprite::create(localPath("choose_frame.png"));
    this->addChildToUILayer(pFrame);
    pFrame->setAnchorPoint(Vec2(0, 0.2));
    CMVisibleRect::setPosition(pFrame, 80, 600);
    pFrame->setVisible(false);
    pFrame->setScale(0.1);
    
    Button* pItem0 = Button::create(localPath("btn_redog.png"));
    pFrame->addChild(pItem0);
    pItem0->setTag(0);
    pItem0->setPosition(Vec2(150, 300));
    
    Button* pItem1 = Button::create(localPath("btn_jadog.png"));
    pFrame->addChild(pItem1);
    pItem1->setTag(1);
    pItem1->setPosition(Vec2(400, 300));
    pItem0->setScale(0.2);
    pItem1->setScale(0.2);
    pItem0->setVisible(false);
    pItem1->setVisible(false);
    m_pBg->runAction(Sequence::create(DelayTime::create(0.5),
                                      ScaleTo::create(1, 1), NULL));
    m_pCart->runAction(Sequence::create(DelayTime::create(0.5),
                                        ScaleTo::create(1, 1),
                                        CallFunc::create([=](){
        m_pCart->stopRunCar();
        pMask->setVisible(true);
        pFrame->setVisible(true);
        pFrame->runAction(RotateBy::create(0.2, 360));
        pFrame->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(1, 1)),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("choose_pop.mp3");
        }), NULL));
        pItem0->runAction(Sequence::create(DelayTime::create(1),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_choose_page.mp3");
            pItem0->setVisible(true);
        }),
                                           EaseBackOut::create(ScaleTo::create(1, 1)),
                                           CallFunc::create([=](){
            pItem0->setTouchEnabled(true);
        }), NULL));
        pItem1->runAction(Sequence::create(DelayTime::create(1),
                                           CallFunc::create([=](){
            pItem1->setVisible(true);
        }),
                                           EaseBackOut::create(ScaleTo::create(1, 1)),
                                           CallFunc::create([=](){
            pItem1->setTouchEnabled(true);
        }), NULL));
    }), NULL));
    
    auto listener = [=](Ref* ref,Widget::TouchEventType type){
        if (type!=Widget::TouchEventType::ENDED) {
            return;
        }
        pItem0->setTouchEnabled(false);
        pItem1->setTouchEnabled(false);
        AudioHelp::getInstance()->playSelectedEffect();
        Button* pButton = (Button*)ref;
        Sprite* pOK = Sprite::create(localPath("chosen.png"));
        pFrame->addChild(pOK);
        pOK->setPosition(pButton->getPosition()+Vec2(70, -50));
        int tag = pButton->getTag();
        if (tag==1) {
            AudioHelp::getInstance()->playEffect("vo_choose_japanese.mp3");
        }else{
            AudioHelp::getInstance()->playEffect("vo_nice_choice.mp3");
        }
        pOK->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(0.5, Vec2(100-200*tag, 0)), NULL));
        pButton->runAction(Sequence::create(DelayTime::create(0.5),
                                            MoveBy::create(0.5, Vec2(100-200*tag, 0)),
                                            DelayTime::create(0.5),
                                            CallFunc::create([=](){
            pFrame->runAction(Sequence::create(ScaleTo::create(1, 0.1),
                                               CallFunc::create([=](){
                pFrame->setVisible(false);
                pMask->setVisible(false);
                HomeCart* pCart = (HomeCart*)m_pCart;
                pCart->runCar();
                GameDataManager::getInstance()->m_nPackage = tag;
                AudioHelp::getInstance()->playLoopEffect("driving.mp3");
                pCart->runAction(Sequence::create(MoveBy::create(3, Vec2(-visibleSize.width, -400)),
                                                  CallFunc::create([=](){
                    AudioHelp::getInstance()->stopLoopEffect();
                    AdsManager::getInstance()->showAds(kTypeCrosspromoAds);
                    if (tag==0) {
                        SceneManager::replaceTheScene<GrillSausageScene>();
                    }else{
                        SceneManager::replaceTheScene<MixFlourScene>();
                    }
                }), NULL));
            }), NULL));
        }), NULL));
        if (pButton==pItem0) {
            pItem1->setVisible(false);
        }else{
            pItem0->setVisible(false);
        }
    };
    pItem0->addTouchEventListener(listener);
    pItem1->addTouchEventListener(listener);
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
    Dialog* dialog=Dialog::create(CCSize2(451,446), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
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