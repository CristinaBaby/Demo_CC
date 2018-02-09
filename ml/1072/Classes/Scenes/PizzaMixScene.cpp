
#include "PizzaMixScene.h"
#include "SceneManager.h"

PizzaMixScene::PizzaMixScene()
{
    m_nAddCount = 0;
    m_nMixCount = 0 ;
    m_bWhriskIsReady = false;
}

PizzaMixScene::~PizzaMixScene()
{
    
}
bool PizzaMixScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    Sprite* pTable = Sprite::create(localPath("bk1.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320, 320);
    this->addChildToBGLayer(pTable);
    
    AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=]{
        _showMixIngredient(0);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=]{
        m_pGuideLayer->showGuideMove(m_pFlour->getPosition(), m_pFlourBagFront->getPosition());
    }), NULL));
    
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void PizzaMixScene::onEnter()
{
    ExtensionScene::onEnter();
}

void PizzaMixScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void PizzaMixScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "whrisk"){
        if (m_bWhriskIsReady){
            AudioHelp::getInstance()->playLoopEffect("stir.mp3");
            pDragNode->setLocalZOrder(5);
            
            Rect rect = m_pBowl->getBoundingBox();
            Rect limitRect = Rect(rect.origin.x+150, rect.origin.y+170, rect.size.width-220, rect.size.height*0.3);
            pDragNode->setLimitRect(limitRect);
            pDragNode->setOrgPositionDefault();
            pDragNode->setTouchEnabled(true);
            
            m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                         CallFunc::create([=](){
                m_pMixture->setScaleX(-m_pMixture->getScaleX());
            }), NULL)));
        }
    }
}

void PizzaMixScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "whrisk"){
        Rect rect = m_pBowl->getBoundingBox();
        rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            _mixBatter();
        }
        
    }else if (name == "flour") {
        if (pDragNode->index ==0) {
            Rect rect = m_pFlourBag->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())) {
                m_pGuideLayer->removeGuide();
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pFlourBag->getPosition()+Vec2(0, 200));
                pDragNode->setLocalZOrder(5);
                pDragNode->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(0, -200)),
                                                                    RotateBy::create(0.5, -50), NULL),
                                                      CallFunc::create([=](){
                    auto action = Sequence::create(ScaleTo::create(0.3, 1.1),
                                                   ScaleTo::create(0.3, 1),
                                                   ScaleTo::create(0.3, 1.1),
                                                   ScaleTo::create(0.3, 1),NULL);
                    m_pFlourBag->runAction(action);
                    m_pFlourBagFront->runAction(action->clone());
                    pDragNode->getDragSprite()->setTexture(localPath("measuring_cup1.png"));
                }),
                                                      DelayTime::create(0.5),
                                                      Spawn::create(MoveBy::create(0.5, Vec2(0, 300)),
                                                                    RotateBy::create(0.8, 50), NULL),
                                                      CallFunc::create([=](){
                    _createMixUI();
                    m_pFlourBagFront->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                                 CallFuncN::create([=](Node* pNode){
                        pNode->removeFromParent();
                    }), NULL));
                    m_pFlourBag->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                            CallFuncN::create([=](Node* pNode){
                        pNode->removeFromParent();
                    }), NULL));
                }),
                                                      DelayTime::create(1.2),
                                                      CallFunc::create([=](){
                    
                    pDragNode->setLocalZOrder(20);
                    pDragNode->index = 1;
                    pDragNode->setTouchEnabled(true);
                    pDragNode->runAction(MoveTo::create(1, Vec2(50, 200)+m_pBowl->getPosition()));
                }), NULL));
            }
        }
    }
}

void PizzaMixScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
    pDragNode->stopAllActions();
    
    std::string name = pDragNode->getName();
    if (name=="whrisk") {
        m_pWhrisk->stopAllActions();
        m_pMixture->stopAllActions();
        m_pMixture->setRotation(0);
    }else{
        if (name == "flour") {
            if (pDragNode->index==0) {
                return;
            }
        }
        Rect rect = m_pBowl->getBoundingBox();
        rect.size = rect.size+Size(100, 100);
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setTouchEnabled(false);
            
            _addIngredient(pDragNode);
        }else{
            _back(pDragNode);
        }
    }
}

void PizzaMixScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void PizzaMixScene::_initData()
{
    setExPath("content/make/pizza/0/");
}

