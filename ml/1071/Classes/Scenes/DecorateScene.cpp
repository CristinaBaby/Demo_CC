
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
    m_bPacked = false;
    m_bButtonEable = true;
    
    m_pSauce = nullptr;
    m_pSauseBottle = nullptr;
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
    m_pBg = Sprite::create(localPath("bg/bg3.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
//    m_pDecorateLayer->setPosition(Vec2(0,100));
    m_pDecorateLayer->setLocalZOrder(5);
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(visibleSize.width,200));
    this->addChildToUILayer(m_pTypeScrollView);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
//    m_pTypeScrollView->boxPathEX = "content/category/icon/box_2.png";
//    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(15);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,(640-m_pTypeScrollView->getContentSize().width)/2+visibleSize.width,  130+40,kBorderNone,kBorderBottom);
    m_pTypeScrollView->setSelectedAnimate(false);
    
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
    
    AudioHelp::getInstance()->playBackGroundMusic("bg_make.mp3");
}
#pragma mark - initData
void DecorateScene::_initData()
{
    setExPath("content/make/");
    
    std::memset(m_bGuide, 0, 3);
    std::memset(m_bVOGuide, 0, 8);
    GameDataManager::getInstance()->m_bPacked = false;

}

void DecorateScene::_initDefaultDecorate()
{
    DecorateManager* pDecManager = DecorateManager::getInstance();
    
    pDecManager->initWithParent(m_pDecorateLayer, eDecorateTypeFood);
    //    ******************      normal        **********************
    Sprite*pPlate = Sprite::create(localPath("dec/plate.png"));
    m_pPlate = pPlate;
    
    Vec2 pos = Vec2(320, 500);
    pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone));
    
    Sprite* pBreadDown = Sprite::create(localPath("dec/bread_down.png"));
    pDecManager->addDecoration(pBreadDown, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone));
    
    
    Sprite* pSausage = Sprite::create(localPath("dec/bread_sausage.png"));
    pDecManager->addDecoration(pSausage, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos+Vec2(0, -5),kBorderNone,kBorderNone),3);
    
    Sprite* pBreadUp = Sprite::create(localPath("dec/bread_on.png"));
    pDecManager->addDecoration(pBreadUp, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone),5);
    m_pFood = pBreadUp;
    
    if (!m_pClipping) {
        Sprite* pStencil = Sprite::create(localPath("dec/mask.png"));
        
        m_pClipping = ClippingNode::create(pStencil);
    }
    pDecManager->addDecoration(m_pClipping, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone));
    m_pClipping->setAlphaThreshold(0.5);
    
    m_pClippingUp = ClippingNode::create(Sprite::create(localPath("dec/mask.png")));
    pDecManager->addDecoration(m_pClippingUp, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone),4);
    m_pClippingUp->setAlphaThreshold(0.5);

    m_pClippingSause = ClippingNode::create(Sprite::create(localPath("dec/bread_mask.png")));
    pDecManager->addDecoration(m_pClippingSause, eDecorationLayerFood,"",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone),6);
    m_pClippingSause->setAlphaThreshold(0.5);
    
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normal"));
}

void DecorateScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    
    DecorateManager::getInstance()->reset();
    m_pClipping = nullptr;
    m_pClippingUp = nullptr;
    m_pClippingSause = nullptr;
    
    m_pSauce = nullptr;
    
    if (m_pSauseBottle) {
        m_pSauseBottle->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                   CallFuncN::create([=](Node* pNode){
            pNode->removeFromParent();
        }), NULL));
        m_pSauseBottle = nullptr;
    }
    m_bPacked = false;
//    if (m_pDecorationScrollView) {
//        m_pDecorationScrollView->removeFromParent();
//        m_pDecorationScrollView = nullptr;
//    }
    //        _removePlate();
    _initDefaultDecorate();
    
    
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(1),
                                                  CallFunc::create([=](){
        m_pTypeScrollView->selectBtn(0);
    }), NULL));
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
    
    Vec2 pos = Vec2(candySize.width/2-m_pPlate->getPositionX(),candySize.height/2-m_pPlate->getPositionY()+50);
    
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
    m_pGameUI->hideReset();
    m_pGameUI->hideNext();
    
    
}

