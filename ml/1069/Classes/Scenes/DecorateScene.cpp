
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
    m_bPacked = false;
    m_bCanDecorate = true;
    m_pScribble = nullptr;
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
    Sprite* pBg;
    if (GameDataManager::getInstance()->m_nPackage==0){
        pBg = Sprite::create(localPath("dec_bg.jpg"));
    }else{
        pBg = Sprite::create(localPath("bg.png"));
    }
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
//    Sprite* pTable = Sprite::create(localPath("table.png"));
//    CMVisibleRect::setPosition(pTable, 320, 180);
//    this->addChildToBGLayer(pTable);
    m_pBg = pBg;
    
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    m_pDecorateLayer->setPosition(Vec2(50,0));
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(210,480));
    this->addChildToUILayer(m_pTypeScrollView);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  -10-visibleSize.width/2,(640-m_pTypeScrollView->getContentSize().height)/2-50,kBorderLeft);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->boxPathEX = "content/category/icon/box_1.png";
    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionV);
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                  EaseBackIn::create(MoveBy::create(1, Vec2(visibleSize.width/2, 0))),
                                                  DelayTime::create(2), NULL));
    m_pTypeScrollView->setSingleAsset(false);
    m_pTypeScrollView->setMargin(-10);
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("pack"));

    _initDefaultDecorate();
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGameUI->showNextLayout();
        m_pGameUI->showResetLayout();
        AudioHelp::getInstance()->playEffect("vo_pack_pizza.mp3");
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/dysc.png",CMVisibleRect::getPosition(320+visibleSize.width, 650));
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(DecorateScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(DecorateScene::TouchMoved, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(DecorateScene::TouchEnded, this);
    
    m_boxSize = Size(542,326);
    m_pGameUI->showNormalLayout();
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
    std::memset(m_bVOGuide, 0, 4);
    GameDataManager::getInstance()->m_bPacked = false;
}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(m_pDecorateLayer, eDecorateTypeBoth);
//    ******************      normal pizza       **********************
    if (GameDataManager::getInstance()->m_nPackage==0) {
        Sprite*pPlate = Sprite::create(localPath("bottom.png"));
        m_pPlate = pPlate;
        
        pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(530, 235-visibleSize.height)),-1);
        
        Sprite*pPia = Sprite::create(localPath("pizza.png"));
        pDecManager->addDecoration(pPia, eDecorationLayerAllFront,"",CMVisibleRect::getPositionAdapted(Vec2(530, 240-visibleSize.height)),-1);
        m_pPizza = pPia;
        
        std::string str;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        str = FileUtility::getStoragePath()+"normal_decripe.png";
#else
        str = FileUtility::getStoragePath()+"/normal_decripe.png";
#endif
        Sprite*pPizza = Sprite::create(str);
        
        //        380,260
        if (pPizza) {
            pPizza->setScale(0.9, 0.6);
            //            pPizza->setRotation3D(Vec3(-50, 0, 0));
//            pDecManager->addDecoration(pPizza, eDecorationLayerAllFront,"",CMVisibleRect::getPositionAdapted(Vec2(530, 250-visibleSize.height)),-1);
            pPia->addChild(pPizza);
            pPizza->setPosition(pPia->getAnchorPointInPoints()+Vec2(0, 32));
        }
        
        auto action = Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
        pPlate->runAction(action);
        pPia->runAction(action->clone());
//        if (pPizza) {
//            pPizza->runAction(action->clone());
//        }

    }else {
//    gummy pizza
        Sprite*pPlate = Sprite::create(localPath("palte.png"));
        m_pPlate = pPlate;
        
        std::string str;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        str = FileUtility::getStoragePath()+"gummypiece.png";
#else
        str = FileUtility::getStoragePath()+"/gummypiece.png";
#endif
        Sprite*pPizza = Sprite::create(str);
        
        pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(Vec2(530, 235-visibleSize.height)),-1);
        
//        380,260
        if (pPizza) {
            pPizza->setScale(1, 0.75);
//            pPizza->setRotation3D(Vec3(-50, 0, 0));
            pDecManager->addDecoration(pPizza, eDecorationLayerAllFront,"",CMVisibleRect::getPositionAdapted(Vec2(530, 250-visibleSize.height)),-1);
            m_pPizza = pPizza;
        }
        
        auto action = Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
        pPlate->runAction(action);
        if (pPizza) {
            pPizza->runAction(action->clone());
        }
    }

}
void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    _unpack();
    m_bCanDecorate = false;
    m_bPacked = false;
    GameDataManager::getInstance()->m_bPacked = false;
    
    this->runAction(Sequence::create(DelayTime::create(1.2),
                                     CallFunc::create([=](){
        DecorateManager::getInstance()->reset();
        
        m_pScribble = nullptr;
        _initDefaultDecorate();
        m_bCanDecorate = true;
        
    }), NULL));
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


