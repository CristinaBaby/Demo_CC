

#include "CutBreadScene.h"
#include "SceneManager.h"

CutBreadScene::CutBreadScene()
{
    m_pPlateRight = nullptr;
    m_pKnife = nullptr;
    m_pCurCutBread = nullptr;
    m_nMoveCount = 0;
}

CutBreadScene::~CutBreadScene()
{
    
}
bool CutBreadScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create("content/make/bg/bg1.jpg");
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    _showPlate();
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_cut_bread.mp3");
        m_pGuideLayer->showGuideMove(m_pPlateLeft->getPosition(), m_pBoard->getPosition());
    }),  NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void CutBreadScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CutBreadScene::onExit()
{
    ExtensionScene::onExit();
}

void CutBreadScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CutBreadScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->stopAllActions();
    pDragNode->setLocalZOrder(20);
    pDragNode->setScale(1);
    if (pDragNode->index==0) {
        
    }else{
    }
    _hideShadow(pDragNode);
}
void CutBreadScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="knife") {
        if (m_pCurCutBread) {
            if (m_pCurCutBread->index==0) {
                Rect rect = m_pCurCutBread->getDragSprite()->getBoundingBox();
                rect.origin = m_pCurCutBread->convertToWorldSpace(rect.origin);
                if (rect.containsPoint(worldPoint)) {
                    
                    m_nMoveCount++;
                    if (m_nMoveCount%20==0) {
                        m_pCurCutBread->index=1;
                        m_nMoveCount = 0;
                        AudioHelp::getInstance()->playEffect("cut.mp3");
                        pDragNode->setTouchEnabled(false);
                        Vec2 pos = m_pCurCutBread->getPosition();
                        pos.y = m_pCurCutBread->getPositionY()-50;
                        pos.x += m_pCurCutBread->getContentSize().width*0.8;
                        pDragNode->setPosition(pos);
                        int opacity = m_pCrumbs->getOpacity()+90;
                        m_pCrumbs->setOpacity(opacity>255?255:opacity);
                        
                        Sprite* pTempBread = Sprite::create(localPath("make2/bread1_2.png"));
                        this->addChildToContentLayer(pTempBread);
                        pTempBread->setPosition(m_pCurCutBread->getPosition());
                        pTempBread->setLocalZOrder(15);
                        m_pCurCutBread->setLocalZOrder(2);
                        pDragNode->setLocalZOrder(5);
                        pDragNode->runAction(Sequence::create(MoveBy::create(2, Vec2(-m_pCurCutBread->getContentSize().width, 0)),
                                                              CallFunc::create([=](){
                            pTempBread->removeFromParent();
                            m_pCurCutBread->getDragSprite()->setTexture(localPath("make2/bread_2.png"));
                            _finishCut();
                        }), NULL));
                        m_pCurCutBread->index=1;
                    }
                    
                    m_pGuideLayer->removeGuide();
                }
            }
        }
    }
    
}
void CutBreadScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="knife") {
//        if (m_pCurCutBread) {
//            if (m_pCurCutBread->index==0) {
//                
//            }else{                
//                AudioHelp::getInstance()->stopLoopEffect();
//                m_pKnife->setTouchEnabled(false);
//                m_pGuideLayer->removeGuide();
//                _finishCut();
//            }
//        }
    }else if (name=="bread"){
        m_pGuideLayer->removeGuide();
        if (pDragNode->index==0) {
            Rect doughRect = m_pBoard->getBoundingBox();
            doughRect.origin = m_pBoard->getParent()->convertToWorldSpace(doughRect.origin);
            
            Rect rect = pDragNode->getDragSprite()->getBoundingBox();
            rect.origin = pDragNode->convertToWorldSpace(rect.origin);
            int index = m_BreadBoardMap.size();
            if (rect.intersectsRect(doughRect) && index==0) {
                AudioHelp::getInstance()->playEffect("fall.mp3");
                Vec2 pos = m_pBoard->getPosition();
                
                m_BreadBoardMap.insert(make_pair(pDragNode->getTag(),pDragNode));;
                pDragNode->setTouchEnabled(false);
                _showShadow(pDragNode);
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                      CallFunc::create([=](){
                    pDragNode->setOrgPositionDefault();
                    m_pGuideLayer->showGuide(localPath("make2/bread_marking.png"), pos);
                    pDragNode->setLocalZOrder(5);
                }), NULL));
                if (m_BreadRightVector.size()==0) {
                    _showKnife();
                }
                m_pCurCutBread = pDragNode;
            }else{
                pDragNode->setTouchEnabled(false);
                pDragNode->back(pDragNode->getTag(),
                                [=](){
                                    _showShadow(pDragNode);
                                    pDragNode->setTouchEnabled(true);
                                });
                pDragNode->runAction(ScaleTo::create(0.5, 0.5));
            }
        }else{
            Rect plateRect = m_pPlateRight->getBoundingBox();
            if (plateRect.containsPoint(pDragNode->getPosition())) {
                AudioHelp::getInstance()->playEffect("fall.mp3");
                int addCount = m_BreadRightVector.size();
                Vec2 pos = Vec2(480,730.0-40.0*(addCount%3));
                pDragNode->setTouchEnabled(false);
                pDragNode->setScale(0.5);
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(pos)),
                                                      CallFunc::create([=](){
                    _showShadow(pDragNode);
                }), NULL));
                
                m_BreadBoardMap.erase(pDragNode->getTag());
                
                m_BreadRightVector.push_back(pDragNode);
                m_pCurCutBread = nullptr;
                m_pKnife->setTouchEnabled(true);
                if (m_BreadRightVector.size()>=3) {
                    AudioHelp::getInstance()->playEffect("done.mp3");
                    AudioHelp::getInstance()->_playEffectNiceWork();
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(10);
                    pParticle->setPosition(visibleSize*0.5);
                    
                    this->runAction(Sequence::create(DelayTime::create(2),
                                                     CallFunc::create([=](){
                        SceneManager::replaceTheScene<GrillBreadScene>();
                    }), NULL));
                }
            }else{
                pDragNode->setTouchEnabled(false);
                pDragNode->back(pDragNode->getTag(),
                                [=](){
                                    _showShadow(pDragNode);
                                    pDragNode->setTouchEnabled(true);
                                });
            }
        }
    }
}
void CutBreadScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}
#pragma mark - initData
void CutBreadScene::_initData()
{
    setExPath("content/make/normal/");
    m_nNextSceneTag = GameUIEvent::eSceneTagGrillBread;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagGrillBread;
}
void CutBreadScene::_hideShadow(DragNode* pNode)
{
    std::string name = pNode->getName();
    if (name=="knife") {
        Node* pShadow = pNode->getChildByName("shadow");
        pShadow->setVisible(false);
    }else if (name=="bread"){
        if (pNode->index==0) {
            pNode->getDragSprite()->setTexture(localPath("make2/bread1_1.png"));
        }else{
            pNode->getDragSprite()->setTexture(localPath("make2/bread2_1.png"));
        }
    }
}

