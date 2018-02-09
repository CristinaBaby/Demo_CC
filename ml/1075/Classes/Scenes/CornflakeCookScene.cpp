
#include "CornflakeCookScene.h"
#include "SceneManager.h"

CornflakeCookScene::CornflakeCookScene()
{
    m_nStep = 0;
    m_nMoveCount = 0;
}

CornflakeCookScene::~CornflakeCookScene()
{
    
}
bool CornflakeCookScene::init()
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
    
    _showStove();
    _showPan();
    
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        _showIngredient(0);
        _showIngredient(1);
        AudioHelp::getInstance()->playEffect("vo_add_water.mp3");
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_add_corn_pan.mp3");
    }),NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void CornflakeCookScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CornflakeCookScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CornflakeCookScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "ingredient"){
        int tag = pDragNode->getTag();
        IngredientManager::getInstance()->showShadow(tag,false);
        if (tag == 8) {
            pDragNode->setDragSprite(Sprite::create(localPath("flour_spoon.png")));
            ContainerNode* pNode = (ContainerNode*)pDragNode->getUserData();
            if (!pNode) {
                pNode = ContainerNode::create(pDragNode);
            }
            pNode->addFood(localPath("flour_spoon1.png"));
            pDragNode->setUserData(pNode);
        }
    }else if (name == "shovel"){
        if (pDragNode->index==0) {
            pDragNode->getDragSprite()->setTexture(localPath("spatula_1.png"));
        }
    }
}

void CornflakeCookScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setScale(1);
    std::string name = pDragNode->getName();
    if (name == "shovel"){
        int index = pDragNode->index;
        pDragNode->setLocalZOrder(8);
        Rect bowlRect = m_pPan->getBoundingBox();
        bowlRect.size = Size(400,bowlRect.size.height);
#pragma mark        搅拌
        m_pGuideLayer->removeGuide();
        if(index == 0){
            if (bowlRect.containsPoint(worldPoint)) {
                AudioHelp::getInstance()->playLoopEffect("liquid_mixing.mp3");
                MixManager::getInstance()->mix();
            }else{
                AudioHelp::getInstance()->stopLoopEffect();
            }
#pragma mark       舀起corn
        }else if (index == 1) {
            if (bowlRect.containsPoint(worldPoint)){
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pPanIn->getPosition()+Vec2(200, 200)),
                                                      CallFunc::create([=](){
                    pDragNode->setLocalZOrder(0);
                }),
                                                      MoveBy::create(1, Vec2(-100, -100)),
                                                      CallFunc::create([=](){
                    m_pPanIn->setVisible(false);
                    Sprite* pCorn = Sprite::create(localPath("cornmeal6.png"));
                    pDragNode->getDragSprite()->addChild(pCorn);
                    pCorn->setName("corn");
                    pCorn->setPosition(pDragNode->getActionPoint());
                    pCorn->setScale(0.8);
                    pCorn->setRotation(-90);
                    pDragNode->index = 2;
                    m_pGuideLayer->removeGuide();
                    m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pSievel->getPosition()+Vec2(-100, 130));
                    m_pShovel->setActionPoint(Vec2(50, 36));
                    m_pShovel->setLimitRect(Rect::ZERO);
                    pDragNode->setTouchEnabled(true);
                }), NULL));
            }
