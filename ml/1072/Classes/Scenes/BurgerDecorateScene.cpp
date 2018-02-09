
#include "BurgerDecorateScene.h"
#include "SceneManager.h"

BurgerDecorateScene::BurgerDecorateScene()
{
    m_pDecorationScrollView = nullptr;
    m_pBreadOn = nullptr;
}

BurgerDecorateScene::~BurgerDecorateScene()
{
    
}
bool BurgerDecorateScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_decoration.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
        
    _showBurger();
    _showBeaf();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_cheese_veggies.mp3");
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pBeaf->getPosition(), m_pPlate->getPosition());
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    m_pGameUI->showNormalLayout();
    return true;
}

void BurgerDecorateScene::onEnter()
{
    ExtensionScene::onEnter();
    if (m_pBreadOn) {
        m_pBreadOn->setPosition(Vec2(0, visibleSize.height));
    }
    if (m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagNext)) {
        m_pGameUI->showNextLayout();
    }
}

void BurgerDecorateScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    if(tag == GameUILayoutLayer::eUIButtonTagNext){
        btn->setEnabled(false);
        MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_bread);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        
        m_pBreadOn->runAction(Sequence::create(MoveTo::create(0.5, _getTopPos()+Vec2(0, 80)),
                                               CallFunc::create([=](){
            m_pFood->runAction(Sequence::create(ScaleBy::create(0.5, 1,0.9),
                                                EaseBackOut::create(ScaleTo::create(0.5, 1)),
                                                CallFunc::create([=](){
                _savePizza();
                SceneManager::pushTheScene<BurgerEatScene>();
            }), NULL));
        }), NULL));
    }else if(tag == GameUILayoutLayer::eUIButtonTagReset){
        AudioHelp::getInstance()->playResetEffect();
        m_pDec->removeAllChildren();
        
    }
}
void BurgerDecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BurgerDecorateScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void BurgerDecorateScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void BurgerDecorateScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    if(name == "beaf"){
        Rect rect = m_pPlate->getBoundingBox();
        rect.size = rect.size+Size(0,100);
        if (rect.containsPoint(pDragNode->getPosition())) {
            Sprite* pBeaf = Sprite::create(localPath("beef_patty.png"));
            m_pFood->addChild(pBeaf);
            pBeaf->setPosition(Vec2(0, 20));
            m_pPlateBeaf->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                     CallFunc::create([=](){
                m_pGameUI->showNextLayout();
                m_pGameUI->showResetLayout();
                _showDecorations();
                m_pPlateBeaf->removeFromParent();
            }), NULL));
            
            m_pFood->runAction(Spawn::create(MoveBy::create(1, Vec2(160, 0)),
                                             ScaleTo::create(1, 1), NULL));
            m_pPlate->runAction(Spawn::create(MoveBy::create(1, Vec2(160, 0)),
                                              ScaleTo::create(1, 1), NULL));
            pDragNode->removeFromParent();
        }else{
            pDragNode->back();
        }
    }
    m_pGuideLayer->removeGuide();
}

void BurgerDecorateScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void BurgerDecorateScene::_initData()
{
    setExPath("content/make/burger/5/");
}

void BurgerDecorateScene::_showBurger()
{
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 160-visibleSize.width, 520);
    m_pPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pPlate->setScale(0.5);
    
    m_pFood = Node::create();
    this->addChildToContentLayer(m_pFood);
    CMVisibleRect::setPositionAdapted(m_pFood, 160-visibleSize.width, 540);
    m_pFood->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pFood->setScale(0.5);
    
    m_pDec = Node::create();
    m_pFood->addChild(m_pDec,1);
    
    m_pBreadOn = Sprite::create(localPath("bread_on.png"));
    m_pFood->addChild(m_pBreadOn,10);
    m_pBreadOn->setPosition(Vec2(0, visibleSize.height));
    
    m_pBreadDown = Sprite::create(localPath("down.png"));;
    m_pFood->addChild(m_pBreadDown);
}

