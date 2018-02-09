
#include "MapScene.h"
#include "SceneManager.h"
#include "Analytics.h"

MapScene::MapScene()
{
    m_bCanTouch = false;
}

MapScene::~MapScene()
{
    
}
bool MapScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    float height = 1750;
    float scale = MAX(1.0f*visibleSize.width/768.0, 1.0f*visibleSize.height/height);
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width, visibleSize.height));
    m_pScrollView->setPosition(Vec2::ZERO);
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 0,kBorderLeft,kBorderBottom);
    this->addChildToContentLayer(m_pScrollView);
    m_pScrollView->setClippingEnabled(false);
    m_pScrollView->setTouchEnabled(true);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->getInnerContainer()->setContentSize(Size(visibleSize.width, height*scale));
    m_pScrollView->getInnerContainer()->setPosition(Vec2::ZERO);
    float delt = 0.1;
    float delt0 = 0.1;
    float delt1 = 0;
    
    if (GameDataManager::getInstance()->getMapLocked()){
        delt1 = 1;
        delt0 = 3;
    }else{
        if (GameDataManager::getInstance()->getStepCount()>=6) {
            if (!GameDataManager::getInstance()->getMapFinish()) {
                delt0 = 3;
            }
        }
        
        if (GameDataManager::getInstance()->getStepCount()<2) {
            delt = 3;
        }else{
            delt = 2;
        }
        if (GameDataManager::getInstance()->getStepCount()<=3&&GameDataManager::getInstance()->getStepCount()>1) {
            m_pScrollView->scrollToBottom(0.1, true);
        }else{
            if (GameDataManager::getInstance()->getMapFinish()&&GameDataManager::getInstance()->getStepCount()>=6){
                m_pScrollView->scrollToTop(0.1, true);
            }else{
                m_pScrollView->scrollToPercentVertical(50, 0.1, true);
            }
        }
    }
    this->runAction(Sequence::create(DelayTime::create(1+delt1),
                                     CallFunc::create([=]()
                                                      {
                                                          if (GameDataManager::getInstance()->getMapLocked()){
                                                              m_pScrollView->scrollToBottom(3, true);
                                                          }else{
                                                              if (GameDataManager::getInstance()->getStepCount()>=6) {
                                                                  if (!GameDataManager::getInstance()->getMapFinish()) {
                                                                      m_pScrollView->scrollToPercentVertical(80, 3,true);
                                                                      AudioHelp::getInstance()->playEffect("parts2play.mp3");
                                                                      this->runAction(Sequence::create(DelayTime::create(3.2),
                                                                                                       CallFunc::create([=](){
                                                                          AudioHelp::getInstance()->playEffect("play.mp3");
                                                                      }), NULL));
                                                                  }
                                                              }
                                                          }
                                                      }),
                                     DelayTime::create(delt0),
                                     CallFunc::create([=]()
                                                      {
                                                          if (GameDataManager::getInstance()->getMapLocked()){
                                                          }else{
                                                              if (GameDataManager::getInstance()->getStepCount()<2) {
                                                                  m_pScrollView->scrollToBottom(3, true);
                                                              }else if (GameDataManager::getInstance()->getStepCount()==2) {
                                                                  m_pScrollView->scrollToPercentVertical(80, delt, true);
                                                              }else if (GameDataManager::getInstance()->getStepCount()==3) {
                                                                  m_pScrollView->scrollToPercentVertical(50, delt, true);
                                                              }else if (GameDataManager::getInstance()->getStepCount()<6) {
                                                                  m_pScrollView->scrollToTop(2, true);
                                                              }else if (GameDataManager::getInstance()->getStepCount()>=6) {
                                                                  if (GameDataManager::getInstance()->getMapFinish()){
                                                                      m_pScrollView->scrollToPercentVertical(80, 2,true);
                                                                  }
                                                              }
                                                          }
                                                      }),
                                     DelayTime::create(delt),
                                     CallFunc::create([=]()
                                                      {
                                                          if (!GameDataManager::getInstance()->getMapLocked()) {
                                                              m_bCanTouch = true;
                                                          }
                                                          GameDataManager::getInstance()->setMapFinish(true);
                                                      }), NULL));
    
    FileUtils::getInstance()->addSearchPath("cocos/");
    Node* scene = CSLoader::createNode("cocos/MapScene.csb");
    m_pScrollView->getInnerContainer()->addChild(scene);
    scene->setPosition(Vec2::ZERO);
    ui::Helper::doLayout(scene);
    scene->setScale(scale);
    m_MapLayer = scene;
    
    Node* pHouseRoot = m_MapLayer->getChildByName("house");
    
    _showHouse();
    _showShopHouse();
    
    Button* pMore = dynamic_cast<Button*>(pHouseRoot->getChildByName("Button_1"));
    pMore->setTag(6);
    pMore->addTouchEventListener(CC_CALLBACK_2(MapScene::onButtonCallback, this));
    
    if (GameDataManager::getInstance()->getMapLocked()) {
        _showGirl();
    }else{
        _winterMagic();
    }
    AudioHelp::getInstance()->playBackGroundMusic("bg_home_map.mp3");
    return true;
}


