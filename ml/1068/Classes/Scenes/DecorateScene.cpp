
#include "DecorateScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"

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
    m_bShowShop = false;
    m_bCanTouch = false;
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
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pTable = Sprite::create(localPath("table.png"));
    CMVisibleRect::setPosition(pTable, 320, 180);
    this->addChildToBGLayer(pTable);
    m_pBg = pBg;
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(150,540));
    this->addChildToUILayer(m_pTypeScrollView);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  0-visibleSize.width/2,150,kBorderLeft);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setSingleAsset(false);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionV);
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                  EaseBackIn::create(MoveBy::create(1, Vec2(visibleSize.width/2, 0))),
                                                  DelayTime::create(2), NULL));
    m_pTypeScrollView->setSingleAsset(false);
    if (GameDataManager::getInstance()->m_nPackage==0){
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("snowcone"));
        
    }else{
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("icecream"));
    }
    
    _initDefaultDecorate();
    
    m_pGameUI->showNextLayout();
    m_pGameUI->showResetLayout();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/dysc.png",CMVisibleRect::getPosition(320+visibleSize.width, 650));
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(DecorateScene::TouchBegan, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(DecorateScene::TouchEnded, this);
    
//    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(false);
//    listener->onTouchBegan = CC_CALLBACK_2(DecorateScene::TouchBegan, this);
//    listener->onTouchEnded = CC_CALLBACK_2(DecorateScene::TouchEnded, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void DecorateScene::onEnter()
{
    ExtensionScene::onEnter();
    m_bShowShop = false;
    if (GameDataManager::getInstance()->m_nPackage==0){
        FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_DECORATE_SNOWCONE);
    }else{
        FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_DECORATE_ICECREAM);
    }
}

void DecorateScene::onExit()
{
    if (GameDataManager::getInstance()->m_nPackage==0){
        FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_DECORATE_SNOWCONE);
    }else{
        FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_DECORATE_ICECREAM);
    }
    ExtensionScene::onExit();
}
#pragma mark - initData
void DecorateScene::_initData()
{
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    setExPath("content/dec/");
    m_nNextSceneTag = GameUIEvent::eSceneTagShare;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagShare;
    std::memset(m_bGuide, 0, 3);
}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(this->m_pContentLayer, eDecorateTypeBoth);
//    ******************      Beach Food        **********************
    if (GameDataManager::getInstance()->m_nPackage==0) {
    Sprite*pConeBack = Sprite::create("content/make/snowcone/crush/cone0_0.png");
    
    std::stringstream ostrSnow;
    ostrSnow<<"content/make/snowcone/shape/shape"<<GameDataManager::getInstance()->m_nShapeIndex+1<<".png";
    Sprite*pSnow = Sprite::create(ostrSnow.str());
    
//    pSnow->setScale(1.28);
    pDecManager->addDecoration(pSnow, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320+50, 555-visibleSize.height)));
    
    
        Sprite*pColor;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        pColor = Sprite::create(FileUtility::getStoragePath()+"snow.png");
#else
        pColor = Sprite::create(FileUtility::getStoragePath()+"/snow.png");
#endif
        if (pColor) {
//            pColor->setScale(1.28);
            pDecManager->addDecoration(pColor, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320+50, 555-visibleSize.height)));
        }
        Sprite*pConeFront = Sprite::create("content/make/snowcone/crush/cone0_1.png");
        
        
        pDecManager->addDecoration(pConeBack, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(320+50, 250-50-visibleSize.height)));
        pDecManager->addDecoration(pConeFront, eDecorationLayerFoodFront,"cone",CMVisibleRect::getPositionAdapted(Vec2(320+50, 250-50-visibleSize.height)));
        
        auto action = Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
        pConeBack->runAction(action);
        pSnow->runAction(action->clone());
        pConeFront->runAction(action->clone());
        if (pColor) {
            pColor->runAction(action->clone());
        }

    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("snowcone"));
    }else {
//    icecream
        Sprite*pConeBack = Sprite::create("content/make/icecream/wafer_cone1.png");
        Sprite*pCone = Sprite::create("content/make/icecream/wafer_cone.png");
        Sprite*pConeFront = Sprite::create("content/make/icecream/wafer_cone_on.png");
        std::stringstream ostr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ostr<<"icecream.png";
#else
        ostr<<"/icecream.png";
#endif
        Sprite*pIcecream = Sprite::create(FileUtility::getStoragePath()+ostr.str());
        
        pDecManager->addDecoration(pConeBack, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(380, 260-90-visibleSize.height)),-1);
        pDecManager->addDecoration(pCone, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(380, 260-90-visibleSize.height)),-1);
        std::stringstream ostrIcecream;
        ostrIcecream<<"content/make/icecream/choose/icecream/icecream_"<<GameDataManager::getInstance()->m_nIcecreamIndex<<".png";
        Sprite* pFirstIcecream = Sprite::create(ostrIcecream.str());
