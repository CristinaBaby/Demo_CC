
#include "JuicePourScene.h"
#include "SceneManager.h"

JuicePourScene::JuicePourScene()
{
    
    m_pClippingStraw = nullptr;
}

JuicePourScene::~JuicePourScene()
{
    
}
bool JuicePourScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create("content/make/common/bg1.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showCup();
    _showBlenderCup();
    
    m_pGameUI->showNormalLayout();
    
    return true;
}

void JuicePourScene::onEnter()
{
    ExtensionScene::onEnter();
}

void JuicePourScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        btn->runAction(Sequence::create(DelayTime::create(0.5),
                                        CallFunc::create([=](){
            btn->setEnabled(true);
        }), NULL));
        SceneManager::pushTheScene<DrinkJuiceScene>();
        return;
    }
}

void JuicePourScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void JuicePourScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "blender_cup"){
        
    }else{
//        pDragNode->setScale(1);
        pDragNode->setLocalZOrder(10);
//        m_pStrawScrollView->setLocalZOrder(10);
        m_pGuideLayer->removeGuide();
    }
}

void JuicePourScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "blender_cup"){
        Vec2 pos = m_pCup->getPosition();
        Rect rect = Rect(pos.x-200, pos.y-200, 500, 500);
        if (rect.containsPoint(worldPoint)) {
            m_pGuideLayer->removeGuide();
            pDragNode->setTouchEnabled(false);
            _pourJuice();
        }
    }else{
        if (pDragNode->isMoving()){
            pDragNode->setVisible(true);
            pDragNode->setScale(1);
            if(m_pStrawScrollView->isTouchEnabled()){
                AudioHelp::getInstance()->playEffect("soda_cup.mp3");
            }
            m_pStrawScrollView->setTouchEnabled(false);
        }
    }
}

void JuicePourScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    if (name == "blender_cup"){
        
    }else{
        m_pStrawScrollView->setLocalZOrder(0);
        m_pStrawScrollView->setTouchEnabled(true);
        int tag = pDragNode->getTag();
        if (worldPoint.y<visibleSize.height*0.7) {
            if (!IAPManager::getInstance()->getItemIsBought(1)){
                if(!ConfigManager::getInstance()->getVideoUnLocked("straw", tag) && tag%2==1) {
//                    RewardManager::getInstance()->showRewardAds("straw", tag);
//                    pDragNode->setVisible(false);
//                    pDragNode->setScale(0.8*.5);
//                    pDragNode->setTouchEnabled(true);
//                    pDragNode->setPosition(pDragNode->getOrgPosition());
//                    return;
                    
                    pDragNode->setVisible(false);
                    pDragNode->setScale(0.8*.5);
                    pDragNode->setTouchEnabled(true);
                    pDragNode->setPosition(pDragNode->getOrgPosition());
                    
                    ShopMiniLayer* pLayer = ShopMiniLayer::create(true);
                    this->addChild(pLayer);
                    pLayer->setLocalZOrder(100);
                    pLayer->shopGetFree = [=](){
                        LockUIManager::getInstance()->onRewardSuccess = [=](){
                            AudioHelp::getInstance()->playEffect("bling.mp3");
                            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                            pDragNode->addChild(pParticle,100);
                            pParticle->setPosition(Vec2::ZERO);
//                            item->unlock();
                        };
                        RewardManager::getInstance()->showRewardAds("straw", tag);
                        pLayer->removeFromParent();
                    };
                    
                    pLayer->shopDismissed = [=](){
                        if (IAPManager::getInstance()->getItemIsBought(2)) {
                            DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
                            int strawCount = data.totalCount;
                            for (int i = 0; i<strawCount; i++) {
                                Node* pCup = m_pStrawScrollView->getChildByName("cup"+std::to_string(i));
                                if (pCup) {
                                    Node* pLock = pCup->getChildByTag(TAG_REWARD_LOCK);
                                    if (pCup) {
                                        pLock->setVisible(false);
                                    }
                                }
                            }
                        }
                    };
                    return;
                }
                
            }
//            AudioHelp::getInstance()->playEffect("vo_greate_choice.mp3");
            if(!m_pClippingStraw){
                m_pClippingStraw = ClippingNode::create(Sprite::create("content/category/straw/straw_mask2.png"));
                m_pClippingStraw->setAlphaThreshold(0);
                m_pClippingStraw->setInverted(true);
                m_pClippingStraw->setName("straw_clipping");
                m_pCup->addChild(m_pClippingStraw,8);
            }
            
            AudioHelp::getInstance()->_playEffectLaugh();
            
            Node* pNode = m_pClippingStraw->getChildByName("straw");
            if (pNode) {
                pNode->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(50, 300)),
                                                                ScaleTo::create(0.5, 0.5), NULL),
                                                  MoveBy::create(0.5, Vec2(-visibleSize.width, 100)),
                                                  CallFunc::create([=](){
                    pNode->removeFromParent();
                }), NULL));
            }else{
                m_pGameUI->showNextLayout();
            }
            
            Button* pNext = m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagNext);
            pNext->setTouchEnabled(false);
            pNext->setEnabled(false);
            
            Vec2 startPos = m_pCup->getPosition();
            pDragNode->setTouchEnabled(false);
            GameDataManager::getInstance()->m_nStickIndex = tag;
            pDragNode->runAction(Sequence::create(Spawn::create(EaseSineOut::create(MoveTo::create(0.5, pDragNode->getParent()->convertToNodeSpace(startPos+Vec2(100, 400)))),
                                                                ScaleTo::create(0.5, 1), NULL),
                                                  CallFunc::create([=](){
                pDragNode->setVisible(false);
                pDragNode->setScale(0.8*0.5);
                pDragNode->setTouchEnabled(true);
                pDragNode->setPosition(pDragNode->getOrgPosition());
                AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
                
                auto pMilkDrop = dynamic_cast<ParticleSystem*>(m_pCup->getChildByName("juice_drop"));
                if (pMilkDrop) {
                    pMilkDrop->removeFromParent();
                }
                
                ParticleSystemQuad* pWaterDrop = ParticleSystemQuad::create("particle/juice_drop.plist");
                m_pCup->addChild(pWaterDrop,8);
                pWaterDrop->setPosition(Vec2(0, 80));
                pWaterDrop->setName("juice_drop");
                
                
                KettleNode* pJuice = (KettleNode*)m_pCup->getUserData();
                if (pJuice) {
                    pJuice->waveWater(2,0.3);
                }
                
                Sprite* pStraw = Sprite::create("content/category/straw/"+std::to_string(tag+1)+".png");
                m_pClippingStraw->addChild(pStraw);
                pStraw->setAnchorPoint(Vec2::ZERO);
                pStraw->setPosition(m_pClippingStraw->convertToNodeSpace(startPos+Vec2(-50, -130)+Vec2(100, 400)));
                pStraw->runAction(Sequence::create(EaseSineIn::create(MoveBy::create(0.2, Vec2(-100, -400))),
                                                   CallFunc::create([=](){
                    auto pMilkDrop = dynamic_cast<ParticleSystem*>(m_pCup->getChildByName("juice_drop"));
                    if (pMilkDrop) {
                        pMilkDrop->stopSystem();
                    }
                    
                    Button* pNext = m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagNext);
                    pNext->setTouchEnabled(true);
                    pNext->setEnabled(true);
                }), NULL));
                pStraw->setName("straw");
            }), NULL));
        }else{
            pDragNode->setVisible(false);
            pDragNode->setScale(0.8*0.5);
            pDragNode->setTouchEnabled(true);
            pDragNode->setPosition(pDragNode->getOrgPosition());
        }
    }
    
}

void JuicePourScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "blender_cup"){
        
    }else{
        m_pStrawScrollView->setTouchEnabled(true);
    }
    
}

#pragma mark - initData
void JuicePourScene::_initData()
{
    setExPath("content/make/juice/");
    m_nFlavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
}

