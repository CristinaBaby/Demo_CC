
#include "DecorateGiftScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"

static string gDecType[] = {
    "light",
    "balloon",
    "gift_other"
};
DecorateGiftScene::DecorateGiftScene()
{
    m_pDecorationScrollView = nullptr;
}

DecorateGiftScene::~DecorateGiftScene()
{
    
}
bool DecorateGiftScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    int m_nBgIndex = GameDataManager::getInstance()->m_nBgIndex;
    //    bg
    std::stringstream ostr;
    ostr<<"content/category/bg/scene"<<m_nBgIndex<<".jpg";
    Sprite* pBg = Sprite::create(ostr.str());
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    m_pBg = pBg;
    
    
    std::stringstream ostrKid;
    int kidIndex = arc4random()%3;
    ostrKid<<"content/common/make/child"<<kidIndex+1<<"/";
    Sprite* pKid = Sprite::create(ostrKid.str()+"1.png");
    pKid->setTag(0);
    CMVisibleRect::setPosition(pKid, 60,300,kBorderRight);
    this->addChildToContentLayer(pKid);
    pKid->setLocalZOrder(-1);
    
    Sprite* pEye = Sprite::create(ostrKid.str()+"wink.png");
    pKid->addChild(pEye);
    pEye->setName("eye");
    pEye->setPosition(pKid->getContentSize()*0.5);
    pEye->setOpacity(0);
    pEye->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.02),
                                                           DelayTime::create(0.3),
                                                           FadeOut::create(0.02),
                                                           DelayTime::create(3),
                                                           NULL)));
    std::stringstream ostrTable;
    ostrTable<<"content/category/bg/table"<<m_nBgIndex<<".png";
    Sprite* m_pTable = Sprite::create(ostrTable.str());
    this->addChildToContentLayer(m_pTable);
    CMVisibleRect::setPosition(m_pTable, 480, 20);
    
    for (int i = 0; i<m_nType; i++) {
        std::stringstream ostr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ostr<<"dec"<<i<<".png";
#else
        ostr<<"/dec"<<i<<".png";
#endif
        Sprite* pSprite = Sprite::create(FileUtility::getStoragePath()+ostr.str());
        if (pSprite) {
            this->addChildToContentLayer(pSprite);
            pSprite->setPosition(visibleSize*0.5);
        }
    }
    m_pDecorateLayer = Layer::create();
    this->addChildToContentLayer(m_pDecorateLayer);
    
    
    m_pCandleLayer = Layer::create();
    this->addChildToContentLayer(m_pCandleLayer);
    m_pCandleLayer->setLocalZOrder(1);
    m_pCandleLayer->setScale(0.6);
    m_pCandleLayer->setPosition(0, -visibleSize.height*0.1);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_pDecoratedCake = Sprite::create(FileUtility::getStoragePath()+"decorate.png");
#else
    m_pDecoratedCake = Sprite::create(FileUtility::getStoragePath()+"/decorate.png");
#endif
    m_pCandleLayer->addChild(m_pDecoratedCake);
    m_pDecoratedCake->setPosition(visibleSize.width/2,visibleSize.height*0.5);
    
    float delt = 100;
    std::vector<Vec3> candleVector = GameDataManager::getInstance()->m_VectorCandle;
    CandlePosition candlePos;
    Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(320+delt, 260+20));
    candlePos.init(5, 5, 65, 48, pos);
    for_each(candleVector.begin(), candleVector.end(), [=,&candlePos](Vec3 vec){
        int index = vec.x;
        int type = vec.y;
        int tag = vec.z;
        Vec2 worldPos = candlePos.getPosition(index);
        Vec2 posCandle = m_pContentLayer->convertToNodeSpace(worldPos);
        CandleNode* pCandle = CandleNode::create();
        pCandle->initCandle(type, tag);
        pCandle->setFirePosition(CandleData::getCandleData(type).firePos);
        if (type==3) {
            if(tag==6){
                pCandle->setFirePosition(Vec2(CandleData::getCandleData(type).firePos.x+18,CandleData::getCandleData(type).firePos.y-11));
            }else if(tag==7){
                pCandle->setFirePosition(Vec2(CandleData::getCandleData(type).firePos.x+21,CandleData::getCandleData(type).firePos.y));
            }
        }
        pCandle->setPosition(Vec2(posCandle.x, posCandle.y-CandleData::getCandleData(type).holderHeight));
        m_pCandleLayer->addChild(pCandle);
        pCandle->setLocalZOrder(50-index);
        pCandle->setTag(0);
        m_VectorCandle.pushBack(pCandle);
    });

    m_pGameUI->showNextLayout();
    m_pGameUI->showResetLayout();
    
    _showItems();
    return true;
}

void DecorateGiftScene::onEnter()
{
    ExtensionScene::onEnter();
    m_bShowShop = false;
}

