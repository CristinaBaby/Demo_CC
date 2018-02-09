
#include "DecorateScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "StickFoodHelp.h"

static std::string gPackageName[] = {
    "lemonade",
    "hotdog",
    "frenchfries",
    "snowcone",
    "churro",
    "burrito",
    "funnelcake",
    "frybread",
};
DecorateScene::DecorateScene()
{
    m_pDecorationScrollView = nullptr;
    m_bCanTouch = false;
    m_bPacked = false;
    m_bButtonEable = true;
}

DecorateScene::~DecorateScene()
{
    
}
bool DecorateScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    GameDataManager::getInstance()->m_nBgIndex = -1;
    
    //    bg
    m_pBg = Sprite::create(localPath("bg/bg_2.png"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pTable = Sprite::create(localPath("bg/bg_table2.png"));
    CMVisibleRect::setPositionAdapted(m_pTable, 320, 380,kBorderNone,kBorderBottom);
    this->addChildToBGLayer(m_pTable);
    m_pTable->setLocalZOrder(1);
    
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    m_pDecorateLayer->setPosition(Vec2(0,100));
    m_pDecorateLayer->setLocalZOrder(5);
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(640,200));
    this->addChildToUILayer(m_pTypeScrollView);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
//    m_pTypeScrollView->boxPathEX = "content/category/icon/box_2.png";
//    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(30);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,(640-m_pTypeScrollView->getContentSize().width)/2+visibleSize.width,  130,kBorderNone,kBorderBottom);
    m_pTypeScrollView->setSelectedAnimate();
    
    _initDefaultDecorate();
    
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(1.5),
                                                  MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                                  DelayTime::create(0.5),
                                                  CallFunc::create([=](){
        m_pTypeScrollView->selectBtn(0);
        m_pTypeScrollView->showSelectedAnimation();
    }), NULL));

    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(DecorateScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(DecorateScene::TouchMoved, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(DecorateScene::TouchEnded, this);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_decorate.mp3");
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGameUI->showNextLayout();
        m_pGameUI->showResetLayout();
    }), NULL));
    GameDataManager::getInstance()->m_nDecorateStep = 0;
    
    m_pGameUI->showNormalLayout();
    m_CandySize = Size(300+40,450+100);
    m_nNextSceneTag = GameUIEvent::eSceneTagShare;
    return true;
}

void DecorateScene::onEnter()
{
    ExtensionScene::onEnter();
//    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_DECORATE);
    
}
#pragma mark - initData
void DecorateScene::_initData()
{
    m_nFlavor = GameDataManager::getInstance()->m_nFlavorIndex;
    setExPath("content/make/");
    
    std::memset(m_bGuide, 0, 3);
    std::memset(m_bVOGuide, 0, 8);
    GameDataManager::getInstance()->m_nPackage = false;

}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(m_pDecorateLayer, eDecorateTypeDrink);
    //    ******************      Food        **********************
    Sprite*pCupShadow = Sprite::create("content/make/cup/cup_shadow.png");
    Sprite*pCupDown = Sprite::create("content/make/cup/cup_down.png");
    Sprite*pCupUp = Sprite::create("content/make/cup/cup_on.png");
    Sprite*pLidDown = Sprite::create("content/make/cup/cup_lio_down.png");
    Sprite*pLidUp = Sprite::create("content/make/cup/cup_lio_on.png");
    Sprite*pPattern = Sprite::create("content/make/cup/pattern/"+std::to_string(GameDataManager::getInstance()->m_nCupIndex+1)+".png");
    m_pCup = pCupUp;
    
    Vec2 pos = Vec2(320, 480);
    pDecManager->addDecoration(pCupShadow, eDecorationLayerDrinkBack,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderBottom));
    pDecManager->addDecoration(pLidDown, eDecorationLayerDrinkBack,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderBottom));
    pDecManager->addDecoration(pCupDown, eDecorationLayerDrinkBack,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderBottom));
    pDecManager->addDecoration(pCupUp, eDecorationLayerDrinkFront,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderBottom));
    pDecManager->addDecoration(pPattern, eDecorationLayerDrinkFront,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderBottom));
    pDecManager->addDecoration(pLidUp, eDecorationLayerDrinkFront,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderBottom));
    
    std::stringstream ostr;
    ostr<<"content/make/cup/Food/"<<m_nFlavor+1<<".png";
    Sprite* pFood = Sprite::create(ostr.str());
    pDecManager->setDrinkMaskSprite(Sprite::create("content/make/cup/cup_mask.png"));
    pDecManager->addDecoration(pFood, eDecorationLayerDrink,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderBottom));
    
    
    if (!m_pClipping) {
        Sprite* pStencil = Sprite::create("content/make/cup/cup_mask.png");
        
        m_pClipping = ClippingNode::create(pStencil);
    }
    m_pClipping->setAlphaThreshold(0.5);
    pDecManager->addDecoration(m_pClipping, eDecorationLayerDrinkFood,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderBottom));
    
    
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal"));
}

