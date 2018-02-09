
#include "AddCandleScene.h"
#include "PhysicsShapeCache.h"
#include "SceneManager.h"

AddCandleScene::AddCandleScene()
{
    m_pItemScrollView = nullptr;
    m_pCurCandle = nullptr;
    m_bGuideShown = false;
}

AddCandleScene::~AddCandleScene()
{
    
}
bool AddCandleScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create("content/make/dec/dec_bk.jpg");
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    float delt = 100;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_pDecoratedCake = Sprite::create(FileUtility::getStoragePath()+"decorate.png");
#else
    m_pDecoratedCake = Sprite::create(FileUtility::getStoragePath()+"/decorate.png");
#endif
    if (m_pDecoratedCake) {
        this->addChildToContentLayer(m_pDecoratedCake);
        m_pDecoratedCake->setPosition(visibleSize.width/2,visibleSize.height/2);
    }
//    m_pDecoratedCake->setOpacity(100);
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(140,475));
    this->addChildToUILayer(m_pTypeScrollView);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView, 10-visibleSize.width/2, (visibleSize.height-m_pTypeScrollView->getContentSize().height)/2,kBorderLeft,kBorderBottom);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(AddCandleScene::onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/category/icon/";
    m_pTypeScrollView->setLocalZOrder(10);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionV);
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                  EaseBackIn::create(MoveBy::create(1, Vec2(visibleSize.width/2, 0))), NULL));
    m_pTypeScrollView->setSingleAsset(false);
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("candle"));
    
    m_nSegment = 30;
    memset(m_flag, 0, sizeof(m_flag));
    Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(320+delt, 260+20));
    
    m_CandlePos.init(5, 5, 65, 48, pos);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(5);
    
    _showGuideCandlePos();
    
//    m_pGameUI->showBackLayout();
    m_pGameUI->showResetLayout();
    return true;
}

#pragma mark - initData
void AddCandleScene::_initData()
{
    setExPath("content/category/candle/");
    m_nNextSceneTag = GameUIEvent::eSceneTagChooseBg;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagChooseBg;
    m_nType = -1;
}

void AddCandleScene::onEnter()
{
    ExtensionScene::onEnter();
    if (m_nType!=-1) {
        _showItemScrollView(false);
    }
}

void AddCandleScene::onExit()
{
    ExtensionScene::onExit();
    if (m_nType!=-1) {
        _hideItemScrollView();
    }
}
void AddCandleScene::update(float)
{
    m_nPressTime++;
    log("=======%d",m_nPressTime);
}

void AddCandleScene::_reset()
{
    for_each(m_VectorCandle.begin(), m_VectorCandle.end(), [=](Sprite* pCandle){
        pCandle->removeFromParent();
    });
    m_VectorCandle.clear();
    m_candleIndex.clear();
    m_pGameUI->hideNext();
    
    memset(m_flag, 0, sizeof(m_flag));
    for_each(m_GuideCandleVector.begin(), m_GuideCandleVector.end(), [=](Sprite* pSprite){
        pSprite->stopAllActions();
        pSprite->setVisible(true);
        pSprite->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=]()
                                                                                   {
                                                                                       pSprite->setVisible(true);
                                                                                   }),
                                                                  FadeTo::create(0.3, 180),
                                                                  DelayTime::create(0.1),
                                                                  FadeTo::create(0.3, 255),
                                                                  DelayTime::create(0.1),
                                                                  FadeTo::create(0.3, 180),
                                                                  DelayTime::create(0.1),
                                                                  FadeTo::create(0.3, 255),
                                                                  DelayTime::create(0.5),
                                                                  CallFunc::create([=]()
                                                                                   {
                                                                                       pSprite->setVisible(false);
                                                                                   }),
                                                                  DelayTime::create(3), NULL)));
    });
}

