
#include "PizzaAddIngredientScene.h"
#include "SceneManager.h"

PizzaAddIngredientScene::PizzaAddIngredientScene()
{
    m_pBowl = nullptr;
    
    m_nRollCount = 0;
    m_bRollToolReady = false;
    m_pDoughGrid = nullptr;
    m_bOilReady = false;
    m_bSauceReady = false;
    m_nOilPercent = 100;
    m_nSaucePercent = 100;
    m_bGraterReady = false;
    m_nCheeseCount = 0;
    m_pBanner = nullptr;
    m_pDecorationScrollView = nullptr;
}

PizzaAddIngredientScene::~PizzaAddIngredientScene()
{
    
}
bool PizzaAddIngredientScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("pizza_bg2.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
//    Sprite* pTable = Sprite::create(localPath("pizza_bg2.jpg"));
//    CMVisibleRect::setPositionAdapted(pTable, 320, 480);
//    this->addChildToBGLayer(pTable);
//    m_pTable = pTable;
    
    _createRollUI();
    _showSpreadFlour();
    
    this->runAction(Sequence::create(DelayTime::create(3),
                                     CallFunc::create([=](){
        this->_pourDough();
//        test
//        _showPan();
//        m_pDoughGrid->setLocalZOrder(10);
//        m_pDoughGrid->runAction(Sequence::create(JumpBy::create(1, Vec2::ZERO, 400, 1),
//                                                 Waves::create(1, Size(20, 20), 2, 5, true, true),
//                                                 CallFunc::create([=](){
//            m_pDoughPan = Sprite::create(localPath("dough2.png"));
//            this->addChildToContentLayer(m_pDoughPan);
//            m_pDoughPan->setPosition(m_pDoughGrid->getPosition());
//            m_pDoughGrid->removeFromParent();
//            _createSauceUI();
//        }), NULL));
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        this->_showRollTool();
    }),NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void PizzaAddIngredientScene::onEnter()
{
    ExtensionScene::onEnter();
}

void PizzaAddIngredientScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setTouchEnabled(false);
        m_pGameUI->hideNext();
        _finishDecorate();
        return;
    }
    ExtensionScene::onButtonCallback(btn);
}
void PizzaAddIngredientScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void PizzaAddIngredientScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    std::string name = pDragNode->getName();
    if (name=="rolltool") {
        if (!m_bRollToolReady) {
            m_bRollToolReady = true;
//            m_pRollTool->runAction(RotateBy::create(0.3, 20));
        }
        AudioHelp::getInstance()->playLoopEffect("roll_dough.mp3");
        m_pDoughGrid->runAction(RepeatForever::create(Sequence::create(Waves::create(1, Size(20, 20), 2, 5, true, true),
                                                                       DelayTime::create(0.1), NULL)));
    }else if (name=="oilbrush") {
        if (m_bOilReady){
            m_pOilScribble->paint(worldPoint);
        }
    }else if (name=="saucespoon") {
        if (m_bSauceReady){
            m_pSauceScribble->paint(worldPoint);
        }
    }
}

void PizzaAddIngredientScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="rolltool") {
        Rect rect = Rect(m_pDoughGrid->getPositionX()-250, m_pDoughGrid->getPositionY()-250, 500, 500);
        rect.origin = m_pDoughGrid->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            m_nRollCount++;
            if (m_nRollCount%10==0){
                int opacity = m_pFlourSprinkle->getOpacity()-1;
                if (opacity>0) {
                    m_pFlourSprinkle->setOpacity(opacity);
                }
            }
            int count = 400;
            if (m_nRollCount%count==0) {
                if (m_nRollCount/count==1) {
                    m_pDough = Sprite::create(localPath("dough2.png"));
                    m_pDoughGrid->addChild(m_pDough);
                    m_pDough->setOpacity(100);
                    m_pDough->setScale(0.8);
                    m_pDough->runAction(Sequence::create(FadeTo::create(0.1, 255),
                                                         ScaleTo::create(2, 1), NULL));
                }else if (m_nRollCount/count<4){
                    m_pDough->setTexture(localPath("dough"+std::to_string(m_nRollCount/count+1)+".png"));
                    if (m_nRollCount/count == 3) {
                        
                        m_pDough->runAction(Sequence::create(DelayTime::create(1),
                                                             CallFunc::create([=](){
                            
                            pDragNode->setTouchEnabled(false);
                            m_pDoughGrid->stopAllActions();
                            m_pDoughGrid->runAction(Waves::create(0.1, Size(20, 20), 2, 5, false, false));
                            m_pFlourSprinkle->runAction(FadeTo::create(0.3, 0));
                            AudioHelp::getInstance()->stopLoopEffect();
                            m_pRollTool->runAction(Sequence::create(DelayTime::create(0.5),
                                                                    MoveBy::create(1, Vec2(visibleSize.width*1.2, 0)),
                                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                                    CallFunc::create([=](){
                                this->_finishRoll();
                            }), NULL));
                            m_pDough->setScale(1);
                            return;
                        }), NULL));
                    }
                }
            }
        }
        
    }else if (name=="oilbrush") {
        if (m_bOilReady) {
            _brushOil();
        }else{
            Rect rect = m_pOilPlate->getBoundingBox();
            rect.origin = m_pOilPlate->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                _brushOil();
            }
        }
    }else if (name=="saucespoon") {
        if (m_bSauceReady) {
            _brushSauce();
        }else{
            Rect rect = m_pSaucePlate->getBoundingBox();
            rect.origin = m_pSaucePlate->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                _brushSauce();
            }
        }
    }else if (name=="cheese") {
        if (m_pGrater->index==0) {
            return;
        }
        Rect rect = pDragNode->getBoundingBox();
        if (rect.containsPoint(m_pGrater->getPosition())) {
            if (m_bGraterReady) {
                _sliceCheese();
            }else{
                m_bGraterReady = true;
                Vec2 location = m_pPan->getPosition();
                Vec2 pos = Vec2(location.x, location.y+80);
                m_pGrater->runAction(RepeatForever::create(Sequence::create(CircleMove::create(20, pos, 150, 360*6),
                                                                            NULL)));
            }
        }
    }
}

void PizzaAddIngredientScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    m_pGuideLayer->removeGuide();
    if (name=="rolltool"){
        AudioHelp::getInstance()->stopLoopEffect();
        m_pDoughGrid->stopAllActions();
        m_pDoughGrid->runAction(Waves::create(0.1, Size(20, 20), 2, 5, false, false));
    }else if (name=="grater") {
        m_pGrater->setTouchEnabled(false);
        Vec2 location = m_pPan->getPosition();
        Vec2 pos = Vec2(location.x, location.y+80);
        m_pGrater->setPosition(Vec2(location.x+150, location.y+80));
        m_pGuideLayer->showGuideMove(m_pCheese->getPosition(), m_pGrater->getPosition());
        m_pGrater->index = 1;
    }else if (name=="cheese") {
        if (!m_bGraterReady) {
            pDragNode->back();
        }
    }
}

void PizzaAddIngredientScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void PizzaAddIngredientScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    
}
void PizzaAddIngredientScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if (m_nIndex<0) {
        return;
    }
    Vec2 pos = pTouch->getLocation();
    std::stringstream ostr;
    ostr<<"content/category/beforbake/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
    Sprite* unit = Sprite::create(ostr.str());
    
    int radius = 230-MIN(unit->getContentSize().width/2, unit->getContentSize().height/2);
    Vec2 offset = pos-m_pPan->getPosition()-Vec2(0, 10);
    if (powf(offset.x, 2)+powf(offset.y, 2)>powf(radius, 2)) {
        return;
    }
    if (std::strcmp(m_sCurTypePath.c_str(), "meat")==0 || std::strcmp(m_sCurTypePath.c_str(), "fruit")==0 || std::strcmp(m_sCurTypePath.c_str(), "vegetable")==0){
        m_pGuideLayer->removeGuide();
        int angle = 20*(arc4random()%10);
        unit->setRotation(angle);
        unit->setPosition(m_pDecoration->convertToNodeSpace(pos));
        m_pDecoration->addChild(unit);
        
        std::stringstream ostrBaked;
        ostrBaked<<"content/category/afterbake/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        Sprite* pBakedUnit = Sprite::create(ostrBaked.str());
        pBakedUnit->setRotation(angle);
        pBakedUnit->setPosition(unit->getPosition());
        m_pDecorationRipe->addChild(pBakedUnit);
        
        AudioHelp::getInstance()->playSelectedEffect();
//        _playEffectNice();
    }
    
}


