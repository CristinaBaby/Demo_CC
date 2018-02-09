
#include "DecHotdogScene.h"
#include "SceneManager.h"

int DecHotdogScene::ms_nSausageCount = 0;

DecHotdogScene::DecHotdogScene()
{
    m_nStep = 0;
    m_pSauce = nullptr;
    m_pSauseBottle = nullptr;
    
    m_pBowlMayonnaise = nullptr;
    m_pMayonnaise = nullptr;
    m_pSpoonIn = nullptr;
    m_pSpoon = nullptr;
    m_pScribble = nullptr;
    
    m_pBowlSeaweed = nullptr;
    m_pSeaweed = nullptr;
    
    m_pBowlGiner = nullptr;
    m_pGiner = nullptr;
    
    m_bBtnEnable = true;
}

DecHotdogScene::~DecHotdogScene()
{
}
bool DecHotdogScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg1.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->setTouchLayerEnable(false);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(DecHotdogScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(DecHotdogScene::TouchMove, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(DecHotdogScene::TouchEnded, this);

    _showHotdog();
    _showSauseBottle();
    
    m_pGameUI->showNormalLayout();
    return true;
}

void DecHotdogScene::onEnter()
{
    ExtensionScene::onEnter();
}

void DecHotdogScene::onExit()
{
    ExtensionScene::onExit();
}

void DecHotdogScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void DecHotdogScene::onButtonCallback(Button* btn)
{
    if (!m_bBtnEnable) {
        return;
    }
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagNext) {
        btn->setEnabled(false);
        m_pGameUI->hideNext();
        m_pGuideLayer->removeGuide();
        m_pGameUI->hideNext();
        
        if (m_nStep==0) {
            m_pSauce->setDrawing(false);
            m_pSauseBottle->stopAllActions();
            m_pSauseBottle->runAction(Sequence::create(RotateTo::create(0.5, 0),
                                                       MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                       NULL));
            _showMayonnaise();
        }else if (m_nStep==1) {
            m_pSpoon->setTouchEnabled(false);
            m_pBowlMayonnaise->stopAllActions();
            m_pMayonnaise->stopAllActions();
            m_pSpoon->stopAllActions();
            m_pBowlMayonnaise->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            m_pMayonnaise->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            m_pSpoon->runAction(MoveBy::create(1, Vec2(-visibleSize.width*1.5, 0)));
            _showSeaweed();
        }else if (m_nStep==2) {
            if (m_pBowlSeaweed) {
                if(m_pBowlSeaweed->getPositionX()>0){
                    m_pBowlSeaweed->setTouchEnabled(false);
                    m_pBowlSeaweed->stopAllActions();
                    m_pBowlSeaweed->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                }
            }
            _showGinger();
        }else if (m_nStep==3) {
            ms_nSausageCount++;
            _saveHotdog();
            m_bBtnEnable = false;
            m_pGameUI->hideNext();
            m_pGameUI->hideReset();
            
            if (m_pBowlGiner) {
                if(m_pBowlGiner->getPositionX()>0){
                    m_pBowlGiner->setTouchEnabled(false);
                    m_pBowlGiner->stopAllActions();
                    m_pBowlGiner->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                }
            }
            if (ms_nSausageCount>1) {
                Node* pFood = Node::create();
                Vec2 pos = m_pHotdog->getPosition();
                pFood->setPosition(Vec2(visibleSize.width, 0)+pos);
                pFood->runAction(JumpBy::create(1, Vec2(-visibleSize.width, 0), 200, 1));
                this->addChildToContentLayer(pFood);
                
                if (ms_nSausageCount==3) {
                    m_pHotdog->runAction(MoveBy::create(0.5, Vec2(0, -100)));
                    m_pClipping->runAction(MoveBy::create(0.5, Vec2(0, -100)));
                }
                for (int i = 0; i<ms_nSausageCount-1; i++) {
                    
                    std::string name = "hotdog"+std::to_string(i+1)+".png";
                    FileUtils::getInstance()->getWritablePath()+ name;
                    Sprite* pHotdog = Sprite::create("content/make/japanese/decoration/hotdog.png");
                    pFood->addChild(pHotdog);
                    pHotdog->setPosition(Vec2(0,100-100*(i%3)));
                    
                    Sprite* pDec = Sprite::create(FileUtils::getInstance()->getWritablePath()+ name);
                    pFood->addChild(pDec);
                    pDec->setPosition(Vec2(0,100-100*(i%3)));
                }
            }
            if(ms_nSausageCount==1 || ms_nSausageCount==2){
                AudioHelp::getInstance()->playEffect("vo_make_another.mp3");
            }
            m_pHotdog->setLocalZOrder(5);
            m_pClipping->setLocalZOrder(5);
            
            if (ms_nSausageCount==3) {
                ms_nSausageCount = 0;
                AudioHelp::getInstance()->stopEffect("vo_japanese_add_syrup.mp3");
                AudioHelp::getInstance()->stopEffect("vo_seaweed.mp3");
                AudioHelp::getInstance()->stopEffect("vo_ginger.mp3");
                
                AudioHelp::getInstance()->playEffect("done.mp3");
                AudioHelp::getInstance()->_playEffectNiceWork();
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                this->addChildToUILayer(pParticle);
                pParticle->setLocalZOrder(10);
                pParticle->setPosition(visibleSize*0.5);
                this->runAction(Sequence::create(DelayTime::create(2),
                                                 CallFunc::create([=](){
                    SceneManager::replaceTheScene<ShareScene>();
                }), NULL));
            }else{
                this->runAction(Sequence::create(DelayTime::create(2),
                                                 CallFunc::create([=](){
                    SceneManager::replaceTheScene<DecHotdogScene>();
                }), NULL));
            }
        }
        m_nStep++;
        return;
    }else if (tag==GameUILayoutLayer::eUIButtonTagReset) {
        btn->setEnabled(false);
        m_pGameUI->hideReset();
        _reset();
    }
    ExtensionScene::onButtonCallback(btn);
}

void DecHotdogScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    
    std::string name = pDragNode->getName();
    if (name == "drawSauce"){
        if (pDragNode->index ==0) {
            pDragNode->index = 1;
            pDragNode->setPosition(pDragNode->getPosition()+Vec2(0, 100));
            pDragNode->runAction(RotateBy::create(0.5, -160));
        }else{
            m_pSauce->paint(worldPoint);
        }
    }
    
}

void DecHotdogScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="spoon") {
        int index = pDragNode->index;
        if (index==0) {
            pDragNode->index = 1;
            m_pMayonnaise->runAction(ScaleBy::create(0.5, 0.8));
            m_pMayonnaise->runAction(RotateBy::create(0.5, 60));
            
            Rect bowlRect = m_pBowlMayonnaise->getBoundingBox();
            bowlRect.origin = m_pBowlMayonnaise->getParent()->convertToWorldSpace(bowlRect.origin);
            if (bowlRect.containsPoint(worldPoint)) {
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pBowlMayonnaise->getPosition()+Vec2(60, 50));
                pDragNode->runAction(Sequence::create(RotateBy::create(0.3, -30),
                                                      CallFunc::create([=](){
                    pDragNode->getDragSprite()->setTexture(localPath("decoration/mayonnaise_spoon1.png"));
                }),
                                                      DelayTime::create(0.5),
                                                      CallFunc::create([=](){
                    pDragNode->getDragSprite()->setTexture(localPath("decoration/mayonnaise_spoon.png"));
                    ClippingNode* pClipping = ClippingNode::create(Sprite::create(localPath("decoration/mayonnaise7_mask.png")));
                    pDragNode->addChild(pClipping);
                    pClipping->setAlphaThreshold(0.5);
                    
                    m_pSpoonIn = Sprite::create(localPath("decoration/mayonnaise1.png"));
                    pClipping->addChild(m_pSpoonIn);
                    
                    pDragNode->runAction(RotateBy::create(0.3, 30));
                    pDragNode->setTouchEnabled(true);
                }), NULL));
                
            }
        }else{
            Rect rect = m_pHotdog->getBoundingBox();
            rect.origin = m_pHotdog->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                pDragNode->setTouchEnabled(false);
                m_bBtnEnable = false;
                m_pSpoonStream = Sprite::create(localPath("decoration/mayonnaise1.png"));
                m_pSpoon->addChild(m_pSpoonStream);
                m_pSpoonStream->runAction(AnimationHelp::createAnimate(localPath("decoration/mayonnaise"), 2, 7,false));
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pHotdog->getPosition()+Vec2(-100, 200)),
                                                      CallFunc::create([=](){
                    m_pSpoonIn->runAction(MoveBy::create(5, Vec2(0, -90)));
                    Vec2 beginPos = Vec2(44,130)-Vec2(m_pHotdog->getContentSize().width/2,m_pHotdog->getContentSize().height/2);
                    beginPos = m_pScribble->convertToWorldSpace(beginPos);
                    int count = 0;
                    for (int i = 0; i<6; i++) {
                        std::vector<Vec2> posVector = interpolate(beginPos,beginPos+Vec2(48, -94),4);
                        for_each(posVector.begin(), posVector.end(), [=,&count](Vec2 pos){
                            m_pScribble->runAction(Sequence::create(DelayTime::create(0.015*count),
                                                                    CallFunc::create([=](){
                                m_pScribble->paint(pos);
                                pDragNode->setPosition(pos+Vec2(80, 200));
                            }), NULL));
                            count++;
                        });
                        beginPos = beginPos+Vec2(48, -95);
                        std::vector<Vec2> posVector2 = interpolate(beginPos,beginPos+Vec2(18, 94),4);
                        for_each(posVector2.begin(), posVector2.end(), [=,&count](Vec2 pos){
                            m_pScribble->runAction(Sequence::create(DelayTime::create(0.05+0.015*count),
                                                                    CallFunc::create([=](){
                                m_pScribble->paint(pos);
                                pDragNode->setPosition(pos+Vec2(80, 200));
                            }), NULL));
                            count++;
                        });
                        beginPos = beginPos+Vec2(8, 96);
                        
                    }
                    
                }),
                                                      DelayTime::create(4.8),
