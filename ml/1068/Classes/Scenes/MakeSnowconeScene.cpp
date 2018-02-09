
#include "MakeSnowconeScene.h"
#include "SceneManager.h"

static Vec2 gWaterPos[][6] ={
    {
        Vec2(108, 272),
        Vec2(266, 272),
        Vec2(102, 145),
        Vec2(273, 145),
    },
    {
        Vec2(91, 311),
        Vec2(264, 311),
        Vec2(438, 311),
        Vec2(173, 184),
        Vec2(356, 184)
    },
    {
        Vec2(119, 278),
        Vec2(299, 278),
        Vec2(116, 129),
        Vec2(302, 129),
    },
};

static Vec3 gHSB[10] = {
    Vec3(0, 0, 0),
    Vec3(-80, -10, 0),
    Vec3(59, 28, 25),
    Vec3(-10, 0, 0),
    Vec3(51, 100, 73),
    Vec3(0, 0, 0),
    Vec3(-4, 52, 37),
    Vec3(53, 61, 51),
    Vec3(35, 35, 12),
    Vec3(97, -11, 11),
};
MakeSnowconeScene::MakeSnowconeScene()
{
    m_pTray = nullptr;
    m_pWaterBottle = nullptr;
    m_pSnowConeNode = nullptr;
    m_pCone = nullptr;
    m_pConeFront = nullptr;
    m_pConeIce = nullptr;
    m_pSpoon = nullptr;
    m_pSnowSCribble = nullptr;
    m_pColorSCribble = nullptr;
    m_pBrush = nullptr;
    m_pBottleList = nullptr;
    m_pScrollView = nullptr;
    
    m_nShapeIndex = 0;
    m_nUpCount = 0;
    m_nDownCount = 0;
    m_nPourCount = 0;
    m_bSpoonFull = false;
    m_nSnowPutCount = 0;
    m_nKnifeCount = 0;
    m_bFreezed = false;
    m_bResetShown = false;
}

MakeSnowconeScene::~MakeSnowconeScene()
{
    CC_SAFE_RELEASE(m_pBrush);
}

bool MakeSnowconeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    Sprite* pBg = Sprite::create("content/make/bg/bg.png");
    this->addChildToBGLayer(pBg);
    pBg->setPosition(visibleSize*0.5);
    
    Sprite *pTable = Sprite::create("content/make/snowcone/table1.png");
    pTable->setOpacity(0);
    CMVisibleRect::setPositionAdapted(pTable, 320, 180);
    this->addChildToContentLayer(pTable);
    
//    Sprite *pTableCloth = Sprite::create(localPath("ice/table_cloth.png"));
//    CMVisibleRect::setPositionAdapted(pTableCloth, 320-visibleSize.width, 20+400);
//    this->addChildToContentLayer(pTableCloth);
    
    pTable->runAction(Sequence::create(DelayTime::create(1),
                                       EaseBounceOut::create(FadeIn::create(1)),
                                       CallFunc::create([=]()
                                                        {
//                                                            pTableCloth->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, -400)));
                                                        }), NULL));
    _showIconIce();
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/choose_shape.png");
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    return true;
}

void MakeSnowconeScene::onEnter()
{
    ExtensionScene::onEnter();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_MAKE_SNOWCONE);
//    Scene* scene = (Scene*)this->getParent();
//    
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
//    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
}

void MakeSnowconeScene::onExit()
{
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_MAKE_SNOWCONE);
    
    ExtensionScene::onExit();
}
void MakeSnowconeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(2)){
        if (m_pBottleList) {
            Vector<Node*> Children = m_pBottleList->getInnerContainer()->getChildren();
            for_each(Children.begin(), Children.end(), [=](Node* pBall){
                Node* pLock = pBall->getChildByName("lock");
                if (pLock) {
                    pLock->setVisible(false);
                }
            });
        }
        
        if (m_pScrollView){
            Vector<Node*> ChildrenShape = m_pScrollView->getInnerContainer()->getChildren();
            for_each(ChildrenShape.begin(), ChildrenShape.end(), [=](Node* pShape){
                LockItem* pItem = dynamic_cast<LockItem*>(pShape);
                if (pItem) {
                    pItem->unlock();
                }
            });
        }
    }
}
void MakeSnowconeScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        
        
        Image* pImage = getResultRender()->newImage();
        std::stringstream ostr;
        
        bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ostr<<"snow.png";
#else
        ostr<<"/snow.png";
#endif
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+ostr.str(), false);
        pImage->autorelease();
        GameDataManager::getInstance()->m_bColored = true;
        log("===save success %d==",issuccess);
        SceneManager::replaceTheScene<DecorateScene>();
        return;
    }else if (btn->getTag()==GameUILayoutLayer::eUIButtonTagReset) {
        btn->setEnabled(false);
        _resetColor();
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
    }
    ExtensionScene::onButtonCallback(btn);
}


RenderTexture* MakeSnowconeScene::getResultRender()
{
    Size size = m_pConeIce->getContentSize();
    RenderTexture* render = RenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    Vec2 pos = m_pConeIce->getPosition();
    m_pConeIce->setPosition(Vec2(size.width/2, size.height/2));
    m_pColorSCribble->setPosition(Vec2(size.width/2, size.height/2));
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pConeIce->visit();
    m_pColorSCribble->visit();
    render->end();
    m_pConeIce->setPosition(pos);
    m_pColorSCribble->setPosition(pos);
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;
}
void MakeSnowconeScene::onItemClicked(LockItem* item)
{
    if (!m_bShapTouchEnable) {
        return;
    }
    AudioHelp::getInstance()->playSelectedEffect();
    int tag = item->getTag();
    m_nShapeIndex = tag;
    m_nUpCount = 2;
    m_nDownCount =2;
    if (tag==1) {
        m_nUpCount = 3;
        m_nDownCount =2;
    }
    
    _removeIconIce(tag);
    _showTray();
}