#pragma mark ==== Mix UI =====
void PizzaMixScene::_createMixUI()
{
    m_pBowl = Sprite::create(localPath("bowl0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width/2, 400);
    
    m_pBowlFront = Sprite::create(localPath("bowl1.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    m_pBowlFront->setLocalZOrder(10);
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 320, 410);
    m_pMixture->setLocalZOrder(5);
    
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    
}

void PizzaMixScene::_showMixIngredient(int index)
{
    if (index==0) {
        AudioHelp::getInstance()->playEffect("vo_add_pizza_flour.mp3");
        m_pFlourBag = Sprite::create(localPath("flour1_0.png"));
        this->addChildToContentLayer(m_pFlourBag);
        CMVisibleRect::setPositionAdapted(m_pFlourBag, 260+visibleSize.width, 430);
        
        m_pFlourBagFront = Sprite::create(localPath("flour1_1.png"));
        this->addChildToContentLayer(m_pFlourBagFront);
        m_pFlourBagFront->setPosition(m_pFlourBag->getPosition());
        m_pFlourBagFront->setLocalZOrder(10);
        
        m_pFlour = _createDrageNode(localPath("measuring_cup0.png"),false);
        this->addChildToContentLayer(m_pFlour);
        CMVisibleRect::setPositionAdapted(m_pFlour, 450+visibleSize.width, 720);
        m_pFlour->setActionPoint(Vec2(8, 118));
        m_pFlour->setTag(index);
        m_pFlour->setName("flour");
        m_pFlour->setLocalZOrder(10);
        
        m_pFlourBag->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        m_pFlourBagFront->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        m_pFlour->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                             CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL));
    }else if (index==1) {
        AudioHelp::getInstance()->playEffect("vo_add_pizza_water.mp3");
        m_pWater = _createDrageNode(localPath("water1.png"),false);
        this->addChildToContentLayer(m_pWater);
        CMVisibleRect::setPositionAdapted(m_pWater, 320+visibleSize.width, 780);
        m_pWater->setActionPoint(Vec2(6, 259));
        m_pWater->setTag(index);
        m_pWater->setName("water");
        
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pWater->runAction(action->clone());
    }else if (index==2) {
        AudioHelp::getInstance()->playEffect("vo_add_pizza_yeast.mp3");
        m_pYeastBowl = Sprite::create(localPath("yeast.png"));
        this->addChildToContentLayer(m_pYeastBowl);
        CMVisibleRect::setPositionAdapted(m_pYeastBowl, 320+visibleSize.width, 700);
        
        m_pYeastSpoon = _createDrageNode(localPath("spoon_yeast.png"),false);
        this->addChildToContentLayer(m_pYeastSpoon);
        CMVisibleRect::setPositionAdapted(m_pYeastSpoon, 330+visibleSize.width, 760);
        m_pYeastSpoon->setActionPoint(Vec2(15, 20));
        m_pYeastSpoon->setTag(index);
        
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pYeastSpoon->runAction(action);
        m_pYeastBowl->runAction(action->clone());
    }else if (index==3) {
        AudioHelp::getInstance()->playEffect("vo_add_pizza_salt.mp3");
        m_pSaltBowl = Sprite::create(localPath("salt.png"));
        this->addChildToContentLayer(m_pSaltBowl);
        CMVisibleRect::setPositionAdapted(m_pSaltBowl, 320+visibleSize.width, 700);
        
        m_pSaltSpoon = _createDrageNode(localPath("spoon_salt.png"),false);
        this->addChildToContentLayer(m_pSaltSpoon);
        CMVisibleRect::setPositionAdapted(m_pSaltSpoon, 340+visibleSize.width, 770);
        m_pSaltSpoon->setActionPoint(Vec2(15, 20));
        m_pSaltSpoon->setTag(index);
        
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pSaltSpoon->runAction(action);
        m_pSaltBowl->runAction(action->clone());
    }else if (index==4) {
        AudioHelp::getInstance()->playEffect("vo_add_pizza_oil.mp3");
        m_pOilBowl = Sprite::create(localPath("oil.png"));
        this->addChildToContentLayer(m_pOilBowl);
        CMVisibleRect::setPositionAdapted(m_pOilBowl, 320+visibleSize.width, 700);
        
        m_pOilSpoon = _createDrageNode(localPath("spoon_oil.png"),false);
        this->addChildToContentLayer(m_pOilSpoon);
        CMVisibleRect::setPositionAdapted(m_pOilSpoon, 310+visibleSize.width, 750);
        m_pOilSpoon->setActionPoint(Vec2(15, 20));
        m_pOilSpoon->setTag(index);
        
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pOilSpoon->runAction(action);
        m_pOilBowl->runAction(action->clone());
    }else if (index == IngredientTag::eTagCloth) {
        m_pCloth = Sprite::create(localPath("cloth1.png"));
        this->addChildToContentLayer(m_pCloth);
        CMVisibleRect::setPositionAdapted(m_pCloth, 320+visibleSize.width, 400+visibleSize.height);
        m_pCloth->setScale(0.5);
        m_pCloth->setLocalZOrder(21);
        m_pCloth->runAction(Sequence::create(Spawn::create(MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height)),
                                                           ScaleTo::create(1.3, 1.1), NULL),
                                             ScaleTo::create(0.3, 1),
                                             CallFunc::create([=](){
            _ferment();
        }), NULL));
        
    }
}