#pragma mark - initData
void PizzaAddIngredientScene::_initData()
{
    setExPath("content/make/pizza/1/");
}
#pragma mark ==== roll UI =====
void PizzaAddIngredientScene::_createRollUI()
{
    m_pFlourSprinkle = Sprite::create(localPath("powder.png"));
    this->addChildToContentLayer(m_pFlourSprinkle);
    CMVisibleRect::setPositionAdapted(m_pFlourSprinkle, 320, 300+50);
    
    m_pDoughGrid = NodeGrid::create();
    this->addChildToContentLayer(m_pDoughGrid);
    CMVisibleRect::setPositionAdapted(m_pDoughGrid, 320, 300+50);
    
    m_pDough = Sprite::create(localPath("dough1.png"));
    m_pDoughGrid->addChild(m_pDough);
    
    m_pDoughGrid->setVisible(false);
    m_pFlourSprinkle->setOpacity(0);
}


void PizzaAddIngredientScene::_pourDough()
{
    m_pBowl = Sprite::create(localRootPath("0/bowl0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 210-visibleSize.width/2, 350+50);
    
    m_pBowlFront = Sprite::create(localRootPath("0/bowl1.png"));
    this->addChildToContentLayer(m_pBowlFront);
    CMVisibleRect::setPositionAdapted(m_pBowlFront, 210-visibleSize.width/2, 350+50);
    
    m_pDoughBowl = Sprite::create(localRootPath("0/mix5.png"));
    this->addChildToContentLayer(m_pDoughBowl);
    CMVisibleRect::setPositionAdapted(m_pDoughBowl, 210-visibleSize.width/2, 350+50+50);
    
    m_pBowl->setLocalZOrder(5);
    m_pDoughBowl->setLocalZOrder(6);
    m_pBowlFront->setLocalZOrder(10);
    auto bowlAction = Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width/2, 50+100)),
                                       RotateBy::create(0.3, 50),
                                       DelayTime::create(0.3),
                                       MoveBy::create(0.3, Vec2(-200, 0)),
                                       RotateBy::create(0.3, Vec3(0, -40, 0)),
                                       MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                       CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    m_pBowl->runAction(bowlAction);
    m_pBowlFront->runAction(bowlAction->clone());
    m_pDoughBowl->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width/2, 50)),
                                            RotateBy::create(0.3, 50),
                                            JumpBy::create(0.8, Vec2::ZERO, 120, 1),
                                            MoveBy::create(0.5, Vec2(50, -30)),
                                            CallFunc::create([=](){
        m_pDoughGrid->setVisible(true);
        m_pDoughBowl->removeFromParent();
        m_pDoughBowl = nullptr;
    }), NULL));
}

void PizzaAddIngredientScene::_showSpreadFlour()
{
    Sprite* pFlourBag = Sprite::create(localRootPath("0/measuring_cup2.png"));
    this->addChildToContentLayer(pFlourBag);
    pFlourBag->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(pFlourBag, 320+visibleSize.width, 480);
    ccBezierConfig cfg;
    cfg.controlPoint_1 = CMVisibleRect::getPosition(200, 200);
    cfg.controlPoint_2 = CMVisibleRect::getPosition(900, 400);
    cfg.endPosition = CMVisibleRect::getPosition(800, 900);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour2.plist");
    pFlourBag->addChild(pParticle);
    pParticle->setPosition(Vec2(7, 119));
    pParticle->setVisible(false);
    
    pFlourBag->setRotation(100);
    pFlourBag->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                          Spawn::create(JumpBy::create(1, Vec2::ZERO, 30, 2),
                                                        RotateBy::create(0.5, -140), NULL),
                                          CallFunc::create([=](){
        pParticle->setVisible(true);
    }),
                                          DelayTime::create(0.6),
                                          BezierTo::create(0.5, cfg),
                                          CallFunc::create([=](){
        pParticle->removeFromParent();
    }), NULL));
    for (int i = 0; i<6; i++) {
        Sprite* pFlour = Sprite::create(localPath("powder0.png"));
        this->addChildToContentLayer(pFlour);
        pFlour->setLocalZOrder(5);
        Vec2 pos = Vec2(100.0+10.0*(arc4random()%50),200.0+10.0*(arc4random()%50));
        CMVisibleRect::setPositionAdapted(pFlour, pos);
        pFlour->setOpacity(0);
        float dt = 0.1*(arc4random()%10);
        pFlour->runAction(Sequence::create(DelayTime::create(1.5+dt),
                                           FadeIn::create(0.5),
                                           Spawn::create(FadeOut::create(0.5),
                                                         ScaleTo::create(0.3, 2), NULL),
                                           CallFunc::create([=](){
            pFlour->removeFromParent();
        }), NULL));
    }
    m_pFlourSprinkle->runAction(Sequence::create(DelayTime::create(2),
                                                 FadeIn::create(2), NULL));
}

