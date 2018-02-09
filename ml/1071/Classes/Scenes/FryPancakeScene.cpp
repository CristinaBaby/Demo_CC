
#include "FryPancakeScene.h"
#include "SceneManager.h"

FryPancakeScene::FryPancakeScene()
{
    m_pBowl = nullptr;
    m_pSpoon = nullptr;
    m_pFryPan = nullptr;
    m_pPlate = nullptr;
//    m_nStep = 0;
    m_nFryCount = 0;
//    m_nMixCount = 0;
    m_bSpoonIsReady = false;
//    m_bReadyAdd = true;
//    m_bCanTouch = false;
}

FryPancakeScene::~FryPancakeScene()
{
}
bool FryPancakeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg2.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _createFryUI();
        AudioHelp::getInstance()->playEffect("vo_pour_oil.mp3");
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    
    
    m_pGameUI->showNormalLayout();
    return true;
}

void FryPancakeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FryPancakeScene::onExit()
{
    ExtensionScene::onExit();
}

void FryPancakeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FryPancakeScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
}

void FryPancakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
    if (pContainer) {
        pContainer->showShadow(false);
    }
    int tag = pDragNode->getTag();
    
    pDragNode->setScale(1);
    pDragNode->setLocalZOrder(50);
    
}

void FryPancakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="spoon") {
        if (pDragNode->index == 0) {
            Rect rect = m_pBowl->getBoundingBox();
            rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                AudioHelp::getInstance()->playEffect("fall.mp3");
                m_pGuideLayer->removeGuide();
                pDragNode->index = 1;
                m_pMixture->runAction(ScaleBy::create(0.5, 0.8));
                m_pMixture->runAction(RotateBy::create(0.5, 60));
                Sprite* pSprite =Sprite::create(localPath("make2/spoon_buttrer.png"));
                m_pSpoon->addChild(pSprite);
                pSprite->setPosition(pDragNode->getActionPoint()-pDragNode->getContentSize()*0.5);
                pSprite->setName("mixture");
            }
        }else if (pDragNode->index == 1){
            Rect rect = m_pPan->getBoundingBox();
            rect.origin = m_pPan->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                AudioHelp::getInstance()->playEffect("buter.mp3");
                m_pGuideLayer->removeGuide();
                pDragNode->index = 2;
                Node* pMixture = pDragNode->getChildByName("mixture");
                if (pMixture) {
                    pMixture->runAction(Sequence::create(DelayTime::create(0.7),
                                                         FadeOut::create(1),
                                                         CallFunc::create([=](){
                        pMixture->removeFromParent();
                    }), NULL));
                }
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pPanOil->getPosition()+Vec2(50, 100)),
                                                      RotateBy::create(0.3, -40),
                                                      CallFunc::create([=](){
                    
                    m_pFryPan = Sprite::create(localPath("make2/rolls1.png"));
                    this->addChildToContentLayer(m_pFryPan);
                    m_pFryPan->setPosition(m_pPanOil->getPosition());
                    m_pFryPan->setScale(0.3);
                    m_pFryPan->setOpacity(0);
                    m_pFryPan->runAction(FadeIn::create(0.3));
                    m_pFryPan->runAction(ScaleTo::create(1, 1));
                    int rotation = 12*(arc4random()%30);
                    m_pFryPan->setRotation(rotation);
                }),
                                                      DelayTime::create(1),
                                                      RotateBy::create(0.3, 40),
                                                      MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                      CallFunc::create([=](){
                    
                    _doFry();
                    pDragNode->removeFromParent();
                }), NULL));
            }
        }
        
    }else if(name == "spatula"){
        if (pDragNode->index == 0) {
            Rect rect = m_pFryPan->getBoundingBox();
            rect.origin = m_pFryPan->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                AudioHelp::getInstance()->playEffect("fall.mp3");
                m_pGuideLayer->removeGuide();
                pDragNode->index = 1;
                m_pFryPan->removeFromParent();
                m_pFryPan = nullptr;
                
                AudioHelp::getInstance()->stopLoopEffect();
                
                Sprite* pSprite =Sprite::create(localPath("make2/rolls7.png"));
                pDragNode->addChild(pSprite);
                pSprite->setScale(0.6);
                pSprite->setPosition(pDragNode->getActionPoint()-pDragNode->getContentSize()*0.5);
                pSprite->setName("pancake");
                if(m_nFryCount == 0){
                    m_pGuideLayer->showGuideMove(m_pSpatula->getPosition(), m_pPlate->getPosition());
                }
            }
        }else if (pDragNode->index == 1){
            Rect rect = m_pPlate->getBoundingBox();
            rect.origin = m_pPlate->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                AudioHelp::getInstance()->playEffect("fall.mp3");
                m_pGuideLayer->removeGuide();
                pDragNode->index = 2;
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pPlate->getPosition()+Vec2(50, 100)),
                                                      RotateBy::create(0.3, -40),
                                                      CallFunc::create([=](){
                    Node* pMixture = pDragNode->getChildByName("pancake");
                    if (pMixture) {
                        pMixture->removeFromParent();
                    }
                    Sprite* pSprite =Sprite::create(localPath("make2/rolls7.png"));
                    this->addChildToContentLayer(pSprite);
                    pSprite->setPosition(m_pPlate->getPosition());
                    pSprite->setScale(0.7);
                    int rotation = 12*(arc4random()%30);
                    pSprite->setRotation(rotation);
                    
                    m_nFryCount++;
                    if (m_nFryCount==3) {
                        _finishAll();
                    }else{
                        _showSpoon();
                    }
                }),
                                                      DelayTime::create(0.5),
                                                      RotateBy::create(0.3, 40),
                                                      MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                      CallFunc::create([=](){
                    pDragNode->removeFromParent();
                }), NULL));
            }
        }
    }
}

void FryPancakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->stopAllActions();
 
    std::string name = pDragNode->getName();
    if (name=="oil") {
        pDragNode->setTouchEnabled(false);
        pDragNode->removeFromParent();
        
        _addOil();

    }
//    if (pDragNode==m_pSpoon) {
//        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
//        if (stick) {
//            stick->stopAllActions();
//        }
//        m_pSpoon->stopAllActions();
//        m_pMixture->stopAllActions();
//        m_pMixture->setRotation(0);
//    }else{
//        Rect rect = m_pBowl->getBoundingBox();
//        rect.size = rect.size+Size(100, 100);
//    }
}

void FryPancakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->stopAllActions();
    
    if (pDragNode==m_pSpoon) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        if (stick) {
            stick->stopAllActions();
        }
        m_pSpoon->stopAllActions();
        m_pMixture->stopAllActions();
        m_pMixture->setRotation(0);
    }

    _back(pDragNode);
}

#pragma mark - initData
void FryPancakeScene::_initData()
{
    setExPath("content/make/japanese/");
    
    m_nNextSceneTag = GameUIEvent::eSceneTagFrySausage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagFrySausage;
}

#pragma mark ==== Mix UI =====
void FryPancakeScene::_createFryUI()
{
    m_pStove = Sprite::create(localPath("make2/stove.png"));
    this->addChildToContentLayer(m_pStove);
    CMVisibleRect::setPositionAdapted(m_pStove, 320, 330);
    
    m_pFire = Sprite::create(localPath("make2/stove_1.png"));
    this->addChildToContentLayer(m_pFire);
    CMVisibleRect::setPositionAdapted(m_pFire, 320, 330);
    
    m_pFireInner = Sprite::create(localPath("make2/stove_2.png"));
    this->addChildToContentLayer(m_pFireInner);
    CMVisibleRect::setPositionAdapted(m_pFireInner, 320, 330);
    m_pFireInner->runAction(RepeatForever::create(Sequence::create(Spawn::create(FadeTo::create(0.5, 150),
                                                                                 ScaleTo::create(0.5, 1.1), NULL),
                                                                   Spawn::create(FadeTo::create(0.5, 255),
                                                                                 ScaleTo::create(0.5, 1), NULL),
                                                                   DelayTime::create(0.5), NULL)));
    m_pOil = _createDrageNode(localPath("make2/olive.png"));
    this->addChildToContentLayer(m_pOil);
    CMVisibleRect::setPositionAdapted(m_pOil, 240-visibleSize.width/2, 780);
    m_pOil->setName("oil");
    
    ContainerNode* pOil = ContainerNode::create(m_pOil);
    pOil->addShadow(localPath("make2/olive_shadow.png"));
    m_pOil->setUserData(pOil);
    
    m_pPan = Sprite::create(localPath("make2/pan.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 200+50-visibleSize.width, 300+50);
    
    m_pOil->setTouchEnabled(false);
    m_pOil->runAction(Sequence::create(DelayTime::create(1),
                                       MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)),
                                       CallFunc::create([=](){
        m_pOil->setOrgPositionDefault();
        m_pOil->setTouchEnabled(true);
    }), NULL));
    m_pPan->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                       CallFunc::create([=](){
        m_pFireInner->stopAllActions();
    }), NULL));
    
}