void MakeSnowconeScene::onShapeItemClicked(LockItem* item)
{
    if (!m_bShapTouchEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    if(item->isLocked()){
        ShopLayer* pLayer = ShopLayer::create();
        this->addChildToUILayer(pLayer);
        pLayer->setLocalZOrder(100);
        pLayer->showBannerDismiss(true);
        return;
    }
    AudioHelp::getInstance()->playSelectedEffect();
    int tag = item->getTag();
    m_nShapeIndex = tag;
    GameDataManager::getInstance()->m_nShapeIndex = m_nShapeIndex;
    std::stringstream ostr;
    ostr<<"shape/shape"<<tag+1<<".png";
    
    _removeIconShape(tag);
    m_pKnife = _createDrageNode(localPath("shape/cutter.png"));
    this->addChildToContentLayer(m_pKnife);
    m_pKnife->setLimitRect(Rect(0, 90, visibleSize.width+m_pKnife->getContentSize().width*0.3, visibleSize.height));
    m_pKnife->setLocalZOrder(10);
    CMVisibleRect::setPosition(m_pKnife, 320+visibleSize.width, 450);
    m_pKnife->setTouchEnabled(false);
    m_pKnife->setActionPoint(Vec2(30, 130));
    m_pKnife->runAction(Sequence::create(DelayTime::create(3.5),
                                         MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pKnife->setOrgPositionDefault();
        m_pKnife->setTouchEnabled(true);
    }), NULL));
    
    Sprite* pShape = Sprite::create(localPath(ostr.str()));
    MyRenderTexture* pTempRT = MyRenderTexture::create(pShape->getContentSize().width, pShape->getContentSize().height);
    pShape->setPosition(pShape->getContentSize()*0.5);
    pTempRT->begin();
    pShape->visit();
    pTempRT->end();
    Director::getInstance()->getRenderer()->render();
    m_nShapePercent = 100-pTempRT->getPercentageTransparent();
    
    m_pSnowSCribble->useTarget(localPath(ostr.str()));
    _showCone(Vec2(-50, 100));
}

void MakeSnowconeScene::onFreezerCallback(FreezerNode *)
{
    m_pGuideLayer->removeGuide();
    if (m_pFreezer->getIsOpen()) {
        if (!m_bFreezed) {
            m_pFreezer->setTouchEnable(false);
            m_pTray->setTouchEnabled(true);
            Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
            pos = m_pGuideLayer->convertToNodeSpace(pos);
            m_pGuideLayer->showGuideMove(m_pTray->getPosition(), pos);
        }else{
            m_pFreezer->setTouchEnable(false);
            _showShelf(50);
            m_pTray->runAction(Sequence::create(DelayTime::create(0.5),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pTray->setVisible(true);
                                                                     m_pFreezer->runAction(Sequence::create(DelayTime::create(0.5),
                                                                                                            MoveBy::create(0.5, Vec2(visibleSize.width*1.5, 200)), NULL));
                                                                     m_pTrayInFreezer->setVisible(false);
                                                                 }),
                                                Spawn::create(ScaleTo::create(0.5, 1.1),
                                                              MoveBy::create(0.5, Vec2(0, -50)), NULL),
                                                DelayTime::create(0.3),
                                                Spawn::create(ScaleTo::create(0.3, 0.9,1.1),
                                                              JumpBy::create(0.5, Vec2::ZERO, 50, 1), NULL),
                                                ScaleTo::create(0.3, 1),
                                                DelayTime::create(1),
                                                CallFunc::create([=]()
                                                                 {
                                                                     _showMachine();
                                                                 }),
                                                Spawn::create(MoveTo::create(0.6, CMVisibleRect::getPosition(526, 750)),
                                                              ScaleTo::create(1, 0.4), NULL),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_nStep = MakeSnowconeScene::eStepCrushIce;
                                                                     //show guide open machine
                                                                     m_pGuideLayer->showGuideArrow(Vec2(m_pMachine->getCapWorldRect().getMidX(), m_pMachine->getCapWorldRect().getMidY()),-1);
//                                                                     m_pGuideLayer->showGuideMove(Vec2(m_pMachine->getCapWorldRect().getMaxX(), m_pMachine->getCapWorldRect().getMidY()), Vec2(m_pMachine->getCapWorldRect().getMinX(), m_pMachine->getCapWorldRect().getMidY()));
                                                                     m_pMachine->setTouchEnable(true);
                                                                 }),
                                                NULL));
        }
    }else{
    }
}

void MakeSnowconeScene::onMachineStateCallback(int state)
{
    switch (state) {
        case MachineNode::eStateCapOpen:
        {
            m_pMachine->setTouchEnable(false);
            m_pGuideLayer->removeGuide();
            m_pTray->setMovable(false);
            m_pTray->setTouchEnabled(true);
            m_pGuideLayer->showGuideTap(m_pTray->getPosition());
            m_pMachine->setButtonEnable(false);
        }
            break;
        case MachineNode::eStateCapClose:
        {
            m_pMachine->setTouchEnable(false);
            m_pGuideLayer->removeGuide();
            m_pMachine->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 100)),
                                                   CallFunc::create([=](){                
                m_pGuideLayer->showGuideTap(Vec2(m_pMachine->getPositionX(), m_pMachine->getPositionY()+50));
                m_pMachine->setButtonEnable(true);
            }), NULL));
        }
            break;
        case MachineNode::eStateTurnOn:
        {
            m_pGuideLayer->removeGuide();
            m_pMachine->setButtonEnable(false);
        }
            break;
        case MachineNode::eStateTurnOff:
        {
            m_pGuideLayer->removeGuide();
            m_pMachine->setButtonEnable(false);
        }
            break;
        case MachineNode::eStateFinish:
        {
            m_pGuideLayer->removeGuide();
            m_pMachine->setButtonEnable(false);
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                m_pMachine->runAction(ScaleTo::create(0.5, 1.2));
                m_pMachine->separateBox();                
            }),
                                             DelayTime::create(1.8),
                                             CallFunc::create([=](){
                m_pMachine->runAction(Spawn::create(JumpTo::create(1, CMVisibleRect::getPosition(320, 1000), 400, 1),
                                                    ScaleTo::create(0.8, 1), NULL));
                _showShelf();
                _showCone();
            }), NULL));
        }
            break;
            
        default:
            break;
    }
}
void MakeSnowconeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    _hideShadow(pDragNode);
    pDragNode->setLocalZOrder(10);
    if (pDragNode==m_pKnife) {
        m_pSnowSCribble->paint(worldPoint);
    }else if ("bottle"==pDragNode->getName()){
        m_pGuideLayer->removeGuide();
        CCImageColorSpace cs;
        Sprite* pColor;
        std::stringstream ostr;
        m_nFlavorIndex = pDragNode->getTag();
        if (m_nFlavorIndex==0) {
            ostr<<"color/color"<<m_nShapeIndex+1<<"_1.png";
            pColor = Sprite::create(localPath(ostr.str()));
        }else if (m_nFlavorIndex==5) {
            ostr<<"color/color"<<m_nShapeIndex+1<<"_6.png";
            pColor = Sprite::create(localPath(ostr.str()));
        }else{
            ostr<<"color/color"<<m_nShapeIndex+1<<"_1.png";
            Vec3 hsb = gHSB[m_nFlavorIndex];
            Texture2D* texture = cs.textureWithHSB(localPath(ostr.str()), hsb.x, hsb.y, hsb.z);
            pColor = Sprite::createWithTexture(texture);
        }
        pColor->setPosition(pColor->getContentSize()*0.5);
        m_pColorSCribble->useTarget(pColor);
    }
}

void MakeSnowconeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pSpoon){
#pragma mark ======spoon 舀冰 =============
        if (!m_bSpoonFull) {
            Rect rect = m_pMachine->getBoxWorldRect();
            if (rect.containsPoint(worldPoint)) {
                m_bSpoonFull = true;
                m_pSpoon->setTouchEnabled(false);
                Sprite* pSpoonIn = Sprite::create(localPath("crush/spoon0.png"));
                m_pSpoon->setVisible(false);
                AudioHelp::getInstance()->playEffect("scoopup_ice.mp3");
                m_pMachine->getIce(pSpoonIn, [=](){
                    m_pSpoon->setPosition(m_pSpoon->getParent()->convertToNodeSpace(pSpoonIn->getParent()->convertToWorldSpace(pSpoonIn->getPosition())));
                    m_pSpoon->getDragSprite()->setTexture(localPath("crush/spoon1.png"));
                    if (m_nSnowPutCount==0) {
                        m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2, visibleSize.height*0.8), Vec2(visibleSize.width/2, visibleSize.height*0.4));
                    }
                    m_pMachine->setIcePercent((4-m_nSnowPutCount)*20);
                    m_pSpoon->setTouchEnabled(true);
                    m_pSpoon->setVisible(true);
                    pSpoonIn->removeFromParent();
                });
            }
        }else{
#pragma mark ======spoon 倒冰 =============
            Rect rect = m_pCone->getBoundingBox();
            rect.origin.y += 370;
            rect.size.height = 400;
            rect.origin = m_pCone->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                m_pGuideLayer->removeGuide();
                m_pSpoon->setTouchEnabled(false);
                m_bSpoonFull = false;
                m_pSpoon->setPosition(Vec2(m_pSnowConeNode->getPositionX()+100, m_pSnowConeNode->getPositionY()+m_pCone->getContentSize().height*0.8+50*m_nSnowPutCount));
                m_pSpoon->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                     CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("pour_ice_cup.mp3");
                    
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/ice2.plist");
                    
                    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                    this->addChildToContentLayer(pParticle);
                    pParticle->setLocalZOrder(10);
                }),
                                                     DelayTime::create(1),
                                                     CallFunc::create([=](){
                    if (m_nSnowPutCount==0) {
                        m_pConeIce->setVisible(true);
                    }else{
                        std::stringstream ostr;
                        ostr<<"crush/ice0_"<<m_nSnowPutCount<<".png";
                        m_pConeIce->setTexture(localPath(ostr.str()));
                    }
                    m_pSpoon->getDragSprite()->setTexture(localPath("crush/spoon0.png"));
                    m_nSnowPutCount++;
                    if (m_nSnowPutCount==4) {
                        _moveawayShelf();
                        m_pMachine->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
                        
                        m_pSpoon->runAction(Sequence::create(DelayTime::create(0.5),
                                                             MoveBy::create(0.5, Vec2(visibleSize.width, 100)),
                                                             CallFunc::create([=](){
                            _showIconShape();
                            _moveawayCone();
                            m_pGuideLayer->removeGuide();
                            
                        }), NULL));
                    }else{
                        m_pSpoon->setTouchEnabled(true);
                    }
                    
                }),
                                                     RotateTo::create(0.3, 1), NULL));
            }
        }
    }else if (pDragNode==m_pKnife) {
#pragma mark ======Knife 塑形 =============
        Vec2 prePoint = Vec2(pDragNode->getOffset(), worldPoint);
        m_pSnowSCribble->paint(prePoint, worldPoint);
        ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("shaveice");
        if (!pParticle) {
            pParticle = ParticleSystemQuad::create("particle/shaveice.plist");
            pParticle->setName("shaveice");
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(9);
        }
        pParticle ->setPosition(worldPoint);
        if (++m_nKnifeCount%10==0) {
            int percent = 100-m_pSnowSCribble->getCanvas()->getPercentageTransparent();
            if (100.0*percent/m_nShapePercent>90) {
                log("++++++shape finish");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                this->addChildToUILayer(pParticle);
                pParticle->setLocalZOrder(10);
                pParticle->setPosition(visibleSize*0.5);
                
                std::stringstream ostr;
                ostr<<"shape/shape"<<m_nShapeIndex+1<<".png";
                m_pConeIce->setOpacity(255);
                m_pConeIce->setTexture(localPath(ostr.str()));
                m_pSnowSCribble->setVisible(false);
//                m_pGameUI->showNextLayout();
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                      CallFunc::create([=](){
                    _showShelf(0,false);
                    _showColorBottle();
                    
                    m_pSnowConeNode->runAction(Sequence::create(DelayTime::create(0.5),
                                                                MoveBy::create(0.8, Vec2(50, -100)), NULL));
                }), NULL));
                Node* pShape = m_pUILayer->getChildByName("shape");
                if (pShape) {
                    pShape->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, 300))),
                                                       CallFunc::create([=](){
                        pShape->removeFromParent();
                    }), NULL));
                }
                ParticleSystemQuad* pParticle2 = (ParticleSystemQuad*)m_pContentLayer->getChildByName("shaveice");
                if (pParticle2) {
                    pParticle2->stopSystem();
                    pParticle2->removeFromParent();
                }
            }else{
//                m_pConeIce->setOpacity(255*(1.0-percent/m_nShapePercent));
                m_pConeIce->setOpacity(255-m_nKnifeCount/5);
            }
        }
    }else if ("bottle"==pDragNode->getName()) {
#pragma mark ======bottle 涂色 =============
        int tag = pDragNode->getTag();
        if (pDragNode->isMoving()) {
            if (!IAPManager::getInstance()->getItemIsBought(2) && !gNoneIap && tag>=5){
                pDragNode->setTouchEnabled(false);
                ShopLayer* pLayer = ShopLayer::create();
                this->addChildToUILayer(pLayer);
                pLayer->setLocalZOrder(100);
                pLayer->showBannerDismiss();
                pDragNode->back();
                m_pBottleList->setTouchEnabled(false);
                pLayer->shopDismissed = [=](){
                    pDragNode->setTouchEnabled(true);
                    m_pBottleList->setTouchEnabled(true);
                };
                return;
            }
            m_pBottleList->setTouchEnabled(false);
            if (pDragNode->getRotation()>-0.1) {
                if (pDragNode->getPositionY()<visibleSize.height*0.5) {
                    pDragNode->setRotation(-0.1);
                    pDragNode->runAction(Sequence::create(RotateBy::create(0.3, -150),
                                                         CallFunc::create([=]()
                                                                          {
                                                                              int tag = pDragNode->getTag();
                                                                              std::stringstream ostr;
                                                                              ostr<<"color/bottle/"<<tag+1<<".png";
                                                                              pDragNode->getDragSprite()->setTexture(localPath(ostr.str()));
                                                                              
                                                                          }), NULL));
                    schedule(schedule_selector(MakeSnowconeScene::_playEffectYummy), 5);
                    m_pGameUI->showNextLayout();
                }
            }else{
                Rect rect = m_pConeIce->getBoundingBox();
                rect.origin = m_pConeIce->getParent()->convertToWorldSpace(rect.origin);
                if (rect.containsPoint(worldPoint)) {
                    Vec2 pre = Vec2(pDragNode->getOffset(), worldPoint);
                    m_pColorSCribble->paint(pre,worldPoint);
                    Director::getInstance()->getRenderer()->render();
                    if(!m_bResetShown){
                        m_bResetShown = true;
                        m_pGameUI->showResetLayout();
                    }
                }
            }
        }
    }
    
}

void MakeSnowconeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("shaveice");
    if (pParticle) {
        pParticle->stopSystem();
        pParticle->removeFromParent();
    }
    if (pDragNode==m_pWaterBottle) {
        Rect rect = m_pTray->getDragSprite()->getBoundingBox();
        rect.origin = m_pTray->convertToWorldSpace(rect.origin);
        Vec2 point = pDragNode->getPosition();
        point= pDragNode->getParent()->convertToWorldSpace(point);
        if (rect.containsPoint(point)) {
            _pourWater();
        }
    }else if (pDragNode==m_pTray) {
        if (m_nStep==MakeSnowconeScene::eStepMakeIce) {
            _putTrayInFreezer();
        }
        
    }else if ("bottle"==pDragNode->getName()) {
        unschedule(schedule_selector(MakeSnowconeScene::_playEffectYummy));
        pDragNode->setTouchEnabled(false);
        m_pBottleList->setTouchEnabled(true);
        pDragNode->runAction(Sequence::create(RotateTo::create(0.3, 0),
                                             CallFunc::create([=]()
                                                              {
                                                                  int tag = pDragNode->getTag();
                                                                  std::stringstream ostr;
                                                                  ostr<<"content/make/snowcone/color/bottle/icon/"<<tag+1<<".png";
                                                                  pDragNode->getDragSprite()->setTexture(ostr.str());
                                                              }), NULL));
        pDragNode->back(0,[=](){
            pDragNode->setTouchEnabled(true);
        });

    }
    
}

void MakeSnowconeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    _showShadow(pDragNode);
    ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("shaveice");
    if (pParticle) {
        pParticle->stopSystem();
        pParticle->removeFromParent();
    }
    if (pDragNode==m_pTray) {
        if(m_nStep==MakeSnowconeScene::eStepCrushIce){
            m_pTray->setTouchEnabled(false);
            m_pMachine->setTouchEnable(false);
            pDragNode->setLocalZOrder(0);
            _moveawayShelf();
            m_pTray->runAction(Sequence::create(RotateBy::create(0.3, -50),
                                                CallFunc::create([=](){
                _addIceMachine();
            }), NULL));
        }
        
    }
}

#pragma mark - initData
void MakeSnowconeScene::_initData()
{
    
    setExPath("content/make/snowcone/");
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecorate;
    m_nStep = MakeSnowconeScene::eStepMakeIce;
}

void MakeSnowconeScene::_showShadow(DragNode* pDragNode)
{
    if (pDragNode==m_pWaterBottle) {
        pDragNode->getDragSprite()->setTexture(localPath("ice/kettle0.png"));
    }
}

void MakeSnowconeScene::_hideShadow(DragNode* pDragNode)
{
    if (pDragNode==m_pWaterBottle) {
        pDragNode->getDragSprite()->setTexture(localPath("ice/kettle1.png"));
    }
    
}
void MakeSnowconeScene::_showIconIce()
{
    int count = 3;
    for (int i = 0; i<count; i++) {
        std::stringstream ostr;
        ostr<<"ice/icon_ice"<<i<<".png";
        LockItem* pItem = LockItem::create();
        if ((i<6 && i>2 && GameDataManager::getInstance()->getShapeLocked(i))) {
            pItem->initWithName(localPath(ostr.str()), "",true);
        }else{
            pItem->initWithName(localPath(ostr.str()), "",false);
        }
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(MakeSnowconeScene::onItemClicked, this);
        CMVisibleRect::setPositionAdapted(pItem, 120+180*i, 805+visibleSize.height/2,kBorderNone,kBorderTop);
        this->addChildToUILayer(pItem);
        m_IconIces.pushBack(pItem);
        float dt = 0.2+i*0.4;
        pItem->runAction(Sequence::create(DelayTime::create(dt),
                                          EaseBackOut::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height/2))),
                                          CallFunc::create([=]()
                                                           {
                                                               if (i==count-1) {
                                                                   m_bShapTouchEnable = true;
                                                               }
                                                           }), NULL));

    }
    m_bShapTouchEnable = false;
    
    
}

void MakeSnowconeScene::_removeIconIce(int index)
{
    m_bShapTouchEnable = false;
    for_each(m_IconIces.begin(), m_IconIces.end(), [=,&index](LockItem* pItem){
        pItem->stopAllActions();
        int tag = pItem->getTag();
        if (index==tag) {
            pItem->runAction(Sequence::create(Spawn::create(EaseBackOut::create(ScaleTo::create(0.5, 1.2)),
                                                            JumpBy::create(0.4, Vec2::ZERO, 20, 1), NULL),
                                              Spawn::create(EaseBackOut::create(ScaleTo::create(0.5, 1)),
                                                            JumpBy::create(0.4, Vec2::ZERO, 20, 1), NULL),
                                              EaseBackIn::create(MoveBy::create(0.5, Vec2(0, visibleSize.height/2))), NULL));
        }else{
            pItem->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, visibleSize.height/2))));
        }
    });
}

