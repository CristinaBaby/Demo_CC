
#include "DecorateScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"
#include "StickFoodHelp.h"

DecorateScene::DecorateScene()
{
    m_pTypeScrollView = nullptr;
    m_pDecorationScrollView = nullptr;
    m_bCanTouch = false;
    m_pClipping = nullptr;
    m_pIcing = nullptr;
    m_bPacked = false;
    m_bButtonEable = true;
    m_pSauce = nullptr;
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
    m_pBg = Sprite::create("content/dec/bg_decoration.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    m_pDecorateLayer->setPosition(Vec2(0,0));
    m_pDecorateLayer->setLocalZOrder(5);
    
    if(GameDataManager::getInstance()->m_nPackage==0){
        m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(586,170));
        this->addChildToUILayer(m_pTypeScrollView);
        m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onTypeCallback, this);
        m_pTypeScrollView->btnPathEX = "content/category/icon/";
        m_pTypeScrollView->boxPathEX = "content/category/icon/frame1.png";
        m_pTypeScrollView->setLocalZOrder(2);
        m_pTypeScrollView->setSingleAsset(true);
        m_pTypeScrollView->setMargin(10);
        m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
        CMVisibleRect::setPositionAdapted(m_pTypeScrollView,(640-m_pTypeScrollView->getContentSize().width)/2,  790+visibleSize.height,kBorderNone,kBorderTop);
        m_pTypeScrollView->setSelectedAnimate(false);
        
        
        m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(1.5),
                                                      MoveBy::create(0.5, Vec2(0,-visibleSize.height)), NULL));
    }
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
    GameDataManager::getInstance()->m_nDecorateStep = 0;
    
//    m_pGameUI->showNormalLayout();
    m_CandySize = Size(640,640);
    m_nNextSceneTag = GameUIEvent::eSceneTagShare;
    return true;
}

void DecorateScene::onEnter()
{
    ExtensionScene::onEnter();
//    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_DECORATE);
    AudioHelp::getInstance()->playBackGroundMusic("background.mp3");
    
}
#pragma mark - initData
void DecorateScene::_initData()
{
    m_nFlavor = GameDataManager::getInstance()->m_nFlavorIndex;
    if(GameDataManager::getInstance()->m_nPackage==0){
        setExPath("content/dec/donut/");
    }else{
        
        setExPath("content/dec/twist/");
    }
    
    std::memset(m_bGuide, 0, 5);
    std::memset(m_bVOGuide, 0, 8);
    GameDataManager::getInstance()->m_bPacked = false;

}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(m_pDecorateLayer, eDecorateTypeFood);
    //    ******************      Donut        **********************
    if(GameDataManager::getInstance()->m_nPackage==0){
        Vec2 pos = Vec2(320, 400);
//        Sprite*pPlate = Sprite::create(localPath("plate.png"));
//        pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(pos));
        
        Sprite* pDonut = Sprite::create(localPath("donut.png"));
        pDecManager->addDecoration(pDonut, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos));
        m_pDonut = pDonut;
        
        
        if (!m_pClipping) {
            Sprite* pStencil = Sprite::create(localPath("donut.png"));
            
            m_pClipping = ClippingNode::create(pStencil);
        }
        m_pClipping->setAlphaThreshold(0.5);
        pDecManager->addDecoration(m_pClipping, eDecorationLayerAllFront,"",CMVisibleRect::getPositionAdapted(pos),5);
        
        
        m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("donut"));
    }else{
//        *******************  twist   ***********************
        
        Vec2 pos = Vec2(320, 400);
        Sprite*pPlate = Sprite::create(localPath("plate.png"));
        pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(pos));
        
