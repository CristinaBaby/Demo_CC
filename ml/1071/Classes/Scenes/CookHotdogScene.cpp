
#include "CookHotdogScene.h"
#include "SceneManager.h"

CookHotdogScene::CookHotdogScene()
{
    m_pBowl = nullptr;
    m_nFryCount = 0;
    m_bSpoonIsReady = false;
}

CookHotdogScene::~CookHotdogScene()
{
}
bool CookHotdogScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg2.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _createFryUI();
        AudioHelp::getInstance()->playEffect("vo_pour_sake.mp3");
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    
    
    m_pGameUI->showNormalLayout();
    return true;
}

void CookHotdogScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CookHotdogScene::onExit()
{
    ExtensionScene::onExit();
}

void CookHotdogScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CookHotdogScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
}

void CookHotdogScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
    if (pContainer) {
        pContainer->showShadow(false);
    }
    int tag = pDragNode->getTag();
    
    pDragNode->setScale(1);
    pDragNode->setLocalZOrder(50);
    
}

void CookHotdogScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}

void CookHotdogScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
    pDragNode->stopAllActions();
 
    std::string name = pDragNode->getName();
    if (name=="sake") {
        pDragNode->setTouchEnabled(false);
        pDragNode->setVisible(false);
        pDragNode->back();
        _addSake();

    }
}

void CookHotdogScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->stopAllActions();
    
    _back(pDragNode);
}

#pragma mark - initData
void CookHotdogScene::_initData()
{
    setExPath("content/make/japanese/");
    
    m_nNextSceneTag = GameUIEvent::eSceneTagDecHotdog;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecHotdog;
}

#pragma mark ==== Mix UI =====
void CookHotdogScene::_createFryUI()
{
    m_pStove = Sprite::create(localPath("make2/stove.png"));
    this->addChildToContentLayer(m_pStove);
    CMVisibleRect::setPositionAdapted(m_pStove, 320, 330);
    
    m_pSakePlatform = Sprite::create(localPath("make5/sake1_1.png"));
    this->addChildToContentLayer(m_pSakePlatform);
    CMVisibleRect::setPositionAdapted(m_pSakePlatform, 550+visibleSize.width, 750);
    
    m_pSakeBottle = _createDrageNode(localPath("make5/sake1.png"));
    this->addChildToContentLayer(m_pSakeBottle);
    CMVisibleRect::setPositionAdapted(m_pSakeBottle, 550+visibleSize.width, 750);
    m_pSakeBottle->setName("sake");
    
    ContainerNode* pSake = ContainerNode::create(m_pSakeBottle);
    pSake->addShadow(localPath("make5/sake1_shadow.png"));
    m_pSakeBottle->setUserData(pSake);
    
    m_pPan = Sprite::create(localPath("make2/pan.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 200+50, 300+50);
    
    for (int i = 0; i<3; i++) {
        Sprite* pHotdog = Sprite::create(localPath("make5/hotdog.png"));
        this->addChildToContentLayer(pHotdog);
        pHotdog->setLocalZOrder(1);
        pHotdog->setPosition(m_pPan->getPosition()+Vec2(70, 100-75*(i%3)));
        m_HotdogVector.push_back(pHotdog);
    }
    m_pSakeBottle->setTouchEnabled(false);
    m_pSakeBottle->runAction(Sequence::create(DelayTime::create(1),
                                       MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFunc::create([=](){
        m_pSakeBottle->setOrgPositionDefault();
        m_pSakeBottle->setTouchEnabled(true);
    }), NULL));
    m_pSakePlatform->runAction(Sequence::create(DelayTime::create(1),
                                                MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),NULL));
}

void CookHotdogScene::_showPanLid()
{
    m_pPanLid = Sprite::create(localPath("make5/lid.png"));
    this->addChildToContentLayer(m_pPanLid);
    m_pPanLid->setPosition(m_pPan->getPosition()+Vec2(visibleSize.width+65, 15));
    m_pPanLid->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    m_pPanLid->setLocalZOrder(5);
    
}

#pragma mark ======== deel ========================
void CookHotdogScene::_back(DragNode* pDragNode)
{
    pDragNode->back(0,[=](){
        ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
        if (pContainer) {
            pContainer->showShadow();
        }
    });
}


void CookHotdogScene::_addSake()
{
    Sprite* pBottle = Sprite::create(localPath("make5/sake2.png"));
    pBottle->setPosition(Vec2(m_pPan->getPositionX()+200, m_pPan->getPositionY()+100));
    this->addChildToContentLayer(pBottle);
    
    Sprite* pOilStream = Sprite::create(localPath("make5/sake2_1.png"));
    this->addChildToContentLayer(pOilStream);
    pOilStream->setPosition(pBottle->getPosition());
    pOilStream->setLocalZOrder(10);
    
    pOilStream->runAction(Repeat::create(AnimationHelp::createAnimate(localPath("make5/sake2_"), 1, 2), 10));
    AudioHelp::getInstance()->playPourWaterEffect();
    
    m_pPanSake = Sprite::create(localPath("make5/pan_sake.png"));
    this->addChildToContentLayer(m_pPanSake);
    CMVisibleRect::setPositionAdapted(m_pPanSake, 320, 320+50);
    m_pPanSake->setOpacity(0);
    m_pPanSake->setScale(0.2);
    m_pPanSake->runAction(Sequence::create(DelayTime::create(0.3),
                                          Spawn::create(ScaleTo::create(0.8, 1),
                                                        FadeIn::create(1), NULL), NULL));
    pBottle->runAction(Sequence::create(DelayTime::create(2),
                                        CallFunc::create([=](){
        pOilStream->removeFromParent();
        AudioHelp::getInstance()->stopPourWaterEffect();
        m_pSakeBottle->setVisible(true);
        pBottle->setVisible(false);
        
        _back(m_pSakeBottle);
        _showPanLid();
        m_pSakePlatform->runAction(Sequence::create(DelayTime::create(0.5),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width,0)), NULL));
        m_pSakeBottle->runAction(Sequence::create(DelayTime::create(0.5),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width,0)), NULL));
    }),
                                        DelayTime::create(0.5),
                                        CallFunc::create([=](){
        _doCook();
    }),
                                        
                                        NULL));
}