void AddCandleScene::_showGuideCandlePos()
{
    for (int i = 0; i<29; i++) {
        Sprite* pSprite = Sprite::create("content/make/dec/candle_site.png");
        this->addChildToUILayer(pSprite);
        Vec2 pos = m_CandlePos.getPosition(i);
        pos = m_pUILayer->convertToNodeSpace(pos);
        pSprite->setPosition(pos);
        pSprite->setTag(i);
        m_GuideCandleVector.pushBack(pSprite);
        pSprite->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=]()
                                                                                   {
                                                                                       pSprite->setVisible(true);
                                                                                   }),
                                                                  FadeTo::create(0.3, 180),
                                                                  DelayTime::create(0.1),
                                                                  FadeTo::create(0.3, 255),
                                                                  DelayTime::create(0.1),
                                                                  FadeTo::create(0.3, 180),
                                                                  DelayTime::create(0.1),
                                                                  FadeTo::create(0.3, 255),
                                                                  DelayTime::create(0.5),
                                                                  CallFunc::create([=]()
                                                                                   {
                                                                                       pSprite->setVisible(false);
                                                                                   }),
                                                                  DelayTime::create(3), NULL)));
    }
}
void AddCandleScene::_showItemScrollView(bool nooffset)
{
    if (m_pItemScrollView) {
        m_pItemScrollView->getInnerContainer()->removeAllChildren();
    }else{
        Sprite* pBox = Sprite::create("content/category/icon/candle_box.png");
        this->addChildToUILayer(pBox);
        pBox->setAnchorPoint(Vec2::ZERO);
        CMVisibleRect::setPositionAdapted(pBox, 180, 100-300,kBorderLeft,kBorderBottom);
        pBox->setName("box");
        
        m_pItemScrollView = ExpandScrollView::create();
        m_pItemScrollView->setColor(Color3B::GRAY);
        m_pItemScrollView->setContentSize(Size(pBox->getContentSize().width-10,pBox->getContentSize().height));
        CMVisibleRect::setPosition(m_pItemScrollView, 185, 100,kBorderLeft);
        this->addChildToUILayer(m_pItemScrollView);
        m_pItemScrollView->setScrollBarEnabled(false);
    }
    if (nooffset) {
        m_pItemScrollView->getInnerContainer()->setPosition(Vec2::ZERO);
    }
    m_pItemScrollView->stopAllActions();
//    CMVisibleRect::setPositionAdapted(m_pItemScrollView, 185, 100-300,kBorderLeft,kBorderBottom);
//    m_pItemScrollView->runAction(MoveBy::create(0.5, Vec2(0, 300)));
    
    CMVisibleRect::setPositionAdapted(m_pItemScrollView, 40, 80,kBorderLeft);
    m_pItemScrollView->setScale(0.01,1);
    m_pItemScrollView->runAction(Spawn::create(MoveBy::create(0.5, Vec2(80, 0)),
                                                     ScaleTo::create(0.5, 1), NULL));
    
    Node* pBox = m_pUILayer->getChildByName("box");
    if (pBox) {
        pBox->stopAllActions();
//        CMVisibleRect::setPositionAdapted(pBox, 180, 100-300,kBorderLeft,kBorderBottom);
//        pBox->runAction(MoveBy::create(0.5, Vec2(0, 300)));
        CMVisibleRect::setPositionAdapted(pBox, 40, 80,kBorderLeft);
        pBox->setScale(0.01,1);
        pBox->runAction(Spawn::create(MoveBy::create(0.5, Vec2(80, 0)),
                                                   ScaleTo::create(0.5, 1), NULL));
    }
    
    stringstream ostrName;
    ostrName<<"candle"<<m_nType;
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(ostrName.str());
    for (int i = 0; i<data.totalCount; i++) {
        stringstream ostr;
        ostr<<"content/category/icon_candle/"<<m_nType<<"_"<<i<<".png";
        DragNode* pNode = _createDrageNode(ostr.str());
        pNode->setPosition(Vec2(92+i*100, 20));
        
        pNode->index = m_nType;
        pNode->getDragSprite()->setAnchorPoint(Vec2(0.5, 0));
        pNode->setActionPoint(Vec2(pNode->getDragSprite()->getContentSize().width/2, 0));
        pNode->setTag(i);
        pNode->setOrgPositionDefault();
        pNode->setScale(0.7);
        
        if (data.totalCount>6) {
            pNode->setCanSwallow(false);
        }
        m_pItemScrollView->getInnerContainer()->addChild(pNode);
        if (i>=data.freeCount && !IAPManager::getInstance()->getItemIsBought(1) && !gNoneIap) {
//            加锁
            Sprite* pLock = Sprite::create("content/common/lock.png");
            pNode->addChild(pLock);
            pLock->setPosition(Vec2(30, 30));
            pLock->setName("lock");
        }
    }
    m_pItemScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    if (data.totalCount>6){
        m_pItemScrollView->setInnerContainerSize(Size(92+data.totalCount*100,60));
    }else{
        m_pItemScrollView->setInnerContainerSize(m_pItemScrollView->getContentSize());
    }
}

