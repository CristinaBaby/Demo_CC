
#include "FunnelMixSecne.h"
#include "SceneManager.h"

FunnelMixSecne::FunnelMixSecne()
{
    m_nAddCount = 0;
    m_nMilkCount = 0;
    m_bReadyAdd = true;
    m_nMixCount = 0;
    m_nEggCount = 0;
}

FunnelMixSecne::~FunnelMixSecne()
{
    
}
bool FunnelMixSecne::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("vo_funnel_sugar.mp3");
        _showMixIngredient(0);
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_funnel_flour.mp3");
        
        _showMixIngredient(1);
    }), NULL));
    _createMixUI();
    return true;
}

void FunnelMixSecne::onEnter()
{
    ExtensionScene::onEnter();
}

void FunnelMixSecne::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FunnelMixSecne::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    int tag = pDragNode->getTag();
    if (IngredientTag::eTagEgg == tag) {
        pDragNode->setDragSprite(Sprite::create(localPath("egg.png")));
    }
    if (IngredientTag::eTagFlour==tag) {
        
        pDragNode->setDragSprite(Sprite::create(localPath("flour_spoon1.png")));
        ContainerNode* pFlour = (ContainerNode*)m_pFlour->getUserData();
        if(! pFlour){
            pFlour = ContainerNode::create(m_pFlour);
            m_pFlour->setUserData(pFlour);
        }
        pFlour->addFood(localPath("flour_spoon2.png"),localPath("flour_spoon2.png"));
    }else if (IngredientTag::eTagPowder == tag || IngredientTag::eTagSalt == tag) {
        if (pDragNode->index == -1) {
            pDragNode->setDragSprite(Sprite::create(localPath("spoon_down.png")));
//            pDragNode->index++;
        }
    }
    if (IngredientTag::eTagWhrisk==tag) {
        
        if (pDragNode->index == 1){
            AudioHelp::getInstance()->playLoopEffect("stir.mp3");
            
            pDragNode->setLocalZOrder(5);

            Rect rect = m_pBowl->getBoundingBox();
            Rect limitRect = Rect(rect.origin.x+185, rect.origin.y+260, rect.size.width-280, rect.size.height*0.4);
            pDragNode->setLimitRect(limitRect);
            pDragNode->setOrgPositionDefault();
            pDragNode->setTouchEnabled(true);
            m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                         CallFunc::create([=](){
                m_pMixture->setScaleX(-m_pMixture->getScaleX());
            }), NULL)));
        }else if (pDragNode->index == -1) {
            m_pWhrisk->index = 0;
            m_pWhrisk->runAction(RotateBy::create(0.5, -60));
        }
    }
}

void FunnelMixSecne::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (pDragNode->getTag() == IngredientTag::eTagPowder) {
        if ((pDragNode->index+1)%2 == 0) {
            Rect rect = m_pPowder->getBoundingBox();
            rect.origin = m_pPowder->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
                pDragNode->index++;
                pDragNode->setPosition(m_pPowder->getPosition()+Vec2(50, 150));
                pDragNode->runAction(Sequence::create(RotateBy::create(0.3, -30),
                                                      CallFunc::create([=](){
                    ContainerNode* pContainer = ContainerNode::create(pDragNode);
                    pContainer->addFront(localPath("spoon_on.png"));
                    pContainer->addFood(localPath("spoon_powder.png"));
                    pDragNode->setUserData(pContainer);
                }),
                                                      RotateBy::create(0.3, 30),
                                                      CallFunc::create([=](){
                    
                    pDragNode->setTouchEnabled(true);
                }), NULL));
            }
        }
    }else if (pDragNode->getTag() == IngredientTag::eTagSalt){
        if ((pDragNode->index+1)%2 == 0) {
            Rect rect = m_pSalt->getBoundingBox();
            rect.origin = m_pSalt->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index++;
                m_pGuideLayer->removeGuide();
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pSalt->getPosition()+Vec2(20, 80));
                pDragNode->runAction(Sequence::create(RotateBy::create(0.3, -30),
                                                      CallFunc::create([=](){
                    ContainerNode* pContainer = ContainerNode::create(pDragNode);
                    pContainer->addFront(localPath("spoon_on.png"));
                    pContainer->addFood(localPath("spoon_salt.png"));
                    pDragNode->setUserData(pContainer);
                }),
                                                      RotateBy::create(0.3, 30),
                                                      CallFunc::create([=](){
                    
                    pDragNode->setTouchEnabled(true);
                }), NULL));
            }
        }
        
    }
    if (name=="whisk"){
        Rect rect = m_pBowl->getBoundingBox();
        rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            _mixBatter();
        }
        
    }
}

