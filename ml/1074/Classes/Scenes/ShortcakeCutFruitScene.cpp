
#include "ShortcakeCutFruitScene.h"
#include "SceneManager.h"
#include "PolygonHelp.h"

//static std::string gsFruitConfig[] = {
//    "",
//    "",
//    "",
//    "",
//};
static int gnFruitCutCount[] = {
    1,1,5,5,1,5,5,5,4,5,4,1
};
ShortcakeCutFruitScene::ShortcakeCutFruitScene()
{
    m_bItemEnable = false;
    m_nMoveCount = 0;
    m_nFlavorIndex = -1;
    m_nCutCount = 0;
    m_bMogoFlag = false;
    m_pKnife = nullptr;
}

ShortcakeCutFruitScene::~ShortcakeCutFruitScene()
{
    
}
bool ShortcakeCutFruitScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("common/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showBoard();
    
    _showFruitScrollView();
    
    m_pGameUI->showNormalLayout();
    return true;
}

void ShortcakeCutFruitScene::onEnter()
{
    ExtensionScene::onEnter();
    Scene* scene = this->getScene();
    
//        scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
        scene->getPhysicsWorld()->setAutoStep(false);
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(ShortcakeCutFruitScene::updateStart), 2);
}

void ShortcakeCutFruitScene::updateStart(float delta)
{
    Scene* scene = this->getScene();
    scene->getPhysicsWorld()->setAutoStep(false);
    scheduleUpdate();
}
void ShortcakeCutFruitScene::update(float delta)

{
    
    // use fixed time and calculate 3 times per frame makes physics simulate more precisely.
    
    //这里表示先走3步瞧瞧  如果fps是1/60  三个setp就是1/180
    
    for (int i = 0; i < 3; ++i)
        
    {
        log("=======i %d");
        Scene* scene = this->getScene();
        scene->getPhysicsWorld()->step(1/180.0f);
        
    }
    
}
void ShortcakeCutFruitScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ShortcakeCutFruitScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "knife"){
        if (pDragNode->index == 0) {
            pDragNode->index = 1;
            
            m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)),
                                                      CallFunc::create([=](){
                m_pScrollView->removeFromParent();
                m_pScrollView = nullptr;
            }), NULL));
            m_pBanner->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)),
                                                  CallFunc::create([=](){
                m_pBanner->removeFromParent();
                m_pBanner = nullptr;
            }), NULL));
        }
    }
}

void ShortcakeCutFruitScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "knife"){
        if (pDragNode->index == 1) {
            for (auto pFruit:m_Fruits) {
                Rect rect = pFruit->getBoundingBox();
                if (!rect.containsPoint(worldPoint)) {
                    continue;
                }
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
                int cutCount = gnFruitCutCount[m_nFlavorIndex];
                AudioHelp::getInstance()->playEffect("cut_food.mp3");
                if (m_nCutCount == 2) {
                    AudioHelp::getInstance()->playEffect("vo_careful_fingers.mp3");
                }
                if (m_nFlavorIndex == 10) {
                    //                    火龙果 单独处理
                    Vec2 pos = Vec2(0-50*m_nCutCount, -150);
                    pos = pFruit->convertToWorldSpace(pos);
                    float rotate = -20;
                    Vec2 moveDelt = Vec2(260, -160);
                    if(m_nCutCount<2) {
                        rotate = 80;
                        moveDelt = Vec2(-260, -160);
                        pos = Vec2(100, 110-50*m_nCutCount);
                    }
                    pos = pFruit->convertToWorldSpace(pos);
                    pDragNode->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, pos),
                                                                        RotateTo::create(0.5, rotate),NULL),
                                                          MoveBy::create(0.5, moveDelt),
                                                          CallFunc::create([=](){
                        m_nCutCount++;
                        pFruit->setTexture(localPath("fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(m_nCutCount)+".png"));
                        if (m_nCutCount == cutCount) {
                            pFruit->runAction(Sequence::create(DelayTime::create(0.5),
                                                               Spawn::create(JumpBy::create(0.5, Vec2::ZERO, 50, 1),
                                                                             RotateBy::create(0.5, Vec3(-70,0,0)), NULL),
                                                               CallFunc::create([=](){
                                Sprite* pRight = Sprite::create(localPath("fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(cutCount+1)+".png"));
                                this->addChildToContentLayer(pRight);
                                pRight->setPosition(pFruit->getPosition());
                                m_FruitsBoard.pushBack(pRight);
                            }),
                                                               Hide::create(),
                                                               CallFunc::create([=](){
                                _finishCut();
                            }), NULL));
                        }else{
                            pDragNode->setTouchEnabled(true);
                            float rotate = -20;
                            Vec2 pos = Vec2(-50*(m_nCutCount-2), 0);
                            if(m_nCutCount<2) {
                                rotate = 60;
                                pos = Vec2(0, 0);
                            }
                            m_pGuideLayer->showGuide("content/common/tip/cut_line.png",  pFruit->getPosition()+pos,rotate);
                        }
                    }),
                                                          NULL));
                }else if(m_nFlavorIndex == 8){
//                    芒果 单独处理
                    int tag = pFruit->getTag();
                    if (tag==1 && m_bMogoFlag) {
                        continue;
                    }
                    Vec2 pos = pFruit->getPosition()+Vec2(50, 0);
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                          MoveBy::create(0.5, Vec2(10, -40)),
                                                          CallFunc::create([=](){
                        
                        if (m_nCutCount == 0) {
                            pFruit->setTexture(localPath("fruit9_1.png"));
                            pFruit->setPosition(pFruit->getPosition()+Vec2(-100, 0));
                            Sprite* pFruitRight = Sprite::create(localPath("fruit9_3.png"));
                            this->addChildToContentLayer(pFruitRight);
                            pFruitRight->setPosition(pFruit->getPosition()+Vec2(200, 0));
                            m_Fruits.pushBack(pFruitRight);
                            pFruitRight->setTag(1);
                            m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition());
                        }else {
                            if (tag==1) {
                                m_bMogoFlag = true;
                                pFruit->setTexture(localPath("fruit9_4.png"));
                                m_FruitsBoard.pushBack(pFruit);
                                m_Fruits.eraseObject(pFruit);
                                if (m_Fruits.size()>0) {
                                    m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition()+Vec2(-200, 0));
                                }
                            }else{
                                if (m_bMogoFlag) {
                                    if (m_nCutCount == 2){
                                        Sprite* pFruitMiddle = Sprite::create(localPath("fruit9_2.png"));
                                        this->addChildToContentLayer(pFruitMiddle);
                                        pFruitMiddle->setPosition(pFruit->getPosition()+Vec2(50, 0));
                                        pFruitMiddle->runAction(JumpBy::create(0.5, Vec2(visibleSize.width, 0), 200, 1));
                                        pFruit->setTexture(localPath("fruit9_3.png"));
                                        m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition());
                                    }else{
                                        pFruit->setTexture(localPath("fruit9_4.png"));
                                        m_FruitsBoard.pushBack(pFruit);
                                    }
                                }else{
                                    if (m_nCutCount == 1){
                                        Sprite* pFruitMiddle = Sprite::create(localPath("fruit9_2.png"));
                                        this->addChildToContentLayer(pFruitMiddle);
                                        pFruitMiddle->setPosition(pFruit->getPosition()+Vec2(50, 0));
                                        pFruitMiddle->runAction(JumpBy::create(0.5, Vec2(visibleSize.width, 0), 200, 1));
                                        pFruit->setTexture(localPath("fruit9_3.png"));
                                        m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition());
                                    }else{
                                        pFruit->setTexture(localPath("fruit9_4.png"));
                                        m_FruitsBoard.pushBack(pFruit);
                                        m_Fruits.eraseObject(pFruit);
                                        if (m_Fruits.size()>0) {
                                            m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition()+Vec2(200, 0));
                                        }
                                    }
                                    
                                }
                            }
                        }
                        m_nCutCount++;
                        
                        if (m_nCutCount == cutCount){
                            _finishCut();
                        }else{
                            pDragNode->setTouchEnabled(true);
                        }
                    }), NULL));
                    break;
                }else{
                    Vec2 pos = Vec2(pFruit->getContentSize().width*(1-m_nCutCount*0.01)*(cutCount-m_nCutCount)/(1.5+cutCount)+30, pFruit->getContentSize().height/2);
                    pos = pFruit->convertToWorldSpace(pos);
                    if (cutCount == 1){
                        pos = pFruit->getPosition()+Vec2(50, -40);
                    }
//                    pDragNode->setPosition(pos);
                    pDragNode->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, pos),
                                                                        CallFunc::create([=](){
                        if (m_nFlavorIndex != 10) {
                            
                        }
                        if (cutCount != 1){
                            pDragNode->runAction(RotateBy::create(0.5, 3*m_nCutCount));
                        }
                        
                    }), NULL),
                                                          MoveBy::create(0.5, Vec2(10, -40)),
                                                          CallFunc::create([=](){
                        m_nCutCount++;
                        if (cutCount == 1) {
                            pFruit->setTexture(localPath("fruit"+std::to_string(m_nFlavorIndex+1)+"_1.png"));
                            m_FruitsBoard.pushBack(pFruit);
                            m_Fruits.eraseObject(pFruit);
                            if (m_Fruits.size()==0) {
                                _finishCut();
                            }else{
                                pDragNode->setTouchEnabled(true);
//                                Vec2 pos = Vec2(pFruit->getContentSize().width*(cutCount-m_nCutCount+1)/(cutCount+1), pFruit->getContentSize().height/2);
//                                pos = pFruit->convertToWorldSpace(pos);
                                Vec2 pos = CMVisibleRect::getPosition(320+80*2-m_nCutCount*160,400);
                                
                                m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pos,3*m_nCutCount);
                            }
                        }else{
//                            if (cutCount != 11){
                                Sprite* pRight = Sprite::create(localPath("fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(cutCount)+".png"));
                                this->addChildToContentLayer(pRight);
                                pRight->setPosition(pDragNode->getPosition());
                                m_FruitsBoard.pushBack(pRight);
//                            }
                            if (m_nCutCount == cutCount) {
                                if (m_nFlavorIndex == 10) { //西瓜
                                    pFruit->setTexture(localPath("fruit"+std::to_string(m_nFlavorIndex+1)+".png"));
                                    pFruit->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
                                }else{
                                    pFruit->setVisible(false);
                                }
                                Sprite* pLeft = Sprite::create(localPath("fruit"+std::to_string(m_nFlavorIndex+1)+"_5.png"));
                                this->addChildToContentLayer(pLeft);
                                pLeft->setPosition(pDragNode->getPosition()+Vec2(-20, 0));
                                m_FruitsBoard.pushBack(pLeft);
                                _finishCut();
                            }else{
                                pFruit->setTexture(localPath("fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(m_nCutCount)+".png"));
                                pDragNode->setTouchEnabled(true);
                                Vec2 pos = Vec2(pFruit->getContentSize().width*(1-m_nCutCount*0.01)*(cutCount-m_nCutCount)/(cutCount+2), pFruit->getContentSize().height/2);
                                pos = pFruit->convertToWorldSpace(pos);
                                if (cutCount == 1){
                                    pos = pFruit->getPosition()+Vec2(50, -40);
                                }
                                m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pos,3*m_nCutCount);
                            }
                        }
                    }), NULL));
                }
            }
        }
    }
}

void ShortcakeCutFruitScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void ShortcakeCutFruitScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void ShortcakeCutFruitScene::_initData()
{
    setExPath("content/make/shortcake/make3/");
}

void ShortcakeCutFruitScene::_showBoard()
{
    m_pBoard = Sprite::create(localPath("board.png"));
    this->addChildToContentLayer(m_pBoard);
    CMVisibleRect::setPositionAdapted(m_pBoard, 320-visibleSize.width, 380);
    m_pBoard->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
}

void ShortcakeCutFruitScene::_showKnife()
{
    if(m_pKnife){
        return;
    }
    m_pKnife = _createDrageNode(localPath("knife.png"));
    this->addChildToContentLayer(m_pKnife);
    CMVisibleRect::setPositionAdapted(m_pKnife, 600-visibleSize.width, 400);
    m_pKnife->setName("knife");
    m_pKnife->setTouchEnabled(false);
    m_pKnife->setActionPoint(Vec2(25, 260));
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            //            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pKnife->runAction(action);
    
}
void ShortcakeCutFruitScene::_showFruitScrollView()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width,200));
    this->addChildToUILayer(m_pScrollView);
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 120-600,kBorderLeft,kBorderBottom);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setLocalZOrder(5);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setClippingEnabled(false);
    
    int typeCount = 12;
    m_pScrollView->getInnerContainer()->setContentSize(Size((typeCount+1)*180, 150));
    
    m_pBanner = Sprite::create("content/category/icon/fruit_banner.png");
    this->addChildToUILayer(m_pBanner);
    m_pBanner->setPosition(m_pScrollView->getPosition()+m_pBanner->getAnchorPointInPoints()+Vec2(0, -40));
    
    for (int i = 0; i<typeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool bLock = false;
        if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i%2==1){
            if (!ConfigManager::getInstance()->getVideoUnLocked("shortcake_fruit", i)) {
                LockUIManager::getInstance()->registerLock("shortcake_fruit", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                pItem->lockType = 1;
                bLock = true;
            }
        }
        pItem->initWithName(localPath("icon_fruit"+std::to_string(i+1)+".png"), "content/common/video.png",bLock);
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setTag(TAG_REWARD_LOCK);
//            Lock->setPosition(Vec2(50, -50));
        }
        
        pItem->setTag(i);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(100+i*180,90));
        
        pItem->onItemClicked = CC_CALLBACK_1(ShortcakeCutFruitScene::_onFruitTypeClicked, this);
        m_typeItems.pushBack(pItem);
    }
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, 600)),
                                              CallFunc::create([=](){
        m_bItemEnable = true;
        AudioHelp::getInstance()->playEffect("vo_make_fruit_shortcake.mp3");
    }), NULL));
    m_pBanner->runAction(MoveBy::create(1, Vec2(0, 600)));
    AudioHelp::getInstance()->playEffect("show_bar.mp3");
}


