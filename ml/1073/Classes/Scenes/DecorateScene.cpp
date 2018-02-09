
#include "DecorateScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "StickFoodHelp.h"

static Vec3 gHSB[20] = {
    Vec3(0, 0, 0),
    Vec3(15, 15, 0),
    Vec3(40, 15, 0),
    Vec3(55, 15, 10),
    Vec3(80, 0, 0),
    
    Vec3(115, -15, 0),
    Vec3(180, -5, 0),
    Vec3(-165, 0, 0),
    Vec3(-85, 15, 0),
    Vec3(-45, 0, 0),
    
    Vec3(45, -30, 70),
    Vec3(0, 20, 60),
    Vec3(-80, 20, 60),
    Vec3(0, 0, 0),
    Vec3(-20, 20, 20),
    
    Vec3(80, -40, -5),
    Vec3(160, -5, 40),
    Vec3(-135, 0, 0),
    Vec3(-132, -50, -10),
    Vec3(-40, 20, 60),
};
DecorateScene::DecorateScene()
{
    m_pClipping = nullptr;
    m_pDecorationScrollView = nullptr;
    m_bCanTouch = false;
    m_bPacked = false;
    m_bButtonEable = true;
    m_pFoodRender = nullptr;
}

DecorateScene::~DecorateScene()
{
    CC_SAFE_RELEASE(m_pFoodRender);
}
bool DecorateScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    GameDataManager::getInstance()->m_nBgIndex = -1;
    
    AudioHelp::getInstance()->playBackGroundMusic("background_make.mp3");
    //    bg
    if (GameDataManager::getInstance()->m_nPackage == 0) {
        m_pBg = Sprite::create("content/make/cotton/bg/bg3.jpg");
    }else{
        m_pBg = Sprite::create("content/make/snowcone/bg/bg4.png");
    }
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
        
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    m_pDecorateLayer->setPosition(Vec2(0,100));
    m_pDecorateLayer->setLocalZOrder(5);
    
    Sprite* pSprite = Sprite::create("content/category/icon/dec_frame.png");
    this->addChildToUILayer(pSprite);
    CMVisibleRect::setPositionAdapted(pSprite,320,  200-visibleSize.height*0.5,kBorderNone,kBorderBottom);
    pSprite->runAction(Sequence::create(DelayTime::create(1.5),
                                        MoveBy::create(1, Vec2(0, visibleSize.height*0.5)),NULL));
    pSprite->setLocalZOrder(20);
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(606,120));
    this->addChildToUILayer(m_pTypeScrollView);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_4(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
//    m_pTypeScrollView->boxPathEX = "content/category/icon/box_2.png";
//    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(30);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,(640-m_pTypeScrollView->getContentSize().width)/2,  80-visibleSize.height*0.5,kBorderNone,kBorderBottom);
    m_pTypeScrollView->setSelectedAnimate();
    m_pTypeScrollView->setLocalZOrder(20);
    
    _initDefaultDecorate();
    
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(1.5),
                                                  MoveBy::create(1, Vec2(0, visibleSize.height*0.5)),
                                                  DelayTime::create(0.5),
                                                  CallFunc::create([=](){
        _getFoodRenderData();
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
        if (GameDataManager::getInstance()->m_nPackage == 0) {
            AudioHelp::getInstance()->playEffect("vo_cotton_decorate.mp3");
        }else{
            
        }
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        m_pGameUI->showNextLayout();
        m_pGameUI->showResetLayout(true,false);
    }), NULL));
    GameDataManager::getInstance()->m_nDecorateStep = 0;
    
    m_pGameUI->showNormalLayout();
