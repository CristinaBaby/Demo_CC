
#include "MakeIcecreamScene.h"
#include "SceneManager.h"

MakeIcecreamScene::MakeIcecreamScene()
{
    m_pBowl = nullptr;
    m_pWhrisk = nullptr;
    m_nAddCount = 0;
    m_nMixCount = 0;
    m_bWhriskIsReady = false;
    m_bReadyAdd = true;
}

MakeIcecreamScene::~MakeIcecreamScene()
{
    
}
bool MakeIcecreamScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg2.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pTable = Sprite::create(localPath("bg/table.png"));
    CMVisibleRect::setPosition(pTable, 320, 200);
    this->addChildToBGLayer(pTable);
    
    _showShelf();
    this->runAction(Sequence::create(DelayTime::create(3),
                                     CallFunc::create([=](){        
        _showIngredient(0);
        _showBowl();
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(Vec2(visibleSize.width*0.6,visibleSize.height*0.65), Vec2(visibleSize.width*0.5,visibleSize.height*0.4));
    }), NULL));
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/cyis.png");
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    return true;
}

void MakeIcecreamScene::onEnter()
{
    ExtensionScene::onEnter();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_MAKE_ICECREAM);
}

void MakeIcecreamScene::onExit()
{
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_MAKE_ICECREAM);
    
    ExtensionScene::onExit();
}


void MakeIcecreamScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    if (pDragNode==m_pEgg) {
        m_pEgg2->setVisible(true);
        pDragNode->setDragSprite(Sprite::create(localPath("mix/egg_small1.png")));
        pDragNode->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
    }else if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        stick->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("mix/stir/agitator_"), 2, 4)));
        
        if (m_bWhriskIsReady){
            AudioHelp::getInstance()->playLoopEffect("machine_stir.mp3");
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
            m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.2),
                                                                         CallFunc::create([=]()
                                                                                          {
                                                                                              m_pMixture->setScaleX(-1);
                                                                                          }),
                                                                         DelayTime::create(0.2),
                                                                         CallFunc::create([=]()
                                                                                          {
                                                                                              m_pMixture->setScaleX(1);
                                                                                          }),
                                                                         NULL)));

        }
    }
    
}
void MakeIcecreamScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
    if (m_pWhrisk==pDragNode){
        Rect rect = m_pBowl->getBoundingBox();
        rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            if (m_bWhriskIsReady){
                m_nMixCount++;

                if (m_nMixCount%50==0) {
                    int mixtureStep = m_nMixCount/50;
                    if (mixtureStep<=5){
                        std::stringstream ostr;
                        ostr<<"mix/stir/batter_"<<mixtureStep<<".png";
                        Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                        m_pMixture->removeAllChildren();
                        m_pMixture->addChild(pMixture);
                        
                    }
                    if (mixtureStep>=5){
                        //mix finish
                        m_pMixture->stopAllActions();
                        _finishMix();
                        AudioHelp::getInstance()->stopLoopEffect();
                    }
                }
            }else{
                m_bWhriskIsReady = true;
                AudioHelp::getInstance()->playLoopEffect("machine_stir.mp3");
//                pDragNode->setTouchEnabled(false);
                pDragNode->setLocalZOrder(5);
                m_pWhrisk->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.01, Vec2(0.1, 1)),
                                                                                             MoveBy::create(0.01, Vec2(-0.2, -2)),
                                                                                             MoveBy::create(0.01, Vec2(0.1, 1)),
                                                                                             NULL)));
