
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"
#include "cocostudio/CocoStudio.h"


static Vec2 gItemPos[5] = {
    Vec2(170,580),
    Vec2(24,424),
    Vec2(160,210),
    Vec2(610,490),
    Vec2(620,260),
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
    Sprite* pBg = Sprite::create(localPath("start_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    for (int i = 0; i<5; i++) {
        Sprite* pItem = Sprite::create(localPath("item"+std::to_string(i)+".png"));
        this->addChildToContentLayer(pItem);
        CMVisibleRect::setPositionAdapted(pItem,gItemPos[i]);
        int k = i;
        if (i>=3) {
            k = i+1;
        }
        pItem->runAction(Sequence::create(ScaleTo::create(1, 0.9),
                                          ScaleTo::create(1, 1),
                                          MoveTo::create(0.5,Vec2(visibleSize.width/2+20*(-1+i/3*2)*(i%3+1),visibleSize.height*0.4+20*(1-k%3))),
                                          ScaleTo::create(0.1, 0.6,0.9),
                                          CallFunc::create([=](){
            if (i==0) {
                m_pGameUI->showHomeUILayout();
                int radom = arc4random()%5;
                for (int t=0; t<2; t++) {
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/foodbomb.plist");
                    pParticle->setTexture(Sprite::create(localPath("p"+std::to_string((radom+t)%5)+".png"))->getTexture());
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(10);
                    pParticle->setPosition(pItem->getPosition());
                }
            }
        }),
                                          Spawn::create(EaseBackIn::create(MoveBy::create(0.5,Vec2((k/3<1?-1:1)*50*(k%2+1),(1-i%3)*110))),
                                                        ScaleTo::create(0.3, 1), NULL),
                                          NULL));
    }
    
    Sprite* pLogo = Sprite::create(localPath("logo.png"));
    CMVisibleRect::setPositionAdapted(pLogo,320-visibleSize.width,810,kBorderNone,kBorderTop);
    this->addChildToContentLayer(pLogo);
    pLogo->runAction(Sequence::create(DelayTime::create(3),
                                      CallFunc::create([=](){
    }),
                                      EaseBackOut::create(MoveBy::create(2,Vec2(visibleSize.width,0))),
                                      NULL));
    
    
//    Node* pHomeLayer = CSLoader::createNode("res/HomeLayer.csb");
//    ui::Helper::doLayout(pHomeLayer);
//    CMVisibleRect::setPositionAdapted(pHomeLayer, 0, 0);
//    this->addChildToContentLayer(pHomeLayer);
//    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_start_page0.mp3");
        
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_start_page1.mp3");
        
    }),NULL));
    
    m_bShowAds = false;
//
    m_moreGameButton = SSCMoreGameButton::create();
//    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
    m_moreGameButton->showButton(m_pContentLayer,true);
    m_moreGameButton->setClickCall(CC_CALLBACK_0(HomeScene::clickedMoreGameButton,this));
    CMVisibleRect::setPosition(m_moreGameButton,80,150,kBorderRight,kBorderBottom);
    AudioHelp::getInstance()->playBackGroundMusic("bgm1.mp3");
    
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