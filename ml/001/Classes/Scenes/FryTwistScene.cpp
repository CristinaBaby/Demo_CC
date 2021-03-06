

#include "FryTwistScene.h"

FryTwistScene::FryTwistScene()
{
    
}

FryTwistScene::~FryTwistScene()
{
    
}
bool FryTwistScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create("content/make/bg/bg_make3.jpg");
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    _showPlate();
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_time_fry.mp3");
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_drag_ry.mp3");
        m_pGuideLayer->showGuideMove(m_pPlateLeft->getPosition(), m_pPot->getPosition());
    }),
                                     DelayTime::create(2.2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_fry_golden.mp3");
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_not_burned.mp3");
    }),   NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void FryTwistScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FryTwistScene::onExit()
{
    ExtensionScene::onExit();
}

void FryTwistScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FryTwistScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->setLocalZOrder(10);
    if (pDragNode->index==0) {
        
    }else{
        Node* pBubble = pDragNode->getChildByName("bubble");
        if (pBubble) {
            pBubble->runAction(FadeOut::create(0.3));
        }
        ProgressBarNew* pBar = (ProgressBarNew*)pDragNode->getUserData();
        if(pBar){
            pBar->pauseProgress();
        }
        
    }
}
void FryTwistScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void FryTwistScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->index==0) {
        //    100,76,385,255
        Rect doughRect = Rect(100, 76, 385, 255);
        doughRect.origin = m_pPot->convertToWorldSpace(doughRect.origin);
        Rect rect = pDragNode->getDragSprite()->getBoundingBox();
        rect.origin = pDragNode->convertToWorldSpace(rect.origin);
        if (rect.intersectsRect(doughRect)) {
            float lengthX = 40;
            float lengthY = 116;
            Vec2 pos = pDragNode->getPosition();
            Rect tempRect = doughRect;
            tempRect.origin = pDragNode->getParent()->convertToNodeSpace(doughRect.origin);
            
            if (worldPoint.x-lengthX<doughRect.getMinX()) {
                pos.x = tempRect.getMinX()+lengthX;
            }
            if (worldPoint.x+lengthX>doughRect.getMaxX()) {
                pos.x = tempRect.getMaxX()-lengthX;
            }
            if (worldPoint.y-lengthY<doughRect.getMinY()) {
                pos.y = tempRect.getMinY()+lengthY-30;
            }
            if (worldPoint.y+lengthY>doughRect.getMaxY()) {
                pos.y = tempRect.getMaxY()-lengthX-30;
            }
            
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(pos);
            pDragNode->setOrgPositionDefault();
            _doFry(pDragNode);
        }else{
            pDragNode->back(pDragNode->getTag());
        }
        
    }else{
        Rect plateRect = m_pPlateRight->getBoundingBox();
        if (plateRect.containsPoint(pDragNode->getPosition())) {
            _finishFry(pDragNode);
        }else{
            pDragNode->back(0,[=](){
                Node* pBubble = pDragNode->getChildByName("bubble");
                if (pBubble) {
                    pBubble->runAction(FadeIn::create(0.3));
                }
                ProgressBarNew* pBar = (ProgressBarNew*)pDragNode->getUserData();
                if(pBar){
                    pBar->resumeProgress();
                }
            });
        }
    }
}
void FryTwistScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}
#pragma mark - initData
void FryTwistScene::_initData()
{
    setExPath("content/make/twist/");
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecorate;
    GameDataManager::getInstance()->m_nPackage = 1;
}


void FryTwistScene::_showPlate()
{
    auto action = Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pDrag = dynamic_cast<DragNode*>(pNode);
        if (pDrag) {
            pDrag->setOrgPositionDefault();
            pDrag->setTouchEnabled(true);
        }
    }), NULL);
    m_pPlateLeft = Sprite::create("content/make/donut/make3/plate1.png");
    this->addChildToContentLayer(m_pPlateLeft);
    CMVisibleRect::setPositionAdapted(m_pPlateLeft, 130-visibleSize.width, 750);
    m_pPlateLeft->runAction(action);
    
    for (int i = 0; i<6; i++) {
        DragNode* pDonut = _createDrageNode(localPath("make3/twist_donut.png"));
        this->addChildToContentLayer(pDonut);
        pDonut->setTag(i);
        pDonut->setLocalZOrder(i);
        pDonut->index = 0;
        CMVisibleRect::setPositionAdapted(pDonut, 20+40*(i%6)-visibleSize.width, 750);
        m_DonutLeftVector.push_back(pDonut);
        pDonut->runAction(action->clone());
        
    }
    
    m_pPlateRight = Sprite::create("content/make/donut/make3/plate2.png");
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPositionAdapted(m_pPlateRight, 520+visibleSize.width, 750);
    m_pPlateRight->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    
    
    m_pPot = Sprite::create("content/make/donut/make3/pot.png");
    this->addChildToContentLayer(m_pPot);
    CMVisibleRect::setPositionAdapted(m_pPot, 320, 380);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fry_oil.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(m_pPot->getPosition());
}

