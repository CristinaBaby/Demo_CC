
#include "MakeGummyPizzaScene.h"
#include "SceneManager.h"

MakeGummyPizzaScene::MakeGummyPizzaScene()
{
    m_pWater = nullptr;
    m_pGelatin = nullptr;
    m_pPowderJellew = nullptr;
    m_pSyrupCorn = nullptr;
    
    m_pSpoon = nullptr;
    m_pTray = nullptr;
    m_pBowlFrontView = nullptr;
    m_pBowl = nullptr;
    m_pMixtureFrontView = nullptr;
    m_pMixture = nullptr;
    m_pTableMat = nullptr;
    
    m_bSpoonReady = false;
    m_nMixCount = 0;
    m_nStep = 0;
    m_nCandyAddCount = 0;
    
    m_pOilBottle = nullptr;
    m_pPan = nullptr;
    m_pPanFrontView = nullptr;
    
    m_pKnife = nullptr;
    m_pJellewBoard = nullptr;
    m_nJellewCutCount = 0;
    m_bIsJellewOnBoard = false;
    
}

MakeGummyPizzaScene::~MakeGummyPizzaScene()
{
    
}
bool MakeGummyPizzaScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg.png"));
    pBg->setAnchorPoint(Vec2(0.5, 0));
    pBg->setPosition(Vec2(visibleSize.width/2, -80));
    this->addChildToBGLayer(pBg);
    m_pBg = pBg;
    m_pOven = OvenNode::create();
    this->addChildToContentLayer(m_pOven);
    CMVisibleRect::setPositionAdapted(m_pOven, 100, 800);
    m_pOven->onDoorCallback = CC_CALLBACK_1(MakeGummyPizzaScene::onOvenDoorCallback, this);
    m_pOven->onBakeCallback = CC_CALLBACK_2(MakeGummyPizzaScene::onOvenBakeCallback, this);
    m_pOven->setTouchEnable(false);
    
    m_pFreezer = FreezerNode::create();
    this->addChildToContentLayer(m_pFreezer);
    CMVisibleRect::setPositionAdapted(m_pFreezer, 750, 800);
    m_pFreezer->onDoorCallback = CC_CALLBACK_1(MakeGummyPizzaScene::onFreezerDoorCallback, this);
    m_pFreezer->setTouchEnable(false);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _createMakeCandyUI();
    }), NULL));
    
//    **************** test cut jellew  ********
//    _showBoard();
//    _showJellewIngredient(0);
//    *****************************
    
//    *************** test make Vanilla *****
//    _createMakeVanillaUI();
//    _createMakeJellewPieceUI();
//    ****************************************
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
//    AdsManager::getInstance()->setLayoutBanner(LAYOUT_TYPE::kLayoutCenterRight);
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        if (!UserDefault::getInstance() -> getBoolForKey("removeAds")) {
            AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
        }
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void MakeGummyPizzaScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagNext) {
        btn->setTouchEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(0.5),
                                        CallFunc::create([=](){
            
            btn->setTouchEnabled(true);
        }), NULL));
        if (m_nStep==eStepAddRedCandy) {
            m_pGameUI->hideNext();
            _finishAddCandy(0);
        }else if(m_nStep == eStepAddJellew){
            m_pGameUI->hideNext();
            _saveMixture();
            _finishAddCandy(1);
        }else{
            
            SceneManager::replaceTheScene<DecorateScene>();
        }
    }
}

void MakeGummyPizzaScene::onOvenDoorCallback(OvenNode* pOven)
{
    m_pGuideLayer->removeGuide();
//    pOven->setTouchEnable(true);
//    open
    if (pOven->getIsOpen()) {
        pOven->setSwitchEnable(false);
        if (!pOven->getBaked()) {
            Node* pIn = pOven->getChildByName("ovenin");
            if (!pIn) {
                m_pBowlFrontView->setTouchEnabled(true);
                m_pGuideLayer->showGuideMove(m_pBowlFrontView->getPosition(), Vec2(pOven->getDoorWorldRect().getMidX(), pOven->getDoorWorldRect().getMidY()));
            }
        }else{
            pOven->setTouchEnable(false);
            Node* pIn = pOven->getChildByName("ovenin");
            if (pIn) {
                pIn->removeFromParent();
            }
            m_pBowlFrontView->setVisible(true);
            m_pBowlFrontView->setTouchEnabled(true);
            m_pGuideLayer->showGuideMove(Vec2(pOven->getDoorWorldRect().getMidX(), pOven->getDoorWorldRect().getMidY()),m_pBowl->getPosition());
        }
//        close
    }else{
        if (!pOven->getBaked()){
            m_pBowlFrontView->setTouchEnabled(false);
            Node* pIn = pOven->getChildByName("ovenin");
            if (pIn){
                pOven->setSwitchEnable(true);
                m_pGuideLayer->showGuideTap(pOven->getSwitchWorldPos());
            }
        }
    }
}

void MakeGummyPizzaScene::onOvenBakeCallback(OvenNode* pOven,int state)
{
    if (state==0) {
        m_pGuideLayer->removeGuide();
    }else{
        _finishBake();
    }
}

void MakeGummyPizzaScene::onFreezerDoorCallback(FreezerNode* pFreezer)
{
    m_pGuideLayer->removeGuide();
    //    open
    if (pFreezer->getIsOpen()) {
        if (!pFreezer->getFreezed()) {
            Node* pIn = pFreezer->getChildByName("freezerin");
            if (!pIn) {
                m_pPanFrontView->setTouchEnabled(true);
                m_pGuideLayer->showGuideMove(m_pPanFrontView->getPosition(), Vec2(pFreezer->getDoorWorldRect().getMidX(), pFreezer->getDoorWorldRect().getMidY()));
            }
        }else{
            pFreezer->setTouchEnable(false);
            Node* pIn = pFreezer->getChildByName("freezerin");
            if (pIn) {
                pIn->removeFromParent();
            }
            m_pPanFrontView->setVisible(true);
            m_pPanFrontView->setTouchEnabled(true);
            m_pGuideLayer->showGuideMove(Vec2(pFreezer->getDoorWorldRect().getMidX(), pFreezer->getDoorWorldRect().getMidY()),m_pPan->getPosition());
        }
        //        close
    }else{
        if (!pFreezer->getFreezed()){
            Node* pIn = pFreezer->getChildByName("freezerin");
            if (pIn) {
                m_pPanFrontView->setTouchEnabled(false);
                
                pFreezer->setTouchEnable(false);
                _freeze();
            }
        }
    }
    
}
void MakeGummyPizzaScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    _hideShadow(pDragNode);
    pDragNode->setLocalZOrder(10);
    if (pDragNode == m_pSpoon){
        if (m_bSpoonReady){
            AudioHelp::getInstance()->playLoopEffect("stir.mp3");
            m_pMixture->runAction(RepeatForever::create(RotateBy::create(1, 360)));
        }
    }else if (pDragNode==m_pRedCandy || pDragNode==m_pGreenCandy || pDragNode==m_pYellowCandy){
        pDragNode->setVisible(true);
        if (pDragNode!=m_pRedCandy) {
            std::stringstream ostr;
            int radom = arc4random()%3;
            if (pDragNode==m_pYellowCandy) {
                ostr<<"make2/gummy_yellow2_"<<radom+1<<".png";
            }else if (pDragNode==m_pGreenCandy){
                ostr<<"make2/gummy_green2_"<<radom+1<<".png";
            }
            pDragNode->setDragSprite(Sprite::create(localPath(ostr.str())));
            pDragNode->setContentSize(m_pJellewPlate->getContentSize());
        }
    }
}
void MakeGummyPizzaScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode == m_pSpoon){
        Rect rect = m_pBowl->getDragSprite()->getBoundingBox();
        rect.origin = m_pBowl->convertToWorldSpace(rect.origin);
        rect.size = Size(rect.size.width-50, rect.size.height-50);
        rect.origin = rect.origin+Vec2(25, 25);
        if (rect.containsPoint(worldPoint)){
            _mix(m_nStep);
        }
        
    }
    
}
void MakeGummyPizzaScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
    for_each(m_JellewGreens.begin(), m_JellewGreens.end(), [=](DragNode* pJellew){
        if (pJellew == pDragNode) {
            Rect rect = m_pBoard->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())&& m_nJellewCutCount==0 && !m_bIsJellewOnBoard) {
                auto iter = std::find(m_JellewGreens.begin(), m_JellewGreens.end(), pJellew);
                if (iter != m_JellewGreens.end())
                {
                    m_JellewGreens.erase(iter);
                }
                _showJellewBoard(0);
                if (!m_pKnife) {
                    _showKnife();
                }
                m_nJellewType = 0;
                m_pKnife->setTouchEnabled(true);
                pDragNode->removeFromParent();
            }else{
                pDragNode->back();
            }
            return;
        }
    });
    for_each(m_JellewYellows.begin(), m_JellewYellows.end(), [=](DragNode* pJellew){
        if (pJellew == pDragNode) {
            Rect rect = m_pBoard->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())&& m_nJellewCutCount==0 && !m_bIsJellewOnBoard) {
                auto iter = std::find(m_JellewYellows.begin(), m_JellewYellows.end(), pJellew);
                if (iter != m_JellewYellows.end())
                {
                    m_JellewYellows.erase(iter);
                }
                _showJellewBoard(1);
                m_nJellewType = 1;
                m_pKnife->setTouchEnabled(true);
                pDragNode->removeFromParent();
            }else{
                pDragNode->back();
            }
            return;
        }
    });
    
    if (pDragNode==m_pWater || pDragNode==m_pGelatin || pDragNode==m_pSyrupCorn || pDragNode==m_pPowderJellew || pDragNode==m_pMilk || pDragNode==m_pSugar || pDragNode==m_pVanilla || pDragNode==m_pPowderJellew2) {
        Rect rect = m_pBowl->getDragSprite()->getBoundingBox();
        rect.origin = m_pBowl->convertToWorldSpace(rect.origin);
        Vec2 pos = pDragNode->getPosition();
        pos = pDragNode->getParent()->convertToWorldSpace(pos);
        if (rect.containsPoint(pos) && m_nCandyAddCount==pDragNode->getTag()) {
            pDragNode->setTouchEnabled(false);
            if (m_nStep==eStepMakeCandy) {
                _addCandyIngredient(pDragNode);
            }else if (m_nStep==eStepMakeVanlia) {
                _addVanlliaIngredient(pDragNode);
            }else if (m_nStep==eStepMakeJellew) {
                _addJewllewPieceIngredient(pDragNode);
            }
        }else{
            pDragNode->back(0,[=](){
                _showShadow(pDragNode);
            });
        }
    }else if (pDragNode==m_pSpoon) {
        m_pMixture->stopAllActions();
    }else if (pDragNode==m_pBowlFrontView) {
        if (!m_pOven->getBaked()) {
            Rect rect = m_pOven->getDoorWorldRect();
            if (rect.containsPoint(worldPoint)) {
                _putinOven();
            }else{
                pDragNode->back();
            }
        }else{
            Rect rect = m_pBowl->getDragSprite()->getBoundingBox();
            rect.origin = m_pBowl->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                _putoutOven();
            }else{
                pDragNode->back();
            }
        }
    }else if (pDragNode==m_pBowl) {
        Rect rect = m_pPan->getDragSprite()->getBoundingBox();
        rect.origin = m_pPan->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            _addBatterPan();
        }
        
    }else if (pDragNode==m_pPanFrontView) {
        if (!m_pFreezer->getFreezed()) {
            Rect rect = m_pFreezer->getDoorWorldRect();
            if (rect.containsPoint(worldPoint)) {
                _putinFreezer();
            }else{
                pDragNode->back();
            }
        }else{
            Rect rect = m_pPan->getDragSprite()->getBoundingBox();
            rect.origin = m_pPan->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                _putoutFreezer();
            }else{
                pDragNode->back();
            }
        }
    }else if (pDragNode==m_pOilBottle) {
        Rect rect = m_pPan->getDragSprite()->getBoundingBox();
        rect.origin = m_pPan->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            _addOil();
        }
    }else if (pDragNode==m_pRing) {
        Vec2 pos = m_pRing->getPosition();
        int radius = 100;
        Vec2 offset = pos-(m_pPan->getPosition()-Vec2(0, 10));
        if (powf(offset.x, 2)+powf(offset.y, 2)<=powf(radius, 2)) {
            int size = m_ringVector.size();
            bool newRing = true;
            for (int i = 0; i<size; i++) {
                Sprite* pRing = m_ringVector.at(i);
                Vec2 pos1 = pRing->getPosition();
                pos1 = pRing->getParent()->convertToWorldSpace(pos1);
                Vec2 offsetRing = pos1-worldPoint;
                float distance = pRing->getContentSize().width;
                
                if (powf(offsetRing.x, 2)+powf(offsetRing.y, 2)<powf(distance, 2)) {
                    newRing = false;
                    break;
                }
            }
            if (newRing) {
                Sprite* pSprite = Sprite::create(localPath("make1/mould_small.png"));
                m_pMixturePan->addChild(pSprite);
                pSprite->setPosition(m_pMixturePan->convertToNodeSpace(worldPoint));
                m_ringVector.push_back(pSprite);
                if (size>=3) {
//                    finish
                    _finishCutCandy();
                }
            }
        }
        m_pRing->back();
    }else if (pDragNode==m_pKnife) {
        Rect rect = m_pJellewBoard->getDragSprite()->getBoundingBox();
        rect.origin = m_pJellewBoard->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            
            _cutJellew(m_nJellewType);
        }
    }else if (pDragNode==m_pJellewBoard) {
        Rect rect = m_pJellewPlateEmpty->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())) {
            _putJellew(m_nJellewType);
        }else{
            pDragNode->back();
        }
    }else if (pDragNode==m_pJellewGreenBlock) {
        Rect rect = m_pJellewPlateEmpty->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())) {
            _putJellew(0);
        }else{
            pDragNode->back();
        }
    }else if (pDragNode==m_pJellewYellowBlock) {
        Rect rect = m_pJellewPlateEmpty->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())) {
            _putJellew(1);
        }else{
            pDragNode->back();
        }
    }else if (pDragNode==m_pRedCandy || pDragNode==m_pGreenCandy || pDragNode==m_pYellowCandy) {
        Vec2 pos = pDragNode->getPosition();
        int radius = 140;
        if (pDragNode!=m_pRedCandy) {
            radius = 200;
        }
        Vec2 offset = pos-m_pPan->getPosition()-Vec2(0, 10);
        if (powf(offset.x, 2)+powf(offset.y, 2)<=powf(radius, 2)){
            Sprite* pCandy = Sprite::createWithTexture(pDragNode->getDragSprite()->getTexture());
            m_pMixturePan->addChild(pCandy);
            pCandy->setPosition(m_pMixturePan->convertToNodeSpace(worldPoint));
            float angle = -50+10*(arc4random()%10);
            pCandy->setRotation(angle);
            m_candyVector.push_back(pCandy);
            pDragNode->setVisible(false);
            pDragNode->setPosition(pDragNode->getOrgPosition());
            m_pGameUI->showNextLayout();
        }else{
            pDragNode->back(0,[=](){
                pDragNode->setVisible(false);
            });
        }
    }else if (pDragNode == m_pGummyPiece) {
        Rect rect = m_pPan->getDragSprite()->getBoundingBox();
        rect.origin = m_pPan->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            _addGummyPiece();
        }else{
            pDragNode->back();
        }
    }
}
void MakeGummyPizzaScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    _showShadow(pDragNode);
    if (pDragNode==m_pRedCandy || pDragNode==m_pGreenCandy || pDragNode==m_pYellowCandy){
        pDragNode->setVisible(false);
    }else if (pDragNode==m_pKnife) {
        Rect rect = m_pJellewBoard->getDragSprite()->getBoundingBox();
        rect.origin = m_pJellewBoard->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            
            _cutJellew(m_nJellewType);
        }
    }
}
#pragma mark - initData
void MakeGummyPizzaScene::_initData()
{
    setExPath("content/make/gummy/");
}


