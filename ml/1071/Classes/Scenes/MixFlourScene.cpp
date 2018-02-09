
#include "MixFlourScene.h"
#include "SceneManager.h"

MixFlourScene::MixFlourScene()
{
    m_pBowl = nullptr;
    m_pWhrisk = nullptr;
    m_nStep = 0;
    m_nAddCount = 0;
    m_nMixCount = 0;
    m_bWhriskIsReady = false;
    m_bReadyAdd = true;
    m_bCanTouch = false;
}

MixFlourScene::~MixFlourScene()
{
}
bool MixFlourScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg1.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _createMixUI();
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=]{
        _showMixIngredient(0);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=]{
        AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
        m_pGuideLayer->showGuideMove(Vec2(visibleSize.width*0.7, visibleSize.height*0.5),m_pBowl->getPosition());
    }), NULL));
    
//    test
//    _showMixIngredient(1);
//    m_nAddCount = 4;
//    m_nStep = 1;
//    this->runAction(Sequence::create(DelayTime::create(1),
//                                     CallFunc::create([=]{
//        _moveDownMixIngredient(IngredientTag::eTagButter);
//        _moveDownMixIngredient(IngredientTag::eTagMilk);
//    }), NULL));
    
    m_pGameUI->showNormalLayout();
    AudioHelp::getInstance()->playBackGroundMusic("bg_make.mp3");
    return true;
}

void MixFlourScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_JAPANESE);
}

void MixFlourScene::onExit()
{
    ExtensionScene::onExit();
}

void MixFlourScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void MixFlourScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
}

void MixFlourScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
    if (pContainer) {
        pContainer->showShadow(false);
    }
    int tag = pDragNode->getTag();
    
    pDragNode->setScale(1);
    pDragNode->setLocalZOrder(50);
    if (IngredientTag::eTagWhrisk==tag) {
        
        if (m_bWhriskIsReady){
            Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
            
            if (stick){
                stick->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("make1/blender_"), 1, 3)));
            }
            
            AudioHelp::getInstance()->playLoopEffect("stir.mp3");
            pDragNode->setLocalZOrder(5);
            m_pWhrisk->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.01, Vec2(0.1, 1)),
                                                                        MoveBy::create(0.01, Vec2(-0.2, -2)),
                                                                        MoveBy::create(0.01, Vec2(0.1, 1)),
                                                                        NULL)));
            Rect rect = m_pBowl->getBoundingBox();
            Rect limitRect = Rect(rect.origin.x+190, rect.origin.y+250, rect.size.width-200, rect.size.height*0.4);
            pDragNode->setLimitRect(limitRect);
            pDragNode->setOrgPositionDefault();
            pDragNode->setTouchEnabled(true);
//            m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
//                                                                         CallFunc::create([=](){
//                m_pMixture->setScaleX(-m_pMixture->getScaleX());
//            }), NULL)));
        }
    }
    
}

void MixFlourScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (m_pWhrisk==pDragNode){
        Rect rect = m_pBowl->getBoundingBox();
        rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            _mixBatter();
        }
        
    }
}

void MixFlourScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
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
        rect.size = rect.size+Size(100, 100);
        if (rect.containsPoint(pDragNode->getPosition()) && m_bReadyAdd) {
            pDragNode->setTouchEnabled(false);
            m_bReadyAdd = false;
            _addIngredient(pDragNode);
        }else{
            _back(pDragNode);
        }
    }
}

void MixFlourScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->stopAllActions();
    
    if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        if (stick) {
            stick->stopAllActions();
        }
        m_pWhrisk->stopAllActions();
        m_pMixture->stopAllActions();
        m_pMixture->setRotation(0);
    }

    _back(pDragNode);
}

#pragma mark - initData
void MixFlourScene::_initData()
{
    setExPath("content/make/japanese/");
    
    m_nNextSceneTag = GameUIEvent::eSceneTagFryPancake;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagFryPancake;
}

