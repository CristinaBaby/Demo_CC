
#include "DecorateScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "StickFoodHelp.h"
#include "CandlePosition.h"

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
    m_pSprinkleMask = nullptr;
}

DecorateScene::~DecorateScene()
{
    CC_SAFE_RELEASE(m_pFoodRender);
    CC_SAFE_RELEASE(m_pSprinkleMask);
}
bool DecorateScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    GameDataManager::getInstance()->m_nBgIndex = -1;
    
    AudioHelp::getInstance()->playBackGroundMusic("music_make.mp3");
    //    bg
    m_pBg = Sprite::create(localPath("bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    m_pDecorateLayer->setPosition(Vec2(0,0));
    m_pDecorateLayer->setLocalZOrder(5);
    
//    Sprite* pSprite = Sprite::create("content/category/icon/dec_frame.png");
//    this->addChildToUILayer(pSprite);
//    CMVisibleRect::setPositionAdapted(pSprite,320,  200-visibleSize.height*0.5,kBorderNone,kBorderBottom);
//    pSprite->runAction(Sequence::create(DelayTime::create(1.5),
//                                        MoveBy::create(1, Vec2(0, visibleSize.height*0.5)),NULL));
//    pSprite->setLocalZOrder(20);
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(606,150));
    this->addChildToUILayer(m_pTypeScrollView);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_4(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
//        m_pTypeScrollView->boxPathEX = "content/category/icon/fruit_banner.png";
    //    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(30);
    if (m_nPackage == 1){
        m_pTypeScrollView->setMargin(5);
    }
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,(640-m_pTypeScrollView->getContentSize().width)/2,  150-visibleSize.height*0.5,kBorderNone,kBorderBottom);
    m_pTypeScrollView->setSelectedAnimate(false);
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
        if (m_nPackage == 0) {
            AudioHelp::getInstance()->playEffect("vo_time_decorate.mp3");
        }else{
            
        }
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        m_pGameUI->showNextLayout();
        m_pGameUI->showResetLayout(true);
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
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    if (m_nPackage == 0){
        setExPath("content/make/cupcake/dec/");
    }else if (m_nPackage==1){
        setExPath("content/make/shortcake/dec/");
    }else if (m_nPackage==2){
        setExPath("content/make/donut/dec/");
    }
    
    std::memset(m_bGuide, 0, 5);
    std::memset(m_bVOGuide, 0, 8);
    
    m_nTypeIndex = 0;
}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(m_pDecorateLayer, eDecorateTypeFood);
    //    ******************      Cupcake        **********************
    if (m_nPackage == 0) {
        m_nFlavor = GameDataManager::getInstance()->m_nFlavorIndex;
        m_nShape = GameDataManager::getInstance()->m_nShapeIndex;
        
        Sprite* pCupShadow = Sprite::create(localPath("cup_shadow.png"));
        pDecManager->addDecoration(pCupShadow, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(320,450-115)));
        
        Sprite* pCup = Sprite::create(localPath("cup_down/"+std::to_string(m_nShape+1)+".png"));
        pDecManager->addDecoration(pCup, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(320,450)));
        
        Sprite* pCupFront = Sprite::create(localPath("cup_on/"+std::to_string(m_nShape+1)+".png"));
        pDecManager->addDecoration(pCupFront, eDecorationLayerFoodFront,"",CMVisibleRect::getPositionAdapted(Vec2(320,450)));
        pCupFront->setOpacity(180);
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"cupcake.png");
        Sprite* pFood = Sprite::create(FileUtils::getInstance()->getWritablePath()+"cupcake.png");
        if (pFood) {
            pDecManager->addDecoration(pFood, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,450)));
            m_pFood = pFood;
        }
        
        m_pDecNode = Node::create();
        pDecManager->addDecoration(m_pDecNode, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,450+130)));
        
        for (int i = 0; i<3; i++) {
            std::string path = "";
            Vec2 pos = Vec2(-75, 50);
            if (i <2) {
                pos = Vec2(0, 68);
                path = "marshmallow"+std::to_string(i+1)+".png";
            }else{
                pos = Vec2(0, 78);
                path = "topping_rainbow.png";
            }
            Sprite* pAdd = Sprite::create(localPath(path));
            pAdd->setPosition(pos);
            m_pDecNode->addChild(pAdd);
            pAdd->setLocalZOrder(10-i);
        }
        Sprite* pFrosting = Sprite::create(localPath("cream/"+std::to_string(m_nFlavor+1)+".png"));
        m_pDecNode->addChild(pFrosting);
        
        currentSprinkle = AddSprinkle::create();
        m_pDecNode->addChild(currentSprinkle);
        currentSprinkle->setPosition(Vec2(-pFrosting->getContentSize().width/2, -pFrosting->getContentSize().height/2));
        currentSprinkle->setRenderNode(pFrosting);
        currentSprinkle->caculatePos(pFrosting->getContentSize());
        
        
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal",std::to_string(m_nPackage)));
        
    }else if (m_nPackage==1) {
        m_nShape = GameDataManager::getInstance()->m_nShapeIndex;
        
        Sprite*pPlate = Sprite::create(localPath("cake_plate.png"));
        pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"plate",CMVisibleRect::getPositionAdapted(Vec2(320, 350-visibleSize.height)));
        pPlate->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
        
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"cake.png");
        Sprite* pFood = Sprite::create(FileUtils::getInstance()->getWritablePath()+"cake.png");
        pFood->setAnchorPoint(Vec2(0.5, 0));
        if (pFood) {
            pDecManager->addDecoration(pFood, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,280-visibleSize.height)));
            pFood->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
            m_pFood = pFood;
        }
        
        m_pDecNode = Node::create();
        pDecManager->addDecoration(m_pDecNode, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,280)));
        
        m_pSprinkleMask = Sprite::create(localPath("cream1.png"));
        m_pSprinkleMask->setPosition(Vec2(0, pFood->getContentSize().height-200));
        m_pSprinkleMask->retain();
        m_pDecNode->addChild(m_pSprinkleMask);
        
        
        currentSprinkle = AddSprinkle::create();
        m_pDecNode->addChild(currentSprinkle);
        currentSprinkle->setPosition(Vec2(-m_pSprinkleMask->getContentSize().width/2, -m_pSprinkleMask->getContentSize().height/2+pFood->getContentSize().height-200));
        currentSprinkle->setRenderNode(m_pSprinkleMask);
        currentSprinkle->caculatePos(m_pSprinkleMask->getContentSize());
        m_pSprinkleMask->setVisible(false);
        
