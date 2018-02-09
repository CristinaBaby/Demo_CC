
#include "OmeletteFryScene.h"
#include "SceneManager.h"

OmeletteFryScene::OmeletteFryScene()
{
    m_nStep = 0;
    m_pPlate = nullptr;
}

OmeletteFryScene::~OmeletteFryScene()
{
    
}
bool OmeletteFryScene::init()
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
        AudioHelp::getInstance()->playEffect("vo_add_oil_pan.mp3");
        _showIngredient(0);
//        _showIngredient(1);
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void OmeletteFryScene::onEnter()
{
    ExtensionScene::onEnter();
}

void OmeletteFryScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void OmeletteFryScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "ingredient"){
        AudioHelp::getInstance()->playEffect("drag_corn.mp3");
    }
}

void OmeletteFryScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "mixture") {
        Rect rect = m_pPan->getBoundingBox();
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            m_pGuideLayer->removeGuide();
            pDragNode->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, -100),
                                                                MoveTo::create(0.5, m_pPanIn->getPosition()+Vec2(50, 100)), NULL),
                                                  CallFunc::create([=](){
                KettleNode* pContainer = (KettleNode*)pDragNode->getUserData();
                if (pContainer) {
                    pContainer->pourWaterOnce(100,2);
                }
                Sprite* pAdd = Sprite::create(localPath("omelet1.png"));
                if (pAdd) {
                    m_pOmelette = pAdd;
                    pAdd->setPosition(Vec2::ZERO);
                    pAdd->setOpacity(0);
                    m_pPanIn->addChild(pAdd,5);
                    pAdd->runAction(Sequence::create(FadeIn::create(1),
                                                     CallFunc::create([=](){
                    }), NULL));
                }
                AudioHelp::getInstance()->playEffect("pouring_sauce.mp3");
            }),
                                                  DelayTime::create(2),
                                                  RotateTo::create(0.5, 0),
                                                  MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                  CallFunc::create([=](){
                //                finish pour
                _doFryOmelette(m_pOmelette);
                m_pPanIn->removeChildByName("butter");
//                m_pPanIn->removeChildByName("oil");
            }), NULL));
        }
    }else if (name == "shovel"){
        int index = pDragNode->index;
        if (index==0) {
            Rect rect = m_pOmelette->getBoundingBox();
            rect.origin = m_pOmelette->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                Sprite* pEgg = Sprite::create(localPath("omelet5.png"));
                pDragNode->addChild(pEgg);
                pEgg->setName("omelette");
                pEgg->setScale(0.6);
                pEgg->setPosition(pDragNode->convertToNodeSpace(worldPoint));
                
                pDragNode->index = 1;
                m_pGuideLayer->removeGuide();
                m_pGuideLayer->showGuideMove(worldPoint, m_pPlate->getPosition());
                m_pOmelette->setVisible(false);
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }else if (index==1){
            Rect rect = m_pPlate->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                Node* pEgg = pDragNode->getChildByName("omelette");
                if (pEgg) {
                    pEgg->removeFromParent();
                }
                
                Sprite* pEggPlate = Sprite::create(localPath("omelet5.png"));
                m_pPlate->addChild(pEggPlate);
                
                pEggPlate->setScale(0.6,0.3);
                pEggPlate->setPosition(m_pPlate->getAnchorPointInPoints());
                
                pDragNode->index = 0;
                m_pGuideLayer->removeGuide();
                
                //finish fry
                m_pShovel->setTouchEnabled(false);
                m_pShovel->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                _finish();
            }
        }else if (index==2){
            Rect rect = m_pPlate->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                Node* pEgg = pDragNode->getChildByName("omelette");
                if (pEgg) {
                    pEgg->removeFromParent();
                }
                
                Sprite* pEggPlate = Sprite::create(localPath("omelet5.png"));
                m_pPlate->addChild(pEggPlate);
                
                pEggPlate->setScale(0.6,0.3);
                pEggPlate->setPosition(m_pPlate->getAnchorPointInPoints());
                
                pDragNode->index = 0;
                m_pGuideLayer->removeGuide();
                
                //finish fry
                m_pShovel->setTouchEnabled(false);
                m_pShovel->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                auto pParticle = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
                if (pParticle) {
                    pParticle->stopSystem();
                }
                _finish();
            }
        }
    }
}

void OmeletteFryScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        int tag = pDragNode->getTag();
        Rect rectBowl = m_pPan->getBoundingBox();
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)) {
            m_pGuideLayer->removeGuide();
            _addIngredient(pDragNode);
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),[=](){
                IngredientManager::getInstance()->showShadow(pDragNode->getTag(),true);
                pDragNode->setTouchEnabled(true);
            });
        }
        
    }
}

void OmeletteFryScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
    
}

#pragma mark - initData
void OmeletteFryScene::_initData()
{
    setExPath("content/make/omelette/make2/");
}


void OmeletteFryScene::_showStove()
{
    m_pStove = Sprite::create("content/make/common/stove/stove.png");
    this->addChildToContentLayer(m_pStove);
    CMVisibleRect::setPositionAdapted(m_pStove, 320, 320);
    
    m_pFire = Sprite::create("content/make/common/stove/stove_1.png");
    this->addChildToContentLayer(m_pFire);
    m_pFire->setPosition(m_pStove->getPosition()+Vec2(0,-10));
    
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/common/stove/stove_", 1, 2)));
}
void OmeletteFryScene::_showPan()
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
    this->addChildToContentLayer(m_pPanIn);
    CMVisibleRect::setPositionAdapted(m_pPanIn, 320, 400);
    
}

void OmeletteFryScene::_showMixture()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pBowl = _createDrageNode(localRootPath("make1/bowl_down.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320+visibleSize.width, 720);
    m_pBowl->setScale(0.6);
    m_pBowl->setName("mixture");
    m_pBowl->setTouchEnabled(false);
    m_pBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pBowl->getPosition(), m_pPanIn->getPosition());
        m_pBowl->setTouchEnabled(true);
    }), NULL));
    
    KettleNode* pContainer = KettleNode::create(m_pBowl);
    pContainer->addWater(localRootPath("make1/stir7.png"), localRootPath("make1/bowl_mask.png"));
    pContainer->addFront(localRootPath("make1/bowl_on.png"));
    m_pBowl->setUserData(pContainer);
    
}

void OmeletteFryScene::_showShovel()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
//    AudioHelp::getInstance()->playEffect("vo_turn_over_omlette.mp3");
    m_pShovel = _createDrageNode("content/make/common/stove/shovel.png");
    this->addChildToContentLayer(m_pShovel);
    CMVisibleRect::setPositionAdapted(m_pShovel, 400+visibleSize.width, 600);
    m_pShovel->setTouchEnabled(false);
    m_pShovel->setActionPoint(Vec2(85, 70));
    m_pShovel->setName("shovel");
    m_pShovel->setLocalZOrder(20);
    m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pShovel->getPosition(), m_pPanIn->getPosition());
        m_pShovel->setTouchEnabled(true);
    }), NULL));
}

void OmeletteFryScene::_showPlate()
{
    if (m_pPlate) {
        return;
    }
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320+visibleSize.width, 680);
    m_pPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
}

