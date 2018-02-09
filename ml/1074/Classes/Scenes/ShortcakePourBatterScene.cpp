
#include "ShortcakePourBatterScene.h"
#include "SceneManager.h"

ShortcakePourBatterScene::ShortcakePourBatterScene()
{
    m_nMoveCount = 0;
}

ShortcakePourBatterScene::~ShortcakePourBatterScene()
{
    
}
bool ShortcakePourBatterScene::init()
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
    
    _showPan();
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _showButters();
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void ShortcakePourBatterScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ShortcakePourBatterScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ShortcakePourBatterScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "butter"){
        int index = pDragNode->index;
        if (index == 0) {
            pDragNode->index = 1;
            m_pCurButter = pDragNode;
            m_pButterBottle->setTouchEnabled(false);
            m_pButterBottle->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }else{
            pDragNode->setLocalZOrder(8);
        }
    }else if (name == "butter_bottle"){
        int index = pDragNode->index;
        if (index == 0){
            if (!IAPManager::getInstance()->getItemIsBought(2)) {
                if (!ConfigManager::getInstance()->getVideoUnLocked("shortcake_butter", 1)){
                    ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
                    this->addChildToUILayer(pLayer);
                    pLayer->setLocalZOrder(100);
                    pDragNode->setTouchEnabled(false);
                    pLayer->shopDismissed = [=](){
                        
                        pDragNode->setTouchEnabled(true);
                    };
                    pLayer->shopGetFree = [=](){
                        pDragNode->setTouchEnabled(true);
                        LockUIManager::getInstance()->onRewardSuccess = [=](){
                            AudioHelp::getInstance()->playEffect("bling.mp3");
                            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                            pDragNode->addChild(pParticle,100);
                            pParticle->setPosition(Vec2::ZERO);
                        };
                        RewardManager::getInstance()->showRewardAds("shortcake_butter", 1);
                        pLayer->removeFromParent();
                    };
                    return;
                }
            }
            AudioHelp::getInstance()->playEffect("bottle_open.mp3");
            pDragNode->setDragSprite(Sprite::create(localPath("butter_spray1.png")));
            pDragNode->index = 1;
            m_pCurButter = pDragNode;
            
            m_pButter->setTouchEnabled(false);
            m_pButter->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
            m_pButterPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }else{
            pDragNode->setLocalZOrder(8);
        }
    }

}

void ShortcakePourBatterScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "butter" || name == "butter_bottle"){
        if (name == "butter"){
            pDragNode->setLocalZOrder(5);
        }
        int index = pDragNode->index;
        Rect rect = m_pPan->getBoundingBox();
        if (index == 1) {
            if (name == "butter") {
                if (rect.containsPoint(worldPoint)) {
                    rect.origin = rect.origin+Vec2(130, 100);
                    rect.size = Size(150,140);
                    pDragNode->setLimitRect(rect);
                    pDragNode->index = 2;
                }
            }else{
                if (rect.containsPoint(worldPoint-Vec2(140, 100))) {
                    rect.origin = rect.origin+Vec2(70, 200)+Vec2(140, 100);
                    rect.size = Size(350,140);
                    pDragNode->setLimitRect(rect);
                    pDragNode->index = 2;
                }
            }
        }else if (index == 2){
            if (name == "butter"){
                pDragNode->setDragSprite(Sprite::create(localPath("butter2.png")));
                AudioHelp::getInstance()->playLoopEffect("pouring_sauce.mp3");
                
            }else{
                AudioHelp::getInstance()->playLoopEffect("butter_spray.mp3");
                Sprite* pSpray = Sprite::create(localPath("butter_spray3.png"));
                pSpray->setAnchorPoint(Vec2(1, 1));
                pDragNode->addChild(pSpray);
                pSpray->setName("spray");
                pSpray->setRotation(40);
                pSpray->setPosition(pDragNode->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                
                pDragNode->setDragSprite(Sprite::create(localPath("butter_spray2.png")));
                pDragNode->runAction(Sequence::create(RotateTo::create(0.5, -40),
                                                      CallFunc::create([=](){
                }), NULL));
                
//                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/bubble.plist");
//                pDragNode->addChild(pParticle);
//                pParticle->setRotation(40);
//                pParticle->setName("particle");
//                pParticle->setPosition(pDragNode->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
            }
            pDragNode->index = 3;
        }else if (index == 3){
            if (name == "butter"){
                m_pButterScribble->paint(worldPoint+Vec2(-50, 0)+pDragNode->getOffset(),worldPoint+Vec2(-50, 0));
                m_pButterScribble->paint(worldPoint+pDragNode->getOffset(),worldPoint);
                m_pButterScribble->paint(worldPoint+Vec2(50, 0)+pDragNode->getOffset(),worldPoint+Vec2(-50, 0));
                Director::getInstance()->getRenderer()->render();
                AudioHelp::getInstance()->playLoopEffect("pouring_sauce.mp3");
                
            }else{
                AudioHelp::getInstance()->playLoopEffect("butter_spray.mp3");
                Vec2 pos = worldPoint-Vec2(160, 130);
                m_pButterScribble->paint(pos+pDragNode->getOffset(),pos);
                Director::getInstance()->getRenderer()->render();
            }
            _paintButter();
        }
    }else if (name == "bowl"){
        Rect rect = m_pPan->getBoundingBox();
//        Rect rectBowl = m_pBowl->getDragSprite()->getBoundingBox();
        
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rect.intersectsRect(rectIngredient)) {
            pDragNode->setTouchEnabled(false);
            m_pGuideLayer->removeGuide();
            pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pPan->getPosition()+Vec2(100, 160)),
                                                  RotateBy::create(0.5, -100),
                                                  CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pouring_sauce.mp3");
                KettleNode* pContainner = (KettleNode*)pDragNode->getUserData();
                if (pContainner) {
                    pContainner->pourWaterOnce(100,1.5);
                }
                Sprite* pAdd = Sprite::create("content/make/shortcake/make1/mod/mod_batter"+std::to_string(m_nFlavorIndex+1)+".png");
                m_pMixture->addChild(pAdd);
                pAdd->setOpacity(0);
                pAdd->runAction(FadeIn::create(1.5));
            }),
                                                  DelayTime::create(2),
                                                  CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("vo_sweet.mp3");
            }),
                                                  RotateTo::create(0.5, 0),
                                                  MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                  CallFunc::create([=](){
                _finish();
            }),  NULL));
        }
    }
}

void ShortcakePourBatterScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    AudioHelp::getInstance()->stopLoopEffect();
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name == "butter" || name == "butter_bottle"){
        int index = pDragNode->index;
        if (index == 3){
            if (name == "butter"){
                
            }else{
                pDragNode->removeChildByName("spray");
                pDragNode->removeChildByName("particle");
                pDragNode->index = 1;
                pDragNode->setDragSprite(Sprite::create(localPath("butter_spray1.png")));
//                pDragNode->runAction(RotateTo::create(1, 0));
            }
            
        }
    }
}

void ShortcakePourBatterScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

#pragma mark - initData
void ShortcakePourBatterScene::_initData()
{
    setExPath("content/make/shortcake/make1/");
    m_nFlavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
}

void ShortcakePourBatterScene::_showPan()
{
    m_pPan = Sprite::create(localPath("cake_mod0.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320-visibleSize.width, 380);
    m_pPan->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pPanFront = Sprite::create(localPath("cake_mod1.png"));
    this->addChildToContentLayer(m_pPanFront);
    m_pPanFront->setLocalZOrder(10);
    m_pPanFront->setPosition(m_pPan->getPosition());
    m_pPanFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pMixture = Node::create();
    m_pPan->addChild(m_pMixture);
    m_pMixture->setPosition(Vec2(m_pPan->getContentSize().width*0.5,m_pPan->getContentSize().height*0.5));
    
//    m_pButterPan = Sprite::create(localPath("butter3.png"));
//    m_pMixture->addChild(m_pButterPan);
//    m_pButterPan->setOpacity(0);
    
    Sprite* pButter = Sprite::create(localPath("butter3.png"));
    pButter->setPosition(pButter->getContentSize()*0.5);
    m_pButterScribble = ScribbleNode::create(pButter->getContentSize());
    m_pButterScribble->useBrush("content/brush.png",Scribble::BrushType::eBrush);
    m_pMixture->addChild(m_pButterScribble);
    m_pButterScribble->antiAliasing();
    m_pButterScribble->useTarget(pButter);
}

void ShortcakePourBatterScene::_showButters()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pButterPlate = Sprite::create(localPath("butter_plate.png"));
    this->addChildToContentLayer(m_pButterPlate);
    CMVisibleRect::setPositionAdapted(m_pButterPlate, 190+visibleSize.width, 730);
    
    m_pButter = _createDrageNode(localPath("butter0.png"));
    this->addChildToContentLayer(m_pButter);
    CMVisibleRect::setPositionAdapted(m_pButter, 190+visibleSize.width, 735);
    m_pButter->setName("butter");
    
    m_pButterBottle = _createDrageNode(localPath("butter_spray0.png"));
    this->addChildToContentLayer(m_pButterBottle);
    CMVisibleRect::setPositionAdapted(m_pButterBottle, 500+visibleSize.width, 820);
    m_pButterBottle->setName("butter_bottle");
    m_pButterBottle->setActionPoint(Vec2(40, 238));
    
    if (!IAPManager::getInstance()->getItemIsBought(2)) {
        if (!ConfigManager::getInstance()->getVideoUnLocked("shortcake_butter", 1)){
            LockUIManager::getInstance()->registerLock("shortcake_butter", 1, m_pButterBottle);
            LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
        }
    }
    
    m_pButter->setTag(0);
    m_pButterBottle->setTag(1);
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
            if (pNode->getName() == "butter_bottle"){
                if (ConfigManager::getInstance()->getVideoUnLocked("shortcake_butter", 1)) {
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
                    m_pButterBottle->addChild(pParticle,20);
                    pParticle->setPosition(Vec2::ZERO);
                }
                AudioHelp::getInstance()->playEffect("vo_butter_cake_mold_ayer.mp3");
            }
        }
    }), NULL);
    m_pButterPlate->runAction(action);
    m_pButter->runAction(action->clone());
    m_pButterBottle->runAction(action->clone());
}

