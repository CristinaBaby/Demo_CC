
#include "DecorateScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "StickFoodHelp.h"


static std::string gsModels[] = {
    "round",
    "heart",
    "cat",
    "star",
    "ginger",
    "tree",
};
DecorateScene::DecorateScene()
{
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
    
    //    bg
    m_pBg = Sprite::create(localPath("bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
        
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    m_pDecorateLayer->setPosition(Vec2(0,0));
    m_pDecorateLayer->setLocalZOrder(5);
    
    m_pBanner = Sprite::create("content/category/icon/dec_frame1.png");
    this->addChildToUILayer(m_pBanner);
    CMVisibleRect::setPositionAdapted(m_pBanner,320,  820+visibleSize.height,kBorderNone,kBorderTop);
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(visibleSize.width,300));
    this->addChildToUILayer(m_pTypeScrollView);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_4(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
//    m_pTypeScrollView->boxPathEX = "content/category/icon/dec_frame1.png";
//    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(30);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,(640-m_pTypeScrollView->getContentSize().width)/2,  710+visibleSize.height,kBorderNone,kBorderTop);
    
    m_pTypeScrollView->setSelectedAnimate();
    
    _initDefaultDecorate();
    
    m_pBanner->setLocalZOrder(11);
    m_pTypeScrollView->setLocalZOrder(11);
    m_pBanner->runAction(Sequence::create(DelayTime::create(1.5),
                                          EaseElasticOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height)), 1.0f), NULL));
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(1.5),
                                                  EaseElasticOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height)), 1.0f),
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
        
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_decorate_cookies.mp3");
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        m_pGameUI->showNextLayout(true,false);
        m_pGameUI->showResetLayout();
    }), NULL));
    GameDataManager::getInstance()->m_nDecorateStep = 0;
    
//    m_pGameUI->showNormalLayout();
    m_CandySize = Size(300+40,450+100);
    m_nNextSceneTag = GameUIEvent::eSceneTagShare;
    AudioHelp::getInstance()->playBackGroundMusic("music_make.mp3");
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
    m_nShape = GameDataManager::getInstance()->m_nShapeIndex;
    m_nFlavor = GameDataManager::getInstance()->m_nFlavorIndex;
    setExPath("content/make/classic/dec/");
    
    std::memset(m_bGuide, 0, 3);
    std::memset(m_bVOGuide, 0, 8);
    GameDataManager::getInstance()->m_nPackage = false;
    m_nTypeIndex = 0;

}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(m_pDecorateLayer, eDecorateTypeFood);
    //    ******************      Food        **********************
    Sprite*pPlate = Sprite::create("content/category/plate/1.png");
    std::stringstream ostr;
    if(m_nFlavor == 4){
        ostr<<"chocolate_chip.png";
    }else{
        ostr<<gsModels[m_nShape]<<"_"<<m_nFlavor+1<<".png";
    }
    Sprite*pCookie = Sprite::create(localPath(ostr.str()));
    pCookie->setFlipX(true);
    
    m_pFood = pCookie;
    
    Vec2 pos = Vec2(320, 400);
    
    pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"plate",CMVisibleRect::getPositionAdapted(pos));
    pDecManager->addDecoration(pCookie, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos));
    
    ActionHelper::showBezier(pPlate, pPlate->getPosition(), ActionHelper::ShowDirection::show_from_bottom);
    ActionHelper::showBezier(pCookie, pCookie->getPosition(), ActionHelper::ShowDirection::show_from_bottom);
    
    m_pDecNode = Node::create();
    pDecManager->addDecoration(m_pDecNode, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos));
    
    m_pIcingScribble = ScribbleNode::create(Sprite::create("content/category/icing/"+gsModels[m_nShape]+"/1.png")->getContentSize());
//    m_pIcingScribble = ScribbleNode::create(m_pFood->getContentSize()*1.1);
    m_pIcingScribble->useBrush("content/brush.png",Scribble::BrushType::eBrush);
    m_pDecNode->addChild(m_pIcingScribble);
    
    if(m_nShape == 1){
        m_pIcingScribble->setPosition(Vec2(4, -15));
    }else if(m_nShape == 2){
        m_pIcingScribble->setPosition(Vec2(6, 0));
    }else if(m_nShape == 3){
        m_pIcingScribble->setPosition(Vec2(6, 4));
    }else if(m_nShape == 4){
        m_pIcingScribble->setPosition(Vec2(-3, 22));
    }else if(m_nShape == 5){
        m_pIcingScribble->setPosition(Vec2(8, 10));
    }else{
        m_pIcingScribble->setPosition(Vec2(6, -3));
    }
    
    currentSprinkle = AddSprinkle::create();
    m_pDecNode->addChild(currentSprinkle);
    currentSprinkle->setPosition(Vec2(-m_pFood->getContentSize().width/2, -m_pFood->getContentSize().height/2));
    currentSprinkle->setRenderNode(m_pFood);
    currentSprinkle->caculatePos(m_pFood->getContentSize());
    
    if (m_nFlavor == 4) {
        m_nShape = 0;
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal",std::to_string(m_nFlavor)));
    }else{
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal",std::to_string(m_nFlavor),gsModels[m_nShape]));
    }
}

