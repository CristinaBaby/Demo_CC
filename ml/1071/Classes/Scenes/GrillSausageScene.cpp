

#include "GrillSausageScene.h"
#include "SceneManager.h"

GrillSausageScene::GrillSausageScene()
{
    m_pPlateRight = nullptr;
}

GrillSausageScene::~GrillSausageScene()
{
    
}
bool GrillSausageScene::init()
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
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_grill_sausages.mp3");
        m_pGuideLayer->showGuideMove(m_pPlateLeft->getPosition(), m_pGrill->getPosition());
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_not_burned.mp3");
    }), NULL));
    
    AudioHelp::getInstance()->playBackGroundMusic("bg_make.mp3");
    m_pGameUI->showNormalLayout();
    return true;
}

void GrillSausageScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_NORMAL);
}

void GrillSausageScene::onExit()
{
    ExtensionScene::onExit();
}

void GrillSausageScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void GrillSausageScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->setLocalZOrder(20);
    pDragNode->getDragSprite()->stopAllActions();
    pDragNode->getDragSprite()->setScale(1);
    if (pDragNode->index==0) {
        
    }else{
        ProgressBarNew* pBar = (ProgressBarNew*)pDragNode->getUserData();
        if(pBar){
            pBar->pauseProgress();
        }
        
        pDragNode->stopAllActions();
    }
}
void GrillSausageScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void GrillSausageScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    if (pDragNode->index==0) {
        //    100,76,385,255
        Rect doughRect = m_pGrill->getBoundingBox();
        doughRect.origin = m_pGrill->getParent()->convertToWorldSpace(doughRect.origin);
        
        Rect rect = pDragNode->getDragSprite()->getBoundingBox();
        rect.origin = pDragNode->convertToWorldSpace(rect.origin);
        if (rect.intersectsRect(doughRect)) {
//            AudioHelp::getInstance()->playEffect("fall.mp3");
            int index = 0;
            int count = m_SausagePotMap.size();
            for (int i = 0; i<3; i++) {
                if (!m_bFull[i]) {
                    break;
                }
                index++;
            }
            m_bFull[index] = true;
            pDragNode->flag = index;
            Vec2 pos = Vec2(270.0+15.0*(index%3),80.0+80.0*(index%3));
            pos = m_pGrill->convertToWorldSpace(pos);
            
            m_SausagePotMap.insert(make_pair(pDragNode->getTag(),pDragNode));;
            pDragNode->stopAllActions();
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                  CallFunc::create([=](){
                pDragNode->setOrgPositionDefault();
                _doFry(pDragNode);
            }), NULL));
        }else{
            pDragNode->back(pDragNode->getTag());
            pDragNode->getDragSprite()->runAction(ScaleTo::create(0.5, 0.5));
        }
    }else{
        Rect plateRect = m_pPlateRight->getBoundingBox();
        if (plateRect.containsPoint(pDragNode->getPosition())) {
            AudioHelp::getInstance()->playEffect("fall.mp3");
            _finishFry(pDragNode);
        }else{
            pDragNode->back(0,[=](){
                ProgressBarNew* pBar = (ProgressBarNew*)pDragNode->getUserData();
                if(pBar){
                    pBar->resumeProgress();
                    pBar->setVisible(false);
                }
                pDragNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(10, 10)),
                                                                            MoveBy::create(0.1, Vec2(-10, -10)), NULL)));
            });
        }
    }
}
void GrillSausageScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}
#pragma mark - initData
void GrillSausageScene::_initData()
{
    setExPath("content/make/normal/");
    m_nNextSceneTag = GameUIEvent::eSceneTagCutBread;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagCutBread;
    std::memset(m_bFull, 0, 3);
}


void GrillSausageScene::_showPlate()
{
    auto action = Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pDrag = dynamic_cast<DragNode*>(pNode);
        if (pDrag) {
            pDrag->setOrgPositionDefault();
            pDrag->setTouchEnabled(true);
        }
    }), NULL);
    m_pPlateLeft = Sprite::create(localPath("make1/plate.png"));
    this->addChildToContentLayer(m_pPlateLeft);
    CMVisibleRect::setPositionAdapted(m_pPlateLeft, 170-visibleSize.width, 680);
    m_pPlateLeft->runAction(action);
    
    for (int i = 0; i<3; i++) {
        DragNode* pSausage = _createDrageNode(localPath("make1/sausage1.png"));
        this->addChildToContentLayer(pSausage);
        pSausage->setTag(i);
        pSausage->setLocalZOrder(i);
        pSausage->index = 0;
        CMVisibleRect::setPositionAdapted(pSausage, 170-visibleSize.width, 670+30*(i%3));
        m_SausageLeftVector.push_back(pSausage);
        pSausage->getDragSprite()->setScale(0.5);
        pSausage->runAction(action->clone());
        
    }
    
    m_pGrill = Sprite::create(localPath("make1/grill_2.png"));
    this->addChildToContentLayer(m_pGrill);
    CMVisibleRect::setPositionAdapted(m_pGrill, 320, 380);
    
    m_pGrill2 = Sprite::create(localPath("make1/grill_1.png"));
    this->addChildToContentLayer(m_pGrill2);
    m_pGrill2->setPosition(m_pGrill->getPosition());
    m_pGrill2->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1),
                                                                FadeOut::create(1.5), NULL)));
}