//        currentSprinkle = AddSprinkle::create();
//        m_pDecNode->addChild(currentSprinkle);
//        currentSprinkle->setPosition(Vec2(-m_pFood->getContentSize().width/2, -m_pFood->getContentSize().height/2));
//        currentSprinkle->setRenderNode(m_pFood);
//        currentSprinkle->caculatePos(Rect(0,m_pFood->getContentSize().height-180,m_pFood->getContentSize().width,180));
        
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("shortcake",std::to_string(m_nPackage)));
    }else if (m_nPackage==2) {
        
        m_nShape = GameDataManager::getInstance()->m_nShapeIndex;
        
        Sprite*pPlate = Sprite::create(localPath("plate.png"));
        pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"plate",CMVisibleRect::getPositionAdapted(Vec2(320, 450-visibleSize.height)));
        pPlate->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
        
//        Sprite*pShadow = Sprite::create(localPath(std::to_string(m_nShape)+"/shadow.png"));
//        pDecManager->addDecoration(pShadow, eDecorationLayerFoodBack,"plate",CMVisibleRect::getPositionAdapted(Vec2(320, 500-visibleSize.height)));
//        pShadow->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
        
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"donut.png");
        Sprite* pFood = Sprite::create(FileUtils::getInstance()->getWritablePath()+"donut.png");
        if (pFood) {
            pDecManager->addDecoration(pFood, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,500-visibleSize.height)));
            pFood->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
            m_pFood = pFood;
        }
        
        m_pDecNode = Node::create();
        pDecManager->addDecoration(m_pDecNode, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(Vec2(320,500-visibleSize.height)));
        m_pDecNode->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
        
        int flavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
        std::ostringstream ostr;
        if (flavorIndex<5) {
            ostr<<m_nShape<<"/jam"<<flavorIndex+1<<".png";
        }else{
            ostr<<m_nShape<<"/sauce"<<flavorIndex-4<<".png";
        }
        Sprite* pIcing = Sprite::create(localPath(ostr.str()));
        m_pDecNode->addChild(pIcing);
        
        currentSprinkle = AddSprinkle::create();
        m_pDecNode->addChild(currentSprinkle);
        currentSprinkle->setPosition(Vec2(-m_pFood->getContentSize().width/2, -m_pFood->getContentSize().height/2));
        currentSprinkle->setRenderNode(m_pFood);
        currentSprinkle->caculatePos(m_pFood->getContentSize());
        
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal",std::to_string(m_nPackage)));
    }
    m_CandySize = m_pFood->getContentSize();
}

