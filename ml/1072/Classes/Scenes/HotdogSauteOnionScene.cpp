
#include "HotdogSauteOnionScene.h"
#include "PolygonHelp.h"
#include "SceneManager.h"

HotdogSauteOnionScene::HotdogSauteOnionScene()
{
    m_nAddOilCount = 0;
    m_nMoveCount = 0;
    m_nAddOnionCount = 0;
}

HotdogSauteOnionScene::~HotdogSauteOnionScene()
{
    
}
bool HotdogSauteOnionScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_make3.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showStove();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=]{
        AudioHelp::getInstance()->playEffect("vo_hotdog_fry_onion.mp3");
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=]{
        _showPan();
        _showOil();
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void HotdogSauteOnionScene::onEnter()
{
    ExtensionScene::onEnter();
    Scene* scene = this->getScene();
    
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0,0));
//    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
    auto world = scene->getPhysicsWorld();
//    world->setAutoStep(false);//物理世界自动同步关掉
    //创建一个物理世界, 大小和屏幕的尺寸相同, 使用默认材质, debug框的宽度为3个像素
//    auto body = PhysicsBody::createEdgeBox( visibleSize*0.5, PHYSICSBODY_MATERIAL_DEFAULT, 3);
//    //创建一个碰撞图形
//    auto edgeShape = Node::create();
//    
//    //将图形和刚刚创建的世界绑定
//    edgeShape->setPhysicsBody( body);
//    
//    //设置图形的位置在屏幕正中间
//    edgeShape->setPosition( visibleSize.width / 2, visibleSize.height / 2);
//    
//    //添加进图层
//    addChild( edgeShape);

//    scheduleUpdate();
}

void HotdogSauteOnionScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void HotdogSauteOnionScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name=="oil_spoon") {
        if (pDragNode->index==0){
            pDragNode->setDragSprite(Sprite::create(localPath("oil_poon1_1.png")));
        }
        
    }else if (name=="onion_plate"){
        pDragNode->getDragSprite()->setTexture(localPath("plate_onions1.png"));
    }else if (name=="sugar_spoon"){
        if (pDragNode->index==0){
            pDragNode->setDragSprite(Sprite::create(localPath("suger_spoon1.png")));
        }
    }else if (name=="garlic_bowl"){
        pDragNode->getDragSprite()->setTexture(localPath("garlic1.png"));
    }
}

void HotdogSauteOnionScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="oil_spoon") {
        if (pDragNode->index==0) {
            Rect rect = m_pOilBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())) {
                pDragNode->index = 1;
                pDragNode->setPosition(m_pOilBowl->getPosition()+Vec2(60, 20));
                pDragNode->setOrgPositionDefault();
                
                pDragNode->setTouchEnabled(false);
                this->runAction(Sequence::create(DelayTime::create(0.3),
                                                 CallFunc::create([=](){
                    pDragNode->setTouchEnabled(true);
                }), NULL));
                Sprite* pOil = (Sprite*)pDragNode->getChildByName("oil");
                if (!pOil) {
                    pOil = Sprite::create(localPath("oil_poon1_2.png"));
                    pDragNode->addChild(pOil);
                    pOil->setName("oil");
                }
                pOil->setScale(1);
                pOil->setOpacity(255);
                pOil->setPosition(Vec2::ZERO);
            }
        }else{
            Rect rect = m_pStove->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pOil->getPosition()+Vec2(50,200));
                
                AudioHelp::getInstance()->playEffect("add_water.mp3");
                if(m_nAddOilCount==0){
                    m_pOil->setScale(0.2);
                    m_pOil->runAction(Sequence::create(FadeIn::create(1),
                                                       ScaleTo::create(1, 0.6), NULL));
                }else if (m_nAddOilCount==1) {
                    m_pOil->runAction(ScaleTo::create(2, 1));
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fry_oil.plist");
                    m_pOil->addChild(pParticle);
                    pParticle->setPosition(m_pOil->getAnchorPointInPoints());
                }
                Sprite* pOilStream = Sprite::create(localPath("oil_poon1_2.png"));
                pDragNode->addChild(pOilStream);
                pOilStream->setName("stream");
                pOilStream->runAction(AnimationHelp::createAnimate(localPath("oil_poon1_"), 3, 4,false));
                
                Node* pOil = pDragNode->getChildByName("oil");
                if (pOil) {
                    pOil->runAction(Sequence::create(DelayTime::create(0.5),
                                                     Spawn::create(MoveBy::create(2, Vec2(-60, -20)),
                                                                   ScaleTo::create(2, 0.3), NULL),
                                                     FadeOut::create(0.3),
                                                     CallFunc::create([=](){
                        pOilStream->removeFromParent();
                        if(m_nAddOilCount==0){
                            pDragNode->back(1,[=](){
                                pDragNode->index = 0;
                                pDragNode->setTouchEnabled(true);
                            });
                        }else if (m_nAddOilCount==1){
                            _finishAddOil();
                        }
                        m_nAddOilCount++;
                    }), NULL));
                }
            }
        }
    }else if(name == "shovel"){
        Rect rect = m_pPan->getBoundingBox();
        
        if (rect.containsPoint(worldPoint)) {
            m_nMoveCount++;
            m_pOnionNode->setRotation(m_pOnionNode->getRotation()+1);
        }
        static int count = 600;
        if (m_nMoveCount>count*3){
            m_pSugarInPan->setOpacity(m_pSugarInPan->getOpacity()-5<0?0:m_pSugarInPan->getOpacity()-5);
            m_pGarlicInPan->setOpacity(m_pGarlicInPan->getOpacity()-5<0?0:m_pGarlicInPan->getOpacity()-5);
        }
        if (m_nMoveCount>count*4){
            if (pDragNode->index==1) {
                Rect rect = m_pPan->getBoundingBox();
                rect.origin = m_pPan->getParent()->convertToWorldSpace(rect.origin);
                if (rect.containsPoint(worldPoint)){
                    if((m_nMoveCount-count*4)%10==0){
                        pDragNode->index = 2;
                        m_nMoveCount = count*4;
                        Sprite* pOnion = (Sprite*)m_pShovel->getChildByName("onion");
                        if ((!pOnion)) {
                            pOnion = Sprite::create(localPath("onions_cooked_shovel.png"));
                            m_pShovel->addChild(pOnion);
                            pOnion->setName("onion");
                            pOnion->setPosition(m_pShovel->convertToNodeSpace(worldPoint));
                        }
                        pOnion->setOpacity(255);
                        
                        Vector<Node*> pChildren = Vector<Node*>(m_pOnionNode->getChildren());
                        int count = pChildren.size();
                        if (m_nAddOnionCount==0){
                            count = count/2;
                        }
                        for (int i = 0; i<count; i++) {
                            Node* pNode = pChildren.at(i);
                            pNode->removeFromParent();
                        }
                    }
                }
                
            }else if (pDragNode->index==2){
                Rect rect = m_pPlate->getBoundingBox();
                rect.origin = m_pPlate->getParent()->convertToWorldSpace(rect.origin);
                if (rect.containsPoint(worldPoint)) {
                    pDragNode->index = 1;
                    pDragNode->setTouchEnabled(false);
                    m_nAddOnionCount++;
                    pDragNode->setPosition(m_pPlate->getPosition()+Vec2(120, 80));
                    Sprite* pOnion = (Sprite*)m_pShovel->getChildByName("onion");
                    if ((pOnion)) {
                        pOnion->runAction(FadeOut::create(0.5));
                    }
                    if (m_nAddOnionCount==2){
                        Sprite* pPaperInPlate = Sprite::create(localPath("paper_1.png"));
                        m_pPlate->addChild(pPaperInPlate);
                        pPaperInPlate->setPosition(m_pPlate->getAnchorPointInPoints());
                        pPaperInPlate->setOpacityModifyRGB(0);
                        pPaperInPlate->runAction(FadeIn::create(1));
                        Sprite* pOnionInPlate = Sprite::create(localPath("onions_cooked_shovel.png"));
                        m_pPlate->addChild(pOnionInPlate,2);
                        pOnionInPlate->setPosition(m_pPlate->getAnchorPointInPoints());
                    }else{
                        Sprite* pOnionInPlate = Sprite::create(localPath("onions_cooked.png"));
                        m_pPlate->addChild(pOnionInPlate,2);
                        pOnionInPlate->setPosition(m_pPlate->getAnchorPointInPoints());
                    }
                    this->runAction(Sequence::create(DelayTime::create(0.5),
                                                     CallFunc::create([=](){
                        if (m_nAddOnionCount==1) {
                            pDragNode->setTouchEnabled(true);
                        }else{
                            pDragNode->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                                  CallFunc::create([=](){
                                _finishAll();
                            }), NULL));
                        }
                    }), NULL));
                }
                
            }
        }
        if (m_nMoveCount%count==0 && m_nMoveCount<=count*4 && pDragNode->index==0) {
            static int onionShape[4] = {5,10,10,9};
            int step = m_nMoveCount/count;
            if (step>4){
                return;
            }
            Vector<Node*> pChildren = m_pOnionNode->getChildren();
            int k = 0;
            for_each(pChildren.begin(), pChildren.end(), [=,&k](Node* pNode){
                Sprite* pSlice = dynamic_cast<Sprite*>(pNode);
                if (pSlice) {
                    log("============%s",localPath("onion/"+std::to_string(step+1)+"_"+std::to_string(k%onionShape[step-1]+1)+".png").c_str());
                    pSlice->runAction(Sequence::create(DelayTime::create(0.1+k*0.005),
                                                       CallFunc::create([=](){
                        pSlice->setTexture(localPath("onion/"+std::to_string(step+1)+"_"+std::to_string(k%onionShape[step-1]+1)+".png"));
                    }), NULL));
                }
                k++;
            });
            if (step==3) {
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(MoveBy::create(1, Vec2(visibleSize.width*1.5, 0)));
                this->runAction(Sequence::create(DelayTime::create(1.5),
                                                      CallFunc::create([=](){
                    _showBrownSugar();
                }), NULL));
            }else if (step==4) {
//                pDragNode->setTouchEnabled(false);
//                pDragNode->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width*1.5, 0)),
//                                                      CallFunc::create([=](){
//                    _showPlate();
//                    pDragNode->index = 1;
//                }), NULL));
                pDragNode->ignoreMoving = true;
                m_pOil->runAction(Sequence::create(FadeTo::create(0.5,50),
                                                   CallFunc::create([=](){
                    m_pOil->removeAllChildren();
//                    m_pOil->setVisible(false);
                }), NULL));
                AudioHelp::getInstance()->playEffect("vo_smell_it.mp3");
                this->runAction(Sequence::create(DelayTime::create(1.5),
                                                 CallFunc::create([=](){
                    _showPlate();
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    pDragNode->index = 1;
                    pDragNode->ignoreMoving = false;
                    AudioHelp::getInstance()->playEffect("vo_hotdog_onion2paper.mp3");
                }),NULL));
            }
        }
    }else if (name=="sugar_spoon") {
        if (pDragNode->index==0) {
            Rect rect = m_pSugarBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())) {
                pDragNode->index = 1;
                pDragNode->setPosition(m_pSugarBowl->getPosition()+Vec2(40, 30));
                pDragNode->setOrgPositionDefault();
                
                pDragNode->setTouchEnabled(false);
                this->runAction(Sequence::create(DelayTime::create(0.5),
                                                 CallFunc::create([=](){
                    pDragNode->setTouchEnabled(true);
                }), NULL));
                
                Sprite* pSugar = (Sprite*)pDragNode->getChildByName("sugar");
                if (!pSugar) {
                    pSugar = Sprite::create(localPath("suger_spoon2.png"));
                    pDragNode->addChild(pSugar);
                    pSugar->setName("sugar");
                }
                pSugar->setScale(1);
                pSugar->setOpacity(255);
                pSugar->setPosition(Vec2::ZERO);
            }
        }else{
            Rect rect = m_pStove->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pOil->getPosition()+Vec2(150,120));
                _pourBrownSugar();
            }
        }
    }
}

void HotdogSauteOnionScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="onion_plate"){
        Rect rect = m_pPan->getBoundingBox();
        pDragNode->setTouchEnabled(false);
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setPosition(m_pPan->getPosition()+Vec2(-50, 200));
            pDragNode->setLocalZOrder(1);
            pDragNode->runAction(Sequence::create(RotateBy::create(0.5, 10),
                                                  CallFunc::create([=](){
                m_pOnionInPlate->runAction(ScaleTo::create(1, 0.4));
                m_pOnionInPlate->runAction(Sequence::create(MoveBy::create(1.5, Vec2(50, -30)),
                                                            FadeOut::create(0.3), NULL));
                _pourOnion();
            }),
                                                  DelayTime::create(2),
                                                  CallFunc::create([=]{
                
            }),
                                                  RotateTo::create(0.5, 1),
                                                  MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
        }else{
            pDragNode->back(0,[=](){
                pDragNode->getDragSprite()->setTexture(localPath("plate_onions.png"));
                pDragNode->setTouchEnabled(true);
            });
        }
    }else if (name=="garlic_bowl"){
        Rect rect = m_pPan->getBoundingBox();
        pDragNode->setTouchEnabled(false);
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setPosition(m_pPan->getPosition()+Vec2(100, 200));
            pDragNode->runAction(Sequence::create(RotateBy::create(0.5, 10),
                                                  CallFunc::create([=](){
                m_pGarlicInBowl->runAction(ScaleTo::create(1, 0.4));
                m_pGarlicInBowl->runAction(Sequence::create(MoveBy::create(1.5, Vec2(-50, -30)),
                                                            FadeOut::create(0.3), NULL));
                _pourGarlic();
            }),
                                                  DelayTime::create(2),
                                                  CallFunc::create([=]{
                
            }),
                                                  RotateTo::create(0.5, 1),
                                                  MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
        }else{
            pDragNode->back(0,[=](){
                pDragNode->getDragSprite()->setTexture(localPath("garlic2.png"));
                pDragNode->setTouchEnabled(true);
            });
        }
    }
    
}

void HotdogSauteOnionScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void HotdogSauteOnionScene::_initData()
{
    setExPath("content/make/hotdog/1/");
}

void HotdogSauteOnionScene::_showStove()
{
    m_pStove = Sprite::create("content/make/common/stove/stove.png");
    CMVisibleRect::setPosition(m_pStove, 320, 300);
    this->addChildToContentLayer(m_pStove);
    
    m_pFire = Sprite::create("content/make/common/stove/stove_1.png");
    CMVisibleRect::setPosition(m_pFire, 320, 300);
    this->addChildToContentLayer(m_pFire);
    
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/common/stove/stove_", 1, 2)));
}

void HotdogSauteOnionScene::_showPan()
{
    m_pPan = Sprite::create(localPath("pan.png"));
    CMVisibleRect::setPosition(m_pPan, 200-visibleSize.width, 300);
    this->addChildToContentLayer(m_pPan);
    m_pPan->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    m_pOil = Sprite::create(localPath("pan_oil.png"));
    CMVisibleRect::setPosition(m_pOil, 300, 320);
    this->addChildToContentLayer(m_pOil);
    m_pOil->setOpacity(0);
    
    m_pOnionNode = Node::create();
    this->addChildToContentLayer(m_pOnionNode);
    m_pOnionNode->setLocalZOrder(5);
    m_pOnionNode->setPosition(m_pOil->getPosition());
    
}

void HotdogSauteOnionScene::_showOil()
{
    AudioHelp::getInstance()->playEffect("vo_add_hotdog_oil.mp3");
    m_pOilBowl = Sprite::create(localPath("oil.png"));
    CMVisibleRect::setPosition(m_pOilBowl, 240+visibleSize.width, 790);
    this->addChildToContentLayer(m_pOilBowl);
    m_pOilBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pOilSpoon = _createDrageNode(localPath("oil_spoon.png"));
    CMVisibleRect::setPosition(m_pOilSpoon, 490+visibleSize.width, 770);
    m_pOilSpoon->setLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height-100));
    this->addChildToContentLayer(m_pOilSpoon);
    m_pOilSpoon->setName("oil_spoon");
    
    m_pOilSpoon->setTouchEnabled(false);
    m_pOilSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pOilSpoon->setOrgPositionDefault();
        m_pOilSpoon->setTouchEnabled(true);
    }), NULL));
}
void HotdogSauteOnionScene::_showOnion()
{
    AudioHelp::getInstance()->playEffect("vo_hotdog_pour_onion.mp3");
    
    m_pOnionPlate = _createDrageNode(localPath("plate_onions.png"));
    this->addChildToContentLayer(m_pOnionPlate);
    CMVisibleRect::setPosition(m_pOnionPlate, 240+visibleSize.width, 790);
    m_pOnionPlate->setTouchEnabled(false);
    m_pOnionPlate->setName("onion_plate");
    
    m_pOnionPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pOnionPlate->setOrgPositionDefault();
        m_pOnionPlate->setTouchEnabled(true);
    }), NULL));
    
    m_pOnionInPlate = Sprite::create(localPath("onions.png"));
    m_pOnionPlate->addChild(m_pOnionInPlate);
}