void PizzaMixScene::_showWhisk()
{
    m_pWhrisk = _createDrageNode(localPath("wood_spoon0.png"));
    this->addChildToContentLayer(m_pWhrisk);
    CMVisibleRect::setPositionAdapted(m_pWhrisk, 320+visibleSize.width, 500);
    m_pWhrisk->setActionPoint(Vec2(58, 40));
    m_pWhrisk->setLocalZOrder(10);
    m_pWhrisk->setName("whrisk");
    
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                               m_pWhrisk->setOrgPositionDefault();
                                                               m_pWhrisk->setTouchEnabled(true);
                                                           }),
                                          NULL));
}

void PizzaMixScene::_removeWhisk()
{
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                           }),
                                          NULL));
}


#pragma mark ======== deel ========================
void PizzaMixScene::_back(DragNode* pIngredient)
{
    pIngredient->setTouchEnabled(false);
    pIngredient->back(0,[=](){
        ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
        if (pContainer) {
            pContainer->showShadow();
        }
        pIngredient->setTouchEnabled(true);
    });
}

void PizzaMixScene::_addIngredient(DragNode* pIngredient)
{
    int tag = pIngredient->getTag();
    std::string lNameStr = "";
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setLocalZOrder(5);
    switch (tag) {
#pragma mark === add Flour ===
        case IngredientTag::eTagFlour:
        {
            lNameStr = "flour2.png";
            pIngredient->setTouchEnabled(false);
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+180, m_pBowl->getPositionY()+250));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                pIngredient->getDragSprite()->setTexture(localPath("measuring_cup2.png"));
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pFlour->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
                
            }),
                                                    DelayTime::create(3),
                                                    CallFunc::create([=](){
                pIngredient->getDragSprite()->setTexture(localPath("measuring_cup3.png"));
            }),
                                                    DelayTime::create(1),
                                                    RotateBy::create(0.3, 80),
                                                    DelayTime::create(0.5),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                MenuLayer::addIngredient(MenuLayer::ePackagePizza, MenuLayer::ePizza_flour);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                _finishAdd(m_nAddCount);
                m_nAddCount++;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Water ===
        case IngredientTag::eTagWater:
        {
            lNameStr = "mix_water.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+120, m_pBowl->getPositionY()+330));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourMilkEffect();
                pIngredient->getDragSprite()->setTexture(localPath("water2.png"));
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/water2.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
                pParticle->setName("waterstream");
                
                ParticleSystemQuad* pWaterParticle = ParticleSystemQuad::create("particle/water.plist");
                this->addChildToContentLayer(pWaterParticle);
                pWaterParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pWaterParticle->setLocalZOrder(6);
                pWaterParticle->setName("waterdrop");
                
                Sprite* pFlourBowl = (Sprite*)m_pMixture->getChildByTag(tag-1);
                if (pFlourBowl) {
                    pFlourBowl->runAction(Sequence::create(DelayTime::create(1),
                                                       FadeOut::create(0.5), NULL));
                }
                Sprite* pFlour = Sprite::create(localPath("mix1.png"));
                if (pFlour) {
                    pFlour->setOpacity(0);
                    m_pMixture->addChild(pFlour,-1);
                    pFlour->runAction(Sequence::create(DelayTime::create(1),
                                                       FadeIn::create(0.5), NULL));
                }
            }),
                                                    DelayTime::create(3),
                                                    CallFunc::create([=](){
                pIngredient->getDragSprite()->setTexture(localPath("water3.png"));
            }),
                                                    DelayTime::create(1),
                                                    RotateTo::create(0.3, 0),
                                                    DelayTime::create(0.5),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                MenuLayer::addIngredient(MenuLayer::ePackagePizza, MenuLayer::ePizza_water);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                _finishAdd(m_nAddCount);
                m_nAddCount++;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
            