//                pDragNode->runAction(Sequence::create(Spawn::create(RotateTo::create(0.5, -50),
//                                                                    NULL),
//                                                      CallFunc::create([=]()
//                                                                       {
//                                                                       }),
//                                                      NULL));
                Rect rect = m_pBowl->getBoundingBox();
                Rect limitRect = Rect(rect.origin.x+190, rect.origin.y+250, rect.size.width-200, rect.size.height*0.4);
                pDragNode->setLimitRect(limitRect);
                pDragNode->setOrgPositionDefault();
                pDragNode->setTouchEnabled(true);
                m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.2),
                                                                             CallFunc::create([=]()
                                                                                              {
                                                                                                  m_pMixture->setScaleX(-1);
                                                                                              }),
                                                                             DelayTime::create(0.2),
                                                                             CallFunc::create([=]()
                                                                                              {
                                                                                                  m_pMixture->setScaleX(1);
                                                                                              }),
                                                                             NULL)));
                

            }
            
        }
        
    }else if (pDragNode==m_pDragBowl) {
        Rect rect = m_pFreezer->getBarrelWorldRect();
        rect.size.height +=100;
        if(rect.containsPoint(worldPoint)){
            m_pDragBowl->setTouchEnabled(false);
            m_pDragBowl->removeFromParent();
            _showPourBatter();
            m_pGuideLayer->removeGuide();
        }
    }
}
void MakeIcecreamScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
    if (!m_pBowl) {
        return;
    }
    if (pDragNode == m_pDragBowl) {
        return;
    }
    if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        stick->stopAllActions();
        m_pWhrisk->stopAllActions();
        m_pMixture->stopAllActions();
    }else{
        Rect rect = m_pBowl->getBoundingBox();
        if (m_nAddCount==0) {
            if (pDragNode!=m_pFlour) {
                pDragNode->back(0,[=](){
                    if (pDragNode==m_pEgg) {
                        m_pEgg2->setVisible(false);
                        pDragNode->setDragSprite(Sprite::create(localPath("mix/egg_small.png")));
                        pDragNode->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
                    }
                });
                return;
            }
        }
        if (rect.containsPoint(pDragNode->getPosition()) && m_bReadyAdd) {
            pDragNode->setTouchEnabled(false);
            m_bReadyAdd = false;
            _addIngredient((IngredientIndex)pDragNode->getTag());
        }else{
            pDragNode->back(0,[=](){
                if (pDragNode==m_pEgg) {
                    m_pEgg2->setVisible(false);
                    pDragNode->setDragSprite(Sprite::create(localPath("mix/egg_small.png")));
                    pDragNode->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
                }
            });
        }
    }
}
void MakeIcecreamScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    AudioHelp::getInstance()->stopLoopEffect();
    if (pDragNode==m_pEgg) {
        m_pEgg2->setVisible(false);
        pDragNode->setDragSprite(Sprite::create(localPath("mix/egg_small.png")));
        pDragNode->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
    }else if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        stick->stopAllActions();
        m_pWhrisk->stopAllActions();
        m_pMixture->stopAllActions();
    }
}

#pragma mark - initData
void MakeIcecreamScene::_initData()
{
    setExPath("content/make/icecream/");
    
}


void MakeIcecreamScene::_showShelf()
{
    m_pShelfNode = Node::create();
    this->addChildToBGLayer(m_pShelfNode);
    m_pShelfNode->setPosition(Vec2(0, visibleSize.height/2));
    
    
    m_pShelf = Sprite::create(localPath("bg/table2_3.png"));
    m_pShelfNode->addChild(m_pShelf);
    CMVisibleRect::setPositionAdapted(m_pShelf, 320, 840,kBorderNone,kBorderTop);
    m_pShelf->setAnchorPoint(Vec2(0.5, 0.9));
    
    _showShelfBoard();
    
    m_pSounder = Sprite::create(localPath("bg/speakers_big.png"));
    m_pShelfNode->addChild(m_pSounder,1);
    CMVisibleRect::setPositionAdapted(m_pSounder, 68, 800,kBorderNone,kBorderTop);
    
    m_pSounder2 = Sprite::create(localPath("bg/speakers_small.png"));
    m_pShelfNode->addChild(m_pSounder2,1);
    CMVisibleRect::setPositionAdapted(m_pSounder2, 184, 814,kBorderNone,kBorderTop);
    
    m_pShelfNode->runAction(Sequence::create(DelayTime::create(0.5),
                                             EaseBackOut::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height/2))), NULL));
    
    for (int i = 0; i<3; i++) {
        ParticleSystemQuad* pNode = ParticleSystemQuad::create("particle/note.plist");
        std::stringstream ostr;
        int radom = arc4random()%6+1;
        ostr<<"particle/note_"<<radom<<".png";
        pNode->setTexture(Sprite::create(ostr.str())->getTexture());
        m_pSounder->addChild(pNode);
        pNode->setPosition(m_pSounder->getContentSize()*0.5);
    }
    for (int i = 0; i<3; i++) {
        ParticleSystemQuad* pNode = ParticleSystemQuad::create("particle/note.plist");
        std::stringstream ostr;
        int radom = arc4random()%6+1;
        ostr<<"particle/note_"<<radom<<".png";
        pNode->setTexture(Sprite::create(ostr.str())->getTexture());
        m_pSounder2->addChild(pNode);
        pNode->setPosition(m_pSounder->getContentSize()*0.5);
    }
}

void MakeIcecreamScene::_showShelfBoard()
{
    
    m_pBoardBack = Sprite::create(localPath("bg/table2_2.png"));
    m_pShelfNode->addChild(m_pBoardBack);
    CMVisibleRect::setPositionAdapted(m_pBoardBack, 430+visibleSize.width, 848,kBorderNone,kBorderTop);
    m_pBoardBack->setAnchorPoint(Vec2(0.5, -1));
    
    m_pBoard = Sprite::create(localPath("bg/table2_1.png"));
    m_pShelfNode->addChild(m_pBoard);
    CMVisibleRect::setPositionAdapted(m_pBoard, 430+visibleSize.width, 848,kBorderNone,kBorderTop);
    m_pBoard->setAnchorPoint(Vec2(0.5, 1));
    
    m_pBoard->runAction(Sequence::create(DelayTime::create(1.5),
                                         EaseBackOut::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0))), NULL));
    
    m_pBoardBack->runAction(Sequence::create(DelayTime::create(1.5),
                                             EaseBackOut::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0))), NULL));
}
void MakeIcecreamScene::_removeShelfBoard()
{
    if (!m_pBoard) {
        return;
    }
    m_pBoard->runAction(Sequence::create(DelayTime::create(0.5),
                                         EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                         CallFunc::create([=](){
        m_pBoard->removeFromParent();
        m_pBoard = nullptr;
    }), NULL));
    
    m_pBoardBack->runAction(Sequence::create(DelayTime::create(0.5),
                                             EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                             CallFunc::create([=](){
        m_pBoardBack->removeFromParent();
        m_pBoardBack = nullptr;
    }), NULL));
}

