
#include "FunnelPourScene.h"
#include "SceneManager.h"

FunnelPourScene::FunnelPourScene()
{
    
}

FunnelPourScene::~FunnelPourScene()
{
    
}
bool FunnelPourScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    
    _showBowl();
    _showFunnel();
    
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideTap(m_pFunnel->getPosition());
        m_pFunnel->setTouchEnabled(true);
    }), NULL));
    return true;
}

void FunnelPourScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FunnelPourScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FunnelPourScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "bowl"){
        pDragNode->getDragSprite()->setTexture(localRootPath("1/bowl_1.png"));
    }
}

void FunnelPourScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
}

void FunnelPourScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    if (name == "hand"){
        Rect rect = m_pFunnel->getBoundingBox();
        Rect handRect = m_pHand->getBoundingBox();
        if (handRect.intersectsRect(rect)) {
            pDragNode->setTouchEnabled(false);
            m_pBowl->index = 1;
            m_pHand->setPosition(m_pFunnel->getWorldSpaceActionPoint()+Vec2(20,15));
            m_pHand->setLocalZOrder(0);
            m_pGuideLayer->showGuideMove(m_pBowl->getPosition(), m_pFunnel->getPosition());
            AudioHelp::getInstance()->playEffect("vo_funnel_pour_batter2mold.mp3");
        }
    }else if (name == "bowl" && pDragNode->index == 1) {
        Rect rect = m_pBowl->getBoundingBox();
        Rect rectFunnel = m_pFunnel->getBoundingBox();
        if (rect.intersectsRect(rectFunnel)) {
            _pourBatter();
        }else{
            pDragNode->back(0,[=](){
                pDragNode->getDragSprite()->setTexture(localRootPath("1/bowl.png"));
            });
        }
    }else{
        pDragNode->back(0,[=](){
            pDragNode->getDragSprite()->setTexture(localRootPath("1/bowl.png"));
        });
    }
    m_pGuideLayer->removeGuide();
}

void FunnelPourScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name == "funnel") {
        pDragNode->setLocalZOrder(0);
        pDragNode->setTouchEnabled(false);
        _showHand();
    }
}

#pragma mark - initData
void FunnelPourScene::_initData()
{
    setExPath("content/make/funnel/2/");
}

void FunnelPourScene::_showFunnel()
{
    m_pFunnel = _createDrageNode(localPath("funnel_down.png"));
    m_pFunnel->setTouchEnabled(false);
    m_pFunnel->setMovable(false);
    m_pFunnel->setName("funnel");
    m_pFunnel->setActionPoint(Vec2(147, 4));
    this->addChildToContentLayer(m_pFunnel);
    CMVisibleRect::setPositionAdapted(m_pFunnel, 320-visibleSize.width, 400);
    
    m_pFunnelFront = Sprite::create(localPath("funnel_on.png"));
    this->addChildToContentLayer(m_pFunnelFront);
    m_pFunnelFront->setPosition(m_pFunnel->getPosition());
    m_pFunnelFront->setLocalZOrder(10);
    
    m_pBatter = Sprite::create(localPath("funnel_batter.png"));
    this->addChildToContentLayer(m_pBatter);
    m_pBatter->setPosition(m_pFunnel->getPosition());
    m_pBatter->setOpacity(0);
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL);
    m_pFunnel->runAction(action);
    m_pFunnelFront->runAction(action->clone());
    m_pBatter->runAction(action->clone());
}

void FunnelPourScene::_showBowl()
{
    
    m_pBowl = _createDrageNode(localRootPath("1/bowl.png"));
    m_pBowl->setName("bowl");
    m_pBowl->setActionPoint(Vec2(44, 253));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width/2, 740);
    m_pBowl->setScale(0.7);
  
    ContainerNode* pBowlContainer = ContainerNode::create(m_pBowl);
    pBowlContainer->addFood(localRootPath("1/bowl_batter6.png"),localRootPath("1/bowl_batter6.png"));
    pBowlContainer->addFront(localRootPath("1/bowl_on.png"));
    m_pBowl->setUserData(pBowlContainer);
    
    m_pBowl->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(1, Vec2(visibleSize.width/2, 0)),
                                        CallFunc::create([=](){
        m_pBowl->setOrgPositionDefault();
    }), NULL));
    

}

void FunnelPourScene::_showHand()
{
    m_pHand = _createDrageNode(localPath("hands.png"));
    this->addChildToContentLayer(m_pHand);
    m_pHand->getDragSprite()->setAnchorPoint(Vec2(1, 1));
    m_pHand->setName("hand");
    
    m_pHand->setTouchEnabled(false);
    m_pHand->setPosition(m_pFunnel->getWorldSpaceActionPoint()+Vec2(-visibleSize.width-120, -visibleSize.height*0.5-50));
    
    AudioHelp::getInstance()->playEffect("vo_funnel_drag_finger_hold_mold.mp3");
    m_pHand->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(0.5, Vec2(visibleSize.width, visibleSize.height*0.5)),
                                        CallFunc::create([=](){
        m_pHand->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pHand->getPosition(), m_pFunnel->getPosition());
//        m_pBowl->index = 1;
//        m_pGuideLayer->showGuideMove(m_pBowl->getPosition(), m_pFunnel->getPosition());
    }), NULL));
}

void FunnelPourScene::_pourBatter()
{
    m_pBowl->setPosition(m_pFunnel->getPosition()+Vec2(100, 300));
    m_pBowl->runAction(Sequence::create(RotateBy::create(0.5, -70),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->playPourMilkEffect();
        m_pBowl->setLocalZOrder(5);
        ContainerNode* pContainer = (ContainerNode*)m_pBowl->getUserData();
        if (pContainer) {
            pContainer->pourFood(true,6);
        }
        m_pBatter->setLocalZOrder(5);
        m_pBatter->runAction(FadeIn::create(3));
        
        Sprite* pStream = Sprite::create(localPath("funnel_batter1.png"));
        this->addChildToContentLayer(pStream);
        pStream->setLocalZOrder(5);
        pStream->setPosition(m_pBowl->getWorldSpaceActionPoint()+Vec2(0, -95));
        pStream->runAction(Sequence::create(Repeat::create(Sequence::create(AnimationHelp::createAnimate(localPath("funnel_batter"), 1, 2),
                                                                            DelayTime::create(0.2), NULL), 5),
                                            CallFunc::create([=](){
            pStream->removeFromParent();
        }), NULL));
    }),
                                        DelayTime::create(3),
                                        RotateTo::create(0.5, 0),
                                        MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=](){
        _finish();
    }), NULL));
}

void FunnelPourScene::_finish()
{
    
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<FunnelFryScene>();
    }), NULL));
}