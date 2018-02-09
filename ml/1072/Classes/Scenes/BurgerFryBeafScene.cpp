
#include "BurgerFryBeafScene.h"
#include "SceneManager.h"

BurgerFryBeafScene::BurgerFryBeafScene()
{
    m_nAddOilCount = 0;
    m_nFryCount = 0;
    m_pCurBeaf = nullptr;
}

BurgerFryBeafScene::~BurgerFryBeafScene()
{
    
}
bool BurgerFryBeafScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_make1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showStove();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=]{
        AudioHelp::getInstance()->playEffect("vo_fry_beaf.mp3");
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=]{
        _showPan();
        _showOil();
    }), NULL));
    
    m_pAccelerListener = EventListenerAcceleration::create(CC_CALLBACK_2(BurgerFryBeafScene::onAccelerationCallback, this));
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pAccelerListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pAccelerListener, this);
    Device::setAccelerometerEnabled(false);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void BurgerFryBeafScene::onEnter()
{
    ExtensionScene::onEnter();
}

void BurgerFryBeafScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BurgerFryBeafScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name=="oil_spoon") {
        if (pDragNode->index==0){
            pDragNode->setDragSprite(Sprite::create(localPath("oil_spoon1_1.png")));
        }
        
    }
}

void BurgerFryBeafScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
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
                    pOil = Sprite::create(localPath("oil_spoon1_2.png"));
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
                }
                Sprite* pOilStream = Sprite::create(localPath("oil_spoon1_3.png"));
                pDragNode->addChild(pOilStream);
                pOilStream->setName("stream");
                pOilStream->runAction(AnimationHelp::createAnimate(localPath("oil_spoon1_"), 3, 4,false));
                
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
        if(pDragNode->index == 0){
            Rect rect = m_pBeafInOil->getBoundingBox();
            rect.origin = m_pBeafInOil->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index = 1;
                m_pBeafInOil->stopAllActions();
                m_pBeafInOil->setVisible(false);
                Sprite* pBeaf = Sprite::create(localPath("beef_patty6.png"));
                pBeaf->setPosition(pDragNode->convertToNodeSpace(worldPoint));
                pDragNode->addChild(pBeaf);
                pBeaf->setName("beaf");
                if (m_nFryCount==0) {
                    _showPlateLeft();
                }
                AudioHelp::getInstance()->stopFryEffect();
            }
        }else if (pDragNode->index==1){
            Rect rect = m_pPlateLeft->getBoundingBox();
            rect.origin = m_pPlateLeft->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index = 2;
                pDragNode->setTouchEnabled(false);
                Node* pNode = pDragNode->getChildByName("beaf");
                if (pNode) {
                    pNode->removeFromParent();
                }
                m_pBeafInOil->setVisible(false);
                
                Sprite* pBeaf = Sprite::create(localPath("beef_patty6.png"));
                this->addChildToContentLayer(pBeaf);
                pBeaf->setPosition(m_pPlateLeft->getPosition()+Vec2(60-40*m_nFryCount, 0));
                pBeaf->setScale(0.8);
                pBeaf->setLocalZOrder(5);
                m_nFryCount++;
                _removeShovel();
                
                _showBeafPlate();
                
                m_pCurBeaf = nullptr;
                if (m_nFryCount>=3) {
                    _finishAll();
                }
            }
            
        }
    }
}

void BurgerFryBeafScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    
    std::string name = pDragNode->getName();
    if (name=="beaf"){
        if(!m_pCurBeaf) {
            Rect rect = m_pPan->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())) {
                pDragNode->setTouchEnabled(false);
                m_pCurBeaf = pDragNode;
                
                auto iter = std::find(m_pBeafVector.begin(), m_pBeafVector.end(), pDragNode);
                if (iter != m_pBeafVector.end())
                {
                    m_pBeafVector.erase(iter);
                }
                
                m_pBeafInOil = Sprite::create(localPath("beef_patty0.png"));
                this->addChildToContentLayer(m_pBeafInOil);
                m_pBeafInOil->setPosition(m_pOil->getPosition());
                
                pDragNode->setVisible(false);
                pDragNode->setPosition(m_pOil->getPosition());
                pDragNode->setOrgPositionDefault();
                
                m_pBeafInOil->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                                               MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
                m_nFryStep = 0;
                _hideBeafPlate();
                AudioHelp::getInstance()->playEffect("vo_notburn.mp3");
                _doFry();
            }else{
                pDragNode->back(pDragNode->getTag());
            }
        }else{
            if (pDragNode->index ==1) {
            }else{
                pDragNode->back(pDragNode->getTag());
            }
        }
    }
}

void BurgerFryBeafScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void BurgerFryBeafScene::onAccelerationCallback(Acceleration *acc, Event * event)
{
    if (fabsf(acc->x)>0.8 || acc->y>0.4) {
        
        Device::setAccelerometerEnabled(false);
        log("====x,y,z %f,%f,%f",acc->x,acc->y,acc->z);
        m_pGuideLayer->removeGuide();
        m_nFryStep++;
        m_pBeafInOil->stopAllActions();
        m_pBeafInOil->runAction(Sequence::create(Spawn::create(JumpBy::create(1, Vec2::ZERO, 100, 1),
                                                               Sequence::create(RotateBy::create(0.5, Vec3(90, 0, 0)),
                                                                             CallFunc::create([=](){
                                                                   if (m_nFryStep<=1){
                                                                       m_pBeafInOil->setTexture(localPath("beef_patty0.png"));
                                                                   }
                                                               }),
                                                                             RotateBy::create(0.5, Vec3(90, 0, 0)), NULL), NULL),
                                                 CallFunc::create([=](){
            m_pBeafInOil->setPosition(m_pOil->getPosition());
            m_pBeafInOil->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                                           MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
            if (m_nFryStep==2) {
                _finishFry();
            }else if(m_nFryStep<2){
                _doFry();
            }
        }), NULL));
    }
}
#pragma mark - initData
void BurgerFryBeafScene::_initData()
{
    setExPath("content/make/burger/4/");
}

void BurgerFryBeafScene::_showStove()
{
    m_pStove = Sprite::create("content/make/common/stove/stove.png");
    CMVisibleRect::setPosition(m_pStove, 320, 300);
    this->addChildToContentLayer(m_pStove);
    
    m_pFire = Sprite::create("content/make/common/stove/stove_1.png");
    CMVisibleRect::setPosition(m_pFire, 320, 300);
    this->addChildToContentLayer(m_pFire);
    
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/common/stove/stove_", 1, 2)));
}

void BurgerFryBeafScene::_showPan()
{
    m_pPan = Sprite::create(localPath("pan.png"));
    CMVisibleRect::setPosition(m_pPan, 200-visibleSize.width, 300);
    this->addChildToContentLayer(m_pPan);
    m_pPan->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    m_pOil = Sprite::create(localPath("pan_oil.png"));
    CMVisibleRect::setPosition(m_pOil, 310, 330);
    this->addChildToContentLayer(m_pOil);
    m_pOil->setOpacity(0);
        
}