#pragma mark ==== Mix UI =====
void MixFlourScene::_createMixUI()
{
    m_pBowl = Sprite::create(localPath("make1/bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 210-visibleSize.width/2, 430);
    
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 210, 430);
    
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    
//    AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
}

void MixFlourScene::_showMixIngredient(int index)
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    if (index==0) {        
        m_pFlour = _createDrageNode(localPath("make1/flour.png"),false);
        this->addChildToContentLayer(m_pFlour);
        CMVisibleRect::setPositionAdapted(m_pFlour, 530+visibleSize.width, 580);
        m_pFlour->setActionPoint(Vec2(19, 237));
        m_pFlour->setTag(IngredientTag::eTagFlour);
        
        ContainerNode* pFlour = ContainerNode::create(m_pFlour);
//        pFlour->addShadow(localPath("make1/flour2_shadow.png"));
        m_pFlour->setUserData(pFlour);
        
        
        m_pCabbage = _createDrageNode(localPath("make1/plate.png"));
        this->addChildToContentLayer(m_pCabbage);
        CMVisibleRect::setPositionAdapted(m_pCabbage, 530+visibleSize.width, 300);
        m_pCabbage->setTag(IngredientTag::eTagCabbage);
        
        
        ContainerNode* pCabbage = ContainerNode::create(m_pCabbage);
        pCabbage->addShadow(localPath("make1/plate_shadow.png"));
        pCabbage->addFood(localPath("make1/cabbage.png"));
        pCabbage->addFoodShadow(localPath("make1/cabbage_shadow.png"));
        m_pCabbage->setUserData(pCabbage);
        
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        
        m_pCabbage->runAction(action);
        m_pFlour->runAction(action->clone());
    }else if (index==1){
        
        m_pEgg = _createDrageNode(localPath("make1/egg.png"));
        this->addChildToContentLayer(m_pEgg);
        CMVisibleRect::setPositionAdapted(m_pEgg, 530+visibleSize.width, 300);
        m_pEgg->setTag(IngredientTag::eTagEgg);
        
        m_pWater = _createDrageNode(localPath("make1/water.png"),false);
        this->addChildToContentLayer(m_pWater);
        CMVisibleRect::setPositionAdapted(m_pWater, 530+visibleSize.width, 580);
        m_pWater->setActionPoint(Vec2(29, 140));
        m_pWater->setTag(IngredientTag::eTagWater);
        
        
        
        KettleNode* pEgg = KettleNode::create(m_pEgg);
        pEgg->addShadow(localPath("make1/egg_shadow.png"));
        m_pEgg->setUserData(pEgg);
        
        
        KettleNode* pKettleWater = KettleNode::create(m_pWater);
        pKettleWater->addShadow(localPath("make1/water_shadow.png"));
        pKettleWater->addWater(localPath("make1/water1.png"), localPath("make1/water1.png"));
        m_pWater->setUserData(pKettleWater);
        
        
        
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pWater->runAction(action);
        m_pEgg->runAction(action->clone());
    }
}

void MixFlourScene::_showWhisk()
{
    AudioHelp::getInstance()->playEffect("vo_mix_ingredients.mp3");
    
    m_bWhriskIsReady = false;
    m_pWhrisk = _createDrageNode(localPath("make1/agitator1.png"));
    this->addChildToContentLayer(m_pWhrisk);
    CMVisibleRect::setPositionAdapted(m_pWhrisk, 550+visibleSize.width, 360);
    m_pWhrisk->setActionPoint(Vec2(45, 60));
    m_pWhrisk->setLocalZOrder(10);
    m_pWhrisk->setTag(IngredientTag::eTagWhrisk);
    
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

void MixFlourScene::_removeWhisk()
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


#pragma mark ======== deel ========================
void MixFlourScene::_back(DragNode* pIngredient)
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

void MixFlourScene::_addIngredient(DragNode* pIngredient)
{
    int tag = pIngredient->getTag();
    std::string lNameStr = "";
    float dt = 1.5;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setLocalZOrder(5);
    switch (tag) {
#pragma mark === add Flour ===
        case IngredientTag::eTagFlour:
        {
            lNameStr = "make1/bowl_flour.png";
            pIngredient->setTouchEnabled(false);
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+180, m_pBowl->getPositionY()+250));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                Sprite* pBag = Sprite::create(localPath("make1/flour2_2.png"));
                this->addChildToContentLayer(pBag);
                pBag->setLocalZOrder(8);
                
                Sprite* pBagFront = Sprite::create(localPath("make1/flour2_1.png"));
                this->addChildToContentLayer(pBagFront);
                pBagFront->setLocalZOrder(11);
                
                pBag->setPosition(pIngredient->getPosition());
                pBagFront->setPosition(pIngredient->getPosition());
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pBag->convertToWorldSpace(Vec2(30, 98))));
                pParticle->setLocalZOrder(10);
                
                pBag->runAction(Sequence::create(ScaleTo::create(0.5, 1.1,0.9),
                                                 DelayTime::create(1.5),
                                                 ScaleTo::create(0.5, 1),
                                                 RotateBy::create(0.3, 80),
                                                 DelayTime::create(0.5),
                                                 MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
                    m_nAddCount++;
                    if (m_nAddCount==2) {
                        _finishAdd(0);
                    }
                    m_bReadyAdd = true;
                    pBagFront->removeFromParent();
                    pBag->removeFromParent();
                }),
                                                 NULL));
                pBagFront->runAction(Sequence::create(ScaleTo::create(0.5, 1.1,0.9),
                                                      DelayTime::create(1.5),
                                                      ScaleTo::create(0.5, 1),
                                                      RotateBy::create(0.3, 100),
                                                      DelayTime::create(0.5),
                                                      MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                      NULL));
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
            