void PizzaAddIngredientScene::_showRollTool()
{
    AudioHelp::getInstance()->playEffect("vo_roll_dough.mp3");
    AudioHelp::getInstance()->playEffect("cut_tool_fall.mp3");
    m_pRollTool = _createDrageNode(localPath("roller.png"));
    this->addChildToContentLayer(m_pRollTool);
    m_pRollTool->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(m_pRollTool, 320+visibleSize.width, 740+50);
    m_pRollTool->setTouchEnabled(false);
    m_pRollTool->setName("rolltool");
    m_pRollTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pRollTool->setOrgPositionDefault();
        m_pRollTool->setTouchEnabled(true);
    }), NULL));
}

void PizzaAddIngredientScene::_removeRollTool()
{
    AudioHelp::getInstance()->stopLoopEffect();
    
}

void PizzaAddIngredientScene::_showPan()
{
    m_pPan = Sprite::create(localPath("pan.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320+visibleSize.width, 300+50);
    m_pPan->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFunc::create([=](){
        
    }), NULL));
}

#pragma mark ===== sauce UI ====
void PizzaAddIngredientScene::_createSauceUI()
{
    AudioHelp::getInstance()->playEffect("vo_brush_oil.mp3");
    _showSauceIngredient();
    
    m_pOilScribble = ScribbleNode::create(m_pDoughPan->getContentSize());
    this->addChildToContentLayer(m_pOilScribble);
    m_pOilScribble->setPosition(m_pDoughGrid->getPosition());
    m_pOilScribble->useBrush("content/make/brush.png");
    m_pOilScribble->useTarget(localPath("oil1.png"));
    
    m_pSauceScribble = ScribbleNode::create(m_pDoughPan->getContentSize());
    this->addChildToContentLayer(m_pSauceScribble);
    m_pSauceScribble->setPosition(m_pDoughGrid->getPosition());
    m_pSauceScribble->useBrush("content/make/brush.png");
    m_pSauceScribble->useTarget(localPath("sauce1.png"));
    
    MyRenderTexture* pOilTempRT = MyRenderTexture::create(m_pDoughPan->getContentSize().width, m_pDoughPan->getContentSize().height);
    Sprite* pOilTemp = Sprite::create(localPath("oil1.png"));
    pOilTemp->setAnchorPoint(Vec2::ZERO);
    pOilTempRT->begin();
    pOilTemp->visit();
    pOilTempRT->end();
    
    MyRenderTexture* pSauceTempRT = MyRenderTexture::create(m_pDoughPan->getContentSize().width, m_pDoughPan->getContentSize().height);
    Sprite* pSauceTemp = Sprite::create(localPath("sauce1.png"));
    pSauceTemp->setAnchorPoint(Vec2::ZERO);
    pSauceTempRT->begin();
    pSauceTemp->visit();
    pSauceTempRT->end();
    
    Director::getInstance()->getRenderer()->render();
    
    m_nOilPercent = 100-pOilTempRT->getPercentageTransparent();
    m_nSaucePercent = 100-pSauceTempRT->getPercentageTransparent();
    
    m_pSauceSpoon = _createDrageNode(localPath("sauce_spoon0.png"));
    this->addChildToContentLayer(m_pSauceSpoon);
    CMVisibleRect::setPositionAdapted(m_pSauceSpoon, 480+visibleSize.width, 740+50);
    m_pSauceSpoon->setActionPoint(Vec2(29, 174));
    
    m_pOilBrush = _createDrageNode(localPath("oil_brush0.png"));
    this->addChildToContentLayer(m_pOilBrush);
    CMVisibleRect::setPositionAdapted(m_pOilBrush, 150+visibleSize.width, 740+50);
    m_pOilBrush->setActionPoint(Vec2(34, 148));
    
    m_pOilBrush->setName("oilbrush");
    m_pSauceSpoon->setName("saucespoon");
    
    m_pOilBrush->setTouchEnabled(false);
    m_pOilBrush->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pOilBrush->setOrgPositionDefault();
        m_pOilBrush->setTouchEnabled(true);
    }), NULL));
    m_pSauceSpoon->setTouchEnabled(false);
    m_pSauceSpoon->setMovable(false);
    m_pSauceSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pSauceSpoon->setOrgPositionDefault();
        m_pSauceSpoon->setTouchEnabled(true);
    }), NULL));
}