void BurgerFryBeafScene::_showOil()
{
    AudioHelp::getInstance()->playEffect("vo_pour_oil.mp3");
    m_pOilBowl = Sprite::create(localPath("oilve oil.png"));
    CMVisibleRect::setPosition(m_pOilBowl, 320+visibleSize.width, 820);
    this->addChildToContentLayer(m_pOilBowl);
    m_pOilBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pOilSpoon = _createDrageNode(localPath("oil_spoon_1.png"));
    CMVisibleRect::setPosition(m_pOilSpoon, 320+visibleSize.width, 800);
    this->addChildToContentLayer(m_pOilSpoon);
    m_pOilSpoon->setName("oil_spoon");
    
    m_pOilSpoon->setTouchEnabled(false);
    m_pOilSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pOilSpoon->setOrgPositionDefault();
        m_pOilSpoon->setTouchEnabled(true);
    }), NULL));
}
void BurgerFryBeafScene::_showBeaf()
{
    AudioHelp::getInstance()->playEffect("vo_drag_dry_patty.mp3");
    m_pPlateRight = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPosition(m_pPlateRight, 450+visibleSize.width, 770);
    m_pPlateRight->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    for (int i = 0; i<3; i++) {
        DragNode* pNode = _createDrageNode(localPath("beef_patty.png"));
        this->addChildToContentLayer(pNode);
        CMVisibleRect::setPosition(pNode, 520-45*i+visibleSize.width, 770);
        pNode->setTouchEnabled(false);
        pNode->setTag(i);
        pNode->setLocalZOrder(i);
        pNode->setName("beaf");
        pNode->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
            pNode->setOrgPositionDefault();
            pNode->setTouchEnabled(true);
        }), NULL));
        m_pBeafVector.push_back(pNode);
    }
    
    
}

void BurgerFryBeafScene::_showPlateLeft()
{
    m_pPlateLeft = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlateLeft);
    CMVisibleRect::setPosition(m_pPlateLeft, 190-visibleSize.width, 770);
    m_pPlateLeft->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
}

void BurgerFryBeafScene::_showBeafPlate()
{
    if (m_pBeafVector.size()<=0) {
        return;
    }
    for_each(m_pBeafVector.begin(), m_pBeafVector.end(), [=](DragNode* pNode){
        pNode->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
            pNode->setTouchEnabled(true);
        }), NULL));
    });
    m_pPlateRight->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
}

void BurgerFryBeafScene::_hideBeafPlate()
{
    for_each(m_pBeafVector.begin(), m_pBeafVector.end(), [=](DragNode* pNode){
        pNode->setTouchEnabled(false);
        pNode->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    });
    m_pPlateRight->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
}

void BurgerFryBeafScene::_showShovel()
{
    m_pShovel = _createDrageNode(localPath("shovel.png"));
    this->addChildToContentLayer(m_pShovel);
    CMVisibleRect::setPositionAdapted(m_pShovel, 500+visibleSize.width, 350);
    m_pShovel->setTouchEnabled(false);
    m_pShovel->setName("shovel");
    m_pShovel->setLocalZOrder(20);
    m_pShovel->setActionPoint(Vec2(82, 82));
    
    m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pShovel->setOrgPositionDefault();
        m_pShovel->setTouchEnabled(true);
    }), NULL));
}

void BurgerFryBeafScene::_removeShovel()
{
    m_pShovel->setTouchEnabled(false);
    m_pShovel->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pShovel->removeFromParent();
    }), NULL));
}

void BurgerFryBeafScene::_doFry()
{
    AudioHelp::getInstance()->playFryEffect();
    m_pBeafInOil->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("beef_patty"), 0, 6,false,true,2),
                                             CallFunc::create([=]{
        if(m_nFryStep<1){
            m_pGuideLayer->showGuideShake(m_pBeafInOil->getPosition()+Vec2(0,100));
            Device::setAccelerometerEnabled(true);
        }else{
            _finishFry();
        }
    }), NULL));
//    this->runAction(Sequence::create(DelayTime::create(5),
//                                     CallFunc::create([=](){
//        m_pGuideLayer->showGuideShake(m_pBeafInOil->getPosition());
//        Device::setAccelerometerEnabled(true);
//    }), NULL));
}

void BurgerFryBeafScene::_finishAddOil()
{
    m_pOilSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pOilBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    _showBeaf();
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fry_oil.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(m_pOil->getPosition());
}

void BurgerFryBeafScene::_finishFry()
{
    m_nFryStep = 0;
    m_pCurBeaf->setDragSprite(Sprite::create(localPath("beef_patty6.png")));
//    m_pCurBeaf->setVisible(true);
    
    
    _showShovel();
//    _showPlateLeft();
}

void BurgerFryBeafScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<BurgerDecorateScene>();
    }), NULL));
}