#pragma mark === add Cabbage ===
        case IngredientTag::eTagCabbage:
        {
            lNameStr = "make1/bowl_cabbage.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+50, m_pBowl->getPositionY()+200));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pour.mp3");
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
                for (int i = 0; i<8; i++) {
                    Sprite* pCabbage = Sprite::create(localPath("make1/cabbage_"+std::to_string(i%4+1)+".png"));
                    this->addChildToContentLayer(pCabbage);
                    pCabbage->setPosition(pIngredient->getPosition());
                    pCabbage->setLocalZOrder(10);
                    pCabbage->setRotation(i*20);
                    pCabbage->runAction(Sequence::create(DelayTime::create(0.01+0.2*i),
                                                        Spawn::create(MoveBy::create(0.5, Vec2(-50, -100)),
                                                                      RotateBy::create(0.5, 10+i*10), NULL),
                                                        CallFunc::create([=](){
                        pCabbage->removeFromParent();
                    }), NULL));
                }
            }),
                                                    DelayTime::create(1.5),
                                                    RotateBy::create(0.3, 30),
                                                    DelayTime::create(0.5),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==2) {
                    _finishAdd(0);
                }
                m_bReadyAdd = true;
            }),NULL));
        }
            break;

#pragma mark === add Egg ===
        case IngredientTag::eTagEgg:
        {
            lNameStr = "make1/bowl_egg3_2.png";
            posAdd = Vec2(20,10);
            pIngredient->setTouchEnabled(false);
            pIngredient->removeFromParent();
            Sprite* pEgg = Sprite::create(localPath("make1/egg1.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setPosition(m_pBowl->getPosition()+Vec2(50, 200));
            AudioHelp::getInstance()->playAddEggEffect();
            pEgg->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("make1/egg"), 1, 2,false),
                                             CallFunc::create([=](){
                Sprite* pEgg2 = Sprite::create(localPath("make1/egg3.png"));
                this->addChildToContentLayer(pEgg2);
                pEgg2->setLocalZOrder(6);
                pEgg2->setPosition(m_pBowl->getPosition()+Vec2(50, 100));
                pEgg2->runAction(Sequence::create(DelayTime::create(1),
                                                  CallFunc::create([=](){
                    pEgg2->removeFromParent();
                }), NULL));
                
                pEgg->setVisible(false);
            }),
                                             DelayTime::create(1),
                                             CallFunc::create([=](){
                Sprite* pEgg2 = Sprite::create(localPath("make1/bowl_egg3_1.png"));
                this->addChildToContentLayer(pEgg2);
                pEgg2->setLocalZOrder(6);
                pEgg2->setPosition(m_pBowl->getPosition()+Vec2(20, 30));
                pEgg2->runAction(Sequence::create(DelayTime::create(0.5),
                                                  CallFunc::create([=](){
                    m_nAddCount++;
                    if (m_nAddCount==4) {
                        _finishAdd(1);
                    }
                    m_bReadyAdd = true;
                    pEgg2->removeFromParent();
                }), NULL));
                pEgg->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark === add Water ===
        case IngredientTag::eTagWater:
        {
            lNameStr = "make1/bowl_water.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+250));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourMilkEffect();
                
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
                
                KettleNode* pContainer = (KettleNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourWater();
                }
                Sprite* pFlour = Sprite::create(localPath("bowl_flour2.png"));
                if (pFlour) {
                    pFlour->setOpacity(0);
                    m_pMixture->addChild(pFlour,1);
                    pFlour->runAction(Sequence::create(DelayTime::create(dt),
                                                     FadeIn::create(0.5), NULL));
                }
            }),
                                                    DelayTime::create(2),
                                                    RotateTo::create(0.3, 0),
                                                    DelayTime::create(0.5),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==4) {
                    _finishAdd(1);
                }
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
        m_pMixture->addChild(pAdd);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         FadeIn::create(0.5),
                                         CallFunc::create([=](){
        }), NULL));
    }
}