void FryPancakeScene::_showBowl()
{
    
    m_pBowl = Sprite::create(localPath("make1/bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 180-visibleSize.width/2, 680+20);
    
    m_pMixture = Sprite::create(localPath("make1/stir7.png"));
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 180-visibleSize.width/2, 680+20);
    
    m_pBowl->setScale(0.6);
    m_pMixture->setScale(0.6);
    
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pMixture->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    
}

void FryPancakeScene::_showSpoon()
{
    AudioHelp::getInstance()->playEffect("vo_mix_ingredients.mp3");
    
    m_bSpoonIsReady = false;
    m_pSpoon = _createDrageNode(localPath("make2/spoon.png"));
    this->addChildToContentLayer(m_pSpoon);
    CMVisibleRect::setPositionAdapted(m_pSpoon, 420+visibleSize.width, 650+20);
    m_pSpoon->setActionPoint(Vec2(65, 40));
    m_pSpoon->setLocalZOrder(10);
    m_pSpoon->setName("spoon");
    m_pSpoon->index = 0;
    m_pSpoon->setRotation(30);
    
    m_pSpoon->setTouchEnabled(false);
    m_pSpoon->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                               m_pSpoon->setOrgPositionDefault();
                                                               m_pSpoon->setTouchEnabled(true);
                                                           }),
                                          NULL));
}

void FryPancakeScene::_removeSpoon()
{
    m_pSpoon->setTouchEnabled(false);
    m_pSpoon->runAction(Sequence::create(DelayTime::create(0.5),
                                         EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                         CallFunc::create([=]()
                                                          {
                                                              m_pSpoon->removeFromParent();
                                                          }),
                                         NULL));
}


void FryPancakeScene::_showSpatula()
{
    m_bSpatulaIsReady = false;
    m_pSpatula = _createDrageNode(localPath("make2/shovel.png"));
    this->addChildToContentLayer(m_pSpatula);
    CMVisibleRect::setPositionAdapted(m_pSpatula, 400+80+visibleSize.width, 650+10);
    m_pSpatula->setActionPoint(Vec2(70, 70));
    m_pSpatula->setLocalZOrder(10);
    m_pSpatula->setName("spatula");
    m_pSpatula->setRotation(30);
    m_pSpatula->index = 0;
    
    m_pSpatula->setTouchEnabled(false);
    m_pSpatula->runAction(Sequence::create(DelayTime::create(0.5),
                                         EaseBackOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                         CallFunc::create([=]()
                                                          {
                                                              m_pSpatula->setOrgPositionDefault();
                                                              m_pSpatula->setTouchEnabled(true);
                                                          }),
                                         NULL));
    
}