void MakeSnowconeScene::_showIconShape()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width, 200));
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    this->addChildToContentLayer(m_pScrollView);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setClippingEnabled(false);
    m_pScrollView->setPosition(Vec2(0, -500));
    m_pScrollView->runAction(Sequence::create(MoveTo::create(1, Vec2(0, visibleSize.height/2)),
                                              DelayTime::create(0.5),
                                              CallFunc::create([=](){
        m_pScrollView->scrollToRight(2, true);
    }),
                                              DelayTime::create(2),
                                              CallFunc::create([=](){
        m_pScrollView->scrollToLeft(1, true);
        m_bShapTouchEnable = true;
    }), NULL));
    
    int count = 7;
    for (int i = 0; i<count; i++) {
        std::stringstream ostr;
        ostr<<"shape/icon/icon_shape"<<i+1<<".png";
        LockItem* pItem = LockItem::create();
        if (i>2 && !IAPManager::getInstance()->getItemIsBought(2) && !gNoneIap) {
            pItem->initWithName(localPath(ostr.str()), "content/common/lock.png",true);
        }else{
            pItem->initWithName(localPath(ostr.str()), "",false);
        }
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(MakeSnowconeScene::onShapeItemClicked, this);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(120+180*i, 100);
        
    }
    m_pScrollView->getInnerContainer()->setContentSize(Size(120+180*count, m_pScrollView->getContentSize().height));
    m_bShapTouchEnable = false;
    m_pScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                              CallFunc::create([=](){
        m_pScrollView->scrollToRight(2, true);
    }),
                                              DelayTime::create(2),
                                              CallFunc::create([=](){
        m_pScrollView->scrollToLeft(1, true);
        m_bShapTouchEnable = true;
    }), NULL));
    
}
void MakeSnowconeScene::_removeIconShape(int index)
{
    m_bShapTouchEnable = false;
    m_pScrollView->setTouchEnabled(false);
    Vector<Node*> pChildren = m_pScrollView->getInnerContainer()->getChildren();
    for_each(pChildren.begin(), pChildren.end(), [=,&index](Node* pNode){
        LockItem* pItem = (LockItem*)pNode;
        pItem->setItemTouchEnable(false);
        pItem->stopAllActions();
        int tag = pItem->getTag();
        if (index==tag) {
            pItem->runAction(Sequence::create(Spawn::create(EaseBackOut::create(ScaleTo::create(0.5, 1.2)),
                                                            JumpBy::create(0.4, Vec2::ZERO, 20, 1), NULL),
                                              Spawn::create(EaseBackOut::create(ScaleTo::create(0.5, 1)),
                                                            JumpBy::create(0.4, Vec2::ZERO, 20, 1), NULL),
                                              CallFunc::create([=](){
                
                pItem->setVisible(false);
                std::stringstream ostr;
                ostr<<"shape/icon/icon_shape"<<index+1<<".png";
                Sprite* pSprite = Sprite::create(localPath(ostr.str()));
                this->addChildToUILayer(pSprite);
                pSprite->setName("shape");
                pSprite->setPosition(m_pUILayer->convertToNodeSpace(pItem->getParent()->convertToWorldSpace(pItem->getPosition())));
                pSprite->runAction(MoveTo::create(0.5, CMVisibleRect::getPosition(100,100,kBorderLeft,kBorderTop)));
                
                m_pScrollView->getInnerContainer()->removeAllChildren();
            }), NULL));
        }else{
            pItem->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, visibleSize.height/2))));
        }
    });
    
}

void MakeSnowconeScene::_showColorBottle()
{
    m_pBottleList = ExpandScrollView::create();
    m_pBottleList->setContentSize(Size(visibleSize.width, 200));
    CMVisibleRect::setPositionAdapted(m_pBottleList, visibleSize.width, 600+100,kBorderNone,kBorderTop);
    this->addChildToUILayer(m_pBottleList);
    m_pBottleList->setClippingEnabled(false);
    m_pBottleList->setTouchEnabled(false);
    m_pBottleList->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pBottleList->setScrollBarEnabled(false);
    
    float delt = 0.1;
    m_pBottleList->runAction(Sequence::create(DelayTime::create(1),
                                              MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                              DelayTime::create(delt),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pBottleList->scrollToRight(3, true);
                                                               }),
                                              DelayTime::create(3),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pBottleList->scrollToLeft(1, true);
                                                               }),
                                              DelayTime::create(1),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2, visibleSize.height*0.8), visibleSize*0.5);
                                                                   m_pBottleList->setTouchEnabled(true);
                                                               }),
                                              NULL));
    
    int count = 10;
    for (int i = 0; i<10; i++) {
        std::stringstream ostr;
        ostr<<"content/make/snowcone/color/bottle/icon/"<<i+1<<".png";
        DragNode* pBottle = _createDrageNode(ostr.str());
        pBottle->setPosition(Vec2(50+150*i, 100));
        pBottle->setTag(i);
        pBottle->setCanSwallow(false);
        pBottle->setDirection(DragNode::Direction::VERTICAL);
        pBottle->setName("bottle");
        pBottle->setActionPoint(Vec2(45, 347));
        pBottle->setOrgPositionDefault();
        m_pBottleList->getInnerContainer()->addChild(pBottle);
        pBottle->setScale(0.8);
        
        pBottle->setVisible(false);
        if (!IAPManager::getInstance()->getItemIsBought(2) && !gNoneIap && i>=5) {
            Sprite* pLock = Sprite::create("content/common/lock.png");
            pBottle->addChild(pLock,5);
            pLock->setPosition(Vec2(30, -140));
            pLock->setName("lock");
            
            pLock->setVisible(false);
        }
        pBottle->runAction(Sequence::create(DelayTime::create(0.2*i+1),
                                            CallFunc::create([=]()
                                                             {
                                                                 pBottle->setPosition(pBottle->getParent()->convertToNodeSpace(Vec2(visibleSize.width*0.5, visibleSize.height*0.3)));
                                                                 pBottle->setVisible(true);
                                                                 pBottle->setScale(0.5);
                                                                 AudioHelp::getInstance()->playEffect("syrup_up.mp3");
                                                             }),
                                            Spawn::create(MoveTo::create(0.5, pBottle->getOrgPosition()),
                                                          ScaleTo::create(0.5, 0.8), NULL),
                                            CallFunc::create([=]()
                                                             {
                                                                 Node* pLock = pBottle->getChildByName("lock");
                                                                 if (pLock) {
                                                                     pLock->setVisible(true);
                                                                 }
                                                             }), NULL));
    }
    m_pBottleList->getInnerContainer()->setContentSize(Size(50+150*count, 200));
    
}