void AddCandleScene::_hideItemScrollView()
{
    if (m_pItemScrollView) {
        m_pItemScrollView->stopAllActions();
//        CMVisibleRect::setPositionAdapted(m_pItemScrollView, 185, 100,kBorderLeft,kBorderBottom);
//        m_pItemScrollView->runAction(MoveBy::create(0.5, Vec2(0, -300)));
        m_pItemScrollView->runAction(Spawn::create(MoveBy::create(0.5, Vec2(-80, 0)),
                                                         ScaleTo::create(0.5,0, 1), NULL));
    }
    
    Node* pBox = m_pUILayer->getChildByName("box");
    if (pBox) {
        pBox->stopAllActions();
//        CMVisibleRect::setPositionAdapted(pBox, 180, 100,kBorderLeft,kBorderBottom);
//        pBox->runAction(MoveBy::create(0.5, Vec2(0, -300)));
        pBox->runAction(Spawn::create(MoveBy::create(0.5, Vec2(-80, 0)),
                                                   ScaleTo::create(0.5,0, 1), NULL));
    }
}
void AddCandleScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagNext) {
        GameDataManager::getInstance()->m_VectorCandle.operator=(m_candleIndex);
    }else if (tag==GameUILayoutLayer::eUIButtonTagReset) {
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to reset the candles?");
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


void AddCandleScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(1)) {
        Vector<Node*> vec = m_pItemScrollView->getInnerContainer()->getChildren();
        for_each(vec.begin(), vec.end(), [=](Node* pNode){
            DragNode* pItem = dynamic_cast<DragNode*>(pNode);
            if (pItem) {
                Node* pLock = pItem->getChildByName("lock");
                if (pLock) {
                    pLock->removeFromParent();
                }
            }
        });
    }
}