void ShortcakePourBatterScene::_showBowl()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pBowl = _createDrageNode(localPath("bowl1_0.png"));
    this->addChildToContentLayer(m_pBowl);
    KettleNode* pContainner = KettleNode::create(m_pBowl);
    pContainner->addShadow(localPath("bowl1_shadow.png"));
    pContainner->addFront(localPath("bowl1_1.png"));
    pContainner->addWater(localPath("mod/mod_batter"+std::to_string(m_nFlavorIndex+1)+".png"),localPath("bowl1_mask.png"));
    m_pBowl->setUserData(pContainner);
    m_pBowl->setActionPoint(Vec2(31, 215));
    m_pBowl->setName("bowl");
    
    m_pBowl->setScale(0.7);
    m_pBowl->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 760);
    m_pBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                           CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pBowl->getPosition(), m_pPan->getPosition());
        m_pBowl->setOrgPositionDefault();
        m_pBowl->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_pour_mixture_mold.mp3");
    }), NULL));
}

void ShortcakePourBatterScene::_paintButter()
{
    m_nMoveCount++;
//    m_pButterPan->setOpacity(m_nMoveCount);
    if (m_nMoveCount==0) {
        
        AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
    }
    if (m_nMoveCount%50 == 0) {
        int percent = m_pButterScribble->getCanvas()->getPercentageTransparent();
        log("====percent===%d",percent);
        if ((m_pCurButter->getTag()==0 &&percent<=65)||(m_pCurButter->getTag()==1 &&percent<=72)) {
            
            m_pButterScribble->resetCanvas();
            m_pButterScribble->paint(localPath("butter3.png"));
            m_pCurButter->ignoreMoving = true;
            //        m_pButterPan->runAction(FadeTo::create(0.5, 255));
            m_pCurButter->runAction(Sequence::create(DelayTime::create(0.5),
                                                     CallFunc::create([=](){
                AudioHelp::getInstance()->stopLoopEffect();
                m_pCurButter->setTouchEnabled(false);
                if (m_pCurButter->getTag()==1) {
                    m_pCurButter->runAction(RotateTo::create(0.5,0));
                    m_pCurButter->removeChildByName("spray");
                    m_pCurButter->removeChildByName("particle");
                    m_pCurButter->setDragSprite(Sprite::create(localPath("butter_spray1.png")));
                }
            }),
                                                     MoveTo::create(0.5, m_pCurButter->getOrgPosition()),
                                                     DelayTime::create(0.5),
                                                     CallFunc::create([=](){
                if (m_pCurButter->getTag()==0) {
                    m_pButterPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                }
                AudioHelp::getInstance()->stopLoopEffect();
                AudioHelp::getInstance()->playEffect("ding.mp3");
                AudioHelp::getInstance()->_playEffectNiceWork();
            }),
                                                     MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                     CallFunc::create([=](){
                _finishPaintButter();
            }), NULL));
        }
    }
}

void ShortcakePourBatterScene::_finishPaintButter()
{
    _showBowl();
}

void ShortcakePourBatterScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<ShortcakeBakeScene>();
    }), NULL));
    
}