void FunnelMixSecne::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
    pDragNode->stopAllActions();
    
    
    if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        if (stick) {
            stick->stopAllActions();
        }
        m_pWhrisk->stopAllActions();
        m_pMixture->stopAllActions();
        m_pMixture->setRotation(0);
    }else{
        Rect rect = m_pBowl->getBoundingBox();
        rect.size = rect.size+Size(200, 180);
        rect.origin = rect.origin-Vec2(-80, -80);
        int tag = pDragNode->getTag();
        bool doAction = true;
        if (tag == IngredientTag::eTagPowder || tag == IngredientTag::eTagSalt) {
            if ((pDragNode->index+1)%2==0) {
                doAction = false;
            }
        }
        if (rect.containsPoint(pDragNode->getPosition()) && doAction && m_bReadyAdd) {
            pDragNode->setTouchEnabled(false);
            m_bReadyAdd = false;
            _addIngredient(pDragNode);
        }else{
            _back(pDragNode);
        }
    }
}

void FunnelMixSecne::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void FunnelMixSecne::_initData()
{
    setExPath("content/make/funnel/1/");
}

void FunnelMixSecne::_back(DragNode* pIngredient)
{
    pIngredient->setTouchEnabled(false);
    pIngredient->back(0,[=](){
        ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
        if (pContainer) {
            pContainer->showShadow();
        }
        int tag = pIngredient->getTag();
        if (IngredientTag::eTagEgg == tag) {
            pIngredient->back(0,[=](){
                pIngredient->setDragSprite(Sprite::create(localPath("egg"+std::to_string(pIngredient->index)+".png")));
            });
        }
        pIngredient->setTouchEnabled(true);
    });
//    pIngredient->runAction(ScaleTo::create(0.5, 0.6));
}
void FunnelMixSecne::_createMixUI()
{
    m_pBowl = Sprite::create(localPath("bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 210+110-visibleSize.width/2, 350);
    
    m_pBowlFront = Sprite::create(localPath("bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    CMVisibleRect::setPositionAdapted(m_pBowlFront, 210+110-visibleSize.width/2, 350);
    
    m_pMixture = ClippingNode::create(Sprite::create(localPath("mask.png")));
    m_pMixture->setAlphaThreshold(0.5);
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 210+115, 350);
    m_pMixture->setLocalZOrder(5);
    
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFront->setLocalZOrder(8);
    
    //    AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
}

void FunnelMixSecne::_showWhisk()
{
    AudioHelp::getInstance()->playEffect("vo_mix_ingredients.mp3");
    
    m_pWhrisk = _createDrageNode(localPath("agitator1.png"));
    this->addChildToContentLayer(m_pWhrisk);
    CMVisibleRect::setPositionAdapted(m_pWhrisk, 320+visibleSize.width, 700);
    m_pWhrisk->setActionPoint(Vec2(61, 55));
    m_pWhrisk->setLocalZOrder(10);
    m_pWhrisk->setTag(IngredientTag::eTagWhrisk);
    m_pWhrisk->index = -1;
    m_pWhrisk->setName("whisk");
    
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

void FunnelMixSecne::_removeWhisk()
{
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                               m_pWhrisk->removeFromParent();
                                                           }),
                                          NULL));
}