void MakeIcecreamScene::_showIngredient(int step)
{
    if(0==step){
        m_pFlour = _createDrageNode(localPath("mix/flour_small.png"),false);
        this->addChildToContentLayer(m_pFlour);
        CMVisibleRect::setPositionAdapted(m_pFlour, 292+visibleSize.width, 626-visibleSize.height/2,kBorderNone,kBorderTop);
        m_pFlour->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        m_pFlour->setTag(0);
        
        m_pMilk = _createDrageNode(localPath("mix/milk_small.png"),false);
        this->addChildToContentLayer(m_pMilk);
        CMVisibleRect::setPositionAdapted(m_pMilk, 423+visibleSize.width, 626-visibleSize.height/2,kBorderNone,kBorderTop);
        m_pMilk->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        m_pMilk->setTag(1);
        
        m_pSugar = _createDrageNode(localPath("mix/sugar_small.png"),false);
        this->addChildToContentLayer(m_pSugar);
        CMVisibleRect::setPositionAdapted(m_pSugar, 554+visibleSize.width, 626-visibleSize.height/2,kBorderNone,kBorderTop);
        m_pSugar->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        m_pSugar->setTag(2);
        
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, visibleSize.height/2)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pFlour->runAction(action);
        m_pMilk->runAction(action->clone());
        m_pSugar->runAction(action->clone());
    }else if (1==step) {
        m_pFlavor = _createDrageNode(localPath("mix/vanilla_small.png"),false);
        this->addChildToContentLayer(m_pFlavor);
        CMVisibleRect::setPositionAdapted(m_pFlavor, 284+visibleSize.width, 626-visibleSize.height/2,kBorderNone,kBorderTop);
        m_pFlavor->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        m_pFlavor->setActionPoint(Vec2(102, 133));
        m_pFlavor->setTag(3);
        
        m_pOil = _createDrageNode(localPath("mix/olive_small.png"),false);
        this->addChildToContentLayer(m_pOil);
        CMVisibleRect::setPositionAdapted(m_pOil, 390+visibleSize.width, 626-visibleSize.height/2,kBorderNone,kBorderTop);
        m_pOil->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        m_pOil->setActionPoint(Vec2(48, 165));
        m_pOil->setTag(4);
        
        m_pSalt = _createDrageNode(localPath("mix/salt_small.png"),false);
        this->addChildToContentLayer(m_pSalt);
        CMVisibleRect::setPositionAdapted(m_pSalt, 480+visibleSize.width, 626-visibleSize.height/2,kBorderNone,kBorderTop);
        m_pSalt->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        m_pSalt->setActionPoint(Vec2(48, 165));
        m_pSalt->setTag(5);
        
        m_pEgg = _createDrageNode(localPath("mix/egg_small.png"),false);
        this->addChildToContentLayer(m_pEgg);
        CMVisibleRect::setPositionAdapted(m_pEgg, 564+visibleSize.width, 626-visibleSize.height/2,kBorderNone,kBorderTop);
        m_pEgg->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        m_pEgg->setTag(6);
        
        m_pEgg2 = _createDrageNode(localPath("mix/egg_small1.png"),false);
        this->addChildToContentLayer(m_pEgg2);
        m_pEgg2->setVisible(false);
        m_pEgg2->setTouchEnabled(false);
        CMVisibleRect::setPositionAdapted(m_pEgg2, 564+visibleSize.width, 650-visibleSize.height/2,kBorderNone,kBorderTop);
        m_pEgg2->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        m_pEgg2->setTag(7);
        
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, visibleSize.height/2)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pFlavor->runAction(action);
        m_pOil->runAction(action->clone());
        m_pSalt->runAction(action->clone());
        m_pEgg->runAction(action->clone());
        m_pEgg2->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, visibleSize.height/2)),
                                            CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
            }
        }), NULL));
    }
}

void MakeIcecreamScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("mix/stir/bowl_down.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width/2, 270);
    
    m_pBowlFront = Sprite::create(localPath("mix/stir/bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    CMVisibleRect::setPositionAdapted(m_pBowlFront, 320-visibleSize.width/2, 270);
    m_pBowlFront->setLocalZOrder(9);
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 320, 270);
    m_pMixture->setLocalZOrder(5);

    m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));

}
void MakeIcecreamScene::_addIngredient(IngredientIndex index)
{
    std::string lNameStr = "";
    float dt = 1;
    Vec2 posAdd = Vec2::ZERO;
    switch (index) {
#pragma mark =======add flour====
        case IngredientIndex::eFlour:
        {
            lNameStr = "mix/stir/flour.png";
            m_pFlour->removeFromParent();
            
            Node* pCocosFlour = CSLoader::createNode("cocos/FlourNode.csb");
            
            Node* pBagNode = Node::create();
            this->addChildToContentLayer(pBagNode);
            pBagNode->setLocalZOrder(5);
            
            Node* pCocosBag = pCocosFlour->getChildByName("flour_1");
            Node* pCocosIn = pCocosFlour->getChildByName("flour");
            Sprite* pBag = Sprite::create(localPath("mix/flour_1.png"));
            Sprite* pIn = Sprite::create(localPath("mix/flour.png"));
            pBagNode->addChild(pBag);
            pBagNode->addChild(pIn);
            pBag->setPosition(pCocosBag->getPosition());
            pIn->setPosition(pCocosIn->getPosition());
            pIn->setAnchorPoint(pCocosIn->getAnchorPoint());
            
            pBagNode->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+250));
            
            pBagNode->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                                 CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.5),
                                                              MoveBy::create(1, Vec2(0, -5)), NULL),
                                                FadeOut::create(0.5),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pBagNode->convertToWorldSpace(pIn->getPosition())));
                pParticle->setLocalZOrder(5);
            }),
                                                 DelayTime::create(2),
                                                 RotateTo::create(0.3, 0),
                                                 MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==3) {
                    _showIngredient(1);
                }
                
                m_bReadyAdd = true;
            }),
                                                 NULL));
        }
            break;
            
#pragma mark =======add milk====
        case IngredientIndex::eMilk:
        {
            lNameStr = "mix/stir/milk.png";
            m_pMilk->removeFromParent();
            
            Node* pCocosMilk = CSLoader::createNode("cocos/MilkNode.csb");
            
            Node* pMilkNode = Node::create();
            this->addChildToContentLayer(pMilkNode);
            pMilkNode->setLocalZOrder(5);
            
            Node* pCocosBottle = pCocosMilk->getChildByName("milk");
            Node* pCocosIn = pCocosMilk->getChildByName("milk_1");
            Sprite* pBag = Sprite::create(localPath("mix/milk.png"));
            Sprite* pIn = Sprite::create(localPath("mix/milk_1.png"));
            pMilkNode->addChild(pBag);
            pMilkNode->addChild(pIn);
            pBag->setPosition(pCocosBottle->getPosition());
            pIn->setPosition(pCocosIn->getPosition());
            pIn->setAnchorPoint(pCocosIn->getAnchorPoint());
            
            pMilkNode->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+250));
            
            pMilkNode->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                                 CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("add_water.mp3");
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.5),
                                                              MoveBy::create(0.5, Vec2(-20, 0)), NULL),
                                                FadeOut::create(0.5),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/milk.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setScaleX(-1);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pMilkNode->convertToWorldSpace(pIn->getPosition())));
                pParticle->setLocalZOrder(5);
            }),
                                                  DelayTime::create(2),
                                                  RotateTo::create(0.3, 0),
                                                  MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                  CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==3) {
                    _showIngredient(1);
                }
                
                m_bReadyAdd = true;
            }),
                                                 NULL));
            
        }
            break;
#pragma mark =======add sugar====
        case IngredientIndex::eSugar:
        {
            lNameStr = "mix/stir/sugar.png";
            m_pSugar->removeFromParent();
            
            posAdd = Vec2(-85, -30);
            Node* pSugarNode = Node::create();
            this->addChildToContentLayer(pSugarNode);
            pSugarNode->setLocalZOrder(5);
            
            Sprite* pBowl = Sprite::create(localPath("mix/bowl_sugar_down.png"));
            Sprite* pIn = Sprite::create(localPath("mix/sugar.png"));
            Sprite* pBowlFront = Sprite::create(localPath("mix/bowl_sugar_on.png"));
            pSugarNode->addChild(pBowl);
            pSugarNode->addChild(pIn);
            pSugarNode->addChild(pBowlFront);
            
            pSugarNode->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+250));
            pSugarNode->setLocalZOrder(5);
            pSugarNode->runAction(Sequence::create(RotateBy::create(0.3, -100),
                                                   CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.5),
                                                              MoveBy::create(1, Vec2(-40, -20)), NULL),
                                                FadeOut::create(0.5),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setScaleX(-1);
                Vec2 pos = pSugarNode->convertToWorldSpace(Vec2(pIn->getPositionX()-60, pIn->getPositionY()+20));
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pos));
                pParticle->setLocalZOrder(5);
                                                                   }),
                                                  DelayTime::create(3),
                                                  RotateTo::create(0.3, 0),
                                                   MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                   CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==3) {
                    _showIngredient(1);
                }
                
                m_bReadyAdd = true;
            }),
                                                  NULL));
        }
            break;