//        Sprite* pDonutShadow = Sprite::create(localPath("donut_1.png"));
//        pDecManager->addDecoration(pDonutShadow, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(pos));
        
        
        for(int i = 0 ;i<4;i++){
            Vec2 pos = Vec2(330, 400+150-80*i);
            Sprite* pDonut = Sprite::create(localPath("donut.png"));
            pDecManager->addDecoration(pDonut, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos));
            pDonut->setRotation(90);
            
            Sprite* pStencil = Sprite::create(localPath("donut.png"));
            
            ClippingNode* pClipping = ClippingNode::create(pStencil);
            pClipping->setAlphaThreshold(0.5);
            pDecManager->addDecoration(pClipping, eDecorationLayerAllFront,"",CMVisibleRect::getPositionAdapted(pos),5);
            pClipping->setRotation(90);
            
            m_pClippingArray.push_back(pClipping);
            m_pDonut = pDonut;
        }
        std::stringstream ostr;
        m_sCurTypeStr = "sugar";
        ostr<<"content/category/"<<m_sCurTypeStr<<"_icon/";
        
        m_pDecorationScrollView = ItemScrollView::createWithSize(Size(524,170),false);
        this->addChildToUILayer(m_pDecorationScrollView);
        
        m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onDecorationCallback, this);
        m_pDecorationScrollView->onOKClicked = CC_CALLBACK_0(DecorateScene::onDecorateOK, this);
        
        m_pDecorationScrollView->setLocalZOrder(1);
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,(640-m_pDecorationScrollView->getContentSize().width)/2,  790+visibleSize.height/2,kBorderNone,kBorderTop);
        m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height/2)),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_decorate_sugar.mp3");
        }),NULL));
        m_pDecorationScrollView->btnPathEX = ostr.str();
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/box2_2.png";
        m_pDecorationScrollView->boxPathEX = "content/category/icon/frame3.png";
        m_pDecorationScrollView->bgPath = "content/category/icon/box2_1.png";
        m_pDecorationScrollView->setMargin(10);
        m_pDecorationScrollView->reloadData();
    }
    
}

void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    
    auto temp = m_pContentLayer->getChildByName("drawSauce");
    if (temp) {
        temp->removeFromParent();
    }
    
    DecorateManager::getInstance()->reset();
    m_pClippingArray.clear();
    m_pClipping = nullptr;
    m_pIcing = nullptr;
    m_pSauce = nullptr;
    m_bPacked = false;
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->removeFromParent();
        m_pDecorationScrollView = nullptr;
    }
    //        _removePlate();
    _initDefaultDecorate();
    
    
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
    Size candySize = m_pDonut->getContentSize()*scale;
    if(GameDataManager::getInstance()->m_nPackage==1){
        candySize = Size(m_pDonut->getContentSize().height*0.88,m_pDonut->getContentSize().width*0.87);
    }
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 posOrigin = m_pDecorateLayer->getPosition();
    
    if(GameDataManager::getInstance()->m_nPackage==0){
        DecorateManager* pDecManager = DecorateManager::getInstance();
//        Layer* pLayer = pDecManager->getEatableLayer();
        Layer* pLayer2 = pDecManager->getEatableFrontLayer();
        Vec2 pos = Vec2(-m_pDonut->getPositionX()+m_pDonut->getContentSize().width/2,-m_pDonut->getPositionY()+m_pDonut->getContentSize().height/2+5);
        
//        pLayer->setPosition(pos);
        pLayer2->setPosition(pos);
//        pLayer2->setScale(0.73, 0.47);
        pLayer2->setScale(0.62, 0.98);
        pLayer2->setRotation3D(Vec3(65, 0, 0));
        Sprite* pTemp = Sprite::create("content/dec/donut/donut2.png");
        pTemp->setPosition(candySize*0.5);
        
        render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
        pTemp->visit();
        pLayer2->visit();
        render->end();
        
    }else{
        Vec2 pos = m_pDonut->getPosition();
        m_pDonut->setPosition(candySize*0.5);
        ClippingNode* pClipping = m_pClippingArray.at(3);
        pClipping->setPosition(candySize*0.5);
        pClipping->setScale(0.88, 0.84);
        Sprite* pTemp = Sprite::create("content/dec/twist/donut2_1.png");
        pTemp->setPosition(m_pDonut->getPosition()+Vec2(0, -4));
        render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
        pTemp->visit();
        pClipping->Node::visit();
        render->end();
        m_pDonut->setPosition(pos);
        pClipping->setScale(1);
        pClipping->setPosition(pos);
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
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+ name, false);
    pImage->autorelease();
    GameDataManager::getInstance()->setBoxName(name);
}


