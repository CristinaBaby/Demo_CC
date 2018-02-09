
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
    Sprite* pBg = Sprite::create(localPath("dec_bk.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    m_pBg = pBg;
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(140,475));
    this->addChildToUILayer(m_pTypeScrollView);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView, 10-visibleSize.width/2, (visibleSize.height-m_pTypeScrollView->getContentSize().height)/2,kBorderLeft,kBorderBottom);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->setLocalZOrder(10);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionV);
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                  EaseBackIn::create(MoveBy::create(1, Vec2(visibleSize.width/2, 0))), NULL));
    m_pTypeScrollView->setSingleAsset(false);
    _initDefaultDecorate();
    
    m_pGameUI->showNextLayout();
    m_pGameUI->showResetLayout();
    
    return true;
}

void DecorateScene::onEnter()
{
    ExtensionScene::onEnter();
    
    m_bShowShop = false;
}

void DecorateScene::onExit()
{
    ExtensionScene::onExit();
}
#pragma mark - initData
void DecorateScene::_initData()
{
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    setExPath("content/make/dec/");
    m_nNextSceneTag = GameUIEvent::eSceneTagAddCandle;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagAddCandle;
    
}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(this->m_pContentLayer, eDecorateTypeBoth);
//    ******************      cake        **********************
    
    Sprite*pStand = Sprite::create("content/make/dec/stand.png");
    
    Sprite*pCake = Sprite::create("content/make/dec/cake1.png");
    
    std::stringstream ostrFrosting;
    ostrFrosting<<"content/make/dec/frosting"<<GameDataManager::getInstance()->m_nFrostingType<<"/"<<GameDataManager::getInstance()->m_nFrostingIndex<<".png";
    Sprite*pFrosting = Sprite::create(ostrFrosting.str());
    
    float delt = 100;
    
    Sprite*pIcing;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    pIcing = Sprite::create(FileUtility::getStoragePath()+"icing.png");
#else
    pIcing = Sprite::create(FileUtility::getStoragePath()+"/icing.png");
#endif
    pDecManager->addDecoration(pCake, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(480+delt, 330)));
    pDecManager->addDecoration(pFrosting, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(480+delt, 330)));
    if (pIcing) {
        pDecManager->addDecoration(pIcing, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(480+delt, 330)));
        
    }
    
    pDecManager->addDecoration(pStand, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(480+delt, 150)));
    

    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("common"));
    
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
        
    }else if (GameUILayoutLayer::eUIButtonTagBack==tag){
        if (GameDataManager::getInstance()->m_nDecorateStep==1){
            _reset();
            m_sCurTypeStr = "";
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
        Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
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
    AudioHelp::getInstance()->playSelectedEffect();
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/icon_"<<m_sCurTypeStr<<"/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(170, 440),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateScene::onDecorationCallback, this);
            m_pDecorationScrollView->onOKClicked = CC_CALLBACK_0(DecorateScene::onDecorateOK, this);
        }else{
            m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
        }
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionV);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateScene::onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
//        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, 5, 80,kBorderLeft);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, 50, (640-m_pDecorationScrollView->getContentSize().height)*0.5,kBorderLeft);
        m_pDecorationScrollView->setScale(0.5);
        m_pDecorationScrollView->runAction(Spawn::create(MoveBy::create(0.5, Vec2(80, 0)),
                                                         ScaleTo::create(0.5, 1), NULL));
        m_pDecorationScrollView->btnPathEX = ostr.str();
//        m_pDecorationScrollView->bgPath = "content/category/icon/dec_icon2.png";
        m_pDecorationScrollView->reloadData();
//        m_pTypeScrollView->stopAllActions();
//        CMVisibleRect::setPositionAdapted(m_pTypeScrollView, 5, 80,kBorderLeft);
//        m_pTypeScrollView->runAction(EaseBackIn::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))));
    }else{
        if (m_pDecorationScrollView) {
            m_pDecorationScrollView->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(-80, 0)),
                                                                              ScaleTo::create(0.5, 0), NULL),
                                                                CallFunc::create([=]()
                                                                                 {
                                                                                     m_pDecorationScrollView->removeFromParent();
                                                                                     m_pDecorationScrollView = nullptr;
                                                                                 }), NULL));
//            m_pDecorationScrollView->removeFromParent();
//            m_pDecorationScrollView = nullptr;
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
    ostr<<"content/category/"<<m_sCurTypeStr<<"/"<<m_sCurTypeStr<<index<<".png";
    
    if (std::strcmp(m_sCurTypeStr.c_str(), "background")==0){
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypeStr<<"/"<<m_sCurTypeStr<<index<<".png";
        m_pBg->setTexture(ostr.str());
        GameDataManager::getInstance()->m_nBgIndex = index;
        return;
    }
    
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    
    float delt = 100;
    if (std::strcmp(m_sCurTypeStr.c_str(), "other food")==0) {
        FillMaterialModel* unit = FillMaterialModel::create(ostr.str(),decData.small);
        unit->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height));
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(300.0f+10*(arc4random()%30), 200.0f+10*(arc4random()%30)));
        if (decData.eatable) {
            DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllFront,decData.single?m_sCurTypeStr:"",position,1);
            pParticle->setPosition(unit->getPosition());
        }else{
            DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllUneatFront,decData.single?m_sCurTypeStr:"",position,1);
            pParticle->setPosition(unit->getPosition());
        }
        
        
    }else{
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(300.0f+10*(arc4random()%30)+delt, 200.0f+10*(arc4random()%30)));
        if (decData.zoomable||decData.rotatable) {
            FillMaterialModel* unit = FillMaterialModel::create(ostr.str(),decData.small);
            Rect rect = Rect(0, 0, visibleSize.width, visibleSize.height);
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
            pDecoration->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height));
            if (decData.eatable) {
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFood,decData.single?m_sCurTypeStr:"",position,1);
                pParticle->setPosition(pDecoration->getPosition());
            }else{
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerAllUneatFront,decData.single?m_sCurTypeStr:"",position,1);
                pParticle->setPosition(pDecoration->getPosition());
            }
        }
    }
    
}