void DecorateScene::_showBox()
{
    GameDataManager::getInstance()->m_bPacked = true;
    m_pBox = Sprite::create(localPath("box2.png"));
    this->addChildToContentLayer(m_pBox);
    CMVisibleRect::setPositionAdapted(m_pBox, 500+50, 600+visibleSize.height);
    m_pBox->setLocalZOrder(-1);
    
    m_pBoxFront = Sprite::create(localPath("box2_2.png"));
    this->addChildToContentLayer(m_pBoxFront);
    CMVisibleRect::setPositionAdapted(m_pBoxFront, 500+50, 600+visibleSize.height);
    m_pBoxFront->setLocalZOrder(-1);
    auto action = MoveBy::create(1, Vec2(0, -visibleSize.height));
    m_pBox->runAction(action);
    m_pBoxFront->runAction(action->clone());
    
    m_pBoxNode = Node::create();
    m_pDecorateLayer->addChild(m_pBoxNode);
    m_pBoxNode->setLocalZOrder(10);
    CMVisibleRect::setPosition(m_pBoxNode, 530, 240);
}

void DecorateScene::_moveBox()
{
    auto action = Sequence::create(MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                   CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    m_pBox->runAction(action);
    m_pBoxFront->runAction(action->clone());
}

void DecorateScene::_pack()
{
    m_pPizza->runAction(Sequence::create(JumpBy::create(0.8, Vec2::ZERO, 150, 1),
                                         CallFunc::create([=](){
        m_pPizza->setVisible(false);
        
        m_pBox->setLocalZOrder(5);
    }), NULL));
    
    m_pPlate->runAction(Sequence::create(MoveBy::create(0.3, Vec2(0, -180)),
                                         MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
//        m_pPlate->setPosition(m_pPlate->getPosition()+Vec2(-visibleSize.width, 100));
    }), NULL));
    m_pBoxFront->runAction(MoveBy::create(0.4, Vec2(0, -240)));
    m_pBox->runAction(Sequence::create(MoveBy::create(0.4, Vec2(0, -240)),
                                       AnimationHelp::createAnimate(localPath("box"), 2, 0,false,true,0.2),
                                       CallFunc::create([=](){
        m_pBoxFront->setVisible(false);
        m_pBox->setVisible(false);
        Sprite* pBoxShadow = Sprite::create("content/category/color/box_shadow.png");
        m_pBoxNode->addChild(pBoxShadow,0);
        pBoxShadow->setName("shadow");
        
        Sprite* pBox = Sprite::create("content/category/color/1.png");
        m_pBoxNode->addChild(pBox,1);
        pBox->setName("box");
        
        Sprite* pRope = Sprite::create(localPath("rope.png"));
        m_pBoxNode->addChild(pRope,5);
        pRope->setPosition(Vec2(-6, 16));
        pRope->setName("rop");
//        m_pPizza->setVisible(true);
    }), NULL));
}

void DecorateScene::_unpack()
{
    if(m_pBoxNode){
        m_pBoxNode->removeAllChildren();
        m_pBox->setVisible(true);
        _moveBox();
    }
    m_pPizza->setVisible(false);
    
//    m_pPizza->runAction(Sequence::create(DelayTime::create(0.2),
//                                         CallFunc::create([=](){
//        m_pPizza->setVisible(true);
//    }),
//                                         DelayTime::create(0.4),
//                                         
//                                         JumpBy::create(1, Vec2::ZERO, 150, 1), NULL));
//    
//    m_pPlate->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
//                                         MoveBy::create(0.5, Vec2(0, -100)),
//                                         NULL));
//    
//    m_pBoxFront->setVisible(true);
//    m_pBox->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("box"), 0, 1,false,true,0.2),
//                                       CallFunc::create([=](){
//        m_pBox->setLocalZOrder(-1);
//        m_pBoxFront->setLocalZOrder(-1);
//        m_pBoxFront->runAction(MoveBy::create(0.4, Vec2(0, 100)));
//    }),
//                                       MoveBy::create(0.4, Vec2(0, 100)),
//                                       CallFunc::create([=](){
//        _moveBox();
//    }),
//                                       NULL));
}


