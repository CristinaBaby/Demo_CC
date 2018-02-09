
#include "DecorateLogoLayer.h"
#include "SceneManager.h"
#include "CMVisibleRect.h"


DecorateLogoLayer::DecorateLogoLayer()
{
//    m_pColor = nullptr;
    m_pPattern = nullptr;
    m_pScribble = nullptr;
    m_pSticker = nullptr;
    m_pTypeScrollView = nullptr;
    m_pDecorationScrollView = nullptr;
    onLogoDecoratedCallback = nullptr;
}

DecorateLogoLayer::~DecorateLogoLayer()
{
    
}
bool DecorateLogoLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(color);
    
    m_pRootNode = Node::create();
    this->addChild(m_pRootNode);
    m_pRootNode->setPosition(Vec2(0, visibleSize.height));
    m_pRootNode->runAction(EaseBounceOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))));
    //    bg
    Sprite* pBg = Sprite::create("content/logo_dec/bg-draw.png");
    pBg->setPosition(Vec2(visibleSize.width*0.5,visibleSize.height*0.5+80));
    m_pRootNode->addChild(pBg,20);
    
    Sprite* pBg2 = Sprite::create("content/logo_dec/bg-draw2.png");
    pBg2->setPosition(Vec2(visibleSize.width*0.5,visibleSize.height*0.5+60));
    m_pRootNode->addChild(pBg2,1);
    m_pBg = pBg2;
    
    Rect rect = pBg2->getBoundingBox();
    
    m_pBtnOK = Button::create("content/logo_dec/btn_confirm.png");
    m_pRootNode->addChild(m_pBtnOK,20);
    m_pBtnOK->setPosition(Vec2(rect.getMaxX(), rect.getMaxY()-70));
    m_pBtnOK->addTouchEventListener(CC_CALLBACK_2(DecorateLogoLayer::onButtonCallback, this));
    m_pBtnOK->setTag(1);
    
    m_pBtnCancel = Button::create("content/logo_dec/btn_delete.png");
    m_pRootNode->addChild(m_pBtnCancel,20);
    m_pBtnCancel->setPosition(Vec2(rect.origin.x, rect.getMaxY()-70));
    m_pBtnCancel->addTouchEventListener(CC_CALLBACK_2(DecorateLogoLayer::onButtonCallback, this));
    m_pBtnCancel->setTag(0);

    _showDecorationBanner();
    
    m_pLogoClipping = ClippingNode::create(Sprite::create("content/logo_dec/mask.png"));
    m_pRootNode->addChild(m_pLogoClipping,5);
    m_pLogoClipping->setAlphaThreshold(0.5);
    m_pLogoClipping->setPosition(Vec2(visibleSize.width*0.5,visibleSize.height*0.5+60));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChild(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(100);
    
    std::memset(m_bGuide, 0, 4);
    std::memset(m_bVOGuide, 0, 4);
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){
        if (m_bCanTouch) {
            if(m_sCurTypeStr=="pen_logo"){
                Vec2 pos = this->convertToWorldSpace(pTouch->getLocation());
                m_pScribble->paint(pos);
            }
        }
        return m_bCanTouch;
    };
    listener->onTouchMoved = [=](Touch *pTouch, Event *pEvent){
        if (m_bCanTouch)  {
            if(m_sCurTypeStr=="pen_logo"){
                Vec2 pos = this->convertToWorldSpace(pTouch->getLocation());
                Vec2 posPre = this->convertToWorldSpace(pTouch->getPreviousLocation());
                m_pScribble->paint(posPre,pos);
                return;
            }
        }
    };
    listener->onTouchEnded = [=](Touch *pTouch, Event *pEvent){
        if (m_bCanTouch)  {
            
            m_pGuideLayer->removeGuide();
            std::stringstream ostr;
            ostr<<"content/logo_dec/"<<m_curDecData.pathName<<"/"<<m_nIndex<<".png";
            if(m_sCurTypeStr=="color_logo"){
                if (m_pPattern) {
                    m_pPattern->removeFromParent();
                }
                m_pPattern = Sprite::create(ostr.str());
                m_pLogoClipping->addChild(m_pPattern,0);
                m_pPattern->setScale(1.3);
                
            }else if (m_sCurTypeStr=="pattern_logo"){
                if (m_pPattern) {
                    m_pPattern->removeFromParent();
                }
                m_pPattern = Sprite::create(ostr.str());
                m_pLogoClipping->addChild(m_pPattern,0);
            }
        }
        
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    AudioHelp::getInstance()->playEffect("vo_design_logo.mp3");
    return true;
}