#pragma mark       将corn倒入筛子
        }else if (index == 2) {
            Rect bowlRect = m_pSievel->getDragSprite()->getBoundingBox();
            bowlRect.origin = m_pSievel->convertToWorldSpace(bowlRect.origin);
            if (bowlRect.containsPoint(worldPoint)) {
                m_pGuideLayer->removeGuide();
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pSievel->getPosition()+Vec2(0, 300)),
                                                      RotateBy::create(0.5, -30),
                                                      CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("drag_corn.mp3");
                    Node* pNode = pDragNode->getDragSprite()->getChildByName("corn");
                    if (pNode) {
                        pNode->removeFromParent();
                    }
                    Sprite* pCorn = Sprite::create(localPath("cornmeal6.png"));
                    m_pCornLayer = MakeEatLayer::create();
                    Size size = Size(pCorn->getContentSize().width, pCorn->getContentSize().height);
                    MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
                    pCorn->setPosition(Vec2(size.width/2, size.height/2));
                    rt->beginWithClear(0, 0, 0, 0);
                    pCorn->visit();
                    rt->end();
                    Director::getInstance()->getRenderer()->render();
                    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
                    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
                    m_pCornLayer->setRenderTexture(rt);
                    m_pCornLayer->setPosition(m_pSievel->getActionPoint()+Vec2(-size.width/2, -size.height/2));
                    
                    m_pSievel->getDragSprite()->addChild(m_pCornLayer);
                    m_pCornLayer->setName("corn");
                    m_pCornLayer->setShapeTexture(Sprite::create(localPath("cornflake0.png"))->getTexture());
                    m_pCornLayer->setLocalZOrder(4);
                    
                    m_pSievel->runAction(MoveBy::create(1, Vec2(0, 200)));
                    AudioHelp::getInstance()->playEffect("vo_great.mp3");
                }),
                                                      MoveBy::create(1, Vec2(100, 200)),
                                                      CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("vo_smash_corn_pancake_pieces.mp3");
                    pDragNode->index = 3;
                    m_pGuideLayer->removeGuide();
                    m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pSievel->getPosition()+Vec2(-100, 130));
                    pDragNode->setTouchEnabled(true);
                }), NULL));
            }
#pragma mark       用铲子捣碎corn
        }else if (index == 3) {
            Rect rect = m_pSievel->getDragSprite()->getBoundingBox();
            rect.origin = m_pSievel->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                if (m_pCornLayer->getRenderTexture()->getAlphaWithLocation(m_pCornLayer->convertToNodeSpace(worldPoint))<=0){
                    return;
                }
                m_nMoveCount++;
                if (m_nMoveCount%10==0) {
                    AudioHelp::getInstance()->playEffect("smash.mp3");
                    m_pCornLayer->doEat(worldPoint);
                    //                m_pCornLayer->doEat(worldPoint+Vec2(50, 50));
                    //                m_pCornLayer->doEat(worldPoint+Vec2(50, -50));
                    //                m_pCornLayer->doEat(worldPoint+Vec2(-50, -50));
                    //                m_pCornLayer->doEat(worldPoint+Vec2(-50, 50));
                    
                    for (int i=0; i<5; i++) {
                        Sprite* pSlice = Sprite::create(localPath("cornflake0.png"));
                        m_pSievel->getDragSprite()->addChild(pSlice);
                        pSlice->setPosition(m_pSievel->getDragSprite()->convertToNodeSpace(worldPoint));
                        pSlice->setScale(1.2);
                        Vec2 pos = m_pSievel->getActionPoint();
                        pSlice->setLocalZOrder(1);
                        pSlice->runAction(Spawn::create(RotateBy::create(0.6, -30+6*(arc4random()%10)),
                                                        MoveTo::create(0.5, Vec2(pSlice->getPositionX()-50.0+10.0*(arc4random()%10),pos.y-80.0+16.0*(arc4random()%10))), NULL));
                    }
                    int percent = m_pCornLayer->getPercent();
                    if (percent>=80) {
                        m_pShovel->setTouchEnabled(false);
                        m_pCornLayer->setVisible(false);
                        m_pShovel->getDragSprite()->removeAllChildren();
                        
                        m_pShovel->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                        m_pSievel->runAction(JumpBy::create(1, Vec2::ZERO, 50, 4));
                        m_pSievel->runAction(ActionHelper::createShakeAction());
                        
                        for(auto pSlice:m_pSievel->getDragSprite()->getChildren()){
                            pSlice->runAction(JumpTo::create(2, m_pSievel->getActionPoint(), 50, 4));
//                            pSlice->runAction(MoveTo::create(3, m_pSievel->getActionPoint()));
                            pSlice->runAction(Sequence::create(DelayTime::create(1),
                                                               CallFunc::create([=](){
                                pSlice->removeFromParent();
                            }), NULL));
                        }
                        this->runAction(Sequence::create(DelayTime::create(1),
                                                         CallFunc::create([=](){
                            
                            m_pGuideLayer->removeGuide();
                            ClippingNode* pClipping = ClippingNode::create(Sprite::create(localPath("colander_mask.png")));
                            Sprite* pFinish = Sprite::create(localPath("cornflake1.png"));
                            pClipping->addChild(pFinish);
                            pClipping->setName("clipping");
                            pFinish->setOpacity(0);
                            pFinish->runAction(FadeIn::create(0.5));
                            m_pSievel->addChild(pClipping);
                            pClipping->setAlphaThreshold(0.5);
                            _finish();
                            
                        }), NULL));
                    }else if (percent>60) {
//                        if (m_pSievel->getChildByName("clipping")) {
//                            return;
//                        }
                    }
                }
            }
        }
    }
}

void CornflakeCookScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    AudioHelp::getInstance()->stopLoopEffect();
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        int tag = pDragNode->getTag();
        Rect rectBowl = m_pPan->getBoundingBox();
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)){
            _addIngredient(pDragNode);
            m_pGuideLayer->removeGuide();
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),[=](){
                IngredientManager::getInstance()->showShadow(pDragNode->getTag(),true);
                pDragNode->setTouchEnabled(true);
            });
        }
        
    }else if (name == "shovel"){
        int index = pDragNode->index;
        if(index == 1){
            AudioHelp::getInstance()->stopLoopEffect();
        }
    }
}

void CornflakeCookScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CornflakeCookScene::_initData()
{
    setExPath("content/make/cornmeal/");
}

void CornflakeCookScene::_showStove()
{
    m_pStove = Sprite::create("content/make/common/stove/stove.png");
    this->addChildToContentLayer(m_pStove);
    CMVisibleRect::setPositionAdapted(m_pStove, 320, 320);
    
    m_pFire = Sprite::create("content/make/common/stove/stove_1.png");
    this->addChildToContentLayer(m_pFire);
    m_pFire->setPosition(m_pStove->getPosition()+Vec2(0,-10));
    
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/common/stove/stove_", 1, 2)));
}
void CornflakeCookScene::_showPan()
{
    m_pPan = Sprite::create("content/make/common/stove/pan_down.png");
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320+150+visibleSize.width, 420);
    
    m_pPanFront = Sprite::create("content/make/common/stove/pan_on.png");
    this->addChildToContentLayer(m_pPanFront);
    m_pPanFront->setPosition(m_pPan->getPosition());
    m_pPanFront->setLocalZOrder(5);
    
    m_pPan->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    m_pPanFront->runAction(Sequence::create(DelayTime::create(0.5),
                                            MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    
    m_pPanIn = Node::create();
    m_pPanIn->setLocalZOrder(1);
    this->addChildToContentLayer(m_pPanIn);
    CMVisibleRect::setPositionAdapted(m_pPanIn, 320, 400);
    
}

void CornflakeCookScene::_showSieve()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    AudioHelp::getInstance()->playEffect("vo_move_pancake_sieve.mp3");
    m_pSievel = _createDrageNode(localPath("colander.png"));
    this->addChildToContentLayer(m_pSievel);
    CMVisibleRect::setPositionAdapted(m_pSievel, 420+visibleSize.width, 200);
    m_pSievel->setActionPoint(Vec2(180, 216));
    m_pSievel->setName("sievel");
    m_pSievel->setTouchEnabled(false);
    m_pSievel->setLocalZOrder(5);
    m_pSievel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pSievel->getPosition(), m_pPanIn->getPosition());
//        m_pSievel->setTouchEnabled(true);
    }), NULL));
    
//    KettleNode* pContainer = KettleNode::create(m_pSievel);
//    pContainer->addWater(localRootPath("stir7.png"), localRootPath("bowl_mask.png"));
//    pContainer->addFront(localRootPath("bowl_on.png"));
//    m_pSievel->setUserData(pContainer);
    
}

void CornflakeCookScene::_showShovel()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pShovel = _createDrageNode(localPath("spatula.png"));
    this->addChildToContentLayer(m_pShovel);
    CMVisibleRect::setPositionAdapted(m_pShovel, 400+visibleSize.width, 600);
    m_pShovel->setTouchEnabled(false);
    m_pShovel->setActionPoint(Vec2(60, 76));
    m_pShovel->setName("shovel");
    m_pShovel->setLocalZOrder(20);
    m_pShovel->getDragSprite()->runAction(RotateBy::create(1.3, 40));
    Rect bowlRect = m_pPan->getBoundingBox();
    bowlRect.origin = bowlRect.origin+Vec2(150, 80);
    bowlRect.size = Size(400,bowlRect.size.height);
    m_pShovel->setLimitRect(bowlRect);
    m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pShovel->getPosition(), m_pPanIn->getPosition());
        m_pShovel->setTouchEnabled(true);
    }), NULL));
    
    MixManager::getInstance()->reset();
    MixManager::getInstance()->_mixtureStartIndex = 0;
    MixManager::getInstance()->_mixCount = 3;
    MixManager::getInstance()->_mixSpeed = 100;
    MixManager::getInstance()->_mixRotateSpeed = 0;
    MixManager::getInstance()->_mixturePathEX = localPath("cornmeal");
    MixManager::getInstance()->_mixtureNode = m_pPanIn;