void FunnelMixSecne::_showMixIngredient(int index)
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    if (index==0) {
        m_pSugar = _createDrageNode(localPath("sugar_down.png"),false);
        this->addChildToContentLayer(m_pSugar);
        CMVisibleRect::setPositionAdapted(m_pSugar, 200+visibleSize.width, 700);
        m_pSugar->setActionPoint(Vec2(198, 93));
        m_pSugar->setTag(IngredientTag::eTagSugar);
        
        ContainerNode* pSugar = ContainerNode::create(m_pSugar);
        pSugar->addShadow(localPath("sugar_shadow.png"));
        pSugar->addFront(localPath("sugar_on.png"));
        pSugar->addFood(localPath("sugar.png"),localPath("sugar.png"));
        m_pSugar->setUserData(pSugar);
        
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pSugar->runAction(action);
    }else if (index==1) {
        m_pFlourBag = Sprite::create(localPath("flour.png"));
        this->addChildToContentLayer(m_pFlourBag);
        CMVisibleRect::setPositionAdapted(m_pFlourBag, 430+visibleSize.width, 770);
        
        m_pFlour = _createDrageNode(localPath("flour1.png"),false);
        this->addChildToContentLayer(m_pFlour);
        CMVisibleRect::setPositionAdapted(m_pFlour, 460+visibleSize.width, 860);
        m_pFlour->setActionPoint(Vec2(23, 21));
        m_pFlour->setTag(IngredientTag::eTagFlour);
        
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pFlourBag->runAction(action);
        m_pFlour->runAction(action->clone());
    }else if (index==2) {
        m_pEgg0 = _createDrageNode(localPath("egg3.png"));
        this->addChildToContentLayer(m_pEgg0);
        CMVisibleRect::setPositionAdapted(m_pEgg0, 230+visibleSize.width, 690);
        
        m_pEgg1 = _createDrageNode(localPath("egg2.png"));
        this->addChildToContentLayer(m_pEgg1);
        m_pEgg1->setPosition(m_pEgg0->getPosition());
        
        m_pEgg2 = _createDrageNode(localPath("egg1.png"));
        this->addChildToContentLayer(m_pEgg2);
        m_pEgg2->setPosition(m_pEgg0->getPosition());
        
        m_pEgg0->setTag(IngredientTag::eTagEgg);
        m_pEgg1->setTag(IngredientTag::eTagEgg);
        m_pEgg2->setTag(IngredientTag::eTagEgg);
        m_pEgg0->index = 3;
        m_pEgg1->index = 2;
        m_pEgg2->index = 1;
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pEgg0->runAction(action);
        m_pEgg1->runAction(action->clone());
        m_pEgg2->runAction(action->clone());
    }else if (index==3) {
        m_pMilk = _createDrageNode(localPath("milk_down.png"));
        this->addChildToContentLayer(m_pMilk);
        CMVisibleRect::setPosition(m_pMilk, 450+visibleSize.width, 690);
        m_pMilk->setTag(IngredientTag::eTagMilk);
        m_pMilk->setActionPoint(Vec2(8, 125));
        KettleNode* pMilkContainer = KettleNode::create(m_pMilk);
        pMilkContainer->addFront(localPath("milk_on.png"));
        pMilkContainer->addShadow(localPath("milk_shadow.png"));
        pMilkContainer->addWater(localPath("milk.png"), localPath("milk.png"));
        m_pMilk->setUserData(pMilkContainer);
        m_pMilk->index = m_nMilkCount;
        m_nMilkCount++;
        
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pMilk->runAction(action);
    }else if (index==4) {
        m_pPowder = Sprite::create(localPath("baking powder.png"));
        this->addChildToContentLayer(m_pPowder);
        CMVisibleRect::setPositionAdapted(m_pPowder, 240+visibleSize.width, 740);
        
        m_pPowderSpoon = _createDrageNode(localPath("spoon.png"));
        this->addChildToContentLayer(m_pPowderSpoon);
        m_pPowderSpoon->setTag(IngredientTag::eTagPowder);
        m_pPowderSpoon->setActionPoint(Vec2(14, 45));
        CMVisibleRect::setPositionAdapted(m_pPowderSpoon, 500+visibleSize.width, 700);
        m_pPowderSpoon->index = -1;
        
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pPowder->runAction(action);
        m_pPowderSpoon->runAction(action->clone());
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            m_pGuideLayer->showGuideMove(m_pPowderSpoon->getPosition(), m_pPowder->getPosition());
        }), NULL));
        
    }else if (index==5) {
        m_pSalt = Sprite::create(localPath("salt.png"));
        this->addChildToContentLayer(m_pSalt);
        CMVisibleRect::setPositionAdapted(m_pSalt, 240+visibleSize.width, 700);
        
        m_pSaltSpoon = _createDrageNode(localPath("spoon.png"));
        this->addChildToContentLayer(m_pSaltSpoon);
        m_pSaltSpoon->setTag(IngredientTag::eTagSalt);
        m_pSaltSpoon->setActionPoint(Vec2(14, 45));
        CMVisibleRect::setPositionAdapted(m_pSaltSpoon, 500+visibleSize.width, 700);
        m_pSaltSpoon->index = -1;
        auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pSalt->runAction(action);
        m_pSaltSpoon->runAction(action->clone());
        
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            m_pGuideLayer->showGuideMove(m_pSaltSpoon->getPosition(), m_pSalt->getPosition());
        }), NULL));
    }
    
}

