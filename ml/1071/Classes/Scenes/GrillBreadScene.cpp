

#include "GrillBreadScene.h"
#include "SceneManager.h"

GrillBreadScene::GrillBreadScene()
{
    m_pPlateRight = nullptr;
}

GrillBreadScene::~GrillBreadScene()
{
    
}
bool GrillBreadScene::init()
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
        AudioHelp::getInstance()->playEffect("vo_grill_bread.mp3");
        m_pGuideLayer->showGuideMove(m_pPlateLeft->getPosition(), m_pGrill->getPosition());
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_not_burned.mp3");
    }),  NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void GrillBreadScene::onEnter()
{
    ExtensionScene::onEnter();
}

void GrillBreadScene::onExit()
{
    ExtensionScene::onExit();
}

void GrillBreadScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void GrillBreadScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
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
void GrillBreadScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void GrillBreadScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->index==0) {
        //    100,76,385,255
        Rect doughRect = m_pGrill->getBoundingBox();
        doughRect.origin = m_pGrill->getParent()->convertToWorldSpace(doughRect.origin);
        
        Rect rect = pDragNode->getDragSprite()->getBoundingBox();
        rect.origin = pDragNode->convertToWorldSpace(rect.origin);
        int index = m_BreadPotMap.size();
        if (rect.intersectsRect(doughRect) && index==0) {
//            AudioHelp::getInstance()->playEffect("fall.mp3");
            Vec2 pos = m_pGrill->getPosition()+Vec2(0, 20);
            
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pos),
                                                  CallFunc::create([=](){
                pDragNode->setOrgPositionDefault();
                _doFry(pDragNode);
            }), NULL));
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),
                            [=](){
                                
                                pDragNode->setTouchEnabled(true);
                            });
            pDragNode->runAction(ScaleTo::create(0.5, 0.5));
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
                pDragNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(5, 5)),
                                                                            MoveBy::create(0.1, Vec2(-5, -5)), NULL)));
            });
        }
    }
}
void GrillBreadScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}
#pragma mark - initData
void GrillBreadScene::_initData()
{
    setExPath("content/make/normal/");
    m_nNextSceneTag = GameUIEvent::eSceneTagPutinSausage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagPutinSausage;
}


void GrillBreadScene::_showPlate()
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
        DragNode* pBread = _createDrageNode(localPath("make2/bread_2.png"));
        this->addChildToContentLayer(pBread);
        pBread->setTag(i);
        pBread->setLocalZOrder(i);
        pBread->index = 0;
        CMVisibleRect::setPositionAdapted(pBread, 170-visibleSize.width, 730-40*(i%3));
        m_BreadLeftVector.push_back(pBread);
        pBread->setScale(0.5);
        pBread->runAction(action->clone());
        
    }
    
    m_pGrill = Sprite::create(localPath("make3/grill.png"));
    this->addChildToContentLayer(m_pGrill);
    CMVisibleRect::setPositionAdapted(m_pGrill, 320, 380+10);
    
    m_pGrill2 = Sprite::create(localPath("make3/grill_1.png"));
    this->addChildToContentLayer(m_pGrill2);
    m_pGrill2->setPosition(m_pGrill->getPosition());
    m_pGrill2->runAction(RepeatForever::create(Sequence::create(FadeTo::create(1,255),
                                                                DelayTime::create(0.5),
                                                                FadeTo::create(1.5,150), NULL)));
}