void DecorateScene::_addDecoration(int index,DecorateConfigData decData)
{
    AudioHelp::getInstance()->playSelectedEffect();
    std::stringstream ostr;
    ostr<<"content/category/"<<decData.pathName<<"/"<<index<<".png";
    
    
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    pParticle->setPosition(m_pBoxNode->getPosition());
    m_nIndex = index;
    
    if(std::strcmp(decData.pathName.c_str(), "color")==0){
        Sprite* pBox = (Sprite*)m_pBoxNode->getChildByName("box");
        if (pBox) {
            pBox->setTexture(ostr.str());
        }else{
            Sprite* pBox = Sprite::create(ostr.str());
            m_pBoxNode->addChild(pBox,1);
            pBox->setName("box");
        }
    }else if(std::strcmp(decData.pathName.c_str(), "stickers")==0){
        Sprite* pUnit = (Sprite*)m_pBoxNode->getChildByName("stickers");
        if (pUnit) {
            pUnit->setTexture(ostr.str());
        }else{
            Sprite* pUnit = Sprite::create(ostr.str());
            m_pBoxNode->addChild(pUnit,2);
            pUnit->setName("stickers");
            pUnit->setPosition(Vec2(0, 15));
        }
        
    }else if(std::strcmp(decData.pathName.c_str(), "bowknot")==0){
        Sprite* pContent = Sprite::create(ostr.str());
        DecorationUtil* pUnit = DecorationUtil::createDefault(pContent->getContentSize());
        pUnit->setIgnoreTip();
        pUnit->addChild(pContent);
        
        Sprite* pBox = (Sprite*)m_pBoxNode->getChildByName("box");
        Rect rect = pBox->getBoundingBox();
        rect.origin = m_pBoxNode->convertToWorldSpace(rect.origin);
        pUnit->setMoveLimitRect(pBox->getBoundingBox());
        
        //            pUnit->setPosition(<#const cocos2d::Vec2 &position#>)
        m_pBoxNode->addChild(pUnit,6);
    }else if(std::strcmp(decData.pathName.c_str(), "pen")==0){
        
        if(!m_pScribble){
            m_pScribble = ScribbleShapeNode::create(visibleSize);
            m_pBoxNode->addChild(m_pScribble,3);
            m_pScribble->useTarget("content/category/color/1.png");//指定将要绘制的对象（衣服纹理）
            m_pScribble->setShape("content/category/color/1.png");
            m_pScribble->antiAliasing();
        }
        if(index==0){
            if (m_pScribble) {
                m_pScribble->useBrush("content/category/brush.png",Scribble::BrushType::eEaser,0.5);
                m_pScribble->useTargetColor(Color4F(255,255,255,0));
            }
            
        }else{
//            m_pScribble->useBrush("content/category/brush.png",Scribble::BrushType::eBrush,0.5);
            Color4B color = ConfigManager::getInstance()->getColor().at(index-1);
            m_pScribble->useBrush("content/make/brush.png",Scribble::eBrush,0.15);
            m_pScribble->useTargetColor(Color4F(color));
        }
        
        m_bCanTouch = true;
        return;
    }
    
    AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
    
    this->addChildToUILayer(pParticle);
}

void DecorateScene::_saveBox()
{
    Size mixtureSize = Size(m_boxSize.width+100,m_boxSize.height+100);
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_bPacked) {
        Vec2 pos = m_pBoxNode->getPosition();
        m_pBoxNode->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        
        render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        m_pBoxNode->visit();
        render->end();
        
        m_pBoxNode->setPosition(pos);
    }else{
        Vec2 pos = CMVisibleRect::getPosition(530, 240);
        m_pContentLayer->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2)-pos);
        
        render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        m_pContentLayer->visit();
        render->end();
        
        m_pContentLayer->setPosition(Vec2::ZERO);
    }
    
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
    issuccess = pImage->saveToFile(FileUtility::getStoragePath()+ name, false);
#else
    issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"/"+ name, false);
#endif
    pImage->autorelease();
    GameDataManager::getInstance()->setBoxName(name);
}