void HotdogSauteOnionScene::_showShovel()
{
    m_pShovel = _createDrageNode(localPath("shovel.png"));
    this->addChildToContentLayer(m_pShovel);
    CMVisibleRect::setPositionAdapted(m_pShovel, 320+visibleSize.width, 350);
    m_pShovel->setTouchEnabled(false);
    m_pShovel->setName("shovel");
    m_pShovel->setLocalZOrder(10);
    m_pShovel->setActionPoint(Vec2(77, 80));
    
    m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pShovel->setOrgPositionDefault();
        m_pShovel->setTouchEnabled(true);
        
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        
        PolygonHelp::getInstance()->addShapesWithFile("sholve", body,-m_pShovel->getDragSprite()->getContentSize().width/2,-m_pShovel->getDragSprite()->getContentSize().height/2);
        body->getFirstShape()->setMaterial(PhysicsMaterial(1.0f, 0.5f, 1.0f));
        m_pShovel->getDragSprite()->setPhysicsBody(body);
    }), NULL));
}

void HotdogSauteOnionScene::_showBrownSugar()
{
    AudioHelp::getInstance()->playEffect("vo_add_hotdog_sugar.mp3");
    m_pSugarBowl = Sprite::create(localPath("suger_bowl.png"));
    CMVisibleRect::setPosition(m_pSugarBowl, 300+visibleSize.width, 800);
    this->addChildToContentLayer(m_pSugarBowl);
    m_pSugarBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pSugarSpoon = _createDrageNode(localPath("suger_spoon.png"));
    CMVisibleRect::setPosition(m_pSugarSpoon, 490+visibleSize.width, 770);
    this->addChildToContentLayer(m_pSugarSpoon);
    m_pSugarSpoon->setName("sugar_spoon");
    m_pSugarSpoon->setActionPoint(Vec2(18, 90));
    
    m_pSugarSpoon->setTouchEnabled(false);
    m_pSugarSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pSugarSpoon->setOrgPositionDefault();
        m_pSugarSpoon->setTouchEnabled(true);
    }), NULL));
    
}

