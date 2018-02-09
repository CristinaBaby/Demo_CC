
#include "PuzzleGameScene.h"
PuzzleGameScene::PuzzleGameScene()
{
    
}

PuzzleGameScene::~PuzzleGameScene()
{
    
}
bool PuzzleGameScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    FileUtils::getInstance()->addSearchPath("cocos/");
    
    Node* scene = CSLoader::createNode("PuzzleScene.csb");
    this->addChildToContentLayer(scene);
    CMVisibleRect::setPosition(scene, 0, 0);
    ui::Helper::doLayout(scene);
    
    
    m_pPuzzleLayer = scene->getChildByName("puzzleLayer");
    m_pBalls = scene->getChildByName("ball");
    Vector<Node*> ballChildren = m_pBalls->getChildren();
    for_each(ballChildren.begin(), ballChildren.end(), [=](Node* pBall){
        int tag = pBall->getTag();
        std::stringstream ostr;
        ostr<<"shape"<<tag;
        pBall->stopAllActions();
        Node* pShape = pBall->getChildByName(ostr.str());
        pShape->setVisible(true);
        pBall->setPosition(Vec2(pBall->getPositionX(), pBall->getPositionY()+visibleSize.height));
        pBall->runAction(Sequence::create(DelayTime::create(0.5+0.1*tag),
                                          EaseSineIn::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height))),
                                          CallFunc::create([=]()
                                                           {
                                                               AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                                                               if (tag == 0) {
                                                                   AudioHelp::getInstance()->playEffect("many_shapes.mp3");
                                                                   this->runAction(Sequence::create(DelayTime::create(2),
                                                                                                    CallFunc::create([=]()
                                                                                                                     {
                                                                                                                         AudioHelp::getInstance()->playEffect("choose_shape.mp3");
                                                                                                                     }), NULL));
                                                               }
                                                               
                                                           }), NULL));
        Button* pButton = dynamic_cast<Button*>(pBall->getChildByName("ball"));
        if (pButton) {
            pButton->setTag(tag);
            pButton->addTouchEventListener(CC_CALLBACK_2(PuzzleGameScene::onBallcallback, this));
        }
        if (!IAPManager::getInstance()->getItemIsBought(0) && !gNoneIap && tag>=3) {
            Sprite* pLock = Sprite::create("content/common/lock.png");
            pLock->setName("lock");
            pBall->addChild(pLock,5);
            pLock->setPosition(Vec2(50, -50));
        }
    });
    m_pPuzzleLayer->setPosition(m_pPuzzleLayer->getPositionX(), m_pPuzzleLayer->getPositionY()+visibleSize.height);
    m_pPuzzleLayer->setVisible(false);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/cas.png");
    return true;
}

void PuzzleGameScene::onEnter()
{
    ExtensionScene::onEnter();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_SHAPE);
}

void PuzzleGameScene::onExit()
{
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_SHAPE);
    
    ExtensionScene::onExit();
}

void PuzzleGameScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(0)){
        Vector<Node*> ballChildren = m_pBalls->getChildren();
        for_each(ballChildren.begin(), ballChildren.end(), [=](Node* pBall){
            Node* pLock = pBall->getChildByName("lock");
            if (pLock) {
                pLock->setVisible(false);
            }
        });

    }
}

void PuzzleGameScene::onBallcallback(Ref* ref,Widget::TouchEventType type)
{
    if(type==Widget::TouchEventType::ENDED){
        Node* pNode = (Node*)ref;
        int tag = pNode->getTag();
        if (!IAPManager::getInstance()->getItemIsBought(0) && !gNoneIap && tag>=3){
            ShopLayer* pLayer = ShopLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->showBannerDismiss();
            return;
        }
        AudioHelp::getInstance()->playSelectedEffect();
        m_nBallIndex = tag;
        Vector<Node*> ballChildren = m_pBalls->getChildren();
        for_each(ballChildren.begin(), ballChildren.end(), [=](Node* pBall){
            Button* pButton = dynamic_cast<Button*>(pBall->getChildByName("ball"));
            if (pButton) {
                pButton->setTouchEnabled(false);
                pButton->setEnabled(false);
            }
            int m = pBall->getTag();
            if (m==m_nBallIndex){
                Vec2 pos = visibleSize*0.5;
                pos = pBall->getParent()->convertToNodeSpace(pos);
                pBall->runAction(Sequence::create(Spawn::create(EaseBackOut::create(ScaleTo::create(1, 1.2)),
                                                                MoveTo::create(0.5, pos), NULL),
                                                  DelayTime::create(0.6),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       _showPuzzleLayer(tag);
                                                                   }), NULL));
                
            }else{
                if (pBall->getPositionX()<visibleSize.width/2) {
                    pBall->runAction(JumpBy::create(0.5, Vec2(-visibleSize.width, 0), 100, 1));
                }else{
                    pBall->runAction(JumpBy::create(0.5, Vec2(visibleSize.width, 0), 100, 1));
                }
            }
        });
    }
}