//    m_CandySize = Size(300+40,450+100);
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
    if (GameDataManager::getInstance()->m_nPackage == 0){
        setExPath("content/make/cotton/3/");
    }else if (GameDataManager::getInstance()->m_nPackage==2){
        setExPath("content/make/snowcone/4/");
    }
    
    std::memset(m_bGuide, 0, 5);
    std::memset(m_bVOGuide, 0, 8);
    
    m_nTypeIndex = 0;
}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(m_pDecorateLayer, eDecorateTypeFood);
    //    ******************      Cotton candy        **********************
        if (GameDataManager::getInstance()->m_nPackage == 0) {
        m_nFlavor = GameDataManager::getInstance()->m_nFlavorIndex;
        m_nShape = GameDataManager::getInstance()->m_nShapeIndex;
        
        Sprite* pStick = Sprite::create(localRootPath("2/stick.png"));
        pDecManager->addDecoration(pStick, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(320,300)));
        
        std::string cottonPath = "";
        if (m_nShape==0) {
            cottonPath = localRootPath("2/candy/"+std::to_string(m_nFlavor+1)+"_5.png");
        }else{
            cottonPath = localPath(std::to_string(m_nShape+1)+"/"+std::to_string(m_nFlavor+1)+".png");
        }
        Sprite* pCotton = Sprite::create(cottonPath);
        pDecManager->addDecoration(pCotton, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,500)));
        m_pFood = pCotton;
//        if (!m_pClipping) {
//            Sprite* pStencil = Sprite::createWithTexture(pCotton->getTexture());
//            
//            m_pClipping = ClippingNode::create(pStencil);
//        }
//        m_pClipping->setAlphaThreshold(0.5);
//        pDecManager->addDecoration(m_pClipping, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,500)));
        
            m_pDecNode = Node::create();
            pDecManager->addDecoration(m_pDecNode, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,500)));
            
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("cottoncandy"));
            
    }else if (GameDataManager::getInstance()->m_nPackage==2) {
        
        m_nShape = GameDataManager::getInstance()->m_nShapeIndex;
        Sprite*pConeBack = Sprite::create(localRootPath("2/cone0_0.png"));
        
        std::stringstream ostrSnow;
        ostrSnow<<"content/make/snowcone/3/shape"<<m_nShape<<".png";
        Sprite*pSnow = Sprite::create(ostrSnow.str());
        m_pFood = pSnow;
        
        //    pSnow->setScale(1.28);
        pDecManager->addDecoration(pSnow, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320, 560+62-visibleSize.height)));
        
        
//        Sprite*pColor = Sprite::create(FileUtils::getInstance()->getWritablePath()+"snow.png");
//        if (pColor) {
//            //            pColor->setScale(1.28);
//            pDecManager->addDecoration(pColor, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320, 555+50-visibleSize.height)));
//        }
        Sprite*pConeFront = Sprite::create(localRootPath("2/cone0_1.png"));
        
        
        pDecManager->addDecoration(pConeBack, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(320, 250-visibleSize.height)));
        pDecManager->addDecoration(pConeFront, eDecorationLayerFoodFront,"cone",CMVisibleRect::getPositionAdapted(Vec2(320, 250-visibleSize.height)));
        
        auto action = Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
        pConeBack->runAction(action);
        pSnow->runAction(action->clone());
        pConeFront->runAction(action->clone());
//        if (pColor) {
//            pColor->runAction(action->clone());
//        }
        
        m_pColorSCribble = ScribbleNode::create(pSnow->getContentSize());
        pDecManager->addDecoration(m_pColorSCribble, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320, 560+62)));
        m_pColorSCribble->useBrush("content/make/snowcone/brush.png",Scribble::BrushType::eBrush);
        
//        if (!m_pClipping) {
//            Sprite* pStencil = Sprite::createWithTexture(pSnow->getTexture());
//            
//            m_pClipping = ClippingNode::create(pStencil);
//        }
//        m_pClipping->setAlphaThreshold(0.5);
//        pDecManager->addDecoration(m_pClipping, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,560+62)));
        
        m_pDecNode = Node::create();
        pDecManager->addDecoration(m_pDecNode, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,560+62)));
        
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("snowcone"));
    }
    m_CandySize = m_pFood->getContentSize();
}