void DecorateGiftScene::onExit()
{
    ExtensionScene::onExit();
}
#pragma mark - initData
void DecorateGiftScene::_initData()
{
    setExPath("content/make/dec/");
    
    m_nType = GameDataManager::getInstance()->m_nDecorateStep;
    if (m_nType>=2) {
        m_nNextSceneTag = GameUIEvent::eSceneTagShare;
        GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagShare;
    }else{
        m_nNextSceneTag = GameUIEvent::eSceneTagDecGift;
        GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecGift;
    }
}


void DecorateGiftScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
//    DecorateManager::getInstance()->reset();
    
    m_pDecorateLayer->removeAllChildren();
    
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

void DecorateGiftScene::_showItems()
{
    m_sCurTypeStr = gDecType[m_nType];
    std::stringstream ostr;
    ostr.str("");
    ostr<<"content/category/bg/icon_"<<m_sCurTypeStr<<"/";
    if (!m_pDecorationScrollView) {
        m_pDecorationScrollView = ItemScrollView::createWithSize(Size(170, 440),false);
        this->addChildToUILayer(m_pDecorationScrollView);
        
        m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateGiftScene::onDecorationCallback, this);
    }else{
        m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    }
    m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionV);
    m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(DecorateGiftScene::onDecorationCallback, this);
    m_pDecorationScrollView->stopAllActions();
    //        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, 5, 80,kBorderLeft);
    CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, -300, (640-m_pDecorationScrollView->getContentSize().height)*0.5,kBorderLeft);
    m_pDecorationScrollView->setScale(0.5);
    m_pDecorationScrollView->setMargin(50);
    m_pDecorationScrollView->runAction(Spawn::create(MoveBy::create(0.5, Vec2(330, 0)),
                                                     ScaleTo::create(0.5, 1), NULL));
    m_pDecorationScrollView->btnPathEX = ostr.str();
    m_pDecorationScrollView->reloadData();
    m_pDecorationScrollView->showItemAnimation();
}
void DecorateGiftScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
}

RenderTexture* DecorateGiftScene::getResultRender()
{
    RenderTexture* render = RenderTexture::create(visibleSize.width, visibleSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    
    m_pDecorateLayer->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;
}

void DecorateGiftScene::onButtonCallback(Button* btn)
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
        std::stringstream ostr;
        
        bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ostr<<"dec"<<m_nType<<".png";
#else
        ostr<<"/dec"<<m_nType<<".png";
#endif
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+ostr.str(), false);
        pImage->autorelease();
        log("===save success %d==",issuccess);
    }
    
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        
    }else if (GameUILayoutLayer::eUIButtonTagBack==tag){
        
    }else if (GameUILayoutLayer::eUIButtonTagReset==tag){
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to reset your decoration?");
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
    
    ExtensionScene::onButtonCallback(btn);
}

void DecorateGiftScene::onDecorationCallback(int index,DecorateConfigData decData)
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
    ostr<<"content/category/bg/"<<m_sCurTypeStr<<"/"<<index<<".png";
    
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    this->addChildToUILayer(pParticle);
    if (std::strcmp(m_sCurTypeStr.c_str(), "light")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width,unit->getContentSize().height));
        pDecoration->setIgnoreTip();
        pDecoration->addChild(unit);
        
        Vec2 position = CMVisibleRect::getPosition(250.0f+20*(arc4random()%30), unit->getContentSize().height*0.4,kBorderNone,kBorderTop);
        
        pDecoration->setMoveLimitRect(Rect(0, visibleSize.height-unit->getContentSize().height*0.5, visibleSize.width, unit->getContentSize().height*1.3));
        m_pDecorateLayer->addChild(pDecoration);
        pDecoration->setPosition(position);
        
        pParticle->setPosition(pDecoration->getPosition());

    }else if (std::strcmp(m_sCurTypeStr.c_str(), "balloon")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width,unit->getContentSize().height));
        pDecoration->setIgnoreTip();
        pDecoration->addChild(unit);
        
        Vec2 position = CMVisibleRect::getPosition(250.0f+20*(arc4random()%30), 400.0f+10*(arc4random()%20));
        
        pDecoration->setMoveLimitRect(Rect(0, 0, visibleSize.width, visibleSize.height));
        m_pDecorateLayer->addChild(pDecoration);
        pDecoration->setPosition(position);
        
        pParticle->setPosition(pDecoration->getPosition());
        
    }else if (std::strcmp(m_sCurTypeStr.c_str(), "gift_other")==0) {
        Sprite* unit = Sprite::create(ostr.str());
        DecorationUtil* pDecoration = DecorationUtil::createDefault(Size(unit->getContentSize().width,unit->getContentSize().height));
        pDecoration->setIgnoreTip();
        pDecoration->addChild(unit);
        
        Vec2 position = CMVisibleRect::getPosition(250.0f+20*(arc4random()%30), 400.0f+10*(arc4random()%20));
        
        pDecoration->setMoveLimitRect(Rect(0, 0, visibleSize.width, visibleSize.height));
        m_pDecorateLayer->addChild(pDecoration);
        pDecoration->setPosition(position);
        
        pParticle->setPosition(pDecoration->getPosition());
        
    }else{
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
    
}