void MakeGummyPizzaScene::_createMakeCandyUI()
{
    m_nStep = eStepMakeCandy;
    _showBowl();
    _showCandyIngredient(0);
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=]{
        m_pGuideLayer->showGuideMove(m_pWater->getPosition(), m_pBowl->getPosition());
    }), NULL));
}

void MakeGummyPizzaScene::_showCandyIngredient(int group)
{
    _showTray();
    switch (group) {
        case 0:
        {
            m_pWater = _createDrageNode(localPath("make1/water_2.png"));
            this->addChildToContentLayer(m_pWater);
            CMVisibleRect::setPositionAdapted(m_pWater, 800+visibleSize.width/2, 430);
            Sprite* pWater = Sprite::create(localPath("make1/water_3.png"));
            m_pWater->addChild(pWater,1);
            pWater->setName("water");
            pWater->setPosition(Vec2(-10, 30));
            m_pWater->setTag(0);
            m_pWater->setActionPoint(Vec2(15, 150));
            
            m_pGelatin = _createDrageNode(localPath("make1/gelatin_1.png"));
            this->addChildToContentLayer(m_pGelatin);
            CMVisibleRect::setPositionAdapted(m_pGelatin, 800+visibleSize.width/2, 205);
            Sprite* pGelatin = Sprite::create(localPath("make1/gelatin_3.png"));
            m_pGelatin->addChild(pGelatin,1);
            pGelatin->setName("gelatin");
            pGelatin->setPosition(Vec2(-35, 18));
            m_pGelatin->setTag(1);
            m_pGelatin->setActionPoint(Vec2(37, 110));
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pWater->setTouchEnabled(false);
            m_pWater->runAction(action);
            
            m_pGelatin->setTouchEnabled(false);
            m_pGelatin->runAction(action->clone());
        }
            break;
        case 1:
        {
            m_pPowderJellew = _createDrageNode(localPath("make1/jelly powde.png"));
            this->addChildToContentLayer(m_pPowderJellew);
            CMVisibleRect::setPositionAdapted(m_pPowderJellew, 800+visibleSize.width/2, 430);
            m_pPowderJellew->setTag(2);
            m_pPowderJellew->setActionPoint(Vec2(140, 170));
            
            m_pSyrupCorn = _createDrageNode(localPath("make1/corn-syrup_2.png"));
            this->addChildToContentLayer(m_pSyrupCorn);
            CMVisibleRect::setPositionAdapted(m_pSyrupCorn, 800+visibleSize.width/2, 205);
            m_pSyrupCorn->setTag(3);
            m_pSyrupCorn->setActionPoint(Vec2(10, 98));
            Sprite* pSyrup = Sprite::create(localPath("make1/corn syrup_3.png"));
            m_pSyrupCorn->addChild(pSyrup,1);
            pSyrup->setName("syrup");
            pSyrup->setPosition(Vec2(0, 8));
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pPowderJellew->setTouchEnabled(false);
            m_pPowderJellew->runAction(action);
            
            m_pSyrupCorn->setTouchEnabled(false);
            m_pSyrupCorn->runAction(action->clone());
        }
            break;
        case 2:
        {
            _showOilBottle();
        }
            break;
        default:
            break;
    }
    
}

void MakeGummyPizzaScene::_showRing()
{
    AudioHelp::getInstance()->playEffect("vo_cut4pieces.mp3");
    m_pRing = _createDrageNode(localPath("make1/mould_big.png"));
    m_pRing->setTouchEnabled(false);
    this->addChildToContentLayer(m_pRing);
    
    CMVisibleRect::setPositionAdapted(m_pRing,800+visibleSize.width/2, 300);
    m_pRing->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                         CallFunc::create([=](){
        m_pRing->setOrgPositionDefault();
        m_pRing->setTouchEnabled(true);
    }), NULL));
}

void MakeGummyPizzaScene::_showJellewIngredient(int group)
{
    _showTray();
    switch (group) {
        case 0:
        {
            m_pJellewPlate = Sprite::create(localPath("make1/cut/palte2.png"));
            this->addChildToContentLayer(m_pJellewPlate);
            CMVisibleRect::setPositionAdapted(m_pJellewPlate, 800+visibleSize.width/2, 430);
            
            m_pJellewPlateEmpty = Sprite::create(localPath("make1/cut/palte2.png"));
            this->addChildToContentLayer(m_pJellewPlateEmpty);
            CMVisibleRect::setPositionAdapted(m_pJellewPlateEmpty, 800+visibleSize.width/2, 205);
            
            m_pJellewGreenBlock = Node::create();
            this->addChildToContentLayer(m_pJellewGreenBlock);
            CMVisibleRect::setPositionAdapted(m_pJellewGreenBlock, 800, 205);
            
            for (int i = 0; i<3; i++) {
                DragNode* pDragNode = _createDrageNode(localPath("make1/cut/gummy_green_small.png"));
                this->addChildToContentLayer(pDragNode);
                CMVisibleRect::setPositionAdapted(pDragNode, 800-20+i*25+visibleSize.width/2, 430);
                m_JellewGreens.push_back(pDragNode);
            }
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pJellewPlate->runAction(action);
            
            for_each(m_JellewGreens.begin(), m_JellewGreens.end(), [=](DragNode* pJellew){
                pJellew->setTouchEnabled(false);
                pJellew->runAction(action->clone());
            });
        }
            break;
        case 1:
        {
            CMVisibleRect::setPositionAdapted(m_pJellewPlate, 800+visibleSize.width/2, 430);
            
            CMVisibleRect::setPositionAdapted(m_pJellewPlateEmpty, 800+visibleSize.width/2, 205);
            
            m_pJellewYellowBlock = Node::create();
            this->addChildToContentLayer(m_pJellewYellowBlock);
            CMVisibleRect::setPositionAdapted(m_pJellewYellowBlock, 800, 205);
            
            for (int i = 0; i<3; i++) {
                DragNode* pDragNode = _createDrageNode(localPath("make1/cut/gummy_yellow_small.png"));
                this->addChildToContentLayer(pDragNode);
                CMVisibleRect::setPositionAdapted(pDragNode, 800-20+i*25+visibleSize.width/2, 430);
                m_JellewYellows.push_back(pDragNode);
            }
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pJellewPlate->runAction(action);
            
            for_each(m_JellewYellows.begin(), m_JellewYellows.end(), [=](DragNode* pJellew){
                pJellew->setTouchEnabled(false);
                pJellew->runAction(action->clone());
            });
        }
            break;
        default:
            break;
    }

}

void MakeGummyPizzaScene::_showBoard()
{
    m_pBoard = Sprite::create(localPath("make1/cut/cutting_board.png"));
    this->addChildToContentLayer(m_pBoard);
    CMVisibleRect::setPosition(m_pBoard, 320-visibleSize.width, 300);
    m_pBoard->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
}

