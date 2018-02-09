

#include "HotdogGrillSausageScene.h"
#include "SceneManager.h"

HotdogGrillSausageScene::HotdogGrillSausageScene()
{
    m_pPlateRight = nullptr;
}

HotdogGrillSausageScene::~HotdogGrillSausageScene()
{
    
}
bool HotdogGrillSausageScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localRootPath("bg/bg_make3.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    _showStove();
    _showPlate();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_hotdog_prepare_sausages.mp3");
        m_pGuideLayer->showGuideMove(m_pPlateLeft->getPosition(), m_pGrill->getPosition());
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_hotdog_sausage2pan.mp3");
    }), NULL));
    
//    AudioHelp::getInstance()->playBackGroundMusic("bg_make.mp3");
    m_pGameUI->showNormalLayout();
    return true;
}

void HotdogGrillSausageScene::onEnter()
{
    ExtensionScene::onEnter();
}

void HotdogGrillSausageScene::onExit()
{
    ExtensionScene::onExit();
}

void HotdogGrillSausageScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void HotdogGrillSausageScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->setLocalZOrder(20);
    pDragNode->getDragSprite()->stopAllActions();
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
void HotdogGrillSausageScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void HotdogGrillSausageScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
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
            Vec2 pos = Vec2(220.0,280.0+100.0*(index%3));
            pos = m_pGrill->convertToWorldSpace(pos);
            pDragNode->setLocalZOrder(1);
            
            m_SausagePotMap.insert(make_pair(pDragNode->getTag(),pDragNode));;
            if (m_SausagePotMap.size()+m_SausageRightVector.size()>=3) {
                m_pPlateLeft->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            }
            pDragNode->setScale(1);
            pDragNode->stopAllActions();
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                  CallFunc::create([=](){
                pDragNode->setOrgPositionDefault();
                _doFry(pDragNode);
            }), NULL));
        }else{
            pDragNode->back(pDragNode->getTag(),[=](){
                
                pDragNode->setScale(0.7);
            });
        }
    }else{
        Rect plateRect = m_pPlateRight->getBoundingBox();
        if (plateRect.containsPoint(pDragNode->getPosition())) {
            AudioHelp::getInstance()->playEffect("fall.mp3");
            pDragNode->setScale(0.7);
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
void HotdogGrillSausageScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}
#pragma mark - initData
void HotdogGrillSausageScene::_initData()
{
    setExPath("content/make/hotdog/3/");
    std::memset(m_bFull, 0, 3);
}

void HotdogGrillSausageScene::_showStove()
{
    m_pStove = Sprite::create("content/make/common/stove/stove.png");
    CMVisibleRect::setPosition(m_pStove, 320, 300);
    this->addChildToContentLayer(m_pStove);
    
    m_pFire = Sprite::create("content/make/common/stove/stove_1.png");
    CMVisibleRect::setPosition(m_pFire, 320, 300);
    this->addChildToContentLayer(m_pFire);
    
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/common/stove/stove_", 1, 2)));
}


void HotdogGrillSausageScene::_showPlate()
{
    auto action = Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pDrag = dynamic_cast<DragNode*>(pNode);
        if (pDrag) {
            pDrag->setOrgPositionDefault();
            pDrag->setTouchEnabled(true);
        }
    }), NULL);
    m_pPlateLeft = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlateLeft);
    CMVisibleRect::setPositionAdapted(m_pPlateLeft, 300-50-visibleSize.width, 780);
    m_pPlateLeft->runAction(action);
    m_pPlateLeft->setScale(0.7);
    
    for (int i = 0; i<3; i++) {
        DragNode* pSausage = _createDrageNode(localPath("1.png"));
        this->addChildToContentLayer(pSausage);
        pSausage->setTag(i);
        pSausage->setLocalZOrder(i);
        pSausage->index = 0;
        CMVisibleRect::setPositionAdapted(pSausage, 300-50-visibleSize.width, 840-50*(i%3));
        pSausage->runAction(action->clone());
        
        pSausage->setScale(0.7);
    }
    
    m_pGrill = Sprite::create(localPath("pan.png"));
    this->addChildToContentLayer(m_pGrill);
    CMVisibleRect::setPositionAdapted(m_pGrill, 320, 230);
    
    m_pGrill2 = Sprite::create(localPath("pan_oil.png"));
    this->addChildToContentLayer(m_pGrill2);
    m_pGrill2->setPosition(m_pGrill->getPosition());
    m_pGrill2->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1),
                                                                FadeTo::create(1.5,160), NULL)));
}

void HotdogGrillSausageScene::_showPlateRight()
{
    if (m_pPlateRight) {
        return;
    }
    m_pPlateRight = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPositionAdapted(m_pPlateRight, 300+200+visibleSize.width, 780);
    m_pPlateRight->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    m_pPlateRight->setScale(0.7);
    
}
void HotdogGrillSausageScene::_doFry(DragNode* pNode)
{
    if (m_SausageRightVector.size() == 3) {
        AudioHelp::getInstance()->playEffect("vo_hotdog_notburn.mp3");
    }
    AudioHelp::getInstance()->playLoopEffect("grill.mp3");
    
    pNode->getDragSprite()->runAction(Sequence::create(AnimationHelp::createAnimate(localPath(""), 1, 8,false,true,5),
                                                       CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("fry_ok.mp3");
        pNode->stopAllActions();
        pNode->setPosition(pNode->getOrgPosition());
        pNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(10, 10)),
                                                                    MoveBy::create(0.1, Vec2(-10, -10)), NULL)));
        pNode->index = 1;
        pNode->setTouchEnabled(true);
        if(m_SausageRightVector.size()<3){
            _showPlateRight();
            this->runAction(Sequence::create(DelayTime::create(0.5),
                                             CallFunc::create([=](){
                m_pGuideLayer->showGuideMove(pNode->getPosition(), m_pPlateRight->getPosition());
            }), NULL));
            AudioHelp::getInstance()->playEffect("fry_ok.mp3");
        }
        _showPlateRight();
    }), NULL));
    pNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                            MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
}

void HotdogGrillSausageScene::_finishFry(DragNode* pNode)
{
    m_bFull[pNode->flag] = false;
    m_SausagePotMap.erase(pNode->getTag());
    int count = m_SausageRightVector.size();
    
    pNode->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(pNode, 300+200, 840-50*(count%4));
    pNode->setScale(0.7);
    m_SausageRightVector.push_back(pNode);
    
    if (count==2) {
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            SceneManager::replaceTheScene<HotdogMixSauceScene>();
        }), NULL));
    }
    if (m_SausagePotMap.size()==0) {
        AudioHelp::getInstance()->stopLoopEffect();
    }
}

void HotdogGrillSausageScene::_onProgressCallback(ProgressBarNew* pBar,int segment)
{
    if (segment<5) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->setDragSprite(Sprite::create(localPath("sausage"+std::to_string(segment+1)+".png")));
            
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
            pDragNode->setDragSprite(Sprite::create(localPath("sausage"+std::to_string(segment-1)+".png")));
            if (segment==7) {
                pDragNode->index = 0;
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
                
                pDragNode->stopAllActions();
                
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
                CMVisibleRect::setPositionAdapted(pDragNode, 300, 670+30*(tag%3)+visibleSize.height);
                pDragNode->setDragSprite(Sprite::create(localPath("sausage1.png")));
                
                pDragNode->stopAllActions();
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