void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    
    m_bCanTouch = false;
    
    for_each(m_candles.begin(),m_candles.end(),[=](Node* pNode){
        pNode->removeFromParent();
    });
    m_candles.clear();
    
    DecorateManager::getInstance()->reset();
    m_pClipping = nullptr;
    m_bPacked = false;
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->removeFromParent();
    }
    m_pDecorationScrollView = nullptr;
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
    float scale = 1.2;
    Size candySize = Size(m_CandySize.width,m_CandySize.height)*scale;
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 posOrigin = m_pDecNode->getPosition();
    
    //    Vec2 pos = Vec2(-210, -80);
    //    Vec2 pos = Vec2(-210-CMVisibleRect::getDesignOffset().x, -80-CMVisibleRect::getDesignOffset().y);
    
    Vec2 pos = Vec2(candySize.width/2,candySize.height/2);
    
    m_pDecNode->setPosition(pos);
    
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pDecNode->visit();
    render->end();
    
    m_pDecNode->setPosition(posOrigin);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    unsigned long time =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
    std::string name = StringUtils::format("%ld.png",time);
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+ "food_dec.png", false);
    pImage->autorelease();
    GameDataManager::getInstance()->setBoxName(name);
}


void DecorateScene::_displayFood(){
    AudioHelp::getInstance()->playEffect("vo_nice_design.mp3");
    
    m_pGameUI->showEatLayout();
    m_pGameUI->showSaleLayout();
    
//    m_pDecorateLayer->runAction(MoveBy::create(1, Vec2(0, 450)));
//    m_pDecorateLayer->runAction(ScaleTo::create(1, 1));
//    m_pDecorateLayer->runAction(RotateBy::create(1, Vec3(-40, 0, 0)));
    //    m_pTable->runAction(MoveBy::create(1, Vec2(0, 480)));
//    m_pBanner->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
//                                          CallFunc::create([=](){
//        
//    }), NULL));
    
    _saveFood();
    m_pGameUI->hideReset();
    m_pGameUI->hideNext();
    
}