void HotdogSauteOnionScene::_showGarlic()
{
    
    m_pGarlicBowl = _createDrageNode(localPath("garlic2.png"));
    this->addChildToContentLayer(m_pGarlicBowl);
    CMVisibleRect::setPosition(m_pGarlicBowl, 240+visibleSize.width, 790);
    m_pGarlicBowl->setTouchEnabled(false);
    m_pGarlicBowl->setName("garlic_bowl");
    m_pGarlicBowl->setActionPoint(Vec2(67, 71));
    
    m_pGarlicBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pGarlicBowl->setOrgPositionDefault();
        m_pGarlicBowl->setTouchEnabled(true);
    }), NULL));
    
    m_pGarlicInBowl = Sprite::create(localPath("garlic.png"));
    m_pGarlicBowl->addChild(m_pGarlicInBowl);

}

void HotdogSauteOnionScene::_showPlate()
{
    m_pPlate = Sprite::create(localPath("onions_cooked_plate.png"));
    CMVisibleRect::setPosition(m_pPlate, 360+visibleSize.width, 820);
    this->addChildToContentLayer(m_pPlate);
    m_pPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pPaper = Sprite::create(localPath("paper.png"));
    m_pPlate->addChild(m_pPaper);
    m_pPaper->setPosition(m_pPlate->getAnchorPointInPoints());
}