void PizzaAddIngredientScene::_showSauceIngredient()
{
    m_pSaucePlate = Sprite::create(localPath("sauce_plate.png"));
    this->addChildToContentLayer(m_pSaucePlate);
    CMVisibleRect::setPositionAdapted(m_pSaucePlate, 480+visibleSize.width, 740+50);
    
    m_pOilPlate = Sprite::create(localPath("oil_plate.png"));
    this->addChildToContentLayer(m_pOilPlate);
    CMVisibleRect::setPositionAdapted(m_pOilPlate, 150+visibleSize.width, 740+50);
    
    m_pOilPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    m_pSaucePlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
//    m_pOilGrid = NodeGrid::create();
//    m_pOilPlate->addChild(m_pOilGrid);
//    m_pOilGrid->setPosition(Vec2(0, 10)+m_pOilPlate->getContentSize()*0.5);
//    
//    m_pOil = Sprite::create(localPath("oil0.png"));
//    m_pOilGrid->addChild(m_pOil);
//    
//    m_pSauceGrid = NodeGrid::create();
//    m_pSaucePlate->addChild(m_pSauceGrid);
//    m_pSauceGrid->setPosition(Vec2(0, 10)+m_pSaucePlate->getContentSize()*0.5);
//    
//    m_pSauce = Sprite::create(localPath("sauce0.png"));
//    m_pSauceGrid->addChild(m_pSauce);
}

void PizzaAddIngredientScene::_removeSauceIngredient()
{
    
}

#pragma mark ====== cheese UI ======
void PizzaAddIngredientScene::_createCheeseUI()
{
    m_pCheeseSlice = Node::create();
    this->addChildToContentLayer(m_pCheeseSlice);
    m_pCheeseSlice->setPosition(m_pPan->getPosition());
    
    _showCheeseIngredient();
}

void PizzaAddIngredientScene::_showCheeseIngredient()
{
    m_pCheese = _createDrageNode(localPath("cheese0.png"));
    this->addChildToContentLayer(m_pCheese);
    CMVisibleRect::setPositionAdapted(m_pCheese, 150+visibleSize.width, 740+50);
    m_pCheese->setName("cheese");
    
    m_pGrater = _createDrageNode(localPath("grater.png"));
    this->addChildToContentLayer(m_pGrater);
    CMVisibleRect::setPositionAdapted(m_pGrater, 480+visibleSize.width, 740+50);
    m_pGrater->setName("grater");
    
    m_pCheese->setTouchEnabled(false);
    m_pGrater->setTouchEnabled(false);
    m_pGrater->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGrater->setOrgPositionDefault();
        m_pGrater->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pGrater->getPosition(), m_pPan->getPosition());
    }), NULL));
    
    m_pCheese->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pCheese->setOrgPositionDefault();
        m_pCheese->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_slice_cheese.mp3");
    }), NULL));
}

#pragma mark ======= decorate UI =====
void PizzaAddIngredientScene::_showDecorations()
{
    m_pDecoration = Node::create();
    this->addChildToContentLayer(m_pDecoration);
    m_pDecoration->setPosition(m_pPan->getPosition());
    
    m_pDecorationRipe = Node::create();
    m_pDecorationRipe->retain();
    
    m_pBanner = Sprite::create("content/category/icon/dec_frameq1.png");
    this->addChildToUILayer(m_pBanner);
//    CMVisibleRect::setPositionAdapted(m_pBanner,  320,830);
    CMVisibleRect::setPositionAdapted(m_pBanner,  320,70-visibleSize.height,kBorderNone,kBorderBottom);
    m_pBanner->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height)));
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(visibleSize.width,130));
    this->addChildToUILayer(m_pTypeScrollView);
//    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  0,800,kBorderLeft);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  0,200,kBorderLeft,kBorderBottom);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(PizzaAddIngredientScene::_onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->boxPathEX = "";
    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    m_pTypeScrollView->setScale(1,0.8);
    m_pTypeScrollView->runAction(EaseBackIn::create(ScaleTo::create(0.5, 1)));
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(30);
    m_pTypeScrollView->setSelectedAnimate(false);
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("pizza"));
    m_pTypeScrollView->selectBtn(0);
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(PizzaAddIngredientScene::TouchBegan, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(PizzaAddIngredientScene::TouchEnded, this);
    std::memset(m_bGuide, 0, 3);
}