void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    GameDataManager::getInstance()->m_nSpoonIndex = -1;
    
    if (GameDataManager::getInstance()->m_nPackage==2){
        m_bCanTouch = false;
        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_fruit);
        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_candy);
        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_cone);
        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_spoon);
        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_syrup);
//        m_pColorSCribble->resetCanvas();
    }else if (GameDataManager::getInstance()->m_nPackage==0){
        MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_fruit);
        MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_candy);
    }
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
    
    Vec2 pos = Vec2(candySize.width/2-m_pFood->getPositionX(),candySize.height/2-m_pFood->getPositionY()+50);
    
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
//    m_pTable->runAction(MoveBy::create(1, Vec2(0, 480)));
    m_pBanner->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                          CallFunc::create([=](){
        _saveFood();
        m_pGameUI->hideReset();
        m_pGameUI->hideNext();
        
    }), NULL));
    
    
}

void DecorateScene::_getFoodRenderData()
{
    Size candySize = Size(m_CandySize.width,m_CandySize.height);
    m_pFoodRender = MyRenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                            GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 posOrigin = m_pFood->getPosition();
    Vec2 pos = Vec2(candySize.width/2,candySize.height/2);
    
    m_pFood->setPosition(pos);
    
    m_pFoodRender->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pFood->visit();
    m_pFoodRender->end();
    
    m_pFood->setPosition(posOrigin);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(m_pFoodRender, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(m_pFoodRender, EVENT_COME_TO_FOREGROUND);
    m_pFoodRender->retain();
//    this->addChildToContentLayer(m_pFoodRender);
//    CMVisibleRect::setPositionAdapted(m_pFoodRender, 320, 480);
//    m_pFoodRender->setPosition(m_pFood->getPosition());
}
void DecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
    if(IAPManager::getInstance()->getItemIsBought(1)){
        
        if (GameDataManager::getInstance()->m_nPackage == 0){
            m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("cottoncandy"));
        }else if (GameDataManager::getInstance()->m_nPackage==2){
            m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("snowcone"));
        }
    }
}

void DecorateScene::onButtonCallback(Button* btn)
{
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
            
//            m_pGameUI->hideNext();
//            m_pGameUI->hideReset();
            btn->runAction(Sequence::create(DelayTime::create(0.5),
                                            CallFunc::create([=](){
                btn->setEnabled(true);
            }), NULL));
//            GameDataManager::getInstance()->m_nDecorateStep=1;
        }else{
            _displayFood();
        }
        
        
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
//        Dialog* dialog=Dialog::create(Size(481,537), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
//        dialog->setContentText("Are you sure you want to reset your food?");
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
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            btn->setEnabled(true);
        }), NULL));
    }
    
}

void DecorateScene::onTypeCallback(int index,int type,DecorateTypeConfigData typeData,bool selected)
{
    m_sCurTypeStr = string(typeData.decTypeName);
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_sCurTypePath = string(data.pathName);
    AudioHelp::getInstance()->playSelectedEffect();
//    if (selected) {
        if (type==1) {
            if (!ConfigManager::getInstance()->getVideoUnLocked(typeData.decTypeName+"_type", 0)){
                ShopMiniLayer* pLayer = ShopMiniLayer::create();
                this->addChildToUILayer(pLayer);
                pLayer->setLocalZOrder(100);
                pLayer->shopGetFree = [=](){
                    RewardManager::getInstance()->showRewardAds(typeData.decTypeName+"_type", 0);
                    m_pDecorationScrollView->setSelected(false);
                    pLayer->removeFromParent();
                };
                m_pTypeScrollView->selectBtn(m_nTypeIndex);
                m_pTypeScrollView->showSelectedAnimation();
                return;
            }
        }
    m_nTypeIndex = index;
    m_nIndex = -1;
    m_pGuideLayer->removeGuide();
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/icon_"<<m_sCurTypePath<<"/";
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
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,(640-m_pDecorationScrollView->getContentSize().width)/2,  180,kBorderNone,kBorderBottom);
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
        
        m_pDecorationScrollView->btnPathEX = ostr.str();
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/kk1.png";
//        m_pDecorationScrollView->boxPathEX = "content/category/icon/frame.png";
        m_pDecorationScrollView->bgPath = "content/category/icon/kk0.png";
        m_pDecorationScrollView->setMargin(10);
        m_pDecorationScrollView->setLocalZOrder(20);
        
        m_pDecorationScrollView->reloadData();
        
        if (m_sCurTypePath=="cone"){
            if (!m_bVOGuide[4]) {
                m_bVOGuide[4] = true;
                AudioHelp::getInstance()->playEffect("vo_snowcone_decorate_cones.mp3");
            }
            return;
        }
        if (m_sCurTypePath=="spoon"){
            if (!m_bVOGuide[4]) {
                m_bVOGuide[4] = true;
                AudioHelp::getInstance()->playEffect("vo_snowcone_decorate_spoon.mp3");
            }
            return;
        }
        if (m_sCurTypePath=="syrup"){
            if (!m_bVOGuide[5]) {
                m_bVOGuide[5] = true;
                AudioHelp::getInstance()->playEffect("vo_snowcone_decorate_syrup.mp3");
            }
            return;
        }