void JuicePourScene::_showCup()
{
    m_pCup = Node::create();
    this->addChildToContentLayer(m_pCup);
    CMVisibleRect::setPositionAdapted(m_pCup, 320+visibleSize.width, 420);
    
    KettleNode* pContainer = KettleNode::create(m_pCup);
    m_pCup->setUserData(pContainer);
    pContainer->addBody(localPath("cup0_h.png"));
    pContainer->addFront(localPath("cup0.png"));
    pContainer->addWater(localPath("cup_juice"+std::to_string(m_nFlavorIndex)+".png"),localPath("cup_mask.png"));
    pContainer->getWater()->setScale(1, 0);
    
    m_pCup->setLocalZOrder(5);
    m_pCup->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
}

void JuicePourScene::_showBlenderCup()
{
    m_pBlenderCup = _createDrageNode("content/make/common/blender/blender_cup_down.png");
    this->addChildToContentLayer(m_pBlenderCup);
    CMVisibleRect::setPositionAdapted(m_pBlenderCup, 320+visibleSize.width, 820);
    m_pBlenderCup->setName("blender_cup");
    m_pBlenderCup->setActionPoint(Vec2(28, 282));
    
    KettleNode* pContainer = KettleNode::create(m_pBlenderCup);
    m_pBlenderCup->setUserData(pContainer);
    pContainer->addFront("content/make/common/blender/blender_cup_on.png");
    pContainer->addWater("content/make/common/blender/juice"+std::to_string(m_nFlavorIndex)+"_1.png","content/make/common/blender/blender_cup_mask.png",Vec2(-24,0));
    
    m_pBlenderCup->setTouchEnabled(false);
    m_pBlenderCup->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pBlenderCup->setOrgPositionDefault();
        m_pGuideLayer->showGuideMove(m_pBlenderCup->getPosition(), m_pCup->getPosition());
        m_pBlenderCup->setTouchEnabled(true);
    }), NULL));
}

void JuicePourScene::_pourJuice()
{
    m_pBlenderCup->runAction(Sequence::create(MoveTo::create(0.5, m_pCup->getPosition()+Vec2(100, 350)),
                                              RotateBy::create(0.5, -90),
                                              CallFunc::create([=](){
        m_pBlenderCup->setLocalZOrder(1);
        AudioHelp::getInstance()->playEffect("pouring_juice.mp3");
        KettleNode* pContainer = (KettleNode*)m_pBlenderCup->getUserData();
        if (pContainer) {
            pContainer->pourWater(4);
        }
        
        KettleNode* pJuice = (KettleNode*)m_pCup->getUserData();
        if (pJuice) {
            pJuice->getWater()->runAction(ScaleTo::create(4, 1));
        }
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_milk.plist");
        pParticle->setTexture(Sprite::create(localPath("drop"+std::to_string(m_nFlavorIndex)+".png"))->getTexture());
        m_pCup->addChild(pParticle,8);
        pParticle->setName("particle");
        pParticle->setRotation(-30);
//        pParticle->setRotation(60);
//        pParticle->setLocalZOrder(20);
        pParticle->setPosition(m_pCup->convertToNodeSpace(m_pBlenderCup->getWorldSpaceActionPoint()));
        pParticle->runAction(MoveBy::create(4, Vec2(60, 0)));
        
    }),
                                              MoveBy::create(4, Vec2(60, 0)),
                                              CallFunc::create([=](){
        auto particle = dynamic_cast<ParticleSystem*>(m_pCup->getChildByName("particle"));
        if (particle) {
            particle->stopSystem();
        }
    }),
                                              DelayTime::create(0.5),
                                              RotateTo::create(0.5, 0),
                                              MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                              CallFunc::create([=](){
        _finish();
    }), NULL));
}

