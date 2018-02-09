
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
    m_pClipping = nullptr;
    m_pBag = nullptr;
    m_pPlateShadow = nullptr;
    m_pPlateBack = nullptr;
    m_pPlateFront = nullptr;
    m_pDessert = nullptr;
    m_pDessert2 = nullptr;
    m_bPacked = false;
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
    m_pBg = Sprite::create(localPath("bg.png"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pTable = Sprite::create(localPath("table1.png"));
    CMVisibleRect::setPositionAdapted(m_pTable, 480, 70,kBorderNone,kBorderBottom);
    this->addChildToBGLayer(m_pTable);
    
    m_pShed = Sprite::create(localPath("awnings.png"));
    CMVisibleRect::setPosition(m_pShed, 480, 0,kBorderNone,kBorderTop);
    this->addChildToBGLayer(m_pShed);
    
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    m_pDecorateLayer->setPosition(Vec2(100,0));
    
    m_pDecBg = Sprite::create("content/category/icon/box_1.png");
    this->addChildToUILayer(m_pDecBg);
    CMVisibleRect::setPositionAdapted(m_pDecBg,  170-visibleSize.width/2,640/2,kBorderLeft);
    m_pDecBg->runAction(Sequence::create(DelayTime::create(0.5),
                                         EaseBackIn::create(MoveBy::create(1, Vec2(visibleSize.width/2, 0))),
                                         ScaleTo::create(0.3,1.1,1),
                                         ScaleTo::create(0.3,1), NULL));
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(170,470));
    this->addChildToUILayer(m_pTypeScrollView);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->boxPathEX = "content/category/icon/box_2.png";
    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setSingleAsset(false);
    m_pTypeScrollView->setMargin(15);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionV);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  10,(640-m_pTypeScrollView->getContentSize().height)/2,kBorderLeft);
    m_pTypeScrollView->setScale(0.3);
    m_pTypeScrollView->setVisible(false);
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(1.5),
                                                  CallFunc::create([=](){
        m_pTypeScrollView->setVisible(true);
    }),
                                                  ScaleTo::create(0.5, 1), NULL));
    _initDefaultDecorate();
    
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
    
    m_pGameUI->showNormalLayout();
    m_CandySize = Size(420+100,500+150);
    m_nNextSceneTag = GameUIEvent::eSceneTagShare;
    return true;
}

void DecorateScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_DECORATE);
    if (GameDataManager::getInstance()->m_nDecorateStep==1) {
        m_pGameUI->hideBack();
//        this->runAction(Sequence::create(DelayTime::create(0.5),
//                                         CallFunc::create([=]()
//                                                          {
//                                                              m_pGameUI->showBackLayout();
//                                                          }), NULL));
    }
    
}
#pragma mark - initData
void DecorateScene::_initData()
{
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    setExPath("content/make/");
    
    std::memset(m_bGuide, 0, 3);
    std::memset(m_bVOGuide, 0, 8);
    GameDataManager::getInstance()->m_bPacked = false;

}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(m_pDecorateLayer, eDecorateTypeFood);
//    ******************      cotton candy        **********************
    std::stringstream ostrStick;
    ostrStick<<"content/category/straw/"<<GameDataManager::getInstance()->m_nStickIndex+1<<".png";
    Sprite*pStick = Sprite::create(ostrStick.str());
//    pStick->setRotation(160);
    m_pStick = pStick;
    
    std::stringstream ostrCotton;
    ostrCotton<<"content/make/make/candy"<<GameDataManager::getInstance()->m_nShapeIndex+1<<"/"<<GameDataManager::getInstance()->m_nFlavorIndex+1<<".png";
    Sprite*pCotton = Sprite::create(ostrCotton.str());
    if (GameDataManager::getInstance()->m_nShapeIndex==1) {
        pCotton->setRotation(93);
    }else{
        
        pCotton->setRotation(150);
    }
    float delt = -20;
    if (GameDataManager::getInstance()->m_nShapeIndex==2) {
        delt=0;
    }else if (GameDataManager::getInstance()->m_nShapeIndex==1){
        delt= -30;
    }else if (GameDataManager::getInstance()->m_nShapeIndex==3){
        pCotton->setRotation(145);
    }
    m_pCandy = pCotton;
    m_pCandy->setScale(1.2);
    
    pDecManager->addDecoration(pCotton, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(485+delt, 400)));
    
    pDecManager->addDecoration(pStick, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(460, 240)));
    
    
    if (!m_pClipping) {
        Sprite* pStencil = Sprite::create(ostrCotton.str());
        pStencil->setScale(m_pCandy->getScale());
        m_pClipping = ClippingNode::create(pStencil);
    }
    m_pClipping->setRotation(pCotton->getRotation());
    m_pClipping->setAlphaThreshold(0.5);
    pDecManager->addDecoration(m_pClipping, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(485+delt, 400)));
    
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal"));
    
}

