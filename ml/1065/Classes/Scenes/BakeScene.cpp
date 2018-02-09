
#include "BakeScene.h"
BakeScene::BakeScene()
{
    m_bBowlShown = false;
    m_pBowl = nullptr;
    m_pBowlInOven = nullptr;
    m_pBar = nullptr;
    m_bCakeReady = false;
}

BakeScene::~BakeScene()
{
    
}
bool BakeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    Sprite* pBg = Sprite::create(localPath("bake/bake_bk.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pOven = OvenNode::create();
    this->addChildToContentLayer(m_pOven);
    m_pOven->onOvenStateCallback = CC_CALLBACK_1(BakeScene::onOvenStateCallback, this);
    CMVisibleRect::setPosition(m_pOven, 600, 350);
    m_pOven->setSwitchEnable(false);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGuideLayer2 = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer2);
    
    m_pGuideLayer->showGuideArrow(Vec2(m_pOven->getPositionX()-100, m_pOven->getPositionY()), -2);
    m_pGuideLayer2->showGuideTap(Vec2(m_pOven->getPositionX(), m_pOven->getPositionY()-100));
    return true;
}

#pragma mark - initData
void BakeScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagFrosting;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagFrosting;
    
}

void BakeScene::onOvenStateCallback(OvenNode::State state)
{
    switch (state) {
        case OvenNode::State::eStateDoorOpen:
        {
            if (!m_bBowlShown) {
                m_bBowlShown = true;
                _showBowl();
                
                m_pGuideLayer->removeGuide();
                m_pGuideLayer2->removeGuide();
                
                m_pGuideLayer->showGuideMove(CMVisibleRect::getPosition(850, 100), Vec2(m_pOven->getPositionX()-50, m_pOven->getPositionY()-200));
            }
            m_pOven->setSwitchEnable(false);
            m_pGuideLayer->removeGuide();
            m_pGuideLayer2->removeGuide();
        }
            break;
        case OvenNode::State::eStateDoorClose:
        {
            if (m_bCakeReady) {
                m_pGuideLayer->removeGuide();
                m_pGuideLayer2->removeGuide();
                m_pOven->setSwitchEnable();
                m_pGuideLayer->showGuideTap(m_pOven->getSwitchWorldPosition());
            }
        }
            break;
        case OvenNode::State::eStateTurnOn:
        {
            m_pGuideLayer->removeGuide();
            m_pGuideLayer2->removeGuide();
            _beginBakeDough();
        }
            break;
        case OvenNode::State::eStateTurnOff:
        {
            m_pBar->pauseProgress();
            this->_pauseBake();
            m_pGameUI->showNextLayout();
            m_pOven->setDoorTouchEnable(false);
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setPosition(visibleSize*0.5);
        }
            break;
        default:
            break;
    }
}

void BakeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    
}

void BakeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}

void BakeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (m_pOven->state()==OvenNode::State::eStateDoorOpen) {
        if (m_pOven->getDoorWorldRect().containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setVisible(false);
            m_bCakeReady = true;
            if (!m_pBowlInOven) {
                m_pBowlInOven = Sprite::create(localPath("bake/cakepan1.png"));
                m_pOven->addChild(m_pBowlInOven);
            }else{
                m_pBowlInOven->setTexture(localPath("bake/cakepan1.png"));
            }
            m_pBowlInOven->setVisible(true);
            m_pBowlInOven->setPosition(Vec2(-80, -150));
            m_pGuideLayer->removeGuide();
            m_pGuideLayer2->removeGuide();
            m_pGuideLayer->showGuideArrow(Vec2(m_pOven->getPositionX()-100, m_pOven->getPositionY()-200), 2);
            m_pGuideLayer2->showGuideTap(Vec2(m_pOven->getPositionX(), m_pOven->getPositionY()-100));
        }else{
            pDragNode->back();
        }
    }else{
        pDragNode->back();
    }
}

void BakeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void BakeScene::_showBowl()
{
    if (!m_pBowl) {
        m_pBowl = _createDrageNode(localPath("mix/pan0.png"));
        this->addChildToContentLayer(m_pBowl);
        Sprite* pMixtrue = Sprite::create(localPath("mix/pan_batter.png"));
        m_pBowl->addChild(pMixtrue);
        pMixtrue->setName("mixture");
        Sprite* pBowlFront = Sprite::create(localPath("mix/pan1.png"));
        m_pBowl->addChild(pBowlFront,10);
    }
    CMVisibleRect::setPosition(m_pBowl, 850, 100+visibleSize.height);
    m_pBowl->setVisible(true);
    m_pBowl->setTouchEnabled(false);
    m_pBowl->runAction(Sequence::create(EaseSineOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                        CallFunc::create([=]()
                                                         {
                                                             m_pBowl->setOrgPositionDefault();
                                                             m_pBowl->setTouchEnabled(true);
                                                         }), NULL));
    
}
void BakeScene::_resumeBake()
{
    AudioHelp::getInstance()->playLoopEffect("oven_work1.mp3");
    Director::getInstance()->getScheduler()->resumeTarget(this);
    m_pBar->resume();
    _showBar();
    m_pOven->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05, Vec2(0.5, 0.5)),
                                                              MoveBy::create(0.05, Vec2(-1, -1)),
                                                              MoveBy::create(0.05, Vec2(0.5, 0.5)),
                                                              NULL)));
}
void BakeScene::_pauseBake()
{
    AudioHelp::getInstance()->stopLoopEffect();
    Director::getInstance()->getScheduler()->pauseTarget(this);
    m_pBar->pause();
    _hideBar();
    m_pOven->stopAllActions();
    m_pGuideLayer->removeGuide();
    m_pGuideLayer2->removeGuide();
    
}