void DecorateScene::_savePizza(){
    
}
void DecorateScene::_displayPizza()
{
    AudioHelp::getInstance()->playEffect("vo_nice_design.mp3");
    m_bCanDecorate = false;
    m_pTypeScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                                  DelayTime::create(0.5),
                                                  CallFunc::create([=](){
        
//        m_pGameUI->showEatLayout();
//        m_pGameUI->showSaleLayout();
        m_bCanDecorate = true;
        AudioHelp::getInstance()->playEffect("vo_eat_sell.mp3");
    }), NULL));
    
    m_pGameUI->showEatLayout();
    m_pGameUI->showSaleLayout();
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)));
    }
    m_pDecorateLayer->runAction(MoveBy::create(0.5, Vec2(-50, 100)));
    
    m_pGameUI->hideReset();
    m_pGameUI->hideNext();
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
    if(!m_bCanDecorate){
        return;
    }
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        _displayPizza();
        _saveBox();
        return;
        
    }else if (GameUILayoutLayer::eUIButtonTagEat==tag) {
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        m_pPizza->setVisible(true);
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
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(250, 430),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = data;
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateScene::onDecorationCallback, this);
        }else{
            m_pDecorationScrollView->decorationData = data;
        }
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionV);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateScene::onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        m_pDecorationScrollView->setMargin(10);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, (640-m_pDecorationScrollView->getContentSize().width)*0.5,720+visibleSize.height/2);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,  120-visibleSize.width/2,(640-m_pDecorationScrollView->getContentSize().height)/2-50,kBorderLeft);
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
        m_pDecorationScrollView->btnPathEX = ostr.str();
        
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
        m_pDecorationScrollView->boxPathEX = "content/category/icon/box_2.png";
        m_pDecorationScrollView->setMargin(30);
        m_pDecorationScrollView->reloadData();
        
        if (typeData.decTypeName=="color") {
            if (!m_bVOGuide[1]) {
                m_bVOGuide[1] = true;
                AudioHelp::getInstance()->playEffect("vo_choose_box.mp3");
            }
        }else if (typeData.decTypeName=="pen") {
            if (!m_bVOGuide[2]) {
                m_bVOGuide[2] = true;
                AudioHelp::getInstance()->playEffect("vo_draw_box.mp3");
            }
        }else if (typeData.decTypeName=="stickers") {
            if (!m_bVOGuide[3]) {
                m_bVOGuide[3] = true;
                AudioHelp::getInstance()->playEffect("vo_stickers_box.mp3");
            }
        }
    }else{
        m_sCurTypePath = "";
        if (m_pDecorationScrollView) {
            m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2,0)),
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
            int type = 0;
            if(decData.videoCount>0){
                type = (index)%2;
            }
            if (type==0) {
                ShopLayer* pLayer = ShopLayer::create();
                this->addChildToUILayer(pLayer);
                pLayer->setLocalZOrder(100);
                pLayer->showBannerDismiss();
                m_pDecorationScrollView->setSelected(false);
                return;
            }else if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
                RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
                m_pDecorationScrollView->setSelected(false);
                return;
            }
        }
    }
    m_bCanTouch = false;
    if (!m_bCanDecorate) {
        return;
    }
    if(!m_bPacked){
        m_bPacked = true;
        m_bCanDecorate = false;
        _showBox();
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            _pack();
        }),
                                         DelayTime::create(1.3),
                                         CallFunc::create([=](){
            _addDecoration(index,decData);
            
            m_bCanDecorate = true;
        }),  NULL));
    }else{
        m_bCanDecorate = true;
        _addDecoration(index,decData);
    }
}


void DecorateScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanDecorate) {
        return;
    }
    if (m_sCurTypeStr=="pen" || m_sCurTypeStr=="eraser"){
        if (m_pScribble) {
            if (m_nIndex>=0){
                Point lTouchPoint = this->convertToWorldSpace(pTouch->getLocation());
                m_pScribble->paint(lTouchPoint);
            }
        }
    }
}
void DecorateScene::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanDecorate) {
        return;
    }
    if (!m_bCanTouch) {
        return;
    }
    if (m_nIndex<0) {
        return;
    }
    if (m_sCurTypeStr=="pen" || m_sCurTypeStr=="eraser"){
        if (m_pScribble) {
            if (m_nIndex>=0){
                Point lTouchPoint = this->convertToWorldSpace(pTouch->getLocation());
                Point lPreTouchPoint = this->convertToWorldSpace(pTouch->getPreviousLocation());
                m_pScribble->paint(lTouchPoint, lPreTouchPoint);
            }
        }
    }
}
void DecorateScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanDecorate) {
        return;
    }
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
        
    }
    
}