void PizzaAddIngredientScene::_removeDecorations()
{
    if (m_pBanner) {
        m_pBanner->runAction(Sequence::create(ScaleTo::create(0.5, 1,0),
                                                      CallFunc::create([=]()
                                                                       {
                                                                           m_pBanner->removeFromParent();
                                                                           m_pBanner = nullptr;
                                                                       }), NULL));
    }
    if (m_pTypeScrollView) {
        m_pTypeScrollView->runAction(Sequence::create(ScaleTo::create(0.5, 1,0),
                                                      CallFunc::create([=]()
                                                                       {
                                                                           m_pTypeScrollView->removeFromParent();
                                                                           m_pTypeScrollView = nullptr;
                                                                       }), NULL));
    }
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->removeFromParent();
        m_pDecorationScrollView = nullptr;
    }
}

#pragma mark ====== deel =========

void PizzaAddIngredientScene::_brushOil()
{
    if (m_bOilReady){
        Vec2 endPos = m_pOilBrush->getWorldSpaceActionPoint();
        Vec2 startPos = Vec2(m_pOilBrush->getOffset(), endPos);
        m_pOilScribble->paint(startPos, endPos);
        Sprite* pBrushOil = (Sprite*)m_pSauceSpoon->getChildByName("brush_oil");
        static int moveCount = 0;
        moveCount++;
        if (moveCount%20==0) {
            int percent = 100-m_pOilScribble->getCanvas()->getPercentageTransparent();
            if (pBrushOil) {
                pBrushOil->setOpacity(225-200.0*percent/m_nSaucePercent);
            }
            if (100.0*percent/m_nOilPercent>=96) {
                moveCount = 0;
                m_pOilScribble->paint(localPath("oil1.png"));
                m_pOilBrush->runAction(RotateBy::create(0.5, 100));
                _finishBrushOil();
            }
        }
    }else{
        m_bOilReady = true;
//        m_pOilGrid->runAction(Repeat::create(Waves::create(0.5, Size(10, 10), 1, 5, true, true), 2));
//        m_pOil->runAction(ScaleTo::create(0.5, 0.5));
        m_pOilBrush->runAction(RotateBy::create(0.5, -100));
        
        Sprite* pBrushOil = Sprite::create(localPath("oil_brush1.png"));
        m_pOilBrush->addChild(pBrushOil);
        pBrushOil->setName("brush_oil");
    }
}

void PizzaAddIngredientScene::_brushSauce()
{
    if (m_bSauceReady){
        Vec2 endPos = m_pSauceSpoon->getWorldSpaceActionPoint();
        Vec2 startPos = Vec2(m_pSauceSpoon->getOffset(), endPos);
        m_pSauceScribble->paint(startPos, endPos);
        Sprite* pSpoonSauce = (Sprite*)m_pSauceSpoon->getChildByName("spoon_sauce");
        static int moveCount = 0;
        moveCount++;
        if (moveCount%20==0) {
            int percent = 100-m_pSauceScribble->getCanvas()->getPercentageTransparent();
            if (pSpoonSauce) {
                pSpoonSauce->setOpacity(225-200.0*percent/m_nSaucePercent);
            }
            if (100.0*percent/m_nSaucePercent>=96) {
                moveCount = 0;
                m_pSauceScribble->paint(localPath("sauce1.png"));
                m_pSauceSpoon->runAction(RotateBy::create(0.5, 90));
                _finishBrushSauce();
            }
        }
    }else{
        m_bSauceReady = true;
//        m_pSauceGrid->runAction(Repeat::create(Waves::create(0.5, Size(5, 7), 1, 5, true, true), 2));
//        m_pSauce->runAction(ScaleTo::create(0.5, 0.5));
        m_pSauceSpoon->runAction(RotateBy::create(0.5, -90));
        Sprite* pSpoonSauce = Sprite::create(localPath("sauce_spoon1.png"));
        m_pSauceSpoon->addChild(pSpoonSauce);
        pSpoonSauce->setName("spoon_sauce");
    }
}