void DecorateLogoLayer::onEnter()
{
    Layer::onEnter();
    
}

void DecorateLogoLayer::onExit()
{
    Layer::onExit();
}

void DecorateLogoLayer::onButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED){
        Button* pBtn = (Button*)ref;
        int tag = pBtn->getTag();
        if (tag==0) {
            _reset();
        }else{
            _saveLogo();
            if(onLogoDecoratedCallback){
                onLogoDecoratedCallback();
            }
            this->setVisible(false);
            this->removeFromParent();
        }
    }
    
}


void DecorateLogoLayer::_reset()
{
    m_pLogoClipping->removeAllChildren();
//    m_pColor = nullptr;
    m_pPattern = nullptr;
    m_pScribble = nullptr;
    m_pSticker = nullptr;
}

bool DecorateLogoLayer::_saveLogo()
{
    Size mixtureSize = m_pBg->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    m_pLogoClipping->setAlphaThreshold(0);
    Vec2 pos = m_pLogoClipping->getPosition();
    m_pLogoClipping->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
    
    if(m_pSticker){
        Vector<Node*> pChildren = m_pSticker->getChildren();
        for_each(pChildren.begin(), pChildren.end(), [=](Node* pNode){
            DecorationUtil* pUnit =  dynamic_cast<DecorationUtil*>(pNode);
            if(pUnit) {
                pUnit->m_bIsShotScreen = true;
            }
        });
    }
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pLogoClipping->Node::visit();
    render->end();
    
    m_pLogoClipping->setPosition(pos);
    m_pLogoClipping->setAlphaThreshold(0.5);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issuccess = pImage->saveToFile(FileUtility::getStoragePath()+ "logo.png", false);
#else
    issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"/logo.png", false);
#endif
//    pImage->autorelease();
    return issuccess;
}

void DecorateLogoLayer::_setStickerTouchEnable(bool enable)
{
    if(m_pSticker){
        Vector<Node*> pChildren = m_pSticker->getChildren();
        for_each(pChildren.begin(), pChildren.end(), [=](Node* pNode){
            DecorationUtil* pUnit =  dynamic_cast<DecorationUtil*>(pNode);
            if(pUnit) {
                pUnit->setTouchEnable(enable);
            }
        });
    }
}
void DecorateLogoLayer::_showDecorationBanner()
{
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(520,100));
    m_pRootNode->addChild(m_pTypeScrollView,20);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  220,200);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(DecorateLogoLayer::_onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/logo_dec/";
    m_pTypeScrollView->setLocalZOrder(20);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionH);
    
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(35);
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("logo"));
    
}
void DecorateLogoLayer::_onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
{
    m_sCurTypeStr = typeData.decTypeName;
    AudioHelp::getInstance()->playSelectedEffect();
    m_nIndex = -1;
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_sCurTypePath = string(data.pathName);
    m_bCanTouch = false;
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/logo_dec/icon/"<<data.pathName<<"/";
        m_pTypeScrollView->setSelectedAnimate(true);
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(580, 100),false);
            m_pRootNode->addChild(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = data;
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateLogoLayer::_onDecorationCallback, this);
        }else{
            m_pDecorationScrollView->decorationData = data;
        }
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateLogoLayer::_onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        m_pDecorationScrollView->setMargin(10);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,  200,200);
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(0,-120)));
        m_pDecorationScrollView->btnPathEX = ostr.str();
        
        //    m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
        m_pDecorationScrollView->boxPathEX = "content/logo_dec/hurdle.png";
        if (typeData.decTypeName!="pen_logo") {
            m_pDecorationScrollView->setMargin(30);
        }
        m_pDecorationScrollView->reloadData();
        
        if (typeData.decTypeName=="pattern_logo") {
            if (!m_bVOGuide[1]) {
                m_bVOGuide[1] = true;
                AudioHelp::getInstance()->playEffect("vo_paint_box.mp3");
            }
        }else if (typeData.decTypeName=="pen_logo") {
            if (!m_bVOGuide[2]) {
                m_bVOGuide[2] = true;
                AudioHelp::getInstance()->playEffect("vo_draw_box.mp3");
            }
        }else if (typeData.decTypeName=="stickers_logo") {
            if (!m_bVOGuide[3]) {
                m_bVOGuide[3] = true;
                AudioHelp::getInstance()->playEffect("vo_stickers_box.mp3");
            }
        }
    }else{
        m_sCurTypePath = "";
        if (m_pDecorationScrollView) {
            m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0,120)),
                                                                CallFunc::create([=]()
                                                                                 {
                                                                                     m_pDecorationScrollView->removeFromParent();
                                                                                     m_pDecorationScrollView = nullptr;
                                                                                 }), NULL));
        }

    }
    
}