void ShortcakeCutFruitScene::showFruit(int index)
{
    m_pGuideLayer->removeGuide();
    int cutCount = gnFruitCutCount[m_nFlavorIndex];
    m_nFlavorIndex = index;
    int fruitCount = 1;
    if (cutCount==1) {
        fruitCount = 3;
    }
    if(m_pKnife){
        m_pKnife->setTouchEnabled(false);
    }
    _showKnife();
    for (Sprite* pItem:m_Fruits) {
        
        pItem->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
            pItem->removeFromParent();
        }), NULL));
    }
    float dt = 1;
    if (m_Fruits.size() == 0){
        dt = 0.1;
    }
    m_Fruits.clear();
    for (int i = 0; i<fruitCount; i++) {
        Sprite* pFruit = Sprite::create(localPath("fruit"+std::to_string(index+1)+"_0.png"));
        this->addChildToContentLayer(pFruit);
//        if(fruitCount>1){
            CMVisibleRect::setPositionAdapted(pFruit, 320+80*(fruitCount-1)-i*160+visibleSize.width, 400);
//        }else{
//            CMVisibleRect::setPositionAdapted(pFruit, 320+visibleSize.width, 400);
//        }
        m_Fruits.pushBack(pFruit);
        pFruit->setTag(i);
        
        pFruit->runAction(Sequence::create(DelayTime::create(dt),
                                           MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           CallFunc::create([=](){
            m_bItemEnable = true;
            m_pKnife->setTouchEnabled(true);
            if(cutCount==1 && i == 0){
                m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition());
            }else if(cutCount!=1){
                if (m_nFlavorIndex == 10) {
                    m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition()+Vec2(-50, 50), 40);
                }else if(m_nFlavorIndex == 8){
                    m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pFruit->getPosition());
                }else{
                    Vec2 pos = Vec2(pFruit->getContentSize().width*(cutCount)/(cutCount+1), pFruit->getContentSize().height/2);
                    pos = pFruit->convertToWorldSpace(pos);
                    m_pGuideLayer->showGuide("content/common/tip/cut_line.png", pos);
                }
            }
        }), NULL));
    }
    AudioHelp::getInstance()->playEffect("present_in.mp3");
}

