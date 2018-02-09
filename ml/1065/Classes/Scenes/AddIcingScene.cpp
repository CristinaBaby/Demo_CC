
#include "AddIcingScene.h"
#include "PhysicsShapeCache.h"
#include "SceneManager.h"

AddIcingScene::AddIcingScene()
{
    m_pTool = nullptr;
    m_pDarkIcing = nullptr;
    m_pBrightIcing = nullptr;
}

AddIcingScene::~AddIcingScene()
{
    
}
bool AddIcingScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("dec_bk.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pStand = Sprite::create(localPath("stand.png"));
    this->addChildToContentLayer(m_pStand);
    CMVisibleRect::setPosition(m_pStand, 480, 150);
//
    m_pCake = Sprite::create(localPath("cake1.png"));
    this->addChildToContentLayer(m_pCake);
    CMVisibleRect::setPosition(m_pCake, 480, 330);
//    m_pCake->setOpacity(100);
//
    int frostingType = GameDataManager::getInstance()->m_nFrostingType;
    int frostingIndex = GameDataManager::getInstance()->m_nFrostingIndex;
    std::stringstream ostr;
    ostr<<"frosting"<<frostingType<<"/"<<frostingIndex<<".png";
    m_pFrosting = Sprite::create(localPath(ostr.str()));
    this->addChildToContentLayer(m_pFrosting);
    m_pFrosting->setPosition(m_pCake->getPosition());
    
    m_pAcingLayer = Layer::create();
    this->addChildToContentLayer(m_pAcingLayer);
    m_pAcingLayer->setLocalZOrder(10);
    
//
    
    
    _showChooseIcing();
    
    m_IcingHSB.init();
    
    m_pGameUI->showResetLayout();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGuideLayer2 = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer2);
    
    return true;
}

#pragma mark - initData
void AddIcingScene::_initData()
{
    setExPath("content/make/dec/");
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecorate;
    m_nCurType = 0;
    m_nCurIndex = 0;
}

void AddIcingScene::onEnter()
{
    ExtensionScene::onEnter();
}

void AddIcingScene::_reset()
{
//    m_pAcingLayer->removeAllChildren();
//    _showChooseIcing();
    SceneManager::replaceTheScene<AddIcingScene>();
}
void AddIcingScene::_showChooseIcing()
{
    ChooseLayer* pLayer = ChooseLayer::create();
    pLayer->showChooseIcing();
    this->addChildToUILayer(pLayer);
    pLayer->setLocalZOrder(6);
    pLayer->onItemCallback = CC_CALLBACK_1(AddIcingScene::_onIcingCallback, this);
}

void AddIcingScene::_showColorScrollView()
{
    Vector<Node*> datas;
    int count = 15;
    for (int i = 0; i<count; i++) {
        ostringstream ostr;
        ostr<<"color/color"<<i<<".png";
//        Sprite* pSprite = Sprite::create(localPath(ostr.str()));
//        pSprite->setTag(i);
//        datas.pushBack(pSprite);
        
        LockItem* pItem = LockItem::create();
        if (!IAPManager::getInstance()->getItemIsBought(2) && i>6 && !gNoneIap) {
            pItem->initWithName(localPath(ostr.str()), "content/common/lock.png",true);
        }else{
            pItem->initWithName(localPath(ostr.str()), "content/common/lock.png");
        }
        pItem->setTag(i);
        pItem->setItemTouchEnable(false);
        datas.pushBack(pItem);
    }
    Sprite* pBg = Sprite::create(localPath("icings_box_2.png"));
    this->addChildToUILayer(pBg);
    CMVisibleRect::setPosition(pBg, -20, 320,kBorderLeft);
    
    Sprite* pPoint = Sprite::create(localPath("pointer.png"));
    this->addChildToUILayer(pPoint);
    CMVisibleRect::setPositionAdapted(pPoint, 20, 320,kBorderLeft);
    pPoint->setLocalZOrder(5);
    
    m_pColorScrollview = CircularScrollView::create(Size(220, 1000));
    CMVisibleRect::setPositionAdapted(m_pColorScrollview, 75, 320,kBorderLeft);
    m_pColorScrollview->setEnableAdjust(true, m_pColorScrollview->getContentSize().height/2);
    m_pColorScrollview->setTouchEnable(true);
    m_pColorScrollview->setAdjustCallBack(CC_CALLBACK_1(AddIcingScene::_onColorItemCallback, this));
    m_pColorScrollview->setSpace(20);
    this->addChildToUILayer(m_pColorScrollview);
    auto _adapter = CircularAdapter::create(m_pColorScrollview);
    _adapter->setDatas(datas);
    m_pColorScrollview->setAdapter(_adapter);
    m_pColorScrollview->scrollToFirst(0.5);
}