void DecorateLogoLayer::_onDecorationCallback(int index,DecorateConfigData decData)
{
    if (!gNoneIap) {
        if (index>=decData.freeCount+decData.beginIndex && !(index<decData.totalCount+decData.beginIndex && index>=decData.holidayIndex && decData.holidayIndex>=0 && decData.holidayCount==0)){

            ShopLayer* pLayer = ShopLayer::create();
            m_pRootNode->addChild(pLayer,100);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss();
            m_pDecorationScrollView->setSelected(false);
            return;
        }
    }
    m_nIndex = index;
    m_bCanTouch = false;
    AudioHelp::getInstance()->playSelectedEffect();
    m_curDecData = decData;
    if(m_sCurTypeStr=="pen_logo"){
        if (!m_pScribble) {
            m_pScribble = ScribbleShapeNode::create(Size(1136,230));;
            m_pScribble->setShape("content/logo_dec/mask.png");
            m_pLogoClipping->addChild(m_pScribble,2);
        }else{
        }
        Color4B color = ConfigManager::getInstance()->getColor().at(index);
        m_pScribble->useBrush("content/make/brush.png",Scribble::eBrush,0.3);
        m_pScribble->antiAliasing();
        m_pScribble->useTargetColor(Color4F(color));
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_bGuide[2] = true;
            m_pGuideLayer->showGuideMove(m_pBg->getPosition(), m_pBg->getPosition()+Vec2(200, 80));
        }
        _setStickerTouchEnable(false);
        return;
    }else if(m_sCurTypeStr=="color_logo"){
        m_bCanTouch = true;
        if (!m_bGuide[0]) {
            m_bGuide[0] = true;
            m_pGuideLayer->showGuideTap(m_pBg->getPosition());
        }
        _setStickerTouchEnable(false);
        return;
        
    }else if (m_sCurTypeStr=="pattern_logo"){
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_bGuide[1] = true;
            m_pGuideLayer->showGuideTap(m_pBg->getPosition());
        }
        _setStickerTouchEnable(false);
        return;
    }
    
    if (m_sCurTypeStr=="stickers_logo"){
        std::stringstream ostr;
        ostr<<"content/logo_dec/"<<decData.pathName<<"/"<<index<<".png";
        _setStickerTouchEnable(true);
        
        if (!m_pSticker) {
            m_pSticker = Node::create();
            m_pLogoClipping->addChild(m_pSticker,4);
        }
        
        Sprite* pSprite = Sprite::create(ostr.str());
        m_pRootNode->addChild(pSprite,30);
        pSprite->setPosition(m_pDecorationScrollView->getWorldItemPos());
        pSprite->runAction(Sequence::create(MoveTo::create(0.5, m_pLogoClipping->getPosition()),
                                            CallFunc::create([=](){
            Sprite* pContent = Sprite::createWithTexture(pSprite->getTexture());
            DecorationUtil* pUnit = DecorationUtil::createDefault(pContent->getContentSize());
            pUnit->addChild(pContent);
            Rect rect = m_pBg->getBoundingBox();
            rect.origin = Vec2(-rect.size.width/2, -rect.size.height/2);
            pUnit->setMoveLimitRect(rect);
            m_pSticker->addChild(pUnit);
            pSprite->removeFromParent();
        }), NULL));

    }

}