void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    
    DecorateManager::getInstance()->reset();
//    m_pClipping = nullptr;
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

void DecorateScene::_getFoodRenderData()
{
    Size candySize = m_pFood->getContentSize();
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
}

void DecorateScene::_saveFood()
{
    float scale = 1;
    Size candySize = m_pFood->getContentSize()*1.2;
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 posOrigin = m_pDecNode->getPosition();
    Vec2 posCake = m_pFood->getPosition();
    Vec2 offset = posOrigin - posCake;
//    Vec2 pos = Vec2(-210, -80);
//    Vec2 pos = Vec2(-210-CMVisibleRect::getDesignOffset().x, -80-CMVisibleRect::getDesignOffset().y);
    
    Vec2 pos = Vec2(candySize.width/2,candySize.height/2);
    
    m_pDecNode->setPosition(pos+offset);
    m_pFood->setPosition(pos);
    
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pFood->visit();
    m_pDecNode->visit();
    render->end();
    
    m_pDecNode->setPosition(posOrigin);
    m_pFood->setPosition(posCake);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+ "dec.png", false);
    pImage->autorelease();
}


void DecorateScene::_displayFood(){
    AudioHelp::getInstance()->playEffect("vo_nice_design.mp3");
    
    m_pGameUI->showEatLayout();
    m_pGameUI->showSaleLayout();
    
    if (m_pTypeScrollView){
        m_pTypeScrollView->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)));
    }
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)));
    }
    if (m_pBanner) {
        m_pBanner->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)));
    }
    _saveFood();
    m_pGameUI->hideReset();
    m_pGameUI->hideNext();
    
    AudioHelp::getInstance()->playEffect("vo_eat_serve.mp3");
    
}
void DecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypePath);
    m_pDecorationScrollView->reloadData();
    if (IAPManager::getInstance()->getItemIsBought(1)) {
        m_pTypeScrollView->unlock();
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
        m_bCanTouch = false;
        currentSprinkle->setEnable(false);
//        _saveFood();
//        btn->runAction(Sequence::create(DelayTime::create(0.5),
//                                        CallFunc::create([=](){
//            btn->setEnabled(true);
//        }), NULL));
        _displayFood();
        return;
    }else if (GameUILayoutLayer::eUIButtonTagSale==tag) {
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                             m_bButtonEable = true;
                                                         }), NULL));
        SceneManager::pushTheScene<SaleScene>();
    }else if (GameUILayoutLayer::eUIButtonTagEat==tag) {
        btn->setEnabled(false);
        
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        SceneManager::pushTheScene<ShareScene>();
    }else if (GameUILayoutLayer::eUIButtonTagBack==tag){
//        if (GameDataManager::getInstance()->m_nDecorateStep==1){
//            _reset();
//            m_sCurTypeStr = "";
//            m_pGameUI->hideBack();
//            GameDataManager::getInstance()->m_nDecorateStep = 0;
//            if (m_pDecorationScrollView) {
//                m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 450)),
//                                                                    CallFunc::create([=]()
//                                                                                     {
//                                                                                         m_pDecorationScrollView->removeFromParent();
//                                                                                         m_pDecorationScrollView = nullptr;
//                                                                                     }), NULL));
//            }
//            m_pTypeScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 200)),
//                                                          CallFunc::create([=]()
//                                                                           {
//                                                                               m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal"));
//                                                                           }),
//                                                          EaseBackIn::create(MoveBy::create(1, Vec2(0, -200))), NULL));
//            return;
//        }
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

void DecorateScene::onTypeCallback(int index,int type,DecorateTypeConfigData typeData,bool selected)
{
    m_sCurTypeStr = string(typeData.decTypeName);
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_sCurTypePath = string(data.pathName);
    AudioHelp::getInstance()->playSelectedEffect();
    if (type==1) {
        if (!ConfigManager::getInstance()->getVideoUnLocked(typeData.decTypeName+"_type", 0)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            Vec2 pos = m_pTypeScrollView->getWorldItemPos();
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("basic/start.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(100);
                    pParticle->setPosition(pos);
                };
                RewardManager::getInstance()->showRewardAds(typeData.decTypeName+"_type", 0);
                m_pDecorationScrollView->setSelected(false);
                pLayer->removeFromParent();
            };
            //点击带锁的类别 回到之前类别
            m_pTypeScrollView->selectBtn(m_nTypeIndex);
            m_pTypeScrollView->showSelectedAnimation();
            return;
        }
    }
    
    m_nTypeIndex = index;
    m_nIndex = -1;
    m_pGuideLayer->removeGuide();
    currentSprinkle->setEnable(false);
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/"<<m_sCurTypePath<<"_icon/";
        if(m_sCurTypePath == "icing"){
            ostr<<gsModels[m_nShape]<<"/";
        }
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(590,150),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onDecorationCallback, this);
            m_pDecorationScrollView->onOKClicked = CC_CALLBACK_0(DecorateScene::onDecorateOK, this);
        }else{
            m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
        }
        m_pDecorationScrollView->decorationData.display();
        m_pDecorationScrollView->setLocalZOrder(11);
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        //        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, 5, 80,kBorderLeft);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,(640-m_pDecorationScrollView->getContentSize().width)/2,  690,kBorderNone,kBorderTop);
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
        
        m_pDecorationScrollView->btnPathEX = ostr.str();
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/xx.png";
        m_pDecorationScrollView->boxPathEX = "content/category/icon/dec_frame2.png";