//                                                      Repeat::create(Sequence::create(MoveBy::create(0.4,Vec2(48, -95)),
//                                                                                      MoveBy::create(0.3,Vec2(8, 96)), NULL), 6),
//                                                      MoveBy::create(0.4,Vec2(20, -40)),
                                                      CallFunc::create([=](){
                    m_pScribble->getCanvas()->clear(0, 0, 0, 0);
                    m_pScribble->paint(localPath("decoration/mayonnaise.png"));
                    
                    m_bBtnEnable = true;
                    m_pSpoon->back();
                    m_pSpoonIn->removeFromParent();
                    m_pSpoonStream->removeFromParent();
                    m_pGameUI->hideNext();
//                    m_pGameUI->showNextLayout();
                    onButtonCallback(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagNext));
                }), NULL));
            }
        }
    }else if (name == "drawSauce"){
        m_pGuideLayer->removeGuide();
        m_pSauce->paint(worldPoint-pDragNode->getOffset(), worldPoint);
        m_pGameUI->showNextLayout();
        m_pGameUI->showResetLayout();
    }
    
}

void DecHotdogScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
//    pDragNode->stopAllActions();
 
    std::string name = pDragNode->getName();
    if (name=="sake") {
        pDragNode->setTouchEnabled(false);
        pDragNode->setVisible(false);
        pDragNode->back();

    }else if (name == "seaweed") {
        Rect rect = m_pPlate->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setTouchEnabled(false);
            _pourSeaweed();
        }else{
            _back(pDragNode);
        }
    }else if (name == "ginger") {
        Rect rect = m_pPlate->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setTouchEnabled(false);
            _pourGinger();
        }else{
            _back(pDragNode);
        }
    }
}

void DecHotdogScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->stopAllActions();
    
    _back(pDragNode);
}

void DecHotdogScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    switch (m_nStep) {
        case 0:
        {
            
        }
            break;
        case 1:
        {
            
        }
            break;
        case 2:
        {
            
        }
            break;
        case 3:
        {
            
        }
            break;
            
        default:
            break;
    }
}