void HotdogSauteOnionScene::_pourOnion()
{
    AudioHelp::getInstance()->playEffect("buter.mp3");
    Vec2 pos = m_pOnionPlate->getPosition()+Vec2(50, -50);
    pos = m_pOnionNode->convertToNodeSpace(pos);
    for (int i = 0; i<200; i++) {
        Sprite* pSlice;
        if (i<4) {
            pSlice = Sprite::create(localPath("onion/"+std::to_string(i%2+1)+".png"));
        }else{
            pSlice = Sprite::create(localPath("onion/1_"+std::to_string(i%5+1)+".png"));
        }
        m_pOnionNode->addChild(pSlice);
        float rotation = 5.0*(arc4random()%80);
        float scale = 0.5+0.1*(arc4random()%10);
        float x = -90.0+10.0*(arc4random()%18);
        float y = 90.0-10.0*(arc4random()%18);
        Vec2 movePos = Vec2(x, y);
        pSlice->setPosition(pos);
        pSlice->setRotation(rotation);
        pSlice->setTag(i);
//        pSlice->setScale(scale);
        pSlice->setOpacity(0);
        pSlice->retain();
        pSlice->runAction(Sequence::create(DelayTime::create(0.1+0.01*i),
                                           FadeIn::create(0.1),
                                           MoveTo::create(1, movePos),
                                           DelayTime::create(0.5),
                                           CallFunc::create([=](){
            if(i%4==0){
                float length = MAX(pSlice->getContentSize().width*0.5,pSlice->getContentSize().height*0.5);
                log("===========length %f=====",length);
                auto body = PhysicsBody::createBox(Size(40,40),PhysicsMaterial(0.1f, 1.0f, 0.0f));
                //            auto body = PhysicsBody::createCircle(MIN(pSprite->getContentSize().width, pSprite->getContentSize().height)*0.6,PhysicsMaterial(0.8f, 0.1f, 100.8f));
                body->setDynamic(true);
                pSlice->setPhysicsBody(body);
                pSlice->setLocalZOrder(2);
                //            pSlice->setPosition(movePos);
            }
        }), NULL));
    }
    
    this->runAction(Sequence::create(DelayTime::create(5),
                                     CallFunc::create([=](){
        _finishPourOnion();
    }), NULL));
}

