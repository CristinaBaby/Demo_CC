
#include "CornflakeAddMilkScene.h"
#include "SceneManager.h"

CornflakeAddMilkScene::CornflakeAddMilkScene()
{
    
}

CornflakeAddMilkScene::~CornflakeAddMilkScene()
{
    
}
bool CornflakeAddMilkScene::init()
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
    
    _showBowl();
    
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
        _showSievel();
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void CornflakeAddMilkScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CornflakeAddMilkScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CornflakeAddMilkScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void CornflakeAddMilkScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void CornflakeAddMilkScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    
    AudioHelp::getInstance()->stopLoopEffect();
    std::string name = pDragNode->getName();
    if (name == "sievel") {
        Rect rectBowl = m_pBowl->getBoundingBox();
        
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)){
            m_pGuideLayer->removeGuide();
            _pourSievel();
        }
    }else if (name == "milk"){
        Rect rectBowl = m_pBowl->getBoundingBox();
        
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)){
            m_pGuideLayer->removeGuide();
            _pourMilk();
        }
        
    }
}

void CornflakeAddMilkScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CornflakeAddMilkScene::_initData()
{
    setExPath("content/make/cornmeal/");
}

void CornflakeAddMilkScene::_showBowl()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pBowl = Sprite::create(localPath("bowl0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320+visibleSize.width, 360);
    
    m_pBowlFront = Sprite::create(localPath("bowl1.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    
    m_pBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    m_pBowlFront->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 320, 360);
}

void CornflakeAddMilkScene::_showSievel()
{
    m_pSievel = _createDrageNode(localPath("colander.png"));
    this->addChildToContentLayer(m_pSievel);
    CMVisibleRect::setPositionAdapted(m_pSievel, 520+visibleSize.width, 500);
    m_pSievel->setActionPoint(Vec2(110, 140));
    m_pSievel->setName("sievel");
    m_pSievel->setTouchEnabled(false);
    m_pSievel->setLocalZOrder(5);
    
    ContainerNode* pContainer = ContainerNode::create(m_pSievel);
    pContainer->addFood(localPath("cornflake1.png"),localPath("colander_mask.png"));
    m_pSievel->setUserData(pContainer);
    
    m_pSievel->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pSievel->getPosition(), m_pBowl->getPosition());
                m_pSievel->setTouchEnabled(true);
    }), NULL));
    
}

void CornflakeAddMilkScene::_showMilk()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    AudioHelp::getInstance()->playEffect("vo_add_milk_bowl.mp3");
    m_pMilk = _createDrageNode(localPath("milk0.png"));
    this->addChildToContentLayer(m_pMilk);
    CMVisibleRect::setPositionAdapted(m_pMilk, 420+visibleSize.width, 780);
    m_pMilk->setActionPoint(Vec2(18, 216));
    m_pMilk->setName("milk");
    m_pMilk->setTouchEnabled(false);
    
    m_pMilk->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pMilk->getPosition(), m_pBowl->getPosition());
        m_pMilk->setTouchEnabled(true);
    }), NULL));
}

void CornflakeAddMilkScene::_pourSievel()
{
    m_pSievel->setTouchEnabled(false);
    m_pSievel->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, -30),
                                                        MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(180, 370)), NULL),
                                          CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("pour_cereal.mp3");
        AudioHelp::getInstance()->playPourPowderEffect();
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_corn.plist");
        m_pSievel->addChild(pParticle);
        pParticle->setPosition(m_pSievel->getActionPoint());
        pParticle->setName("particle");
        pParticle->setRotation(30);
        pParticle->setLocalZOrder(20);
        pParticle->setPosition(m_pSievel->convertToNodeSpace(m_pSievel->getWorldSpaceActionPoint()));
        
        ContainerNode* pContainer = (ContainerNode*)m_pSievel->getUserData();
        if (pContainer) {
            pContainer->pourFood(2);
        }
        
        Sprite* pAdd = Sprite::create(localPath("cornflake2.png"));
        if (pAdd) {
            pAdd->setPosition(Vec2::ZERO);
            pAdd->setOpacity(0);
            m_pMixture->addChild(pAdd);
            pAdd->runAction(Sequence::create(FadeIn::create(1.5),
                                             CallFunc::create([=](){
                m_pGuideLayer->removeGuide();
            }), NULL));
        }
    }),
                                          DelayTime::create(2),
                                          CallFunc::create([=](){
        auto particle = dynamic_cast<ParticleSystem*>(m_pSievel->getChildByName("particle"));
        if (particle) {
            particle->stopSystem();
            particle->removeFromParent();
        }
        
        AudioHelp::getInstance()->_playEffectNiceWork();
    }),
                                          RotateTo::create(0.5, 0),
                                          MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
        _showMilk();        
    }), NULL));
}

