
#include "JuiceMakeScene.h"
#include "SceneManager.h"
#include "PolygonHelp.h"

JuiceMakeScene::JuiceMakeScene()
{
    
}

JuiceMakeScene::~JuiceMakeScene()
{
    
}
bool JuiceMakeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create("content/make/common/bg1.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pGameUI->showNormalLayout();
    
    return true;
}

void JuiceMakeScene::onEnter()
{
    ExtensionScene::onEnter();
    Scene* scene = this->getScene();
    
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
    scene->getPhysicsWorld()->setAutoStep(false);
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(JuiceCutFruitScene::updateStart), 2);
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _showBlender();
    }), NULL));
}

void JuiceMakeScene::updateStart(float delta)
{
    Scene* scene = this->getScene();
    scene->getPhysicsWorld()->setAutoStep(false);
    scheduleUpdate();
}
void JuiceMakeScene::update(float delta)

{
    
    // use fixed time and calculate 3 times per frame makes physics simulate more precisely.
    
    //这里表示先走3步瞧瞧  如果fps是1/60  三个setp就是1/180
    
    for (int i = 0; i < 3; ++i)
        
    {
        Scene* scene = this->getScene();
        scene->getPhysicsWorld()->step(1/180.0f);
        
    }
    
}
void JuiceMakeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void JuiceMakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void JuiceMakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void JuiceMakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="bowl") {
        Rect rect = m_pBlender->getCup()->getCupWorldRect();
        if (rect.containsPoint(worldPoint)) {
            _pourFruits();
        }
    }else if (name=="water") {
        Rect rect = m_pBlender->getCup()->getCupWorldRect();
        rect.size = Size(rect.size.width, rect.size.height+100);
        if (rect.containsPoint(worldPoint)) {
            _pourWater();
        }
    }
}

void JuiceMakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void JuiceMakeScene::_initData()
{
    setExPath("content/make/juice/");
    m_nFlavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
}

void JuiceMakeScene::_showBowl()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pBowl = _createDrageNode(localPath("bowl0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 490+visibleSize.width, 300);
    
    m_pBowl->setTouchEnabled(false);
    m_pBowl->setName("bowl");
    m_pBowl->runAction(Sequence::create(DelayTime::create(1),
                                        MoveBy::create(1.0,Vec2(-visibleSize.width,0)),
                                        CallFunc::create([=](){
        m_pBowl->setOrgPositionDefault();
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pBowl->getPosition(), m_pBlender->getPosition()+Vec2(0, 250));
        m_pBowl->setTouchEnabled(true);
    }),
                                        DelayTime::create(0.2),
                                        CallFunc::create([=](){
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        PolygonHelp::getInstance()->addShapesWithFile("fruit_bowl", body,-m_pBowl->getContentSize().width,-m_pBowl->getContentSize().height);
        body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
        m_pBowl->setPhysicsBody(body);
        
        for(auto pSlice:m_Fruits) {
            auto body = PhysicsBody::createCircle(MIN(pSlice->getContentSize().width, pSlice->getContentSize().height)*0.1,PhysicsMaterial(1.0f, 0.0f, 1.0f));
            body->setDynamic(true);
            pSlice->setPhysicsBody(body);
        }
    }),NULL));
    m_pBowlFront = Sprite::create(localPath("bowl2.png"));
    m_pBowl->addChild(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    
    
    for (int k = 0; k<14; k++) {
        std::string Path;
        if (m_nFlavorIndex==0 || m_nFlavorIndex==3) {
            Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(k%2+1)+".png";
        }else if (m_nFlavorIndex==1) {
            Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(k%2+2)+".png";
        }else if (m_nFlavorIndex == 2 || m_nFlavorIndex == 6) {
            Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_2.png";
        }else if (m_nFlavorIndex == 4 || m_nFlavorIndex == 5) {
            Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_4.png";
        }else{
            Path = "cut/fruit"+std::to_string(m_nFlavorIndex)+"_"+std::to_string(k%2+4)+".png";
        }
        Sprite* pSlice = Sprite::create(localPath(Path));
        m_pBowl->addChild(pSlice);
        pSlice->setPosition(Vec2(50-10.0*(arc4random()%10), 5.0*(arc4random()%10)));
        pSlice->setLocalZOrder(5);
        pSlice->setRotation(50.0-10.0*(arc4random()%10));
        pSlice->setScale(0.5);
        m_Fruits.pushBack(pSlice);
    }
}

void JuiceMakeScene::_showBlender()
{
    int flavor = GameDataManager::getInstance()->m_nFlavorIndex;
    int index = 1;
    m_pBlender = BlenderNode::create(index);
    this->addChildToContentLayer(m_pBlender);
    CMVisibleRect::setPosition(m_pBlender, 280+visibleSize.width, 400);
    m_pCup = m_pBlender->getCup();
    m_pCup->initWithIndex(flavor);
    m_pCup->setWaterPercent(100);
    if(0==index){
        m_pCup->onFinishCrushIce = CC_CALLBACK_0(JuiceMakeScene::_finish, this);
    }else{
        m_pBlender->onBlenderSwitchTurnOff = CC_CALLBACK_0(JuiceMakeScene::_finish, this);
        m_pBlender->onBlenderStopWorking = [=](){
            if (index==0) {
            }else{
                m_pGuideLayer->removeGuide();
                m_pBlender->getCup()->finishedCrush();
                _finish();
            }
        };
    }
    m_pCup->onBeginCrushIce = CC_CALLBACK_0(JuiceMakeScene::_beginCrush, this);
    m_pCup->onLidOpen = CC_CALLBACK_0(JuiceMakeScene::_blenderLidOpen, this);
    
//    int shapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
//    Node* pCocosNode = CSLoader::createNode("Modul"+std::to_string(shapeIndex)+".csb");
//    int iceCount = pCocosNode->getChildrenCount()-1;
    
    m_pCup->showLid(true,false);
    m_pBlender->runAction(Sequence::create(DelayTime::create(0.5),
                                           CallFunc::create([=]{
        AudioHelp::getInstance()->playEffect("slide_in_out.mp3");
    }),
                                           MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           CallFunc::create([=]{
        Rect rect = m_pCup->getCupWorldRect();
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMaxY()));
        m_pCup->setTouchEnable();
    }),
                                           DelayTime::create(0.2),
                                           CallFunc::create([=]{
//        m_pBlender->setReadyWork();
    }), NULL));
    
}