void DecorateScene::_disableMovableFood(bool disable,bool down)
{
    if(down){
        Vector<Node*> pChildren = m_pClipping->getChildren();
        for_each(pChildren.begin(), pChildren.end(), [=](Node* pNode){
            DecorationUtil* pUnit = dynamic_cast<DecorationUtil*>(pNode);
            if (pUnit) {
                pUnit->setTouchEnable(!disable);
            }
        });
    }
    if(!down){
        Vector<Node*> pChildren = m_pClippingUp->getChildren();
        for_each(pChildren.begin(), pChildren.end(), [=](Node* pNode){
            DecorationUtil* pUnit = dynamic_cast<DecorationUtil*>(pNode);
            if (pUnit) {
                pUnit->setTouchEnable(!disable);
            }
        });
    }
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
        SceneManager::pushTheScene<ShareScene>();
        
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
        
        AudioHelp::getInstance()->playEffect("vo_nice_design.mp3");
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
        _reset();
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
        if (m_sCurTypeStr!="sauce") {
            if(m_pSauce){
                m_pSauce->setDrawing(false);
            }
            if (m_pSauseBottle) {
                m_pSauseBottle->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                           CallFuncN::create([=](Node* pNode){
                    pNode->removeFromParent();
                }), NULL));
                m_pSauseBottle = nullptr;
            }
            if (m_sCurTypeStr=="cheese" || m_sCurTypeStr=="meat" || m_sCurTypeStr=="vegetable"){
                _disableMovableFood(false);
                _disableMovableFood(true,false);
            }else if (m_sCurTypeStr=="others"){
                _disableMovableFood();
                _disableMovableFood(false,false);
            }else{
                _disableMovableFood();
                _disableMovableFood(true,false);
            }
        }else{
            _disableMovableFood();
            _disableMovableFood(true,false);
        }
        
        if (m_sCurTypePath=="cheese"){
            if (!m_bVOGuide[0]) {
                m_bVOGuide[0] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_cheese.mp3");
            }
        }else if (m_sCurTypePath=="sidedish"){
            if (!m_bVOGuide[1]) {
                m_bVOGuide[1] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_side_dishes.mp3");
            }
        }else if (m_sCurTypePath=="vegetable"){
            if (!m_bVOGuide[2]) {
                m_bVOGuide[2] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_vegetable.mp3");
            }
        }else if (m_sCurTypePath=="sauce"){
            if (!m_bVOGuide[3]) {
                m_bVOGuide[3] = true;
                AudioHelp::getInstance()->playEffect("vo_decorate_syrup.mp3");
            }
        }
        
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/"<<m_sCurTypeStr<<"_icon/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(660,150),false);
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
//        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/box2_2.png";
        m_pDecorationScrollView->boxPathEX = "content/category/icon/dec_frame.png";
