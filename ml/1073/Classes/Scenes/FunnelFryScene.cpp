
#include "FunnelFryScene.h"
#include "SceneManager.h"

FunnelFryScene::FunnelFryScene()
{
    m_pCake = nullptr;
}

FunnelFryScene::~FunnelFryScene()
{
    
}
bool FunnelFryScene::init()
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
    
    m_pFire = Sprite::create(localPath("fire1.png"));
    this->addChildToBGLayer(m_pFire);
    CMVisibleRect::setPositionAdapted(m_pFire, 320, 410);
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("fire"), 1, 2)));
    
    _showPan();
    _showOilCup();
    
    return true;
}

void FunnelFryScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FunnelFryScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FunnelFryScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "funnel"){
        if (pDragNode->index==0) {
            _pourCake();
            unschedule(schedule_selector(FunnelFryScene::_update));
            pDragNode->index = 1;
        }
    }
}

void FunnelFryScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "funnel"){
        m_pGuideLayer->removeGuide();
//        unschedule(schedule_selector(FunnelFryScene::_update));
    }else if (name == "clip"){
        if (pDragNode->index==0) {
            Rect rect = m_pCake->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                m_pBar->pauseProgress();
                m_pGuideLayer->removeGuide();
                
                pDragNode->index = 1;
                
                pDragNode->setPosition(m_pPan->getPosition()+Vec2(200, 300));
                pDragNode->setDragSprite(Sprite::create(localPath("clip2_down.png")));
                m_pCake->setVisible(false);
                Sprite* pCake = Sprite::create(localPath("cake"+std::to_string(m_pCake->getTag())+"_4.png"));
                pDragNode->addChild(pCake);
                pCake->setRotation3D(Vec3(-40, 30, 0));
                pCake->setPosition(Vec2(-pCake->getContentSize().width/2+50, -pCake->getContentSize().height/2));
                
                Sprite* pClipFront = Sprite::create(localPath("clip2_on.png"));
                pDragNode->addChild(pClipFront);
                
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }else if (pDragNode->index == 2) {
            pDragNode->setTouchEnabled(false);
            m_pGuideLayer->removeGuide();
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/cake_oil.plist");
            pDragNode->addChild(pParticle);
            pParticle->setLocalZOrder(10);
            pParticle->setPosition(Vec2(-100, -100));
            
            this->runAction(Repeat::create(Sequence::create(DelayTime::create(0.16),
                                                            CallFunc::create([=](){
                
                AudioHelp::getInstance()->playSelectedEffect();
            }), NULL), 3));
            pDragNode->runAction(Sequence::create(JumpBy::create(0.5, Vec2::ZERO, 50, 3),
                                                  DelayTime::create(1),
                                                  CallFunc::create([=](){
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/cake_oil2.plist");
                pDragNode->addChild(pParticle);
                pParticle->setPosition(Vec2(-100, -150));
            }),
                                                  DelayTime::create(3),
                                                  CallFunc::create([=](){
                _finish();
            }), NULL));
        }
        
    }
    
}

void FunnelFryScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name == "oilcup"){
        Rect rectPan = m_pPan->getBoundingBox();
        Rect rect = pDragNode->getBoundingBox();
        if (rectPan.intersectsRect(rect)) {
            pDragNode->removeFromParent();
            _pourOil();
        }
    }else if (name == "hand"){
        pDragNode->setTouchEnabled(false);
        m_pGuideLayer->showGuideMove(m_pFunnel->getPosition(), m_pFunnel->getPosition()+Vec2(50, 0));
        m_pFunnel->setTouchEnabled(true);
        _showFunnelStream();
        pDragNode->runAction(Sequence::create(MoveBy::create(1,Vec2(-visibleSize.width,-visibleSize.height/2)),
                                              CallFunc::create([=](){
        }),
                                              NULL));
    }else if (name == "clip"){
        if (pDragNode->index == 1) {
            m_pGuideLayer->showGuideMove(m_pClip->getPosition(), m_pClip->getPosition()+Vec2(0, 100));
            pDragNode->index = 2;
        }
    }
}

void FunnelFryScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void FunnelFryScene::_initData()
{
    setExPath("content/make/funnel/3/");
}


void FunnelFryScene::_showPan()
{
    m_pPan = Sprite::create(localPath("pan_down.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320+visibleSize.width, 490);
    
    m_pPanFront = Sprite::create(localPath("pan_on.png"));
    m_pPanFront->setPosition(m_pPan->getPosition());
    this->addChildToContentLayer(m_pPanFront);
    m_pPanFront->setLocalZOrder(10);
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFunc::create([=](){
        
    }), NULL);
    
    m_pPan->runAction(action);
    m_pPanFront->runAction(action->clone());
}

void FunnelFryScene::_showOilCup()
{
    m_pOilCup = _createDrageNode(localPath("oil1.png"));
    this->addChildToContentLayer(m_pOilCup);
    CMVisibleRect::setPositionAdapted(m_pOilCup, 320+visibleSize.width, 800);
    m_pOilCup->setName("oilcup");
    
    ContainerNode* pContainer = ContainerNode::create(m_pOilCup);
    pContainer->addShadow(localPath("oil1_1.png"));
    m_pOilCup->setUserData(pContainer);
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_funnel_500oil.mp3");
        m_pOilCup->setOrgPositionDefault();
        m_pGuideLayer->showGuideMove(m_pOilCup->getPosition(), m_pPan->getPosition());
        m_pOilCup->setTouchEnabled(true);
    }), NULL);
    
    m_pOilCup->runAction(action);
}

void FunnelFryScene::_showThermomter()
{
    Sprite* pSprite = Sprite::create(localPath("thermometer.png"));
    this->addChildToContentLayer(pSprite);
    CMVisibleRect::setPositionAdapted(pSprite, 370+visibleSize.width, 600);
    Sprite* pSpritePoint = Sprite::create(localPath("thermometer2.png"));
    pSprite->addChild(pSpritePoint);
    pSpritePoint->setPosition(pSprite->getContentSize()*0.5);
    
    pSprite->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        JumpBy::create(0.5, Vec2(0,-50), 50, 1),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_funnel_temprature.mp3");
        pSpritePoint->setTexture(localPath("thermometer1.png"));
        Label* pLabel = Label::createWithTTF("0", font_felt, 30);
        pLabel->setPosition(Vec2(155, 255));
        pLabel->setTextColor(Color4B::BLACK);
        pSprite->addChild(pLabel);
        pLabel->setTag(0);
        pLabel->setRotation(-65);
        pLabel->runAction(Repeat::create(Sequence::create(DelayTime::create(0.03),
                                                          CallFunc::create([=](){
            pLabel->setString(std::to_string(pLabel->getTag()+1));
            pLabel->setTag(pLabel->getTag()+1);
            if (pLabel->getTag()==200) {
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fry_oil.plist");
                m_pOilInPan->addChild(pParticle);
                pParticle->setLocalZOrder(10);
                pParticle->setPosition(m_pOilInPan->getContentSize()*0.5);
            }
        }), NULL), 350));
        
        MenuLayer::addIngredient(MenuLayer::ePackageFunnelcake, MenuLayer::eFunnelcake_oil);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        
    }),
                                        DelayTime::create(13),
                                        JumpBy::create(1,Vec2::ZERO,50,2),
                                        DelayTime::create(1),
                                        MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=](){
        
        _showFunnel();
    }), NULL));
    
}


