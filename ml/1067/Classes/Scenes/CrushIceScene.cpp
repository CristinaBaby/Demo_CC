
#include "CrushIceScene.h"
#include "PolygonHelp.h"


CrushIceScene::CrushIceScene()
{
    updateCount = 0;
}

CrushIceScene::~CrushIceScene()
{
    
}
bool CrushIceScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    Sprite* pBg = Sprite::create("content/make/make_bg.jpg");
    this->addChildToBGLayer(pBg);
    pBg->setPosition(visibleSize*0.5);
    
    m_pTable = Sprite::create(localPath("table.png"));
    CMVisibleRect::setPositionAdapted(m_pTable, 320, 150);
    this->addChildToContentLayer(m_pTable);

    m_pMachine = MachineNode::create();
    this->addChildToContentLayer(m_pMachine);
    CMVisibleRect::setPosition(m_pMachine, 320, 300-80);
    m_pMachine->setVisible(false);
    m_pMachine->setTouchEnable(false);
    m_pMachine->onFinishedCallback = CC_CALLBACK_0(CrushIceScene::_finish, this);
    m_pMachine->onBeginCallback = [=](){
        m_pGuideLayer->removeGuide();
    };
//    _showMachineUnit();
    
    Sprite* pMachine0 = Sprite::create(localPath("machine/machine0_1.png"));
    this->addChildToContentLayer(pMachine0);
    CMVisibleRect::setPosition(pMachine0, 320+visibleSize.width, 300-80);
    pMachine0->setLocalZOrder(1);
    Sprite* pMachine0_1 = Sprite::create(localPath("machine/machine0_2.png"));
    pMachine0->addChild(pMachine0_1);
    pMachine0_1->setPosition(pMachine0->getContentSize()*0.5);
    
    pMachine0->runAction(Sequence::create(DelayTime::create(0.3),
                                          EaseSineOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                               pMachine0->setVisible(false);
                                                               m_pMachine->setVisible(true);
                                                               _addMachineUnit();
                                                           }), NULL));
    
//    scheduleUpdate();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    
    return true;
}

void CrushIceScene::onEnter()
{
    ExtensionScene::onEnter();
    
//    Scene* scene = this->getScene();
    
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
//    scene->getPhysicsWorld()->setGravity(Vect(0,0));
}

void CrushIceScene::update( float delta)
{
}
#pragma mark - initData
void CrushIceScene::_initData()
{
    setExPath("content/make/ice/");
    m_nNextSceneTag = GameUIEvent::eSceneTagMap;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagMap;
    m_nIceShape = GameDataManager::getInstance()->m_nShapeIndex;
}

void CrushIceScene::_showMachineUnit()
{
    Sprite* pMachine0 = Sprite::create(localPath("machine/machine0_1.png"));
    this->addChildToContentLayer(pMachine0);
    CMVisibleRect::setPosition(pMachine0, 460+visibleSize.width, 230+visibleSize.height);
    pMachine0->setScale(0.6);
    pMachine0->setLocalZOrder(1);
    Sprite* pMachine0_1 = Sprite::create(localPath("machine/machine0_2.png"));
    pMachine0->addChild(pMachine0_1);
    pMachine0_1->setPosition(pMachine0->getContentSize()*0.5);
    
    Sprite* pMachine1 = Sprite::create(localPath("machine/machine1_0.png"));
    this->addChildToContentLayer(pMachine1);
    CMVisibleRect::setPosition(pMachine1, 140-visibleSize.width, 400+visibleSize.height);
    
    Sprite* pMachine2 = Sprite::create(localPath("machine/machine2_0.png"));
    this->addChildToContentLayer(pMachine2);
    CMVisibleRect::setPosition(pMachine2, 480+visibleSize.width, 500+visibleSize.height);
    
    Sprite* pCone = Sprite::create(localPath("machine/cone0.png"));
    this->addChildToContentLayer(pCone);
    CMVisibleRect::setPosition(pCone, 130-visibleSize.width, 200+visibleSize.height);
    pCone->setLocalZOrder(1);
    
    pMachine0->runAction(Sequence::create(DelayTime::create(0.3),
                                          EaseSineOut::create(MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height))),
                                          JumpBy::create(1, Vec2::ZERO, 30, 2),
                                          DelayTime::create(4),
                                          Spawn::create(MoveTo::create(0.5, CMVisibleRect::getPosition(320, 300)),
                                                        ScaleTo::create(0.5, 1), NULL),
                                          CallFunc::create([=]()
                                                           {
                                                               pMachine0->setVisible(false);
                                                               m_pMachine->setVisible(true);
                                                           }),
                                          DelayTime::create(2),
                                          CallFunc::create([=]()
                                                           {
                                                               _addMachineUnit();
                                                           }), NULL));
    
    pMachine1->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseSineOut::create(MoveBy::create(1, Vec2(visibleSize.width, -visibleSize.height))),
                                          JumpBy::create(1, Vec2::ZERO, 30, 2),
                                          DelayTime::create(3),
                                          EaseSineIn::create(MoveBy::create(1, Vec2(-visibleSize.width, visibleSize.height))), NULL));
    
    pMachine2->runAction(Sequence::create(DelayTime::create(0.8),
                                          EaseSineOut::create(MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height))),
                                          JumpBy::create(1, Vec2::ZERO, 30, 2),
                                          DelayTime::create(3),
                                          EaseSineIn::create(MoveBy::create(1, Vec2(visibleSize.width, visibleSize.height))), NULL));
    
    pCone->runAction(Sequence::create(DelayTime::create(1),
                                      EaseSineOut::create(MoveBy::create(1, Vec2(visibleSize.width, -visibleSize.height))),
                                      JumpBy::create(1, Vec2::ZERO, 30, 2),
                                      DelayTime::create(3),
                                      EaseSineIn::create(MoveBy::create(1, Vec2(-visibleSize.width, visibleSize.height))), NULL));
}