void MakeGummyPizzaScene::_removeBoard()
{
    m_pBoard->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
}

void MakeGummyPizzaScene::_showKnife()
{
    AudioHelp::getInstance()->playEffect("vo_cut_gummy_candy.mp3");
    m_pKnife = _createDrageNode(localPath("make1/cut/knife.png"));
    this->addChildToContentLayer(m_pKnife);
    m_pKnife->stopAllActions();
    m_pKnife->setTouchEnabled(false);
    m_pKnife->setActionPoint(Vec2(22, 180));
    CMVisibleRect::setPositionAdapted(m_pKnife,500+visibleSize.width, 300);
    m_pKnife->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pKnife->setOrgPositionDefault();
        m_pKnife->setTouchEnabled(true);
    }), NULL));
}

void MakeGummyPizzaScene::_showJellewBoard(int type)
{
    m_bIsJellewOnBoard = true;
    if (!m_pJellewBoard) {
        if (type == 0) {
            m_pJellewBoard = _createDrageNode(localPath("make1/cut/gummy_green_big1.png"));
        }else{
            m_pJellewBoard = _createDrageNode(localPath("make1/cut/gummy_yellow_big1.png"));
        }
        this->addChildToContentLayer(m_pJellewBoard);
    }else{
        if (type == 0) {
            m_pJellewBoard->setDragSprite(Sprite::create(localPath("make1/cut/gummy_green_big1.png")));
        }else{
            m_pJellewBoard->setDragSprite(Sprite::create(localPath("make1/cut/gummy_yellow_1.png")));
        }
    }
    m_pJellewBoard->setPosition(m_pBoard->getPosition());
    m_pJellewBoard->setOrgPositionDefault();
    m_pJellewBoard->setVisible(true);
    m_pJellewBoard->setTouchEnabled(false);
}

void MakeGummyPizzaScene::_createMakeVanillaUI()
{
    m_nStep = eStepMakeVanlia;
    m_nCandyAddCount = 0;
    _showBowl();
    _showVanillaIngredient(0);
}

void MakeGummyPizzaScene::_showVanillaIngredient(int group)
{
    _showTray();
    switch (group) {
        case 0:
        {
            m_pMilk = _createDrageNode(localPath("make2/milk1.png"));
            this->addChildToContentLayer(m_pMilk);
            CMVisibleRect::setPositionAdapted(m_pMilk, 800+visibleSize.width/2, 430);
            Sprite* pWater = Sprite::create(localPath("make2/milk1_2.png"));
            m_pMilk->addChild(pWater,1);
            pWater->setName("milk");
            pWater->setPosition(Vec2(-10, 30));
            m_pMilk->setTag(0);
            m_pMilk->setActionPoint(Vec2(15, 150));
            
            m_pGelatin = _createDrageNode(localPath("make1/gelatin_1.png"));
            this->addChildToContentLayer(m_pGelatin);
            CMVisibleRect::setPositionAdapted(m_pGelatin, 800+visibleSize.width/2, 205);
            Sprite* pGelatin = Sprite::create(localPath("make1/gelatin_3.png"));
            m_pGelatin->addChild(pGelatin,1);
            pGelatin->setName("gelatin");
            pGelatin->setPosition(Vec2(-35, 18));
            m_pGelatin->setTag(1);
            m_pGelatin->setActionPoint(Vec2(37, 110));
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pMilk->setTouchEnabled(false);
            m_pMilk->runAction(action);
            
            m_pGelatin->setTouchEnabled(false);
            m_pGelatin->runAction(action->clone());
        }
            break;
        case 1:
        {
            m_pSugar = _createDrageNode(localPath("make2/sugar_2.png"));
            this->addChildToContentLayer(m_pSugar);
            CMVisibleRect::setPositionAdapted(m_pSugar, 800+visibleSize.width/2, 430);
            m_pSugar->setTag(2);
            m_pSugar->setActionPoint(Vec2(30, 50));
            Sprite* pSugar = Sprite::create(localPath("make2/sugar_3.png"));
            m_pSugar->addChild(pSugar,1);
            pSugar->setName("sugar");
            
            m_pVanilla = _createDrageNode(localPath("make2/vanilla extract.png"));
            this->addChildToContentLayer(m_pVanilla);
            CMVisibleRect::setPositionAdapted(m_pVanilla, 800+visibleSize.width/2, 205);
            m_pVanilla->setTag(3);
            m_pVanilla->setActionPoint(Vec2(23, 29));
            Sprite* pSyrup = Sprite::create(localPath("make2/vanilla extract1.png"));
            m_pVanilla->addChild(pSyrup,1);
            pSyrup->setPosition(Vec2(-30, -30));
            pSyrup->setName("vanilla");
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pSugar->setTouchEnabled(false);
            m_pSugar->runAction(action);
            
            m_pVanilla->setTouchEnabled(false);
            m_pVanilla->runAction(action->clone());
        }
            break;
        case 2:
        {
            m_pSyrupCorn = _createDrageNode(localPath("make1/corn syrup_1.png"));
            this->addChildToContentLayer(m_pSyrupCorn);
            CMVisibleRect::setPositionAdapted(m_pSyrupCorn, 800+visibleSize.width/2, 300);
            m_pSyrupCorn->setTag(4);
            m_pSyrupCorn->setActionPoint(Vec2(10, 98));
            Sprite* pSyrup = Sprite::create(localPath("make1/corn syrup_3.png"));
            m_pSyrupCorn->addChild(pSyrup,1);
            pSyrup->setName("syrup");
            pSyrup->setPosition(Vec2(0, 8));
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            
            m_pSyrupCorn->setTouchEnabled(false);
            m_pSyrupCorn->runAction(action);
        }
            break;
        default:
            break;
    }

}

void MakeGummyPizzaScene::_showAddCandyIngredient(int group)
{
    _showTray();
    switch (group) {
        case 0:
        {
            m_nStep = eStepAddRedCandy;
            m_pCandyPlate = Sprite::create(localPath("make2/plate_gummy_rad.png"));
            this->addChildToContentLayer(m_pCandyPlate);
            CMVisibleRect::setPositionAdapted(m_pCandyPlate, 800+visibleSize.width/2, 300);
            
            for (int i = 0; i<10; i++) {
                Sprite* pCandy = Sprite::create(localPath("make2/gummy_rad.png"));
                m_pCandyPlate->addChild(pCandy);
                float radomX = -50+10.0f*(arc4random()%10);
                float y = sqrtf(powf(50, 2)-powf(radomX, 2));
                pCandy->setPosition(Vec2(radomX, (i%2==0)?y:-y)+m_pCandyPlate->getContentSize()*0.5);
            }
            
            m_pRedCandy = _createDrageNode(localPath("make2/gummy_rad.png"));
            this->addChildToContentLayer(m_pRedCandy);
            CMVisibleRect::setPositionAdapted(m_pRedCandy, 800+visibleSize.width/2, 300);
            m_pRedCandy->setVisible(false);
            m_pRedCandy->setContentSize(m_pCandyPlate->getContentSize()*0.8);
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pCandyPlate->runAction(action);
            
            m_pRedCandy->setTouchEnabled(false);
            m_pRedCandy->runAction(action->clone());
        }
            break;
        case 1:
        {
            m_nStep = eStepAddJellew;
            m_pJellewPlate = Sprite::create(localPath("make1/cut/palte2.png"));
            this->addChildToContentLayer(m_pJellewPlate);
            CMVisibleRect::setPositionAdapted(m_pJellewPlate, 800+visibleSize.width/2, 430);
            
            m_pJellewPlateEmpty = Sprite::create(localPath("make1/cut/palte2.png"));
            this->addChildToContentLayer(m_pJellewPlateEmpty);
            CMVisibleRect::setPositionAdapted(m_pJellewPlateEmpty, 800+visibleSize.width/2, 205);
            
            for (int i = 0; i<3; i++) {
                std::stringstream ostrGreen;
                std::stringstream ostrYellow;
                ostrGreen<<"make1/cut/gummy_green"<<i+1<<".png";
                ostrYellow<<"make1/cut/gummy_yellow"<<i+1<<".png";
                Sprite* pCandy = Sprite::create(localPath(ostrGreen.str()));
                m_pJellewPlate->addChild(pCandy);
                pCandy->setPosition(m_pJellewPlate->getContentSize()*0.5);
                
                Sprite* pCandy2 = Sprite::create(localPath(ostrYellow.str()));
                m_pJellewPlateEmpty->addChild(pCandy2);
                pCandy2->setPosition(m_pJellewPlateEmpty->getContentSize()*0.5);
            }
            m_pGreenCandy = _createDrageNode(localPath("make2/gummy_green2_1.png"));
            this->addChildToContentLayer(m_pGreenCandy);
            CMVisibleRect::setPositionAdapted(m_pGreenCandy, 800+visibleSize.width/2, 430);
            m_pGreenCandy->setVisible(false);
            m_pGreenCandy->setContentSize(m_pJellewPlate->getContentSize());
            
            m_pYellowCandy = _createDrageNode(localPath("make2/gummy_yellow2_1.png"));
            this->addChildToContentLayer(m_pYellowCandy);
            CMVisibleRect::setPositionAdapted(m_pYellowCandy, 800+visibleSize.width/2, 205);
            m_pYellowCandy->setVisible(false);
            m_pYellowCandy->setContentSize(m_pJellewPlate->getContentSize());
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pJellewPlate->runAction(action);
            m_pJellewPlateEmpty->runAction(action->clone());
            
            m_pGreenCandy->setTouchEnabled(false);
            m_pGreenCandy->runAction(action->clone());
            
            m_pYellowCandy->setTouchEnabled(false);
            m_pYellowCandy->runAction(action->clone());
        }
            break;
        default:
            break;
    }
    
}


void MakeGummyPizzaScene::_createMakeJellewPieceUI()
{
    m_nStep = eStepMakeJellew;
    m_nCandyAddCount = 0;
    _showBowl();
    _showJellewPieceIngredient(0);
}