//    }else{
//    }
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
                pLayer->removeFromParent();
            };
            
            return;
        }
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    
    m_nIndex = index;
    if (m_sCurTypePath=="sprinkles"){
        m_bCanTouch = true;
        if (!m_bGuide[0]) {
            m_bGuide[0] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()));
            AudioHelp::getInstance()->playEffect("vo_decorate_sprinkle.mp3");
            //
        }else if (GameDataManager::getInstance()->m_nPackage==0){
//            MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_sugar);
//            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        }
        return;
    }
    if (m_sCurTypePath=="fruit"){
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_bGuide[1] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()));
            AudioHelp::getInstance()->playEffect("vo_decorate_fruits.mp3");
            if (GameDataManager::getInstance()->m_nPackage==2){
                MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_fruit);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }else if (GameDataManager::getInstance()->m_nPackage==0){
                MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_fruit);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }
        }
        return;
    }
    if (m_sCurTypePath=="candy"){
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_bGuide[2] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()));
            AudioHelp::getInstance()->playEffect("vo_snowcone_decorate_candies.mp3");
            if (GameDataManager::getInstance()->m_nPackage==2){
                MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_candy);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }else if (GameDataManager::getInstance()->m_nPackage==0){
                MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_candy);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }
        }
        return;
    }
    if (m_sCurTypePath=="syrup"){
        m_bCanTouch = true;
        if (!m_bGuide[3]) {
            m_bGuide[3] = true;
            m_pGuideLayer->showGuideMove(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()), m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition())+Vec2(-50,-100));
            MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_syrup);
            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        }
        CCImageColorSpace cs;
        Sprite* pColor;
        std::stringstream ostr;
        if (index==14) {
            ostr<<"shape"<<m_nShape<<"_color14.png";
        }else{
            ostr<<"shape"<<m_nShape<<"_color.png";
        }
        Vec3 hsb = gHSB[index-1];
        Texture2D* texture = cs.textureWithHSB(localPath(ostr.str()), hsb.x, hsb.y, hsb.z);
        pColor = Sprite::createWithTexture(texture);
        pColor->setPosition(pColor->getContentSize()*0.5);
        m_pColorSCribble->useTarget(pColor);
        return;
    }
    
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
        Rect rect = Rect(-m_pFood->getAnchorPointInPoints().x+30, -m_pFood->getAnchorPointInPoints().y+20, m_pFood->getContentSize().width-60, m_pFood->getContentSize().height-20);
        
        unit->setMoveLimitRect(rect);
        m_pClipping->addChild(unit);
        Vec2 position = Vec2(-50.0f+10*(arc4random()%10), -100.0f+10*(arc4random()%20));
        unit->setPosition(position);
        pParticle->setPosition(m_pClipping->getPosition()+m_pDecorateLayer->getPosition()+position);
        
    }else if (std::strcmp(decData.pathName.c_str(), "spoon")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(400, 500));
        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFoodFront,decData.single?m_sCurTypePath:"",position,0);
        
        pParticle->setPosition(unit->getPosition());
        GameDataManager::getInstance()->m_nSpoonIndex = index;
        
        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_spoon);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    }else if (std::strcmp(decData.pathName.c_str(), "cone")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(320, 250));
        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFoodFront,decData.single?m_sCurTypePath:"",position,-1);
        
        pParticle->setPosition(unit->getPosition());
        
        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_cone);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
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
    if (!m_bCanTouch) {
        return;
    }
    if (m_nIndex<0) {
        return;
    }
    if (m_sCurTypePath=="syrup"){
        Rect rect = m_pFood->getBoundingBox();
        rect.origin = m_pFood->getParent()->convertToWorldSpace(rect.origin);
        Vec2 pos = pTouch->getLocation();
        if (rect.containsPoint(pos)) {
            m_pColorSCribble->paint(pos);
            Director::getInstance()->getRenderer()->render();
            Sprite* pDot = Sprite::create(localPath("color"+std::to_string(m_nIndex)+".png"));
            this->addChildToUILayer(pDot);
            pDot->setPosition(pos);
            pDot->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 1.2),
                                                           FadeOut::create(0.5), NULL),
                                             CallFuncN::create([=](Node* pNode){
                pNode->removeFromParent();
            }), NULL));
            m_nMoveCount = 0;
        }
        
    }
}