void AddCandleScene::onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
{
    if (selected) {
        m_nType = index;
        _showItemScrollView();
        if (!m_bGuideShown) {
            m_bGuideShown = true;
            m_pGuideLayer->showGuideMove(CMVisibleRect::getPosition(460, 150,kBorderNone,kBorderBottom), visibleSize*0.5,1);
        }
    }else{
        _hideItemScrollView();
    }
}
void AddCandleScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(50);
    m_nPressTime = 0;
    scheduleUpdate();
    
    m_pGuideLayer->removeGuide();
}
void AddCandleScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    unscheduleUpdate();
    if (m_pTool==pDragNode) {
        Vec2 po = m_pContentLayer->convertToNodeSpace(worldPoint);
        
    }
    if (m_pCurCandle) {
        Vec2 pos = pDragNode->getParent()->convertToWorldSpace(pDragNode->getPosition());
        m_pCurCandle->setPosition(m_pCurCandle->getParent()->convertToNodeSpace(pos));
    }
    
    if (m_nPressTime>0 ) {
        if (!m_pCurCandle) {
            m_pItemScrollView->setTouchEnabled(false);
            pDragNode->setVisible(false);
            m_pCurCandle = Sprite::createWithTexture(pDragNode->getDragSprite()->getTexture());
            this->addChildToUILayer(m_pCurCandle);
            m_pCurCandle->setLocalZOrder(10);
            m_pCurCandle->setAnchorPoint(Vec2(0.5, 0));
            Vec2 pos = pDragNode->getParent()->convertToWorldSpace(pDragNode->getPosition());
            m_pCurCandle->setPosition(m_pCurCandle->getParent()->convertToNodeSpace(pos));
        }
    }else{
        pDragNode->setTouchEnabled(false);
    }
    
}
void AddCandleScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    unscheduleUpdate();
    stringstream ostrName;
    ostrName<<"candle"<<m_nType;
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(ostrName.str());
    if (pDragNode->getTag()>=data.freeCount && !IAPManager::getInstance()->getItemIsBought(1) && !gNoneIap)  {
        pDragNode->setTouchEnabled(false);
        if (m_pCurCandle){
            m_pCurCandle->removeFromParent();
            m_pCurCandle = nullptr;
        }
        pDragNode->setPosition(pDragNode->getOrgPosition());
        pDragNode->setVisible(true);
        pDragNode->setTouchEnabled(true);
        
        m_pItemScrollView->setTouchEnabled(true);
        if (!m_pItemScrollView->isScrolled()) {
//            SceneManager::pushTheScene<ShopScene>();
            
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss();
        }
        return;
    }
    CandlePosition::CandlePos candlePos = m_CandlePos.containPosition(worldPoint);
    int index = candlePos.index;
    if(index!=-1 && !m_flag[index]){
        m_flag[index] = true;
        pDragNode->setTouchEnabled(false);
        pDragNode->setVisible(false);
        int type = pDragNode->index;
        int tag = pDragNode->getTag();
        Vec2 pos = candlePos.pos;
        pos = m_pContentLayer->convertToNodeSpace(pos);
        pDragNode->setLocalZOrder(50-index);
        m_candleIndex.push_back(Vec3(index, type, tag));
        
        Sprite* pSpritePos = m_GuideCandleVector.at(index);
        if (pSpritePos) {
            pSpritePos->stopAllActions();
            pSpritePos->setVisible(false);
        }
        
        if (m_pCurCandle){
            m_pCurCandle->removeFromParent();
            m_pCurCandle = nullptr;
        }
        stringstream ostr;
        ostr<<gCandleName[type]<<"/"<<tag<<"_0.png";
        Sprite* pCandle = Sprite::create(localPath(ostr.str()));
        this->addChildToContentLayer(pCandle);
        pCandle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getParent()->convertToWorldSpace(pDragNode->getPosition())));
        pCandle->setAnchorPoint(Vec2(0.5, 0));
        pCandle->setLocalZOrder(50-index);
        
        pCandle->runAction(Sequence::create(MoveTo::create(0.3, Vec2(pos.x, pos.y-CandleData::getCandleData(pDragNode->index).holderHeight)),
                                              JumpBy::create(0.5, Vec2(0,10), 20, 1),
                                              CallFunc::create([=]()
                                                               {
                                                                   stringstream ostr;
                                                                   ostr<<gCandleName[type]<<"/"<<tag<<"_1.png";
                                                                   pCandle->setTexture(localPath(ostr.str()));
                                                               }),
                                            MoveBy::create(0.1, Vec2(0, -10)), NULL));
        m_VectorCandle.pushBack(pCandle);
        if (m_VectorCandle.size()==1) {
            m_pGameUI->showNextLayout();
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setPosition(visibleSize*0.5);
        }
        pDragNode->setPosition(pDragNode->getOrgPosition());
        pDragNode->setVisible(true);
        pDragNode->setTouchEnabled(true);
        
    }else{
        bool isFull = true;
        for (int i = 0; i<29; i++) {
            if (!m_flag[i]) {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
            dialog->setContentText("Your cake has been full of candles now!");
            Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
            dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        }
        
        if (m_pCurCandle){
            m_pCurCandle->removeFromParent();
            m_pCurCandle = nullptr;
        }
        pDragNode->setPosition(pDragNode->getOrgPosition());
        pDragNode->setVisible(true);
        pDragNode->setTouchEnabled(true);
    }
    
    m_pItemScrollView->setTouchEnabled(true);
}
void AddCandleScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}