void HotdogSauteOnionScene::update(float)
{
    Scene* scene = this->getScene();
    auto world = scene->getPhysicsWorld();
    world->step(0.001f);//step()不要传入dt（delta time）, 传入固定值，PhysicsBody才会稳定
}
void HotdogSauteOnionScene::_pourBrownSugar()
{
    AudioHelp::getInstance()->playPourPowderEffect();
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/brown_sugar.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pSugarSpoon->getWorldSpaceActionPoint()));
    pParticle->setLocalZOrder(10);
    Sprite* pSugar = (Sprite*)m_pSugarSpoon->getChildByName("sugar");
    if (pSugar) {
        pSugar->runAction(FadeOut::create(1.5));
    }
    m_pSugarInPan = Sprite::create(localPath("onion/pan_sugar.png"));
    this->addChildToContentLayer(m_pSugarInPan);
    m_pSugarInPan->setLocalZOrder(6);
    m_pSugarInPan->setPosition(m_pOil->getPosition()+Vec2(50, 20));
    m_pSugarInPan->setOpacity(0);
    m_pSugarInPan->runAction(FadeIn::create(1));
    m_pSugarInPan->runAction(Sequence::create(FadeIn::create(1),
                                               DelayTime::create(1),
                                              CallFunc::create([=](){
        m_pSugarSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
            m_pSugarSpoon->removeFromParent();
        }), NULL));
        m_pSugarBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
            MenuLayer::addIngredient(MenuLayer::ePackageHotdog, MenuLayer::eHotdog_sugar);
            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            
            _showGarlic();
            m_pSugarBowl->removeFromParent();
        }), NULL));
    }), NULL));
}

void HotdogSauteOnionScene::_pourGarlic()
{
    AudioHelp::getInstance()->playEffect("buter.mp3");
    for (int i = 0; i<4; i++) {
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/garlic.plist");
        pParticle->setTexture(Sprite::create(localPath("garlic_particle"+std::to_string(i+1)+".png"))->getTexture());
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pGarlicBowl->getWorldSpaceActionPoint()));
        pParticle->setLocalZOrder(10);
    }
    Sprite* pGarlic = (Sprite*)m_pGarlicBowl->getChildByName("garlic");
    if (pGarlic) {
        pGarlic->runAction(FadeOut::create(1));
    }
    
    m_pGarlicInPan = Sprite::create(localPath("onion/pan_garlic.png"));
    this->addChildToContentLayer(m_pGarlicInPan);
    m_pGarlicInPan->setLocalZOrder(6);
    m_pGarlicInPan->setPosition(m_pOil->getPosition()+Vec2(-50, -10));
    m_pGarlicInPan->setOpacity(0);
    m_pGarlicInPan->runAction(Sequence::create(FadeIn::create(1),
                                               DelayTime::create(1),
                                               CallFunc::create([=](){
        m_pGarlicBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width*1.5, 0)),
                                                  CallFunc::create([=](){
            m_pGarlicBowl->removeFromParent();
        }), NULL));
        m_pShovel->setTouchEnabled(false);
        m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width*1.5, 0)),
                                              CallFunc::create([=](){
            m_pShovel->setTouchEnabled(true);
        }), NULL));
        MenuLayer::addIngredient(MenuLayer::ePackageHotdog, MenuLayer::eHotdog_garlic);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    }), NULL));
}

void HotdogSauteOnionScene::_finishAddOil()
{
    MenuLayer::addIngredient(MenuLayer::ePackageHotdog, MenuLayer::eHotdog_oil);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    m_pOilSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pOilBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    _showOnion();
    
    auto body = PhysicsBody::create();
    body->setDynamic(false);
    
    PolygonHelp::getInstance()->addShapesWithFile("pan", body,-m_pPan->getContentSize().width/2,-m_pPan->getContentSize().height/2);
    body->getFirstShape()->setMaterial(PhysicsMaterial(1.0f, 0.0f, 0.01f));
    m_pPan->setPhysicsBody(body);
}

void HotdogSauteOnionScene::_finishPourOnion()
{
    MenuLayer::addIngredient(MenuLayer::ePackageHotdog, MenuLayer::eHotdog_onion);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    _showShovel();
}
void HotdogSauteOnionScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<HotdogGreaseScene>();
    }), NULL));
}
