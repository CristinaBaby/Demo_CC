
#include "SnowconeCrushIceScene.h"
#include "SceneManager.h"

SnowconeCrushIceScene::SnowconeCrushIceScene()
{
    m_pSnowConeNode = nullptr;
    m_pCone = nullptr;
    m_pConeFront = nullptr;
    m_pConeIce = nullptr;
    m_pIceBag = nullptr;
    m_nSnowPutCount = 0;
    m_pSpoon = nullptr;
}

SnowconeCrushIceScene::~SnowconeCrushIceScene()
{
    
}
bool SnowconeCrushIceScene::init()
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
    
    _showMachine();
    
    m_pGameUI->showNormalLayout();
    return true;
}

void SnowconeCrushIceScene::onEnter()
{
    ExtensionScene::onEnter();
}

void SnowconeCrushIceScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void SnowconeCrushIceScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void SnowconeCrushIceScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if ("spoon" == name){
#pragma mark ======spoon 舀冰 =============
        if (1 == m_pSpoon->index) {
        Rect rect = m_pMachine->getBoxWorldRect();
        if (rect.containsPoint(worldPoint)) {
            m_pSpoon->index = 1;
            m_pSpoon->setTouchEnabled(false);
            Sprite* pSpoonIn = Sprite::create(localPath("spoon_ice.png"));
            m_pSpoon->setVisible(false);
            AudioHelp::getInstance()->playEffect("scoopup_ice.mp3");
            m_pMachine->getIce(pSpoonIn, [=](){
                m_pSpoon->setPosition(m_pSpoon->getParent()->convertToNodeSpace(pSpoonIn->getParent()->convertToWorldSpace(pSpoonIn->getPosition())));
//                m_pSpoon->getDragSprite()->setTexture(localPath("spoon1.png"));
                if (m_nSnowPutCount==0) {
                    m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2, visibleSize.height*0.8), Vec2(visibleSize.width/2, visibleSize.height*0.4));
                }
                m_pMachine->setIcePercent((4-m_nSnowPutCount)*20);
                m_pSpoon->setTouchEnabled(true);
                m_pSpoon->setVisible(true);
                pSpoonIn->removeFromParent();
            });
        }
    }else{
#pragma mark ======spoon 倒冰 =============
        Rect rect = m_pCone->getBoundingBox();
        rect.origin.y += 370;
        rect.size.height = 400;
        rect.origin = m_pCone->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            m_pSpoon->index = 0;
            m_pGuideLayer->removeGuide();
            m_pSpoon->setTouchEnabled(false);
            m_pSpoon->setPosition(Vec2(m_pSnowConeNode->getPositionX()+100, m_pSnowConeNode->getPositionY()+m_pCone->getContentSize().height*0.8+50*m_nSnowPutCount));
            m_pSpoon->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                 CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pour_ice_cup.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/ice2.plist");
                
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                this->addChildToContentLayer(pParticle);
                pParticle->setLocalZOrder(10);
            }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                if (m_nSnowPutCount==0) {
                    m_pConeIce->setVisible(true);
                }else{
                    std::stringstream ostr;
                    ostr<<"ice0_"<<m_nSnowPutCount<<".png";
                    m_pConeIce->setTexture(localPath(ostr.str()));
                }
                m_pSpoon->getDragSprite()->setTexture(localPath("spoon0.png"));
                m_nSnowPutCount++;
                if (m_nSnowPutCount==4) {
                    m_pMachine->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
                    
                    m_pSpoon->runAction(Sequence::create(DelayTime::create(0.5),
                                                         MoveBy::create(0.5, Vec2(visibleSize.width, 100)),
                                                         CallFunc::create([=](){
//                        _moveawayCone();
//                        m_pGuideLayer->removeGuide();
                        _finish();
                        
                    }), NULL));
                }else{
                    m_pSpoon->setTouchEnabled(true);
                }
                
            }),
                                                 RotateTo::create(0.3, 1), NULL));
        }
    }
}
}

void SnowconeCrushIceScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    std::string name = pDragNode->getName();
    if ("ice_bag" == name) {
        Rect rect = m_pMachine->getBoxWorldRect();
        Rect iceRect = pDragNode->getDragSprite()->getBoundingBox();
        iceRect.origin = pDragNode->convertToWorldSpace(iceRect.origin);
        if (rect.intersectsRect(iceRect)) {
            m_pGuideLayer->removeGuide();
            pDragNode->setTouchEnabled(false);
            Vec2 pos = m_pMachine->getPosition()+Vec2(230, 500);
            pDragNode->setPosition(pos);
            int iceCount = 10;
            pDragNode->runAction(Sequence::create(RotateBy::create(0.3, -100),
                                                  CallFunc::create([=](){
            }),
                                                  DelayTime::create(0.4*iceCount+2.0),
                                                  CallFunc::create([=](){
                m_pGuideLayer->showGuideArrow(Vec2(m_pMachine->getCapWorldRect().getMaxX(), m_pMachine->getCapWorldRect().getMidY()),1);
                m_pMachine->setTouchEnable(true);
            }),
                                                  RotateTo::create(0.3, 0),
                                                  MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
            
            for (int i = 0; i<iceCount; i++) {
                this->runAction(Sequence::create(DelayTime::create(0.4+0.4*i),
                                                 CallFunc::create([=,&i](){
                    Sprite* pIce = Sprite::create(localPath("ice0.png"));
                    pIce->setScale(0.2);
                    pIce->setRotation(arc4random()%360);
                    m_pMachine->addIce(pIce, pDragNode->getWorldSpaceActionPoint());
                    pIce->runAction(ScaleTo::create(1, 0.6));
                    
                }), NULL));
            }
            MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_ice);
            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            
        }
    }
}

void SnowconeCrushIceScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void SnowconeCrushIceScene::onMachineStateCallback(int state)
{
    switch (state) {
        case MachineNode::eStateCapOpen:
        {
            m_pMachine->setTouchEnable(false);
            m_pGuideLayer->removeGuide();
            
            _showIceBag();
            
            Vec2 pos = Vec2(m_pMachine->getCapWorldRect().getMidX(),m_pMachine->getCapWorldRect().getMidY());
            m_pGuideLayer->showGuideMove(m_pIceBag->getPosition(),pos+Vec2(50, 10));
            m_pMachine->setButtonEnable(false);
        }
            break;
        case MachineNode::eStateCapClose:
        {
            m_pMachine->setTouchEnable(false);
            m_pGuideLayer->removeGuide();
//            m_pMachine->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 100)),
//                                                   CallFunc::create([=](){
                m_pGuideLayer->showGuideTap(Vec2(m_pMachine->getPositionX(), m_pMachine->getPositionY()+50));
                m_pMachine->setButtonEnable(true);
//            }), NULL));
        }
            break;
        case MachineNode::eStateTurnOn:
        {
            AudioHelp::getInstance()->playEffect("vo_snowcone_machine_works.mp3");
            m_pGuideLayer->removeGuide();
            m_pMachine->setButtonEnable(false);
        }
            break;
        case MachineNode::eStateTurnOff:
        {
            m_pGuideLayer->removeGuide();
            m_pMachine->setButtonEnable(false);
        }
            break;
        case MachineNode::eStateFinish:
        {
            m_pGuideLayer->removeGuide();
            m_pMachine->setButtonEnable(false);
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                m_pMachine->runAction(ScaleTo::create(0.5, 1.2));
                m_pMachine->separateBox();
            }),
                                             DelayTime::create(1.8),
                                             CallFunc::create([=](){
                _finish();
//                m_pMachine->runAction(Spawn::create(JumpTo::create(1, CMVisibleRect::getPosition(320, 1000), 350, 1),
//                                                    ScaleTo::create(0.8, 1), NULL));
//                setExPath("content/make/snowcone/2/");
//                _showCone();
            }), NULL));
        }
            break;
            
        default:
            break;
    }
}

#pragma mark - initData
void SnowconeCrushIceScene::_initData()
{
    setExPath("content/make/snowcone/1/");
}