void MakeSnowconeScene::_showTray()
{
    if (!m_pTray) {
        std::stringstream ostr;
        ostr<<"ice/tray"<<m_nShapeIndex<<".png";
        m_pTray = _createDrageNode(localPath(ostr.str()));
        this->addChildToContentLayer(m_pTray);
        
        m_pTrayClipping=  ClippingNode::create();
        m_pTray->addChild(m_pTrayClipping);
        m_pTrayClipping->setPosition(Vec2::ZERO);
        m_pTrayClipping->setAlphaThreshold(0.5);
    }
    m_pTray->setLocalZOrder(1);
    m_pTray->setScale(0.75);
    m_pTray->setTouchEnabled(false);
    
    std::stringstream ostr;
    ostr<<"ice/mask"<<m_nShapeIndex<<".png";
    m_pTrayMask = Sprite::create(localPath(ostr.str()));
    m_pTrayClipping->setStencil(m_pTrayMask);

    for (int i = 0; i<m_nUpCount+m_nDownCount; i++) {
        std::stringstream ostr;
        ostr<<"ice/tray"<<m_nShapeIndex<<"_water"<<i<<".png";
        
        Sprite* pWater = Sprite::create(localPath(ostr.str()));
        m_pTrayClipping->addChild(pWater);
        pWater->setTag(i);
        pWater->setAnchorPoint(Vec2(0.5, 0.1));
        pWater->setPosition(Vec2(gWaterPos[m_nShapeIndex][i].x-m_pTray->getContentSize().width/2, gWaterPos[m_nShapeIndex][i].y-m_pTray->getContentSize().height/2-pWater->getContentSize().height*0.4));
        pWater->setOpacity(0);
        m_WaterVector.pushBack(pWater);
    }
    m_pTray->stopAllActions();
    CMVisibleRect::setPositionAdapted(m_pTray, 180-visibleSize.width, 350);
    m_pTray->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                        Spawn::create(JumpBy::create(0.4, Vec2::ZERO, 10, 1),
                                        RotateBy::create(0.3, 10), NULL),
                                        RotateBy::create(0.3, -10),
                                        CallFunc::create([=]()
                                                         {
                                                         }), NULL));
    
    if (!m_pWaterBottle) {
        m_pWaterBottle = _createDrageNode(localPath("ice/kettle0.png"));
        this->addChildToContentLayer(m_pWaterBottle);
        m_pWaterBottle->setActionPoint(Vec2(13, 302));
    }
    CMVisibleRect::setPositionAdapted(m_pWaterBottle, 520+visibleSize.width, 380);
    m_pWaterBottle->setTouchEnabled(false);
    m_pWaterBottle->runAction(Sequence::create(DelayTime::create(1),
                                               EaseSineOut::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0))),
                                               CallFunc::create([=]()
                                                                {
                                                                    m_pWaterBottle->setOrgPositionDefault();
                                                                    m_pWaterBottle->setTouchEnabled(true);
                                                                    m_pGuideLayer->showGuideMove(m_pWaterBottle->getPosition(), m_pTray->getPosition());
                                                                }), NULL));
}

void MakeSnowconeScene::_showFreezer()
{
    m_pFreezer = FreezerNode::create();
    this->addChildToContentLayer(m_pFreezer);
    m_pFreezer->onDoorCallback = CC_CALLBACK_1(MakeSnowconeScene::onFreezerCallback, this);
    CMVisibleRect::setPosition(m_pFreezer, 320+visibleSize.width, 550);
    m_pFreezer->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
}

void MakeSnowconeScene::_showMachine()
{
    m_pMachine = MachineNode::create();
    m_pMachine->onStateCallback = CC_CALLBACK_1(MakeSnowconeScene::onMachineStateCallback, this);
    this->addChildToContentLayer(m_pMachine);
    CMVisibleRect::setPosition(m_pMachine, Vec2(320-visibleSize.width, 360));
    m_pMachine->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pMachine->setLocalZOrder(1);
}

void MakeSnowconeScene::_showShelf(float dt,bool isCenter)
{
    Sprite* pShelf =(Sprite*)m_pBGLayer->getChildByName("shelf");
    if (!pShelf) {
        pShelf = Sprite::create(localPath("shelf.png"));
        this->addChildToBGLayer(pShelf);
        pShelf->setName("shelf");
    }
    CMVisibleRect::setPositionAdapted(pShelf, 320, 670+dt+visibleSize.height,kBorderNone,isCenter?kBorderNone:kBorderTop);
    pShelf->runAction(MoveBy::create(0.5, Vec2(0, -visibleSize.height)));
}

void MakeSnowconeScene::_moveawayShelf()
{
    Sprite* pShelf =(Sprite*)m_pBGLayer->getChildByName("shelf");
    if (pShelf){
        pShelf->runAction(Sequence::create(DelayTime::create(0.3),
                                           EaseBackIn::create(MoveBy::create(0.8, Vec2(0, visibleSize.height))), NULL));
    }
    
}

