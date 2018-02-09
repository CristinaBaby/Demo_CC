
#include "SnowconeSculptScene.h"
#include "SceneManager.h"

SnowconeSculptScene::SnowconeSculptScene()
{
    m_pKnife = nullptr;
    m_pSnowConeNode = nullptr;
    m_pCone = nullptr;
    m_pConeFront = nullptr;
    m_pConeIce = nullptr;
    m_pConeIceShape = nullptr;
    m_pConeIceTemp = nullptr;
    m_nMoveCount = 0;
}

SnowconeSculptScene::~SnowconeSculptScene()
{
    
}
bool SnowconeSculptScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg3.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGameUI->showNormalLayout();
    
    _showCone();
    _showKnife();
    
    return true;
}

void SnowconeSculptScene::onEnter()
{
    ExtensionScene::onEnter();
}

void SnowconeSculptScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void SnowconeSculptScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void SnowconeSculptScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    Rect rect = m_pConeIce->getBoundingBox();
    rect.origin = m_pConeIce->getParent()->convertToWorldSpace(rect.origin);
    if (rect.containsPoint(worldPoint)) {
        if (pDragNode->index == 0) {
            pDragNode->index = 1;
            if (rect.getMidX()>worldPoint.x) {
                m_bBeginLeft = true;
                m_pConeIce->setTexture(localPath("shape"+std::to_string(m_nShapeIndex)+"_left1.png"));
                pDragNode->runAction(RotateBy::create(0.5, -60));
            }else{
                m_bBeginLeft = false;
                m_pConeIce->setTexture(localPath("shape"+std::to_string(m_nShapeIndex)+"_right1.png"));
                pDragNode->runAction(RotateBy::create(0.5, -60));
            }
            m_pGuideLayer->removeGuide();
        }else{
            m_nMoveCount++;
            AudioHelp::getInstance()->playLoopEffect("scrape_ice.mp3");
            static int count = 300;
            
            for (int i = 0; i<3; i++) {
                ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("shaveice"+std::to_string(i));
                if (!pParticle) {
                    pParticle = ParticleSystemQuad::create("particle/shaveice.plist");
                    pParticle->setName("shaveice"+std::to_string(i));
                    pParticle->setTexture(Sprite::create(localPath("ice_crush"+std::to_string(i)+".png"))->getTexture());
                    this->addChildToContentLayer(pParticle);
                    pParticle->setLocalZOrder(9);
                }
                pParticle ->setPosition(worldPoint);
            }
            if (m_pConeIceTemp) {
                int opacity = m_pConeIceTemp->getOpacity()-4;
                m_pConeIceTemp->setOpacity(opacity<=0?0:opacity);
            }
            if(m_nMoveCount%count==0){
                int step = m_nMoveCount/count;
//                if (!m_pConeIceTemp) {
//                    if (step==1) {
//                    }else if (step==2) {
//                        if (m_bBeginLeft) {
//                            m_pConeIceTemp = Sprite::create("shape"+std::to_string(m_nShapeIndex)+"_left"+std::to_string(step+1)+".png");
//                        }else{
//                            m_pConeIceTemp = Sprite::create("shape"+std::to_string(m_nShapeIndex)+"_right"+std::to_string(step+1)+".png");
//                        }
//                    }
//                }
                
                m_pConeIceTemp->setOpacity(0);
                if (step<3) {
                    m_pConeIceTemp->setTexture(m_pConeIce->getTexture());
                    if (step==1) {
                        if (m_bBeginLeft) {
                            m_pConeIce->setTexture(localPath("shape"+std::to_string(m_nShapeIndex)+"_left"+std::to_string(step+1)+".png"));
                        }else{
                            m_pConeIce->setTexture(localPath("shape"+std::to_string(m_nShapeIndex)+"_right"+std::to_string(step+1)+".png"));
                        }
                    }else {
                        m_pConeIce->setTexture(localPath("shape"+std::to_string(m_nShapeIndex)+".png"));
                    }
                    m_pConeIceTemp->setOpacity(255);
                }
                if (step==3){
                    m_pConeIce->setVisible(false);
                    m_pLine->removeFromParent();
                    AudioHelp::getInstance()->stopLoopEffect();
                    _finish();
                }
            }
        }
    }
}

void SnowconeSculptScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    for (int i = 0; i<3; i++) {
        ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("shaveice"+std::to_string(i));
        if (pParticle) {
            pParticle->stopSystem();
            pParticle->removeFromParent();
        }
    }
    AudioHelp::getInstance()->stopLoopEffect();
}

void SnowconeSculptScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void SnowconeSculptScene::_initData()
{
    setExPath("content/make/snowcone/3/");
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
}

void SnowconeSculptScene::_showCone()
{
    if (!m_pSnowConeNode){
        m_pSnowConeNode = Node::create();
        this->addChildToContentLayer(m_pSnowConeNode);
    }
    CMVisibleRect::setPosition(m_pSnowConeNode, 320-50, 20-visibleSize.height);
    
    if (!m_pCone) {
        m_pCone = Sprite::create(localRootPath("2/cone0_0.png"));
        m_pSnowConeNode->addChild(m_pCone);
    }
    
    if (!m_pConeFront){
        m_pConeFront = Sprite::create(localRootPath("2/cone0_1.png"));
        m_pSnowConeNode->addChild(m_pConeFront);
        m_pConeFront->setLocalZOrder(1);
    }
    
    if (!m_pConeIceShape){
        m_pConeIceShape = Sprite::create(localPath("shape"+std::to_string(m_nShapeIndex)+".png"));
        m_pSnowConeNode->addChild(m_pConeIceShape);
    }
    m_pConeIceShape->setPosition(Vec2(-5, 360+10));
    
    if (!m_pConeIce){
        m_pConeIce = Sprite::create(localPath("ice.png"));
        m_pSnowConeNode->addChild(m_pConeIce);
    }
    m_pConeIce->setPosition(Vec2(-5, 360+10));
    
    m_pConeIceTemp = Sprite::create(localPath("ice.png"));
    m_pSnowConeNode->addChild(m_pConeIceTemp);
    m_pConeIceTemp->setPosition(m_pConeIce->getPosition());
    
    m_pLine = Sprite::create(localPath("line_shape"+std::to_string(m_nShapeIndex)+".png"));
    m_pSnowConeNode->addChild(m_pLine);
    m_pLine->setLocalZOrder(10);
    m_pLine->setPosition(m_pConeIce->getPosition());
    m_pLine->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.5),
                                                              DelayTime::create(2),
                                                              FadeOut::create(0.5),
                                                              DelayTime::create(1), NULL)));
    m_pSnowConeNode->runAction(Sequence::create(DelayTime::create(2),
                                                MoveBy::create(1, Vec2(0, visibleSize.height)), NULL));
    
}

void SnowconeSculptScene::_showKnife()
{
//    int radom = arc4random()%100;
    if (!m_pKnife){
        m_pKnife = _createDrageNode(localPath("cutter.png"));
        this->addChildToContentLayer(m_pKnife);
        m_pKnife->setTouchEnabled(false);
        m_pKnife->setLocalZOrder(5);
        m_pKnife->setActionPoint(Vec2(16, 474));
    }
    m_pKnife->setName("knife");
    m_pKnife->index = 0;
    m_pKnife->setLimitRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    
//    if (radom<50) {
//        CMVisibleRect::setPositionAdapted(m_pKnife, 600+visibleSize.width, 600);
//    }else{
//        m_pKnife->getDragSprite()->setScaleX(-1);
//        CMVisibleRect::setPositionAdapted(m_pKnife, 60-visibleSize.width, 600);
//    }
    CMVisibleRect::setPositionAdapted(m_pKnife, 600+visibleSize.width, 600);
    m_pKnife->runAction(Sequence::create(DelayTime::create(2),
//                                         MoveBy::create(1, Vec2(radom<50?-visibleSize.width:visibleSize.width, 0)),
                                         MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_snowcone_knife_cut.mp3");
        m_pKnife->setTouchEnabled(true);
        m_pKnife->setOrgPositionDefault();
        m_pGuideLayer->showGuideMove(m_pKnife->getPosition(), m_pSnowConeNode->getPosition()+Vec2(0,300));
    }), NULL));
}

void SnowconeSculptScene::_finish()
{
    
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    m_pKnife->setTouchEnabled(false);
    m_pKnife->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width*1.5, 0)), NULL));
    
    for (int i = 0; i<3; i++) {
        ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("shaveice"+std::to_string(i));
        if (pParticle) {
            pParticle->stopSystem();
            pParticle->removeFromParent();
        }
    }
    
    m_pSnowConeNode->runAction(MoveBy::create(1, Vec2(5, 250)));
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(3),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<DecorateScene>();
    }), NULL));
}