void MapScene::onEnter()
{
    ExtensionScene::onEnter();
    
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAP);
}

void MapScene::onExit()
{
    ExtensionScene::onExit();
}
void MapScene::onButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        if (!m_bCanTouch) {
            return;
        }
        Button* pButton = dynamic_cast<Button*>(ref);
        if (pButton) {
            int tag = pButton->getTag();
            if (tag==5) {
                AudioHelp::getInstance()->playBackGroundMusic("bg_make2.mp3");
                ShopLayer* pLayer = ShopLayer::create();
                this->addChildToUILayer(pLayer);
                pLayer->setLocalZOrder(100);
                pLayer->showBannerDismiss();
                return;
            }else if(tag==6){
                AudioHelp::getInstance()->playBackGroundMusic("bg_make2.mp3");
//                STSystemFunction st;
                SSCInternalLibManager::getInstance()->showMoreGames();
                return;
            }
            if (GameDataManager::getInstance()->getStepCount()<=tag){
                return;
            }
            AudioHelp::getInstance()->playBackGroundMusic("bg_make2.mp3");
            switch (tag) {
                case 0:{
                    
                    AudioHelp::getInstance()->playEffect("make_ice.mp3");
                    SceneManager::replaceTheScene<MakeIceScene>();
                }
                    break;
                case 1:{
//                    AudioHelp::getInstance()->playEffect("make_ice.mp3");
                    GameDataManager::getInstance()->m_bColored = false;
                    SceneManager::pushTheScene<PuzzleGameScene>();
                }
                    break;
                case 2:{
                    
//                    AudioHelp::getInstance()->playEffect("make_ice.mp3");
                    SceneManager::pushTheScene<ColorIceScene>();
                }
                    break;
                case 3:
                    SceneManager::replaceTheScene<DecorateConeScene>();
                    break;
                case 4:
                    SceneManager::replaceTheScene<DecorateScene>();
                    break;
                case 5:
                {
                }
                    break;
                case 6:{
                    
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
}
#pragma mark - initData
void MapScene::_initData()
{
}

void MapScene::_showGirl(){
    log("=====_showGirl====");
    for (int i = 0; i<5; i++) {
        std::stringstream ostr;
        ostr<<"cocos/girl/icegirl"<<i;
        ArmatureDataManager::getInstance()->addArmatureFileInfo(ostr.str()+".png", ostr.str()+".plist", "cocos/girl/icegirl.ExportJson");
    }
    Armature* lArmature = Armature::create("icegirl");
    this->addChildToUILayer(lArmature);
    lArmature->setScale(0.3);
    lArmature->setLocalZOrder(1);
    m_pGirlArmature = lArmature;
    
    ArmatureAnimation* lArmation = lArmature->getAnimation();
    lArmation->play("hot",-1,1);
    CMVisibleRect::setPositionAdapted(lArmature, 130-200, 500-visibleSize.height,kBorderLeft);
    lArmature->runAction(Sequence::create(DelayTime::create(3),
                                          CallFunc::create([=]()
                                                           {
                                                               AudioHelp::getInstance()->playEffect("hot_burning.mp3");
                                                               
                                                           }),
                                          EaseBackOut::create(MoveBy::create(1.5, Vec2(200, visibleSize.height))),
                                          DelayTime::create(3),
                                          CallFunc::create([=]()
                                                           {
                                                               lArmation->stop();
                                                               lArmation->play("hot_magic", -1, 0);
                                                               AudioHelp::getInstance()->playEffect("abcerbra.mp3");
                                                               
                                                           }), NULL));
    
//    飞出时间8s
    lArmation->setMovementEventCallFunc([=](Armature * arm, MovementEventType type, const std::string &movementID){
        if (type==MovementEventType::COMPLETE) {
            if(movementID=="hot_magic"){
                AudioHelp::getInstance()->playEffect("genie_magic.mp3");
                this->runAction(Sequence::create(DelayTime::create(1),
                                                 CallFunc::create([=]()
                                                                  {
                                                                      this->_winterMagic();                                    GameDataManager::getInstance()->setMapLocked(false); 
                                                                  }),
                                                 DelayTime::create(5),
                                                 CallFunc::create([=]()
                                                                  {
                                                                      m_pGirlArmature->getAnimation()->play("fly",-1,1);
                                                                      Node* pNode0 = m_pUILayer->getChildByName("home_light0");
                                                                      if (pNode0) {
                                                                          pNode0->removeFromParent();
                                                                      }
                                                                      Node* pNode1 = m_pUILayer->getChildByName("home_light1");
                                                                      if (pNode1) {
                                                                          pNode1->removeFromParent();
                                                                      }
                                                                      
                                                                      ccBezierConfig cfg;
                                                                      cfg.controlPoint_1 = CMVisibleRect::getPosition(400, 100);
                                                                      cfg.controlPoint_2 = CMVisibleRect::getPosition(600, 1200);
                                                                      cfg.endPosition = CMVisibleRect::getPosition(1200, 1400);
                                                                      m_pGirlArmature->runAction(Sequence::create(Spawn::create(BezierTo::create(3, cfg),
                                                                                                                                ScaleTo::create(2, 0.2), NULL),
                                                                                                                  CallFunc::create([=]()
                                                                                                                                   {
                                                                                                                                       m_bCanTouch = true;
                                                                                                                                               }), NULL));
                                                                  }), NULL));
                
                ParticleSystemQuad* pParticleSnow = ParticleSystemQuad::create("particle/snow0.plist");
                this->addChildToUILayer(pParticleSnow);
                pParticleSnow->setPosition(Vec2(visibleSize.width/2, visibleSize.height+30));
                
                Vec2 point = CocoStudioHelper::getBoneWorlPoint(arm, "effect3");
                point = m_pUILayer->convertToNodeSpace(point);
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/home_light0.plist");
                this->addChildToUILayer(pParticle);
                pParticle->setPosition(point);
                pParticle->setName("home_light0");
                
                ParticleSystemQuad* pParticle2 = ParticleSystemQuad::create("particle/home_light1.plist");
                this->addChildToUILayer(pParticle2);
                pParticle2->setPosition(point);
                pParticle2->setName("home_light1");
                
                lArmation->play("hot_magic2", -1, 1);
                
            }
        }
        
    });
}
void MapScene::_winterMagic(){
    log("=====_winterMagic====");
    float delt = 5;
    if (GameDataManager::getInstance()->getMapLocked()) {
    delt = 10;
        auto action = CSLoader::createTimeline("MapScene.csb", m_MapLayer);
    //    action->setTimeSpeed(3);
        action->play("animation0", false);
        m_MapLayer->runAction(action);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              
                                                              AudioHelp::getInstance()->playEffect("comfortable.mp3");
                                                          }), NULL));
        
    }else{
        auto action = CSLoader::createTimeline("MapScene.csb", m_MapLayer);
        action->play("animation2", false);
        
        m_MapLayer->runAction(action);
    }

    this->runAction(Sequence::create(DelayTime::create(delt),
                                     CallFunc::create([=]()
                                                      {
                                                          auto actionMore = CSLoader::createTimeline("MapScene.csb", m_MapLayer);
                                                          actionMore->play("animation1", true);
                                                          m_MapLayer->runAction(actionMore);
                                                      }), NULL));
    
    for_each(m_Houses.begin(), m_Houses.end(), [=](Node* pHouse)
             {
                 int tag = pHouse->getTag();
                 if (GameDataManager::getInstance()->getStepCount()>tag) {
                     if (GameDataManager::getInstance()->getStepCount()-1 == tag) {
                         GameDataManager::getInstance()->m_bMapUnlock = true;
                         
                         float delt = 0.3;
                         if (GameDataManager::getInstance()->getMapLocked()){
                             delt = 6;
                         }else{
                             if (GameDataManager::getInstance()->getStepCount()<2) {
                                 delt = 3;
                             }else if (GameDataManager::getInstance()->getStepCount()==2) {
                                 delt = 2;
                             }else if (GameDataManager::getInstance()->getStepCount()==3) {
                                 delt = 2;
                             }else{
                                 delt = 2;
                             }
                         }
                         this->runAction(Sequence::create(DelayTime::create(0.8+delt),
                                                          CallFunc::create([=]()
                                                                           {
                                                                               AudioHelp::getInstance()->playEffect("part_map.mp3");
                                                                               ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flower1.plist");
//                                                                               pHouse->addChild(pParticle,10);
//                                                                               pParticle->setPosition(Vec2::ZERO);
                                                                               m_pScrollView->getInnerContainer()->addChild(pParticle,10);
                                                                               Vec2 pos = pHouse->getParent()->convertToWorldSpace(pHouse->getPosition());
                                                                               pos = m_pScrollView->getInnerContainer()->convertToNodeSpace(pos);
                                                                               pParticle->setPosition(pos);
                                                                           }),
                                                          DelayTime::create(0.5),
                                                          CallFunc::create([=]()
                                                                           {                                                                               std::stringstream ostrCSB;
                                                                               ostrCSB<<"House"<<tag<<".csb";
                                                                               auto action = CSLoader::createTimeline(ostrCSB.str(), pHouse);
                                                                               action->play("animation0", true);
                                                                               pHouse->runAction(action);
                                                                           }), NULL));
                     }else{
                         float dt = 0.5;
                         if (GameDataManager::getInstance()->getMapLocked()) {
                             dt = 2.5;
                         }
                         this->runAction(Sequence::create(DelayTime::create(dt),
                                                          CallFunc::create([=]()
                                                                           {
                                                                               std::stringstream ostrCSB;
                                                                               ostrCSB<<"House"<<tag<<".csb";
                                                                               auto action = CSLoader::createTimeline(ostrCSB.str(), pHouse);
                                                                               action->play("animation0", true);
                                                                               pHouse->runAction(action);
                                                                           }), NULL));
                     }
                     
                     
                     
                 }
             });