void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    
    DecorateManager::getInstance()->reset();
    m_pClipping = nullptr;
    m_bPacked = false;
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->removeFromParent();
        m_pDecorationScrollView = nullptr;
    }
    //        _removePlate();
    _initDefaultDecorate();
    
    m_pTypeScrollView->selectBtn(0);
    m_pTypeScrollView->showSelectedAnimation();
    
    m_nIndex = -1;
//    m_pBg->setTexture(Sprite::create(localPath("bg.jpg"))->getTexture());
    GameDataManager::getInstance()->m_nBgIndex = -1;
    Button* next = m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagNext);
    if (next) {
        next->setEnabled(false);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              next->setEnabled(true);
                                                          }), NULL));
    }
}

void DecorateScene::_saveFood()
{
    float scale = 1;
    Size candySize = Size(m_CandySize.width,m_CandySize.height)*scale;
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height+200,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 posOrigin = m_pDecorateLayer->getPosition();
    
//    Vec2 pos = Vec2(-210, -80);
//    Vec2 pos = Vec2(-210-CMVisibleRect::getDesignOffset().x, -80-CMVisibleRect::getDesignOffset().y);
    
    Vec2 pos = Vec2(candySize.width/2-m_pCup->getPositionX(),candySize.height/2-m_pCup->getPositionY()+50);
    
    m_pDecorateLayer->setPosition(pos);
    
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pDecorateLayer->visit();
    render->end();
    
    m_pDecorateLayer->setPosition(posOrigin);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    unsigned long time =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
    std::string name = StringUtils::format("%ld.png",time);
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+ name, false);
    pImage->autorelease();
    GameDataManager::getInstance()->setBoxName(name);
}


void DecorateScene::_displayFood(){
    AudioHelp::getInstance()->playEffect("vo_nice_design.mp3");
    
    m_pGameUI->showEatLayout();
    m_pGameUI->showSaleLayout();
    
    m_pDecorateLayer->runAction(MoveBy::create(1, Vec2(0, 450)));
    m_pDecorateLayer->runAction(ScaleTo::create(1, 1));
    m_pDecorateLayer->runAction(RotateBy::create(1, Vec3(-40, 0, 0)));
    m_pTable->runAction(MoveBy::create(1, Vec2(0, 480)));
    m_pBanner->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                          CallFunc::create([=](){
        _saveFood();
        m_pGameUI->hideReset();
        m_pGameUI->hideNext();
        
    }), NULL));
    
    
}
void DecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
}

void DecorateScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    if (!m_bButtonEable) {
        return;
    }
    m_bButtonEable = false;
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=]{
        m_bButtonEable = true;
    }), NULL));
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        if (GameDataManager::getInstance()->m_nDecorateStep==0) {
            
            m_pGameUI->hideNext();
            m_pGameUI->hideReset();
            btn->runAction(Sequence::create(DelayTime::create(0.5),
                                            CallFunc::create([=](){
                btn->setEnabled(true);
            }), NULL));
            GameDataManager::getInstance()->m_nDecorateStep=1;
        }else{
            _displayFood();
        }
        
        
        return;
    }else if (GameUILayoutLayer::eUIButtonTagSale==tag) {
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
//        SceneManager::pushTheScene<SaleScene>();
    }else if (GameUILayoutLayer::eUIButtonTagEat==tag) {
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        SceneManager::pushTheScene<ShareScene>();
    }else if (GameUILayoutLayer::eUIButtonTagBack==tag){
        if (GameDataManager::getInstance()->m_nDecorateStep==1){
            _reset();
            m_sCurTypeStr = "";
            m_pGameUI->hideBack();
            GameDataManager::getInstance()->m_nDecorateStep = 0;
            if (m_pDecorationScrollView) {
                m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 450)),
                                                                    CallFunc::create([=]()
                                                                                     {
                                                                                         m_pDecorationScrollView->removeFromParent();
                                                                                         m_pDecorationScrollView = nullptr;
                                                                                     }), NULL));
            }
            m_pTypeScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 200)),
                                                          CallFunc::create([=]()
                                                                           {
                                                                               m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal"));
                                                                           }),
                                                          EaseBackIn::create(MoveBy::create(1, Vec2(0, -200))), NULL));
            return;
        }
    }
    ExtensionScene::onButtonCallback(btn);
    
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
//        if (m_pTypeScrollView->getPercent()>=100) {
//        }else{
//            m_pTypeScrollView->scrollToNext();
//        }
        
    }else if (GameUILayoutLayer::eUIButtonTagBack==tag){
//        m_pTypeScrollView->scrollToPre();
        
    }else if (GameUILayoutLayer::eUIButtonTagReset==tag){
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(481,537), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to reset your food?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            btn->setEnabled(true);
            _reset();
        };
        dialog->onNegativeClick = [=](void*){
            btn->setEnabled(true);
            
        };
        btn->setEnabled(true);
    }
    
}

void DecorateScene::onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
{
    m_sCurTypeStr = string(typeData.decTypeName);
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_sCurTypePath = string(data.pathName);
    AudioHelp::getInstance()->playSelectedEffect();
    m_nIndex = -1;
//    m_pGuideLayer->removeGuide();
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/"<<m_sCurTypeStr<<"_icon/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(590,150),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onDecorationCallback, this);
            m_pDecorationScrollView->onOKClicked = CC_CALLBACK_0(DecorateScene::onDecorateOK, this);
        }else{
            m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
        }
        m_pDecorationScrollView->setLocalZOrder(1);
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        //        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, 5, 80,kBorderLeft);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,(640-m_pDecorationScrollView->getContentSize().width)/2,  80,kBorderNone,kBorderBottom);
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
        
        m_pDecorationScrollView->btnPathEX = ostr.str();
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
        m_pDecorationScrollView->boxPathEX = "content/category/icon/frame.png";
        m_pDecorationScrollView->bgPath = "content/category/icon/box2_1.png";
        m_pDecorationScrollView->setMargin(10);
        m_pDecorationScrollView->reloadData();
        
    }else{
//        if (m_pDecorationScrollView) {
////            m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2,0)),
////                                                                CallFunc::create([=]()
////                                                                                 {
////                                                                                     m_pDecorationScrollView->removeFromParent();
////                                                                                     m_pDecorationScrollView = nullptr;
////                                                                                 }), NULL));
//            m_pDecorationScrollView->removeFromParent();
//            m_pDecorationScrollView = nullptr;
//        }
    }
}

void DecorateScene::onDecorateOK()
{
    
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(1, Vec2(0, 450))),
                                                            CallFunc::create([=]()
                                                                             {
                                                                                 
                                                                             }), NULL));
    }
    if (m_pTypeScrollView) {
        m_pTypeScrollView->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pTypeScrollView, 5-visibleSize.width, 80,kBorderLeft);
        m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(1.0f),
                                                      EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))), NULL));
    }
}
void DecorateScene::onDecorationCallback(int index,int type,DecorateConfigData decData)
{
//    m_pTypeScrollView->setNormal();
//    if (m_pDecorationScrollView) {
//        m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, 450)),
//                                                            CallFunc::create([=]()
//                                                                             {
//                                                                                 m_pDecorationScrollView->removeFromParent();
//                                                                                 m_pDecorationScrollView = nullptr;
//                                                                             }), NULL));
//    }

    
    if (type==1) {
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            
            ShopMiniLayer* pLayer = ShopMiniLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
                m_pDecorationScrollView->setSelected(false);
            };
            
            return;
        }
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    
    m_nIndex = index;
