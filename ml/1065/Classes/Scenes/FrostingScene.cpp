
#include "FrostingScene.h"
#include "SceneManager.h"

FrostingScene::FrostingScene()
{
    m_nCurType = -1;
    m_nCurIndex = -1;
    m_pFrosting = nullptr;
    m_pScriblleNode = nullptr;
    m_pTempTexture = nullptr;
    m_pColorScrollview = nullptr;
}

FrostingScene::~FrostingScene()
{
    CC_SAFE_RELEASE(m_pTempTexture);
}
bool FrostingScene::init()
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
    
    m_pCakeDown = Sprite::create(localPath("cake1.png"));
    this->addChildToContentLayer(m_pCakeDown);
    CMVisibleRect::setPosition(m_pCakeDown, 480, 330+visibleSize.height);
    
    m_pCakeUp = Sprite::create(localPath("cake2.png"));
    this->addChildToContentLayer(m_pCakeUp);
    m_pCakeUp->setPosition(m_pCakeDown->getPosition());
    m_pCakeUp->setLocalZOrder(10);
    
    m_pKnife = _createDrageNode(localPath("knife0.png"));
    this->addChildToContentLayer(m_pKnife);
    CMVisibleRect::setPosition(m_pKnife, 850, 230+visibleSize.height);
    m_pKnife->setLocalZOrder(10);
    m_pKnife->setLimitRect(Rect(0, 150, visibleSize.width-100, visibleSize.height-400));
    
    m_pButterKnife = _createDrageNode(localPath("knife1.png"));
    this->addChildToContentLayer(m_pButterKnife);
    CMVisibleRect::setPosition(m_pButterKnife, 850, 230+visibleSize.height);
    m_pButterKnife->setLocalZOrder(10);
    m_pButterKnife->setActionPoint(Vec2(60, 260));
    m_pButterKnife->setLimitRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    
    m_pKnife->runAction(Sequence::create(DelayTime::create(2),
                                         EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                         CallFunc::create([=]()
                                                          {
                                                              m_pGuideLayer->showGuideArrow(Vec2(m_pCakeDown->getPositionX()+150, m_pCakeDown->getPositionY()-30), 0);
                                                              m_pGuideLayer2->showGuideTap(Vec2(m_pCakeDown->getPositionX()+30, m_pCakeDown->getPositionY()-70), 0);
                                                          }), NULL));
    
    m_pCakeDown->runAction(Sequence::create(DelayTime::create(1),
                                            MoveBy::create(0.5, Vec2(0, -visibleSize.height)), NULL));
    m_pCakeUp->runAction(Sequence::create(DelayTime::create(1),
                                          MoveBy::create(0.5, Vec2(0, -visibleSize.height)), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGuideLayer2 = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer2);
    
    return true;
}

#pragma mark - initData
void FrostingScene::_initData()
{
    setExPath("content/make/dec/");
    m_nNextSceneTag = GameUIEvent::eSceneTagAddIcing;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagAddIcing;
    m_nCurType = 0;
    m_nCurIndex = 0;
}

void FrostingScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagReset) {
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to reset the frosting?");
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

void FrostingScene::_reset()
{
//    SceneManager::replaceTheScene<FrostingScene>();
    
    if (m_pFrosting) {
        m_pFrosting->removeFromParent();
    }
    if (m_pScriblleNode) {
        m_pScriblleNode->removeFromParent();
    }
    m_pFrosting = nullptr;
    m_pScriblleNode = nullptr;
    
    m_pButterKnife->ignoreMoving = true;
    _showChooseFrosting();
}

void FrostingScene::_showIngrident(cocos2d::Node *pNode)
{
    pNode->setScale(0.6);
    bool isLeft = pNode->getPositionX()<visibleSize.width/2?true:false;
    ccBezierConfig cfg;
    cfg.controlPoint_1 = CMVisibleRect::getPosition(480-630*(isLeft?1:-1), 520);
    cfg.controlPoint_2 = CMVisibleRect::getPosition(480-550*(isLeft?1:-1), 340);
    cfg.endPosition = CMVisibleRect::getPosition(480, 200);
    pNode->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                      ScaleTo::create(0.3, 0.6,0.5),
                                      ScaleTo::create(0.1, 0.6),
                                      Spawn::create(BezierTo::create(1, cfg),
                                                    ScaleTo::create(1, 1), NULL),
                                      NULL));
}