void OmeletteFryScene::_showIngredient(int index)
{
    //    back shadow front food food2
    static std::string sIngredents[][5] ={
        {"olive.png","olive_shadow.png"}, //oil
        {"butter_plate1.png","butter_plate2.png","","butter.png","butter_shadow.png"}, //batter
        {},
    };
    
    
    Vec2 actionPoint[] = {
        Vec2(0, 0),
        Vec2(71, 61),
    };
    Vec2 pos[] = {
        Vec2(220, 780),
        Vec2(430, 690),
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
    IngredientManager::getInstance()->addContainer(pDragNode,localPath(""), sIngredents[index]);
    
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

void OmeletteFryScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "pan_oil1.png",
        "butter.png",
    };
    int index = pIngredient->getTag();
    IngredientManager::getInstance()->frozeIngredients(index);
    int k = index;
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setTouchEnabled(false);
    
    switch (index) {
#pragma mark =========  add Oil ==========
        case 0:
        {
            pIngredient->setTouchEnabled(false);
            pIngredient->removeFromParent();
            Sprite* pBottle = Sprite::create(localPath("olive1.png"));
            this->addChildToContentLayer(pBottle);
            pBottle->setLocalZOrder(5);
            pBottle->setPosition(m_pPanIn->getPosition()+Vec2(100, 80));
            
            Sprite* pStream = Sprite::create(localPath("olive1_1.png"));
            this->addChildToContentLayer(pStream);
            pStream->setLocalZOrder(5);
            pStream->setPosition(pBottle->getPosition());
            pStream->runAction(RepeatForever::create(Sequence::create(AnimationHelp::createAnimate(localPath("olive1_"), 1, 2), NULL)));
            pBottle->runAction(Sequence::create(DelayTime::create(2),
                                                CallFunc::create([=](){
                
            }), NULL));
            
            Sprite* pOil = Sprite::create(localPath("pan_oil4.png"));
            m_pPanIn->addChild(pOil,5);
            pOil->setScale(0.2);
            pOil->setName("oil");
            pOil->runAction(Sequence::create(FadeIn::create(0.5),
                                             ScaleTo::create(2, 1),
                                             CallFunc::create([=](){
                
                IngredientManager::getInstance()->frozeIngredients(index,false);
                
                AudioHelp::getInstance()->playEffect("present_in.mp3");
                AudioHelp::getInstance()->playEffect("vo_add_butter_pan.mp3");
                _showIngredient(1);
                
                m_nStep++;
                if (m_nStep == 2) {
                    _showMixture();
                }
                MenuLayer::addIngredient(MenuLayer::ePackageOmelette, MenuLayer::eOmelette_oil);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                Node* pButter = m_pPanIn->getChildByName("butter");
                if (pButter) {
//                    pButter->removeFromParent();
                    pButter->setOpacity(100);
                }
                pStream->removeFromParent();
                pBottle->removeFromParent();
            }), NULL));
            return;
        }
#pragma mark =========  add Butter ==========
        case 1:
        {
            posAdd = Vec2(0, 0);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPan->getPosition()+Vec2(-60, 60)),
                                                    RotateTo::create(0.5,-60),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("add_buter.mp3");
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood(true,0.5);
                }
                Sprite* pButter = Sprite::create(localPath(lNameStr));
                m_pPanIn->addChild(pButter,5);
                pButter->setVisible(false);
                pButter->runAction(Sequence::create(DelayTime::create(0.5),
                                                    Show::create(),
                                                    AnimationHelp::createAnimate(localPath("pan_butter"), 1, 5,false,true,0.3),
                                                    FadeOut::create(0.5), NULL));
                
                Sprite* pOil = Sprite::create(localPath("pan_oil1.png"));
                m_pPanIn->addChild(pOil,4);
                pOil->setVisible(false);
                pOil->setName("butter");
                pOil->runAction(Sequence::create(DelayTime::create(0.5),
                                                 Show::create(),
                                                 AnimationHelp::createAnimate(localPath("pan_oil"), 1, 3,false,true,0.3),
                                                 CallFunc::create([=](){
                    pOil->setLocalZOrder(0);
//                    pOil->setOpacity(100);
                }),
                                                 FadeTo::create(2,100), NULL));
            }),
                                                    MoveTo::create(0.5,m_pPan->getPosition()+Vec2(40, 100)),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
                
                m_pGuideLayer->removeGuide();
//                m_pGuideLayer->showGuideMove(m_pPanIn->getPosition(), m_pPanIn->getPosition()+Vec2(100, 80));
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 2) {
                    _showMixture();
                }
                MenuLayer::addIngredient(MenuLayer::ePackageOmelette, MenuLayer::eOmelette_butter);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
            return;
        }
            break;

        default:
            break;
    }
    
}

void OmeletteFryScene::_ingredientOut(DragNode* pNode)
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    IngredientManager::getInstance()->frozeIngredients(pNode->getTag(),false);
}

void OmeletteFryScene::_doFryOmelette(Sprite* pFood)
{
    auto pMilkDrop = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
    if (!pMilkDrop) {
        auto pParticle = ParticleSystemQuad::create("particle/smoke1.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pPanIn->getPosition());
        pParticle->setName("smoke");
    }
    AudioHelp::getInstance()->playLoopEffect("frying_food.mp3");
    pFood->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("omelet"), 1, 5,false,true,2),
                                      CallFunc::create([=](){
        _showShovel();
        _showPlate();
    }), NULL));
}

void OmeletteFryScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<OmeletteAddInsertScene>();
    }), NULL));
    
}