void CutBreadScene::_showShadow(DragNode* pNode)
{
    std::string name = pNode->getName();
    if (name=="knife") {
        Node* pShadow = pNode->getChildByName("shadow");
        pShadow->setVisible(true);
    }else if (name=="bread"){
        if (pNode->index==0) {
            pNode->getDragSprite()->setTexture(localPath("make2/bread_1.png"));
        }else{
            pNode->getDragSprite()->setTexture(localPath("make2/bread_2.png"));
        }
    }
}

void CutBreadScene::_showPlate()
{
    auto action = Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pDrag = dynamic_cast<DragNode*>(pNode);
        if (pDrag) {
            pDrag->setOrgPositionDefault();
            pDrag->setTouchEnabled(true);
        }
    }), NULL);
    m_pPlateLeft = Sprite::create(localPath("make2/plate.png"));
    this->addChildToContentLayer(m_pPlateLeft);
    CMVisibleRect::setPositionAdapted(m_pPlateLeft, 170-visibleSize.width, 680);
    m_pPlateLeft->runAction(action);
    
    for (int i = 0; i<3; i++) {
        DragNode* pBread = _createDrageNode(localPath("make2/bread_1.png"));
        this->addChildToContentLayer(pBread);
        pBread->setTag(i);
        pBread->setLocalZOrder(i);
        pBread->index = 0;
        pBread->setName("bread");
        CMVisibleRect::setPositionAdapted(pBread, 170-visibleSize.width, 730-40*(i%3));
        m_BreadLeftVector.push_back(pBread);
        pBread->setScale(0.5);
        pBread->runAction(action->clone());
        
    }
    
    m_pBoard = Sprite::create(localPath("make2/cutting_board.png"));
    this->addChildToContentLayer(m_pBoard);
    CMVisibleRect::setPositionAdapted(m_pBoard, 320, 380);
    
    m_pCrumbs = Sprite::create(localPath("make2/bread_crumbs.png"));
    this->addChildToContentLayer(m_pCrumbs);
    CMVisibleRect::setPositionAdapted(m_pCrumbs, 320, 380);
    m_pCrumbs->setOpacity(0);
}

void CutBreadScene::_showPlateRight()
{
    if (m_pPlateRight) {
        return;
    }
    m_pPlateRight = Sprite::create(localPath("make2/plate.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPositionAdapted(m_pPlateRight, 480+visibleSize.width, 680);
    m_pPlateRight->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
}

void CutBreadScene::_showKnife()
{
    if (m_pKnife) {
        return;
    }
    m_pKnife = _createDrageNode(localPath("make2/knife.png"));
    this->addChildToContentLayer(m_pKnife);
    CMVisibleRect::setPositionAdapted(m_pKnife, 600+visibleSize.width, 470);
    m_pKnife->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pKnife->setOrgPositionDefault();
    }), NULL));
    m_pKnife->setLocalZOrder(1);
    m_pKnife->setName("knife");
    m_pKnife->setActionPoint(Vec2(20, 164));
    
    Sprite* pShadow = Sprite::create(localPath("make2/knife_shadow.png"));
    m_pKnife->addChild(pShadow,-1);
    pShadow->setName("shadow");
    
    m_pRag = Sprite::create(localPath("make2/napkin.png"));
    this->addChildToContentLayer(m_pRag);
    CMVisibleRect::setPositionAdapted(m_pRag, 600+visibleSize.width, 470+300);
    m_pRag->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, -300)));
}


void CutBreadScene::_finishCut()
{
    _showPlateRight();
    m_pKnife->back(1,[=](){
        _showShadow(m_pKnife);
    });
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        if(m_BreadRightVector.size()==0){
            m_pGuideLayer->showGuideMove(m_pCurCutBread->getPosition(), m_pPlateRight->getPosition());
        }
        m_pCurCutBread->setTouchEnabled(true);
    }), NULL));
}