//        pFirstIcecream->setPosition(CMVisibleRect::getPosition(380, 430-90));
        pFirstIcecream->setScale(0.9);
        pDecManager->addDecoration(pFirstIcecream, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(380, 430-90-visibleSize.height)),2);
//        380,260
        if (pIcecream) {
            pDecManager->addDecoration(pIcecream, eDecorationLayerAllFront,"",CMVisibleRect::getPositionAdapted(Vec2(320, 480-90-0.75*visibleSize.height)),-1);
        }
        pDecManager->addDecoration(pConeFront, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(380, 260-90-visibleSize.height)),2);
        
        auto action = Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
        pConeBack->runAction(action);
        pCone->runAction(action->clone());
        pConeFront->runAction(action->clone());
        pFirstIcecream->runAction(action->clone());
        if (pIcecream) {
            pIcecream->runAction(action->clone());
        }
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("icecream"));
    }

}

void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    DecorateManager::getInstance()->reset();
    
    _initDefaultDecorate();
    
    Button* next = m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagNext);
    if (next) {
        next->setEnabled(false);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              next->setEnabled(true);
                                                          }), NULL));
    }
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(-80, 0)),
                                                                          ScaleTo::create(0.5, 0), NULL),
                                                            CallFunc::create([=]()
                                                                             {
                                                                                 m_pDecorationScrollView->removeFromParent();
                                                                                 m_pDecorationScrollView = nullptr;
                                                                             }), NULL));
    }
}

void DecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
}

RenderTexture* DecorateScene::getResultRender()
{
    RenderTexture* render = RenderTexture::create(visibleSize.width, visibleSize.height*1.6,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    m_pContentLayer->setPosition(Vec2(0, visibleSize.height*0.3));
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    
    m_pContentLayer->visit();
    render->end();
    
    m_pContentLayer->setPosition(Vec2(0, 0));
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;
}

void DecorateScene::onButtonCallback(Button* btn)
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
        bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"decorate.png", false);
#else
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"/decorate.png", false);
#endif
        pImage->autorelease();
        log("===save success %d==",issuccess);
        GameDataManager::getInstance()->setStepCount(6);
        
    }else if (GameUILayoutLayer::eUIButtonTagBack==tag){
        if (GameDataManager::getInstance()->m_nDecorateStep==1){
            _reset();
            m_sCurTypePath = "";
            m_pGameUI->hideBack();
            GameDataManager::getInstance()->m_nDecorateStep = 0;
            if (m_pDecorationScrollView) {
                m_pDecorationScrollView->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(-80, 0)),
                                                                                  ScaleTo::create(0.5, 0), NULL),
                                                                    CallFunc::create([=]()
                                                                                     {
                                                                                         m_pDecorationScrollView->removeFromParent();
                                                                                         m_pDecorationScrollView = nullptr;
                                                                                     }), NULL));
                m_pDecorationScrollView->removeFromParent();
                m_pDecorationScrollView = nullptr;
            }
            m_pTypeScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 200)),
                                                          CallFunc::create([=]()
                                                                           {
                                                                               m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("inner"));
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
//        Dialog* dialog=Dialog::create(Size(497,355), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
//        dialog->setContentText("Are you sure you want to reset your Beach Food?");
//        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
//        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
//        dialog->onPositiveClick = [=](void*){
//            btn->setEnabled(true);
//            _reset();
//        };
//        dialog->onNegativeClick = [=](void*){
//            btn->setEnabled(true);
//            
//        };
        _reset();
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
    }
    
}