void FunnelMixSecne::_addIngredient(DragNode* pIngredient)
{
    int tag = pIngredient->getTag();
    int index = pIngredient->index;
    std::string lNameStr = "";
    float dt = 1;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setLocalZOrder(5);
    switch (tag) {
#pragma mark === add Sugar ===
        case IngredientTag::eTagSugar:
        {
            lNameStr = "bowl_sugar.png";
            posAdd = Vec2(0,-30);
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()-100, m_pBowl->getPositionY()+240));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.3, 80),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setScaleX(-1);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood(false);
                }
            }),
                                                    JumpBy::create(1.5, Vec2::ZERO, 20, 5),
                                                    Spawn::create(RotateTo::create(0.3, 0),
                                                                  MoveBy::create(0.3, Vec2(0, 100)), NULL),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=]{
                m_nAddCount++;
                if (m_nAddCount==2) {
                    _finishAdd(0);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageFunnelcake, MenuLayer::eFunnelcake_sugar);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Flour ===
        case IngredientTag::eTagFlour:
        {
            lNameStr = "bowl_flour.png";
//            posAdd = Vec2(0,-50);
            pIngredient->setTouchEnabled(false);
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+180, m_pBowl->getPositionY()+300));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    DelayTime::create(2),
                                                    RotateTo::create(0.5, 0),
                                                    CallFunc::create([=](){
                m_bReadyAdd = true;
                pIngredient->index++;
                if (pIngredient->index==3) {
                    
                    m_nAddCount++;
                    if (m_nAddCount==2) {
                        _finishAdd(0);
                    }
                    MenuLayer::addIngredient(MenuLayer::ePackageFunnelcake, MenuLayer::eFunnelcake_flour);
                    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                    m_pFlourBag->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
                    pIngredient->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                            
                                                            CallFunc::create([=](){
                        pIngredient->removeFromParent();
                    }), NULL));
                }else{
                    pIngredient->back(1,[=](){
//                        ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
//                        if (pContainer) {
//                            pContainer->addFood(localPath("flour_spoon2.png"));
//                        }
                        pIngredient->setTouchEnabled(true);
                    });
                }
            }),
                                                    NULL));
        }
            break;