void MakeGummyPizzaScene::_showJellewPieceIngredient(int group)
{
    _showTray();
    switch (group) {
        case 0:
        {
            m_pMilk = _createDrageNode(localPath("make2/milk1.png"));
            this->addChildToContentLayer(m_pMilk);
            CMVisibleRect::setPositionAdapted(m_pMilk, 800+visibleSize.width/2, 430);
            Sprite* pWater = Sprite::create(localPath("make2/milk1_2.png"));
            m_pMilk->addChild(pWater,1);
            pWater->setName("milk");
            pWater->setPosition(Vec2(-10, 30));
            m_pMilk->setTag(0);
            m_pMilk->setActionPoint(Vec2(15, 150));
            
            m_pGelatin = _createDrageNode(localPath("make1/gelatin_2.png"));
            this->addChildToContentLayer(m_pGelatin);
            CMVisibleRect::setPositionAdapted(m_pGelatin, 800+visibleSize.width/2, 205);
            Sprite* pGelatin = Sprite::create(localPath("make1/gelatin_3.png"));
            m_pGelatin->addChild(pGelatin,1);
            pGelatin->setName("gelatin");
            pGelatin->setPosition(Vec2(-35, 18));
            m_pGelatin->setTag(1);
            m_pGelatin->setActionPoint(Vec2(37, 110));
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            m_pMilk->setTouchEnabled(false);
            m_pMilk->runAction(action);
            
            m_pGelatin->setTouchEnabled(false);
            m_pGelatin->runAction(action->clone());
        }
            break;
        case 1:
        {
            m_pPowderJellew2 = _createDrageNode(localPath("make3/jelly powde.png"));
            this->addChildToContentLayer(m_pPowderJellew2);
            CMVisibleRect::setPositionAdapted(m_pPowderJellew2, 800+visibleSize.width/2, 430);
            m_pPowderJellew2->setTag(2);
            m_pPowderJellew2->setActionPoint(Vec2(140, 170));
            
            m_pSyrupCorn = _createDrageNode(localPath("make1/corn syrup_1.png"));
            this->addChildToContentLayer(m_pSyrupCorn);
            CMVisibleRect::setPositionAdapted(m_pSyrupCorn, 800+visibleSize.width/2, 205);
            m_pSyrupCorn->setTag(3);
            m_pSyrupCorn->setActionPoint(Vec2(10, 98));
            Sprite* pSyrup = Sprite::create(localPath("make1/corn syrup_3.png"));
            m_pSyrupCorn->addChild(pSyrup,1);
            pSyrup->setName("syrup");
            pSyrup->setPosition(Vec2(0, 8));
            
            auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                           CallFuncN::create([=](Node* pNode){
                DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
                if (pIngredient) {
                    pIngredient->setOrgPositionDefault();
                    pIngredient->setTouchEnabled(true);
                }
            }), NULL);
            
            m_pPowderJellew2->setTouchEnabled(false);
            m_pPowderJellew2->runAction(action);
            
            m_pSyrupCorn->setTouchEnabled(false);
            m_pSyrupCorn->runAction(action->clone());
        }
            break;
        default:
            break;
    }
    
}

void MakeGummyPizzaScene::_showPeiceWithCandy()
{
    _showTray();
    m_nStep = eStepAddJellewPiece;
    
    std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path = FileUtility::getStoragePath()+"candypiece.png";
#else
    path = FileUtility::getStoragePath()+"/candypiece.png";
#endif
    m_pGummyPan = Sprite::create(localPath("make2/plate_medium.png"));
    this->addChildToContentLayer(m_pGummyPan);
    CMVisibleRect::setPositionAdapted(m_pGummyPan,800+visibleSize.width/2, 300);
    m_pGummyPan->setScale(0.6);
    
    m_pGummyPiece = _createDrageNode(path);
    this->addChildToContentLayer(m_pGummyPiece);
    m_pGummyPiece->setTouchEnabled(false);
    m_pGummyPiece->setRotation(0);
    m_pGummyPiece->setScale(0.6);
    
    CMVisibleRect::setPositionAdapted(m_pGummyPiece,800+visibleSize.width/2, 300);
    
    m_pGummyPan->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)));
    m_pGummyPiece->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                         CallFunc::create([=](){
        m_pGummyPiece->setOrgPositionDefault();
        m_pGummyPiece->setTouchEnabled(true);
    }), NULL));
}

#pragma mark ===== common  =======
void MakeGummyPizzaScene::_showShadow(DragNode* pIngredient)
{
    if(pIngredient==m_pWater){
        pIngredient->getDragSprite()->setTexture(localPath("make1/water_2.png"));
    }else if (pIngredient == m_pGelatin) {
        pIngredient->getDragSprite()->setTexture(localPath("make1/gelatin_1.png"));
    }else if (pIngredient == m_pPowderJellew) {
    }else if (pIngredient == m_pSyrupCorn) {
        pIngredient->getDragSprite()->setTexture(localPath("make1/corn syrup_1.png"));
    }else if (pIngredient == m_pMilk) {
        pIngredient->getDragSprite()->setTexture(localPath("make2/milk1.png"));
    }else if (pIngredient == m_pVanilla) {
        pIngredient->getDragSprite()->setTexture(localPath("make2/vanilla extract.png"));
    }else if (pIngredient == m_pSugar) {
        pIngredient->getDragSprite()->setTexture(localPath("make2/sugar_2.png"));
    }
}

void MakeGummyPizzaScene::_hideShadow(DragNode* pIngredient)
{    
    if(pIngredient==m_pWater){
        pIngredient->getDragSprite()->setTexture(localPath("make1/water_1.png"));
    }else if (pIngredient == m_pGelatin) {
        pIngredient->getDragSprite()->setTexture(localPath("make1/gelatin_2.png"));
    }else if (pIngredient == m_pPowderJellew) {
    }else if (pIngredient == m_pSyrupCorn) {
        pIngredient->getDragSprite()->setTexture(localPath("make1/corn-syrup_2.png"));
    }else if (pIngredient == m_pMilk) {
        pIngredient->getDragSprite()->setTexture(localPath("make2/milk1_1.png"));
    }else if (pIngredient == m_pVanilla) {
        pIngredient->getDragSprite()->setTexture(localPath("make2/vanilla extract_h.png"));
    }else if (pIngredient == m_pSugar) {
        pIngredient->getDragSprite()->setTexture(localPath("make2/sugar_1.png"));
    }
}
void MakeGummyPizzaScene::_showTray()
{
    if (!m_pTray) {
        m_pTray = Sprite::create(localPath("pan.png"));
        this->addChildToContentLayer(m_pTray);
        m_pTray->setLocalZOrder(-1);
    }
    m_pTray->stopAllActions();
    CMVisibleRect::setPositionAdapted(m_pTray, 800+visibleSize.width/2, 300);
    m_pTray->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)));
}

void MakeGummyPizzaScene::_hideTray()
{
    m_pTray->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
}

void MakeGummyPizzaScene::_showSpoon()
{
    m_bSpoonReady = false;
    m_nMixCount = 0;
    if (!m_pSpoon) {
        m_pSpoon = _createDrageNode(localPath("spoon.png"));
        this->addChildToContentLayer(m_pSpoon);
    }else{
        m_pSpoon->setDragSprite(Sprite::create(localPath("spoon.png")));
    }
    m_pSpoon->stopAllActions();
    m_pSpoon->setTouchEnabled(false);
    m_pSpoon->setRotation(0);
    m_pSpoon->setLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height));
    
    CMVisibleRect::setPositionAdapted(m_pSpoon,800+visibleSize.width/2, 300);
    m_pSpoon->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                         CallFunc::create([=](){
        m_pSpoon->setTouchEnabled(true);
    }), NULL));
    
}

void MakeGummyPizzaScene::_hideSpoon()
{
    m_pSpoon->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)));
}


void MakeGummyPizzaScene::_showTableMat()
{
    if (!m_pTableMat) {
        m_pTableMat = Sprite::create(localPath("mat.png"));
        this->addChildToContentLayer(m_pTableMat);
    }
    m_pTableMat->setLocalZOrder(-1);
    m_pTableMat->setScale(1);
    CMVisibleRect::setPosition(m_pTableMat, 340-visibleSize.width, 260);
    std::string path = "";
    if (m_nStep == eStepMakeCandy) {
    }
    m_pTableMat->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
}

void MakeGummyPizzaScene::_hideTableMat()
{
    m_pTableMat->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
}

void MakeGummyPizzaScene::_showBowl()
{
    _showTableMat();
    m_pTableMat->setScale(1);
    if (!m_pBowl) {
        m_pBowl = _createDrageNode(localPath("bowl.png"),false);
        this->addChildToContentLayer(m_pBowl);
        m_pMixture = Node::create();
        m_pBowl->addChild(m_pMixture);
        m_pMixture->setPosition(Vec2(5, 5));
    }
    m_pBowl->setTouchEnabled(false);
    m_pBowl->setScale(1);
    m_pBowl->setLocalZOrder(0);
    m_pMixture->setRotation(0);
    m_pBowl->stopAllActions();
    CMVisibleRect::setPosition(m_pBowl, 320-visibleSize.width, 300);
    m_pBowl->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=](){
        
    }), NULL));
}

void MakeGummyPizzaScene::_showBowlFrontView()
{
    if (!m_pBowlFrontView) {
        if (m_nStep==eStepMakeCandy){
//            m_pTableMat->setScale(0.6);
            m_pBowlFrontView = _createDrageNode(localPath("make1/bowl_batter_red.png"));
        }else if (m_nStep==eStepMakeVanlia) {
//            m_pTableMat->setScale(0.8);
            m_pBowlFrontView = _createDrageNode(localPath("make2/bowl_batter_white.png"));
        }else if (m_nStep==eStepMakeJellew) {
//            m_pTableMat->setScale(1);
            m_pBowlFrontView = _createDrageNode(localPath("make3/bowl_batter_yellow.png"));
        }
        this->addChildToContentLayer(m_pBowlFrontView);
    }else{
        if (m_nStep==eStepMakeCandy){
            m_pBowlFrontView->setDragSprite(Sprite::create(localPath("make1/bowl_batter_red.png")));
        }else if (m_nStep==eStepMakeVanlia) {
            m_pBowlFrontView->setDragSprite(Sprite::create(localPath("make2/bowl_batter_white.png")));
        }else if (m_nStep==eStepMakeJellew) {
            m_pBowlFrontView->setDragSprite(Sprite::create(localPath("make3/bowl_batter_yellow.png")));
        }
    }
    m_pBowlFrontView->setPosition(m_pBowl->getPosition());
    m_pBowlFrontView->setOrgPositionDefault();
}

void MakeGummyPizzaScene::_showPan()
{
    _showTableMat();
    if (!m_pPan) {
        if (m_nStep==eStepMakeCandy){
            m_pPan = _createDrageNode(localPath("make1/plate_small.png"),false);
        }else if (m_nStep==eStepMakeVanlia) {
            m_pPan = _createDrageNode(localPath("make2/plate_medium.png"),false);
        }else if (m_nStep==eStepMakeJellew) {
            m_pPan = _createDrageNode(localPath("make3/plate_big.png"),false);
        }
        this->addChildToContentLayer(m_pPan);
        m_pMixturePan = Node::create();
        m_pMixturePan->setPosition(Vec2(-3, 10));
        m_pPan->addChild(m_pMixturePan);
    }else{
        if (m_nStep==eStepMakeCandy){
            m_pPan->setDragSprite(Sprite::create(localPath("make1/plate_small.png")));
        }else if (m_nStep==eStepMakeVanlia) {
            m_pPan->setDragSprite(Sprite::create(localPath("make2/plate_medium.png")));
        }else if (m_nStep==eStepMakeJellew) {
            m_pPan->setDragSprite(Sprite::create(localPath("make3/plate_big.png")));
        }
    }
    if (m_nStep==eStepMakeCandy){
        m_pTableMat->setScale(0.8);
    }else if (m_nStep==eStepMakeVanlia) {
        m_pTableMat->setScale(0.9);
    }else if (m_nStep==eStepMakeJellew) {
        m_pTableMat->setScale(1);
    }
    m_pMixturePan->removeAllChildren();
    m_pPan->stopAllActions();
    CMVisibleRect::setPosition(m_pPan, 320-visibleSize.width, 300);
    m_pPan->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=](){
        
    }), NULL));
    
}