//    MixManager::getInstance()->_mixtureSize = m_pPanIn->getContentSize();
    MixManager::getInstance()->onMixCallback = [=](int index){
        if (index==1){
            AudioHelp::getInstance()->playLoopEffect("water_boiling.mp3");
            
        }else if (index==2) {
            AudioHelp::getInstance()->playEffect("vo_keep_stirring_smooth.mp3");
            
            AudioHelp::getInstance()->playEffect("magic_effect.mp3");
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            m_pPanIn->addChild(pParticle);
            pParticle->setPosition(Vec2::ZERO);
        }
    };
    MixManager::getInstance()->onScheduleCallback = [=](int index){
        //        m_pProgressStar->addStar();
    };
    MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(CornflakeCookScene::_finishMix, this);
}

void CornflakeCookScene::_showPlate()
{
    if (m_pPlate) {
        return;
    }
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320+visibleSize.width, 680);
    m_pPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
}

void CornflakeCookScene::_showIngredient(int index)
{
    //    back shadow front food food2
    static std::string sIngredents[][6] ={
        {"kettle1.png","kettle0.png","kettle2.png","kettle_water.png","","kettle_mask.png"}, //water
        {"cornmeal_box0.png"}, //batter
        {"sugar1.png","sugar1_2.png","sugar1_1.png"}, //sugar
    };
    
    bool isWater = false;
    if (index == 0) {
        isWater = true;
    }
    Vec2 actionPoint[] = {
        Vec2(8, 209),
        Vec2(16, 180),
        Vec2(10, 165),
    };
    Vec2 pos[] = {
        Vec2(220, 750),
        Vec2(430, 750),
        Vec2(320, 750),
    };
    DragNode* pDragNode = _createDrageNode(localPath(sIngredents[index][0]));
    this->addChildToContentLayer(pDragNode);
    pDragNode->setTag(index);
    pDragNode->setLocalZOrder(index);
    pDragNode->setName("ingredient");
    pDragNode->setTouchEnabled(true);
    pDragNode->setActionPoint(actionPoint[index]);
    CMVisibleRect::setPositionAdapted(pDragNode, pos[index]+Vec2(visibleSize.width,0));
    
    IngredientManager::getInstance()->addIngredient(pDragNode);
    IngredientManager::getInstance()->addContainer(pDragNode,localPath(""), sIngredents[index],isWater);
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    pDragNode->runAction(action);
    
}

void CornflakeCookScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "water2.png",
        "cornmeal0.png",
        "bowl_sugar.png",
    };
    int index = pIngredient->getTag();
    IngredientManager::getInstance()->frozeIngredients(index);
    int k = index;
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setTouchEnabled(false);
    
    switch (index) {
#pragma mark =========  add water ==========
        case 0:
        {
            posAdd = Vec2(0, 0);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPanIn->getPosition()+Vec2(30, 250)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pouring_juice.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/water2.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                KettleNode* pContainer = (KettleNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourWaterOnce(100,2);
                }
            }),
                                                    MoveBy::create(2, Vec2(200, 20)),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->_playEffectNiceWork();
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 2) {
                    _showShovel();
                }
                MenuLayer::addIngredient(MenuLayer::ePackageCorn, MenuLayer::eCorn_water);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add Corn ==========
        case 1:
        {
            
            posAdd = Vec2(0, 0);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPanIn->getPosition()+Vec2(0, 250)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pour_cereal.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/corn_dot.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
            }),
                                                    MoveBy::create(2, Vec2(200, 20)),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                
                AudioHelp::getInstance()->playEffect("vo_good.mp3");
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 2) {
                    _showShovel();
                }
                MenuLayer::addIngredient(MenuLayer::ePackageCorn, MenuLayer::eCorn_corn);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
            