void DecorateScene::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if (m_nIndex<0) {
        return;
    }
    m_pGuideLayer->removeGuide();
    if (m_sCurTypePath=="syrup"){
        Rect rect = m_pFood->getBoundingBox();
        rect.origin = m_pFood->getParent()->convertToWorldSpace(rect.origin);
        Vec2 pos = pTouch->getLocation();
        if (rect.containsPoint(pos)) {
            m_pColorSCribble->paint(pTouch->getPreviousLocation(),pos);
            Director::getInstance()->getRenderer()->render();
        }
        m_nMoveCount++;
        if (m_nMoveCount%5==0) {            
            Sprite* pDot = Sprite::create(localPath("color"+std::to_string(m_nIndex)+".png"));
            this->addChildToUILayer(pDot);
            pDot->setPosition(pos);
            pDot->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 1.2),
                                                           FadeOut::create(0.5), NULL),
                                             CallFuncN::create([=](Node* pNode){
                pNode->removeFromParent();
            }), NULL));
        }
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
//    if (pos.x>visibleSize.width-100) {
//        return;
//    }
    if (m_sCurTypePath=="sprinkles" || m_sCurTypePath=="fruit" || m_sCurTypePath=="candy"){
//        m_pGuideLayer->removeGuide();
//        std::stringstream ostr;
//        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
//        Sprite* unit = Sprite::create(ostr.str());
//        int angle = 20*(arc4random()%10);
//        unit->setRotation(angle);
//        unit->setPosition(m_pClipping->convertToNodeSpace(pos));
//        m_pClipping->addChild(unit);
        
//        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllFront,"",pos,1);
        
        AudioHelp::getInstance()->playSelectedEffect();
        
        
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        Sprite* unit = Sprite::create(ostr.str());
        int angle = 20*(arc4random()%10);
        unit->setRotation(angle);

        int alpha = 0;
        if (m_pFoodRender) {
            alpha = m_pFoodRender->getAlphaWithLocation(m_pFood->convertToNodeSpace(pos));
        }
        if(alpha>0){
            unit->setPosition(m_pDecNode->convertToNodeSpace(pos));
            m_pDecNode->addChild(unit);
            m_pGuideLayer->removeGuide();
        }else{
            unit->setPosition(m_pContentLayer->convertToNodeSpace(pos));
            this->addChildToContentLayer(unit);
            unit->setLocalZOrder(20);
            unit->runAction(Sequence::create(Spawn::create(MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                                           RotateBy::create(1, 360), NULL),
                                             CallFunc::create([=](){
                unit->removeFromParent();
            }), NULL));
        }
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