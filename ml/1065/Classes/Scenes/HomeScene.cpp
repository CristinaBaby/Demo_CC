
#include "HomeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "DecorateManager.h"
#include "Analytics.h"

static  Vec2 gKidPos[] = {
    Vec2(170,225),
    Vec2(810,180),
    Vec2(700,260),
    Vec2(600,280),
};
HomeScene::HomeScene()
{
    m_bShowMoreGame = false;
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
    Sprite* pBg = Sprite::create(localPath("home.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/homelight.plist");
    this->addChildToBGLayer(pParticle);
    pParticle->setPosition(visibleSize*0.5);
    
    Sprite* pTable = Sprite::create(localPath("table.png"));
    this->addChildToUILayer(pTable);
    pTable->setAnchorPoint(Vec2(0.5, 0));
    CMVisibleRect::setPositionAdapted(pTable, Vec2(480, -100));
    pTable->setLocalZOrder(1);
    
    
    Sprite* pCake = Sprite::create(localPath("cake.png"));
    this->addChildToUILayer(pCake);
    CMVisibleRect::setPositionAdapted(pCake, Vec2(480, 80-visibleSize.height));
    pCake->setLocalZOrder(5);
    
    Sprite* pLight = Sprite::create(localPath("halo.png"));
    pCake->addChild(pLight);
    pLight->setPosition(Vec2(162, 420));
    pLight->setOpacity(0);
    m_pLight = pLight;
    
    
    for (int i = 3; i>=0; i--) {
        std::stringstream ostr;
        ostr<<"child"<<i+1<<"/";
        Sprite* pKid = Sprite::create(localPath(ostr.str()+"1.png"));
        pKid->setTag(i);
        CMVisibleRect::setPosition(pKid, gKidPos[i]);
        this->addChildToContentLayer(pKid);
        
        if (i>0) {
            pKid->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath(ostr.str()), 1, 2,false,true,0.5)));
            
            Sprite* pEye = Sprite::create(localPath(ostr.str()+"wink.png"));
            pKid->addChild(pEye);
            pEye->setName("eye");
            pEye->setPosition(pKid->getContentSize()*0.5);
            pEye->setOpacity(0);
            pEye->runAction(Sequence::create(DelayTime::create(0.1+0.1*i),
                                             CallFunc::create([=]()
                                                              {
                                                                  pEye->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.02),
                                                                                                                         DelayTime::create(0.3),
                                                                                                                         FadeOut::create(0.02),
                                                                                                                         DelayTime::create(3+0.1*i), NULL)));
                                                              }), NULL));
        }
        m_VectorKid.pushBack(pKid);
    }
    
    pCake->runAction(Sequence::create(DelayTime::create(0.5),
                                      MoveBy::create(1, Vec2(0, visibleSize.height)),
                                      CallFunc::create([=]()
                                                       {
                                                           pLight->runAction(FadeIn::create(0.3));
                                                           m_pGameUI->showHomeUILayout();
                                                           pLight->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.3,255),
                                                                                                                    EaseBackOut::create(FadeTo::create(1, 200)), NULL)));
                                                           pLight->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 1),
                                                                                                                    EaseBackOut::create(ScaleTo::create(1, 1.5)), NULL)));
                                                           
                                                           Node* logo = CSLoader::createNode("logo.csb");
                                                           ui::Helper::doLayout(logo);
                                                           auto action = CSLoader::createTimeline("logo.csb", logo);
                                                           action->play("animation0", false);
                                                           action->setTimeSpeed(0.5);
                                                           logo->runAction(action);
                                                           this->addChildToUILayer(logo);
                                                       }), NULL));
    
    AudioHelp::getInstance()->playBackGroundMusic("birthday.mp3");
    
    m_bShowAds = false;
    GameDataManager::getInstance()->m_nDecorateStep= 0;
    return true;
}