void FunnelFryScene::_showFunnel()
{
    m_pFunnel = _createDrageNode(localRootPath("2/funnel_down.png"));
    m_pFunnel->setTouchEnabled(false);
    m_pFunnel->setName("funnel");
    m_pFunnel->setActionPoint(Vec2(147, 4));
    this->addChildToContentLayer(m_pFunnel);
    CMVisibleRect::setPositionAdapted(m_pFunnel, 320-visibleSize.width, 780);
    Rect rect = m_pPan->getBoundingBox();
    rect.origin = rect.origin+Vec2(50, 340);
    rect.size = rect.size+Size(-100, -40);
    m_pFunnel->setLimitRect(rect);
    m_pFunnel->setLocalZOrder(11);
    
    ContainerNode* pContainer = ContainerNode::create(m_pFunnel);
    pContainer->addFront(localRootPath("2/funnel_on.png"));
    pContainer->addFood(localRootPath("2/funnel_batter.png"));
    m_pFunnel->setUserData(pContainer);
    
    m_pBatter = pContainer->getFood();
    
    m_pHand = _createDrageNode(localRootPath("2/hands.png"));
    m_pHand->setName("hand");
    this->addChildToContentLayer(m_pHand);
    m_pHand->getDragSprite()->setAnchorPoint(Vec2(1, 1));
    m_pHand->setLocalZOrder(10);
    m_pHand->setPosition(m_pFunnel->getWorldSpaceActionPoint()+Vec2(20, -visibleSize.height*0.5+15));
    m_pHand->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(1, Vec2(visibleSize.width, visibleSize.height*0.5)),
                                        CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pHand->getPosition()-Vec2(100,100), m_pHand->getPosition()-Vec2(300,100));
        m_pHand->setTouchEnabled(true);
    }), NULL));
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                   CallFunc::create([=](){
        m_pFunnel->setOrgPositionDefault();
//        m_pFunnel->setTouchEnabled(true);
    }), NULL);
    m_pFunnel->runAction(action);
    
}


void FunnelFryScene::_showFunnelStream()
{
    AudioHelp::getInstance()->playEffect("vo_funnel_fry_pour.mp3");
    m_bFunnelMoved = true;
    scheduleOnce(schedule_selector(FunnelFryScene::_update), 4);
    m_pStream = Sprite::create(localPath("funnel_batter1.png"));
    m_pFunnel->addChild(m_pStream,-1);
    m_pStream->setAnchorPoint(Vec2(0.5, 0));
    m_pStream->setPosition(Vec2(-10, -300));
    m_pStream->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("funnel_batter"), 1, 4,false),
                                          RepeatForever::create(AnimationHelp::createAnimate(localPath("funnel_batter"), 3, 4,false)), NULL));
}

void FunnelFryScene::_showClip()
{
    m_pClip = _createDrageNode(localPath("clip1.png"));
    this->addChildToContentLayer(m_pClip);
    CMVisibleRect::setPositionAdapted(m_pClip, 350+visibleSize.width, 700);
    m_pClip->setTouchEnabled(false);
    m_pClip->setName("clip");
    m_pClip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pClip->setOrgPositionDefault();
        m_pClip->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pClip->getPosition(),m_pCake->getPosition());
    }), NULL));
}

void FunnelFryScene::_update(float)
{
    m_bFunnelMoved = false;
    _pourCake();
}
void FunnelFryScene::_pourOil()
{
    Node* pCup = Node::create();
    this->addChildToContentLayer(pCup);
    pCup->setPosition(m_pPan->getPosition()+Vec2(100, 200));
    
    ContainerNode* pContainer = ContainerNode::create(pCup);
    pContainer->addBody(localPath("oil2_down.png"));
    pContainer->addFront(localPath("oil2_on.png"));
    pContainer->addFood(localPath("oil2.png"));
    pCup->setUserData(pContainer);
    
    AudioHelp::getInstance()->playPourMilkEffect();
    Sprite* pStream = Sprite::create(localPath("oil2_1.png"));
    this->addChildToContentLayer(pStream);
    pStream->setPosition(pCup->getPosition());
    pStream->runAction(Sequence::create(Repeat::create(Sequence::create(AnimationHelp::createAnimate(localPath("oil2_"), 1, 2),
                                                                        DelayTime::create(0.2), NULL), 5),
                                        CallFunc::create([=](){
        
        pStream->removeFromParent();
    }), NULL));
    pContainer->pourFood(true,4);
    
    m_pOilInPan = Sprite::create(localPath("pan_oil.png"));
    this->addChildToContentLayer(m_pOilInPan);
    m_pOilInPan->setPosition(m_pPan->getPosition());
    m_pOilInPan->setOpacity(0);
    m_pOilInPan->setScale(0.2);
    m_pOilInPan->runAction(Sequence::create(FadeIn::create(0.5),
                                            ScaleTo::create(3, 1),
                                            CallFunc::create([=](){
        
        pCup->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
            _showThermomter();
            pCup->removeFromParent();
        }), NULL));
    }), NULL));
}