#pragma mark =======add flavor====
        case IngredientIndex::eFlavor:
        {
            lNameStr = "mix/stir/vanilla.png";
            m_pFlavor->removeFromParent();
            
            posAdd = Vec2(20, 10);
            
            Node* pFlavorNode = Node::create();
            this->addChildToContentLayer(pFlavorNode);
            pFlavorNode->setLocalZOrder(5);
            
            Sprite* pPlate = Sprite::create(localPath("mix/vanilla_palte.png"));
            Sprite* pIn = Sprite::create(localPath("mix/vanilla.png"));
            pFlavorNode->addChild(pPlate);
            pFlavorNode->addChild(pIn);
            
            pFlavorNode->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+200));
            pFlavorNode->setLocalZOrder(5);
            pFlavorNode->runAction(Sequence::create(RotateBy::create(0.3, -40),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                pIn->removeFromParent();
                for (int i = 0; i<4; i++) {
                    std::stringstream ostr;
                    ostr<<"mix/vanilla_"<<i+1<<".png";
                    Sprite* pVanilla = Sprite::create(localPath(ostr.str()));
                    this->addChildToContentLayer(pVanilla);
                    pVanilla->setPosition(pFlavorNode->getPosition());
                    pVanilla->setLocalZOrder(11);
                    float angle = 100-10*(arc4random()%10);
                    Vec2 pos = Vec2(-100.0+20*(arc4random()%10), -10.0f+10.0f*(arc4random()%5));
                    pVanilla->runAction(Sequence::create(Spawn::create(RotateBy::create(1, angle),
                                                                       MoveTo::create(1, Vec2(-pos, m_pBowl->getPosition())), NULL),
                                                         FadeOut::create(0.1),
                                                         CallFunc::create([=](){
                        pVanilla->removeFromParent();
                    }), NULL));
                }
                                                                    }),
                                                   DelayTime::create(1),
                                                   RotateTo::create(0.3, 0),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=]{
                m_nAddCount++;
                if (m_nAddCount==8) {
                    _finishAdd();
                }
                
                m_bReadyAdd = true;
            }),
                                                   NULL));
        }
            break;
#pragma mark =======add oil====
        case IngredientIndex::eOil:
        {
            lNameStr = "mix/stir/oil.png";
            m_pOil->removeFromParent();
            
            AudioHelp::getInstance()->playEffect("add_water.mp3");
            
            Sprite* pBottle = Sprite::create(localPath("mix/olive.png"));
            Sprite* pStream = Sprite::create(localPath("mix/olive_1.png"));
            this->addChildToContentLayer(pBottle);
            pBottle->setLocalZOrder(5);
            this->addChildToContentLayer(pStream);
            pStream->setLocalZOrder(5);
            pBottle->setAnchorPoint(Vec2(10.0/pBottle->getContentSize().width, 58/pBottle->getContentSize().height));
            pStream->setAnchorPoint(Vec2(1, 1));
            
            pBottle->setPosition(Vec2(m_pBowl->getPositionX(), m_pBowl->getPositionY()+200));
            pStream->setPosition(pBottle->getPosition());
            pStream->setOpacity(0);
            
            pStream->runAction(FadeIn::create(0.3));
            pStream->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("mix/olive_"), 1, 2)));
            pBottle->runAction(Sequence::create(DelayTime::create(1.5),
                                                CallFunc::create([=](){
                pStream->runAction(FadeOut::create(0.3));
                pBottle->setLocalZOrder(10);
            }),
                                                Spawn::create(RotateBy::create(0.3, 90),
                                                              MoveBy::create(0.3, Vec2(0, 100)), NULL),
                                                MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                CallFunc::create([=]{
                pStream->removeFromParent();
                m_nAddCount++;
                if (m_nAddCount==8) {
                    _finishAdd();
                }
                
                m_bReadyAdd = true;
            }), NULL));
        }
            break;
#pragma mark =======add salt====
        case IngredientIndex::eSalt:
        {
            lNameStr = "mix/stir/salt.png";
            m_pSalt->removeFromParent();
            
            posAdd = Vec2(-40, 10);
            
            Sprite* pBottle = Sprite::create(localPath("mix/salt.png"));
            this->addChildToContentLayer(pBottle);
            pBottle->setLocalZOrder(5);
            pBottle->setAnchorPoint(Vec2(50.0f/pBottle->getContentSize().width, 180.0f/pBottle->getContentSize().height));
            
            pBottle->setPosition(Vec2(m_pBowl->getPositionX(), m_pBowl->getPositionY()+250));
            pBottle->setLocalZOrder(5);
            pBottle->runAction(Sequence::create(RotateBy::create(0.3, -120),
                                                CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setScaleX(-1);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pBottle->getParent()->convertToWorldSpace(pBottle->getPosition())));
                pParticle->setLocalZOrder(5);
            }),
                                                JumpBy::create(1.5, Vec2::ZERO, 20, 5),
                                                Spawn::create(RotateTo::create(0.3, 0),
                                                              MoveBy::create(0.3, Vec2(0, 100)), NULL),
                                                MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                CallFunc::create([=]{
                pBottle->removeFromParent();
                m_nAddCount++;
                if (m_nAddCount==8) {
                    _finishAdd();
                }
                
                m_bReadyAdd = true;
            }),
                                                NULL));
        }
            break;