void AddIcingScene::_onIcingCallback(int index)
{
    m_nCurType = index;
    switch (m_nCurType) {
        case 0:
            m_nSegment = 30;
            break;
        case 1:
            m_nSegment = 26;
            break;
        case 2:
            m_nSegment = 20;
            break;
        case 3:
            m_nSegment = 18;
            break;
        case 4:
            m_nSegment = 8;
            break;
        case 5:
            m_nSegment = 8;
            break;
        case 6:
            m_nSegment = 30;
            break;
            
        default:
            break;
    }
    if (m_nCurType<4) {
        m_EllipseUpOutter.init(250, 135, Vec2(m_pCake->getPositionX()+10, m_pCake->getPositionY()+50),m_nSegment);
        m_EllipseUpInner.init(185, 100, Vec2(m_pCake->getPositionX()+10, m_pCake->getPositionY()+50),m_nSegment);
        
        m_EllipseDownOutter.init(256, 160, Vec2(m_pCake->getPositionX()+10, m_pCake->getPositionY()-40),m_nSegment);
        m_EllipseDownInner.init(220, 120, Vec2(m_pCake->getPositionX()+10, m_pCake->getPositionY()-40),m_nSegment);
    }else{
        m_EllipseUpOutter.init(0.1, 0.1, Vec2(m_pCake->getPositionX()+10, m_pCake->getPositionY()+50),m_nSegment);
        m_EllipseUpInner.init(0.1, 0.1, Vec2(m_pCake->getPositionX()+10, m_pCake->getPositionY()+50),m_nSegment);
        
        m_EllipseDownOutter.init(256, 160, Vec2(m_pCake->getPositionX(), m_pCake->getPositionY()+20),m_nSegment);
        m_EllipseDownInner.init(220, 120, Vec2(m_pCake->getPositionX(), m_pCake->getPositionY()+20),m_nSegment);
    }
    memset(m_flag, 0, sizeof(m_flag));
    
    _showColorScrollView();
    

    m_pGuideLayer->removeGuide();
    m_pGuideLayer2->removeGuide();
    
    if (m_nCurType<4) {
        m_pGuideLayer->showGuide("content/common/tip/line_up.png", Vec2(m_pCake->getPositionX(), m_pCake->getPositionY()+50));
        m_pGuideLayer2->showGuide("content/common/tip/line_down.png", Vec2(m_pCake->getPositionX(), m_pCake->getPositionY()-120));
    }else{
        m_pGuideLayer->showGuide("content/common/tip/line_down.png", Vec2(m_pCake->getPositionX(), m_pCake->getPositionY()-60));
    }
    int tag = 0;
    m_nCurIndex = 0;
    if (!m_pTool) {
        stringstream ostr;
        ostr<<"bag/"<<tag<<".png";
        m_pTool = _createDrageNode(localPath(ostr.str()));
        this->addChildToUILayer(m_pTool);
        m_pTool->setLocalZOrder(5);
        CMVisibleRect::setPosition(m_pTool, 480, 320+visibleSize.height);
        m_pTool->setActionPoint(Vec2(5, 10));
        m_pTool->setLimitRect(Rect(0, 0, visibleSize.width, visibleSize.height+m_pTool->getContentSize().height/2));;
        m_pTool->setTouchEnabled(false);
        m_pTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height)),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pTool->setTouchEnabled(true);
                                                             }), NULL));
    }else{
        m_pTool->setTouchEnabled(false);
        m_pTool->runAction(Sequence::create(MoveBy::create(0.5, visibleSize),
                                            CallFunc::create([=]()
                                                             {
                                                                 stringstream ostr;
                                                                 ostr<<"bag/"<<tag<<".png";
                                                                 m_pTool->getDragSprite()->setTexture(localPath(ostr.str()));
                                                             }),
                                            MoveTo::create(0.5, visibleSize*0.5),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pTool->setTouchEnabled(true);
                                                             }), NULL));
    }
    
    stringstream ostr;
    ostr<<"icing/icing"<<m_nCurType<<"_0.png";
    
    stringstream ostr2;
    ostr2<<"icing/icing"<<m_nCurType<<"_1.png";
    
    CC_SAFE_RELEASE(m_pDarkIcing);
    CC_SAFE_RELEASE(m_pBrightIcing);
    m_pDarkIcing = Sprite::create(localPath(ostr.str()));
    m_pBrightIcing = Sprite::create(localPath(ostr2.str()));
    m_pDarkIcing->retain();
    m_pBrightIcing->retain();
}