void BakeScene::_beginBakeDough()
{
    m_nBakeCount = 0;
    _resetBar();
    m_pBar->beginProgress(35.0f);
    _showBar();
    _resumeBake();
    schedule(schedule_selector(BakeScene::_doBakeDough), 4);
    //    m_pChickenInPan->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.01, Vec2(1, 1)),
    //                                                                    MoveBy::create(0.01, Vec2(-2, -2)),
    //                                                                    MoveBy::create(0.01, Vec2(1, 1)),
    //                                                                    NULL)));
}

void BakeScene::_doBakeDough(float dt)
{
    m_nBakeCount++;
        std::stringstream ostr;
        ostr<<"bake/cakepan"<<m_nBakeCount<<".png";
        m_pBowlInOven->setTexture(localPath(ostr.str()));
    
    if (m_nBakeCount==1) {
        unschedule(schedule_selector(BakeScene::_doBakeDough));
        //        Bake bad after 5s
        //        scheduleOnce(schedule_selector(BakeHotdogScene::_doBakeDough), 6);
    }else if (m_nBakeCount>2) {
        //        Bake bad
        log("===Bake bad");
        
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=]()
                                                          {
                                                              this->_pauseBake();
                                                              this->_hideBar();
                                                              
                                                              m_pBowlInOven->setVisible(false);
                                                              m_bCakeReady = false;
                                                              m_bBowlShown = false;
                                                              m_pOven->reset();
//                                                              this->_showBowl();
//                                                              
                                                                                                                        }),
                                         NULL));
//        if (m_pParticle) {
//            m_pParticle->removeFromParent();
//            m_pParticle = nullptr;
//        }
    }
    
    
}

void BakeScene::_resetBar()
{
    if (!m_pBar) {
        m_pBar = ProgressBar::create("content/common/progress/bar_v.png", "content/common/progress/face1.png", "content/common/progress/face2.png", "content/common/progress/face3.png","content/common/progress/bar_mash_v.png",ProgressBar::Direction::eDirectionV);
        this->addChildToUILayer(m_pBar);
        CMVisibleRect::setPositionAdapted(m_pBar, 320, 550,kBorderNone,kBorderTop);
        m_pBar->progressNewState = [=](int state){
            if (state == ProgressBar::eStateOK) {
                unschedule(schedule_selector(BakeScene::_doBakeDough));
                AudioHelp::getInstance()->playEffect("oven_done.mp3");
                
//                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/successCherry.plist");
//                this->addChildToUILayer(pParticle);
//                pParticle->setTexture(Sprite::create("particle/particle_chicken.png")->getTexture());
//                pParticle->setPosition(Vec2(visibleSize.width/2, -100));
                m_nBakeCount = 1;
                _doBakeDough(0.1);
                m_pGuideLayer->showGuideTap(m_pOven->getSwitchWorldPosition());
                m_pOven->setSwitchEnable();
                
            }else if (state == ProgressBar::eStateBad) {
                
                log("===Bake bad");
                AudioHelp::getInstance()->playEffect("fail1.mp3");
                m_nBakeCount = 2;
                _doBakeDough(0.1);
                m_pGuideLayer->removeGuide();
                m_pGuideLayer2->removeGuide();
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/bake_smoke.plist");
                m_pOven->addChild(pParticle);
                pParticle->setPosition(Vec2(-130, -180));
                pParticle->setName("smoke");
            }else if (state == ProgressBar::eStateOver) {
            }
        };
    }
    m_pBar->stopProgress();
    m_pBar->setVisible(true);
    m_pBar->setPercentStage(200.0*100/446, 350.0*100/446, 100);
}
void BakeScene::_showBar()
{
    if (m_pBar) {
        m_pBar->stopAllActions();
        if (m_pBar->getDirection()==ProgressBar::Direction::eDirectionH) {
            CMVisibleRect::setPositionAdapted(m_pBar, 480, 550+visibleSize.height/2,kBorderNone,kBorderTop);
            m_pBar->setVisible(true);
            m_pBar->runAction(EaseBackOut::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height/2))));
        }else{
            CMVisibleRect::setPositionAdapted(m_pBar, 50-visibleSize.width/2, visibleSize.height/2,kBorderLeft);
            m_pBar->setVisible(true);
            m_pBar->runAction(EaseBackOut::create(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0))));
        }
    }
}

void BakeScene::_hideBar()
{
    if (m_pBar) {
        m_pBar->stopAllActions();
        if (m_pBar->getDirection()==ProgressBar::Direction::eDirectionH) {
            CMVisibleRect::setPositionAdapted(m_pBar, 480, 600,kBorderNone,kBorderTop);
            m_pBar->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, visibleSize.height/2))),
                                               CallFunc::create([=]()
                                                                {
                                                                    m_pBar->setVisible(false);
                                                                }), NULL));
        }else{
            CMVisibleRect::setPositionAdapted(m_pBar, 50, visibleSize.height/2,kBorderLeft);
            m_pBar->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0))),
                                               CallFunc::create([=]()
                                                                {
                                                                    m_pBar->setVisible(false);
                                                                }), NULL));
        }
    }
}