#pragma mark =======add egg====
        case IngredientIndex::eEgg:
        {
            lNameStr = "mix/stir/egg1.png";
            m_pEgg->setTouchEnabled(false);
            m_pEgg->setVisible(false);
//            m_pEgg->removeFromParent();
            
            posAdd = Vec2(50, -20);
            Sprite* pEgg = Sprite::create(localPath("mix/egg.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setLocalZOrder(5);
            pEgg->setPosition(Vec2(m_pBowl->getPositionX(), m_pBowl->getPositionY()+250));
            
            Sprite* pShellLeft = Sprite::create(localPath("mix/egg_shell_1.png"));
            this->addChildToContentLayer(pShellLeft);
            pShellLeft->setLocalZOrder(4);
            pShellLeft->setPosition(Vec2(m_pBowl->getPositionX()-50, m_pBowl->getPositionY()+250));
            pShellLeft->setOpacity(0);
            
            Sprite* pShellLeftFront = Sprite::create(localPath("mix/egg_shell_1on.png"));
            this->addChildToContentLayer(pShellLeftFront);
            pShellLeftFront->setLocalZOrder(6);
            pShellLeftFront->setPosition(pShellLeft->getPosition());
            pShellLeftFront->setOpacity(0);
            
            Sprite* pShellRight = Sprite::create(localPath("mix/egg_shell_2.png"));
            this->addChildToContentLayer(pShellRight);
            pShellRight->setLocalZOrder(4);
            pShellRight->setPosition(Vec2(m_pBowl->getPositionX()+50, m_pBowl->getPositionY()+250));
            pShellRight->setOpacity(0);
            
            Sprite* pShellRightFront = Sprite::create(localPath("mix/egg_shell_2on.png"));
            this->addChildToContentLayer(pShellRightFront);
            pShellRightFront->setLocalZOrder(6);
            pShellRightFront->setPosition(pShellRight->getPosition());
            pShellRightFront->setOpacity(0);
            
            Sprite* pShell0 = Sprite::create(localPath("mix/egg_shell_1down.png"));
            this->addChildToContentLayer(pShell0);
            pShell0->setLocalZOrder(4);
            pShell0->setPosition(pEgg->getPosition());
            pShell0->setOpacity(0);
            
            Sprite* pShell1 = Sprite::create(localPath("mix/egg_shell_2down.png"));
            this->addChildToContentLayer(pShell1);
            pShell1->setLocalZOrder(4);
            pShell1->setPosition(pEgg->getPosition());
            pShell1->setOpacity(0);
            
            Sprite* pIn = Sprite::create(localPath("mix/stir/egg1.png"));
            pIn->setScale(0.6);
            this->addChildToContentLayer(pIn);
            pIn->setLocalZOrder(5);
            pIn->setPosition(pEgg->getPosition());
            pIn->setOpacity(0);
            
            pEgg->runAction(Sequence::create(ScaleTo::create(0.3, 0.9),
                                             EaseBackOut::create(ScaleTo::create(0.5, 1)),
                                             CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("add_egg.mp3");
                pEgg->setVisible(false);
                pIn->setOpacity(255);
                pShellLeft->setOpacity(255);
                pShellLeftFront->setOpacity(255);
                pShellRight->setOpacity(255);
                pShellRightFront->setOpacity(255);
                pShell0->setOpacity(255);
                pShell1->setOpacity(255);
                pShell0->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, -100.0f+20.0f*(arc4random()%10)),
                                                                  JumpBy::create(0.5, Vec2(-20, -70), 10, 1),
                                                                  FadeOut::create(0.5), NULL),
                                                    CallFunc::create([=](){
                    pShell0->removeFromParent();
                }), NULL));
                pShell1->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, -100.0f+20.0f*(arc4random()%10)),
                                                                  JumpBy::create(0.5, Vec2(20, -70), 10, 1),
                                                                  FadeOut::create(0.5), NULL),
                                                    CallFunc::create([=](){
                    pShell1->removeFromParent();
                }), NULL));
                pIn->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -150)),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                    pShellLeft->removeFromParent();
                    pShellLeftFront->removeFromParent();
                    pShellRight->removeFromParent();
                    pShellRightFront->removeFromParent();
                    m_pEgg2->setTouchEnabled(true);
                    m_nAddCount++;
                    if (m_nAddCount==8) {
                        _finishAdd();
                    }
                    
                    m_bReadyAdd = true;
                }), NULL));
                
            }), NULL));
        }
            break;
        case IngredientIndex::eEgg2:
        {
            lNameStr = "mix/stir/egg2.png";
            m_pEgg2->setTouchEnabled(false);
            m_pEgg2->setVisible(false);
//            m_pEgg2->removeFromParent();
            
            posAdd = Vec2(30, -30);
            
            Sprite* pEgg = Sprite::create(localPath("mix/egg.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setLocalZOrder(5);
            pEgg->setPosition(Vec2(m_pBowl->getPositionX(), m_pBowl->getPositionY()+250));
            
            Sprite* pShellLeft = Sprite::create(localPath("mix/egg_shell_1.png"));
            this->addChildToContentLayer(pShellLeft);
            pShellLeft->setLocalZOrder(4);
            pShellLeft->setPosition(Vec2(m_pBowl->getPositionX()-50, m_pBowl->getPositionY()+250));
            pShellLeft->setOpacity(0);
            
            Sprite* pShellLeftFront = Sprite::create(localPath("mix/egg_shell_1on.png"));
            this->addChildToContentLayer(pShellLeftFront);
            pShellLeftFront->setLocalZOrder(6);
            pShellLeftFront->setPosition(pShellLeft->getPosition());
            pShellLeftFront->setOpacity(0);
            
            Sprite* pShellRight = Sprite::create(localPath("mix/egg_shell_2.png"));
            this->addChildToContentLayer(pShellRight);
            pShellRight->setLocalZOrder(4);
            pShellRight->setPosition(Vec2(m_pBowl->getPositionX()+50, m_pBowl->getPositionY()+250));
            pShellRight->setOpacity(0);
            
            Sprite* pShellRightFront = Sprite::create(localPath("mix/egg_shell_2on.png"));
            this->addChildToContentLayer(pShellRightFront);
            pShellRightFront->setLocalZOrder(6);
            pShellRightFront->setPosition(pShellRight->getPosition());
            pShellRightFront->setOpacity(0);
            
            Sprite* pShell0 = Sprite::create(localPath("mix/egg_shell_1down.png"));
            this->addChildToContentLayer(pShell0);
            pShell0->setLocalZOrder(4);
            pShell0->setPosition(pEgg->getPosition());
            pShell0->setOpacity(0);
            
            Sprite* pShell1 = Sprite::create(localPath("mix/egg_shell_2down.png"));
            this->addChildToContentLayer(pShell1);
            pShell1->setLocalZOrder(4);
            pShell1->setPosition(pEgg->getPosition());
            pShell1->setOpacity(0);
            
            Sprite* pIn = Sprite::create(localPath("mix/stir/egg1.png"));
            pIn->setScale(0.6);
            this->addChildToContentLayer(pIn);
            pIn->setLocalZOrder(5);
            pIn->setPosition(pEgg->getPosition());
            pIn->setOpacity(0);
            
            pEgg->runAction(Sequence::create(ScaleTo::create(0.3, 0.9),
                                             EaseBackOut::create(ScaleTo::create(0.5, 1)),
                                             CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("add_egg.mp3");
                pEgg->setVisible(false);
                pIn->setOpacity(255);
                pShellLeft->setOpacity(255);
                pShellLeftFront->setOpacity(255);
                pShellRight->setOpacity(255);
                pShellRightFront->setOpacity(255);
                pShell0->setOpacity(255);
                pShell1->setOpacity(255);
                pShell0->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, -100.0f+20.0f*(arc4random()%10)),
                                                                  JumpBy::create(0.5, Vec2(-20, -70), 10, 1),
                                                                  FadeOut::create(0.5), NULL),
                                                    CallFunc::create([=](){
                    pShell0->removeFromParent();
                }), NULL));
                pShell1->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, -100.0f+20.0f*(arc4random()%10)),
                                                                  JumpBy::create(0.5, Vec2(20, -70), 10, 1),
                                                                  FadeOut::create(0.5), NULL),
                                                    CallFunc::create([=](){
                    pShell1->removeFromParent();
                }), NULL));
                pIn->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -150)),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                    pShellLeft->removeFromParent();
                    pShellLeftFront->removeFromParent();
                    pShellRight->removeFromParent();
                    pShellRightFront->removeFromParent();
                    m_nAddCount++;
                    if (m_nAddCount==8) {
                        _finishAdd();
                    }
                    
                    m_bReadyAdd = true;
                }), NULL));
                
            }), NULL));

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
            