void AddIcingScene::_onColorItemCallback(Node*pItem)
{
    if (!pItem) {
        return;
    }
    int tag = pItem->getTag();
    if (m_nCurIndex == tag) {
        return;
    }
    LockItem* pLockItem = dynamic_cast<LockItem*>(pItem);
    if (pLockItem) {
        if (pLockItem->isLocked()) {
            m_pColorScrollview->scrollToItem(m_nCurIndex+1, 0.5);
//            SceneManager::pushTheScene<ShopScene>();
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss();
            return;
        }
    }
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/bling.plist");
    pItem->addChild(pParticle);
    pParticle->setPosition(Vec2::ZERO);
    if (!m_pTool) {
        stringstream ostr;
        ostr<<"bag/"<<tag<<".png";
        m_pTool = _createDrageNode(localPath(ostr.str()));
        this->addChildToUILayer(m_pTool);
        m_pTool->setLocalZOrder(50);
        CMVisibleRect::setPosition(m_pTool, 480, 320+visibleSize.height);
        m_pTool->setActionPoint(Vec2(5, 10));
        m_pTool->setLimitRect(Rect(0, 0, visibleSize.width, visibleSize.height+m_pTool->getContentSize().height/2));;
        m_pTool->setTouchEnabled(false);
        m_pTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height)),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pTool->setTouchEnabled(true);
                                                             }), NULL));
    }else{
        m_pTool->setTouchEnabled(false);
        m_pTool->runAction(Sequence::create(MoveBy::create(0.5, visibleSize),
                                            CallFunc::create([=]()
                                                             {
                                                                 stringstream ostr;
                                                                 ostr<<"bag/"<<tag<<".png";
                                                                 m_pTool->getDragSprite()->setTexture(localPath(ostr.str()));
                                                                                                      }),
                                            MoveTo::create(0.5, visibleSize*0.5),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pTool->setTouchEnabled(true);
                                                             }), NULL));
    }
    m_IcingHSB = ConfigManager::getInstance()->getIcingHSB(tag);
    
    CC_SAFE_RELEASE(m_pDarkIcing);
    CC_SAFE_RELEASE(m_pBrightIcing);
    if (tag==0) {
        stringstream ostr;
        ostr<<"icing/icing"<<m_nCurType<<"_0.png";
        
        stringstream ostr2;
        ostr2<<"icing/icing"<<m_nCurType<<"_1.png";
        
        m_pDarkIcing = Sprite::create(localPath(ostr.str()));
        m_pBrightIcing = Sprite::create(localPath(ostr2.str()));
    }else{
        stringstream ostrIcing;
        ostrIcing<<"icing/icing"<<m_nCurType<<"_0.png";
        
        stringstream ostrIcing2;
        ostrIcing2<<"icing/icing"<<m_nCurType<<"_1.png";
        
        stringstream ostr;
        ostr<<"icing/14_icing"<<m_nCurType<<"_0.png";
        
        stringstream ostr2;
        ostr2<<"icing/14_icing"<<m_nCurType<<"_1.png";
        CCImageColorSpace sp;
        
        
        m_pDarkIcing = Sprite::create(localPath(ostrIcing.str()));
        m_pBrightIcing = Sprite::create(localPath(ostrIcing2.str()));
        
        CCImageColorSpace::RGBA rgba = sp.getRGBA(localPath(ostr.str()),m_IcingHSB.hsbDark.x, m_IcingHSB.hsbDark.y, m_IcingHSB.hsbDark.z);
        CCImageColorSpace::RGBA rgba2 = sp.getRGBA(localPath(ostr2.str()),m_IcingHSB.hsbBright.x, m_IcingHSB.hsbBright.y, m_IcingHSB.hsbBright.z);

        m_pDarkIcing->setColor(Color3B(rgba.r,rgba.g,rgba.b));
        m_pBrightIcing->setColor(Color3B(rgba2.r,rgba2.g,rgba2.b));
//        m_pBrightIcing->setColor(Color3B(rgba.r,rgba.g,rgba.b));

    }
    m_pDarkIcing->retain();
    m_pBrightIcing->retain();
    m_nCurIndex = tag;
}

void AddIcingScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagNext) {
        Image* pImage = getResultRender()->newImage();
        
        bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"icing.png", false);
#else
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+"/icing.png", false);
#endif
        pImage->autorelease();
        log("===save success %d==",issuccess);
    }else if (tag==GameUILayoutLayer::eUIButtonTagReset) {
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to reset the icing?");
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

RenderTexture* AddIcingScene::getResultRender()
{
    Size cakeSize = m_pCake->getContentSize();
    RenderTexture* render = RenderTexture::create(cakeSize.width, cakeSize.height*1.2,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    m_pAcingLayer->setPosition(Vec2(-m_pCake->getPositionX()+cakeSize.width*0.5,-m_pCake->getPositionY()+cakeSize.height*0.6));
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pAcingLayer->visit();
    render->end();
    m_pAcingLayer->setPosition(Vec2::ZERO);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;
}

void AddIcingScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(2)) {
        Vector<Node*> vec = m_pColorScrollview->getAllItems();
        for_each(vec.begin(), vec.end(), [=](Node* pNode){
            LockItem* pItem = dynamic_cast<LockItem*>(pNode);
            if (pItem) {
                pItem->unlock();
            }
        });
    }
}
void AddIcingScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    
}
void AddIcingScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (m_pTool==pDragNode) {
        Vec2 po = m_pContentLayer->convertToNodeSpace(worldPoint);
        if (!m_EllipseUpInner.containPoint(po)&& m_EllipseUpOutter.containPoint(po)) {
            Vec3 vec = m_EllipseUpOutter.getSegmentPosition(po);
            int sem = vec.z;
            int mid = (m_nSegment-1)/2;
            if (sem<m_nSegment) {
//                if (!m_flag[sem]) {
                    m_flag[sem] =  true;
                    Vec2 pos = Vec2(vec.x, vec.y+10);
                    Sprite* add = Sprite::createWithTexture(m_pBrightIcing->getTexture());
                    m_pAcingLayer->addChild(add);
                    add->setColor(m_pBrightIcing->getColor());
                    add->setLocalZOrder(sem+1);
                    Vec2 tempVec = Vec2(m_EllipseUpOutter.center, pos);
                    bool rotate = false;
                    if (m_nCurType!=2 && m_nCurType!=1) {
                        rotate = true;
                    }
                    float angle = 0;
                    if (sem<=mid) {
                        if (m_nCurType!=2 && m_nCurType!=1) {
                            angle = 10-sem*180.0f/m_nSegment;
                        }else if (m_nCurType==4) {
                            angle = 10-sem*180.0f/m_nSegment;
                        }
                        add->setRotation(angle);
                        add->setPosition(Vec2(pos.x, pos.y));
                    }else{
                        if (m_nCurType!=2 && m_nCurType!=1) {
                            angle = -sem*180.0f/m_nSegment-80;
                        }else if (m_nCurType==4) {
                            angle = -sem*180.0f/m_nSegment-80;
                        }
                        add->setRotation(angle);
                        add->setPosition(Vec2(pos.x, pos.y));

                    }
                    if (sem==0) {
                        Sprite* add2 = Sprite::createWithTexture(add->getTexture());
                        m_pAcingLayer->addChild(add2);
                        add2->setColor(m_pBrightIcing->getColor());
                        add2->setLocalZOrder(sem+1);
                        if (m_nCurType!=2 && m_nCurType!=1) {
                            angle = 10-sem*180.0f/m_nSegment;
                        }else if (m_nCurType==4) {
                            angle = 10-sem*180.0f/m_nSegment;
                        }
                        add2->setRotation(angle);
                        
                        add->setPosition(Vec2(pos.x, pos.y-20));
                        add2->setPosition(Vec2(pos.x+20, pos.y-60));
                    }else if(sem==mid){
                        Sprite* add2 = Sprite::createWithTexture(add->getTexture());
                        m_pAcingLayer->addChild(add2);
                        add2->setColor(m_pBrightIcing->getColor());
                        add2->setLocalZOrder(sem+1);
                        if (m_nCurType!=2 && m_nCurType!=1) {
                            angle = 10-sem*180.0f/m_nSegment;
                        }else if (m_nCurType==4) {
                            angle = 10-sem*180.0f/m_nSegment;
                        }
                        add2->setRotation(angle);
                        add2->setPosition(Vec2(pos.x+20, pos.y+40));
                        
                        Sprite* add3 = Sprite::createWithTexture(add->getTexture());
                        m_pAcingLayer->addChild(add3);
                        add3->setColor(m_pBrightIcing->getColor());
                        add3->setLocalZOrder(sem+1);
                        if (m_nCurType!=2 && m_nCurType!=1) {
                            angle = -10-sem*180.0f/m_nSegment;
                        }else if (m_nCurType==4) {
                            angle = -10-sem*180.0f/m_nSegment;
                        }
                        add3->setRotation(angle);
                        add3->setPosition(Vec2(pos.x+20, pos.y+80));
                    }else if (sem==mid+1) {
                        Sprite* add2 = Sprite::createWithTexture(add->getTexture());
                        m_pAcingLayer->addChild(add2);
                        add2->setColor(m_pBrightIcing->getColor());
                        add2->setLocalZOrder(sem);
                        add2->setRotation(-sem*180.0f/m_nSegment-90);
                        add2->setPosition(Vec2(pos.x, pos.y));
                        if (m_nCurType!=2 && m_nCurType!=1) {
                            angle = -20-sem*180.0f/m_nSegment;
                        }else if (m_nCurType==4) {
                            angle = -20-sem*180.0f/m_nSegment;
                        }
                        add->setRotation(angle);
                        add->setPosition(Vec2(pos.x+20, pos.y-50));
                    }else if(sem==m_nSegment-1){
                        Sprite* add2 = Sprite::createWithTexture(add->getTexture());
                        m_pAcingLayer->addChild(add2);
                        add2->setColor(m_pBrightIcing->getColor());
                        add2->setLocalZOrder(sem);
                        if (m_nCurType!=2 && m_nCurType!=1) {
                            angle = -sem*180.0f/m_nSegment-90;
                        }else if (m_nCurType==4) {
                            angle = -sem*180.0f/m_nSegment-90;
                        }
                        add2->setRotation(angle);
                        add2->setPosition(Vec2(pos.x+10, pos.y+30));
                    }
//                }
            };
        }
        if (!m_EllipseDownInner.containPoint(po)&& m_EllipseDownOutter.containPoint(po)) {
            Vec3 vec = m_EllipseDownOutter.getSegmentPosition(po);
            int sem = vec.z;
            int mid = (m_nSegment-1)/2;
            if (sem<m_nSegment/2) {
//                if (!m_flag[sem+m_nSegment]) {
                    m_flag[sem+m_nSegment] =  true;
                    Vec2 pos = Vec2(vec.x, vec.y);
                    Sprite* add = Sprite::createWithTexture(m_pDarkIcing->getTexture());
                    m_pAcingLayer->addChild(add);
                    add->setColor(m_pDarkIcing->getColor());
                    add->setLocalZOrder(sem+1);
                    Vec2 tempVec = Vec2(m_EllipseDownOutter.center, pos);
                    bool rotate = false;
                    if (m_nCurType!=2 && m_nCurType!=1) {
                        rotate = true;
                    }
                    float angle = 0;
                    if (sem<=mid) {
                        if (m_nCurType!=2 && m_nCurType!=1) {
                            angle = 10-sem*180.0f/m_nSegment;
                        }
                        if (m_nCurType>=4) {
                            angle = (m_nSegment/4-sem)*180.0f/m_nSegment;
                        }
                        add->setRotation(angle);
                        add->setPosition(Vec2(pos.x, pos.y));
                        if (sem==0) {
                            if (m_nCurType!=2 && m_nCurType!=1) {
                                angle = 10-sem*180.0f/m_nSegment;
                            }
                            if (m_nCurType>=4) {
                                angle = (m_nSegment/4-sem)*180.0f/m_nSegment;
                            }
                            if (m_nCurType<4){
                                Sprite* add2 = Sprite::createWithTexture(add->getTexture());
                                m_pAcingLayer->addChild(add2);
                                add2->setColor(m_pDarkIcing->getColor());
                                add2->setLocalZOrder(sem+1);
                                add2->setRotation(angle);
                                add2->setPosition(Vec2(pos.x+20, pos.y-60));
                                add->setPosition(Vec2(pos.x, pos.y-20));
                            }else{
                                add->setPosition(Vec2(pos.x+30, pos.y-70));
                            }
                        }else if(sem==mid){
                            Sprite* add2 = Sprite::createWithTexture(add->getTexture());
                            m_pAcingLayer->addChild(add2);
                            add2->setColor(m_pDarkIcing->getColor());
                            add2->setLocalZOrder(sem+1);
                            if (m_nCurType!=2 && m_nCurType!=1) {
                                angle = 10-sem*180.0f/m_nSegment;
                            }
                            if (m_nCurType>=4) {
                                angle = (m_nSegment/4-sem-1)*180.0f/m_nSegment;
                            }
                            add2->setRotation(angle);
                            add2->setPosition(Vec2(pos.x+20, pos.y+40));
                            if (m_nCurType>=4){
                                add2->setPosition(Vec2(pos.x+100, pos.y+60));
                            }
                        }
                        log("=====angle %f",angle);
                    }
//                }
            };
        }
//        bool isFull = true;
//        if (m_nCurType<4){
//            for (int i = 0; i<m_nSegment*1.5; i++) {
//                if (!m_flag[i]) {
//                    isFull = false;
//                    break;
//                }
//            }
//        }else{
//            for (int i = m_nSegment; i<m_nSegment*1.5; i++) {
//                if (!m_flag[i]) {
//                    isFull = false;
//                    break;
//                }
//            }
//        }
//        if (isFull) {
//            pDragNode->ignoreMoving = true;
//            m_pGameUI->showNextLayout();
//            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
//            this->addChildToUILayer(pParticle);
//            pParticle->setPosition(visibleSize*0.5);
//        }
        m_pGameUI->showNextLayout();
    }
}
void AddIcingScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    
}
void AddIcingScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}