void HomeScene::onEnter()
{
    ExtensionScene::onEnter();
    m_bShowAds = false;
    
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->removeAds(kTypeBannerAds);
        
    }
}
void HomeScene::onExit()
{
    ExtensionScene::onExit();
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
            for_each(m_VectorKid.begin(), m_VectorKid.end(), [=](Sprite* pKid){
                int tag = pKid->getTag();
                std::stringstream ostr;
                ostr<<"child"<<tag+1<<"/";
                pKid->stopAllActions();
                if (tag==0) {
                    pKid->runAction(Sequence::create(AnimationHelp::createAnimate(localPath(ostr.str()), 2, 3,false),
                                                     CallFunc::create([=]()
                                                                      {
                                                                          AudioHelp::getInstance()->playEffect("ignite_fire.mp3");
                                                                      }), NULL));
                }else{
                    Sprite* pEye = (Sprite*)pKid->getChildByName("eye");
                    if (pEye) {
                        pEye->removeFromParent();
                    }
                    pKid->setTexture(localPath(ostr.str()+"3.png"));
                }
            });
            m_pLight->stopAllActions();
            m_pLight->runAction(Sequence::create(DelayTime::create(1.2),
                                                 FadeOut::create(0.3), NULL));
            
            btn->runAction(Sequence::create(DelayTime::create(0.5),
                                            Spawn::create(FadeOut::create(0.6),
                                                          RotateBy::create(1, 360*2),
                                                          ScaleTo::create(1, 0.01), NULL),
                                            DelayTime::create(0.5),
                                            CallFunc::create([=]()
                                                             {
                                                                 btn->setEnabled(true);
                                                                 
                                                                 GameDataManager::getInstance()->homeToPackage = true;
                                                                 if (!UserDefault::getInstance() -> getBoolForKey("removeAds")) {
                                                                     AdLoadingLayerBase::showLoading<AdsLoadingScene>(false);
                                                                     AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
                                                                         AudioHelp::getInstance()->playBackGroundMusic("bg_make.mp3");
                                                                         SceneManager::replaceTheScene<MixFlourScene>();
                                                                     };
                                                                 }else{
                                                                     AudioHelp::getInstance()->playBackGroundMusic("bg_make.mp3");
                                                                     SceneManager::replaceTheScene<MixFlourScene>();
                                                                 }
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
        case GameUILayoutLayer::eUIButtonTagHomeMore:
        {
            m_bShowMoreGame = true;
            STSystemFunction st;
            st.showMoreGame();
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
    
    FileUtils::getInstance()->addSearchPath("res/");
}

void HomeScene::_showParticle()
{
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dress.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setLocalZOrder(5);
    pParticle->setPosition(Vec2(0, visibleSize.height*0.8));
    
    AudioHelp::getInstance()->playEffect("particles_sparkle.mp3");
    pParticle->runAction(RepeatForever::create(Sequence::create(Spawn::create(MoveBy::create(10, Vec2(visibleSize.width+100, 0)),
                                                                              Sequence::create(EaseBackOut::create(MoveBy::create(1, Vec2(0, 30))),
                                                                                               DelayTime::create(1),
                                                                                               EaseBackOut::create(MoveBy::create(2, Vec2(0, -60))),
                                                                                               DelayTime::create(1),
                                                                                               EaseBackOut::create(MoveBy::create(2, Vec2(0, 60))),
                                                                                               DelayTime::create(1),
                                                                                               EaseBackOut::create( MoveBy::create(1, Vec2(0, -30))),
                                                                                               NULL),
                                                                              NULL),
                                                                DelayTime::create(2),
                                                                CallFunc::create([=]()
                                                                                 {
                                                                                     pParticle->setPosition(Vec2(0, visibleSize.height*0.8));
                                                                                     AudioHelp::getInstance()->playEffect("particles_sparkle.mp3");
                                                                                 }),
                                                                NULL)));
    
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
    Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
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