#pragma mark === add Egg ===
        case IngredientTag::eTagEgg:
        {
            lNameStr = "bowl_egg.png";
            int index = pIngredient->index;
            Vec2 delt = Vec2(30*(index%3), 30*((index+1)%3));
            posAdd = Vec2(-20,-20)+delt;
            dt = 2.5;
            pIngredient->setTouchEnabled(false);
            pIngredient->removeFromParent();
            Sprite* pEgg = Sprite::create(localPath("bowl_egg_1.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setPosition(m_pBowl->getPosition()+Vec2(0, 100)+delt);
            AudioHelp::getInstance()->playAddEggEffect();
            pEgg->setLocalZOrder(6);
            pEgg->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("bowl_egg_"), 1, 4,false,true,0.5),
                                             MoveBy::create(0.5,Vec2(-30, -30)),
//                                             DelayTime::create(2),
                                             CallFunc::create([=](){
                    m_nAddCount++;
                    if (m_nAddCount==7) {
                        _finishAdd(1);
                    }
                m_nEggCount++;
                if(m_nEggCount==3){
                    MenuLayer::addIngredient(MenuLayer::ePackageFunnelcake, MenuLayer::eFunnelcake_egg);
                    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                }
                    m_bReadyAdd = true;
                pEgg->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark === add Milk ===
        case IngredientTag::eTagMilk:
        {
            lNameStr = "bowl_milk.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+70, m_pBowl->getPositionY()+300));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourMilkEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/milk.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
                pParticle->setName("milkstream");
                pParticle->setScaleX(-1);
                
                KettleNode* pContainer = (KettleNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourWater();
                }
            }),
                                                    DelayTime::create(2),
                                                    RotateTo::create(0.3, 0),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                pIngredient->index++;
                if (m_nMilkCount==1) {
                    _showMixIngredient(3);
                }else if(m_nMilkCount==2) {
                    MenuLayer::addIngredient(MenuLayer::ePackageFunnelcake, MenuLayer::eFunnelcake_milk);
                    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                }
                m_nAddCount++;
                if (m_nAddCount==7) {
                    _finishAdd(1);
                }
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
                
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Powder ===
        case IngredientTag::eTagPowder:
        {
            lNameStr = "bowl_powder.png";
            pIngredient->setTouchEnabled(false);
            
            Vec2 delt = Vec2(-20*(index%3), 20*((index+1)%3));
            posAdd = Vec2(0,-20)+delt;
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+130, m_pBowl->getPositionY()+250));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    ScaleTo::create(0.5, 1.1,0.9),
                                                    DelayTime::create(1),
                                                    ScaleTo::create(0.5, 1),
                                                    RotateTo::create(0.3, 0),
//                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                pIngredient->index++;
                if (pIngredient->index==3) {
                    m_nAddCount++;
                    if (m_nAddCount==8) {
                        _finishAdd(2);
                    }
                    MenuLayer::addIngredient(MenuLayer::ePackageFunnelcake, MenuLayer::eFunnelcake_powder);
                    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                    pIngredient->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),NULL));
                    m_pPowder->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                }else{
                    pIngredient->back(1,[=](){
                        pIngredient->setTouchEnabled(true);
                    });
                }
                m_bReadyAdd = true;
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Salt ===
        case IngredientTag::eTagSalt:
        {
            lNameStr = "bowl_salt.png";
            pIngredient->setTouchEnabled(false);
            posAdd = Vec2(0,30);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+250));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.3, -30),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    JumpBy::create(1.5, Vec2::ZERO, 20, 5),
                                                    Spawn::create(RotateTo::create(0.3, 0),
                                                                  MoveBy::create(0.3, Vec2(0, 100)), NULL),
                                                    CallFunc::create([=]{
                m_pSalt->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=]{
                
                
                m_nAddCount++;
                if (m_nAddCount==9) {
                    _finishAdd(3);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageFunnelcake, MenuLayer::eFunnelcake_salt);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
            
        default:
            break;
    }
    Sprite* pAdd = Sprite::create(localPath(lNameStr));
    pAdd->setPosition(posAdd);
    if (pAdd) {
        pAdd->setOpacity(0);
        m_pMixture->addChild(pAdd,5);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         FadeIn::create(0.5),
                                         CallFunc::create([=](){
        }), NULL));
        if(IngredientTag::eTagFlour == tag){
            float scale = 1;
            if (index == 0) {
                scale = 0.4;
            }else if (index == 1) {
                scale = 0.7;
            }
            pAdd->setLocalZOrder(0);
            pAdd->setAnchorPoint(Vec2(0.5, 0));
            pAdd->setPosition(posAdd+Vec2(0, -pAdd->getContentSize().height/2+30));
            pAdd->setScale(scale-0.3);
            pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                             ScaleTo::create(0.3,scale),
                                             CallFunc::create([=](){
            }), NULL));
        }else if (IngredientTag::eTagMilk == tag) {
            float scale = 1;
            if (index == 0) {
                scale = 0.6;
            }
            pAdd->setLocalZOrder(2);
            pAdd->setScale(0.2);
            pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                             ScaleTo::create(0.3,scale), NULL));
            
        }
    }
}