void MixFlourScene::_mixBatter()
{
    m_pMixture->setRotation(m_pMixture->getRotation()+10);
    if (m_bWhriskIsReady){
        m_nMixCount++;
        
        if (m_nMixCount%60==0) {
            int mixtureStep = m_nMixCount/60;
            int mixTotal = 7;
            if (mixtureStep<=mixTotal){
                std::stringstream ostr;
                ostr<<"make1/stir"<<mixtureStep<<".png";
                Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                m_pMixture->removeAllChildren();
                m_pMixture->addChild(pMixture);
            }
            if (mixtureStep>=mixTotal){
                //mix finish
                m_pMixture->stopAllActions();
                m_pWhrisk->setDragSprite(Sprite::create(localPath("make1/agitator1.png")));
                m_pWhrisk->setRotation(-145);
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
        m_pWhrisk->setDragSprite(Sprite::create(localPath("make1/agitator2.png")));
        m_pWhrisk->setLocalZOrder(5);
        m_pWhrisk->setRotation(0);
        
        Rect rect = m_pBowl->getBoundingBox();
        Rect limitRect = Rect(rect.origin.x+190+40, rect.origin.y+250+40, rect.size.width-200-80, rect.size.height*0.4-40);
        m_pWhrisk->setLimitRect(limitRect);
        m_pWhrisk->setOrgPositionDefault();
        m_pWhrisk->setTouchEnabled(false);
        m_pWhrisk->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+100));
        
//        m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
//                                                                     CallFunc::create([=](){
//            m_pMixture->setScaleX(-m_pMixture->getScaleX());
//        }), NULL)));
        m_pWhrisk->runAction(Sequence::create(MoveTo::create(0.5, Vec2(visibleSize.width*0.5,visibleSize.height*0.5+100)),
                                              CallFunc::create([=](){
            
            Rect rect = m_pBowl->getBoundingBox();
            Rect limitRect = Rect(rect.origin.x+190, rect.origin.y+250, rect.size.width-200, rect.size.height*0.4);
            m_pWhrisk->setLimitRect(limitRect);
            m_pWhrisk->setTouchEnabled(true);
        }), NULL));
        m_pBowl->runAction(MoveBy::create(0.5, Vec2(110, 0)));
        m_pMixture->runAction(MoveBy::create(0.5, Vec2(110, 0)));
    }
}

void MixFlourScene::_playEffectNice(float)
{
    static int count = 0;
    count++;
    if (count==6) {
        int radom = arc4random()%100;
        if (radom>50){
            AudioHelp::getInstance()->playEffect("nice.mp3");
        }else {
            AudioHelp::getInstance()->playEffect("fantastic.mp3");
        }
    }
}


#pragma mark ======== finish ========================
void MixFlourScene::_finishAdd(int index)
{
    switch (index) {
        case 0:
        {
            _showMixIngredient(1);
        }
            break;
        case 1:
        {
            _showWhisk();
        }
            break;
        case 2:
        {
            _showWhisk();
        }
            break;
        case 3:
        {
            _showWhisk();
        }
            break;
            
        default:
            break;
    }
}

void MixFlourScene::_finishMix()
{
    _removeWhisk();
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<FryPancakeScene>();
    }), NULL));
}