void ShortcakeCutFruitScene::_showBowl()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pBowl = Sprite::create(localPath("fruit_bowl0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 300-visibleSize.width, 380);
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBowlFront = Sprite::create(localPath("fruit_bowl1.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBatterClipping = ClippingNode::create(Sprite::create(localPath("mask.png")));
    m_pBowl->addChild(m_pBatterClipping);
    m_pBatterClipping->setAlphaThreshold(0.5);
    m_pBatterClipping->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
}

void ShortcakeCutFruitScene::_onFruitTypeClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    int tag = item->getTag();
    if (m_nFlavorIndex==tag) {
        return;
    }
    if (item->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("shortcake_fruit", tag)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopDismissed = [=](){
                if (IAPManager::getInstance()->getItemIsBought(2)) {
                    for_each(m_typeItems.begin(), m_typeItems.end(), [=](LockItem* pItem){
                        pItem->unlock();
                    });
                }
            };
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("bling.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    item->addChild(pParticle,100);
                    pParticle->setPosition(Vec2::ZERO);
                    item->unlock();
                };
                RewardManager::getInstance()->showRewardAds("shortcake_fruit", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    m_bItemEnable = false;
//    AudioHelp::getInstance()->playSelectedEffect();
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    if (m_nFlavorIndex>=0) {
        Node* pPreItem = m_typeItems.at(m_nFlavorIndex);
        pPreItem->runAction(MoveBy::create(0.5, Vec2(0, -35)));
    }
    item->runAction(MoveBy::create(0.5, Vec2(0, 35)));
    m_nFlavorIndex = tag;
    showFruit(tag);
    
}

void ShortcakeCutFruitScene::_finishCut()
{
    AudioHelp::getInstance()->_playEffectNiceWork();
    m_pKnife->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    _showBowl();
    int i = 0;
    int cutCount = gnFruitCutCount[m_nFlavorIndex];
    for (auto pFruit:m_FruitsBoard) {
        pFruit->setLocalZOrder(6);
        pFruit->runAction(Sequence::create(Spawn::create(MoveTo::create(1, m_pBoard->getPosition()+Vec2(100-10*i+5.0*(arc4random()%10), 300+i*10-5.0*(arc4random()%10))),
                                                         RotateBy::create(1, -50.0+5.0*(arc4random()%10)), NULL),
                                           Spawn::create(MoveBy::create(0.5, Vec2(-100, -100)),
                                                         ScaleTo::create(0.5,cutCount==1?0.5:1), NULL),
                                           DelayTime::create(i*0.1),
                                           FadeOut::create(0.3), NULL));
        i++;
    }
    m_pBoard->setLocalZOrder(5);
    m_pBoard->runAction(Sequence::create(MoveBy::create(1, Vec2(200, 350)),
                                         RotateBy::create(0.5, -50),
                                         CallFunc::create([=](){
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        
        PolygonHelp::getInstance()->addShapesWithFile("fruit_bowl", body,-m_pBowl->getContentSize().width/2,-m_pBowl->getContentSize().height/2);
        body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
        m_pBowl->setPhysicsBody(body);
        for (int k = 0; k<14; k++) {
            std::string Path;
            if (cutCount==1) {
                Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(k%2+cutCount+1)+".png";
            }else if (m_nFlavorIndex==10) {
                Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_6.png";
            }else if (m_nFlavorIndex == 8) {
                Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_5.png";
            }else{
                Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(cutCount)+".png";
            }
            Sprite* pSlice = Sprite::create(localPath(Path));
            this->addChildToContentLayer(pSlice);
            pSlice->setPosition(m_pBowl->getPosition()+Vec2(50-5.0*(arc4random()%10), 200+5.0*(arc4random()%10)));
            pSlice->setLocalZOrder(5);
            pSlice->setRotation(50.0-10.0*(arc4random()%10));
            pSlice->setVisible(false);
            pSlice->runAction(Sequence::create(DelayTime::create(0.1+0.05*k),
                                               Show::create(),
                                             CallFunc::create([=](){
//                auto body = PhysicsBody::createBox(Size(pSlice->getContentSize().width*0.5,pSlice->getContentSize().height*0.5),PhysicsMaterial(1.0f, 0.0f, 1.0f));
                
                auto body = PhysicsBody::createCircle(pSlice->getContentSize().width*0.2,PhysicsMaterial(1.0f, 0.0f, 1.0f));
                body->setDynamic(true);
                pSlice->setPhysicsBody(body);
                                             }),NULL));
        }
        AudioHelp::getInstance()->playEffect("pour_corn_kernal.mp3");
    }),
                                         DelayTime::create(2),
                                         CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
    }),
                                         RotateTo::create(0.5,0),
                                         MoveBy::create(1,Vec2(visibleSize.width,0)),
                                         CallFunc::create([=](){
        _finish();
    }), NULL));
}
void ShortcakeCutFruitScene::_finish()
{
//    ********** test *************
//    {
//        std::string cakeStr = GameDataManager::getInstance()->m_sFlavorCakeStr;
//        std::string str;
//        if (cakeStr != "") {
//            cakeStr.append(",");
//        }
//        cakeStr.append(std::to_string(m_nFlavorIndex));
//        GameDataManager::getInstance()->m_sFlavorCakeStr = cakeStr;
//    }
//    {
//        std::string flavorStr = GameDataManager::getInstance()->m_sFlavorCreamStr;
//        std::string str;
//        if (flavorStr != "") {
//            flavorStr.append(",");
//        }
//        flavorStr.append(std::to_string(m_nFlavorIndex));
//        GameDataManager::getInstance()->m_sFlavorCreamStr = flavorStr;
//    }
    
    GameDataManager::getInstance()->m_nFlavorIndex = m_nFlavorIndex;
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<ShortcakeAddFruitScene>();
    }), NULL));
}