#pragma mark === add Yeast ===
        case IngredientTag::eTagYeast:
        {
            lNameStr = "";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+70, m_pBowl->getPositionY()+200));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, 20),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pour.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
                pParticle->setTexture(Sprite::create(localPath("yeast_particle.png"))->getTexture());
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
                
                Sprite* pOil = Sprite::create(localPath("yeast1.png"));
                m_pMixture->addChild(pOil);
                pOil->setOpacity(0);
                pOil->setScale(0.5);
                pOil->setPosition(Vec2(-20, -30));
                pOil->runAction(Sequence::create(DelayTime::create(0.5),
                                                 FadeIn::create(0.3),
                                                 DelayTime::create(1), NULL));
            }),
                                                    DelayTime::create(2),
                                                    CallFunc::create([=](){
                pIngredient->getDragSprite()->setTexture(localPath("spoon.png"));
            }),
                                                    DelayTime::create(0.5),
                                                    RotateBy::create(0.3, -20),
                                                    DelayTime::create(0.5),
                                                    CallFunc::create([=](){
                m_pYeastBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                         CallFuncN::create([=](Node* pNode){
                    pNode->removeFromParent();
                }), NULL));
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                MenuLayer::addIngredient(MenuLayer::ePackagePizza, MenuLayer::ePizza_yeast);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                
                _finishAdd(m_nAddCount);
                m_nAddCount++;
                pIngredient->removeFromParent();
            }),NULL));
        }
            break;
#pragma mark === add Salt ===
        case IngredientTag::eTagSalt:
        {
            lNameStr = "";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+70, m_pBowl->getPositionY()+200));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, 20),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pour.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/salt.plist");
                pParticle->setScaleX(-1);
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
                
                Sprite* pOil = Sprite::create(localPath("salt1.png"));
                m_pMixture->addChild(pOil);
                pOil->setOpacity(0);
                pOil->setScale(0.5);
                pOil->setPosition(Vec2(-50, 0));
                pOil->runAction(Sequence::create(DelayTime::create(0.5),
                                                 FadeIn::create(0.3),
                                                 DelayTime::create(1), NULL));
            }),
                                                    DelayTime::create(1.5),
                                                    CallFunc::create([=](){
                pIngredient->getDragSprite()->setTexture(localPath("spoon.png"));
            }),
                                                    RotateBy::create(0.3, -20),
                                                    DelayTime::create(0.5),
                                                    CallFunc::create([=](){
                m_pSaltBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                         CallFuncN::create([=](Node* pNode){
                    pNode->removeFromParent();
                }), NULL));
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                MenuLayer::addIngredient(MenuLayer::ePackagePizza, MenuLayer::ePizza_salt);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                _finishAdd(m_nAddCount);
                m_nAddCount++;
                pIngredient->removeFromParent();
            }),NULL));
        }
            break;
#pragma mark === add Oil ===
        case IngredientTag::eTagOil:
        {
            lNameStr = "";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+70, m_pBowl->getPositionY()+200));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, 20),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourMilkEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/oil_pour.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
                
                Sprite* pOil = Sprite::create(localPath("bowl_oil.png"));
                m_pMixture->addChild(pOil);
                pOil->setOpacity(0);
                pOil->setScale(0.5);
                pOil->setPosition(Vec2(-20, 10));
                pOil->runAction(Sequence::create(DelayTime::create(0.5),
                                                 FadeIn::create(0.3),
                                                 DelayTime::create(1), NULL));
            }),
                                                    DelayTime::create(1.5),
                                                    CallFunc::create([=](){
                pIngredient->getDragSprite()->setTexture(localPath("spoon.png"));
            }),
                                                    RotateBy::create(0.3, -20),
                                                    DelayTime::create(0.5),
                                                    CallFunc::create([=](){
                m_pOilBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                         CallFuncN::create([=](Node* pNode){
                    pNode->removeFromParent();
                }), NULL));
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                MenuLayer::addIngredient(MenuLayer::ePackagePizza, MenuLayer::ePizza_oil);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                _finishAdd(m_nAddCount);
                m_nAddCount++;
                pIngredient->removeFromParent();
            }),NULL));
        }
            break;
  
            
        default:
            break;
    }
    Sprite* pAdd = Sprite::create(localPath(lNameStr));
    if (pAdd) {
        pAdd->setOpacity(0);
        m_pMixture->addChild(pAdd);
        pAdd->setTag(tag);
        pAdd->setPosition(posAdd);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         CallFunc::create([=](){
            if(IngredientTag::eTagWater == tag){
                pAdd->runAction(ScaleTo::create(0.5, 1.3));
            }
        }),
                                         FadeIn::create(0.5), NULL));
    }
}