void PizzaAddIngredientScene::_sliceCheese()
{
    m_pGuideLayer->removeGuide();
    m_nCheeseCount++;
    
    log("====m_pCheeseSlice=%f,%f",m_pCheeseSlice->getPositionX(),m_pCheeseSlice->getPositionY());
    for (int i = 0; i<3; i++) {
        Sprite* pSlice = Sprite::create(localPath("cheese1_"+std::to_string(i%3)+".png"));
        m_pCheeseSlice->addChild(pSlice);
        Vec2 pos = m_pGrater->getWorldSpaceActionPoint();
        pos = m_pCheeseSlice->convertToNodeSpace(pos);
        pSlice->setPosition(pos);
        log("=====%f,%f",pos.x,pos.y);
        float rotation = 5.0*(arc4random()%80);
        float scale = 0.5+0.1*(arc4random()%10);
        if (i<2) {
            float x = -20.0+5.0*(arc4random()%8);
            float y = -60.0-5.0*(arc4random()%8);
            Vec2 movePos = Vec2(x, y);
            pSlice->runAction(Spawn::create(RotateBy::create(0.5, rotation),
                                            ScaleTo::create(0.3, scale),
                                            MoveBy::create(0.5, movePos), NULL));
        }else{
            float x = -90.0+10.0*(arc4random()%18);
            float y = 90.0-10.0*(arc4random()%18);
            Vec2 movePos = Vec2(x, y);
            pSlice->runAction(Spawn::create(RotateBy::create(0.5, rotation),
                                            ScaleTo::create(0.3, scale),
                                            MoveTo::create(0.5, movePos), NULL));
        }
        static int count = 0;
        count++;
        log("+++++++++++slice count  %d",count);
    }
    int k = 60;
    m_pCheese->setScale(m_pCheese->getScale()-0.001);
    if (m_nCheeseCount%k==0) {
        int mixtureStep = m_nCheeseCount/k;
        if (mixtureStep<=2){
//            std::stringstream ostr;
//            ostr<<"cheese"<<mixtureStep<<".png";
//            m_pCheese->getDragSprite()->setTexture(localPath(ostr.str()));
        }else{
            m_pCheese->setTouchEnabled(false);
            m_pCheese->setVisible(false);
        }
        if (mixtureStep>=3){
            _finishCheeseSlice();
            AudioHelp::getInstance()->stopLoopEffect();
        }
    }
}

void PizzaAddIngredientScene::_onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
{
    m_sCurTypeStr = typeData.decTypeName;
    AudioHelp::getInstance()->playSelectedEffect();
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_sCurTypePath = string(data.pathName);
    m_nIndex = -1;
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/"<<data.pathName<<"_icon/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(visibleSize.width, 200),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = data;
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(PizzaAddIngredientScene::_onDecorationCallback, this);
        }else{
            m_pDecorationScrollView->decorationData = data;
        }
        
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(PizzaAddIngredientScene::_onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        m_pDecorationScrollView->setMargin(20);
        m_pDecorationScrollView->setScale(0);
//        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, visibleSize.width,630,kBorderLeft);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,  visibleSize.width,40,kBorderLeft,kBorderBottom);
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width,0)));
        //        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(150, 0)));
        m_pDecorationScrollView->runAction(ScaleTo::create(0.2, 1));
        
        m_pDecorationScrollView->btnPathEX = ostr.str();
        
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
        //        m_pDecorationScrollView->boxPathEX = "content/category/icon/box_2.png";
        m_pDecorationScrollView->setMargin(30);
        m_pDecorationScrollView->reloadData();
    }else{
        m_sCurTypePath = "";
        if (m_pDecorationScrollView) {
            m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-150, 0)),
                                                                CallFunc::create([=]()
                                                                                 {
                                                                                     m_pDecorationScrollView->removeFromParent();
                                                                                     m_pDecorationScrollView = nullptr;
                                                                                 }), NULL));
        }
    }
    
}

void PizzaAddIngredientScene::_onDecorationCallback(int index,int type,DecorateConfigData decData)
{
    if (type==1) {
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
            };
            m_pDecorationScrollView->setSelected(false);
            return;
        }
    }
    AudioHelp::getInstance()->playSelectedEffect();
    
    m_nIndex = index;
    //    }
    AudioHelp::getInstance()->playSelectedEffect();
    std::stringstream ostr;
    ostr<<"content/category/"<<decData.pathName<<"/"<<index<<".png";
    
    m_nIndex = index;
    m_bCanTouch = false;
    if (std::strcmp(decData.pathName.c_str(), "meat")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[0]) {
            m_pGuideLayer->showGuideTap(Vec2(visibleSize.width*0.6, visibleSize.height*0.5));
            m_bGuide[0] = true;
        }
        return;
        
    }else if (std::strcmp(decData.pathName.c_str(), "fruit")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_pGuideLayer->showGuideTap(Vec2(visibleSize.width*0.6, visibleSize.height*0.5));
            m_bGuide[1] = true;
        }
        return;
        
    }else if (std::strcmp(decData.pathName.c_str(), "vegetable")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_pGuideLayer->showGuideTap(Vec2(visibleSize.width*0.6, visibleSize.height*0.5));
            m_bGuide[2] = true;
        }
        return;
        
    }
//    _playEffectNice();
}