void DecHotdogScene::TouchMove(Touch *pTouch, Event *pEvent)
{
    switch (m_nStep) {
        case 0:
        {
            
        }
            break;
        case 1:
        {
            
        }
            break;
        case 2:
        {
            
        }
            break;
        case 3:
        {
            
        }
            break;
            
        default:
            break;
    }
}

void DecHotdogScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    
}
#pragma mark - initData
void DecHotdogScene::_initData()
{
    setExPath("content/make/japanese/");
    
    m_nNextSceneTag = GameUIEvent::eSceneTag;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTag;
}


#pragma mark ======== deel ========================
void DecHotdogScene::_back(DragNode* pDragNode)
{
    pDragNode->back(0,[=](){
        ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
        if (pContainer) {
            pContainer->showShadow();
        }
    });
}

void DecHotdogScene::_reset()
{
    m_pGameUI->hideNext();
    m_nStep = 0;
    if (m_pSauce) {
        m_pSauce->setDrawing(false);
        m_pSauce->reset();
    }
    if (m_pSauseBottle) {
        m_pSauseBottle->setTouchEnabled(false);
        m_pSauseBottle->runAction(Sequence::create(RotateTo::create(0.3, 0),
                                                   MoveBy::create(0.7, Vec2(visibleSize.width+m_pSauseBottle->getContentSize().width, 0)), NULL));
    }
    if (m_pBowlMayonnaise) {
        m_pBowlMayonnaise->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    }
    if (m_pMayonnaise) {
        m_pMayonnaise->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    }
    if (m_pScribble) {
        if(m_pScribble->getCanvas() != nullptr){
            m_pScribble->getCanvas()->clear(0, 0, 0, 0);
        }
    }
    if (m_pSpoon) {
        m_pSpoon->setTouchEnabled(false);
        m_pSpoon->runAction(MoveBy::create(1, Vec2(-visibleSize.width*1.5, 0)));
        m_pSpoon->getDragSprite()->setTexture(localPath("decoration/mayonnaise_spoon.png"));
        Vector<Node*> pChildren = m_pSpoon->getChildren();
        for_each(pChildren.begin(), pChildren.end(), [=](Node* pNode){
            if (pNode!= m_pSpoon->getDragSprite()) {
                pNode->removeFromParent();
            }
        });
        
        m_pSpoon->index = 0;
    }
    if (m_pBowlSeaweed) {
        m_pBowlSeaweed->setTouchEnabled(false);
        m_pBowlSeaweed->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    }
    if (m_pSeaweed) {
        m_pSeaweed->removeFromParent();
        m_pSeaweed = nullptr;
    }
    if (m_pGiner) {
        m_pGiner->removeFromParent();
        m_pGiner = nullptr;
    }
    if (m_pBowlGiner) {
        m_pBowlGiner->setTouchEnabled(false);
        m_pBowlGiner->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    }
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
                                         _showSauseBottle();
                                     }), NULL));
}