//            m_bReadyAdd = true;
        }), NULL));
    }
}

void MakeIcecreamScene::_finishAdd()
{
    _removeShelfBoard();
    
    m_pWhrisk = _createDrageNode(localPath("mix/stir/agitator.png"));
    this->addChildToContentLayer(m_pWhrisk);
    CMVisibleRect::setPositionAdapted(m_pWhrisk, 350+visibleSize.width, 450);
    m_pWhrisk->setActionPoint(Vec2(35, 68));
    m_pWhrisk->setLocalZOrder(10);
    
    Sprite* pSprite = Sprite::create(localPath("mix/stir/agitator_1.png"));
    m_pWhrisk->addChild(pSprite,-1);
    pSprite->setName("stick");
    
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

void MakeIcecreamScene::_finishMix()
{
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 200))),
                                          CallFunc::create([=]()
                                                           {
                                                               _moveawayBatter();
                                                           }),
                                          NULL));
}


void MakeIcecreamScene::_moveawayBatter()
{
    auto action = Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    m_pBowl->runAction(action);
    m_pBowlFront->runAction(action->clone());
    m_pMixture->runAction(action->clone());
    _showShelfBoard();
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
        _showDragBatter();
        _showFreeze();
    }), NULL));
}

//    freeze UI
void MakeIcecreamScene::_showFreeze()
{
    m_pFreezer = BarrelFreezerNode::create();
    this->addChildToContentLayer(m_pFreezer);
    CMVisibleRect::setPosition(m_pFreezer, 320-visibleSize.width, 290);
    m_pFreezer->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pFreezer->setState(BarrelFreezerNode::eStateOpen);
    m_pFreezer->onFinishedCallback = [=](){
        m_pFreezer->setTouchEnable(false);
        m_pGuideLayer->removeGuide();
        m_pFreezer->runAction(Sequence::create(DelayTime::create(1),
                                               Spawn::create(ScaleTo::create(1, 1.5),
                                                             MoveBy::create(0.5, Vec2(60, 0)), NULL),
                                               DelayTime::create(1.5),
                                               Spawn::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                             ScaleTo::create(0.5, 1), NULL),
                                               CallFunc::create([=](){
            SceneManager::replaceTheScene<GetIcecreamScene>();
        }), NULL));
    };
    m_pFreezer->onHandleCallback = [=](){
        m_pGuideLayer->removeGuide();
    };
}