void FryPancakeScene::_removeSpatula()
{
    
    m_pSpatula->setTouchEnabled(false);
    m_pSpatula->runAction(Sequence::create(DelayTime::create(0.5),
                                         EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                         CallFunc::create([=]()
                                                          {
                                                              m_pSpatula->removeFromParent();
                                                          }),
                                         NULL));
}


void FryPancakeScene::_showPlate()
{
    if (m_pPlate) {
        return;
    }
    m_pPlate = Sprite::create(localPath("make2/plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 480+visibleSize.width, 680+20);
    m_pPlate->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
}


#pragma mark ======== deel ========================
void FryPancakeScene::_back(DragNode* pDragNode)
{
    pDragNode->setTouchEnabled(false);
    pDragNode->back(0,[=](){
        ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
        if (pContainer) {
            pContainer->showShadow();
        }
        pDragNode->setTouchEnabled(true);
    });
}


void FryPancakeScene::_addOil()
{
    Sprite* pBottle = Sprite::create(localPath("make2/olive1_1.png"));
    pBottle->setPosition(Vec2(m_pPan->getPositionX()+200, m_pPan->getPositionY()+100));
    this->addChildToContentLayer(pBottle);
    
    Sprite* pOilStream = Sprite::create(localPath("make2/olive1_2.png"));
    this->addChildToContentLayer(pOilStream);
    pOilStream->setPosition(pBottle->getPosition());
    pOilStream->setLocalZOrder(10);
    
    pOilStream->runAction(Repeat::create(AnimationHelp::createAnimate(localPath("make2/olive1_"), 2, 3), 10));
    AudioHelp::getInstance()->playPourWaterEffect();
    
    m_pPanOil = Sprite::create(localPath("make2/pan_oil.png"));
    this->addChildToContentLayer(m_pPanOil);
    CMVisibleRect::setPositionAdapted(m_pPanOil, 320, 320+50);
    m_pPanOil->setOpacity(0);
    m_pPanOil->setScale(0.2);
    m_pPanOil->runAction(Sequence::create(DelayTime::create(0.3),
                                          Spawn::create(ScaleTo::create(0.8, 1),
                                                        FadeIn::create(1), NULL), NULL));
    pBottle->runAction(Sequence::create(DelayTime::create(2),
                                        CallFunc::create([=](){
        pOilStream->removeFromParent();
        AudioHelp::getInstance()->stopPourWaterEffect();
        _showBowl();
        _showSpoon();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fry_oil.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pPanOil->getPosition()));
    }),
                                        MoveBy::create(0.5, Vec2(visibleSize.width,0)),
                                        
                                        NULL));
}

void FryPancakeScene::_doFry()
{
    AudioHelp::getInstance()->playLoopEffect("fry.mp3");
    m_pFryPan->setLocalZOrder(5);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/oil.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pPanOil->getPosition()));
    pParticle->setLocalZOrder(6);
    
    m_pFryPan->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                            MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
    
    m_pFryPan->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("make2/rolls"), 1, 5,false,true,2.5),
//                                          DelayTime::create(1),
                                          CallFunc::create([=](){
        m_pFryPan->setTexture(localPath("make2/rolls6"));
    }),
                                          AnimationHelp::createAnimate(localPath("make2/rolls"), 6, 7,false,true,2.5),
                                          CallFunc::create([=](){
        _showPlate();
        _showSpatula();
    }),
                                          DelayTime::create(2),
                                          CallFunc::create([=](){
        if(m_nFryCount == 0){
            m_pGuideLayer->showGuideMove(m_pSpatula->getPosition(), m_pFryPan->getPosition());
        }
    }), NULL));
}

void FryPancakeScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<FrySausageScene>();
    }), NULL));
}