void JuiceMakeScene::_showWater()
{
    AudioHelp::getInstance()->playEffect("vo_add_water2blender.mp3");
    m_pWater = _createDrageNode(localPath("kettle0.png"));
    this->addChildToContentLayer(m_pWater);
    m_pWater->setName("water");
    m_pWater->setTouchEnabled(true);
    m_pWater->setActionPoint(Vec2(8, 208));
    CMVisibleRect::setPositionAdapted(m_pWater, Vec2(510+visibleSize.width,320));
    
    KettleNode* pContainer = KettleNode::create(m_pWater);
    pContainer->addWater(localPath("kettle_water.png"), localPath("kettle_mask.png"));
    pContainer->addFront(localPath("kettle2.png"));
    m_pWater->setUserData(pContainer);
    
    
    m_pWater->runAction(Sequence::create(DelayTime::create(1),
                                         MoveBy::create(1.0,Vec2(-visibleSize.width,0)),
                                         CallFunc::create([=](){
        m_pWater->setOrgPositionDefault();
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pWater->getPosition(), m_pBlender->getPosition()+Vec2(0, 250));
        m_pWater->setTouchEnabled(true);
    }),NULL));
}

void JuiceMakeScene::_pourFruits()
{
    m_pBowl->setTouchEnabled(false);
    m_pGuideLayer->removeGuide();
    
    m_pBowl->runAction(Sequence::create(MoveTo::create(0.5, m_pBlender->getCup()->getCupWorldPos()+Vec2(200, 100)),
                                        RotateBy::create(0.5, -120),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("fall_down.mp3");
        m_pBlender->getCup()->setBodyData();
    }),
                                        DelayTime::create(1),
                                        CallFunc::create([=](){
    }),
                                        DelayTime::create(1),
                                        CallFunc::create([=](){
        int range = 2;
        if (m_nFlavorIndex==2 || (m_nFlavorIndex>3 && m_nFlavorIndex<7)) {
            range = 1;
        }
        m_pCup->addFruits(m_nFlavorIndex,range,20);
        
        
        for(auto pSlice:m_Fruits) {
            pSlice->removeFromParent();
        }
    }),
                                        RotateTo::create(0.5, 0),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->_playEffectNiceWork();
        
        m_pBlender->runAction(MoveBy::create(0.5, Vec2(60, 0)));
    }),
                                        MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=](){
        _showWater();
    }), NULL));
}

void JuiceMakeScene::_pourWater()
{
    m_pGuideLayer->removeGuide();
    m_pWater->runAction(Sequence::create(MoveTo::create(0.5, m_pBlender->getCup()->getCupWorldPos()+Vec2(250, 100)),
                                            RotateTo::create(0.5,-90),
                                            CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("pouring_juice.mp3");
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/water2.plist");
        m_pWater->addChild(pParticle);
        pParticle->setPosition(m_pWater->getActionPoint());
        pParticle->setName("particle");
        pParticle->setRotation(90);
        pParticle->setLocalZOrder(20);
        pParticle->setPosition(m_pWater->convertToNodeSpace(m_pWater->getWorldSpaceActionPoint()));
        
        KettleNode* pContainer = (KettleNode*)m_pWater->getUserData();
        if (pContainer) {
            pContainer->pourWaterOnce(100,2);
        }
        m_pBlender->getCup()->addWater();
    }),
                                            MoveBy::create(2, Vec2(50, 20)),
                                            CallFunc::create([=](){
        auto particle = dynamic_cast<ParticleSystem*>(m_pWater->getChildByName("particle"));
        if (particle) {
            particle->stopSystem();
            particle->removeFromParent();
        }
    }),
                                            RotateTo::create(1, 0),
                                            CallFunc::create([=](){
        AudioHelp::getInstance()->_playEffectNiceWork();
    }),
                                            MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        MenuLayer::addIngredient(MenuLayer::ePackageJuice, MenuLayer::eJuice_water);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        m_pBlender->getCup()->showLid(true,true);
        m_pBlender->runAction(Sequence::create(DelayTime::create(1),
                                               CallFunc::create([=]{
            m_pBlender->setReadyWork();
        }),NULL));
        
        Rect rect = m_pBlender->getButtonWorldRect();
        m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMidY()),1);
    }), NULL));
}

void JuiceMakeScene::_beginCrush()
{
    
    m_pGuideLayer->removeGuide();
}

void JuiceMakeScene::_blenderLidOpen()
{
    m_pBlender->runAction(MoveBy::create(0.5, Vec2(-60, 0)));
    _showBowl();
    m_pGuideLayer->removeGuide();
}
void JuiceMakeScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<JuicePourScene>();
    }), NULL));
    
}