//    auto action2 = CSLoader::createTimeline("HouseShop.csb", m_pHouseShop);
//    action2->play("animation0", true);
//    m_pHouseShop->runAction(action2);
}
void MapScene::_showHouse()
{
    log("=====_showHouse====");
    Node* pHouseRoot = m_MapLayer->getChildByName("house");
    for (int i = 0; i<5; i++) {
        std::stringstream ostr;
        ostr<<"house"<<i;
        Node* pHouse = pHouseRoot->getChildByName(ostr.str());
        if (pHouse) {
            //            log("============%d",i);
            pHouse->setTag(i);
            m_Houses.pushBack(pHouse);
            Button* pButton = dynamic_cast<Button*>(pHouse->getChildByName("Button"));
            if(pButton){
                pButton->setTag(i);
                pButton->addTouchEventListener(CC_CALLBACK_2(MapScene::onButtonCallback, this));
                if (GameDataManager::getInstance()->getMapLocked()){
                    pHouse->setPosition(Vec2(Vec2(0, -visibleSize.height*1.5), pHouse->getPosition()));
                    float dt = 0.5+(5-i)*0.3;
                    log("========%f",dt);
                    log("=====_showHouse %d====",i);
                    pHouse->runAction(Sequence::create(DelayTime::create(dt),
                                                       MoveBy::create(0.5, Vec2(0, -visibleSize.height*1.5)),
                                                       CallFunc::create([=](){
                        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                    }),
                                                       ScaleTo::create(0.3, 1,0.9),
                                                       EaseBackOut::create(ScaleTo::create(0.5, 1)), NULL));
                }
            }
        }
    }
}
void MapScene::_showShopHouse()
{
    log("=====_showShopHouse====");
    Node* pHouseRoot = m_MapLayer->getChildByName("house");
    
    m_pHouseShop = pHouseRoot->getChildByName("house_shop");
    Button* pButton = dynamic_cast<Button*>(m_pHouseShop->getChildByName("Button"));
    
    if (gNoneIap) {
        m_pHouseShop->setVisible(false);
        pButton->setTouchEnabled(false);
        pButton->setEnabled(false);
    }else{
        pButton->setTag(5);
        pButton->addTouchEventListener(CC_CALLBACK_2(MapScene::onButtonCallback, this));
        if (GameDataManager::getInstance()->getMapLocked()){
            m_pHouseShop->setPosition(Vec2(Vec2(0, -visibleSize.height*1.5), m_pHouseShop->getPosition()));
            m_pHouseShop->runAction(Sequence::create(DelayTime::create(1),
                                                     MoveBy::create(0.5, Vec2(0, -visibleSize.height*1.5)),
                                                     ScaleTo::create(0.3, 1,0.9),
                                                     EaseBackOut::create(ScaleTo::create(0.5, 1)), NULL));
        }
    }
}