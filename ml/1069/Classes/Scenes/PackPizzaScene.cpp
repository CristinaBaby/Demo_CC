
#include "PackPizzaScene.h"
#include "SceneManager.h"
#include "ShopLayer.h"

PackPizzaScene::PackPizzaScene()
{
    m_pDecorationScrollView = nullptr;
    m_bPacked = false;
    m_bCanDecorate = true;
}

PackPizzaScene::~PackPizzaScene()
{
    
}
bool PackPizzaScene::init()
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
    m_pBg = pBg;
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(150,540));
    this->addChildToUILayer(m_pTypeScrollView);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  0-visibleSize.width/2,(visibleSize.height-m_pTypeScrollView->getContentSize().height)/2,kBorderLeft);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(PackPizzaScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionV);
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                  EaseBackIn::create(MoveBy::create(1, Vec2(visibleSize.width/2, 0))),
                                                  DelayTime::create(2), NULL));
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(20);
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("pack"));

    _initDefaultDecorate();
    
    m_pGameUI->showNextLayout();
    m_pGameUI->showResetLayout();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/dysc.png",CMVisibleRect::getPosition(320+visibleSize.width, 650));
    
    return true;
}

void PackPizzaScene::onEnter()
{
    ExtensionScene::onEnter();
    if (GameDataManager::getInstance()->m_nPackage==0){
        FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_DECORATE_SNOWCONE);
    }else{
        FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_DECORATE_ICECREAM);
    }
}

void PackPizzaScene::onExit()
{
    if (GameDataManager::getInstance()->m_nPackage==0){
        FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_DECORATE_SNOWCONE);
    }else{
        FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_DECORATE_ICECREAM);
    }
    ExtensionScene::onExit();
}
#pragma mark - initData
void PackPizzaScene::_initData()
{
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    setExPath("content/dec/");
    m_nNextSceneTag = GameUIEvent::eSceneTagShare;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagShare;
}

void PackPizzaScene::_initDefaultDecorate()
{
    m_pPizza = Node::create();
    this->addChildToContentLayer(m_pPizza);
    m_pPizza->setLocalZOrder(10);
//    ******************      normal pizza       **********************
    if (GameDataManager::getInstance()->m_nPackage==0) {
        CMVisibleRect::setPositionAdapted(m_pPizza,Vec2(530, 235-visibleSize.height));
        Sprite*pPlate = Sprite::create(localPath("bottom.png"));
        m_pPizza->addChild(pPlate);
        
        Sprite*pPia = Sprite::create(localPath("pizza.png"));
        m_pPizza->addChild(pPia);
        pPia->setPosition(Vec2(0, 15));
        
        std::string str;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        str = FileUtility::getStoragePath()+"normal_decripe.png";
#else
        str = FileUtility::getStoragePath()+"/normal_decripe.png";
#endif
        Sprite*pPizza = Sprite::create(str);
        
        //        380,260
        if (pPizza) {
            pPizza->setScale(1, 0.75);
            m_pPizza->addChild(pPizza);
            pPia->setPosition(Vec2(0, 5));
        }
        
        auto action = Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
        m_pPizza->runAction(action);

    }else {
//    gummy pizza
        CMVisibleRect::setPositionAdapted(m_pPizza,Vec2(530, 235-visibleSize.height));
        
        Sprite*pPlate = Sprite::create(localPath("palte.png"));
        m_pPizza->addChild(pPlate);
        
        std::string str;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        str = FileUtility::getStoragePath()+"gummypiece.png";
#else
        str = FileUtility::getStoragePath()+"/gummypiece.png";
#endif
        Sprite*pPizza = Sprite::create(str);
//        380,260
        if (pPizza) {
            pPizza->setScale(1, 0.75);
            m_pPizza->addChild(pPizza);
            pPizza->setPosition(Vec2(0, 15));
        }
        
        auto action = Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
        m_pPizza->runAction(action);
    }

}

