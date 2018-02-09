
#include "SnowconeAddSlushScene.h"
#include "SceneManager.h"

SnowconeAddSlushScene::SnowconeAddSlushScene()
{
    m_nSnowPutCount = 0;
    m_pSpoon = nullptr;
    m_pSnowConeNode = nullptr;
    m_pCone = nullptr;
    m_pConeFront = nullptr;
    m_pConeIce = nullptr;
}

SnowconeAddSlushScene::~SnowconeAddSlushScene()
{
    
}
bool SnowconeAddSlushScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg2.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    
    _showBox();
    _showCone();
    
    return true;
}

void SnowconeAddSlushScene::onEnter()
{
    ExtensionScene::onEnter();
}

void SnowconeAddSlushScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void SnowconeAddSlushScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void SnowconeAddSlushScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if ("spoon" == name){
#pragma mark ======spoon 舀冰 =============
        if (0 == m_pSpoon->index) {
            Rect rect = m_pIcingFront->getBoundingBox();
            rect.origin = m_pIcingFront->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                m_pSpoon->index = 1;
                m_pSpoon->setTouchEnabled(false);
                m_pSpoon->setLocalZOrder(5);
                AudioHelp::getInstance()->playEffect("scoopup_ice.mp3");
                
                m_pSpoon->setPosition(m_pBox->getPosition()+Vec2(50, 80));
                m_pSpoon->runAction(Sequence::create(RotateBy::create(0.3, -30),
                                                     RotateBy::create(0.3, 30),
                                                     RotateBy::create(0.3, -30),
                                                     RotateBy::create(0.3, 30),
                                                     CallFunc::create([=](){
                    m_pIcingFront->setScaleY(m_pIcingFront->getScaleY()-0.1);
                    m_pIcing->setScaleY(m_pIcing->getScaleY()-0.1);
                    Sprite* pSpoonIn = (Sprite*)m_pSpoon->getChildByName("spoonin");;
                    if (!pSpoonIn) {
                        pSpoonIn = Sprite::create(localPath("spoon_ice.png"));
                        pSpoonIn->setName("spoonin");
                        m_pSpoon->addChild(pSpoonIn);
                    }
                    pSpoonIn->setOpacity(255);
                    
                    m_pSpoon->setTouchEnabled(true);
                    m_pSpoon->setLocalZOrder(20);
                    m_pGuideLayer->showGuideMove(m_pSpoon->getPosition(), m_pSnowConeNode->getPosition());
                }), NULL));
            }
        }else if (1 == m_pSpoon->index){
#pragma mark ======spoon 倒冰 =============
            Rect rect = m_pCone->getBoundingBox();
            rect.origin.y += 370;
            rect.size.height = 400;
            rect.origin = m_pCone->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                m_pSpoon->index = 0;
                m_pGuideLayer->removeGuide();
                m_pSpoon->setTouchEnabled(false);
                m_pSpoon->setPosition(Vec2(m_pSnowConeNode->getPositionX()+100, m_pSnowConeNode->getPositionY()+m_pCone->getContentSize().height*0.8+70*(m_nSnowPutCount+1)));
                m_pSpoon->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                     CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("pour_ice_cup.mp3");
                    
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/ice2.plist");
                    
                    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                    this->addChildToContentLayer(pParticle);
//                    if (m_nSnowPutCount == 0) {
//                        pParticle->setLocalZOrder(4);
//                        m_pSpoon->setLocalZOrder(3);
//                    }else{
//                        
//                    }
                    pParticle->setLocalZOrder(20);
                    
                    Sprite* pSpoonIn = (Sprite*)m_pSpoon->getChildByName("spoonin");;
                    if (pSpoonIn) {
                        pSpoonIn->runAction(FadeOut::create(2));
                    }
                }),
                                                     DelayTime::create(2),
                                                     CallFunc::create([=](){
                    if (m_nSnowPutCount==0) {
                        m_pConeIce->setVisible(true);
                    }else{
                        std::stringstream ostr;
                        ostr<<"ice"<<m_nSnowPutCount<<".png";
                        m_pConeIce->setTexture(localPath(ostr.str()));
                    }
                    m_pSnowConeNode->runAction(EaseSineOut::create(MoveBy::create(0.5, Vec2(0, -50))));
                    
                    m_nSnowPutCount++;
                    if (m_nSnowPutCount==4) {
                        auto action = Sequence::create(DelayTime::create(0.3),
                                                       MoveBy::create(1, Vec2(visibleSize.width,0)), NULL);
                        
                        m_pBox->runAction(action);
                        m_pBoxFront->runAction(action->clone());
                        m_pIcing->runAction(action->clone());
                        m_pIcingFront->runAction(action->clone());
                        
                        m_pSpoon->runAction(Sequence::create(DelayTime::create(0.5),
                                                             MoveBy::create(1, Vec2(visibleSize.width, 100)),
                                                             CallFunc::create([=](){
                            
                        }), NULL));
                        
                        m_pSnowConeNode->runAction(Sequence::create(DelayTime::create(1),
                                                                    EaseSineIn::create(MoveBy::create(1, Vec2(0, 300))),
                                                                    DelayTime::create(0.5),
                                                                    CallFunc::create([=](){
                            
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

void SnowconeAddSlushScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
}

void SnowconeAddSlushScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void SnowconeAddSlushScene::_initData()
{
    setExPath("content/make/snowcone/2/");
}

void SnowconeAddSlushScene::_showBox()
{
    m_pBox = Sprite::create(localPath("bbox0.png"));
    this->addChildToContentLayer(m_pBox);
    CMVisibleRect::setPositionAdapted(m_pBox, 320-visibleSize.width, 720);
    
    m_pBoxFront = Sprite::create(localPath("bbox1.png"));
    this->addChildToContentLayer(m_pBoxFront);
    m_pBoxFront->setPosition(m_pBox->getPosition());
    
    m_pIcing = Sprite::create(localPath("bice.png"));
    this->addChildToContentLayer(m_pIcing);
    m_pIcing->setPosition(m_pBox->getPosition()+Vec2(0,-10-m_pIcing->getContentSize().height*.5));
    
    m_pIcingFront = Sprite::create(localPath("bice2.png"));
    this->addChildToContentLayer(m_pIcingFront);
    m_pIcingFront->setPosition(m_pBox->getPosition()+Vec2(0,-10-m_pIcingFront->getContentSize().height*.5));
    
    m_pIcing->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    m_pIcingFront->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    
    m_pBoxFront->setLocalZOrder(10);
    m_pIcing->setLocalZOrder(1);
    m_pIcingFront->setLocalZOrder(6);
    
    auto action = Sequence::create(DelayTime::create(0.3),
                                   MoveBy::create(1, Vec2(visibleSize.width,0)), NULL);
    
    m_pBox->runAction(action);
    m_pBoxFront->runAction(action->clone());
    m_pIcing->runAction(action->clone());
    m_pIcingFront->runAction(action->clone());
}

void SnowconeAddSlushScene::_showCone(Vec2 posDelt)
{
    if (!m_pSnowConeNode){
        m_pSnowConeNode = Node::create();
        this->addChildToContentLayer(m_pSnowConeNode);
    }
    m_pSnowConeNode->setLocalZOrder(5);
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
    m_pConeIce->setPosition(Vec2(-5, 360+10));
    
    if (!m_pSpoon){
        m_pSpoon = _createDrageNode(localPath("spoon.png"));
        this->addChildToContentLayer(m_pSpoon);
        CMVisibleRect::setPosition(m_pSpoon, 500+visibleSize.width, 450);
        m_pSpoon->setTouchEnabled(false);
        m_pSpoon->setLocalZOrder(5);
        m_pSpoon->setActionPoint(Vec2(18, 24));
    }
    m_pSpoon->setName("spoon");
    m_pSpoon->index = 0;
    
    m_pSnowConeNode->runAction(Sequence::create(DelayTime::create(2),
                                                MoveBy::create(1, Vec2(0, visibleSize.height)), NULL));
    
    if (m_nSnowPutCount==0) {
        m_pSpoon->runAction(Sequence::create(DelayTime::create(2),
                                             MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                             CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_snowcone_scoop_ice.mp3");
            m_pSpoon->setTouchEnabled(true);
            m_pSpoon->setOrgPositionDefault();
            m_pGuideLayer->showGuideMove(m_pSpoon->getPosition(), m_pBox->getPosition());
        }), NULL));
    }
}

void SnowconeAddSlushScene::_finish()
{
    
//    MenuLayer::addIngredient(MenuLayer::ePackageSnowcone, MenuLayer::eSnowcone_ice);
//    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
                SceneManager::replaceTheScene<SnowconeShapScene>();
    }), NULL));
}