void CornflakeAddMilkScene::_pourMilk()
{
    m_pMilk->setTouchEnabled(false);
    m_pMilk->getDragSprite()->setTexture(localPath("milk1.png"));
    m_pMilk->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(180, 280)),
                                            RotateTo::create(0.5,-60),
                                            CallFunc::create([=](){
        AudioHelp::getInstance()->playPourMilkEffect();
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_milk.plist");
        m_pMilk->addChild(pParticle);
        pParticle->setPosition(m_pMilk->getActionPoint());
        pParticle->setName("particle");
        pParticle->setRotation(60);
        pParticle->setLocalZOrder(20);
        pParticle->setPosition(m_pMilk->convertToNodeSpace(m_pMilk->getWorldSpaceActionPoint()));
        
        
        
        ParticleSystemQuad* pMilkDrop = ParticleSystemQuad::create("particle/milk_drop.plist");
        this->addChildToContentLayer(pMilkDrop);
        pMilkDrop->setPosition(m_pBowl->getPosition());
        pMilkDrop->setName("milk_drop");
        pMilkDrop->setVisible(false);
        pMilkDrop->setLocalZOrder(20);
        pMilkDrop->runAction(Sequence::create(DelayTime::create(0.5),
                                              Show::create(), NULL));
        
        
        ParticleSystemQuad* pCornDrop = ParticleSystemQuad::create("particle/corn_drop.plist");
        this->addChildToContentLayer(pCornDrop);
        pCornDrop->setPosition(m_pBowl->getPosition());
        pCornDrop->setName("corn_drop");
        pCornDrop->setVisible(false);
        pCornDrop->setLocalZOrder(20);
        pCornDrop->runAction(Sequence::create(DelayTime::create(0.5),
                                              Show::create(), NULL));
        
        Sprite* pAdd = Sprite::create(localPath("cornflake3.png"));
        m_pMixture->addChild(pAdd);
        pAdd->setOpacity(0);
        pAdd->runAction(FadeIn::create(3));
    }),
//                                        DelayTime::create(2),
//                                            CircleMove::create(2,m_pBowl->getPosition()+Vec2(80, 150),60,180),
                                        MoveBy::create(4, Vec2(50, 20)),
                                            CallFunc::create([=](){
        auto particle = dynamic_cast<ParticleSystem*>(m_pMilk->getChildByName("particle"));
        if (particle) {
            particle->stopSystem();
//            particle->removeFromParent();
        }
        auto pMilkDrop = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("milk_drop"));
        if (pMilkDrop) {
            pMilkDrop->stopSystem();
//            pMilkDrop->removeFromParent();
        }
        auto pCornDrop = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("corn_drop"));
        if (pCornDrop) {
            pCornDrop->stopSystem();
//            pCornDrop->removeFromParent();
        }
        AudioHelp::getInstance()->_playEffectNiceWork();
    }),
                                        DelayTime::create(0.5),
                                            RotateTo::create(1, 0),
                                            MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                            CallFunc::create([=](){
        _finish();
        MenuLayer::addIngredient(MenuLayer::ePackageCorn, MenuLayer::eCorn_milk);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
//        IngredientManager::getInstance()->removeIngredient(pIngredient);
        m_pMilk->removeFromParent();
    }), NULL));
}
void CornflakeAddMilkScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<CornflakeEatScene>();
    }), NULL));
    
}