void MakeSnowconeScene::_showCone(Vec2 posDelt)
{
    if (!m_pSnowConeNode){
        m_pSnowConeNode = Node::create();
        this->addChildToContentLayer(m_pSnowConeNode);
    }
    CMVisibleRect::setPosition(m_pSnowConeNode, 320+posDelt.x, 20+posDelt.y-visibleSize.height);
    
    if (!m_pCone) {
        m_pCone = Sprite::create(localPath("crush/cone0_0.png"));
        m_pSnowConeNode->addChild(m_pCone);
    }
    
    if (!m_pConeFront){
        m_pConeFront = Sprite::create(localPath("crush/cone0_1.png"));
        m_pSnowConeNode->addChild(m_pConeFront);
        m_pConeFront->setLocalZOrder(1);
    }
    
    if (!m_pConeIce){
        m_pConeIce = Sprite::create(localPath("crush/ice0_0.png"));
        m_pSnowConeNode->addChild(m_pConeIce);
        m_pConeIce->setVisible(false);
    }
    m_pConeIce->setPosition(Vec2(0, 360-10));
    
    if (!m_pSnowSCribble){
        m_pSnowSCribble = ScribbleNode::create(m_pConeIce->getContentSize());
        m_pSnowConeNode->addChild(m_pSnowSCribble);
        m_pSnowSCribble->useBrush(localPath("brush.png"),Scribble::BrushType::eBrush);
    }
    m_pSnowSCribble->setPosition(Vec2(0, 360-10));
    
    if (!m_pSpoon){
        m_pSpoon = _createDrageNode(localPath("crush/spoon0.png"));
        this->addChildToContentLayer(m_pSpoon);
        CMVisibleRect::setPosition(m_pSpoon, 320+visibleSize.width, 560);
        m_pSpoon->setTouchEnabled(false);
        m_pSpoon->setLocalZOrder(5);
        m_pSpoon->setActionPoint(Vec2(18, 24));
    }
    
    if (!m_pColorSCribble){
        m_pColorSCribble = ScribbleNode::create(m_pConeIce->getContentSize());
        m_pSnowConeNode->addChild(m_pColorSCribble);
        m_pColorSCribble->useBrush(localPath("brush.png"),Scribble::BrushType::eBrush);
    }
    m_pColorSCribble->setPosition(Vec2(0, 360-10));
    
    m_pSnowConeNode->runAction(Sequence::create(DelayTime::create(2),
                                                MoveBy::create(0.8, Vec2(0, visibleSize.height)), NULL));
    
    if (m_nSnowPutCount==0) {
        m_pSpoon->runAction(Sequence::create(DelayTime::create(2),
                                             MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                             CallFunc::create([=](){
            m_pSpoon->setTouchEnabled(true);
            m_pSpoon->setOrgPositionDefault();
            m_pGuideLayer->showGuideMove(m_pSpoon->getPosition(), Vec2(visibleSize.width/2, visibleSize.height*0.8));
        }), NULL));
    }
}

void MakeSnowconeScene::_moveawayCone()
{
    if (m_pSnowConeNode) {
        m_pSnowConeNode->runAction(MoveBy::create(0.8, Vec2(0, -visibleSize.height)));
    }
}
void MakeSnowconeScene::_pourWater()
{
    m_pWaterBottle->setTouchEnabled(false);
    int index = m_nPourCount;
//    if (m_nPourCount>=m_nUpCount) {
//        index = m_nPourCount-m_nUpCount;
//    }
    log("=======%d",index);
    Node* pNode = m_WaterVector.at(index);
    Vec2 point = pNode->getPosition();
    point = pNode->getParent()->convertToWorldSpace(point);
    point = m_pWaterBottle->getParent()->convertToNodeSpace(point);
//    point.x+=100;
    point.x+=170;
    point.y+=360;
    
    m_pWaterBottle->setPosition(point);
    m_pWaterBottle->setTouchEnabled(false);
    m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                               CallFunc::create([=]()
                                                                {
                                                                    m_pWaterBottle->getDragSprite()->setTexture(localPath("ice/kettle2.png"));
                                                                    
                                                                    
                                                                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/water2.plist");
                                                                    this->addChildToContentLayer(pParticle);
                                                                    pParticle->setName("dropwater");
                                                                    pParticle->setLocalZOrder(10);
                                                                    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pWaterBottle->getWorldSpaceActionPoint()));
                                                                    
                                                                    
                                                                    ParticleSystemQuad* pParticle2  = ParticleSystemQuad::create("particle/water.plist");
                                                                    this->addChildToContentLayer(pParticle2);
                                                                    pParticle2->setName("water");
                                                                    pParticle2->setLocalZOrder(10);
                                                                    
                                                                    pParticle2->setPosition(m_pContentLayer->convertToNodeSpace(m_pWaterBottle->getWorldSpaceActionPoint()));
                                                                    
                                                                    
                                                                    AudioHelp::getInstance()->playEffect("add_water.mp3");
                                                                }), NULL));
    
    
    if (pNode) {
        pNode->setScale(0.2);
        int temp = m_nPourCount;
        log("====temp %d====",temp);
        pNode->runAction(Sequence::create(DelayTime::create(1),
                                          FadeIn::create(0.1),
                                          ScaleTo::create(1.5, 1),
                                          CallFunc::create([=]()
                                                           {
                                                               log("====temp %d====",temp);
                                                               ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("water");
                                                               if (pParticle) {
                                                                   pParticle->stopSystem();
                                                                   pParticle->removeFromParent();
                                                               }
                                                               ParticleSystemQuad* pParticle2 = (ParticleSystemQuad*)m_pContentLayer->getChildByName("dropwater");
                                                               if (pParticle2) {
                                                                   pParticle2->stopSystem();
                                                                   pParticle2->removeFromParent();
                                                               }
                                                               if (temp>=m_nDownCount+m_nUpCount-1) {
                                                                   log("====1====");
                                                                   m_pTray->runAction(Sequence::create(MoveTo::create(0.5, Vec2(visibleSize.width/2, visibleSize.height*0.2)),
                                                                                                       CallFunc::create([=](){
                                                                       _showFreezer();
                                                                   }), NULL));
                                                                   m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, 100),
                                                                                                            
                                                                                                              MoveBy::create(0.5, Vec2(visibleSize.width, 100)),
                                                                                                              CallFunc::create([=]()
                                                                                                                               {
                                                                                                                                   
                                                                                                                                   Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
                                                                                                                                   pos = m_pGuideLayer->convertToNodeSpace(pos);
                                                                                                                                   m_pGuideLayer->showGuideTap(pos);
                                                                                                                               }),NULL));
                                                                   m_pWaterBottle->setTouchEnabled(true);
                                                                   
                                                               }else{
                                                                   log("====3====");
                   m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, 100),
                                                              CallFunc::create([=]()
                                                                               {
                                                                                   m_pWaterBottle->getDragSprite()->setTexture(localPath("ice/kettle1.png"));
                                                                               }),
                                                              MoveTo::create(0.5, m_pWaterBottle->getOrgPosition()),
                                                              CallFunc::create([=]()
                                                                               {
                                                                                   m_pWaterBottle->setTouchEnabled(true);
//                                                                                   _pourWater();
                                                                               }),NULL));
               }
                                                           }), NULL));
    }
    m_nPourCount++;
}