void DecHotdogScene::_showHotdog()
{
    m_pPlate = Sprite::create(localPath("decoration/plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320, 400);
    
    m_pHotdog = Sprite::create(localPath("decoration/hotdog.png"));
    this->addChildToContentLayer(m_pHotdog);
    m_pHotdog->setPosition(m_pPlate->getPosition());
    
    
    m_pClipping = ClippingNode::create(Sprite::create(localPath("decoration/hotdog.png")));
    this->addChildToContentLayer(m_pClipping);
    m_pClipping->setAlphaThreshold(0.5);
    m_pClipping->setPosition(m_pHotdog->getPosition());
}

void DecHotdogScene::_showSauseBottle()
{
    AudioHelp::getInstance()->stopEffect("vo_japanese_add_syrup.mp3");
    AudioHelp::getInstance()->stopEffect("vo_seaweed.mp3");
    AudioHelp::getInstance()->stopEffect("vo_ginger.mp3");
    
    if (!m_pSauseBottle) {
        m_pSauseBottle = _createDrageNode(localPath("decoration/sauce.png"));
        this->addChildToUILayer(m_pSauseBottle);
        m_pSauseBottle->setName("drawSauce");
    }
    m_pSauseBottle->index = 0;
    m_pSauseBottle->setActionPoint(Vec2(63, 305));
    m_pSauseBottle->setRotation(0);
    CMVisibleRect::setPositionAdapted(m_pSauseBottle, 380+visibleSize.width, 400+350);
    
    if(m_pSauce == nullptr){
        m_pSauce = ScribbleSauce::create(m_pHotdog->getContentSize());
        m_pClipping->addChild(m_pSauce,0);
        m_pSauce->setPosition(Vec2(-m_pHotdog->getContentSize().width/2, -m_pHotdog->getContentSize().height/2));
    }
    m_pSauseBottle->setTouchEnabled(false);
    m_pSauseBottle->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
//                                               RotateBy::create(0.5, -160),
                                               CallFunc::create([=](){
        m_pSauseBottle->setOrgPositionDefault();
        m_pSauseBottle->setTouchEnabled(true);
//        m_pGuideLayer->showGuideTapHorizonal(m_pHotdog->getPosition());
        m_pGuideLayer->showGuideMove(m_pSauseBottle->getPosition(), m_pHotdog->getPosition());
        if (ms_nSausageCount>0){
            m_pGameUI->showNextLayout();
        }
        
        m_pSauce->setBrush(Sprite::create(localPath("decoration/sauce_1.png")));

    }), NULL));
}

void DecHotdogScene::_showMayonnaise()
{
    m_pGameUI->showResetLayout();
    AudioHelp::getInstance()->stopEffect("vo_japanese_add_syrup.mp3");
    AudioHelp::getInstance()->stopEffect("vo_seaweed.mp3");
    AudioHelp::getInstance()->stopEffect("vo_ginger.mp3");
    if (!m_pBowlMayonnaise) {
        m_pBowlMayonnaise = Sprite::create(localPath("decoration/mayonnaise_plate.png"));
        this->addChildToContentLayer(m_pBowlMayonnaise);
    }
    CMVisibleRect::setPositionAdapted(m_pBowlMayonnaise, 200-visibleSize.width, 700);
    
    if (!m_pMayonnaise){
        m_pMayonnaise = Sprite::create(localPath("decoration/mayonnaise_1.png"));
        this->addChildToContentLayer(m_pMayonnaise);
    }
    m_pMayonnaise->setRotation(0);
    m_pMayonnaise->setScale(1);
    m_pMayonnaise->setPosition(m_pBowlMayonnaise->getPosition());
    
    if (!m_pSpoon){
        m_pSpoon = _createDrageNode(localPath("decoration/mayonnaise_spoon.png"));
        this->addChildToContentLayer(m_pSpoon);
        m_pSpoon->setName("spoon");
    }
    m_pSpoon->index = 0;
    CMVisibleRect::setPositionAdapted(m_pSpoon, 200+visibleSize.width, 700);
    
    if(!m_pScribble) {
        m_pScribble = ScribbleNode::create(m_pHotdog->getContentSize());
        m_pScribble->useTarget(localPath("decoration/mayonnaise.png"));
        m_pScribble->useBrush("content/brush.png",Scribble::eBrush,1);
        m_pClipping->addChild(m_pScribble,1);
//        this->addChildToContentLayer(m_pScribble);
//        m_pScribble->setPosition(m_pHotdog->getPosition());
    }
    m_pBowlMayonnaise->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pMayonnaise->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    m_pSpoon->setTouchEnabled(false);
    m_pSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pSpoon->setOrgPositionDefault();
        m_pSpoon->setTouchEnabled(true);
        if (ms_nSausageCount>0){
            m_pGameUI->showNextLayout();
        }
        AudioHelp::getInstance()->playEffect("vo_japanese_add_syrup.mp3");
    }), NULL));
    
    
}

