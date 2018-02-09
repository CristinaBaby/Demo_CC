

#include "FrySausageScene.h"
#include "RollSausageScene.h"
#include "SceneManager.h"

FrySausageScene::FrySausageScene()
{
    m_pPlateRight = nullptr;
}

FrySausageScene::~FrySausageScene()
{
    
}
bool FrySausageScene::init()
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
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    _showPlate();
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pPlateLeft->getPosition(), m_pPanOil->getPosition());
        AudioHelp::getInstance()->playEffect("vo_fry_sausage.mp3");
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_not_burned.mp3");
    }),   NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void FrySausageScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FrySausageScene::onExit()
{
    ExtensionScene::onExit();
}

void FrySausageScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FrySausageScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->setLocalZOrder(20);
    pDragNode->stopAllActions();
    pDragNode->setScale(1);
    if (pDragNode->index==0) {
        
    }else{
        ProgressBarNew* pBar = (ProgressBarNew*)pDragNode->getUserData();
        if(pBar){
            pBar->pauseProgress();
        }
        
        pDragNode->stopAllActions();
    }
}
void FrySausageScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void FrySausageScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->index==0) {
        //    100,76,385,255
        Rect doughRect = m_pPan->getBoundingBox();
        doughRect.origin = m_pPan->getParent()->convertToWorldSpace(doughRect.origin);
        
        Rect rect = pDragNode->getDragSprite()->getBoundingBox();
        rect.origin = pDragNode->convertToWorldSpace(rect.origin);
        if (rect.intersectsRect(doughRect)) {
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
            
            Vec2 pos = Vec2(440.0,180.0+80.0*(index%3));
            pos = m_pPan->convertToWorldSpace(pos);
            
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                  CallFunc::create([=](){
                pDragNode->setOrgPositionDefault();
                _doFry(pDragNode);
            }), NULL));
        }else{
            pDragNode->back(pDragNode->getTag());
//            pDragNode->runAction(ScaleTo::create(0.5, 0.5));
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
void FrySausageScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}
#pragma mark - initData
void FrySausageScene::_initData()
{
    setExPath("content/make/japanese/");
    m_nNextSceneTag = GameUIEvent::eSceneTagRollSausage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagRollSausage;
    std::memset(m_bFull, 0, 3);
}


void FrySausageScene::_showPlate()
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
        DragNode* pSausage = _createDrageNode(localPath("make3/1.png"));
        this->addChildToContentLayer(pSausage);
        pSausage->setTag(i);
        pSausage->setLocalZOrder(i);
        pSausage->index = 0;
        CMVisibleRect::setPositionAdapted(pSausage, 170-visibleSize.width, 720-40*(i%3));
        m_SausageLeftVector.push_back(pSausage);
//        pSausage->setScale(0.5);
        pSausage->runAction(action->clone());
        
    }
    
    m_pStove = Sprite::create(localPath("make2/stove.png"));
    this->addChildToContentLayer(m_pStove);
    CMVisibleRect::setPositionAdapted(m_pStove, 320, 330);
    
    m_pPan = Sprite::create(localPath("make2/pan.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 200+50, 300+50);
    
    m_pPanOil = Sprite::create(localPath("make3/pan_oil.png"));
    this->addChildToContentLayer(m_pPanOil);
    CMVisibleRect::setPositionAdapted(m_pPanOil, 320, 320+50);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fry_oil.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pPanOil->getPosition()));
}

void FrySausageScene::_showPlateRight()
{
    if (m_pPlateRight) {
        return;
    }
    m_pPlateRight = Sprite::create(localPath("make2/plate.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPositionAdapted(m_pPlateRight, 480+visibleSize.width, 680);
    m_pPlateRight->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    
}
void FrySausageScene::_doFry(DragNode* pNode)
{
    m_SausagePotMap.insert(make_pair(pNode->getTag(),pNode));;
    AudioHelp::getInstance()->playLoopEffect("fry.mp3");
    
    ProgressBarNew* pBar = ProgressBarNew::create("content/common/progress/progress.png", "content/common/progress/progress_bar.png", "content/common/progress/p_mask.png");
    pBar->setFlag(70, 104, 4, 2, 3);
    pBar->setColorSegment(Color3B(255, 180, 0), Color3B(50, 228, 71), Color3B(228, 71, 50));
    pBar->targetNode = pNode;
    pBar->progressNewState = CC_CALLBACK_2(FrySausageScene::_onProgressCallback, this);
    this->addChildToContentLayer(pBar);
    pBar->setPosition(pNode->getPosition()+Vec2(0, 80));
    pBar->setLocalZOrder(15);
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

void FrySausageScene::_finishFry(DragNode* pNode)
{
    m_bFull[pNode->flag] = false;
    m_SausagePotMap.erase(pNode->getTag());
    int count = m_SausageRightVector.size();
    
    pNode->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(pNode, 480, 650+40*(count%4));
    m_SausageRightVector.push_back(pNode);
    
    m_pGuideLayer->removeGuide();
    
//    pNode->setScale(0.5);
    if (count==2) {
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            SceneManager::replaceTheScene<RollSausageScene>();
        }), NULL));
    }
    if (m_SausagePotMap.size()==0) {
        AudioHelp::getInstance()->stopLoopEffect();
    }
}

void FrySausageScene::_onProgressCallback(ProgressBarNew* pBar,int segment)
{
    if (segment<5) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->setScale(1);
            pDragNode->setDragSprite(Sprite::create(localPath("make3/"+std::to_string(segment+1)+".png")));
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
    }else if (segment>5 && segment<9) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->setDragSprite(Sprite::create(localPath("make3/"+std::to_string(segment)+".png")));
            if (segment==6) {
                pDragNode->index = 0;
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
                
                pDragNode->stopAllActions();
                pDragNode->setScale(1);
                
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
    }else if (segment == 9) {
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
                CMVisibleRect::setPositionAdapted(pDragNode, 170, 720-40*(tag%3)+visibleSize.height);
                pDragNode->setDragSprite(Sprite::create(localPath("make3/1.png")));
                
                pDragNode->stopAllActions();
//                pDragNode->setScale(0.5);
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