void CookHotdogScene::_doCook()
{
    AudioHelp::getInstance()->playLoopEffect("boil.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/vapour.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setLocalZOrder(2);
    pParticle->setPosition(m_pPanSake->getPosition());
    pParticle->setVisible(false);
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        pParticle->setVisible(true);
    }), NULL));
    
    m_pLidClipping = ClippingNode::create(Sprite::create(localPath("make5/mask.png")));
    this->addChildToContentLayer(m_pLidClipping);
    m_pLidClipping->setPosition(m_pPanSake->getPosition());
    m_pLidClipping->setLocalZOrder(4);
    
    for (int i = 0; i<10; i++) {
        Sprite* pWater = Sprite::create(localPath("make5/water.png"));
        m_pLidClipping->addChild(pWater);
        Vec2 pos = Vec2(-190.0+40.0*(arc4random()%10), -190.0+40.0*(arc4random()%10));
        pWater->setPosition(pos);
        pWater->setOpacity(0);
        pWater->runAction(Sequence::create(DelayTime::create(0.5+0.2*i),
                                           FadeIn::create(2), NULL));
    }
    for_each(m_HotdogVector.begin(), m_HotdogVector.end(), [=](Node* pNode){
        pNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                                MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
    });
    m_pPanSake->runAction(Sequence::create(FadeOut::create(8),
                                          CallFunc::create([=](){
        pParticle->stopSystem();
        pParticle->runAction(FadeOut::create(1));
        
        AudioHelp::getInstance()->stopLoopEffect();;
        for_each(m_HotdogVector.begin(), m_HotdogVector.end(), [=](Node* pNode){
            pNode->stopAllActions();
        });
    }),
                                           DelayTime::create(1),
                                           CallFunc::create([=](){
        m_pPanLid->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
        m_pLidClipping->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    }),
                                           DelayTime::create(1),
                                           CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle2 = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle2);
        pParticle2->setLocalZOrder(10);
        pParticle2->setPosition(visibleSize*0.5);
    }),
                                           DelayTime::create(2),
                                           CallFunc::create([=](){
        SceneManager::replaceTheScene<DecHotdogScene>();
    }), NULL));
}

void CookHotdogScene::_finishAll()
{
}