void PizzaMixScene::_mixBatter()
{
    if (m_bWhriskIsReady){
        m_nMixCount++;
        int count = 210;
        if (m_nMixCount%count==0) {
            int mixtureStep = m_nMixCount/count;
            int mixTotal = 4;
            if (mixtureStep<=mixTotal){
                std::stringstream ostr;
                ostr<<"mix"<<mixtureStep+1<<".png";
                Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                m_pMixture->removeAllChildren();
                m_pMixture->addChild(pMixture);
            }
            if (mixtureStep>=mixTotal){
                //mix finish
                m_pMixture->stopAllActions();
                m_pWhrisk->setDragSprite(Sprite::create(localPath("wood_spoon0.png")));
                m_pMixture->setScaleX(1);
                
                _finishMix();
                m_nMixCount = 0;
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }
    }else{
        Sprite *pShadow = (Sprite*)m_pWhrisk->getChildByName("shadow");
        if (pShadow){
            pShadow->removeFromParent();
        }
        
        m_bWhriskIsReady = true;
        AudioHelp::getInstance()->playLoopEffect("stir.mp3");
        m_pWhrisk->setDragSprite(Sprite::create(localPath("wood_spoon1.png")));
        m_pWhrisk->setLocalZOrder(5);
        m_pWhrisk->setRotation(0);
        
        Rect rect = m_pBowl->getBoundingBox();
//        Rect limitRect = Rect(rect.origin.x+190+40, rect.origin.y+250+40, rect.size.width-200-80, rect.size.height*0.4-40);
        Rect limitRect = Rect(rect.origin.x+150, rect.origin.y+170, rect.size.width-220, rect.size.height*0.3);
        m_pWhrisk->setLimitRect(limitRect);
        m_pWhrisk->setOrgPositionDefault();
//        m_pWhrisk->setTouchEnabled(false);
        m_pWhrisk->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+100));
        
        m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                     CallFunc::create([=](){
            m_pMixture->setScaleX(-m_pMixture->getScaleX());
        }), NULL)));
//        m_pWhrisk->runAction(Sequence::create(MoveTo::create(0.5, Vec2(visibleSize.width*0.5,visibleSize.height*0.5+100)),
//                                              CallFunc::create([=](){
//            
//            Rect rect = m_pBowl->getBoundingBox();
//            Rect limitRect = Rect(rect.origin.x+190, rect.origin.y+250, rect.size.width-200, rect.size.height*0.4);
//            m_pWhrisk->setLimitRect(limitRect);
//            m_pWhrisk->setTouchEnabled(true);
//        }), NULL));
//        m_pBowl->runAction(MoveBy::create(0.5, Vec2(110, 0)));
//        m_pMixture->runAction(MoveBy::create(0.5, Vec2(110, 0)));
    }
}


void PizzaMixScene::_ferment()
{
    m_pMixture->runAction(Sequence::create(DelayTime::create(0.5),
                                           ScaleTo::create(4, 1.2),
                                           CallFunc::create([=](){
        _finishAll();
    }), NULL));
}

#pragma mark ======== finish ========================
void PizzaMixScene::_finishAdd(int index)
{
    if(index<IngredientTag::eTagWhrisk-1){
        _showMixIngredient(index+1);
    }else{
        _showWhisk();
    }}

void PizzaMixScene::_finishMix()
{
    _removeWhisk();
    _showMixIngredient(IngredientTag::eTagCloth);
    
//    AudioHelp::getInstance()->playEffect("done.mp3");
//    AudioHelp::getInstance()->_playEffectNiceWork();
//    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
//    this->addChildToUILayer(pParticle);
//    pParticle->setLocalZOrder(10);
//    pParticle->setPosition(visibleSize*0.5);
//    
//    this->runAction(Sequence::create(DelayTime::create(2),
//                                     CallFunc::create([=](){
//
//        SceneManager::replaceTheScene<PizzaAddIngredientScene>();
//    }), NULL));
}


void PizzaMixScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<PizzaAddIngredientScene>();
    }), NULL));
}