void MakeGummyPizzaScene::_showPanFrontView()
{
    if (m_pPanFrontView) {
        m_pPanFrontView->removeFromParent();
        m_pPanFrontView = nullptr;
    }
    if (!m_pPanFrontView) {
        if (m_nStep==eStepMakeCandy){
            m_pPanFrontView = _createDrageNode(localPath("make1/piate1_down.png"));
        }else if (m_nStep==eStepMakeVanlia) {
            m_pPanFrontView = _createDrageNode(localPath("make2/plate_down.png"));
        }else if (m_nStep==eStepMakeJellew || m_nStep==eStepAddJellew || m_nStep==eStepAddJellewPiece) {
            m_pPanFrontView = _createDrageNode(localPath("make3/plate_down.png"));
        }
        this->addChildToContentLayer(m_pPanFrontView);
    }else{
        if (m_nStep==eStepMakeCandy){
            m_pPanFrontView->setDragSprite(Sprite::create(localPath("make1/piate1_down.png")));
        }else if (m_nStep==eStepMakeVanlia) {
            m_pPanFrontView->setDragSprite(Sprite::create(localPath("make2/plate_down.png")));
        }else if (m_nStep==eStepMakeJellew) {
            m_pPanFrontView->setDragSprite(Sprite::create(localPath("make3/plate_down.png")));
        }
    }
    
    m_pPanFrontView->setVisible(true);
    m_pPanFrontView->setPosition(m_pPan->getPosition());
    m_pPanFrontView->setOrgPositionDefault();
    
    Sprite* pMixture = nullptr;
    if (m_nStep==eStepMakeCandy){
        pMixture = Sprite::create(localPath("make1/piate1_mixture.png"));
    }else if (m_nStep==eStepMakeVanlia) {
        pMixture = Sprite::create(localPath("make2/plate_medium_mixture.png"));
    }else if (m_nStep==eStepMakeJellew) {
        pMixture = Sprite::create(localPath("make3/plate_big_mixture1.png"));
    }else if (m_nStep==eStepAddJellew) {
        std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        path = FileUtility::getStoragePath()+"candypiece.png";
#else
        path = FileUtility::getStoragePath()+"/candypiece.png";
#endif
        pMixture = Sprite::create(path);
        pMixture->setScale(1, 0.438);
    }else if (m_nStep==eStepAddJellewPiece) {
        std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        path = FileUtility::getStoragePath()+"gummypiece.png";
#else
        path = FileUtility::getStoragePath()+"/gummypiece.png";
#endif
        pMixture = Sprite::create(path);
        pMixture->setScale(0.83, 0.38);
    }
    m_pPanFrontView->addChild(pMixture);
    pMixture->setName("mixture");
    
    if (m_nStep==eStepMakeCandy){
        m_pMixturePanFrontView = Sprite::create(localPath("make1/piate1_on.png"));
    }else if (m_nStep==eStepMakeVanlia) {
        m_pMixturePanFrontView = Sprite::create(localPath("make2/piate_on.png"));
    }else if (m_nStep==eStepMakeJellew || m_nStep==eStepAddJellew || m_nStep==eStepAddJellewPiece)  {
        m_pMixturePanFrontView = Sprite::create(localPath("make3/piate_on.png"));
    }
    m_pPanFrontView->addChild(m_pMixturePanFrontView);
}

void MakeGummyPizzaScene::_resetBowl()
{
    m_pMixture->removeAllChildren();
    m_pMixtureFrontView->removeAllChildren();
}

void MakeGummyPizzaScene::_resetPan()
{
    m_pMixturePan->removeAllChildren();
}

void MakeGummyPizzaScene::_showOilBottle()
{
    AudioHelp::getInstance()->playEffect("vo_spray_oil.mp3");
    m_pOilBottle = _createDrageNode(localPath("make1/oil.png"));
    this->addChildToContentLayer(m_pOilBottle);
    CMVisibleRect::setPositionAdapted(m_pOilBottle, 800+visibleSize.width/2, 300);
    m_pOilBottle->setActionPoint(Vec2(20, 250));
    
    Sprite* pStick = Sprite::create(localPath("make1/oil_2.png"));
    m_pOilBottle->addChild(pStick,-2);
    pStick->setPosition(Vec2(-52, 81));
    
    Sprite* pHandle = Sprite::create(localPath("make1/oil_3.png"));
    m_pOilBottle->addChild(pHandle,-1);
    pHandle->setAnchorPoint(Vec2(0.7, 1));
    pHandle->setPosition(Vec2(-36, 111));
    pHandle->setName("handle");
    
    auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pOilBottle->setTouchEnabled(false);
    m_pOilBottle->runAction(action);
}

void MakeGummyPizzaScene::_moveBowlLeft()
{
    if (m_nStep==eStepMakeCandy){
        AudioHelp::getInstance()->playEffect("vo_put_into_microwave.mp3");
    }else {
        AudioHelp::getInstance()->playEffect("vo_second_time_micorowave.mp3");
    }
    float dt = 1;
    m_pBg->runAction(MoveBy::create(dt, Vec2(350, -350)));
    m_pTableMat->runAction(Spawn::create(MoveBy::create(dt*0.5, Vec2(280, -180)),
                                         ScaleTo::create(dt*0.5, 0.85,0.2), NULL));
    m_pBowl->runAction(Sequence::create(Spawn::create(MoveBy::create(dt*0.5, Vec2(280, -150)),
                                                      RotateBy::create(dt*0.5, Vec3(-60,0,0)),
                                                      ScaleTo::create(dt*0.5,0.8,0.6), NULL),
                                        CallFunc::create([=](){
        _showBowlFrontView();
        m_pBowl->setVisible(false);
        m_pBowl->setRotation3D(Vec3(0, 0, 0));
        m_pBowl->setScale(1);
    }),
                                        DelayTime::create(1),
                                        CallFunc::create([=](){
//        Rect rect = m_pOven->getDoorWorldRect();
//        m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMidY()));
//        m_pOven->setTouchEnable(true);
        m_pOven->openDoor();
    }), NULL));
    m_pOven->runAction(MoveBy::create(dt, Vec2(350, -350)));
    m_pFreezer->runAction(MoveBy::create(dt, Vec2(350, -350)));
}

void MakeGummyPizzaScene::_moveBowlMiddle()
{
    float dt = 1;
    m_pTableMat->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFrontView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pBg->runAction(MoveBy::create(dt, Vec2(-350, 350)));
        m_pOven->runAction(MoveBy::create(dt, Vec2(-350, 350)));
        m_pFreezer->runAction(MoveBy::create(dt, Vec2(-350, 350)));
        m_pOven->reset();
    }),
                                     DelayTime::create(dt),
                                     CallFunc::create([=](){
        _showPan();
    }),
                                     DelayTime::create(dt),
                                     CallFunc::create([=](){
        _showTray();
        _showOilBottle();
    }), NULL));
    
}

void MakeGummyPizzaScene::_movePanMiddle()
{
    float dt = 1;
    m_pPan->setScale(1,0.438);
    m_pPanFrontView->setVisible(false);
    m_pPan->setVisible(true);
    m_pMixturePan->setVisible(true);
    m_pPanFrontView->setScale(1);
    m_pPanFrontView->setRotation3D(Vec3(0, 0, 0));
    float scale = 1;
    if (m_nStep==eStepMakeCandy){
        scale = 0.8;
    }else if (m_nStep==eStepMakeVanlia) {
        scale = 0.9;
    }else if (m_nStep==eStepMakeJellew) {
        scale = 1;
    }
    m_pTableMat->runAction(Spawn::create(MoveBy::create(dt, Vec2(-280, 150)),
                                         ScaleTo::create(dt, scale), NULL));
    m_pPan->runAction(Sequence::create(Spawn::create(ScaleTo::create(dt, 1),
                                                     MoveBy::create(dt, Vec2(-280, 150)), NULL), NULL));
    m_pBg->runAction(MoveBy::create(dt, Vec2(350, 350)));
    m_pOven->runAction(MoveBy::create(dt, Vec2(350, 350)));
    m_pFreezer->runAction(MoveBy::create(dt, Vec2(350, 350)));
//    m_pFreezer->reset();
}

void MakeGummyPizzaScene::_movePanRight()
{
    if (m_nStep==eStepMakeCandy){
        AudioHelp::getInstance()->playEffect("vo_put_into_refrigerator.mp3");
    }else if (m_nStep!=eStepAddJellewPiece){
        AudioHelp::getInstance()->playEffect("vo_second_time_refrigerator.mp3");
    }
    float dt = 1;
    float scale = 1;
    if (m_nStep==eStepMakeCandy){
        scale = 0.8;
    }else if (m_nStep==eStepMakeVanlia) {
        scale = 0.9;
    }else if (m_nStep==eStepMakeJellew) {
        scale = 1;
    }
    m_pBg->runAction(MoveBy::create(dt, Vec2(-350, -350)));
    m_pTableMat->runAction(Spawn::create(MoveBy::create(dt, Vec2(280, -150)),
                                         ScaleTo::create(dt, scale,0.305*scale), NULL));
    
    m_pPan->runAction(Sequence::create(Spawn::create(MoveBy::create(dt, Vec2(280, -150)),
                                                     ScaleTo::create(dt,1,0.438), NULL),
                                       CallFunc::create([=](){
        _showPanFrontView();
        m_pPan->setVisible(false);
        m_pMixturePan->setVisible(false);
        m_pPan->setRotation3D(Vec3(0, 0, 0));
        m_pPan->setScale(1);
        m_pFreezer->openDoor();
//        Rect rect = m_pFreezer->getDoorWorldRect();
//        m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMidY()));
    }), NULL));
    m_pOven->runAction(MoveBy::create(dt, Vec2(-350, -350)));
    m_pFreezer->runAction(MoveBy::create(dt, Vec2(-350, -350)));
    
}

#pragma mark ===== deel ==========
void MakeGummyPizzaScene::_addCandyIngredient(DragNode *pIngredient)
{
    std::string lNameStr = "";
    float dt = 1;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setLocalZOrder(5);
#pragma mark  ===== add water ======
    if(pIngredient==m_pWater){
        lNameStr = "make1/bowl_water.png";
        pIngredient->setTouchEnabled(false);
        
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+200));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                CallFunc::create([=](){
            AudioHelp::getInstance()->playPourMilkEffect();
            
            Node* pIn = pIngredient->getChildByName("water");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.4),
                                                              MoveBy::create(0.8, Vec2(-70, 0)), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
            };
            
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
        }),
                                                DelayTime::create(2),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            pIngredient->back();
        }),
                                                NULL));

#pragma mark  ===== add Gelatin ======
    }else if (pIngredient == m_pGelatin) {
        lNameStr = "make1/bowl_gelatin.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+180, m_pBowl->getPositionY()+150));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                                CallFunc::create([=](){
            Node* pIn = pIngredient->getChildByName("gelatin");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.4),
                                                              MoveBy::create(0.8, Vec2(-40, 0)), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
            };
            AudioHelp::getInstance()->playPourPowderEffect();
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
            pParticle->setLocalZOrder(6);
        }),
                                                ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            
            pIngredient->back();
            _finishMakeCandyAdd(0);
        }),
                                                NULL));

#pragma mark  ===== add Powder Jellew ======
    }else if (pIngredient == m_pPowderJellew) {
        lNameStr = "make1/bowl_jelly powder.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+200));
        pIngredient->setDragSprite(Sprite::create(localPath("make1/jelly powde1.png")));
        pIngredient->setActionPoint(Vec2(26, 70));
        
        AudioHelp::getInstance()->playPourPowderEffect();
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/jellew_red.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
        pParticle->setLocalZOrder(6);
        pIngredient->runAction(Sequence::create(ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateBy::create(0.3, 120),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            
            pIngredient->back();
        }),
                                                NULL));