void PackPizzaScene::_reset()
{
    AudioHelp::getInstance()->playResetEffect();
    DecorateManager::getInstance()->reset();
    
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

void PackPizzaScene::_showBox()
{
    m_pBox = Sprite::create(localPath("box2.png"));
    this->addChildToContentLayer(m_pBox);
    CMVisibleRect::setPositionAdapted(m_pBox, 500, 600+visibleSize.height);
    
    m_pBoxFront = Sprite::create(localPath("box2_2.png"));
    this->addChildToContentLayer(m_pBoxFront);
    CMVisibleRect::setPositionAdapted(m_pBoxFront, 500, 600+visibleSize.height);
    m_pBoxFront->setLocalZOrder(5);
    auto action = MoveBy::create(1, Vec2(0, -visibleSize.height));
    m_pBox->runAction(action);
    m_pBoxFront->runAction(action->clone());
    
    m_pBoxNode = Node::create();
    this->addChildToContentLayer(m_pBoxNode);
    m_pBoxNode->setLocalZOrder(10);
    m_pBoxNode->setPosition(m_pPizza->getPosition());
}

void PackPizzaScene::_moveBox()
{
    auto action = Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                   CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    m_pBox->runAction(action);
    m_pBoxFront->runAction(action->clone());
}

void PackPizzaScene::_pack()
{
    m_pPizza->runAction(Sequence::create(JumpBy::create(0.8, Vec2::ZERO, 150, 1),
                                         CallFunc::create([=](){
        m_pPizza->setVisible(false);
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
    }), NULL));
}

void PackPizzaScene::_unpack()
{
    m_pBoxNode->removeAllChildren();
    m_pBox->setVisible(true);
    m_pPizza->runAction(Sequence::create(DelayTime::create(0.2),
                                         CallFunc::create([=](){
        m_pPizza->setVisible(true);
    }),
                                         DelayTime::create(0.4),
                                         
                                         JumpBy::create(1, Vec2::ZERO, 150, 1), NULL));
    
    m_pBoxFront->setVisible(true);
    m_pBox->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("box"), 2, 0,false,true,0.2),
                                       CallFunc::create([=](){
        m_pBoxFront->setLocalZOrder(0);
        m_pBoxFront->runAction(MoveBy::create(0.4, Vec2(0, 100)));
    }),
                                       MoveBy::create(0.4, Vec2(0, 100)),
                                       CallFunc::create([=](){
        _moveBox();
    }),
                                       NULL));
}
void PackPizzaScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
}

RenderTexture* PackPizzaScene::getResultRender()
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

void PackPizzaScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        
        _saveBox();
        
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
//        Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
//        dialog->setContentText("Are you sure you want to reset your Sweet Pizza?");
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
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
    }
    
}

void PackPizzaScene::onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
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
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(180, 550),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = data;
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(PackPizzaScene::onDecorationCallback, this);
        }else{
            m_pDecorationScrollView->decorationData = data;
        }
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionV);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_2(PackPizzaScene::onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        m_pDecorationScrollView->setMargin(10);
//        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, (640-m_pDecorationScrollView->getContentSize().width)*0.5,60-visibleSize.height/2);
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)));
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, (640-m_pDecorationScrollView->getContentSize().width)*0.5,720+visibleSize.height/2);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView,  120-visibleSize.width/2,(visibleSize.height-m_pDecorationScrollView->getContentSize().height)/2,kBorderLeft);
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2,0)));
        m_pDecorationScrollView->btnPathEX = ostr.str();
        
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
        m_pDecorationScrollView->boxPathEX = "content/category/icon/box_1.png";
        if (std::strcmp(m_sCurTypePath.c_str(), "sprinkle")==0 || std::strcmp(m_sCurTypePath.c_str(), "fruit")==0  || std::strcmp(m_sCurTypePath.c_str(), "candy")==0) {
            m_pDecorationScrollView->setMargin(30);
        }else{
            m_pDecorationScrollView->setMargin(0);
        }
        m_pDecorationScrollView->setMargin(30);
        m_pDecorationScrollView->reloadData();
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

void PackPizzaScene::onDecorateOK()
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
void PackPizzaScene::onDecorationCallback(int index,DecorateConfigData decData)
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
            
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss();
            m_pDecorationScrollView->setSelected(false);
            
            return;
        }
    }
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
        }),  NULL));
        
        m_bCanDecorate = true;
    }else{
        m_bCanDecorate = true;
        _addDecoration(index,decData);
    }
}

void PackPizzaScene::_addDecoration(int index,DecorateConfigData decData)
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
        m_pBoxNode->addChild(pUnit,3);
    }
    
    AudioHelp::getInstance()->playEffect("decoration_texture.mp3");
    
    this->addChildToUILayer(pParticle);
}

void PackPizzaScene::_saveBox()
{
    Size mixtureSize = Size(m_boxSize.width+100,m_boxSize.height+100);
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 pos = m_pBoxNode->getPosition();
    m_pBoxNode->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
    
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pBoxNode->visit();
    render->end();
    
    m_pBoxNode->setPosition(pos);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long time =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
    std::string name = StringUtils::format("%ld.jpg",time);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issuccess = pImage->saveToFile(FileUtility::getStoragePath()+ name, false);
#else
    issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"/"+ name, false);
#endif
    pImage->autorelease();
}