void MakeSnowconeScene::_putTrayInFreezer()
{
    Point worldPoint = m_pTray->getWorldSpaceActionPoint();
    
    Rect rect = m_pFreezer->getDoorWorldRect();
    if (rect.containsPoint(worldPoint)) {
        m_pTray->setTouchEnabled(false);
        m_pTray->setPosition(Vec2(visibleSize.width/2+50, m_pFreezer->getPositionY()));
        m_pTray->runAction(Sequence::create(ScaleTo::create(0.5, 0.5),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pTrayInFreezer = Sprite::createWithTexture(m_pTray->getDragSprite()->getTexture());
                                                                 for (int i = 0; i<m_nUpCount+m_nDownCount; i++) {
                                                                     std::stringstream ostr;
                                                                     ostr<<"ice/tray"<<m_nShapeIndex<<"_water"<<i<<".png";
                                                                     
                                                                     Sprite* pWater = Sprite::create(localPath(ostr.str()));
                                                                     m_pTrayInFreezer->addChild(pWater);
                                                                     pWater->setTag(i);
                                                                     pWater->setPosition(gWaterPos[m_nShapeIndex][i]);
                                                                 }
                                                                 m_pFreezer->addChild(m_pTrayInFreezer);
                                                                 m_pTrayInFreezer->setScale(0.6,0.5);
                                                                 m_pTrayInFreezer->setPosition(Vec2(50, -50));
                                                                 
                                                                 m_pTray->setVisible(false);
                                                                 m_pFreezer->closeDoor();
                                                                 AudioHelp::getInstance()->playEffect("put_into_fridge.mp3");
                                                             }), NULL));
        m_pFreezer->runAction(Sequence::create(DelayTime::create(1),
                                               MoveBy::create(0.5, Vec2(0, -200)), NULL));
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=]()
                                                          {
                                                              m_pFreezer->doFreeze();
                                                              m_pTrayClipping->setVisible(false);
                                                              m_pTray->setPosition(Vec2(visibleSize.width/2+50, m_pFreezer->getPositionY()));
                                                              
                                                              for (int i = 0; i<m_nUpCount+m_nDownCount; i++) {
                                                                  std::stringstream ostr;
                                                                  ostr<<"ice/tray"<<m_nShapeIndex<<"_ice"<<i<<".png";
                                                                  Sprite* pIce = Sprite::create(localPath(ostr.str()));
                                                                  m_pTray->addChild(pIce);
                                                                  pIce->setTag(i);
                                                                  pIce->setAnchorPoint(Vec2(0.5, 0));
                                                                  
                                                                  pIce->setPosition(Vec2(gWaterPos[m_nShapeIndex][i].x-m_pTray->getContentSize().width/2, gWaterPos[m_nShapeIndex][i].y-m_pTray->getContentSize().height/2-pIce->getContentSize().height/2));
                                                                  m_IceVector.pushBack(pIce);
                                                                  
                                                                  
                                                              }
                                                          }),
                                         DelayTime::create(5),
                                         CallFunc::create([=]()
                                                          {
                                                              AudioHelp::getInstance()->playEffect("fridge_off.mp3");
                                                          }),
                                         DelayTime::create(0.5),
                                         CallFunc::create([=]()
                                                          {
                                                              m_bFreezed = true;
                                                              m_pFreezer->stopFreeze();
                                                              m_pFreezer->setTouchEnable(true);
                                                              Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
                                                              pos = m_pGuideLayer->convertToNodeSpace(pos);
                                                              m_pGuideLayer->showGuideTap(pos);
                                                          }), NULL));
    }

}

void MakeSnowconeScene::_addIceMachine()
{
    m_pTray->runAction(Repeat::create(Sequence::create(DelayTime::create(0.1),
                                                       Spawn::create(ScaleTo::create(0.3, 0.55,0.5),
                                                                     JumpBy::create(0.4, Vec2::ZERO, 50, 1), NULL),
                                                       CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("ice_cube_fall.mp3");
        
        stringstream ostr;
        ostr<<"ice/ice"<<m_nShapeIndex<<".png";
        Sprite* pIce = Sprite::create(localPath(ostr.str()));
        pIce->setScale(0.5);
        float angle = 10.0*(arc4random()%10);
        pIce->setRotation(angle);
        Sprite* pNode = m_IceVector.at(0);
        m_pMachine->addIce(pIce, pNode->getParent()->convertToWorldSpace(pNode->getPosition()));
        pNode->setVisible(false);
        m_IceVector.eraseObject(pNode);
        pNode->removeFromParent();
    }),
                                                       ScaleTo::create(0.3, 0.4), NULL), m_nDownCount+m_nUpCount));
    
    this->runAction(Sequence::create(DelayTime::create(1.1*(m_nDownCount+m_nUpCount)+0.5),
                                     CallFunc::create([=](){
//        m_pGuideLayer->showGuideMove(Vec2(m_pMachine->getCapWorldRect().getMidX(), m_pMachine->getCapWorldRect().getMaxY()), Vec2(m_pMachine->getCapWorldRect().getMaxX(), m_pMachine->getCapWorldRect().getMidY()));
        m_pGuideLayer->showGuideArrow(Vec2(m_pMachine->getCapWorldRect().getMidX(), m_pMachine->getCapWorldRect().getMidY()),1);
        m_pMachine->setTouchEnable(true);
        m_pTray->runAction(Sequence::create(RotateBy::create(0.3, 50),
                                            MoveBy::create(0.5, Vec2(visibleSize.width, 0)), NULL));
//        for_each (m_IceVector.begin(), m_IceVector.end(), [=](Node* pIce){
//            pIce->removeFromParent();
//        });
    }), NULL));
}

void MakeSnowconeScene::_curshIceFinished()
{
    
}


void MakeSnowconeScene::_resetColor()
{
    if(m_bResetShown){
        m_bResetShown = false;
        m_pGameUI->hideReset();
    }
    m_pColorSCribble->resetCanvas();
}
void MakeSnowconeScene::_playEffectYummy(float)
{
    int radom = arc4random()%100;
    if (radom<100&& radom>85){
        AudioHelp::getInstance()->playEffect("tasty.mp3");
    }else if (radom>70) {
        AudioHelp::getInstance()->playEffect("yummy.mp3");
    }
}