void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    if (GameDataManager::getInstance()->m_nDecorateStep==0) {
        DecorateManager::getInstance()->reset();
        m_pClipping = nullptr;
        m_pBag = nullptr;
        m_pDessert = nullptr;
        m_pDessert2 = nullptr;
        m_pPlateShadow = nullptr;
        m_pPlateBack = nullptr;
        m_pPlateFront = nullptr;
        m_bPacked = false;
        if (m_pDecorationScrollView) {
            m_pDecorationScrollView->removeFromParent();
            m_pDecorationScrollView = nullptr;
        }
//        _removePlate();
        _initDefaultDecorate();
    }else{
//        DecorateManager::getInstance()->clearExtra();
        m_pBag->removeAllChildren();
    }
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

void DecorateScene::_showPlate()
{
    Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(700, 100-visibleSize.height));
    if (!m_pPlateShadow) {
        m_pPlateShadow = Sprite::create("content/category/dessert/plate_shadow.png");
        DecorateManager::getInstance()->addDecoration(m_pPlateShadow, eDecorationLayerFoodBack,"",pos,0);
        m_pPlateShadow->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height)));
    }
    if (!m_pPlateBack) {
        m_pPlateBack = Sprite::create("content/category/dessert/plate_down.png");
        DecorateManager::getInstance()->addDecoration(m_pPlateBack, eDecorationLayerFoodBack,"",pos,1);
        m_pPlateBack->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height)));
    }
    if (!m_pPlateFront) {
        m_pPlateFront = Sprite::create("content/category/dessert/plate_on.png");
        DecorateManager::getInstance()->addDecoration(m_pPlateFront, eDecorationLayerFoodFront,"",pos,1);
        m_pPlateFront->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height)));
    }
    if (!m_pDessert) {
        m_pDessert = Node::create();
        DecorateManager::getInstance()->addDecoration(m_pDessert, eDecorationLayerFood,"",Vec2::ZERO,5);
    }
}

void DecorateScene::_removePlate()
{
    if (m_pPlateShadow) {
        m_pPlateShadow->removeFromParent();
        m_pPlateShadow = nullptr;
    }
    if (m_pPlateBack) {
        m_pPlateBack->removeFromParent();
        m_pPlateBack = nullptr;
    }
    if (m_pPlateFront) {
        m_pPlateFront->removeFromParent();
        m_pPlateFront = nullptr;
    }
}

void DecorateScene::_saveCottonCandy()
{
    float scale = 1;
    if (GameDataManager::getInstance()->m_nShapeIndex==0 || GameDataManager::getInstance()->m_nShapeIndex==2){
    }else if (GameDataManager::getInstance()->m_nShapeIndex==3){
        scale = 1.1;
    }else{
        scale = m_pCandy->getScale();
    }
    Size candySize = Size(m_CandySize.width,m_CandySize.height)*scale;
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 posOrigin = m_pDecorateLayer->getPosition();
    
//    Vec2 pos = Vec2(-210, -80);
//    Vec2 pos = Vec2(-210-CMVisibleRect::getDesignOffset().x, -80-CMVisibleRect::getDesignOffset().y);
    
    Vec2 pos = Vec2(candySize.width/2-m_pCandy->getPositionX(),candySize.height/2-m_pCandy->getPositionY()+50);
    
    m_pDecorateLayer->setPosition(pos);
    
    if(m_pPlateShadow){
        m_pPlateShadow->setVisible(false);
    }
    if(m_pPlateBack){
        m_pPlateBack->setVisible(false);
    }
    if(m_pPlateFront){
        m_pPlateFront->setVisible(false);
    }
    if(m_pDessert){
        m_pDessert->setVisible(false);
    }
    if(m_pDessert2){
        m_pDessert2->setVisible(false);
    }
    if(m_pBag){
        Vector<Node*> pChildren = m_pBag->getChildren();
        for_each(pChildren.begin(), pChildren.end(), [=](Node* pNode){
            DecorationUtil* pUnit =  dynamic_cast<DecorationUtil*>(pNode);
            if(pUnit) {
                pUnit->m_bIsShotScreen = true;
            }
        });
    }
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pDecorateLayer->visit();
    render->end();
    
    if(m_pPlateShadow){
        m_pPlateShadow->setVisible(true);
    }
    if(m_pPlateBack){
        m_pPlateBack->setVisible(true);
    }
    if(m_pPlateFront){
        m_pPlateFront->setVisible(true);
    }
    if(m_pDessert){
        m_pDessert->setVisible(true);
    }
    if(m_pDessert2){
        m_pDessert2->setVisible(true);
    }
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issuccess = pImage->saveToFile(SSCFileUtility::getStoragePath()+ name, false);
#else
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+ name, false);
#endif
    pImage->autorelease();
    GameDataManager::getInstance()->setBoxName(name);
}