void FrostingScene::_showFrosting(int type, int index)
{
    if (m_pScriblleNode) {
        m_pScriblleNode->removeFromParent();
    }
    m_pButterKnife->ignoreMoving = true;
    m_pScriblleNode = ScribbleNode::create(m_pCakeUp->getContentSize());
    this->addChildToContentLayer(m_pScriblleNode);
    m_pScriblleNode->setLocalZOrder(5);
    CMVisibleRect::setPosition(m_pScriblleNode, 480, 330);
    m_pScriblleNode->antiAliasing();
    m_pScriblleNode->useBrush(Scribble::BrushType::eBrush);
    std::stringstream ostrTarget;
    ostrTarget<<"frosting"<<m_nCurType<<"/"<<m_nCurIndex<<".png";
    
    std::stringstream ostrBrush;
    ostrBrush<<"frosting"<<m_nCurType<<"/"<<m_nCurIndex<<".png";
    
    m_pScriblleNode->useTarget(localPath(ostrTarget.str()));//指定将要绘制的对象（frosting）
    m_pScriblleNode->useBrush(localPath("point.png"), Scribble::eBrush);//指定用于绘制的画刷。
    Director::getInstance()->getRenderer()->render();
    
    std::stringstream ostrCream;
    ostrCream<<"cream0/"<<m_nCurIndex<<".png";
    if (!m_pFrosting) {
        m_pFrosting = Sprite::create(localPath(ostrCream.str()));
        this->addChildToContentLayer(m_pFrosting);
    }else{
        m_pFrosting->setTexture(localPath(ostrCream.str()));
    }
    m_pFrosting->stopAllActions();
    m_pFrosting->setVisible(true);
    m_pFrosting->setPosition(Vec2(visibleSize.width, visibleSize.height*1.5));
    m_pFrosting->setScale(0.5);
    m_pFrosting->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(1, Vec2(m_pCakeDown->getPositionX(), m_pCakeDown->getPositionY()+100))),
                                            Spawn::create(ScaleTo::create(0.3, 0.9,1.1),
                                                          MoveBy::create(0.3, Vec2(0, 50)), NULL),
                                            Spawn::create(ScaleTo::create(0.3, 1),
                                                          MoveBy::create(0.3, Vec2(0, -50)), NULL),
                                            CallFunc::create([=]()
                                                             {
                                                                 std::stringstream ostrCream2;
                                                                 ostrCream2<<"cream1/"<<m_nCurIndex<<".png";
                                                                 m_pFrosting->setTexture(localPath(ostrCream2.str()));
                                                             }),
                                            Spawn::create(ScaleTo::create(1, 1.1,1.2),
                                                          MoveBy::create(0.3, Vec2(0, -50)), NULL),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pButterKnife->ignoreMoving = false;
                                                             }),
                                            NULL));
    
    Sprite* pFrosting = Sprite::create(localPath(ostrTarget.str()));
    
    CC_SAFE_RELEASE(m_pTempTexture);
    m_pTempTexture = MyRenderTexture::create(pFrosting->getContentSize().width, pFrosting->getContentSize().height);
    m_pTempTexture->retain();
    pFrosting->setPosition(pFrosting->getContentSize()*0.5);
    m_pTempTexture->begin();
    pFrosting->visit();
    m_pTempTexture->end();
}

void FrostingScene::_showChooseFrosting()
{
    ChooseLayer* pLayer = ChooseLayer::create();
    pLayer->showChooseFrosing();
    this->addChildToUILayer(pLayer);
    pLayer->setLocalZOrder(6);
    pLayer->onItemCallback = CC_CALLBACK_1(FrostingScene::_onFrosingCallback, this);
}