#pragma mark =========  add sugar ==========
        case 2:
        {
            posAdd = Vec2(40, -30);
            Node* pLid = pIngredient->getChildByName("front");
            if(pLid){
                AudioHelp::getInstance()->playEffect("bottle_open.mp3");
                pLid->runAction(Sequence::create(JumpBy::create(1, Vec2(100, 600), visibleSize.height, 1),
                                                 CallFunc::create([=](){
                    pLid->setVisible(false);
                }), NULL));
            }
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPanIn->getPosition()+Vec2(30, 150)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pour_cereal.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_sugar.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                pIngredient->getDragSprite()->setTexture(localPath("sugar2.png"));
            }),
                                                    MoveBy::create(2,Vec2(80, 50)),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
                pIngredient->getDragSprite()->setTexture(localPath("sugar1.png"));
                AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_good_job.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                          CallFunc::create([=](){
                        m_pShovel->getDragSprite()->setTexture(localPath("spatula_1.png"));
                        m_pShovel->setTouchEnabled(true);
                    }), NULL));
                    
                }
                MenuLayer::addIngredient(MenuLayer::ePackageCorn, MenuLayer::eCorn_sugar);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;

        default:
            break;
    }
    
    Sprite* pAdd = Sprite::create(localPath(lNameStr));
    if (pAdd) {
        pAdd->setPosition(posAdd);
        pAdd->setOpacity(0);
        m_pPanIn->addChild(pAdd,5);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         FadeIn::create(1),
                                         CallFunc::create([=](){
            m_pGuideLayer->removeGuide();
        }), NULL));
    }
}

void CornflakeCookScene::_ingredientOut(DragNode* pNode)
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    IngredientManager::getInstance()->frozeIngredients(pNode->getTag(),false);
}

void CornflakeCookScene::_doFryCornmeal(Sprite* pFood)
{
    pFood->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("omelet"), 1, 5,false,true,2),
                                      CallFunc::create([=](){
        _showShovel();
        _showPlate();
    }), NULL));
}

void CornflakeCookScene::_finishMix()
{
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->_playEffectNiceWork();
    if (m_nStep==2) {
        m_pShovel->setTouchEnabled(false);
        m_pShovel->getDragSprite()->setTexture(localPath("spatula.png"));
        m_pShovel->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        
        _showIngredient(2);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        AudioHelp::getInstance()->playEffect("vo_add_sugar.mp3");
        
        MixManager::getInstance()->reset();
        MixManager::getInstance()->_mixtureStartIndex = 3;
        MixManager::getInstance()->_mixCount = 1;
        MixManager::getInstance()->_mixSpeed = 200;
        MixManager::getInstance()->_mixRotateSpeed = 0;
        MixManager::getInstance()->_mixturePathEX = localPath("cornmeal");
        MixManager::getInstance()->_mixtureNode = m_pPanIn;
//        MixManager::getInstance()->_mixtureSize = m_pBowl->getContentSize();
        MixManager::getInstance()->onMixCallback = [=](int index){
        };
        MixManager::getInstance()->onScheduleCallback = [=](int index){
            //        m_pProgressStar->addStar();
        };
        MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(CornflakeCookScene::_finishMix, this);
    }else if (m_nStep==3) {
        AudioHelp::getInstance()->playEffect("ding.mp3");
        m_pShovel->setTouchEnabled(false);
        m_pShovel->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        m_pShovel->getDragSprite()->setTexture(localPath("spatula.png"));
        
        m_pPanIn->removeAllChildren();
        Sprite* pFood = Sprite::create(localPath("cornmeal4.png"));
        m_pPanIn->addChild(pFood);
        pFood->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("cornmeal"), 4, 6,false,true,4),
                                          CallFunc::create([=](){
            
            m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                  CallFunc::create([=](){
//                m_pShovel->getDragSprite()->setTexture(localPath("spatula_1.png"));
                m_pShovel->index = 1;
                m_pShovel->setTouchEnabled(true);
            }), NULL));
            _showSieve();
        }), NULL));
    }else{
    }
}

void CornflakeCookScene::_finish()
{
    AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<CornflakeAddMilkScene>();
    }), NULL));
    
}