void DecHotdogScene::_showSeaweed()
{
    AudioHelp::getInstance()->stopEffect("vo_japanese_add_syrup.mp3");
    AudioHelp::getInstance()->stopEffect("vo_seaweed.mp3");
    AudioHelp::getInstance()->stopEffect("vo_ginger.mp3");
    if(m_pBowlSeaweed) {
        m_pBowlSeaweed->removeFromParent();
    }
    m_pBowlSeaweed = _createDrageNode(localPath("decoration/aonori_1_plate.png"));
    this->addChildToContentLayer(m_pBowlSeaweed);
    m_pBowlSeaweed->setTouchEnabled(false);
    m_pBowlSeaweed->setName("seaweed");
    
    ContainerNode* pSeaweed = ContainerNode::create(m_pBowlSeaweed);
    pSeaweed->addFood(localPath("decoration/aonori_1.png"));
    m_pBowlSeaweed->setUserData(pSeaweed);
    CMVisibleRect::setPositionAdapted(m_pBowlSeaweed, 150-visibleSize.width, 650);
    
    m_pBowlSeaweed->setTouchEnabled(false);
    m_pBowlSeaweed->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                               CallFunc::create([=](){
        m_pBowlSeaweed->setOrgPositionDefault();
        m_pBowlSeaweed->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pBowlSeaweed->getPosition(), m_pHotdog->getPosition());
        if (ms_nSausageCount>0){
            m_pGameUI->showNextLayout();
        }
        AudioHelp::getInstance()->playEffect("vo_seaweed.mp3");
    }), NULL));
}

void DecHotdogScene::_showGinger()
{
    AudioHelp::getInstance()->stopEffect("vo_japanese_add_syrup.mp3");
    AudioHelp::getInstance()->stopEffect("vo_seaweed.mp3");
    AudioHelp::getInstance()->stopEffect("vo_ginger.mp3");
    if(m_pBowlGiner) {
        m_pBowlGiner->removeFromParent();
    }
    m_pBowlGiner = _createDrageNode(localPath("decoration/beni_shoga_plate.png"));
    this->addChildToContentLayer(m_pBowlGiner);
    m_pBowlGiner->setTouchEnabled(false);
    m_pBowlGiner->setName("ginger");
    
    ContainerNode* pGiner = ContainerNode::create(m_pBowlGiner);
    pGiner->addFood(localPath("decoration/beni_shoga_1.png"));
    m_pBowlGiner->setUserData(pGiner);
    CMVisibleRect::setPositionAdapted(m_pBowlGiner, 150-visibleSize.width, 650);
    
    m_pBowlGiner->setTouchEnabled(false);
    m_pBowlGiner->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                               CallFunc::create([=](){
        m_pBowlGiner->setOrgPositionDefault();
        m_pBowlGiner->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pBowlGiner->getPosition(), m_pHotdog->getPosition());
        if (ms_nSausageCount>0){
            m_pGameUI->showNextLayout();
        }
        AudioHelp::getInstance()->playEffect("vo_ginger.mp3");
    }), NULL));
}
vector<Vec2> DecHotdogScene::interpolate(Point pStartPosition, Point pEndPosition, float pDelta)
{
    float lDistance = pStartPosition.getDistance(pEndPosition);
    int lDis = (int)lDistance;
    vector<Vec2> lPositions;
    for (int i = 0; i < lDis; i += pDelta) {
        float lDelta = float(i) / lDistance;
        float lDifX = pEndPosition.x - pStartPosition.x;
        float lDifY = pEndPosition.y - pStartPosition.y;
        
        Point lPosition(pStartPosition.x + (lDifX * lDelta), pStartPosition.y + (lDifY * lDelta));
        lPositions.push_back(lPosition);
    }
    return lPositions;
}