void FrostingScene::_showColorScrollView()
{
    if (m_pColorScrollview) {
        return;
    }
    Vector<Node*> datas;
    int count = 15;
    for (int i = 0; i<count; i++) {
        ostringstream ostr;
        ostr<<"color/color"<<i<<".png";
//        Sprite* pSprite = Sprite::create(localPath(ostr.str()));
//        pSprite->setTag(i);
//        datas.pushBack(pSprite);
        
        LockItem* pItem = LockItem::create();
        if (!IAPManager::getInstance()->getItemIsBought(0) && i>6 && !gNoneIap) {
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
//    m_pColorScrollview->setDirectClip(true);
    log("==========%f,%f",m_pColorScrollview->getPositionX(),m_pColorScrollview->getPositionY());
    log("=========%f,%f",m_pColorScrollview->getContentSize().width,m_pColorScrollview->getContentSize().height);
    m_pColorScrollview->setEnableAdjust(true, m_pColorScrollview->getContentSize().height/2);
    m_pColorScrollview->setTouchEnable(true);
    m_pColorScrollview->setAdjustCallBack(CC_CALLBACK_1(FrostingScene::_onColorItemCallback, this));
    m_pColorScrollview->setSpace(10);
    this->addChildToUILayer(m_pColorScrollview);
    auto _adapter = CircularAdapter::create(m_pColorScrollview);
    _adapter->setDatas(datas);
    m_pColorScrollview->setAdapter(_adapter);
    m_pColorScrollview->scrollToFirst(0.5);

}

void FrostingScene::_onFrosingCallback(int index)
{
    m_nCurType = index;
    _showColorScrollView();
    GameDataManager::getInstance()->m_nFrostingType = m_nCurType;
    m_pGameUI->showResetLayout();
    _showFrosting(m_nCurType, m_nCurIndex);
    GameDataManager::getInstance()->m_nFrostingIndex = m_nCurIndex;
}
void FrostingScene::_onColorItemCallback(Node* pItem)
{
    if (!pItem) {
        return;
    }
    int index = pItem->getTag();
    if (index==m_nCurIndex) {
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
    
    m_nCurIndex = index;
    m_pGameUI->hideNext();
    _showFrosting(m_nCurType, m_nCurIndex);
    GameDataManager::getInstance()->m_nFrostingIndex = m_nCurIndex;
}

void FrostingScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(0)) {
        Vector<Node*> vec = m_pColorScrollview->getAllItems();
        for_each(vec.begin(), vec.end(), [=](Node* pNode){
            LockItem* pItem = dynamic_cast<LockItem*>(pNode);
            if (pItem) {
                pItem->unlock();
            }
        });
    }
}

void FrostingScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pKnife) {
        pDragNode->setLocalZOrder(5);
        m_pGuideLayer->removeGuide();
        m_pGuideLayer2->removeGuide();
    }
}
void FrostingScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pKnife) {
        Rect rect = m_pCakeUp->getBoundingBox();
        rect.origin = m_pCakeUp->getParent()->convertToWorldSpace(rect.origin);
        if (rect.origin.x+100>worldPoint.x) {
            m_pGuideLayer->removeGuide();
            m_pGuideLayer2->removeGuide();
            pDragNode->ignoreMoving = true;
            m_pCakeUp->runAction(Sequence::create(JumpBy::create(0.5, Vec2(50, 50), 100, 1),
                                                  MoveBy::create(0.5, Vec2(visibleSize.width, 300)), NULL));
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 100)));
            
            m_pButterKnife->setTouchEnabled(false);
            m_pButterKnife->runAction(Sequence::create(DelayTime::create(1),
                                                       MoveBy::create(0.5, Vec2(0, -visibleSize.height)),
                                                       CallFunc::create([=]()
                                                                        {
                                                                            m_pButterKnife->setTouchEnabled(true);
                                                                        }), NULL));
            _showChooseFrosting();
        }
    }else if(pDragNode==m_pButterKnife){
        if (m_nCurIndex==-1&&m_nCurType==-1) {
            return;
        }
        Rect rect = m_pCakeDown->getBoundingBox();
        rect.origin = m_pCakeDown->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
        
            if (m_pFrosting) {
                if (m_pFrosting->getScaleX()>0.5) {
                    m_pFrosting->setScaleX(m_pFrosting->getScaleX()-0.001);
                    m_pFrosting->setScaleY(m_pFrosting->getScaleY()-0.001);
                }
            }
            if (m_pScriblleNode) {
                m_pScriblleNode->paint(worldPoint);//将frosting制出来
            }

        }
    }

}
void FrostingScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if(pDragNode==m_pButterKnife){
        if (!m_pScriblleNode) {
            return;
        }
        int percentCur = 100-m_pScriblleNode->getCanvasPercent();
        int percentAll = 100-m_pTempTexture->getPercentageTransparent();
        int percent = percentCur*100/percentAll;
        
        log("======%d",percent);
        if (percent>=95) {
            std::stringstream ostrTarget;
            ostrTarget<<"frosting"<<m_nCurType<<"/"<<m_nCurIndex<<".png";
//
            Sprite* p = Sprite::create(localPath(ostrTarget.str()));
            p->setPosition(p->getContentSize()*0.5);
            m_pScriblleNode->paint(p);
            m_pFrosting->setVisible(false);
            m_pGameUI->showNextLayout();
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setPosition(visibleSize*0.5);
        }
    }
}

void FrostingScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}