
#include "DecorateConeScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"

static Color4B gPenColor[] = {
    {255,255,255,255},
    {0,0,0,255},
    {255,0,200,255},
    {168,0,250,255},
    {26,0,250,255},
    {8,182,253,255},
    {0,255,77,255},
    {254,236,53,255},
    {255,121,21,255},
    {255,0,19,255},
};
DecorateConeScene::DecorateConeScene()
{
    m_pDecorationScrollView = nullptr;
    m_pConeFront = nullptr;
    m_pTexture = nullptr;
    m_pScribble = nullptr;
    m_pSticker = nullptr;
    m_pExtra = nullptr;
    m_pColorScribble = nullptr;
    m_bCanTouch = false;
    m_nTextureCount = 0;
    m_nPenCount = 0;
    m_nColorCount = 0;
}

DecorateConeScene::~DecorateConeScene()
{
    
}
bool DecorateConeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    int m_nBgIndex = GameDataManager::getInstance()->m_nBgIndex;
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg_winter.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    m_pBg = pBg;
    
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    
    m_pConeBack = Sprite::create("content/category/color/cone.png");
    m_pDecorateLayer->addChild(m_pConeBack);
    m_pConeBack->setScale(0.99);
    m_pConeBack->setPosition(visibleSize.width/2,visibleSize.height*0.4-visibleSize.height);
    
    m_pConeFront = Sprite::create("content/category/color/0.png");
    
    m_pCone = ClippingNode::create();
    m_pCone->setStencil(Sprite::create("content/category/color/cone_mask.png"));
    m_pCone->setAlphaThreshold(0.5);
    m_pConeFront->setScale(0.99);
    m_pCone->addChild(m_pConeFront);
    m_pCone->setPosition(visibleSize.width/2,visibleSize.height*0.4-visibleSize.height);
    m_pDecorateLayer->addChild(m_pCone);
    
    m_pConeBack->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(0.5, Vec2(0, visibleSize.height)), NULL));
    
    m_pCone->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(0.5, Vec2(0, visibleSize.height)),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("design_cone.mp3");
        
    }), NULL));
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(520,150));
    this->addChildToUILayer(m_pTypeScrollView);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView, (640-520)/2, visibleSize.height*1.5-m_pTypeScrollView->getContentSize().height*1.1,kBorderNone,kBorderBottom);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateConeScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->setLocalZOrder(10);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                  EaseBackIn::create(MoveBy::create(1, Vec2(0, -visibleSize.height/2))), NULL));
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("cone"));

    m_pGameUI->showNextLayout();
    m_pGameUI->showResetLayout();
    
//    auto listen = EventListenerTouchOneByOne::create();
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/dyc.png",CMVisibleRect::getPosition(320+visibleSize.width, 650));
    return true;
}

void DecorateConeScene::onEnter()
{
    ExtensionScene::onEnter();
    m_bShowShop = false;
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_DESIGN_CONE);
}

void DecorateConeScene::onExit()
{
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_DESIGN_CONE);
    ExtensionScene::onExit();
}
bool DecorateConeScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (m_sCurTypeStr=="pen" || m_sCurTypeStr=="eraser") {
        m_pGuideLayer->removeGuide();
        if (m_nPenIndex>=0) {
            if(m_pScribble){
                Point lTouchPoint = this->convertToWorldSpace(touch->getLocation());
                m_pScribble->paint(lTouchPoint);
                _playEffectNice();
            }
        return true;
        }
    }else if (m_sCurTypeStr=="color"){
        m_pGuideLayer->removeGuide();
        if (m_pColorScribble) {
            if (m_nPenIndex>=0){
                Point lTouchPoint = this->convertToWorldSpace(touch->getLocation());
                m_pColorScribble->paint(lTouchPoint);
                _playEffectNice();
            }
        }
        return true;
    }
    return false;
}
void DecorateConeScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    if (m_sCurTypeStr=="pen" || m_sCurTypeStr=="eraser"){
        if (m_pScribble) {
            if (m_nPenIndex>=0){
                Point lTouchPoint = this->convertToWorldSpace(touch->getLocation());
                Point lPreTouchPoint = this->convertToWorldSpace(touch->getPreviousLocation());
                m_pScribble->paint(lTouchPoint, lPreTouchPoint);
            }
        }
    }else if (m_sCurTypeStr=="color"){
        if (m_pColorScribble) {
            if (m_nPenIndex>=0){
                Point lTouchPoint = this->convertToWorldSpace(touch->getLocation());
                Point lPreTouchPoint = this->convertToWorldSpace(touch->getPreviousLocation());
                m_pColorScribble->paint(lTouchPoint, lPreTouchPoint);
            }
        }
    }
}
void DecorateConeScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    
}
#pragma mark - initData
void DecorateConeScene::_initData()
{
    setExPath("content/make/dec/");
    
    m_nType = GameDataManager::getInstance()->m_nDecorateStep;
    m_nNextSceneTag = GameUIEvent::eSceneTagMap;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagMap;
    
    memset(m_bGuide, 0, 2);
}


void DecorateConeScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    
    if (m_pTexture) {
        m_pTexture->removeFromParent();
        m_pTexture = nullptr;
    }
    if (m_pColorScribble) {
        m_pColorScribble->removeFromParent();
        m_pColorScribble = nullptr;
    }
    if (m_pScribble) {
        m_pScribble->removeFromParent();
        m_pScribble = nullptr;
    }
    if (m_pSticker) {
        m_pSticker->removeAllChildren();
    }
    if (m_pExtra) {
        m_pExtra->removeFromParent();
        m_pExtra = nullptr;
    }
    
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

void DecorateConeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
}

RenderTexture* DecorateConeScene::getResultRender()
{
    Size size = m_pConeBack->getContentSize();
    RenderTexture* render = RenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    m_pCone->setPosition(Vec2(size.width/2, size.height/2));
    if (m_pExtra) {
        m_pExtra->setPosition(Vec2(size.width/2+4, size.height/2));
    }
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    
    m_pCone->cocos2d::Node::visit();
    if (m_pExtra) {
        m_pExtra->visit();
    }
    render->end();
    m_pCone->setPosition(m_pConeBack->getPosition());
    if (m_pExtra) {
        m_pExtra->setPosition(Vec2(m_pCone->getPositionX()+4, m_pCone->getPositionY()));
    }
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;
}

void DecorateConeScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        
        
        GameDataManager::getInstance()->m_nDecorateStep++;
        Image* pImage = getResultRender()->newImage();
        
        bool issuccess;
        issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"cone.png", false);
        pImage->autorelease();
        log("===save success %d==",issuccess);
        GameDataManager::getInstance()->setStepCount(5);
        
    }else if (GameUILayoutLayer::eUIButtonTagBack==tag){
        
    }else if (GameUILayoutLayer::eUIButtonTagReset==tag){
        btn->setEnabled(false);
//        Dialog* dialog=Dialog::create(Size(497,355), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
//        dialog->setContentText("Are you sure you want to reset your decoration?");
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
//        btn->setEnabled(true);
        _reset();
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
    }
    
    ExtensionScene::onButtonCallback(btn);
}

void DecorateConeScene::onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
{
    m_pGuideLayer->removeGuide();
    m_sCurTypeStr = string(typeData.decTypeName);
    AudioHelp::getInstance()->playSelectedEffect();
    m_nPenIndex = -1;
    m_bCanTouch = false;
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        

            ostr<<"content/category/"<<m_sCurTypeStr<<"_icon/";
            if (!m_pDecorationScrollView) {
                m_pDecorationScrollView = ItemScrollView::createWithSize(Size(512, 140),false);
                this->addChildToUILayer(m_pDecorationScrollView);
                
                m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
                m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateConeScene::onDecorationCallback, this);
            }else{
                m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
            }
            m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateConeScene::onDecorationCallback, this);
            m_pDecorationScrollView->stopAllActions();
            m_pDecorationScrollView->setMargin(10);
            //        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, 5, 80,kBorderLeft);
            CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, (640-m_pDecorationScrollView->getContentSize().width)*0.5,visibleSize.height-m_pDecorationScrollView->getContentSize().height*1.1,kBorderNone,kBorderBottom);
            m_pDecorationScrollView->setScale(0.5);
            m_pDecorationScrollView->runAction(Spawn::create(MoveBy::create(0.5, Vec2(0, -m_pTypeScrollView->getContentSize().height)),
                                                             ScaleTo::create(0.5, 1), NULL));
            m_pDecorationScrollView->btnPathEX = ostr.str();
        
        if (std::strcmp(m_sCurTypeStr.c_str(), "pen")==0) {
            if(m_nPenCount==0){
                AudioHelp::getInstance()->playEffect("draw_cone.mp3");
            }
            m_pDecorationScrollView->frontHighlightPath = "";
            m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
            m_nPenCount++;
        }else if (std::strcmp(m_sCurTypeStr.c_str(), "texture")==0) {
            if(m_nTextureCount==0){
                AudioHelp::getInstance()->playEffect("pattern_cone.mp3");
            }
            m_pDecorationScrollView->bgHighlightPath = "";
            m_pDecorationScrollView->frontHighlightPath = "content/category/icon/icon_texture_h.png";
            m_nTextureCount++;
        }else if (std::strcmp(m_sCurTypeStr.c_str(), "color")==0) {
            if(m_nColorCount==0){
                AudioHelp::getInstance()->playEffect("paint_cone.mp3");
            }
            m_pDecorationScrollView->bgHighlightPath = "";
            m_pDecorationScrollView->frontHighlightPath = "content/category/icon/icon_color_h.png";
            m_nColorCount++;
        }else{
            m_pDecorationScrollView->frontHighlightPath = "";
            m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
        }
        m_pDecorationScrollView->reloadData();
        
    }else{
        m_sCurTypeStr = "";
        if (m_pDecorationScrollView) {
            m_pDecorationScrollView->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(0, m_pTypeScrollView->getContentSize().height*1.3)),
                                                                              ScaleTo::create(0.5, 0), NULL),
                                                                CallFunc::create([=]()
                                                                                 {
                                                                                     m_pDecorationScrollView->removeFromParent();
                                                                                     m_pDecorationScrollView = nullptr;
                                                                                 }), NULL));
        }
    }
}