void PuzzleGameScene::onPuzzleFinished()
{
    int radoom = arc4random()%100;
    if (radoom>50) {
        AudioHelp::getInstance()->playEffect("great.mp3");
    }else{
        AudioHelp::getInstance()->playEffect("wonderful.mp3");
    }
    AudioHelp::getInstance()->playEffect("finish_particle.mp3");
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    Sprite* pSprite = m_PuzzleManager.copeShape();
    Vec2 pos = pSprite->getPosition();
    pSprite->setPosition(m_pUILayer->convertToNodeSpace(pos));
    this->addChildToUILayer(pSprite);
    
    Node* ball = m_pBalls->getChildByTag(m_nBallIndex);
    Vec2 pos2 = ball->getPosition();
    pos2 = m_pUILayer->convertToNodeSpace(ball->getParent()->convertToWorldSpace(pos2));
    pSprite->runAction(Sequence::create(ScaleTo::create(1.5, 1.5),
                                        Spawn::create(JumpTo::create(1, pos2, 50, 1),
                                                      ScaleTo::create(0.6, 0.7), NULL),
                                        FadeOut::create(0.5),
                                        CallFunc::create([=]()
                                                         {
                                                             
                                                         }), NULL));
    for_each(m_Units.begin(), m_Units.end(), [=](DragNode* pUnit)
             {
                 pUnit->setVisible(false);
             });
    m_pPuzzleLayer->runAction(Sequence::create(DelayTime::create(2),
                                               EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                               CallFunc::create([=]()
                                                                {
                                                                    Node* ball = m_pBalls->getChildByTag(m_nBallIndex);
                                                                    auto action = CSLoader::createTimeline("Ball.csb", ball);
                                                                    action->play("crushed", false);
                                                                    action->setTimeSpeed(0.5);
                                                                    ball->runAction(action);
                                                                    
                                                                    ball->runAction(Repeat::create(Sequence::create(MoveBy::create(0.1, Vec2(2, 2)),
                                                                                                                    MoveBy::create(0.1, Vec2(-4, -4)),
                                                                                                                    MoveBy::create(0.1, Vec2(2, 2)),
                                                                                                                    NULL), 10));
                                                                }),
                                               DelayTime::create(3),
                                               CallFunc::create([=]()
                                                                {
                                                                    AudioHelp::getInstance()->playEffect("part_map.mp3");
                                                                    AudioHelp::getInstance()->playEffect("shape_finish.mp3");
                                                                    
                                                                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/ballboom.plist");
                                                                    this->addChildToUILayer(pParticle);
                                                                    
                                                                    Vec2 pos = ball->getParent()->convertToWorldSpace(ball->getPosition());
                                                                    pParticle->setPosition(m_pUILayer->convertToNodeSpace(pos));
                                                                    ball->runAction(Sequence::create(DelayTime::create(0.3),
                                                                                                     Spawn::create(JumpBy::create(1, Vec2::ZERO, 300, 1),
                                                                                                                   EaseBackOut::create(ScaleTo::create(1.5, 3)), NULL), NULL));
                                                                }),
                                               DelayTime::create(2),
                                               CallFunc::create([=]()
                                                                {
                                                                    _ballBomb();
                                                                }), NULL));
    
}

void PuzzleGameScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(1);
    pDragNode->setScale(0.9);
    m_pGuideLayer->removeGuide();
}
void PuzzleGameScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void PuzzleGameScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    Rect rect = pDragNode->getDragSprite()->getBoundingBox();
    rect.origin = pDragNode->convertToWorldSpace(rect.origin);
    int tag = pDragNode->getTag();
    Node* pUnit = m_PuzzleManager.addPuzzleUnit(rect,tag);
    if (pUnit) {
        AudioHelp::getInstance()->playEffect("work_right.mp3");
        
        Vec2 pos = pUnit->getPosition();
        pos = pDragNode->getParent()->convertToNodeSpace(pUnit->getParent()->convertToWorldSpace(pos));
        pDragNode->setTouchEnabled(false);
        pDragNode->setPosition(pos);
        pDragNode->setVisible(false);
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(pos);
        
    }else{
        pDragNode->runAction(ScaleTo::create(0.3, 0.5));
        pDragNode->back();
    }
}
void PuzzleGameScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