void PizzaAddIngredientScene::_savePizza()
{
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pCheeseSlice) {
        Vec2 posCheese = m_pCheeseSlice->getPosition();
        m_pCheeseSlice->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        
        render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        m_pCheeseSlice->visit();
        render->end();
        m_pCheeseSlice->setPosition(posCheese);
    }
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"normal_cheese.png", false);
    pImage->autorelease();
    
}

void PizzaAddIngredientScene::_savePizzaDecoration()
{
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pDecoration){
        Vec2 pos = m_pDecoration->getPosition();
        m_pDecoration->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        
        render->begin();
        m_pDecoration->visit();
        render->end();
        
        m_pDecoration->setPosition(pos);
    }
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"normal_dec.png", false);
    pImage->autorelease();
}

void PizzaAddIngredientScene::_savePizzaDecorationRipe()
{
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pDecorationRipe) {
        Vec2 pos = m_pDecorationRipe->getPosition();
        m_pDecorationRipe->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        
        render->begin();
        m_pDecorationRipe->visit();
        render->end();
        m_pDecorationRipe->setPosition(pos);
    }
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"normal_decripe.png", false);
    pImage->autorelease();
}

void PizzaAddIngredientScene::_finishRoll()
{
    AudioHelp::getInstance()->stopLoopEffect();
    
    _showPan();
    m_pDoughGrid->setLocalZOrder(10);
    m_pDoughGrid->runAction(Sequence::create(JumpBy::create(1, Vec2::ZERO, 400, 1),
                                             Waves::create(1, Size(20, 20), 2, 5, true, true),
                                             CallFunc::create([=](){
        m_pDoughPan = Sprite::create(localPath("dough4.png"));
        this->addChildToContentLayer(m_pDoughPan);
        m_pDoughPan->setPosition(m_pDoughGrid->getPosition());
        m_pDoughGrid->removeFromParent();
        _createSauceUI();
    }), NULL));
}

void PizzaAddIngredientScene::_finishBrushOil()
{
    m_pOilBrush->setTouchEnabled(false);
    m_pOilBrush->setMovable(false);
    m_pSauceSpoon->setTouchEnabled(true);
    m_pSauceSpoon->setMovable(true);
    m_pOilBrush->back(1);
    
    auto action = Sequence::create(DelayTime::create(0.8),
                                   EaseBackIn::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0))),
                                   CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    
    m_pOilPlate->runAction(action);
    m_pOilBrush->runAction(action->clone());
    AudioHelp::getInstance()->playEffect("vo_add_sauce.mp3");
}

void PizzaAddIngredientScene::_finishBrushSauce()
{
    m_pSauceSpoon->setTouchEnabled(false);
    m_pSauceSpoon->back();
    auto action = Sequence::create(DelayTime::create(0.8),
                                   EaseBackIn::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0))),
                                   CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    m_pSaucePlate->runAction(action->clone());
    m_pSauceSpoon->runAction(action->clone());
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        _createCheeseUI();
        m_pOilBrush = nullptr;
        m_pSauceSpoon = nullptr;
    }), NULL));
}


void PizzaAddIngredientScene::_finishCheeseSlice()
{
    m_pGrater->stopAllActions();
    m_pGrater->runAction(Sequence::create(DelayTime::create(0.5),
                                          MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGrater->removeFromParent();
        m_pGrater = nullptr;
    }), NULL));
    
    
    auto action = Sequence::create(DelayTime::create(1.5),
                                   MoveBy::create(0.5, Vec2(0, 190)), NULL);
    m_pPan->runAction(action);
    m_pDoughPan->runAction(action->clone());
//    m_pTable->runAction(action->clone());
    m_pOilScribble->runAction(action->clone());
    m_pSauceScribble->runAction(action->clone());
    m_pCheeseSlice->runAction(action->clone());
    
    Sprite* pBg = Sprite::create(localRootPath("bg/pizza_bg3.png"));
    this->addChildToBGLayer(pBg);
    pBg->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.5-140));
    pBg->runAction(Sequence::create(DelayTime::create(1.5),
                                    MoveBy::create(0.5, Vec2(0, 140)), NULL));
    
    this->runAction(Sequence::create(DelayTime::create(2.2),
                                     CallFunc::create([=](){
        _savePizza();
        _showDecorations();
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("done.mp3");
        
        m_pGameUI->showNextLayout();
    }), NULL));
}


void PizzaAddIngredientScene::_finishDecorate()
{
    AudioHelp::getInstance()->playEffect("vo_great_combination.mp3");
    _removeDecorations();
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    _savePizzaDecoration();
    _savePizzaDecorationRipe();
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<PizzaBakeScene>();
    }), NULL));
}