
#include "GetIcecreamScene.h"
#include "SceneManager.h"

GetIcecreamScene::GetIcecreamScene()
{
    m_pCone = nullptr;
}

GetIcecreamScene::~GetIcecreamScene()
{
    
}
bool GetIcecreamScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    Sprite* pBg = Sprite::create(localPath("bg/bg2.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pTable = Sprite::create(localPath("bg/table.png"));
    CMVisibleRect::setPosition(pTable, 320, 180);
    this->addChildToBGLayer(pTable);
    
    m_pIcecreamNode0 = Node::create();
    this->addChildToContentLayer(m_pIcecreamNode0);
    m_pIcecreamNode0->setLocalZOrder(3);
    
    m_pIcecreamNode = Node::create();
    this->addChildToContentLayer(m_pIcecreamNode);
    m_pIcecreamNode->setLocalZOrder(5);
    
    _showScrollView();
    this->runAction(Sequence::create(DelayTime::create(5),
                                     CallFunc::create([=](){
        _showCone();
    }), NULL));
    
    _showShelf();
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/cyis.png");
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    return true;
}
void GetIcecreamScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(1)){
        Vector<Node*> Children = m_pScrollView->getInnerContainer()->getChildren();
        for_each(Children.begin(), Children.end(), [=](Node* pBall){
            LockItem* pItem = dynamic_cast<LockItem*>(pBall);
            if (pItem) {
                pItem->unlock();
            }
        });
        
    }
}

RenderTexture* GetIcecreamScene::getResultRender()
{
    RenderTexture* render = RenderTexture::create(visibleSize.width, visibleSize.height*1.5,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pIcecreamNode->visit();
    render->end();
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;
}
void GetIcecreamScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
        
        
        Image* pImage = getResultRender()->newImage();
        std::stringstream ostr;
        
        bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ostr<<"icecream.png";
#else
        ostr<<"/icecream.png";
#endif
        issuccess = pImage->saveToFile(FileUtility::getStoragePath()+ostr.str(), false);
        pImage->autorelease();
        GameDataManager::getInstance()->m_bColored = true;
        log("===save success %d==",issuccess);
        SceneManager::replaceTheScene<DecorateScene>();
        return;
    }else if (btn->getTag()==GameUILayoutLayer::eUIButtonTagReset) {
        btn->setEnabled(false);
        resetIcecreamBall();
        btn->runAction(Sequence::create(DelayTime::create(1),
                                        CallFunc::create([=]()
                                                         {
                                                             btn->setEnabled(true);
                                                         }), NULL));
    }
    ExtensionScene::onButtonCallback(btn);

}
void GetIcecreamScene::resetIcecreamBall()
{
    m_pIcecreamNode0->removeAllChildren();
    m_pIcecreamNode->removeAllChildren();
    m_pGameUI->hideReset();
    m_pGameUI->hideNext();
    m_Icecreams.clear();
}
void GetIcecreamScene::onItemTouchBegin(LockItem* pItem){
//    if (m_pScrollView->isScrolled()) {
//        return;
//    }
//    int tag = pItem->getTag();
//    DragNode* pNode = m_IceCreamIcons.at(tag);
//    pNode->setVisible(true);
}


void GetIcecreamScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
}
void GetIcecreamScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->isMoving()){
        pDragNode->setVisible(true);
        pDragNode->setScale(1);
    }
}
void GetIcecreamScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if(!m_pCone){
        return;
    }
    m_pGuideLayer->removeGuide();
    int tag = pDragNode->getTag();
    if (pDragNode->isVisible()) {
        std::stringstream ostr;
        ostr<<"choose/icecream/icecream_"<<tag<<".png";
        Sprite* pSprite = Sprite::create(localPath(ostr.str()));
        pSprite->setPosition(m_pIcecreamNode->convertToNodeSpace(worldPoint));
        pSprite->setScale(1);
        LockItem* pItem = (LockItem*)m_pScrollView->getInnerContainer()->getChildByTag(tag);
        if (pItem->isLocked()){
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss(true);
            
            pSprite->setLocalZOrder(10);
            pSprite->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height)),
                                                CallFunc::create([=](){
                pSprite->removeFromParent();
            }), NULL));
            this->addChildToContentLayer(pSprite);
            
            pDragNode->setVisible(false);
            pDragNode->setScale(0.8,0.5);
            pDragNode->setPosition(pDragNode->getOrgPosition());
            return;
        }
        if (m_Icecreams.size()==0) {
            Rect rect = Rect(0,390,180,120);
            rect.origin = m_pCone->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                pSprite->setPosition(CMVisibleRect::getPosition(380, 425));
                pSprite->setScale(0.9);
                GameDataManager::getInstance()->m_nIcecreamIndex = tag;
                m_pIcecreamNode0->addChild(pSprite);
                m_Icecreams.pushBack(pSprite);
                m_pGameUI->showResetLayout();
                m_pGameUI->showNextLayout();
            }else{
                this->addChildToContentLayer(pSprite);
                pSprite->setLocalZOrder(10);
                pSprite->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height)),
                                                    CallFunc::create([=](){
                    pSprite->removeFromParent();
                }), NULL));
            }
        }else{
            Rect rect = Rect(0,390,200,90);
            rect.origin = m_pCone->convertToWorldSpace(rect.origin);
            Rect rect2 = Rect::ZERO;
            for_each(m_Icecreams.begin(), m_Icecreams.end(), [=,&rect2](Node* pNode){
//                if (rect2.getMaxY()<pNode->getBoundingBox().getMaxY()) {
//                    rect2 = pNode->getBoundingBox();
//                }
                Rect tempRect = pNode->getBoundingBox();
                tempRect.origin = pNode->getParent()->convertToWorldSpace(tempRect.origin);
                if (rect2.getMaxY()<tempRect.getMaxY()) {
                    rect2 = tempRect;
                }
            });
//            rect2.origin = m_pIcecreamNode->convertToWorldSpace(rect.origin);
            rect.size.height = rect2.getMaxY()-rect.origin.y;
            rect.size.height += 80;
//            rect.origin.y = rect2.getMaxY()-50;
//            rect.size.height = rect.size.height>(visibleSize.height-pDragNode->getContentSize().height/2)?(visibleSize.height-pDragNode->getContentSize().height/2):rect.size.height;
            if (m_Icecreams.size()<5 && rect.containsPoint(worldPoint)) {
                m_Icecreams.pushBack(pSprite);
                m_pIcecreamNode->addChild(pSprite);
                
            }else{
                this->addChildToContentLayer(pSprite);
                pSprite->setLocalZOrder(10);
                pSprite->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height)),
                                                    CallFunc::create([=](){
                    pSprite->removeFromParent();
                }), NULL));
            }
        }
    }
    pDragNode->setVisible(false);
    pDragNode->setScale(0.8,0.5);
    pDragNode->setPosition(pDragNode->getOrgPosition());
}

void GetIcecreamScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}
#pragma mark - initData
void GetIcecreamScene::_initData()
{
    setExPath("content/make/icecream/");
}


void GetIcecreamScene::_showShelf()
{
    m_pShelfNode = Node::create();
    this->addChildToBGLayer(m_pShelfNode);
    m_pShelfNode->setPosition(Vec2(0, visibleSize.height/2));
    
    
    m_pShelf = Sprite::create(localPath("bg/table2_3.png"));
    m_pShelfNode->addChild(m_pShelf);
    CMVisibleRect::setPositionAdapted(m_pShelf, 320, 840,kBorderNone,kBorderTop);
    m_pShelf->setScaleX(-1);
    m_pShelf->setAnchorPoint(Vec2(0.5, 0.9));
    
    m_pSounder = Sprite::create(localPath("bg/speakers_big.png"));
    m_pShelfNode->addChild(m_pSounder);
    CMVisibleRect::setPositionAdapted(m_pSounder, 640-68, 800,kBorderNone,kBorderTop);
    
    m_pSounder2 = Sprite::create(localPath("bg/speakers_small.png"));
    m_pShelfNode->addChild(m_pSounder2);
    CMVisibleRect::setPositionAdapted(m_pSounder2, 640-184, 814,kBorderNone,kBorderTop);
    
    m_pSounder->setScaleX(-1);
    m_pSounder2->setScaleX(-1);
    m_pShelfNode->runAction(Sequence::create(DelayTime::create(0.5),
                                             EaseBackOut::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height/2))), NULL));
    
    for (int i = 0; i<3; i++) {
        ParticleSystemQuad* pNode = ParticleSystemQuad::create("particle/note.plist");
        std::stringstream ostr;
        int radom = arc4random()%6+1;
        ostr<<"particle/note_"<<radom<<".png";
        pNode->setTexture(Sprite::create(ostr.str())->getTexture());
        m_pSounder->addChild(pNode);
        pNode->setPosition(m_pSounder->getContentSize()*0.5);
    }
    for (int i = 0; i<3; i++) {
        ParticleSystemQuad* pNode = ParticleSystemQuad::create("particle/note.plist");
        std::stringstream ostr;
        int radom = arc4random()%6+1;
        ostr<<"particle/note_"<<radom<<".png";
        pNode->setTexture(Sprite::create(ostr.str())->getTexture());
        m_pSounder2->addChild(pNode);
        pNode->setPosition(m_pSounder->getContentSize()*0.5);
    }
}