void DecorateScene::_displayCottonCandy(){
    AudioHelp::getInstance()->playEffect("vo_nice_design.mp3");
    m_pDecBg->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)));
    m_pTypeScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                                  DelayTime::create(1),
                                                  CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_eat_sell.mp3");
    }), NULL));
    
    m_pGameUI->showEatLayout();
    m_pGameUI->showSaleLayout();
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)));
    }
    m_pDecorateLayer->runAction(MoveBy::create(0.5, Vec2(-50, 0)));
    
    m_pGameUI->hideReset();
    m_pGameUI->hideNext();
    
    _saveCottonCandy();
}
void DecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
}

void DecorateScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        
        if (GameDataManager::getInstance()->m_nDecorateStep==0) {
            if (m_bPacked) {
                btn->runAction(Sequence::create(DelayTime::create(1),
                                                CallFunc::create([=]()
                                                                 {
                                                                     btn->setEnabled(true);
                                                                 }), NULL));
                GameDataManager::getInstance()->m_nDecorateStep = 1;
                m_sCurTypeStr = "";
                if (m_pDecorationScrollView) {
                    m_pDecorationScrollView->runAction(Sequence::create(ScaleTo::create(0.3, 0.3),
                                                                        CallFunc::create([=]()
                                                                                         {
                                                                                             m_pDecorationScrollView->removeFromParent();
                                                                                             m_pDecorationScrollView = nullptr;
                                                                                         }), NULL));
                }
                m_pTypeScrollView->runAction(Sequence::create(ScaleTo::create(0.3, 0.1),
                                                              CallFunc::create([=]()
                                                                               {
                                                                                   m_pTypeScrollView->setVisible(false);
                                                                                   m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("bag"));
                                                                                   m_pTypeScrollView->setVisible(true);
                                                                                   AudioHelp::getInstance()->playEffect("vo_decorate_pack.mp3");
                                                                               }),
                                                              EaseBackIn::create(ScaleTo::create(0.5, 1)), NULL));
//                m_pGameUI->showBackLayout();

            }else{
                _displayCottonCandy();
            }
            return;
            
        }else if (GameDataManager::getInstance()->m_nDecorateStep==1){
            _displayCottonCandy();
            return;
        }
    }else if (GameUILayoutLayer::eUIButtonTagEat==tag) {
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        SceneManager::pushTheScene<ShareScene>();
    }else if (GameUILayoutLayer::eUIButtonTagSale==tag) {
        btn->setEnabled(false);
        DecorateManager::getInstance()->reset();
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        SceneManager::replaceTheScene<PriceScene>();
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
    m_pGuideLayer->removeGuide();
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/icon_"<<m_sCurTypeStr<<"/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(170,460),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onDecorationCallback, this);
            m_pDecorationScrollView->onOKClicked = CC_CALLBACK_0(DecorateScene::onDecorateOK, this);
        }else{
            m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
        }
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionV);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        //        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, 5, 80,kBorderLeft);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,  155,(640-m_pDecorationScrollView->getContentSize().height)/2,kBorderLeft);
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
        
        m_pDecorationScrollView->btnPathEX = ostr.str();
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/box2_2.png";
//        m_pDecorationScrollView->boxPathEX = "content/category/icon/box_1.png";
        m_pDecorationScrollView->bgPath = "content/category/icon/box2_1.png";
        m_pDecorationScrollView->setMargin(10);
        m_pDecorationScrollView->reloadData();
        
        if (typeData.decTypeName=="sprinkles") {
            if (!m_bVOGuide[1]) {
                m_bVOGuide[1] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_sprinkles.mp3");
            }
        }else if (typeData.decTypeName=="fruit") {
            if (!m_bVOGuide[2]) {
                m_bVOGuide[2] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_fruits.mp3");
            }
        }else if (typeData.decTypeName=="candy") {
            if (!m_bVOGuide[3]) {
                m_bVOGuide[3] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_candy.mp3");
            }
        }else if (typeData.decTypeName=="dessert") {
            if (!m_bVOGuide[4]) {
                m_bVOGuide[4] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_dessert.mp3");
            }
        }else if (typeData.decTypeName=="bag") {
            if (!m_bVOGuide[5]) {
                m_bVOGuide[5] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_bag.mp3");
            }
        }else if (typeData.decTypeName=="straw") {
            if (!m_bVOGuide[6]) {
                m_bVOGuide[6] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_stick.mp3");
            }
        }else if (typeData.decTypeName=="toy") {
            if (!m_bVOGuide[7]) {
                m_bVOGuide[7] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_toppings.mp3");
            }
        }
    }else{
        if (m_pDecorationScrollView) {
//            m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2,0)),
//                                                                CallFunc::create([=]()
//                                                                                 {
//                                                                                     m_pDecorationScrollView->removeFromParent();
//                                                                                     m_pDecorationScrollView = nullptr;
//                                                                                 }), NULL));
            m_pDecorationScrollView->removeFromParent();
            m_pDecorationScrollView = nullptr;
        }
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

    
    if (type==0) {
        if (!gNoneIap && decData.freeCount!=decData.totalCount && index>=decData.freeCount+decData.beginIndex){
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss();
            m_pDecorationScrollView->setSelected(false);
            return;
        }
    }else if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
        RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
        m_pDecorationScrollView->setSelected(false);
        return;
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    
    m_nIndex = index;
    if (m_sCurTypePath=="sprinkles"){
        m_bCanTouch = true;
        if (!m_bGuide[0]) {
            m_bGuide[0] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pCandy->getPosition()));
        }
        return;
    }
    if (m_sCurTypePath=="fruit"){
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_bGuide[1] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pCandy->getPosition()));
        }
        return;
    }
    if (m_sCurTypePath=="candy"){
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_bGuide[2] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pCandy->getPosition()));
        }
        return;
    }
    
    std::stringstream ostr;
    ostr<<"content/category/"<<m_sCurTypeStr<<"/"<<index<<".png";
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    
    if (std::strcmp(m_sCurTypeStr.c_str(), "bag")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        unit->setName(m_sCurTypeStr);
        m_pBag = unit;
        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFoodFront,decData.single?m_sCurTypeStr:"",CMVisibleRect::getPositionAdapted(Vec2(460, 310)),0);
        if (GameDataManager::getInstance()->m_nShapeIndex==0 || GameDataManager::getInstance()->m_nShapeIndex==2){
            
//            m_pBag->setScale(1.1);
        }else if (GameDataManager::getInstance()->m_nShapeIndex==3){
            m_pBag->setScale(1.1);
        }else{
            m_pBag->setScale(m_pCandy->getScale());
            DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFoodFront,decData.single?m_sCurTypeStr:"",CMVisibleRect::getPositionAdapted(Vec2(460, 295)),0);
        }
        pParticle->setPosition(unit->getPosition()+Vec2(50, 0));
        m_bPacked = true;
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "dessert")==0) {
        if (m_nIndex<11) {
            _showPlate();
            
//            Sprite* unit = Sprite::create(ostr.str());
//            m_pDessert->addChild(unit);
//            Vec2 posDelt = Vec2(10.0*(arc4random()%5), 0);
//            unit->setAnchorPoint(Vec2(0.5, 0));
//            Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(700-25, 45+visibleSize.height));
//            unit->setPosition(pos+posDelt);
//            unit->runAction(MoveBy::create(0.5, Vec2(0, -visibleSize.height)));
//            pParticle->setPosition(m_pPlateFront->getPosition());
            
            Sprite* unit = Sprite::create(ostr.str());
            DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
            pDecoration->setIgnoreTip();
            pDecoration->addChild(unit);
            Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(700-25, 45+unit->getAnchorPointInPoints().y));
            Rect rect = Rect(0, 0, 100, 5);
            rect.origin = pos;
            pDecoration->setMoveLimitRect(rect);
            Vec2 posDelt = Vec2(10.0*(arc4random()%5), 0);
            pDecoration->setPosition(pos+posDelt);
            m_pDessert->addChild(pDecoration);
            pDecoration->setScale(0.2);
            pDecoration->runAction(ScaleTo::create(0.2, 1));
            pParticle->setPosition(pDecoration->getPosition()+Vec2(50, 0));
        }else{
            if (!m_pDessert2) {
                m_pDessert2 = Node::create();
                DecorateManager::getInstance()->addDecoration(m_pDessert2, eDecorationLayerAllFront,"",Vec2::ZERO,1);
            }
            Sprite* unit = Sprite::create(ostr.str());
            DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
            pDecoration->setIgnoreTip();
            pDecoration->addChild(unit);
            pDecoration->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height-300));
            Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(300.0f+10*(arc4random()%60),100.0f+10*(arc4random()%40)));
            pDecoration->setPosition(position);
            m_pDessert2->addChild(pDecoration);
            pParticle->setPosition(pDecoration->getPosition()+Vec2(50, 0));
        }
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "bowknot")==0) {
        Sprite* pBowknot = (Sprite*)m_pBag->getChildByName("bowknot");
        if (pBowknot) {
            pBowknot->removeFromParent();
        }
        Sprite* unit = Sprite::create(ostr.str());
        m_pBag->addChild(unit,2);
        unit->setName("bowknot");
        unit->setPosition(Vec2(0, -160)+m_pBag->getContentSize()*0.5);
        pParticle->setPosition(m_pBag->getPosition()+Vec2(50, -180));
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "straw")==0) {
        m_pStick->setTexture(ostr.str());
        pParticle->setPosition(m_pStick->getPosition()+Vec2(50, 0));
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "toy")==0) {
        Sprite* pContent = Sprite::create(ostr.str());
        DecorationUtil* pUnit = DecorationUtil::createDefault(pContent->getContentSize());
        pUnit->addChild(pContent);
        float scale = m_pBag->getScale();
        Rect rect = m_pBag->getBoundingBox();
        rect.origin = Vec2(50, 200)*scale;
        rect.size = rect.size-Size(160, 330)*scale;
        pUnit->setMoveLimitRect(rect);
        m_pBag->addChild(pUnit,1);
        pUnit->setPosition(m_pBag->getContentSize()*0.5);
        pParticle->setPosition(m_pBag->getPosition()+Vec2(50, 0));

    }else if (std::strcmp(m_sCurTypeStr.c_str(), "stickers")==0) {
        Sprite* pContent = Sprite::create(ostr.str());
        DecorationUtil* pUnit = DecorationUtil::createDefault(pContent->getContentSize());
        pUnit->addChild(pContent);
        float scale = m_pBag->getScale();
        Rect rect = m_pBag->getBoundingBox();
        rect.origin = Vec2(50, 200)*scale;
        rect.size = rect.size-Size(160, 330)*scale;
        pUnit->setMoveLimitRect(rect);
        m_pBag->addChild(pUnit,0);
        pUnit->setPosition(m_pBag->getContentSize()*0.5);
        pParticle->setPosition(m_pBag->getPosition()+Vec2(50, 0));
        
        
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "other food")==0) {
        FillMaterialModel* unit = FillMaterialModel::create(ostr.str(),decData.small);
        unit->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height-300));
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(20.0f+10*(arc4random()%60), 100.0f+10*(arc4random()%60)));
        if (decData.eatable) {
            DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllFront,decData.single?m_sCurTypeStr:"",position,1);
            pParticle->setPosition(unit->getPosition());
        }else{
            DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllUneatFront,decData.single?m_sCurTypeStr:"",position,1);
            pParticle->setPosition(unit->getPosition());
        }
        
        
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