void DecHotdogScene::_pourSeaweed()
{
    m_bBtnEnable = false;
    m_pBowlSeaweed->setPosition(Vec2(m_pPlate->getPositionX()+50, m_pPlate->getPositionY()+200));
    m_pBowlSeaweed->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                            CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("pour.mp3");
        ContainerNode* pContainer = (ContainerNode*)m_pBowlSeaweed->getUserData();
        if (pContainer) {
            pContainer->pourFood();
        }
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/seaweed.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setPosition(Vec2(-20, -50)+m_pBowlSeaweed->getPosition());
        
        for (int i = 0; i<12; i++) {
            Sprite* pItem = Sprite::create(localPath("decoration/aonori_2_"+std::to_string(i%6+1)+".png"));
            this->addChildToContentLayer(pItem);
            pItem->setPosition(m_pBowlSeaweed->getPosition());
            pItem->setLocalZOrder(10);
            pItem->setRotation(i*20);
            pItem->runAction(Sequence::create(DelayTime::create(0.01+0.1*i),
                                                 Spawn::create(MoveBy::create(0.5, Vec2(-50, -100)),
                                                               RotateBy::create(0.5, 10+i*10), NULL),
                                                 CallFunc::create([=](){
                pItem->removeFromParent();
            }), NULL));
        }
        
        Sprite* pSeaweed = Sprite::create(localPath("decoration/aonori.png"));
        m_pClipping->addChild(pSeaweed,3);
        pSeaweed->setOpacity(0);
        pSeaweed->runAction(FadeIn::create(1.5));
        m_pSeaweed = pSeaweed;
    }),
                                            DelayTime::create(3),
                                            RotateBy::create(0.3, 30),
                                            MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                               CallFunc::create([=](){
        m_bBtnEnable = true;
        m_pGameUI->hideNext();
//        m_pGameUI->showNextLayout();
        onButtonCallback(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagNext));
    }),NULL));

}

void DecHotdogScene::_pourGinger()
{
    m_bBtnEnable = false;
    m_pBowlGiner->setPosition(Vec2(m_pPlate->getPositionX()+50, m_pPlate->getPositionY()+200));
    m_pBowlGiner->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                             CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("pour.mp3");
        ContainerNode* pContainer = (ContainerNode*)m_pBowlGiner->getUserData();
        if (pContainer) {
            pContainer->pourFood();
        }
        
        for (int i = 0; i<18; i++) {
            Sprite* pItem = Sprite::create(localPath("decoration/beni_shoga_2_"+std::to_string(i%6+1)+".png"));
            this->addChildToContentLayer(pItem);
            pItem->setPosition(m_pBowlGiner->getPosition());
            pItem->setLocalZOrder(10);
            pItem->setRotation(i*20);
            pItem->runAction(Sequence::create(DelayTime::create(0.01+0.08*i),
                                              Spawn::create(MoveBy::create(0.5, Vec2(-50, -100)),
                                                            RotateBy::create(0.5, 10+i*10), NULL),
                                              CallFunc::create([=](){
                pItem->removeFromParent();
            }), NULL));
        }
        
        Sprite* pGinger = Sprite::create(localPath("decoration/beni_shoga.png"));
        m_pClipping->addChild(pGinger,4);
        pGinger->setOpacity(0);
        pGinger->runAction(FadeIn::create(1.5));
        m_pGiner = pGinger;
    }),
                                               DelayTime::create(3),
                                               RotateBy::create(0.3, 30),
                                               MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                             CallFunc::create([=](){
        m_bBtnEnable = true;
        m_pGameUI->hideNext();
//        m_pGameUI->showNextLayout();
        onButtonCallback(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagNext));
    }),NULL));
}
void DecHotdogScene::_finishAll()
{
}

void DecHotdogScene::_saveHotdog()
{
    Size imageSize = m_pHotdog->getContentSize();
    RenderTexture* render = RenderTexture::create(imageSize.width, imageSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    Vec2 posOrigin = m_pClipping->getPosition();
    
    Vec2 pos = Vec2(imageSize.width/2,imageSize.height/2);
    
    m_pClipping->setPosition(pos);
//    m_pClipping->setScale(1, 0.9);
//    render->beginWithClear(0, 0, 0, 0);
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pClipping->Node::visit();
    render->end();
    
    m_pClipping->setPosition(posOrigin);
//    m_pClipping->setScale(1);
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    Image* pImage = render->newImage();
    
    bool issuccess;
    
    std::string name = "hotdog"+std::to_string(ms_nSausageCount)+".png";
    
//    render->saveToFile(FileUtils::getInstance()->getWritablePath()+ name);
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+ name, false);
    pImage->autorelease();
}