void GrillSausageScene::_showPlateRight()
{
    if (m_pPlateRight) {
        return;
    }
    m_pPlateRight = Sprite::create(localPath("make1/plate.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPositionAdapted(m_pPlateRight, 480+visibleSize.width, 680);
    m_pPlateRight->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    
}
void GrillSausageScene::_doFry(DragNode* pNode)
{
    AudioHelp::getInstance()->playLoopEffect("grill.mp3");
    
    ProgressBarNew* pBar = ProgressBarNew::create("content/common/progress/progress.png", "content/common/progress/progress_bar.png", "content/common/progress/p_mask.png");
    pBar->setFlag(70, 104, 4, 3, 3);
    pBar->setColorSegment(Color3B(255, 180, 0), Color3B(50, 228, 71), Color3B(228, 71, 50));
    pBar->targetNode = pNode;
    pBar->progressNewState = CC_CALLBACK_2(GrillSausageScene::_onProgressCallback, this);
    this->addChildToContentLayer(pBar);
    pBar->setPosition(pNode->getPosition()+Vec2(0, 80));
    pBar->setLocalZOrder(15);
//    pBar->beginProgress(40);
    pNode->setUserData(pBar);
    pNode->setLocalZOrder(1);
    pBar->setVisible(false);
    pBar->runAction(Sequence::create(DelayTime::create(0.1+1.5*m_SausagePotMap.size()),
                                     CallFunc::create([=](){
        pBar->beginProgress(40);
    }), NULL));
    
    pNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                            MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
}

void GrillSausageScene::_finishFry(DragNode* pNode)
{
    m_bFull[pNode->flag] = false;
    m_SausagePotMap.erase(pNode->getTag());
    int count = m_SausageRightVector.size();
    
    pNode->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(pNode, 480, 670+30*(count%4));
    m_SausageRightVector.push_back(pNode);
    
    pNode->getDragSprite()->setScale(0.5);
    if (count==2) {
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            SceneManager::replaceTheScene<CutBreadScene>();
        }), NULL));
    }
    if (m_SausagePotMap.size()==0) {
        AudioHelp::getInstance()->stopLoopEffect();
    }
}

void GrillSausageScene::_onProgressCallback(ProgressBarNew* pBar,int segment)
{
    if (segment<5) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->getDragSprite()->setScale(1);
            pDragNode->setDragSprite(Sprite::create(localPath("make1/sausage"+std::to_string(segment+1)+".png")));
            
            if (segment==4) {
                pDragNode->index = 1;
                pDragNode->setTouchEnabled(true);
                if(m_SausageRightVector.size()<3){
                    _showPlateRight();
                    this->runAction(Sequence::create(DelayTime::create(0.5),
                                                     CallFunc::create([=](){
                        m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPlateRight->getPosition());
                    }), NULL));
                    AudioHelp::getInstance()->playEffect("fry_ok.mp3");
                    
                    pDragNode->stopAllActions();
                    pDragNode->setPosition(pDragNode->getOrgPosition());
                    pDragNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(10, 10)),
                                                                                MoveBy::create(0.1, Vec2(-10, -10)), NULL)));
                }
            }
        }
    }else if (segment>6 && segment<10) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->setDragSprite(Sprite::create(localPath("make1/sausage"+std::to_string(segment-1)+".png")));
            if (segment==7) {
                pDragNode->index = 0;
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
                
                pDragNode->stopAllActions();
                pDragNode->getDragSprite()->setScale(1);
                
                pDragNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                                            MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
                
                bool isPlay = AudioHelp::getInstance()->playFryBurnedEffect();
                if (isPlay) {
                    this->runAction(Sequence::create(DelayTime::create(4),
                                                     CallFunc::create([=](){
                        AudioHelp::getInstance()->stopFryBurnedEffect();
                    }), NULL));
                }
            }
        }
    }else if (segment == 10) {
        AudioHelp::getInstance()->playEffect("fry_failed.mp3");
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode){
            pDragNode->getDragSprite()->runAction(FadeOut::create(0.5));
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke2.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(9);
            pParticle->setPosition(pDragNode->getPosition());
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setLocalZOrder(9);
                pParticle->setPosition(pDragNode->getPosition()-Vec2(0, 30));
                
                int tag = pDragNode->getTag();
                CMVisibleRect::setPositionAdapted(pDragNode, 170, 670+30*(tag%3)+visibleSize.height);
                pDragNode->setDragSprite(Sprite::create(localPath("make1/sausage1.png")));
                
                pDragNode->stopAllActions();
                pDragNode->getDragSprite()->setScale(0.5);
                pDragNode->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                                      CallFunc::create([=](){
                    pDragNode->setOrgPositionDefault();
                    pDragNode->setTouchEnabled(true);
                    pDragNode->setLocalZOrder(tag);
                }), NULL));
                
                m_bFull[pDragNode->flag] = false;
                m_SausagePotMap.erase(tag);
                if (m_SausagePotMap.size()==0) {
                    AudioHelp::getInstance()->stopLoopEffect();
                }
            }), NULL));
        }
    }
}