void FunnelFryScene::_pourCake()
{
    if (m_pCake) {
        return;
    }
    int index = 1;
    if (m_bFunnelMoved) {
        index = 2;
    }
    m_pCake = Sprite::create(localPath("cake"+std::to_string(index)+"_0_1.png"));
    m_pCake->setTag(index);
    this->addChildToContentLayer(m_pCake);
    m_pCake->setPosition(m_pOilInPan->getPosition()+Vec2(0, -10));
    m_pCake->setOpacity(0);
//    m_pCake->setScale(0.1);
    m_pCake->runAction(Sequence::create(DelayTime::create(1),
                                        FadeIn::create(0.5),
                                        CallFunc::create([=](){
        
        AudioHelp::getInstance()->playLoopEffect("fry.mp3");
    }),
                                        AnimationHelp::createAnimate(localPath("cake"+std::to_string(index)+"_0_"), 1, 5,false,true,1.5),
//                                        ScaleTo::create(5, 1),
                                        CallFunc::create([=](){
        m_pStream->removeFromParent();
        m_pFunnel->setTouchEnabled(false);
        m_pFunnel->runAction(MoveBy::create(1, Vec2(visibleSize.width*1.5, 0)));
        m_pGuideLayer->removeGuide();
        _doFry();
    }), NULL));
    m_pBatter->runAction(FadeOut::create(5));
}

void FunnelFryScene::_reset()
{
    for_each(m_pContentLayer->getChildren().begin(), m_pContentLayer->getChildren().end(), [=](Node* pNode){
        pNode->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    });
    this->runAction(Sequence::create(DelayTime::create(1.1),
                                     CallFunc::create([=](){
        m_pContentLayer->removeAllChildren();
        m_pCake = nullptr;
        _showPan();
        _showOilCup();
    }), NULL));
}

void  FunnelFryScene::_doFry()
{
    AudioHelp::getInstance()->playEffect("vo_funnel_fry.mp3");
    this->runAction(Sequence::create(DelayTime::create(3),
                                     CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("vo_fry_fries4.mp3");
    }), NULL));
    
    m_pCake->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                              MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
    
    ProgressBarNew2* pBar = ProgressBarNew2::create("content/common/progress/banner2.png", "content/common/progress/banner1.png", "content/common/progress/banner2.png");
    pBar->setFlag(248, 330, 3, 3, 2);
    
    pBar->progressNewState = CC_CALLBACK_2(FunnelFryScene::_onProgressCallback, this);
    this->addChildToUILayer(pBar);
//    pBar->setVisible(false);
    CMVisibleRect::setPosition(pBar, 340, 800+50);
    pBar->beginProgress(50);
    m_pBar = pBar;
    
//    m_pTimeBg = Sprite::create("content/common/progress/banner1.png");
//    this->addChildToUILayer(m_pTimeBg);
//    CMVisibleRect::setPositionAdapted(m_pTimeBg, 320, 600);
//    
//    m_pProgressTime = ProgressTimer::create(Sprite::create("content/common/progress/banner2.png"));
//    m_pTimeBg->addChild(m_pProgressTime);
//    m_pProgressTime->setPercentage(0);
//    m_pProgressTime->setType(cocos2d::ProgressTimer::Type::BAR);
//    m_pProgressTime->setPosition(Vec2(m_pTimeBg->getContentSize().width/2, m_pTimeBg->getContentSize().height/2));
//    m_nPercent = 0;
//    schedule(schedule_selector(FunnelFryScene::update), 0.5);
}