void SnowconeCrushIceScene::_showMachine()
{
    m_pMachine = MachineNode::create();
    m_pMachine->onStateCallback = CC_CALLBACK_1(SnowconeCrushIceScene::onMachineStateCallback, this);
    this->addChildToContentLayer(m_pMachine);
    CMVisibleRect::setPosition(m_pMachine, Vec2(320, 480-visibleSize.height));
    m_pMachine->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                           CallFunc::create([=](){
        m_pMachine->setTouchEnable(true);
//        Vec2 pos = Vec2(m_pMachine->getCapWorldRect().getMidX(),m_pMachine->getCapWorldRect().getMidY());
//        m_pGuideLayer->showGuideMove(pos+Vec2(50, 10), pos);
        m_pGuideLayer->showGuideArrow(Vec2(m_pMachine->getCapWorldRect().getMidX(), m_pMachine->getCapWorldRect().getMidY()),-1);
    }), NULL));
    m_pMachine->setLocalZOrder(1);
}
void SnowconeCrushIceScene::_showIceBag()
{
    if (!m_pIceBag) {
        m_pIceBag = _createDrageNode(localPath("ice_bag.png"));
        this->addChildToContentLayer(m_pIceBag);
    }
    m_pIceBag->setName("ice_bag");
//    m_pIceBag->setLocalZOrder(1);
    m_pIceBag->setTouchEnabled(false);
    m_pIceBag->setActionPoint(Vec2(20, 182));
    CMVisibleRect::setPositionAdapted(m_pIceBag, 550+visibleSize.width, 760);

    AudioHelp::getInstance()->playEffect("vo_snowcone_ice_cubes.mp3");
    this->runAction(Sequence::create(DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_snowcone_pour_ice_cubes.mp3");
    }), NULL));
    m_pIceBag->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pIceBag->setOrgPositionDefault();
        m_pIceBag->setTouchEnabled(true);
        Vec2 pos = Vec2(m_pMachine->getCapWorldRect().getMaxX(),m_pMachine->getCapWorldRect().getMidY());
        m_pGuideLayer->showGuideMove(m_pIceBag->getPosition(), pos+Vec2(50, 10));
    }), NULL));
}

void SnowconeCrushIceScene::_showCone(Vec2 posDelt)
{
    if (!m_pSnowConeNode){
        m_pSnowConeNode = Node::create();
        this->addChildToContentLayer(m_pSnowConeNode);
    }
    CMVisibleRect::setPosition(m_pSnowConeNode, 320+posDelt.x, 20+posDelt.y-visibleSize.height);
    
    if (!m_pCone) {
        m_pCone = Sprite::create(localPath("cone0_0.png"));
        m_pSnowConeNode->addChild(m_pCone);
    }
    
    if (!m_pConeFront){
        m_pConeFront = Sprite::create(localPath("cone0_1.png"));
        m_pSnowConeNode->addChild(m_pConeFront);
        m_pConeFront->setLocalZOrder(1);
    }
    
    if (!m_pConeIce){
        m_pConeIce = Sprite::create(localPath("ice0.png"));
        m_pSnowConeNode->addChild(m_pConeIce);
        m_pConeIce->setVisible(false);
    }
    m_pConeIce->setPosition(Vec2(0, 360-10));
    
    if (!m_pSpoon){
        m_pSpoon = _createDrageNode(localPath("spoon.png"));
        this->addChildToContentLayer(m_pSpoon);
        CMVisibleRect::setPosition(m_pSpoon, 320+visibleSize.width, 560);
        m_pSpoon->setTouchEnabled(false);
        m_pSpoon->setLocalZOrder(5);
        m_pSpoon->setActionPoint(Vec2(18, 24));
    }
    m_pSpoon->setName("spoon");
    m_pSpoon->index = 0;
    
    m_pSnowConeNode->runAction(Sequence::create(DelayTime::create(2),
                                                MoveBy::create(0.8, Vec2(0, visibleSize.height)), NULL));
    
    if (m_nSnowPutCount==0) {
        m_pSpoon->runAction(Sequence::create(DelayTime::create(2),
                                             MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                             CallFunc::create([=](){
            m_pSpoon->setTouchEnabled(true);
            m_pSpoon->setOrgPositionDefault();
            m_pGuideLayer->showGuideMove(m_pSpoon->getPosition(), Vec2(visibleSize.width/2, visibleSize.height*0.8));
        }), NULL));
    }
}

void SnowconeCrushIceScene::_moveawayCone()
{
    if (m_pSnowConeNode) {
        m_pSnowConeNode->runAction(MoveBy::create(0.8, Vec2(0, -visibleSize.height)));
    }
}

void SnowconeCrushIceScene::_pourIce()
{
    
}
void SnowconeCrushIceScene::_crushIce()
{
    
}

void SnowconeCrushIceScene::_finish()
{
    
    MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_machine);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<SnowconeAddSlushScene>();
    }), NULL));
}