void BurgerDecorateScene::_showBeaf()
{
    m_pPlateBeaf = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlateBeaf);
    CMVisibleRect::setPositionAdapted(m_pPlateBeaf, 480-visibleSize.width, 520);
    m_pPlateBeaf->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pPlateBeaf->setScale(0.5);
    
    m_pBeaf = _createDrageNode(localPath("beef_patty.png"));
    this->addChildToContentLayer(m_pBeaf);
    CMVisibleRect::setPositionAdapted(m_pBeaf, 480-visibleSize.width, 540);
    m_pBeaf->setTouchEnabled(false);
    m_pBeaf->setName("beaf");
    m_pBeaf->setScale(0.5);
    
    m_pBeaf->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pBeaf->setTouchEnabled(true);
    }), NULL));
}
void BurgerDecorateScene::_showDecorations()
{
    m_decPos = Vec2::ZERO;
    m_pBanner = Sprite::create("content/category/icon/dec_frameq1.png");
    this->addChildToUILayer(m_pBanner);
    CMVisibleRect::setPositionAdapted(m_pBanner,  320,70-visibleSize.height,kBorderNone,kBorderBottom);
    m_pBanner->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height)));
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(visibleSize.width,130));
    this->addChildToUILayer(m_pTypeScrollView);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  0,200,kBorderLeft,kBorderBottom);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(BurgerDecorateScene::_onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->boxPathEX = "";
    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    m_pTypeScrollView->setScale(1,0.5);
    m_pTypeScrollView->runAction(EaseBackIn::create(ScaleTo::create(0.5, 1)));
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(60);
    m_pTypeScrollView->setSelectedAnimate(false);
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("burger"));
    m_pTypeScrollView->selectBtn(0);
}

Vec2 BurgerDecorateScene::_getTopPos()
{
    Vector<Node*> pChildren = m_pDec->getChildren();
    Vec2 pos = Vec2::ZERO;
    for_each(pChildren.begin(), pChildren.end(), [=,&pos](Node* pNode){
        if (pNode!=m_pBreadOn) {
            Vec2 pos2 = pNode->getPosition();
            if (pos2.y>pos.y) {
                pos = pos2;
            }
        }
    });
    pos.x = 0;
    if(pos.y==0 ){
        pos.y = 20;
    }
    return pos;
}

void BurgerDecorateScene::_onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
{
    AudioHelp::getInstance()->playSelectedEffect();
    m_sCurTypeStr = string(typeData.decTypeName);
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_sCurTypePath = string(data.pathName);
    
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/"<<data.pathName<<"_icon/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(visibleSize.width, 120),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = data;
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(BurgerDecorateScene::_onDecorationCallback, this);
        }else{
            m_pDecorationScrollView->decorationData = data;
        }
        
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(BurgerDecorateScene::_onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        m_pDecorationScrollView->setMargin(40);
//        m_pDecorationScrollView->setScale(0);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,  0,80,kBorderLeft,kBorderBottom);
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width,0)));
//        //        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(150, 0)));
//        m_pDecorationScrollView->runAction(ScaleTo::create(0.2, 1));
        
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


void BurgerDecorateScene::_onDecorationCallback(int index,int type,DecorateConfigData decData)
{
    if (type==1) {
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
            };
            return;
        }
    }
    AudioHelp::getInstance()->playSelectedEffect();
    std::stringstream ostr;
    ostr<<"content/category/"<<m_sCurTypeStr<<"/"<<index<<".png";
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    
    Vec2 position = _getTopPos()+Vec2(0,20);
    if (position.y>visibleSize.height-m_pFood->getPositionY()-200) {
        position.y = visibleSize.height-m_pFood->getPositionY()-200;
    }
    if (decData.zoomable) {
        Sprite* unit = Sprite::create(ostr.str());
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
        pDecoration->ignoreRotation(true);
        pDecoration->setIgnoreTip();
        pDecoration->addChild(unit);
        pDecoration->setMoveLimitRect(Rect(-60, position.y, 120, 50));
        pDecoration->setPosition(position);
        m_pDec->addChild(pDecoration);
    }else{
        Sprite* unit = Sprite::create(ostr.str());
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
        pDecoration->setIgnoreTip();
        pDecoration->addChild(unit);
        pDecoration->setMoveLimitRect(Rect(-60, position.y, 120, 50));
        pDecoration->setPosition(position);
        m_pDec->addChild(pDecoration);
    }
    pParticle->setPosition(m_decPos+m_pPlate->getPosition());
}

void BurgerDecorateScene::_savePizza()
{
    Size mixtureSize = Size(visibleSize.width,visibleSize.height);
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pFood) {
        Vec2 pos = m_pFood->getPosition();
        m_pFood->setPosition(pos+Vec2(0, -200));
        render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        m_pFood->visit();
        render->end();
        m_pFood->setPosition(pos);
    }
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"burger_dec.png", false);
    pImage->autorelease();
}