//        m_pDecorationScrollView->bgPath = "content/category/icon/box2_1.png";
        m_pDecorationScrollView->setMargin(40);
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
        log("===========%s",decData.decTypeName.c_str());
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            
            ShopMiniLayer* pLayer = ShopMiniLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("basic/start.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(100);
                    pParticle->setPosition(m_pDecorationScrollView->getWorldItemPos());
                    
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
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()));
        }
//        if (!m_bVOGuide[0]) {
//            m_bVOGuide[0] = true;
//            AudioHelp::getInstance()->playEffect("vo_decorate_sprinkle.mp3");
//        }
        return;
    }
    if (m_sCurTypePath=="fruit"){
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_bGuide[1] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()));
        }
//        if (!m_bVOGuide[1]) {
//            m_bVOGuide[1] = true;
//            AudioHelp::getInstance()->playEffect("vo_decorate_candy.mp3");
//        }
        return;
    }
    if (m_sCurTypePath=="icing"){
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_bGuide[2] = true;
            m_pGuideLayer->showGuideMove(m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition()),m_pDecorateLayer->convertToWorldSpace(m_pFood->getPosition())+Vec2(100, 100));
        }
//        if (!m_bVOGuide[2]) {
//            m_bVOGuide[2] = true;
//            AudioHelp::getInstance()->playEffect("vo_decorate_icing.mp3");
//        }
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<gsModels[m_nShape]<<"/"<<m_nIndex<<".png";
        
        Sprite* pTarger = Sprite::create(ostr.str());