//        m_pDecorationScrollView->bgPath = "content/category/icon/box2_1.png";
        m_pDecorationScrollView->bgPath = "";
        m_pDecorationScrollView->bgHighlightPath = "";
        
        if (m_sCurTypeStr=="cheese" || m_sCurTypeStr=="meat" || m_sCurTypeStr=="vegetable" || m_sCurTypeStr=="others"){
            m_pDecorationScrollView->bgPath = "content/category/icon/box.png";
            m_pDecorationScrollView->bgHighlightPath = "content/category/icon/box.png";
        }
        m_pDecorationScrollView->setMargin(10);
        DecorateConfigData tempData = m_pDecorationScrollView->decorationData;
        m_pDecorationScrollView->reloadData(tempData.freeCount<=tempData.videoCount?false:true);
        
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
            RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
            m_pDecorationScrollView->setSelected(false);
            return;
        }
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    
    m_nIndex = index;
    
    std::stringstream ostr;
    ostr<<"content/category/"<<m_sCurTypeStr<<"/"<<index<<".png";
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    
    if (m_sCurTypeStr=="sauce") {
        if(m_pSauce == nullptr){
            m_pSauce = ScribbleSauce::create(m_pFood->getContentSize());
            m_pClippingSause->addChild(m_pSauce,2);
            m_pSauce->setPosition(Vec2(-m_pFood->getContentSize().width/2, -m_pFood->getContentSize().height/2));
        }
        
        m_pSauce->setBrush(Sprite::create("content/category/sauce/sauce1/"+std::to_string(m_nIndex)+".png"));
        if (!m_pSauseBottle) {
            m_pSauseBottle = Sprite::create("content/category/sauce/sauce2/"+std::to_string(m_nIndex)+".png");
            this->addChildToUILayer(m_pSauseBottle);
            m_pSauseBottle->setAnchorPoint(Vec2(0.5, 1));
            m_pSauseBottle->setName("drawSauce");
            m_pSauseBottle->setScale(0.8);
            
            CMVisibleRect::setPositionAdapted(m_pSauseBottle, 380+visibleSize.width, 400+100);
            m_pSauseBottle->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                                                     RotateBy::create(0.5, -160), NULL),
                                                       CallFunc::create([=](){
                m_pGuideLayer->showGuideTapHorizonal(m_pSauseBottle->getPosition());
                m_pSauce->setDrawing(true);
                m_pSauce->callBack = [this](Touch* pTouch,ScribbleSauce::touchType _type){
                    if(_type == ScribbleSauce::touchType::touchMove){
                        m_pGuideLayer->removeGuide();
                        auto _draw = m_pUILayer->getChildByName("drawSauce");
                        if(_draw != nullptr){
                            Rect _limit;
                            _limit.size = m_pFood->getContentSize();
                            
                            
                            auto _pos = m_pFood->convertToNodeSpace(pTouch->getLocation());
                            if(_limit.containsPoint(_pos)) {
                                _draw->setPosition(pTouch->getLocation());
                            }
                        }
                    }
                };
                
            }), NULL));
        }else{
            m_pSauseBottle->setTexture("content/category/sauce/sauce2/"+std::to_string(m_nIndex)+".png");
        }
        pParticle->setPosition(m_pFood->getPosition()+m_pDecorateLayer->getPosition());
    }else if (m_sCurTypeStr=="sidedish") {
        Sprite* pPlate = Sprite::create("content/category/sidedish/plate.png");
//        pPlate->setAnchorPoint(Vec2(0.5, 0));
        
        Sprite* pItem = Sprite::create(ostr.str());
        pItem->setAnchorPoint(Vec2(0.5, 0));
        
        Vec2 pos = Vec2(200, 640);
        DecorateManager* pDecManager = DecorateManager::getInstance();
        pDecManager->addDecoration(pPlate, eDecorationLayerFoodBack,"sidedish_plate",CMVisibleRect::getPositionAdapted(pos+Vec2(0, 90),kBorderNone,kBorderNone));
        
        pDecManager->addDecoration(pItem, eDecorationLayerFood,m_sCurTypeStr,CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone));
        pParticle->setPosition(pItem->getPosition()+m_pDecorateLayer->getPosition());
        
    }else if (m_sCurTypeStr=="others") {
        Sprite* unit = Sprite::create(ostr.str());
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
        pDecoration->setIgnoreTip();
        pDecoration->addChild(unit);
        pDecoration->setMoveLimitRect(Rect(-285+unit->getContentSize().width/2, -98+unit->getContentSize().height/2, 569-unit->getContentSize().width, 196-unit->getContentSize().height*0.7));
        m_pClippingUp->addChild(pDecoration);
        pDecoration->setPosition(Vec2(0, 40));
        pParticle->setPosition(m_pFood->getPosition()+m_pDecorateLayer->getPosition());
        
    }else if (m_sCurTypeStr=="box") {
        if(m_pPlate){
            m_pPlate->removeFromParent();
            m_pPlate = nullptr;
        }
        std::stringstream ostrBack;
        if (m_nIndex>10 && m_nIndex<14) {
            ostrBack<<"content/category/"<<m_sCurTypeStr<<"/down/11_13.png";
        }else if (m_nIndex<=9) {
            ostrBack<<"content/category/"<<m_sCurTypeStr<<"/down/1_9.png";
        }else{
            ostrBack<<"content/category/"<<m_sCurTypeStr<<"/down/"<<index<<".png";
        }
        Vec2 pos = Vec2(319, 476+20);
        DecorateManager* pDecManager = DecorateManager::getInstance();
        
        Sprite* pBoxBack = Sprite::create(ostrBack.str());
        pDecManager->addDecoration(pBoxBack, eDecorationLayerFoodBack,"boxback",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone));
        
        std::stringstream ostrFront;
        ostrFront<<"content/category/"<<m_sCurTypeStr<<"/on/"<<index<<".png";
        Sprite* pBoxFront = Sprite::create(ostrFront.str());
        pDecManager->addDecoration(pBoxFront, eDecorationLayerFoodFront,"box",CMVisibleRect::getPositionAdapted(pos,kBorderNone,kBorderNone));
        
        pParticle->setPosition(m_pFood->getPosition()+m_pDecorateLayer->getPosition());
    }else{
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(260.0f+10*(arc4random()%15), 400.0f+10*(arc4random()%20)));
        if (decData.zoomable) {
            Sprite* unit = Sprite::create(ostr.str());
            DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
            pDecoration->ignoreRotation(true);
            pDecoration->setIgnoreTip();
            pDecoration->addChild(unit);
            pDecoration->setMoveLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height-300));
            m_pClipping->addChild(pDecoration);
        }else{
            Sprite* unit = Sprite::create(ostr.str());
            DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
            pDecoration->setIgnoreTip();
            pDecoration->addChild(unit);
            pDecoration->setMoveLimitRect(Rect(-285+unit->getContentSize().width/2, -98+unit->getContentSize().height/2, 569-unit->getContentSize().width, 196-unit->getContentSize().height*0.8));
            m_pClipping->addChild(pDecoration);
        }
        pParticle->setPosition(m_pFood->getPosition()+m_pDecorateLayer->getPosition());
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