#pragma mark  ===== add Syrup corn ======
    }else if (pIngredient == m_pSyrupCorn) {
        lNameStr = "make1/bowl_corn syrup.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+150, m_pBowl->getPositionY()+250));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                                CallFunc::create([=](){
            AudioHelp::getInstance()->playPourPowderEffect();
            
            Node* pIn = pIngredient->getChildByName("syrup");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(MoveBy::create(0.4, Vec2(-200, 0)),
                                                              MoveBy::create(0.6, Vec2(0, -50)),
                                                              ScaleTo::create(0.4,0.6 , 1), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
//                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2, 0.8),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.9),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.6),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.7),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.4),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                FadeOut::create(0.2),
//                                                CallFunc::create([=](){
//                    pIn->removeFromParent();
//                }), NULL));
            };
        }),
                                                ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            pIngredient->back();
            _finishMakeCandyAdd(1);
        }),
                                                NULL));
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

void MakeGummyPizzaScene::_mix(int step)
{
    if (m_bSpoonReady){
        m_nMixCount++;
        if (m_nMixCount%50==0) {
            int mixtureStep = m_nMixCount/50;
            int count = 5;
            std::string strEX = "";
            if (m_nStep==eStepMakeCandy) {
                count = 5;
                strEX = "make1/mixture";
            }else if (m_nStep==eStepMakeVanlia){
                count = 6;
                strEX = "make2/mixture1_";
            }else if (m_nStep==eStepMakeJellew){
                count = 5;
                strEX = "make3/mixture2_";
            }
            if (mixtureStep<=count){
                std::stringstream ostr;
                ostr<<strEX<<mixtureStep<<".png";
                Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                m_pMixture->removeAllChildren();
                m_pMixture->addChild(pMixture);
                
            }
            if (mixtureStep>=count){
                //mix finish
                m_pMixture->stopAllActions();
                _finishMix(step);
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }
    }else{
        m_bSpoonReady = true;
        AudioHelp::getInstance()->playLoopEffect("stir.mp3");
        m_pSpoon->setLocalZOrder(5);
        Rect rect = m_pBowl->getBoundingBox();
        Rect limitRect = Rect(rect.origin.x+140, rect.origin.y-10, rect.size.width-240, rect.size.height-180);
        m_pSpoon->setLimitRect(limitRect);
        m_pSpoon->setOrgPositionDefault();
        
        m_pSpoon->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()));
        m_pSpoon->runAction(Sequence::create(RotateBy::create(0.3, -130),
                                             CallFunc::create([=](){
            m_pSpoon->setRotation(0);
            m_pSpoon->setDragSprite(Sprite::create(localPath("spoon_1.png")));
        }), NULL));
        m_pSpoon->setActionPoint(Vec2(23, 45));
        m_pMixture->runAction(RepeatForever::create(RotateBy::create(1, 360)));
        m_pBowl->runAction(MoveBy::create(0.5, Vec2(160, 0)));
        m_pTableMat->runAction(MoveBy::create(0.5, Vec2(160, 0)));
        m_pSpoon->runAction(Sequence::create(MoveBy::create(0.5, Vec2(160, 0)),
                                             CallFunc::create([=](){
            m_pSpoon->setPosition(m_pBowl->getPosition());
        }), NULL));
        _hideTray();

    }
}

void MakeGummyPizzaScene::_putinOven()
{
    m_pBowlFrontView->setTouchEnabled(false);
    m_pBowlFrontView->setVisible(false);
    m_pBowlFrontView->setOrgPositionDefault();
    Sprite* pIn = nullptr;
    if (m_nStep==eStepMakeCandy){
        pIn = Sprite::create(localPath("make1/bowl_batter_red.png"));
    }else if (m_nStep==eStepMakeVanlia) {
        pIn = Sprite::create(localPath("make2/bowl_batter_white.png"));
    }else if (m_nStep==eStepMakeJellew) {
        pIn = Sprite::create(localPath("make3/bowl_batter_yellow.png"));
    }
    m_pOven->addChild(pIn);
    pIn->setName("ovenin");
    pIn->setPosition(Vec2(-30, 30));
    pIn->setScale(0.7);
    Vec2 pos = m_pOven->convertToWorldSpace(pIn->getPosition());
    m_pBowlFrontView->setPosition(m_pBowl->getParent()->convertToNodeSpace(pos));
    m_pBowlFrontView->setScale(0.7);
    m_pOven->setTouchEnable(false);
    
    //后面把烤箱和冰箱门改成了自动开关
    m_pOven->closeDoor();
    
}

void MakeGummyPizzaScene::_putinFreezer()
{
//    m_pFreezer->setTouchEnable(true);
    m_pPanFrontView->setTouchEnabled(false);
    m_pPanFrontView->setVisible(false);
    m_pPanFrontView->setOrgPositionDefault();
    Sprite* pIn = nullptr;
    if (m_nStep==eStepMakeCandy){
        pIn = Sprite::create(localPath("make1/piate1_down0.png"));
    }else if (m_nStep==eStepMakeVanlia) {
        pIn = Sprite::create(localPath("make2/plate_down0.png"));
    }else if (m_nStep==eStepMakeJellew) {
        pIn = Sprite::create(localPath("make3/plate_down0.png"));
    }else if (m_nStep==eStepAddJellew) {
        std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        path = FileUtility::getStoragePath()+"candypiece.png";
#else
        path = FileUtility::getStoragePath()+"/candypiece.png";
#endif
        pIn= Sprite::create(localPath("make2/plate_down.png"));
        
        Sprite* mixture = Sprite::create(path);
        mixture->setScale(1, 0.438);
        pIn->addChild(mixture);
        mixture->setPosition(pIn->getContentSize()*0.5);
        
        Sprite* panFront = Sprite::create(localPath("make2/piate_on.png"));
        pIn->addChild(panFront);
        panFront->setPosition(pIn->getContentSize()*0.5);
        
    }else if (m_nStep==eStepAddJellewPiece) {
        std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        path = FileUtility::getStoragePath()+"gummypiece.png";
#else
        path = FileUtility::getStoragePath()+"/gummypiece.png";
#endif
        pIn= Sprite::create(localPath("make3/plate_down.png"));
        
        Sprite* mixture = Sprite::create(path);
        mixture->setScale(0.83, 0.38);
        pIn->addChild(mixture);
        mixture->setPosition(pIn->getContentSize()*0.5);
        
        Sprite* panFront = Sprite::create(localPath("make3/piate_on.png"));
        pIn->addChild(panFront);
        panFront->setPosition(pIn->getContentSize()*0.5);
    }
    m_pFreezer->addChild(pIn);
    pIn->setName("freezerin");
    pIn->setPosition(Vec2(150, 10));
    pIn->setScale(0.7);
    Vec2 pos = m_pFreezer->convertToWorldSpace(pIn->getPosition());
    m_pPanFrontView->setPosition(m_pBowl->getParent()->convertToNodeSpace(pos));
    m_pPanFrontView->setScale(0.7);
//    Rect rect = m_pFreezer->getDoorTouchWorldRect();
//    m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMidY()));
    {
        m_pFreezer->closeDoor();
        m_pFreezer->setTouchEnable(false);
//        Node* pIn = m_pFreezer->getChildByName("freezerin");
//        if (pIn) {
//            m_pPanFrontView->setTouchEnabled(false);
//            
//            m_pFreezer->setTouchEnable(false);
//            _freeze();
//        }
    }
}

void MakeGummyPizzaScene::_putoutOven()
{
    m_pOven->setTouchEnable(false);
    m_pOven->closeDoor();
    m_pBowlFrontView->setTouchEnabled(false);
    m_pBowlFrontView->setPosition(m_pBowl->getPosition());
    m_pBowlFrontView->setOrgPositionDefault();
    _moveBowlMiddle();
    
}

void MakeGummyPizzaScene::_putoutFreezer()
{
    m_pFreezer->setTouchEnable(false);
    m_pFreezer->closeDoor();
    m_pPanFrontView->setTouchEnabled(false);
    m_pPanFrontView->setPosition(m_pPan->getPosition());
    
    m_pPanFrontView->setOrgPositionDefault();
    if (m_nStep==eStepMakeCandy) {
        m_pMixturePan->removeAllChildren();
        Sprite* pMixture = Sprite::create(localPath("make1/plate_pizza_red.png"));
        m_pMixturePan->addChild(pMixture);
    }else if (m_nStep==eStepMakeJellew) {
        m_pMixturePan->removeAllChildren();
        Sprite* pMixture = Sprite::create(localPath("make3/plate_pizza.png"));
        m_pMixturePan->addChild(pMixture);
    }
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _movePanMiddle();
        m_pFreezer->reset();
        m_pFreezer->setTouchEnable(false);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        if (m_nStep==eStepMakeCandy){
            _showTray();
            _showRing();
        }else if (m_nStep==eStepMakeVanlia) {
            AudioHelp::getInstance()->playEffect("vo_drag_candy_dec_gummy_pizza.mp3");
            _showAddCandyIngredient(0);
        }else if (m_nStep==eStepMakeJellew) {
            _showPeiceWithCandy();
        }else if (m_nStep == eStepAddJellew) {
            _finishFreezeGummyPiece(0);
        }else if (m_nStep == eStepAddJellewPiece) {
            _finishFreezeGummyPiece(1);
        }
    }), NULL));
    
}

void MakeGummyPizzaScene::_freeze()
{
    m_pFreezer->doFreeze();
    this->runAction(Sequence::create(DelayTime::create(5),
                                     CallFunc::create([=](){
        _finishFreeze();
    }), NULL));
}

#pragma mark  ===== add oil ======
void MakeGummyPizzaScene::_addOil()
{
    Sprite* pOil;
    if (m_nStep==eStepMakeCandy){
        pOil = Sprite::create(localPath("make1/plate_small_oil.png"));
    }else if (m_nStep==eStepMakeVanlia) {
        pOil = Sprite::create(localPath("make2/plate_medium_oil.png"));
    }else if (m_nStep==eStepMakeJellew) {
        pOil = Sprite::create(localPath("make3/plate_big_oil.png"));
    }
    m_pMixturePan->addChild(pOil);
    pOil->setOpacity(0);
    pOil->runAction(FadeIn::create(2));
    m_pOilBottle->setTouchEnabled(false);
    m_pOilBottle->setPosition(Vec2(m_pPan->getPositionX()+150, m_pPan->getPositionY()+150));
    m_pOilBottle->runAction(Sequence::create(RotateBy::create(0.3, -60),
                                             CallFunc::create([=](){
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/oil.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pOilBottle->getWorldSpaceActionPoint()));
        pParticle->setLocalZOrder(6);
        
        Node* pHandle = m_pOilBottle->getChildByName("handle");
        if (pHandle) {
            pHandle->runAction(Repeat::create(Sequence::create(RotateBy::create(0.2, -40),
                                                               RotateBy::create(0.2, 40),NULL), 4));
        }
    }),
                                             DelayTime::create(2),
                                             RotateTo::create(0.3, 0),
                                             CallFunc::create([=](){
        _finishAddOil();
    }), NULL));
}