//    if (m_sCurTypePath=="sprinkles"){
//        m_bCanTouch = true;
//        if (!m_bGuide[0]) {
//            m_bGuide[0] = true;
//            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pCup->getPosition()));
//        }
//        return;
//    }
//    if (m_sCurTypePath=="fruit"){
//        m_bCanTouch = true;
//        if (!m_bGuide[1]) {
//            m_bGuide[1] = true;
//            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pCup->getPosition()));
//        }
//        return;
//    }
//    if (m_sCurTypePath=="candy"){
//        m_bCanTouch = true;
//        if (!m_bGuide[2]) {
//            m_bGuide[2] = true;
//            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pCup->getPosition()));
//        }
//        return;
//    }
    
    std::stringstream ostr;
    ostr<<"content/category/"<<m_sCurTypeStr<<"/"<<index<<".png";
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    
    if (m_sCurTypeStr=="sprinkles"||m_sCurTypeStr=="candy"||m_sCurTypeStr=="fruit") {
        Sprite* pItem = Sprite::create(ostr.str());
        DecorationUtil* unit = DecorationUtil::createDefault(pItem->getContentSize());
        unit->setIgnoreTip();
        unit->ignoreRotation(true);
        unit->addChild(pItem);
        Rect rect = Rect(-m_pCup->getAnchorPointInPoints().x+30, -m_pCup->getAnchorPointInPoints().y+20, m_pCup->getContentSize().width-60, m_pCup->getContentSize().height-20);
        
        unit->setMoveLimitRect(rect);
        m_pClipping->addChild(unit);
        Vec2 position = Vec2(-50.0f+10*(arc4random()%10), -100.0f+10*(arc4random()%20));
        unit->setPosition(position);
        pParticle->setPosition(m_pClipping->getPosition()+m_pDecorateLayer->getPosition()+position);
        
    }else{
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(260.0f+10*(arc4random()%15), 400.0f+10*(arc4random()%20)));
        if (decData.zoomable||decData.rotatable) {
            FillMaterialModel* unit = FillMaterialModel::create(ostr.str(),decData.small);
            Rect rect = Rect(220, 380, 200, 250);
            rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
            unit->setMoveLimitRect(rect);
            if (decData.eatable) {
                DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFood,decData.single?m_sCurTypeStr:"",position,1);
                pParticle->setPosition(unit->getPosition());
            }else{
                DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllUneatFront,decData.single?m_sCurTypeStr:"",position,1);
                pParticle->setPosition(unit->getPosition());
            }
        }else{
            Sprite* unit = Sprite::create(ostr.str());
            DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
            pDecoration->setIgnoreTip();
            pDecoration->addChild(unit);
            pDecoration->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height-300));
            if (decData.eatable) {
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFood,decData.single?m_sCurTypeStr:"",position,1);
                pParticle->setPosition(pDecoration->getPosition());
            }else{
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerAllUneatFront,decData.single?m_sCurTypeStr:"",position,1);
                pParticle->setPosition(pDecoration->getPosition());
            }
        }
    }
    
    AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
}

void DecorateScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
}

void DecorateScene::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
}

void DecorateScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if (m_nIndex<0) {
        return;
    }
    Vec2 pos = pTouch->getLocation();
    if (pos.x>visibleSize.width-100) {
        return;
    }
    if (m_sCurTypePath=="sprinkles" || m_sCurTypePath=="fruit" || m_sCurTypePath=="candy"){
        m_pGuideLayer->removeGuide();
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        Sprite* unit = Sprite::create(ostr.str());
        int angle = 20*(arc4random()%10);
        unit->setRotation(angle);
        unit->setPosition(m_pClipping->convertToNodeSpace(pos));
        m_pClipping->addChild(unit);
        
//        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllFront,"",pos,1);
        
        AudioHelp::getInstance()->playSelectedEffect();
        
    }
    
}
void DecorateScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    m_pGuideLayer->removeGuide();
}
void DecorateScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->isMoving()){
        pDragNode->setVisible(true);
        pDragNode->setScale(1);
    }
}
void DecorateScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    int tag = pDragNode->getTag();
    
}
void DecorateScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}