void FryTwistScene::_doFry(DragNode* pNode)
{
    m_DonutPotMap.insert(make_pair(pNode->getTag(),pNode));;
    AudioHelp::getInstance()->playLoopEffect("fry_donut.mp3");
    Sprite* pSprite =Sprite::create(localPath("make3/oil.png"));
    pNode->addChild(pSprite,-1);
    pSprite->setName("bubble");
    pSprite->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        pSprite->setScaleX(-pSprite->getScaleX());
    }),
                                                              DelayTime::create(0.4), NULL)));
    
    pNode->setScale(0.9);
    ProgressBarNew* pBar = ProgressBarNew::create("content/common/progress/progress.png", "content/common/progress/progress_bar.png", "content/common/progress/p_mask.png");
    pBar->setFlag(70, 104, 5, 2, 2);
    pBar->setColorSegment(Color3B(255, 180, 0), Color3B(50, 228, 71), Color3B(228, 71, 50));
    pBar->targetNode = pNode;
    pBar->progressNewState = CC_CALLBACK_2(FryTwistScene::_onProgressCallback, this);
    this->addChildToContentLayer(pBar);
    pBar->setPosition(pNode->getPosition()+Vec2(0, 80));
    pBar->setLocalZOrder(15);
    pBar->beginProgress(20);
    pNode->setUserData(pBar);
    pNode->setLocalZOrder(1);
}

void FryTwistScene::_finishFry(DragNode* pNode)
{
    m_DonutPotMap.erase(pNode->getTag());
    int count = m_DonutRightVector.size();
//    if (count>=4) {
//        pNode->back();
//        Node* pBubble = pNode->getChildByName("bubble");
//        if (pBubble) {
//            pBubble->runAction(FadeIn::create(0.3));
//        }
//        ProgressBarNew* pBar = (ProgressBarNew*)pNode->getUserData();
//        if(pBar){
//            pBar->resumeProgress();
//        }
//        return;
//    }
    pNode->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(pNode, 435+40*(count%6), 750);
    m_DonutRightVector.push_back(pNode);
    if (count==3) {
        m_pGameUI->showNextLayout();
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->playEffect(((arc4random()%100)>50?"vo_nice_work.mp3":"vo_well_done.mp3"));
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
    }
    if (m_DonutPotMap.size()==0) {
        AudioHelp::getInstance()->stopLoopEffect();
    }
}

void FryTwistScene::_onProgressCallback(ProgressBarNew* pBar,int segment)
{
    if (segment<6) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->setScale(1);
            pDragNode->setDragSprite(Sprite::create(localPath("make3/donut"+std::to_string(segment)+".png")));
            
            if (segment==5) {
                pDragNode->index = 1;
                pDragNode->setTouchEnabled(true);
                m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPlateRight->getPosition());
                AudioHelp::getInstance()->playEffect("fry_ok.mp3");
            }
        }
    }else if (segment>6 && segment<9) {
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode) {
            pDragNode->setScale(1);
            pDragNode->setDragSprite(Sprite::create(localPath("make3/donut"+std::to_string(segment-1)+".png")));
            if (segment==7) {
                pDragNode->index = 0;
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
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
//        AudioHelp::getInstance()->playEffect("fry_failed.mp3");
        DragNode* pDragNode = dynamic_cast<DragNode*>(pBar->targetNode);
        if (pDragNode){
            pDragNode->getDragSprite()->runAction(FadeOut::create(0.5));
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke3.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(9);
            pParticle->setPosition(pDragNode->getPosition());
//            AudioHelp::getInstance()->stopEffect("vo_burned.mp3");
//            AudioHelp::getInstance()->playEffect("vo_burned.mp3");
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setLocalZOrder(9);
                pParticle->setPosition(pDragNode->getPosition()-Vec2(0, 30));
                
//                pDragNode->setPosition(pDragNode->getOrgPosition()+Vec2(0, visibleSize.height));
                int tag = pDragNode->getTag();
                CMVisibleRect::setPositionAdapted(pDragNode, 20+40*(tag%6), 750+visibleSize.height);
                pDragNode->setDragSprite(Sprite::create(localPath("make3/twist_donut.png")));
                pDragNode->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                                      CallFunc::create([=](){
                    pDragNode->setOrgPositionDefault();
                    pDragNode->setTouchEnabled(true);
                    pDragNode->setLocalZOrder(tag);
                }), NULL));
                m_pGuideLayer->removeGuide();
                Node* pBubble = pDragNode->getChildByName("bubble");
                if (pBubble) {
                    pBubble->removeFromParent();
                }
                m_DonutPotMap.erase(tag);
                if (m_DonutPotMap.size()==0) {
                    AudioHelp::getInstance()->stopLoopEffect();
                }
            }), NULL));
        }
    }
}