void MakeGummyPizzaScene::_addBatterPan()
{
    m_pBowl->setPosition(Vec2(m_pPan->getPositionX()+100, m_pPan->getPositionY()+150));
    
    Sprite* pPour = nullptr;
    std::string strEx = "";
    Vec2 offset = Vec2(30, 20);;
    if (m_nStep==eStepMakeCandy){
        pPour =Sprite::create(localPath("make1/pou0.png"));
        strEx = localPath("make1/pou");
        
        m_pMixture->runAction(MoveBy::create(0.9, Vec2(-45, -90)));
        m_pMixture->runAction(ScaleTo::create(0.8, 0.6));
    }else if (m_nStep==eStepMakeVanlia) {
        pPour =Sprite::create(localPath("make2/pou0.png"));
        strEx = localPath("make2/pou");
        
        m_pMixture->runAction(MoveBy::create(0.9, Vec2(-40, -80)));
        m_pMixture->runAction(ScaleTo::create(0.8, 0.6));
    }else if (m_nStep==eStepMakeJellew) {
        pPour =Sprite::create(localPath("make3/pou0.png"));
        strEx = localPath("make3/pou");
        
        m_pMixture->runAction(MoveBy::create(0.9, Vec2(-40, -75)));
        m_pMixture->runAction(ScaleTo::create(0.8, 0.6));
    }
    this->addChildToUILayer(pPour);
    pPour->setPosition(m_pContentLayer->convertToNodeSpace(m_pBowl->getWorldSpaceActionPoint()+offset));
    pPour->runAction(RepeatForever::create(Sequence::create(AnimationHelp::createAnimate(strEx, 0, 3,false), NULL)));
    pPour->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    pPour->setLocalZOrder(20);
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=](){
        m_pMixture->removeAllChildren();
        m_pMixture->setScale(1);
        m_pMixture->setPosition(Vec2::ZERO);
        pPour->removeFromParent();
        _finishAddBatter();
    }), NULL));;
    
    Sprite* pBatter = nullptr;
    if (m_nStep==eStepMakeCandy){
        pBatter = Sprite::create(localPath("make1/plate_mixture.png"));
    }else if (m_nStep==eStepMakeVanlia) {
        pBatter = Sprite::create(localPath("make2/plate_medium_white.png"));
    }else if (m_nStep==eStepMakeJellew) {
        pBatter = Sprite::create(localPath("make3/plate_mixture.png"));
    }
    m_pMixturePan->addChild(pBatter);
    pBatter->setName("mixture");
    pBatter->setScale(0.4);
    pBatter->setOpacity(0);
    pBatter->runAction(FadeIn::create(0.2));
    pBatter->runAction(ScaleTo::create(1, 1));
}

void MakeGummyPizzaScene::_cutJellew(int type)
{
    AudioHelp::getInstance()->playEffect("cut.mp3");
    m_pKnife->runAction(Sequence::create(MoveBy::create(0.3, Vec2(0, -30)),
                                         MoveBy::create(0.1, Vec2(0, 30)), NULL));
    int startNum = m_nJellewCutCount+1;
    int count = 8;
    if (type==0) {
        m_pJellewBoard->getDragSprite()->runAction(AnimationHelp::createAnimate(localPath("make1/cut/gummy_green_big"), startNum, startNum+1,false));
    }else{
        m_pJellewBoard->getDragSprite()->runAction(AnimationHelp::createAnimate(localPath("make1/cut/gummy_yellow_"), startNum+1, startNum+2,false));
        count = 6;
    }
    
    m_nJellewCutCount+=2;
    if (m_nJellewCutCount>=count) {
//        finish
        _finishCutJellew(type);
        m_pKnife->setTouchEnabled(false);
    }
}

void MakeGummyPizzaScene::_putJellew(int type)
{
    m_pJellewBoard->setTouchEnabled(false);
    m_pJellewBoard->setVisible(false);
    m_pJellewBoard->back();
    if(type==0){
        int count = m_JellewGreens.size();
        std::stringstream ostr;
        ostr<<"make1/cut/gummy_green"<<3-count<<".png";
        Sprite* pJellew = Sprite::create(localPath(ostr.str()));
        m_pJellewGreenBlock->addChild(pJellew);
        
        m_nJellewCutCount = 0;
        if (count ==0) {
            _finishPutJellew(type);
        }
    }else{
        int count = m_JellewYellows.size();
        std::stringstream ostr;
        ostr<<"make1/cut/gummy_yellow"<<3-count<<".png";
        Sprite* pJellew = Sprite::create(localPath(ostr.str()));
        m_pJellewYellowBlock->addChild(pJellew);
        
        m_nJellewCutCount = 0;
        if (count ==0) {
            _finishPutJellew(type);
        }
    }
}

void MakeGummyPizzaScene::_addVanlliaIngredient(DragNode *pIngredient)
{
    std::string lNameStr = "";
    float dt = 1;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setLocalZOrder(5);
#pragma mark  ===== add milk ======
    if(pIngredient==m_pMilk){
        lNameStr = "make2/batter_white.png";
        pIngredient->setTouchEnabled(false);
        
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+250));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                CallFunc::create([=](){
            AudioHelp::getInstance()->playPourMilkEffect();
            
            Node* pIn = pIngredient->getChildByName("milk");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.4),
                                                              MoveBy::create(0.8, Vec2(-70, 0)), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
            };
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/milk.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setScaleX(-1);
            pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
            pParticle->setLocalZOrder(6);
        }),
                                                DelayTime::create(2),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            pIngredient->back();
        }),
                                                NULL));
        
#pragma mark  ===== add Gelatin ======
    }else if (pIngredient == m_pGelatin) {
        lNameStr = "make2/bowl_gelatin.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+140, m_pBowl->getPositionY()+150));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                                CallFunc::create([=](){
            Node* pIn = pIngredient->getChildByName("gelatin");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.4),
                                                              MoveBy::create(0.8, Vec2(-40, 0)), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
            };
            AudioHelp::getInstance()->playPourPowderEffect();
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
            pParticle->setLocalZOrder(6);
        }),
                                                ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            
            pIngredient->back();
            _finishMakeVanillaAdd(0);
        }),
                                                NULL));
        
#pragma mark  ===== add Sugar ======
    }else if (pIngredient == m_pSugar) {
        lNameStr = "make2/bowl_sugar.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+150, m_pBowl->getPositionY()+250));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                                CallFunc::create([=](){
            AudioHelp::getInstance()->playPourPowderEffect();
            
            Node* pIn = pIngredient->getChildByName("sugar");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2, 0.8),
                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
                                                Spawn::create(ScaleTo::create(0.2, 0.9),
                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
                                                Spawn::create(ScaleTo::create(0.2, 0.6),
                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
                                                Spawn::create(ScaleTo::create(0.2, 0.7),
                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
                                                Spawn::create(ScaleTo::create(0.2, 0.4),
                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
            };
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
            pParticle->setLocalZOrder(6);
        }),
                                                ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            pIngredient->back();
        }),
                                                NULL));
#pragma mark  ===== add vanilla ======
    }else if (pIngredient == m_pVanilla) {
        lNameStr = "make2/bowl_vanilla extract.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()-50, m_pBowl->getPositionY()+250));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                                CallFunc::create([=](){
            AudioHelp::getInstance()->playPourPowderEffect();
            
            Node* pIn = pIngredient->getChildByName("vanilla");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(-100, 0)),
                                                              MoveBy::create(1, Vec2(0, -100)),
                                                              FadeOut::create(1), NULL),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
            }
//            if (pIn) {
//                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2, 0.8),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.9),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.6),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.7),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.4),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                FadeOut::create(0.2),
//                                                CallFunc::create([=](){
//                    pIn->removeFromParent();
//                }), NULL));
//            }
            
        }),
                                                ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            pIngredient->back();
            _finishMakeVanillaAdd(1);
        }),
                                                NULL));
#pragma mark  ===== add Syrup corn ======
    }else if (pIngredient == m_pSyrupCorn) {
        lNameStr = "make1/bowl_corn syrup.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+150, m_pBowl->getPositionY()+250));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                                CallFunc::create([=](){
            AudioHelp::getInstance()->playPourPowderEffect();
            
            Node* pIn = pIngredient->getChildByName("syrup");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(MoveBy::create(0.4, Vec2(-200, 0)),
                                                              MoveBy::create(0.6, Vec2(0, -50)),
                                                              ScaleTo::create(0.4,0.6 , 1), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
//            Node* pIn = pIngredient->getChildByName("syrup");
//            if (pIn) {
//                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2, 0.8),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.9),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.6),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.7),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.4),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                FadeOut::create(0.2),
//                                                CallFunc::create([=](){
//                    pIn->removeFromParent();
//                }), NULL));
            };
            
//            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
//            this->addChildToContentLayer(pParticle);
//            pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
//            pParticle->setLocalZOrder(6);
        }),
                                                ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            pIngredient->back();
            _finishMakeVanillaAdd(2);
        }),
                                                NULL));
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


void MakeGummyPizzaScene::_saveMixture()
{
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    Vec2 pos = m_pMixturePan->getPosition();
    m_pMixturePan->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    
    m_pMixturePan->visit();
    render->end();
    m_pMixturePan->setPosition(pos);
    
    m_pContentLayer->setPosition(Vec2(0, 0));
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    if (m_nStep==eStepAddJellew) {
        bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"candypiece.png", false);
        TextureCache::getInstance()->removeTextureForKey(FileUtility::getStoragePath()+"candypiece.png");
#else
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"/candypiece.png", false);
        TextureCache::getInstance()->removeTextureForKey(FileUtility::getStoragePath()+"/candypiece.png");
#endif
    }else if (m_nStep == eStepAddJellewPiece) {
        bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"gummypiece.png", false);
        TextureCache::getInstance()->removeTextureForKey(FileUtility::getStoragePath()+"gummypiece.png");
#else
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"/gummypiece.png", false);
        TextureCache::getInstance()->removeTextureForKey(FileUtility::getStoragePath()+"/gummypiece.png");
#endif
    }
    pImage->autorelease();
}

void MakeGummyPizzaScene::_addJewllewPieceIngredient(DragNode* pIngredient)
{
    std::string lNameStr = "";
    float dt = 1;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setLocalZOrder(5);
#pragma mark  ===== add milk ======
    if(pIngredient==m_pMilk){
        lNameStr = "make3/bowl_milk.png";
        pIngredient->setTouchEnabled(false);
        
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+250));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                CallFunc::create([=](){
            AudioHelp::getInstance()->playPourMilkEffect();
            
            Node* pIn = pIngredient->getChildByName("milk");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.4),
                                                              MoveBy::create(0.8, Vec2(-70, 0)), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
            };
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/milk.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
            pParticle->setLocalZOrder(6);
            pParticle->setScaleX(-1);
        }),
                                                DelayTime::create(2),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            pIngredient->back();
        }),
                                                NULL));
#pragma mark  ===== add Gelatin ======
    }else if (pIngredient == m_pGelatin) {
        lNameStr = "make3/bowl_gelatin.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+150));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                                CallFunc::create([=](){
            Node* pIn = pIngredient->getChildByName("gelatin");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.4),
                                                              MoveBy::create(0.8, Vec2(-40, 0)), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
            };
            AudioHelp::getInstance()->playPourPowderEffect();
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
            pParticle->setLocalZOrder(6);
        }),
                                                ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            
            pIngredient->back();
            _finishMakeJellewPieceAdd(0);
        }),
                                                NULL));
        
#pragma mark  ===== add Powder Jellew ======
    }else if (pIngredient == m_pPowderJellew2) {
        lNameStr = "make3/bowl_jelly powder.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+200));
        pIngredient->setDragSprite(Sprite::create(localPath("make3/jelly powde1.png")));
        pIngredient->setActionPoint(Vec2(26, 70));
        
        AudioHelp::getInstance()->playPourPowderEffect();
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/jellew_yellow.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
        pParticle->setLocalZOrder(6);
        pIngredient->runAction(Sequence::create(ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateBy::create(0.3, 120),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            
            pIngredient->back();
        }),
                                                NULL));