void DecorateScene::_getFoodRenderData()
{
    Size candySize;
    Node* pFoodMask;
    if (m_nPackage == 0) {
        Sprite* pMask = Sprite::create(localPath("mask.png"));
        candySize = pMask->getContentSize();
        pFoodMask = pMask;
    }else if(m_nPackage == 1){
        Sprite* pMask = Sprite::create(localPath("cream1.png"));
        candySize = pMask->getContentSize();
        pFoodMask = pMask;
    }else if(m_nPackage == 2){
        if(m_nShape!=1){
            Sprite* pMask = Sprite::create(localPath(std::to_string(m_nShape)+"/mask.png"));
            candySize = pMask->getContentSize();
            pFoodMask = pMask;
        }else{
            Sprite* pMask = Sprite::create(localPath(std::to_string(m_nShape)+"/sauce5.png"));
            candySize = pMask->getContentSize();
            pFoodMask = pMask;
        }
    }
    m_pFoodRender = MyRenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                            GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 pos = Vec2(candySize.width/2,candySize.height/2);
    Vec2 posFood = pFoodMask->getPosition();
    
    pFoodMask->setPosition(pos);
    
    m_pFoodRender->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    pFoodMask->visit();
    m_pFoodRender->end();
    
    Director::getInstance()->getRenderer()->render();
    pFoodMask->setPosition(posFood);
    
    __NotificationCenter::getInstance()->removeObserver(m_pFoodRender, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(m_pFoodRender, EVENT_COME_TO_FOREGROUND);
    m_pFoodRender->retain();
    
}
void DecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
    if(IAPManager::getInstance()->getItemIsBought(1)){
        
        if (m_nPackage == 0){
            m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal",std::to_string(m_nPackage)));
        }else if (m_nPackage==1){
            m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("shortcake",std::to_string(m_nPackage)));
        }else if (m_nPackage==2){
            m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal",std::to_string(m_nPackage)));
        }
        
        m_pTypeScrollView->selectBtn(m_nTypeIndex);
        m_pTypeScrollView->showSelectedAnimation();
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
        GameDataManager::getInstance()->m_VectorCandle.clear();
        for_each(m_candles.begin(), m_candles.end(), [=](Node* pCandle){
            Vec3 rec = Vec3(pCandle->getPositionX(), pCandle->getPositionY(), pCandle->getTag());
            GameDataManager::getInstance()->m_VectorCandle.push_back(rec);
        });
//        m_bCanTouch = false;
//        currentSprinkle->setEnable(false);
        if(m_nPackage==2){
            _saveFood();
        }
                btn->runAction(Sequence::create(DelayTime::create(0.5),
                                                CallFunc::create([=](){
                    btn->setEnabled(true);
                }), NULL));
//        _displayFood();
//        return;
        
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
            Vec2 pos = m_pTypeScrollView->getWorldItemPos();
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("bling.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(100);
                    pParticle->setPosition(pos);
                    m_pTypeScrollView->unlcok(index);
                };
                RewardManager::getInstance()->showRewardAds(typeData.decTypeName+"_type", 0);
//                m_pDecorationScrollView->setSelected(false);
                pLayer->removeFromParent();
            };
            //点击带锁的类别 回到之前类别
            m_pTypeScrollView->setNormal();
            m_pTypeScrollView->selectBtn(m_nTypeIndex);
            m_pTypeScrollView->showSelectedAnimation();
            return;
        }
    }
    m_nTypeIndex = index;
    m_nIndex = -1;
    m_pGuideLayer->removeGuide();
    currentSprinkle->setEnable(false);
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
    CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,(640-m_pDecorationScrollView->getContentSize().width)/2,  80,kBorderNone,kBorderBottom);
    //        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
    
    m_pDecorationScrollView->btnPathEX = ostr.str();
    m_pDecorationScrollView->bgHighlightPath = "content/category/icon/p.png";
            m_pDecorationScrollView->boxPathEX = "content/category/icon/frame.png";
