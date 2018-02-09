
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"
#include "RoleModel.h"
#include "RateUsManager.h"
#include "SSCInternalLibManager.h"

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
    
    Sprite* pTable = Sprite::create(localPath("table.png"));
    this->addChildToBGLayer(pTable);
    pTable->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(pTable, 300, 0);
    
    //Animation1
    
    RoleModel* pGirl = RoleModel::create();
    pGirl->setRole(localCocosPath("cooker1/"), "cooker1", 5);
    this->addChildToBGLayer(pGirl);
    pGirl->setScale(0.7);
    CMVisibleRect::setPositionAdapted(pGirl, 450, 380-visibleSize.height);
    
    Sprite* pFoodLeft = Sprite::create(localPath("donut_stand1.png"));
    this->addChildToContentLayer(pFoodLeft);
    CMVisibleRect::setPositionAdapted(pFoodLeft, 200, 200);
    
    Sprite* pFoodRight = Sprite::create(localPath("donut_stand2.png"));
    this->addChildToContentLayer(pFoodRight);
    CMVisibleRect::setPositionAdapted(pFoodRight, 520, 100);
    
    Node* pCocosLogo = CSLoader::createNode("HomeLogo.csb");
    this->addChildToUILayer(pCocosLogo);
    CMVisibleRect::setPositionAdapted(pCocosLogo, 150, 750+visibleSize.height,kBorderNone,kBorderTop);
    ui::Helper::doLayout(pCocosLogo);
    
    
    pGirl->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (0==std::strcmp(movementID.c_str(), "Animation1") ){
                ParticleSystemQuad* pBling0 = ParticleSystemQuad::create("particle/bling0.plist");
                this->addChildToContentLayer(pBling0);
                pBling0->setPosition(pFoodLeft->getPosition()+Vec2(0, 30));
                
                ParticleSystemQuad* pBling1 = ParticleSystemQuad::create("particle/bling1.plist");
                this->addChildToContentLayer(pBling1);
                pBling1->setPosition(pFoodRight->getPosition()+Vec2(0, 30));
                pGirl->playArmation("Animation2", true);
                AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
            }
        }
    };
    pGirl->runAction(Sequence::create(DelayTime::create(4),
                                      MoveBy::create(0.5, Vec2(0, visibleSize.height)),
                                      CallFunc::create([=](){
        pGirl->playArmation("Animation1", false);
        AudioHelp::getInstance()->playEffect("vo_start_page.mp3");
        
        m_pGameUI->showHomeUILayout();
        Sprite* pStartBg = Sprite::create(localPath("btn_start0.png"));
        this->addChildToUILayer(pStartBg);
        CMVisibleRect::setPositionAdapted(pStartBg, 320, 330);
        pStartBg->setScale(0.2);
        pStartBg->runAction(ScaleTo::create(0.5, 1));
        
        Sprite* pStartBg2 = Sprite::create(localPath("btn_start1.png"));
        this->addChildToUILayer(pStartBg2);
        CMVisibleRect::setPositionAdapted(pStartBg2, 320, 340);
        pStartBg2->setScale(0.2);
        pStartBg2->runAction(Sequence::create(ScaleTo::create(0.5, 1),
                                              CallFunc::create([=](){
            pStartBg2->runAction(RepeatForever::create(RotateBy::create(2, 360)));
        }), NULL));
        
    }), NULL));
    
    pCocosLogo->runAction(Sequence::create(DelayTime::create(0.5),
                                           CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("logo_drop.mp3");
    }),
                                           EaseBackOut::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height))),
                                           RotateBy::create(1, 360),
                                           CallFunc::create([=](){
        auto action = CSLoader::createTimeline("HomeLogo.csb",pCocosLogo);
        action->play("eat", false);
        pCocosLogo->runAction(action);
        this->runAction(Sequence::create(DelayTime::create(0.2),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
        }),
                                         DelayTime::create(0.5),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("magnify.mp3");
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("drop.mp3");
        }),NULL));
    }), NULL));
    
    m_bShowAds = true;
//
    m_moreGameButton = SSCMoreGameButton::create();
//    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
    m_moreGameButton->showButton(m_pContentLayer,true);
    m_moreGameButton->setClickCall(CC_CALLBACK_0(HomeScene::clickedMoreGameButton,this));
    CMVisibleRect::setPosition(m_moreGameButton, 80, 155,kBorderRight,kBorderBottom);
    //    m_moreGameButton->setClickCall([=](){
    //        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    //    });
    AudioHelp::getInstance()->playBackGroundMusic("happy.mp3");
    
    GameDataManager::getInstance()->m_bShowAds = false;
    return true;
}

void HomeScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_HOME);
    static bool firstTime = true;
    if(firstTime) {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            AdsManager::getInstance()->showAds(kTypeInterstitialAds);
        }), NULL));
        firstTime = false;
    }
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
            AudioHelp::getInstance()->playEffect("vo_start_page_play.mp3");
            
            SceneManager::replaceTheScene<ServeTransitionScene>();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagHomeRestore:
        {
            ShopAdapter::getInstance()->requestRestore();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagHomeRate:
        {
            
//            SSCInternalLibManager::getInstance()->rateUs();
//            RateUsManager::getInstance()->showRateUs();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagHomeMore:
        {
            
            SSCInternalLibManager::getInstance()->showMoreGames();
        }
            break;
        case GameUILayoutLayer::eUIButtonTagPolicy:
        {
            Application::getInstance()->openURL("https://www.makerlabs.net/privacy/");
//            SSCInternalLibManager::getInstance()->
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