void CrushIceScene::_addMachineUnit(int index)
{
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=]()
                                                      {
                                                          AudioHelp::getInstance()->playEffect("tools_fall.mp3");
                                                      }), NULL));
    if(index==0) {
        Sprite* pMachine0 = Sprite::create(localPath("machine/machine1_1.png"));
        this->addChildToContentLayer(pMachine0);
        CMVisibleRect::setPosition(pMachine0, 320+visibleSize.width, 500+visibleSize.height);
        pMachine0->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height)),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pMachine->addUnit(1);
                                                                   pMachine0->removeFromParent();
                                                               }), NULL));
        
        Sprite* pMachine2 = Sprite::create(localPath("machine/cone1_0.png"));
        this->addChildToContentLayer(pMachine2);
        CMVisibleRect::setPosition(pMachine2, 300-visibleSize.width, 300+visibleSize.height);
        pMachine2->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(1, Vec2(visibleSize.width, -visibleSize.height)),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pMachine->addUnit(2);
                                                                   pMachine2->removeFromParent();
                                                                   _addIce();
                                                               }), NULL));
        
    }else if(index==1){
        
        Sprite* pMachine0 = Sprite::create(localPath("machine/machine2_2.png"));
        this->addChildToContentLayer(pMachine0);
        CMVisibleRect::setPosition(pMachine0, 300+visibleSize.width, 800+visibleSize.height);
        pMachine0->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height)),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pMachine->addUnit(3);
                                                                   pMachine0->removeFromParent();
                                                               }), NULL));
        
        Sprite* pMachine2 = Sprite::create(localPath("machine/machine2_1.png"));
        this->addChildToContentLayer(pMachine2);
        CMVisibleRect::setPosition(pMachine2, 300+visibleSize.width, 900+visibleSize.height);
        pMachine2->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height)),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pMachine->addUnit(4);
                                                                   pMachine2->removeFromParent();
                                                                   m_pMachine->setTouchEnable(true);
                                                                   Rect rect = m_pMachine->getBowlWorldRect();
                                                                   rect.origin = m_pUILayer->convertToNodeSpace(rect.origin);
                                                                   
                                                                   m_pGuideLayer->showGuideMix(Vec2(rect.getMidX(), rect.getMaxY()+100),2);
                                                               }), NULL));
    }
}

void CrushIceScene::_addIce()
{
    AudioHelp::getInstance()->playLoopEffect("pour_ice.mp3");
    _produceIce();
    schedule(schedule_selector(CrushIceScene::_produceIce), 0.5);
}

void CrushIceScene::_produceIce(float dt)
{
    for (int i = 0; i<3; i++) {
        stringstream ostr;
        ostr<<"ice"<<m_nIceShape<<".png";
        Sprite* pIce = Sprite::create(localPath(ostr.str()));
        m_IceVector.pushBack(pIce);
    
        stringstream ostrShape;
        ostrShape<<"ice"<<m_nIceShape;
        m_pMachine->addChild(pIce);
    
        pIce->setScale(0.4+0.1*(arc4random()%3));
    Rect rect = m_pMachine->getBowlWorldRect();
    rect.origin = pIce->getParent()->convertToNodeSpace(rect.origin);
    float radom = 20.0*(arc4random()%8);
        float angle = 20.0*(arc4random()%5);
        pIce->setPosition(m_pMachine->convertToNodeSpace(Vec2(visibleSize.width/2, visibleSize.height+20)));
        pIce->runAction(Sequence::create(DelayTime::create(0.1+0.1*i),
                                         Spawn::create(EaseBackOut::create(MoveTo::create(1+0.2*i,Vec2(rect.getMidX()-70+radom, rect.getMidY()+m_IceVector.size()*3))),
                                                       RotateBy::create(1.3+0.2*i, angle), NULL), NULL));
    }
    if (m_IceVector.size()>15) {
        unschedule(schedule_selector(CrushIceScene::_produceIce));
        AudioHelp::getInstance()->stopLoopEffect();
        _addMachineUnit(1);
    }
}

void CrushIceScene::_finish()
{
    AudioHelp::getInstance()->playEffect("finish_particle.mp3");
    int radoom = arc4random()%100;
    if (radoom>50) {
        AudioHelp::getInstance()->playEffect("fantastic.mp3");
    }else{
        AudioHelp::getInstance()->playEffect("wonderful.mp3");
    }
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    m_pGameUI->showNextLayout();
    GameDataManager::getInstance()->setStepCount(2);
}