void DecorateConeScene::onDecorationCallback(int index,DecorateConfigData decData)
{
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
    ostr<<"content/category/"<<m_sCurTypeStr<<"/"<<index<<".png";
    
//    if (!isScheduled(schedule_selector(DecorateConeScene::_playEffectNice))) {
//        schedule(schedule_selector(DecorateConeScene::_playEffectNice), 5);
//    }
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    if (std::strcmp(m_sCurTypeStr.c_str(), "color")==0) {
        m_nPenIndex = index;
        if (!m_bGuide[0]) {
            m_bGuide[0] = true;
            m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2-50, visibleSize.height*0.4), Vec2(visibleSize.width/2+100, visibleSize.height*0.4+100));
        }
        if (!m_pColorScribble) {
            m_pColorScribble = ScribbleNode::create(m_pConeBack->getContentSize());
            m_pCone->addChild(m_pColorScribble,0);
//            m_pConeFront->setPosition(m_pConeFront->getContentSize()*0.5);
            m_pColorScribble->useBrush("content/make/color/brush.png");
            m_pColorScribble->antiAliasing();
        }
        m_pColorScribble->useTarget(ostr.str());
        return;
        
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "texture")==0) {
        AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
        if(!m_pTexture){
            m_pTexture = Sprite::create(ostr.str());
            m_pCone->addChild(m_pTexture,2);
        }else{
            m_pTexture->setTexture(ostr.str());
        }
        pParticle->setPosition(m_pCone->getPosition());
        
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "pen")==0) {
        m_nPenIndex = index;
        if (!m_bGuide[1]) {
            m_bGuide[1] = true;
            m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2-50, visibleSize.height*0.4), Vec2(visibleSize.width/2+100, visibleSize.height*0.4+100));
        }
        this->runAction(Sequence::create(DelayTime::create(0.1),
                                         CallFunc::create([=]()
                                                          {
                                                              m_bCanTouch = true;
                                                          }), NULL));
        if(!m_pScribble){
            m_pScribble = ScribbleShapeNode::create(visibleSize);
            m_pCone->addChild(m_pScribble,3);
            m_pScribble->useTarget("content/category/color/0.png");//指定将要绘制的对象（衣服纹理）
            m_pScribble->setShape("content/category/color/0.png");
            m_pScribble->antiAliasing();
        }
        if(index==0){
            if (m_pScribble) {
                m_pScribble->useBrush("content/category/brush.png",Scribble::BrushType::eEaser);
                m_pScribble->useTargetColor(Color4F(255,255,255,0));
            }

        }else{
            m_pScribble->useBrush("content/category/brush.png",Scribble::BrushType::eBrush);
            m_pScribble->useTargetColor(Color4F(gPenColor[index-1]));
        }

        return;
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "extra")==0) {
        AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
        if(!m_pExtra){
            m_pExtra = Sprite::create(ostr.str());
//            m_pCone->addChild(m_pExtra,5);
            m_pExtra->setPosition(Vec2(m_pCone->getPositionX()+4, m_pCone->getPositionY()));
            m_pDecorateLayer->addChild(m_pExtra,5);
        }else{
            m_pExtra->setTexture(ostr.str());
        }        
        pParticle->setPosition(m_pCone->getPosition());
        
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "sticker")==0) {
        AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
        if(!m_pSticker){
            m_pSticker = Node::create();
            m_pCone->addChild(m_pSticker,4);
        }
        Sprite* unit = Sprite::create(ostr.str());
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width+50,unit->getContentSize().height+50));
        pDecoration->addChild(unit);
        pDecoration->setIgnoreTip();
        Rect rect = m_pConeBack->getBoundingBox();
        rect.origin = m_pSticker->convertToNodeSpace(m_pConeBack->getParent()->convertToWorldSpace(rect.origin));
        pDecoration->setMoveLimitRect(rect);
        pDecoration->setPosition(Vec2(0, -80));
        m_pSticker->addChild(pDecoration);
        
        pParticle->setPosition(m_pCone->getPosition());
        
    }else{
        AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
        Vec2 position = CMVisibleRect::getPositionAdapted(Vec2(300.0f+10*(arc4random()%30), 200.0f+10*(arc4random()%30)));
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
    
    _playEffectNice();
    this->addChildToUILayer(pParticle);
}


void DecorateConeScene::_playEffectNice(float)
{
    static int count = 0;
    count++;
    if (count%3==0) {
//        int radom = arc4random()%100;
//        if (radom>80){
//            AudioHelp::getInstance()->playEffect("nice.mp3");
//        }
        AudioHelp::getInstance()->playEffect("nice.mp3");
    }
}