void DecorateScene::onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
{
    m_sCurTypeStr = typeData.decTypeName;
    AudioHelp::getInstance()->playSelectedEffect();
    m_nIndex = -1;
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_sCurTypePath = string(data.pathName);
    if (selected) {
        
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/"<<data.pathName<<"_icon/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(556, 180),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = data;
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateScene::onDecorationCallback, this);
        }else{
            m_pDecorationScrollView->decorationData = data;
        }
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateScene::onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        m_pDecorationScrollView->setMargin(10);
//        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, (640-m_pDecorationScrollView->getContentSize().width)*0.5,60-visibleSize.height/2);
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)));
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, (640-m_pDecorationScrollView->getContentSize().width)*0.5,720+visibleSize.height/2);
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(0, -visibleSize.height/2)));
        m_pDecorationScrollView->btnPathEX = ostr.str();
        
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
        if (std::strcmp(m_sCurTypePath.c_str(), "sprinkle")==0 || std::strcmp(m_sCurTypePath.c_str(), "fruit")==0  || std::strcmp(m_sCurTypePath.c_str(), "candy")==0) {
            m_pDecorationScrollView->setMargin(30);
        }else{
            m_pDecorationScrollView->setMargin(0);
        }
        m_pDecorationScrollView->setMargin(30);
        m_pDecorationScrollView->reloadData();
    }else{
        m_sCurTypePath = "";
        if (m_pDecorationScrollView) {
            m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)),
                                                                CallFunc::create([=]()
                                                                                 {
                                                                                     m_pDecorationScrollView->removeFromParent();
                                                                                     m_pDecorationScrollView = nullptr;
                                                                                 }), NULL));
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
void DecorateScene::onDecorationCallback(int index,DecorateConfigData decData)
{
//    m_pTypeScrollView->setNormal();
//    if (m_pDecorationScrollView) {
//        m_pDecorationScrollView->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(-80, 0)),
//                                                                          ScaleTo::create(0.5, 0), NULL),
//                                                            CallFunc::create([=]()
//                                                                             {
//                                                                                 m_pDecorationScrollView->removeFromParent();
//                                                                                 m_pDecorationScrollView = nullptr;
//                                                                             }), NULL));
////        m_pDecorationScrollView->removeFromParent();
////        m_pDecorationScrollView = nullptr;
//    }
    
    if (!gNoneIap) {
        if (index>=decData.freeCount+decData.beginIndex && !(index<decData.totalCount+decData.beginIndex && index>=decData.holidayIndex && decData.holidayIndex>=0 && decData.holidayCount==0)){
            if (!m_bShowShop) {
                m_bShowShop = true;
//                SceneManager::pushTheScene<ShopScene>();
            }
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss();
            m_pDecorationScrollView->setSelected(false);
            
//            ShopLayer* pLayer = ShopLayer::create();
//            this->addChildToUILayer(pLayer);
//            pLayer->setLocalZOrder(100);
//            pLayer->showBannerDismiss();
//            pLayer->shopDismissed = [=]()
//            {
//                if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
//                    AdsManager::getInstance()->setVisiable(kTypeBannerAds, true);
//                    
//                }
//            };
            return;
        }
    }
    AudioHelp::getInstance()->playSelectedEffect();
    std::stringstream ostr;
    ostr<<"content/category/"<<decData.pathName<<"/"<<index<<".png";
    
    if (std::strcmp(m_sCurTypePath.c_str(), "background")==0){
        std::stringstream ostr;
        ostr<<"content/category/"<<decData.pathName<<"/"<<m_sCurTypePath<<index<<".png";
        m_pBg->setTexture(ostr.str());
        GameDataManager::getInstance()->m_nBgIndex = index;
        return;
    }
    
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    m_nIndex = index;
    float delt = 100;
    m_bCanTouch = false;
    if (std::strcmp(decData.pathName.c_str(), "sprinkle")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[0]) {
            m_pGuideLayer->showGuideTap(visibleSize*0.5);
            m_bGuide[0] = true;
        }
        return;
        
    }else if (std::strcmp(decData.pathName.c_str(), "fruit")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_pGuideLayer->showGuideTap(visibleSize*0.5);
            m_bGuide[1] = true;
        }
        return;
        
    }else if (std::strcmp(decData.pathName.c_str(), "candy")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_pGuideLayer->showGuideTap(visibleSize*0.5);
            m_bGuide[2] = true;
        }
        return;
        
    }else if (std::strcmp(decData.pathName.c_str(), "extra")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
        pDecoration->setIgnoreTip();
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(500, 420));
        pDecoration->addChild(unit);
        pDecoration->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height));
        DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFoodBack,decData.single?m_sCurTypePath:"",position,1);
        pParticle->setPosition(pDecoration->getPosition());
        
        
    }else if (std::strcmp(decData.pathName.c_str(), "syrup")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(380, 355-90));
        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFood,decData.single?m_sCurTypePath:"",position,0);
        
        std::stringstream ostrFront;
        ostrFront<<"content/category/"<<m_sCurTypePath<<"/front/"<<index<<".png";
        
        Sprite* unit2 = Sprite::create(ostrFront.str());
        DecorateManager::getInstance()->addDecoration(unit2, eDecorationLayerFood,decData.single?m_sCurTypePath+"front":"",position,3);
        
        pParticle->setPosition(unit->getPosition());
        
    }else if (std::strcmp(decData.pathName.c_str(), "sticker")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(380, 260-90-90));
        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFoodFront,decData.single?m_sCurTypePath:"",position,0);
        
        pParticle->setPosition(unit->getPosition());
        
    }else if (std::strcmp(decData.pathName.c_str(), "cone")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(320+50, 200));
        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFoodFront,decData.single?m_sCurTypePath:"",position,-1);
        
        pParticle->setPosition(unit->getPosition());
        
    }else{
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(200.0f+10*(arc4random()%30)+delt, 400.0f+10*(arc4random()%20)));
        if (decData.zoomable||decData.rotatable) {
            Sprite* unit = Sprite::create(ostr.str());
            DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
            pDecoration->setIgnoreTip();
            pDecoration->addChild(unit);
            pDecoration->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height));
            if (decData.eatable) {
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFood,decData.single?m_sCurTypePath:"",position,1);
                if (std::strcmp(decData.pathName.c_str(), "topper")==0) {
                    position = CMVisibleRect::getPositionAdapted(Vec2(240.0f, 500.0f+10*(arc4random()%20)));
                    DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFoodBack,decData.single?m_sCurTypePath:"",position,0);
                }else{
                    DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFood,decData.single?m_sCurTypePath:"",position,1);
                }
                pParticle->setPosition(pDecoration->getPosition());
            }else{
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerAllUneatFront,decData.single?m_sCurTypePath:"",position,1);
                pParticle->setPosition(pDecoration->getPosition());
            }
        }else{
            Sprite* unit = Sprite::create(ostr.str());
            DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
            pDecoration->setIgnoreTip();
            pDecoration->addChild(unit);

            pDecoration->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height));
            if (decData.eatable) {
                if (std::strcmp(decData.pathName.c_str(), "topper")==0) {
                    position = CMVisibleRect::getPositionAdapted(Vec2(240.0f, 500.0f+10*(arc4random()%20)));
                    DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFood,decData.single?m_sCurTypePath:"",position,1);
                }else{
                    DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerAllFront,decData.single?m_sCurTypePath:"",position,1);
                }
                pParticle->setPosition(pDecoration->getPosition());
            }else{
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerAllUneatFront,decData.single?m_sCurTypePath:"",position,1);
                pParticle->setPosition(pDecoration->getPosition());
            }
        }
    }
    AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
    
    _playEffectNice();
    this->addChildToUILayer(pParticle);
}


void DecorateScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    
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
    if (std::strcmp(m_sCurTypePath.c_str(), "sprinkle")==0 || std::strcmp(m_sCurTypePath.c_str(), "fruit")==0 || std::strcmp(m_sCurTypePath.c_str(), "candy")==0){
        m_pGuideLayer->removeGuide();
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        Sprite* unit = Sprite::create(ostr.str());
        int angle = 20*(arc4random()%10);
        unit->setRotation(angle);
        
        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllFront,"",pos,1);
        
        AudioHelp::getInstance()->playSelectedEffect();
        _playEffectNice();
    }
    
}
void DecorateScene::_playEffectNice(float)
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
//    int radom = arc4random()%100;
//    if (radom>85&& radom<100){
//        AudioHelp::getInstance()->playEffect("nice.mp3");
//    }else  if (radom>70){
//        AudioHelp::getInstance()->playEffect("fantastic.mp3");
//    }
}