//        if(m_nShape == 1){
//            pTarger->setPosition(Vec2(m_pIcingScribble->getContentSize().width*0.5+4, m_pIcingScribble->getContentSize().height*0.5-15));
//        }else if(m_nShape == 2){
//            pTarger->setPosition(Vec2(m_pIcingScribble->getContentSize().width*0.5+6, m_pIcingScribble->getContentSize().height*0.5));
//        }else if(m_nShape == 3){
//            pTarger->setPosition(Vec2(m_pIcingScribble->getContentSize().width*0.5+6, m_pIcingScribble->getContentSize().height*0.5+4));
//        }else if(m_nShape == 4){
//            pTarger->setPosition(Vec2(m_pIcingScribble->getContentSize().width*0.5-3, m_pIcingScribble->getContentSize().height*0.5+22));
//        }else if(m_nShape == 5){
//            pTarger->setPosition(Vec2(m_pIcingScribble->getContentSize().width*0.5+8, m_pIcingScribble->getContentSize().height*0.5+10));
//        }else{
//            pTarger->setPosition(m_pIcingScribble->getContentSize()*0.5);
//        }
        pTarger->setPosition(m_pIcingScribble->getContentSize()*0.5);
        m_pIcingScribble->useTarget(pTarger);
        return;
    }
    
    std::stringstream ostr;
    ostr<<"content/category/"<<m_sCurTypePath<<"/"<<index<<".png";
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    
    if (m_sCurTypePath=="sprinkle"||m_sCurTypePath=="candy"||m_sCurTypePath=="fruit") {
//        Sprite* pItem = Sprite::create(ostr.str());
//        DecorationUtil* unit = DecorationUtil::createDefault(pItem->getContentSize());
//        unit->setIgnoreTip();
//        unit->ignoreRotation(true);
//        unit->addChild(pItem);
//        Rect rect = Rect(-m_pFood->getAnchorPointInPoints().x+30, -m_pFood->getAnchorPointInPoints().y+20, m_pFood->getContentSize().width-60, m_pFood->getContentSize().height-20);
//        
//        unit->setMoveLimitRect(rect);
//        m_pClipping->addChild(unit);
//        Vec2 position = Vec2(-50.0f+10*(arc4random()%10), -100.0f+10*(arc4random()%20));
//        unit->setPosition(position);
//        pParticle->setPosition(m_pClipping->getPosition()+m_pDecorateLayer->getPosition()+position);
        
    }else{
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(260.0f+10*(arc4random()%15), 400.0f+10*(arc4random()%20)));
        if (decData.zoomable||decData.rotatable) {
            FillMaterialModel* unit = FillMaterialModel::create(ostr.str(),decData.small);
            Rect rect = Rect(220, 380, 200, 250);
            rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
            unit->setMoveLimitRect(rect);
            if (decData.eatable) {
                DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFood,decData.single?m_sCurTypePath:"",position,1);
                pParticle->setPosition(unit->getPosition());
            }else{
                DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllUneatFront,decData.single?m_sCurTypePath:"",position,1);
                pParticle->setPosition(unit->getPosition());
            }
        }else{
            Sprite* unit = Sprite::create(ostr.str());
            DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
            pDecoration->setIgnoreTip();
            pDecoration->addChild(unit);
            pDecoration->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height-300));
            if (decData.eatable) {
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFood,decData.single?m_sCurTypePath:"",position,1);
                pParticle->setPosition(pDecoration->getPosition());
            }else{
                DecorateManager::getInstance()->addDecoration(pDecoration, eDecorationLayerFoodBack,decData.single?m_sCurTypePath:"",m_pFood->getPosition(),1);
                pParticle->setPosition(pDecoration->getPosition());
            }
            if (m_sCurTypePath=="plate"){
                pDecoration->setTouchEnable(false);
                GameDataManager::getInstance()->m_nPlateIndex = m_nIndex;
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
    if (m_sCurTypePath=="icing"){
        Rect rect = m_pFood->getBoundingBox();
        rect.origin = m_pFood->getParent()->convertToWorldSpace(rect.origin);
        Vec2 pos = pTouch->getLocation();
        if (rect.containsPoint(pos)) {
            m_pIcingScribble->paint(pos);
            Director::getInstance()->getRenderer()->render();
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
    if (m_sCurTypePath=="icing"){
        Rect rect = m_pFood->getBoundingBox();
        rect.origin = m_pFood->getParent()->convertToWorldSpace(rect.origin);
        Vec2 pos = pTouch->getLocation();
        if (rect.containsPoint(pos)) {
            m_pIcingScribble->paint(pTouch->getPreviousLocation(),pos);
            Director::getInstance()->getRenderer()->render();
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
    m_pGuideLayer->removeGuide();
    if (m_sCurTypePath=="fruit" || m_sCurTypePath=="candy"){
        m_pGuideLayer->removeGuide();
        
        int alpha = 0;
        if (m_pFoodRender) {
            alpha = m_pFoodRender->getAlphaWithLocation(m_pFood->convertToNodeSpace(pos));
        }
        if(alpha>0){
            
            std::stringstream ostr;
            ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
            Sprite* unit = Sprite::create(ostr.str());
            int angle = 20*(arc4random()%10);
            unit->setRotation(angle);
            unit->setPosition(m_pDecNode->convertToNodeSpace(pos));
            m_pDecNode->addChild(unit,2);
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setPosition(pos);
//            DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerFood,"",pos,2);
            
        }
        
        AudioHelp::getInstance()->playSelectedEffect();
        
    }else if(m_sCurTypePath=="sprinkle"){
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        
        std::vector<std::string> temps;
        temps.push_back(ostr.str());
        for(int i=0;i<5;i++){
            std::stringstream ostr;
            ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<"_"<<i+1<<".png";
            if(Sprite::create(ostr.str())){
                temps.push_back(ostr.str());
            }
        }
        currentSprinkle ->setSprinklePath(temps);
        currentSprinkle->setEnable(true);
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