void GrillBreadScene::_showPlateRight()
{
    if (m_pPlateRight) {
        return;
    }
    m_pPlateRight = Sprite::create(localPath("make2/plate.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPositionAdapted(m_pPlateRight, 480+visibleSize.width, 680);
    m_pPlateRight->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    
}
void GrillBreadScene::_doFry(DragNode* pNode)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->playLoopEffect("grill.mp3");
    m_BreadPotMap.insert(make_pair(pNode->getTag(),pNode));;
    AudioHelp::getInstance()->playLoopEffect("fry_Bread.mp3");
    
    pNode->setDragSprite(Sprite::create(localPath("make3/bread1.png")));
    
    ProgressBarNew* pBar = ProgressBarNew::create("content/common/progress/progress.png", "content/common/progress/progress_bar.png", "content/common/progress/p_mask.png");
    pBar->setFlag(70, 104, 5, 3, 3);
    pBar->setColorSegment(Color3B(255, 180, 0), Color3B(50, 228, 71), Color3B(228, 71, 50));
    pBar->targetNode = pNode;
    pBar->progressNewState = CC_CALLBACK_2(GrillBreadScene::_onProgressCallback, this);
    this->addChildToContentLayer(pBar);
    pBar->setPosition(pNode->getPosition()+Vec2(0, 80));
    pBar->setLocalZOrder(15);
    pBar->beginProgress(50);
    pNode->setUserData(pBar);
    pNode->setLocalZOrder(1);
    pBar->setVisible(false);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke1.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(pNode->getPosition()+Vec2(0,-50));
        pParticle->setLocalZOrder(2);
        pParticle->setName("smoke");
    }), NULL));
    pNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                            MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
}

void GrillBreadScene::_finishFry(DragNode* pNode)
{
    m_pGuideLayer->removeGuide();
    m_BreadPotMap.erase(pNode->getTag());
    int count = m_BreadRightVector.size();
    
    pNode->setTouchEnabled(false);
    pNode->setDragSprite(Sprite::create(localPath("make4/hotdog_1.png")));
    
    ParticleSystemQuad*pSmoke = (ParticleSystemQuad*)m_pContentLayer->getChildByName("smoke");
    if (pSmoke) {
        pSmoke->stopSystem();
        pSmoke->removeFromParent();
    }
    
    CMVisibleRect::setPositionAdapted(pNode, 480, 670+30*(count%4));
    m_BreadRightVector.push_back(pNode);
    
    pNode->setScale(0.5);
    if (count==2) {
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            SceneManager::replaceTheScene<PutinSausageScene>();
        }), NULL));
    }
    if (m_BreadPotMap.size()==0) {
        AudioHelp::getInstance()->stopLoopEffect();
    }
}

void GrillBreadScene::_onProgressCallback(ProgressBarNew* pBar,int segment)
{
    if (segment<6) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->setScale(1);
            pDragNode->setDragSprite(Sprite::create(localPath("make3/bread"+std::to_string(segment+1)+".png")));
            
            if (segment==5) {
                pDragNode->index = 1;
                pDragNode->setTouchEnabled(true);
                if(m_BreadRightVector.size()<3){
                    _showPlateRight();
                    this->runAction(Sequence::create(DelayTime::create(0.5),
                                                     CallFunc::create([=](){
                        m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPlateRight->getPosition());
                    }), NULL));
                    AudioHelp::getInstance()->playEffect("fry_ok.mp3");
                    
                    pDragNode->stopAllActions();
                    pDragNode->setPosition(pDragNode->getOrgPosition());
                    pDragNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(5, 5)),
                                                                                MoveBy::create(0.1, Vec2(-5, -5)), NULL)));
                }
            }
        }
    }else if (segment>7 && segment<11) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->setDragSprite(Sprite::create(localPath("make3/bread"+std::to_string(segment-1)+".png")));
            if (segment==8) {
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
    }else if (segment == 11) {
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
                CMVisibleRect::setPositionAdapted(pDragNode, 170, 730-40*(tag%3)+visibleSize.height);
                pDragNode->setDragSprite(Sprite::create(localPath("make2/bread_2.png")));
                
                ParticleSystemQuad*pSmoke = (ParticleSystemQuad*)m_pContentLayer->getChildByName("smoke");
                if (pSmoke) {
                    pSmoke->stopSystem();
                    pSmoke->removeFromParent();
                }
                pDragNode->stopAllActions();
                pDragNode->setScale(0.5);
                pDragNode->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                                      CallFunc::create([=](){
                    pDragNode->setOrgPositionDefault();
                    pDragNode->setTouchEnabled(true);
                    pDragNode->setLocalZOrder(tag);
                }), NULL));
                
                m_BreadPotMap.erase(tag);
                if (m_BreadPotMap.size()==0) {
                    AudioHelp::getInstance()->stopLoopEffect();
                }
            }), NULL));
        }
    }
}