void JuicePourScene::_decorateStaw()
{    
    m_sCurTypeStr = "straw";
    std::stringstream ostr;
    ostr.str("");
    ostr<<"content/category/"<<m_sCurTypeStr<<"_icon/";
    m_pStrawScrollView = ExpandScrollView::create();
    m_pStrawScrollView->setContentSize(Size(visibleSize.width, 600));
    this->addChildToContentLayer(m_pStrawScrollView);
    CMVisibleRect::setPositionAdapted(m_pStrawScrollView, visibleSize.width, 460,kBorderLeft);
    m_pStrawScrollView->setClippingEnabled(false);
    
    m_pStrawScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pStrawScrollView->setScrollBarEnabled(false);
    m_pStrawScrollView->setTouchEnabled(true);
    
    m_pStrawScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                   MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                                   CallFunc::create([=](){
        m_pStrawScrollView->scrollToRight(1, true);
    }),
                                                   DelayTime::create(1),
                                                   CallFunc::create([=](){
        m_pStrawScrollView->scrollToLeft(0.5, true);
    }), NULL));
    AudioHelp::getInstance()->playEffect("vo_choose_straw.mp3");
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    int strawCount = data.totalCount;
    float det = 180;
    for (int i = 0; i<strawCount; i++) {
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(1)){
            if (i%2==1){
                if (ConfigManager::getInstance()->getVideoUnLocked("straw", i)){
                    lockName = "";
                }else{
                    
                    lockName = "content/common/video.png";
                    locked = true;
                }
                
            }
            
        }
        
        DragNode* pItem = _createDrageNode("content/category/straw/"+std::to_string(i+1)+".png");
        //        pItem->setVisible(false);
        pItem->setCanSwallow(false);
        pItem->setDirection(DragNode::Direction::VERTICAL);
        pItem->setScale(0.8*0.5);
        pItem->setTag(i);
        m_pStrawScrollView->getInnerContainer()->addChild(pItem,2);
        
        Sprite* pCupShadow = Sprite::create("content/category/straw_icon/cup_shadow.png");
        m_pStrawScrollView->getInnerContainer()->addChild(pCupShadow);
        
        Sprite* pCupBack = Sprite::create("content/category/straw_icon/cup_down.png");
        m_pStrawScrollView->getInnerContainer()->addChild(pCupBack,1);
        
        Sprite* pStraw = Sprite::create("content/category/straw_icon/"+std::to_string(i+1)+".png");
        m_pStrawScrollView->getInnerContainer()->addChild(pStraw,5);
        
        Sprite* pCupFront = Sprite::create("content/category/straw_icon/cup_on.png");
        m_pStrawScrollView->getInnerContainer()->addChild(pCupFront,5);
        pCupFront->setName("cup"+std::to_string(i));
        if (locked) {
            Sprite* pLock = Sprite::create(lockName);
            pCupFront->addChild(pLock);
            pLock->setTag(TAG_REWARD_LOCK);
            pLock->setName("lock"+std::to_string(i));
            LockUIManager::getInstance()->registerLock("straw", i, pLock->getParent());
            //            CMVisibleRect::setPositionAdapted(pLock, 167+30+205*i, 250-50);
            pLock->setPosition(Vec2(100, 30));
        }
        pItem->setContentSize(pCupFront->getContentSize()/0.4);
        
        CMVisibleRect::setPositionAdapted(pCupShadow, 167+det*i, 300);
        pCupBack->setPosition(pCupShadow->getPosition());
        pCupFront->setPosition(pCupShadow->getPosition());
        CMVisibleRect::setPositionAdapted(pStraw, 167+det*i, 300);
        CMVisibleRect::setPositionAdapted(pItem, 167+det*i, 300);
        
        pCupShadow->setPosition(Vec2(167+det*i, 300));
        pCupBack->setPosition(pCupShadow->getPosition());
        pCupFront->setPosition(pCupShadow->getPosition());
        pStraw->setPosition(Vec2(167+det*i, 300));
        pItem->setPosition(Vec2(167+det*i, 310));
        pItem->setOrgPositionDefault();
    }
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    AudioHelp::getInstance()->playEffect("vo_choose_straws.mp3");
    m_pStrawScrollView->setInnerContainerSize(Size(167+det*strawCount, 500));
}

void JuicePourScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
//        SceneManager::replaceTheScene<JuicePourScene>();
        _decorateStaw();
    }), NULL));
    
}