#pragma mark - initData
void PuzzleGameScene::_initData()
{
    setExPath("content/make/color/");
    m_nNextSceneTag = GameUIEvent::eSceneTagMap;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagMap;

}


void PuzzleGameScene::_showPuzzleLayer(int index)
{
    std::stringstream ostr;
    ostr<<"puzzle"<<index;
    m_pPuzzleLayer->setVisible(true);
    m_pPuzzleLayer->getChildByName(ostr.str())->setVisible(true);
    
    m_PuzzleManager.init(m_pPuzzleLayer,index);
    m_PuzzleManager.beginPuzzle();
    m_PuzzleManager.onFinishedCallback = CC_CALLBACK_0(PuzzleGameScene::onPuzzleFinished, this);
    AudioHelp::getInstance()->playEffect("mini_game.mp3");
    m_pPuzzleLayer->runAction(Sequence::create(DelayTime::create(0.5),
                                               EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                               CallFunc::create([=]()
                                                                {
                                                                    _showPuzzleUnits(index);
                                                                    m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2, visibleSize.height*0.2), Vec2(visibleSize.width/2, visibleSize.height*0.5));
                                                                }), NULL));
}

void PuzzleGameScene::_showPuzzleUnits(int index)
{
    for (int i = 0; i<4; i++) {
        std::stringstream ostr;
        ostr<<"shape/puzzle/"<<index+1<<"/puzzle"<<index+1<<"_"<<i+1<<".png";
        DragNode* pUnit = _createDrageNode(ostr.str());
        pUnit->setTag(i);
        m_Units.pushBack(pUnit);
        pUnit->setScale(0.5);
        this->addChildToUILayer(pUnit);
        
    }
    random_shuffle(m_Units.begin(), m_Units.end());
    int i = 0;
    for_each(m_Units.begin(), m_Units.end(), [=,&i](DragNode* pUnit)
             {
                 CMVisibleRect::setPosition(pUnit, 100+i*150, 240);
                 pUnit->setOrgPositionDefault();
                 i++;
             });
}

void PuzzleGameScene::_ballBomb()
{
    Vector<Node*> ballChildren = m_pBalls->getChildren();
    LayerColor* pColorLayer = LayerColor::create(Color4B(0, 0, 0, 160));
    m_pBalls->getParent()->addChild(pColorLayer);
    
    pColorLayer->setPosition(Vec2(CMVisibleRect::getDesignOffset(), Vec2::ZERO));
    
    m_pBalls->setLocalZOrder(10);
    
    m_pGameUI->showNextLayout();
    GameDataManager::getInstance()->setStepCount(3);
    GameDataManager::getInstance()->setIceShape(m_nBallIndex);
    
//    Node* pBall = ballChildren.at(m_nBallIndex);

//    for_each(ballChildren.begin(), ballChildren.end(), [=](Node* pBall){
//        int tag = pBall->getTag();
//        if (tag==m_nBallIndex) {
//            pBall->runAction(Sequence::create(DelayTime::create(0.5),
//                                              MoveTo::create(0.5, visibleSize*0.5),
//                                              ScaleTo::create(0.5, 1.5),
//                                              CallFunc::create([=]()
//                                                               {
//                                                                   m_pGameUI->showNextLayout();
//                                                                   GameDataManager::getInstance()->setStepCount(3);
//                                                                   
//                                                                   ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
//                                                                   this->addChildToUILayer(pParticle);
//                                                                   pParticle->setLocalZOrder(10);
//                                                                   pParticle->setPosition(visibleSize*0.5);
//                                                               }), NULL));
//        }else{
//            if (pBall->getPositionX()<visibleSize.width/2) {
//                pBall->runAction(JumpBy::create(0.5, Vec2(-visibleSize.width, 0), 100, 1));
//            }else{
//                pBall->runAction(JumpBy::create(0.5, Vec2(visibleSize.width, 0), 100, 1));
//            }
//        }
//    });
}