void GetIcecreamScene::_showScrollView()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(200, visibleSize.height));
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 0,kBorderLeft,kBorderBottom);
    this->addChildToUILayer(m_pScrollView);
    m_pScrollView->setClippingEnabled(false);
    m_pScrollView->setTouchEnabled(false);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    m_pScrollView->setScrollBarEnabled(false);
    
    m_pScrollView->runAction(Sequence::create(DelayTime::create(2),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pScrollView->scrollToBottom(3, true);
                                                               }),
                                              DelayTime::create(3),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pScrollView->scrollToTop(1, true);
                                                               }),
                                              DelayTime::create(1),
                                              CallFunc::create([=]()
                                                               {
                                                                   m_pGuideLayer->showGuideMove(Vec2(200, visibleSize.height*0.5), visibleSize*0.5);
                                                                   m_pScrollView->setTouchEnabled(true);
                                                               }),
                                              NULL));
    
    int count = 11;
    for (int i = 0; i<count; i++) {
        std::stringstream ostr;
        ostr<<"choose/icecream/icecream_"<<i<<".png";
        DragNode* pFlavor = _createDrageNode(localPath(ostr.str()));
        pFlavor->setPosition(Vec2(50+150*i, 100));
        pFlavor->setTag(i);
        pFlavor->setCanSwallow(false);
        pFlavor->setDirection(DragNode::Direction::HORIZONTAL);
        m_pScrollView->getInnerContainer()->addChild(pFlavor,5);
        pFlavor->setVisible(false);
        pFlavor->setTouchEnabled(false);
        pFlavor->setPosition(Vec2(100, 200+150*(count-1-i)));
        pFlavor->setOrgPositionDefault();
        m_IceCreamIcons.pushBack(pFlavor);
        pFlavor->setScale(0.8,0.5);
        
        std::stringstream ostrIcon;
        ostrIcon<<"choose/"<<i<<".png";
        LockItem* pItem = LockItem::create();
        pItem->setBg(localPath("choose/box.png"));
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(GetIcecreamScene::onItemTouchBegin, this);
        if (!IAPManager::getInstance()->getItemIsBought(1) && !gNoneIap && i>=5) {
            pItem->initWithName(localPath(ostrIcon.str()), "content/common/lock.png",true);
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
//                pLock->setPosition(Vec2(-50, -10));
                pLock->setVisible(false);
            }
        }else{
            pItem->initWithName(localPath(ostrIcon.str()), "");
        }
        if (i==0) {
            pItem->setPosition(Vec2(100+visibleSize.width, 200+150*(count-1-i)-visibleSize.height));
            pItem->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(1, Vec2(-visibleSize.width, visibleSize.height)),
                                              
                                              CallFunc::create([=](){
                pFlavor->setTouchEnabled(true);
                
            }), NULL));
        }else{
            pItem->setPosition(Vec2(100, 300+150*count));
            pItem->runAction(Sequence::create(DelayTime::create(0.2*i+1),
                                              CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("syrup_up.mp3");
                Vec2 pos = Vec2(100, visibleSize.height+200);
                pItem->setPosition(pItem->getParent()->convertToNodeSpace(pos));
                                                               }),
                                              EaseBackOut::create(MoveTo::create(1, Vec2(100, 200+150*(count-1-i)))),
                                              DelayTime::create(1),
                                              CallFunc::create([=](){
                Sprite* pLock = pItem->getLockSprite();
                if (pLock) {
                    pLock->setVisible(true);
                }
                pFlavor->setTouchEnabled(true);
                                                               }), NULL));
            
        }
    }
    m_pScrollView->getInnerContainer()->setContentSize(Size(200, 200+150*count));
    
}

void GetIcecreamScene::_showCone()
{
    m_pCone = Sprite::create(localPath("choose/wafer_cone.png"));
    this->addChildToContentLayer(m_pCone);
    CMVisibleRect::setPosition(m_pCone, 380, 260-visibleSize.height);
    m_pCone->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
    
    Sprite* pConeFront = Sprite::create(localPath("wafer_cone_on.png"));
    this->addChildToContentLayer(pConeFront);
    CMVisibleRect::setPosition(pConeFront, 380, 260-visibleSize.height);
    pConeFront->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
    pConeFront->setLocalZOrder(4);
}