void DecorateScene::_displayFood(){
    if (GameDataManager::getInstance()->m_nPackage==0) {
        AudioHelp::getInstance()->playEffect("vo_nice_design.mp3");
    }else{
        AudioHelp::getInstance()->playEffect("vo_deocrate_finished.mp3");
    }
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_eat_serve.mp3");
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGameUI->showEatLayout();
        m_pGameUI->showSaleLayout();
    }),NULL));
    
    auto temp = m_pContentLayer->getChildByName("drawSauce");
    if (temp) {
        temp->removeFromParent();
    }
    
    
    if (m_pTypeScrollView){
        m_pTypeScrollView->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)));
    }
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)));
    }
    
    _saveFood();
    m_pGameUI->hideReset();
    m_pGameUI->hideNext();
}
void DecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
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
    auto temp = m_pContentLayer->getChildByName("drawSauce");
    if (temp) {
        temp->removeFromParent();
    }
    if(m_pSauce){
        m_pSauce->setDrawing(false);
    }
    if (m_sCurTypePath=="sprinkle"){
        if (!m_bVOGuide[0]) {
            m_bVOGuide[0] = true;
            AudioHelp::getInstance()->playEffect("vo_decorate_sprinkles.mp3");
        }
    }else if (m_sCurTypePath=="nuts"){
        if (!m_bVOGuide[1]) {
            m_bVOGuide[1] = true;
            AudioHelp::getInstance()->playEffect("vo_decorate_nuts.mp3");
        }
    }else if (m_sCurTypePath=="candy"){
        if (!m_bVOGuide[2]) {
            m_bVOGuide[2] = true;
            AudioHelp::getInstance()->playEffect("vo_decorate_candies.mp3");
        }
    }else if (m_sCurTypePath=="icing"){
        if (!m_bVOGuide[3]) {
            m_bVOGuide[3] = true;
            AudioHelp::getInstance()->playEffect("vo_decorate_frosting.mp3");
        }
    }else if (m_sCurTypePath=="sauce"){
        if (!m_bVOGuide[4]) {
            m_bVOGuide[4] = true;
            AudioHelp::getInstance()->playEffect("vo_decorate_syrup.mp3");
            this->runAction(Sequence::create(Sequence::create(DelayTime::create(2),
                                                              CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("vo_decorate_syrup1.mp3");
            }), NULL), NULL));
        }
    }else if (m_sCurTypePath=="other"){
        if (!m_bVOGuide[5]) {
            m_bVOGuide[5] = true;
            AudioHelp::getInstance()->playEffect("vo_decorate_others.mp3");
        }
    }
    
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/"<<m_sCurTypeStr<<"_icon/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(524,170),false);
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
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,(640-m_pDecorationScrollView->getContentSize().width)/2,  650,kBorderNone,kBorderTop);
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
        
        m_pDecorationScrollView->btnPathEX = ostr.str();
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/box2_2.png";
        m_pDecorationScrollView->boxPathEX = "content/category/icon/frame2.png";
        m_pDecorationScrollView->bgPath = "content/category/icon/box2_1.png";
        m_pDecorationScrollView->setMargin(20);
        if (m_sCurTypeStr == "sauce") {
            m_pDecorationScrollView->reloadData(true);
        }else{
            m_pDecorationScrollView->reloadData();
        }
        if (m_sCurTypeStr == "sauce") {
            m_pDecorationScrollView->insertOthers(ConfigManager::getInstance()->getDecorateFile("sauce_1"),false,true);
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

    
    if (type==1) {
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
            m_pDecorationScrollView->setSelected(false);
            return;
        }
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    
    m_nIndex = index;
    if (m_sCurTypePath=="sprinkle"){
        m_bCanTouch = true;
        if (!m_bGuide[0]) {
            m_bGuide[0] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pDonut->getPosition()));
        }
        return;
    }
    if (m_sCurTypePath=="nuts"){
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_bGuide[1] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pDonut->getPosition()));
        }
        return;
    }
    if (m_sCurTypePath=="candy"){
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_bGuide[2] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pDonut->getPosition()));
        }
        return;
    }
    if (m_sCurTypePath=="other"){
        m_bCanTouch = true;
        if (!m_bGuide[3]) {
            m_bGuide[3] = true;
            m_pGuideLayer->showGuideTap(m_pDecorateLayer->convertToWorldSpace(m_pDonut->getPosition()));
        }
        return;
    }
    
    std::stringstream ostr;
    ostr<<"content/category/"<<m_sCurTypeStr<<"/"<<index<<".png";
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    
    if (m_sCurTypeStr=="sauce" || m_sCurTypeStr=="sauce_1") {
        if(m_pSauce == nullptr){
            m_pSauce = ScribbleSauce::create(m_pDonut->getContentSize());
            m_pClipping->addChild(m_pSauce,1);
            m_pSauce->setPosition(Vec2(-m_pDonut->getContentSize().width/2, -m_pDonut->getContentSize().height/2));
        }
        if ("sauce_1"==decData.decTypeName) {
            std::stringstream ostrSauce;
            ostrSauce<<"content/category/"<<m_sCurTypeStr<<"/"<<decData.decTypeName<<"/"<<index<<".png";
            auto temp = m_pContentLayer->getChildByName("drawSauce");
            if (temp) {
                temp->removeFromParent();
            }
            temp  = Sprite::create(ostrSauce.str());
            temp->setPosition(Vec2(visibleSize.width*1.2,visibleSize.height*0.5));
            this->addChildToContentLayer(temp);
            temp->setName("drawSauce");
            temp->setLocalZOrder(10);
            temp->setScale(0.7);
            temp->runAction(Sequence::create(EaseSineInOut::create(MoveTo::create(0.5, visibleSize*0.5)),
                                             CallFunc::create([=](){
                
//                m_pSauce->reset();
                std::stringstream ostrDot;
                ostrDot<<"content/category/"<<m_sCurTypeStr<<"/"<<decData.decTypeName<<"/"<<index<<"_1.png";
                m_pSauce->setBrush(Sprite::create(ostrDot.str()));
                m_pSauce->setDrawing(true);
                temp->setAnchorPoint(Vec2(0.5, 1));
                temp->runAction(RotateBy::create(0.3, -145));
                m_pSauce->callBack = [this](Touch* pTouch,ScribbleSauce::touchType _type){
                    if(_type == ScribbleSauce::touchType::touchMove){
                        
                        auto _draw = m_pContentLayer->getChildByName("drawSauce");
                        if(_draw != nullptr){
                            Rect _limit;
                            _limit.size = m_pDonut->getContentSize();
                            
                            
                            auto _pos = m_pDonut->convertToNodeSpace(pTouch->getLocation());
                            if(_limit.containsPoint(_pos)) {
                                _draw->setPosition(pTouch->getLocation());
                            }
                        }
                    }
                };
            }),NULL));
            
            
        }else{
//            Sprite* pItem = Sprite::create(ostr.str());
//            
//            DecorateManager::getInstance()->addDecoration(pItem, eDecorationLayerAllFront,decData.single?m_sCurTypeStr:"",m_pDonut->getPosition(),1);
//            pParticle->setPosition(pItem->getPosition());
            
            auto temp = m_pContentLayer->getChildByName("drawSauce");
            if (temp) {
                temp->removeFromParent();
            }
            m_pSauce->setBrush(Sprite::create(ostr.str()));
            m_pSauce->paint(m_pDonut->getPosition());
            m_pSauce->setDrawing(false);
        }
    }else if (m_sCurTypeStr=="icing") {
        if (!m_pIcing) {
            m_pIcing = ScribbleNode::create(m_pDonut->getContentSize());
            m_pIcing->setAntiAliasingBrush(true);
            m_pIcing->useBrush("content/category/hole_stencilsamll.png",Scribble::eBrush);
            DecorateManager::getInstance()->addDecoration(m_pIcing, eDecorationLayerAllFront,decData.single?m_sCurTypeStr:"",m_pDonut->getPosition(),1);
        }
        m_pIcing->useTarget(ostr.str());
        m_bCanTouch = true;
        if (!m_bGuide[4]) {
            m_bGuide[4] = true;
            m_pGuideLayer->showGuideMove(m_pDecorateLayer->convertToWorldSpace(m_pDonut->getPosition())+Vec2(-100, -100), m_pDecorateLayer->convertToWorldSpace(m_pDonut->getPosition())+Vec2(100, 100));
        }
        
        return;
    }else if (m_sCurTypeStr=="sugar") {
        Rect rect = m_pDonut->getBoundingBox();
        Vec2 origin = Vec2(-m_pDonut->getContentSize().width/2 , m_pDonut->getContentSize().height/2);
        
        for (int i = 0; i<5; i++) {
            std::stringstream ostrSugar;
            ostrSugar<<"content/category/"<<m_sCurTypeStr<<"/"<<index<<"_"<<i+1<<".png";
            Sprite* pSugar = Sprite::create(ostrSugar.str());
            if (pSugar) {
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sprinke.plist");
                this->addChildToUILayer(pParticle);
                CMVisibleRect::setPosition(pParticle, 320, 650);
                pParticle->setTexture(pSugar->getTexture());
            }
        }
        for_each(m_pClippingArray.begin(), m_pClippingArray.end(), [=](ClippingNode* pClipping){
            static int sugarCount = 0;
            sugarCount++;
            for (int col = 0; col<7; col++,col+=sugarCount%2) {
                for (int row = 0; row<11; row++) {
                    static int sugarIndex = 0;
//                    for (int i = 0; i<5; i++) {
                        std::stringstream ostrSugar;
                        ostrSugar<<"content/category/"<<m_sCurTypeStr<<"/"<<index<<"_"<<sugarIndex+1<<".png";
                    
                    sugarIndex++;
                    sugarIndex = sugarIndex%5;
                        Sprite* pSugar = Sprite::create(ostrSugar.str());
                        if (pSugar) {
                            int rotation = arc4random()%360;
                            int deltX = (arc4random()%5)*4;
                            int deltY = (arc4random()%5)*8;
                            float scale = 1.0f-0.05f*((arc4random()%4)*2);
                            pSugar->setRotation(rotation);
                            pSugar->setScale(scale);
                            Vec2 sugarPos = origin+Vec2(col*20, -row*40)+Vec2(deltX, deltY);
                            pSugar->setPosition(sugarPos);
                            pClipping->addChild(pSugar);
                            int radom = arc4random()%5;
                            pSugar->setOpacity(0);
                            pSugar->runAction(Sequence::create(DelayTime::create(0.1+0.1*radom),
                                                               FadeIn::create(0.2), NULL));
                        }else{
                            continue;
                        }
//                    }
                }
            }
        });
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
    m_pGuideLayer->removeGuide();
    if (!m_bCanTouch) {
        return;
    }
    if(m_nIndex<0){
        return;
    }
    if (m_sCurTypeStr=="icing") {
        if (m_pIcing) {
            m_pIcing->paint(pTouch->getLocation());
        }
    }
}

void DecorateScene::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if(m_nIndex<0){
        return;
    }
    if (m_sCurTypeStr=="icing") {
        if (m_pIcing) {
            m_pIcing->paint(pTouch->getPreviousLocation(),pTouch->getLocation());
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
    Rect rect = m_pDonut->getBoundingBox();
    Vec2 pos = pTouch->getLocation();
    if (!rect.containsPoint(pos)) {
        return;
    }
    if (m_sCurTypePath=="sprinkle" || m_sCurTypePath=="nuts" || m_sCurTypePath=="other" || m_sCurTypePath=="candy"){
        m_pGuideLayer->removeGuide();
        std::stringstream ostr;
        ostr<<"content/category/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        Sprite* unit = Sprite::create(ostr.str());
        int angle = 20*(arc4random()%10);
        unit->setRotation(angle);
        unit->setPosition(m_pClipping->convertToNodeSpace(pos));
        m_pClipping->addChild(unit,5);
        
//        DecorateManager::getInstance()->addDecoration(unit, eDecorationLayerAllFront,"",pos,1);
        
        AudioHelp::getInstance()->playSelectedEffect();
        
    }
    
}