void FunnelMixSecne::_mixBatter()
{
    if (m_pWhrisk->index ==1){
        m_nMixCount++;
        
        if (m_nMixCount%80==0) {
            int mixtureStep = m_nMixCount/80;
            int mixTotal = 6;
            if (mixtureStep<=mixTotal){
                std::stringstream ostr;
                ostr<<"bowl_batter"<<mixtureStep<<".png";
                Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                m_pMixture->removeAllChildren();
                m_pMixture->addChild(pMixture);
            }
            if (mixtureStep>=mixTotal){
                //mix finish
                m_pMixture->stopAllActions();
                
                m_pWhrisk->setDragSprite(Sprite::create(localPath("agitator1.png")));
                m_pWhrisk->setRotation(-80);
                _finishMix();
                m_nMixCount = 0;
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }
    }else if (m_pWhrisk->index ==0){
        Sprite *pShadow = (Sprite*)m_pWhrisk->getChildByName("shadow");
        if (pShadow){
            pShadow->removeFromParent();
        }
        
        m_pWhrisk->index = 1;
        AudioHelp::getInstance()->playLoopEffect("stir.mp3");
        
        m_pWhrisk->stopAllActions();
        m_pWhrisk->setRotation(0);
        m_pWhrisk->setDragSprite(Sprite::create(localPath("agitator2.png")));
        m_pWhrisk->setLocalZOrder(5);
        
        Rect rect = m_pBowl->getBoundingBox();
        Rect limitRect = Rect(rect.origin.x+190+40, rect.origin.y+250+40, rect.size.width-200-80, rect.size.height*0.4-40);
        m_pWhrisk->setLimitRect(limitRect);
        m_pWhrisk->setOrgPositionDefault();
        m_pWhrisk->setPosition(Vec2(m_pBowl->getPositionX(), m_pBowl->getPositionY()+100));
        m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                     CallFunc::create([=](){
            m_pMixture->setScaleX(-m_pMixture->getScaleX());
        }), NULL)));
    }
}


void FunnelMixSecne::_finishMix()
{
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->stopAllActions();
    m_pWhrisk->setLocalZOrder(20);
    m_pWhrisk->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect(((arc4random()%100)>50?"vo_nice_work.mp3":"vo_well_done.mp3"));
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
//    m_pGameUI->showNextLayout();
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<FunnelPourScene>();
    }), NULL));
}

void FunnelMixSecne::_finishAdd(int index)
{
    if (index==0) {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("vo_funnel_3eggs.mp3");
            _showMixIngredient(2);
        }),
                                         DelayTime::create(2),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_funnel_2cups_milk.mp3");
            
            _showMixIngredient(3);
        }), NULL));
    }else if (index==1) {
        AudioHelp::getInstance()->playEffect("vo_funnel_powder.mp3");
        _showMixIngredient(4);
    }else if (index==2) {
        AudioHelp::getInstance()->playEffect("vo_funnel_salt.mp3");
        _showMixIngredient(5);
    }else if (index==3) {
        _showWhisk();
    }
}