//    m_pDecorationScrollView->bgPath = "content/category/icon/kk0.png";
    m_pDecorationScrollView->setMargin(40);
    m_pDecorationScrollView->setLocalZOrder(20);
    
    m_pDecorationScrollView->reloadData();
    
    if (m_sCurTypePath=="fruit" || m_sCurTypePath=="candy" || m_sCurTypePath=="sprinkle"){
        
        AudioHelp::getInstance()->_playEffectNiceWork();
        
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
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("bling.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(100);
                    pParticle->setPosition(m_pDecorationScrollView->getWorldItemPos());
                    m_pDecorationScrollView->unlcok(index);
                };
                RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
                m_pDecorationScrollView->setSelected(false);
                pLayer->removeFromParent();
            };
            
            return;
        }
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    
    m_nIndex = index;
    if (m_sCurTypePath=="sprinkle"){
        m_bCanTouch = true;
        if (!m_bGuide[0]) {
            m_bGuide[0] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()+Vec2(0,m_pFood->getContentSize().height-100)));
            AudioHelp::getInstance()->playEffect("vo_decorate_sprinkle.mp3");
            //
        }else if (m_nPackage==0){
            //            MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_sugar);
            //            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        }
        currentSprinkle->setEnable(true);
        return;
    }
    currentSprinkle->setEnable(false);
    if (m_sCurTypePath=="fruit"){
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_bGuide[1] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()+Vec2(0,m_pFood->getContentSize().height-100)));
            AudioHelp::getInstance()->playEffect("vo_decorate_fruits.mp3");
            if (m_nPackage==2){
//                MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_fruit);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }else if (m_nPackage==0){
//                MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_fruit);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }
        }
        return;
    }
    if (m_sCurTypePath=="candy"){
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_bGuide[2] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()+Vec2(0,m_pFood->getContentSize().height-100)));
            AudioHelp::getInstance()->playEffect("vo_snowcone_decorate_candies.mp3");
            if (m_nPackage==2){
//                MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_candy);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }else if (m_nPackage==0){
//                MenuLayer::addIngredient(MenuLayer::ePackageCottoncandy, MenuLayer::eCottoncandy_candy);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }
        }
        return;
    }
    if (m_sCurTypePath=="candle"){
        static std::string candleType[] = {"letter","number","sample"};
        int type = 0;
        if (index<26) {
            type = 0;
        }else if (index<36) {
            type = 1;
        }else {
            type = 2;
        }
        int num = index;
        if (index<26) {
            num = index;
        }else if (index<36) {
            num = index-26;
        }else {
            num = index-36;
        }
        m_bButtonEable = false;
        {
            std::stringstream ostr;
            ostr<<"content/category/candle/"<<candleType[type]<<"/"<<num<<"_0.png";
            Sprite* pTemp = Sprite::create(ostr.str());
            this->addChildToUILayer(pTemp);
            pTemp->setPosition(m_pFood->getPosition()+Vec2(0, m_pFood->getContentSize().height-180-500));
            pTemp->runAction(Sequence::create(JumpBy::create(1, Vec2(0, 500), 300, 1),
                                              CallFunc::create([=](){
                pTemp->removeFromParent();
                m_bButtonEable = true;
            }),NULL));
        }
        std::stringstream ostr;
        ostr<<"content/category/candle/"<<candleType[type]<<"/"<<num<<"_1.png";
        Sprite* unit = Sprite::create(ostr.str());
        
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width,unit->getContentSize().height));
        pDecoration->setIgnoreTip();
        pDecoration->addChild(unit);
        this->addChildToUILayer(pDecoration);
        pDecoration->setVisible(false);
        CandleData candleData = CandleData::getCandleData(type+1);
        pDecoration->setPosition(m_pFood->getPosition()+Vec2(0, m_pFood->getContentSize().height-180));
        pDecoration->setMoveLimitRect(Rect(pDecoration->getPositionX()-m_pFood->getContentSize().width/2+40, pDecoration->getPositionY()-70+unit->getAnchorPointInPoints().y-candleData.holderHeight, m_pFood->getContentSize().width-60, 150),true);
        pDecoration->runAction(Sequence::create(DelayTime::create(1),
                                                Show::create(), NULL));
        pDecoration->setTag(index);
        m_candles.push_back(pDecoration);
        return;
    }
    
    std::stringstream ostr;
    ostr<<"content/category/"<<m_sCurTypePath<<"/"<<index<<".png";
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    
    if (m_sCurTypePath=="sprinkle"||m_sCurTypePath=="candy"||m_sCurTypePath=="fruit") {
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
//        GameDataManager::getInstance()->m_nSpoonIndex = index;
        
//        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_spoon);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    }else if (std::strcmp(decData.pathName.c_str(), "cone")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(320, 250));
        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFoodFront,decData.single?m_sCurTypePath:"",position,-1);
        
        pParticle->setPosition(unit->getPosition());
        
//        MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_cone);
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
    AudioHelp::getInstance()->playSelectedEffect();
    if (m_sCurTypePath=="fruit" || m_sCurTypePath=="candy"){
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        Sprite* unit = Sprite::create(ostr.str());
        int angle = 20*(arc4random()%10);
        unit->setRotation(angle);
        
        int alpha = 0;
        if (m_pFoodRender) {
            Vec2 posDelt = Vec2::ZERO;
            if (m_nPackage==0) {
                posDelt = Vec2(0, -180);
            }else if (m_nPackage == 2) {
//                posDelt = Vec2(0, -180);
            }else if (m_nPackage == 1) {
                posDelt = Vec2(0, -m_pFood->getContentSize().height+310);
            }
            alpha = m_pFoodRender->getAlphaWithLocation(m_pFood->convertToNodeSpace(pos)+posDelt);
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
    }else if(m_sCurTypePath=="sprinkle"){
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        
        std::vector<std::string> temps;
        if(Sprite::create(ostr.str())){
            temps.push_back(ostr.str());
        }
        for(int i=0;i<3;i++){
            std::stringstream ostr;
            ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<"_"<<i+1<<".png";
            if(Sprite::create(ostr.str())){
                temps.push_back(ostr.str());
            }
        }
        currentSprinkle ->setSprinklePath(temps);
        AudioHelp::getInstance()->playEffect("sprinkles.mp3");
        m_pGuideLayer->removeGuide();
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