#pragma mark  ===== add Syrup corn ======
    }else if (pIngredient == m_pSyrupCorn) {
        lNameStr = "make3/bowl_corn syrup.png";
        pIngredient->setTouchEnabled(false);
        pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+150, m_pBowl->getPositionY()+250));
        pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -50),
                                                CallFunc::create([=](){
            AudioHelp::getInstance()->playPourPowderEffect();
            
            Node* pIn = pIngredient->getChildByName("syrup");
            if (pIn) {
                pIn->runAction(Sequence::create(Spawn::create(MoveBy::create(0.4, Vec2(-200, 0)),
                                                              MoveBy::create(0.6, Vec2(0, -50)),
                                                              ScaleTo::create(0.4,0.6 , 1), NULL),
                                                FadeOut::create(0.2),
                                                CallFunc::create([=](){
                    pIn->removeFromParent();
                }), NULL));
//            Node* pIn = pIngredient->getChildByName("syrup");
//            if (pIn) {
//                pIn->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2, 0.8),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.9),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.6),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.7),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                Spawn::create(ScaleTo::create(0.2, 0.4),
//                                                              MoveBy::create(0.2, Vec2(-5, -5)), NULL),
//                                                FadeOut::create(0.2),
//                                                CallFunc::create([=](){
//                    pIn->removeFromParent();
//                }), NULL));
            };
            
//            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
//            this->addChildToContentLayer(pParticle);
//            pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
//            pParticle->setLocalZOrder(6);
        }),
                                                ScaleTo::create(0.5, 1.1,0.9),
                                                DelayTime::create(1),
                                                ScaleTo::create(0.5, 1),
                                                RotateTo::create(0.3, 0),
                                                CallFunc::create([=](){
            m_nCandyAddCount++;
            pIngredient->back();
            _finishMakeJellewPieceAdd(1);
        }),
                                                NULL));
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


void MakeGummyPizzaScene::_addGummyPiece()
{
    m_pGummyPiece->setTouchEnabled(false);
    m_pGummyPiece->setVisible(false);
    
    Sprite* pIn = Sprite::createWithTexture(m_pGummyPiece->getDragSprite()->getTexture());
    m_pMixturePan->addChild(pIn);
    _finishAddGummyPiece();
}

#pragma mark =========finish ====
void MakeGummyPizzaScene::_finishMakeCandyAdd(int step)
{
    if (step==0) {
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            m_pWater->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
            m_pGelatin->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _showCandyIngredient(1);
        }), NULL));
    }else if (step==1){
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            m_pPowderJellew->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
            m_pSyrupCorn->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _showSpoon();
            _showTray();
        }), NULL));
    }
}

void MakeGummyPizzaScene::_finishMix(int step)
{
    m_pOven->setBaked(false);
    m_pSpoon->setTouchEnabled(false);
    m_pSpoon->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    if (step==eStepMakeCandy || step==eStepMakeVanlia || step==eStepMakeJellew) {
        _moveBowlLeft();
    }
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
}


void MakeGummyPizzaScene::_finishBake()
{
//    m_pOven->setTouchEnable(true);
//    Rect rect = m_pOven->getDoorWorldRect();
//    m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMidY()));
    
    m_pOven->openDoor();
    
//    Node* pIn = m_pOven->getChildByName("ovenin");
//    if (pIn) {
//        pIn->removeFromParent();
//    }
//    m_pBowlFrontView->setVisible(true);
//    m_pBowlFrontView->setTouchEnabled(true);
//    m_pGuideLayer->showGuideMove(Vec2(m_pOven->getDoorWorldRect().getMidX(), m_pOven->getDoorWorldRect().getMidY()),m_pBowl->getPosition());
}

void MakeGummyPizzaScene::_finishFreeze()
{
    Sprite* pMixture = (Sprite*)m_pPanFrontView->getChildByName("mixture");
    if (pMixture) {
        if (m_nStep==eStepMakeCandy){
            pMixture->setTexture(localPath("make1/piate1_down1.png"));
        }else if (m_nStep==eStepMakeVanlia) {
            pMixture->setTexture(localPath("make2/plate_down1.png"));
        }else if (m_nStep==eStepMakeJellew) {
            pMixture->setTexture(localPath("make3/plate_down1.png"));
        }
    }
    m_pFreezer->stopFreeze();
//    m_pFreezer->setTouchEnable(true);
//    
//    Rect rect = m_pFreezer->getDoorWorldRect();
//    m_pGuideLayer->showGuideTap(Vec2(rect.getMidX(), rect.getMidY()));
    
    m_pFreezer->openDoor();
//    m_pFreezer->setTouchEnable(false);
//    Node* pIn = m_pFreezer->getChildByName("freezerin");
//    if (pIn) {
//        pIn->removeFromParent();
//    }
//    m_pPanFrontView->setVisible(true);
//    m_pPanFrontView->setTouchEnabled(true);
//    m_pGuideLayer->showGuideMove(Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY()),m_pPan->getPosition());
}


void MakeGummyPizzaScene::_finishAddOil()
{
    m_pOilBottle->back();
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _hideTray();
        m_pOilBottle->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        _showTray();
        m_pBowl->setTouchEnabled(false);
        m_pBowl->setVisible(true);
        CMVisibleRect::setPositionAdapted(m_pBowl,800+visibleSize.width/2, 300);
        m_pBowl->setActionPoint(Vec2(173+20, 112+20));
        m_pBowl->setScale(0.6);
        m_pBowl->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                            CallFunc::create([=](){
            m_pBowl->setTouchEnabled(true);
            AudioHelp::getInstance()->playEffect("vo_pour_ mixture_pan.mp3");
        }), NULL));
    }), NULL));
}

void MakeGummyPizzaScene::_finishAddBatter()
{
    //bowl返回tray
    m_pBowl->runAction(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(800, 300))));
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _hideTray();
        m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        
        _movePanRight();
    }), NULL));
    
}

void MakeGummyPizzaScene::_finishCutCandy()
{
    AudioHelp::getInstance()->playEffect("vo_first_step_done.mp3");
    
    std::vector<Sprite*> candyVector;
    int size = m_ringVector.size();
    for (int i = 0; i<size; i++) {
        Sprite* pRing = m_ringVector.at(i);
        Sprite* pSprite =Sprite::create(localPath("make1/pizza_red.png"));
        pSprite->setPosition(pRing->getPosition());
        candyVector.push_back(pSprite);
        pSprite->retain();
    }
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        m_pRing->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        _hideTray();
        
        m_pMixturePan->removeAllChildren();
        
        for (int i = 0; i<size; i++) {
            Sprite* pSprite = candyVector.at(i);
            if (pSprite) {
                m_pMixturePan->addChild(pSprite);
                pSprite->release();
            }
        }
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        
        m_pTableMat->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        m_pPan->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        AudioHelp::getInstance()->playEffect("vo_next_step.mp3");
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        _showBoard();
        _showJellewIngredient(0);
    }), NULL));
}


void MakeGummyPizzaScene::_finishCutJellew(int type)
{
    m_bIsJellewOnBoard = false;
    int count = 0;
    if (type == 0) {
        count = m_JellewGreens.size();
    }else{
        count = m_JellewYellows.size();
    }
    if (count==2) {
        m_pJellewPlateEmpty->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                                        CallFunc::create([=](){
            m_pKnife->setTouchEnabled(false);
            m_pKnife->back();
//            m_pKnife->runAction(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(100, 320))));
            m_pGuideLayer->showGuideMove(m_pBoard->getPosition(), m_pJellewPlateEmpty->getPosition());
            m_pJellewBoard->setTouchEnabled(true);
        }), NULL));
    }else{
        m_pKnife->setTouchEnabled(false);
        m_pKnife->back();
//        m_pKnife->runAction(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(100, 320))));
        m_pGuideLayer->showGuideMove(m_pBoard->getPosition(), m_pJellewPlateEmpty->getPosition());
        m_pJellewBoard->setTouchEnabled(true);
    }
}


void MakeGummyPizzaScene::_finishPutJellew(int type)
{
    if (type == 0) {
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            auto action = MoveBy::create(0.5, Vec2(visibleSize.width/2, 0));
            m_pJellewGreenBlock->runAction(action);
            m_pJellewPlateEmpty->runAction(action->clone());
            m_pJellewPlate->runAction(action->clone());
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _showTray();
            _showJellewIngredient(1);
        }),
                                         NULL));
        
    }else{
        
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            auto action = MoveBy::create(0.5, Vec2(visibleSize.width/2, 0));
            m_pJellewYellowBlock->runAction(action);
            m_pJellewPlateEmpty->runAction(action->clone());
            m_pJellewPlate->runAction(action->clone());
            m_pKnife->setTouchEnabled(false);
            m_pKnife->runAction(MoveBy::create(1, Vec2(-visibleSize.width,0)));
            _removeBoard();
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _createMakeVanillaUI();
        }),
                                         NULL));
    }
}


void MakeGummyPizzaScene::_finishMakeVanillaAdd(int step)
{
    if (step==0) {
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            m_pMilk->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
            m_pGelatin->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _showVanillaIngredient(1);
        }), NULL));
    }else if (step==1) {
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            m_pVanilla->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
            m_pSugar->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _showVanillaIngredient(2);
        }), NULL));
    }else if (step==2){
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            m_pSyrupCorn->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _showSpoon();
            _showTray();
        }), NULL));
    }
}

void MakeGummyPizzaScene::_finishAddCandy(int type)
{
    _hideTray();
    if (type==0) {
        m_pRedCandy->setTouchEnabled(false);
        m_pCandyPlate->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _showAddCandyIngredient(1);
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            
//            m_pGameUI->showNextLayout();
        }), NULL));
    }else if (type==1) {
        m_pGreenCandy->setTouchEnabled(false);
        m_pYellowCandy->setTouchEnabled(false);
        m_pJellewPlate->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        m_pJellewPlateEmpty->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _movePanRight();
        }), NULL));
    }
}


void MakeGummyPizzaScene::_finishMakeJellewPieceAdd(int step)
{
    if (step==0) {
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            m_pMilk->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
            m_pGelatin->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _showJellewPieceIngredient(1);
        }), NULL));
    }else if (step==1){
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _hideTray();
            m_pPowderJellew2->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
            m_pSyrupCorn->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
        }),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
            _showSpoon();
            _showTray();
        }), NULL));
    }
}

void MakeGummyPizzaScene::_finishAddGummyPiece()
{
    _saveMixture();
    
    _hideTray();
    m_pGummyPan->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _movePanRight();
    }), NULL));
}

void MakeGummyPizzaScene::_finishFreezeGummyPiece(int type)
{
    if(type == 0){
        m_pTableMat->runAction(Sequence::create(DelayTime::create(1),
                                                MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                NULL));
        m_pPan->runAction(Sequence::create(DelayTime::create(1),
                                           MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                           CallFunc::create([=](){
            _createMakeJellewPieceUI();
            AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
        }), NULL));
    }else if (type==1) {
        m_pTableMat->runAction(MoveBy::create(0.5, Vec2(100, 0)));
        AudioHelp::getInstance()->playEffect("vo_real_pizza_chef.mp3");
        m_pPan->runAction(Sequence::create(MoveBy::create(0.5, Vec2(100, 0)),
                                           DelayTime::create(2),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("done.mp3");
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setLocalZOrder(10);
            pParticle->setPosition(visibleSize*0.5);
            
            m_pGameUI->showNextLayout();
        }), NULL));
    }
}