void MakeIcecreamScene::_showDragBatter()
{
    m_pDragBowl = _createDrageNode(localPath("freeze/bowl_batter.png"),false);
    this->addChildToContentLayer(m_pDragBowl);
    CMVisibleRect::setPositionAdapted(m_pDragBowl, 470+visibleSize.width, 700-300,kBorderNone,kBorderTop);
    m_pDragBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 300)),
                                            CallFunc::create([=](){
        m_pDragBowl->setOrgPositionDefault();
        m_pDragBowl->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pDragBowl->getPosition(), Vec2(visibleSize.width*0.5,visibleSize.height*0.4));
    }), NULL));
}

void MakeIcecreamScene::_showPourBatter()
{
    AudioHelp::getInstance()->playEffect("add_water.mp3");
    m_pPourBowl = Sprite::create(localPath("freeze/1/bowl_down.png"));
    this->addChildToContentLayer(m_pPourBowl);
    m_pPourBowl->setLocalZOrder(1);
    
    m_pPourBowlFront = Sprite::create(localPath("freeze/1/bowl_on.png"));
    this->addChildToContentLayer(m_pPourBowlFront);
    m_pPourBowlFront->setLocalZOrder(5);
    
    m_pPourMixture = Sprite::create(localPath("freeze/1/batter_1.png"));
    this->addChildToContentLayer(m_pPourMixture);
    m_pPourMixture->setLocalZOrder(3);
    
    m_pPourStream = Sprite::create(localPath("freeze/1/batter_2.png"));
    this->addChildToContentLayer(m_pPourStream);
    m_pPourStream->setLocalZOrder(4);
    
    CMVisibleRect::setPosition(m_pPourBowl, 420, 690);
    m_pPourBowlFront->setPosition(m_pPourBowl->getPosition());
    m_pPourMixture->setPosition(m_pPourBowl->getPosition());
    m_pPourStream->setPosition(m_pPourBowl->getPosition());
    
    m_pPourMixture->runAction(Sequence::create(FadeOut::create(2),
                                               CallFunc::create([=](){
        m_pPourMixture->removeFromParent();
        m_pPourStream->removeFromParent();
        auto action = Sequence::create(RotateBy::create(0.3, 50),
                                       MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            pNode->removeFromParent();
        }), NULL);
        m_pPourBowl->runAction(action);
        m_pPourBowlFront->runAction(action->clone());
        _removeShelfBoard();
        m_pFreezer->setState(BarrelFreezerNode::eStateCovor);
        
        m_pFreezer->runAction(Sequence::create(DelayTime::create(3),
                                               MoveBy::create(0.5, Vec2(-60, 0)),
                                               CallFunc::create([=](){
            Rect rect = m_pFreezer->getBatterHandleWorldRect();
            Vec2 pos = Vec2(rect.origin.x, rect.getMidY());
            m_pGuideLayer->showGuideMove(pos, Vec2(pos.x, pos.y+20));
        }), NULL));
    }), NULL));
    m_pFreezer->setState(BarrelFreezerNode::eStateAddBatter);
    m_pPourStream->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("freeze/1/batter_"), 2, 3)));
}