void FunnelFryScene::update(float)
{
    m_nPercent++;
    if (m_nPercent>100) {
        m_nPercent = 100;
        unschedule(schedule_selector(FunnelFryScene::update));
    }
    m_pProgressTime->setPercentage(m_nPercent);
    
    if (m_nPercent<=63) {
        for (int i = 0; i<3; i++) {
            if (63/3*i == m_nPercent) {
                std::stringstream ostr;
                ostr<<"cake"<<m_pCake->getTag()<<"_"<<i+2<<".png";
                m_pCake->setTexture(localPath(ostr.str()));
                if (i==2) {
                    _showClip();
                }
                break;
            }
        }
    }else if(m_nPercent ==84){
        std::stringstream ostr;
        ostr<<"cake"<<m_pCake->getTag()<<"_5.png";
        m_pCake->setTexture(localPath(ostr.str()));
        m_pClip->setTouchEnabled(false);
        m_pClip->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                            CallFunc::create([=](){
            m_pClip->removeFromParent();
        }), NULL));
    }else if(m_nPercent ==95){
        std::stringstream ostr;
        ostr<<"cake"<<m_pCake->getTag()<<"_6.png";
        m_pCake->setTexture(localPath(ostr.str()));
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke2.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setLocalZOrder(9);
        pParticle->setPosition(m_pCake->getPosition());
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(9);
            pParticle->setPosition(m_pCake->getPosition()-Vec2(0, 30));
            
            
            m_pCake->removeFromParent();
            //            Node* pBubble = pDragNode->getChildByName("bubble");
            //            if (pBubble) {
            //                pBubble->removeFromParent();
            //            }
            AudioHelp::getInstance()->stopLoopEffect();
            _reset();
        }), NULL));
    }
}
void FunnelFryScene::_onProgressCallback(ProgressBarNew2* pBar,int segment)
{
    if (segment<4) {
        std::stringstream ostr;
        ostr<<"cake"<<m_pCake->getTag()<<"_"<<segment<<".png";
        m_pCake->setTexture(localPath(ostr.str()));
        
        if (segment==3) {
            _showClip();
            AudioHelp::getInstance()->playEffect("fry_ok.mp3");
        }
    }else if (segment>5 && segment<7) {
        int index = 0;
        std::stringstream ostr;
        ostr<<"cake"<<m_pCake->getTag()<<"_"<<segment<<".png";
        m_pCake->setTexture(localPath(ostr.str()));
        if (segment==6) {
            m_pGuideLayer->removeGuide();
            bool isPlay = AudioHelp::getInstance()->playFryBurnedEffect();
            if (isPlay) {
                this->runAction(Sequence::create(DelayTime::create(4),
                                                 CallFunc::create([=](){
                    AudioHelp::getInstance()->stopFryBurnedEffect();
                }), NULL));
            }
            m_pClip->setTouchEnabled(false);
            m_pClip->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                CallFunc::create([=](){
                m_pClip->removeFromParent();
            }), NULL));
        }
    }else if(segment==7){
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke2.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setLocalZOrder(9);
        pParticle->setPosition(m_pOilInPan->getPosition());
        
    }else if (segment == 8) {
        //        AudioHelp::getInstance()->playEffect("fry_failed.mp3");
        
        m_pCake->runAction(FadeOut::create(0.5));
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(9);
            pParticle->setPosition(m_pOilInPan->getPosition()-Vec2(0, 30));
            
            m_pCake->removeFromParent();
            //            Node* pBubble = pDragNode->getChildByName("bubble");
            //            if (pBubble) {
            //                pBubble->removeFromParent();
            //            }
            AudioHelp::getInstance()->stopLoopEffect();
            
            _reset();
        }), NULL));
    }
}

void FunnelFryScene::_finish(){
    
    AudioHelp::getInstance()->stopLoopEffect();
    
    GameDataManager::getInstance()->m_nFunnelIndex = m_pCake->getTag();
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<FunnelDecorateScene>();
    }), NULL));
}