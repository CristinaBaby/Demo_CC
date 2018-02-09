
#include "BaconHamFryHamScene.h"
#include "SceneManager.h"

BaconHamFryHamScene::BaconHamFryHamScene()
{
    m_nMoveCount = 0;
    m_pButter = nullptr;
    m_pPlate = nullptr;
    m_bEffectTurn = false;
    m_bEffectBaconOK = false;
}

BaconHamFryHamScene::~BaconHamFryHamScene()
{
    
}
bool BaconHamFryHamScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create("content/make/common/bg1.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showStove();
    _showPan();
    
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
        _showButter();
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void BaconHamFryHamScene::onEnter()
{
    ExtensionScene::onEnter();
}

void BaconHamFryHamScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BaconHamFryHamScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "ingredient"){
        int tag = pDragNode->getTag();
        IngredientManager::getInstance()->showShadow(tag,false);
    }else if(name == "clip"){
        if (pDragNode->index == 0) {
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("clip1_down.png")));
            Sprite* pClipOn = Sprite::create(localPath("clip1_on.png"));
            pDragNode->addChild(pClipOn,20);
            pClipOn->setName("clip_on");
        }
    }
}

void BaconHamFryHamScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "butter")
    {
        int index = pDragNode->index;
        Rect rect = m_pPan->getBoundingBox();
        if (index == 0) {
            if (rect.containsPoint(worldPoint)) {
                rect.origin = rect.origin+Vec2(100, 100);
                rect.size = Size(250,160);
                pDragNode->setLimitRect(rect);
                pDragNode->index = 1;
            }
        }else if (index == 1){
            pDragNode->setDragSprite(Sprite::create("content/make/egg/make1/pan_butter2.png"));
            AudioHelp::getInstance()->playLoopEffect("pouring_sauce.mp3");
            pDragNode->index = 2;
        }else if (index == 2){
            m_pButterScribble->paint(worldPoint+Vec2(-50, 0)+pDragNode->getOffset(),worldPoint+Vec2(-50, 0));
            m_pButterScribble->paint(worldPoint+pDragNode->getOffset(),worldPoint);
            m_pButterScribble->paint(worldPoint+Vec2(50, 0)+pDragNode->getOffset(),worldPoint+Vec2(-50, 0));
            Director::getInstance()->getRenderer()->render();
            AudioHelp::getInstance()->playLoopEffect("pouring_sauce.mp3");
            _paintButter();
        }
        
    }else if(name == "clip"){
        int index = pDragNode->index;
        if (index == 1) {
#pragma mark            夹起生的bacon
            Rect rect = m_pBaconPlate->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index = 2;
                Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                if (pOn) {
                    pOn->setTexture(localPath("clip2_on.png"));
                }
                AudioHelp::getInstance()->playEffect("drag_corn.mp3");
                Sprite* pBacon = Sprite::create(localPath("jam.png"));
                pDragNode->addChild(pBacon);
                pBacon->setName("jam");
                pBacon->setPosition(pDragNode->convertToNodeSpace(worldPoint));
                
                Sprite* pBaconInPlate = m_pBaconVector.at(0);
                auto iter = std::find(m_pBaconVector.begin(), m_pBaconVector.end(), pBaconInPlate);
                if (iter != m_pBaconVector.end())
                {
                    m_pBaconVector.erase(iter);
                }
                pBaconInPlate->setVisible(false);
                
                m_pGuideLayer->removeGuide();
                m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPanIn->getPosition());
            }
        }else if (index==2) {
#pragma mark            将bacon放锅里煎
            Rect rect = m_pPan->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index = 1;
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(Sequence::create(MoveTo::create(0.5, m_pPan->getPosition()+Vec2(0, 50)),
                                                      CallFunc::create([=](){
                    auto pMilkDrop = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
                    if (!pMilkDrop) {
                        auto pParticle = ParticleSystemQuad::create("particle/smoke1.plist");
                        this->addChildToContentLayer(pParticle);
                        pParticle->setPosition(m_pPanIn->getPosition());
                        pParticle->setName("smoke");
                    }
                    
                    Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                    if (pOn) {
                        pOn->setTexture(localPath("clip1_on.png"));
                    }
                    AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
                    Sprite* pBacon = (Sprite*)pDragNode->getChildByName("jam");
                    if (pBacon) {
                        pBacon->removeFromParent();
                    }
//                    pDragNode->setTouchEnabled(true);
                    int size = m_pBaconVector.size();
                    FriedUnit* pFriedBacon = FriedUnit::create();
                    pFriedBacon->setFood(localPath("jam.png"));
                    pFriedBacon->addParticle("particle/fry_bubble1.plist");
                    pFriedBacon->addParticle("particle/fry_bubble2.plist");
                    pFriedBacon->setTag(m_pBaconVector.size());
                    this->addChildToContentLayer(pFriedBacon);
                    pFriedBacon->setPosition(m_pPanIn->getPosition());
                    log("=======%f,%f",pFriedBacon->getPositionX(),pFriedBacon->getPositionY());
                    m_pBaconFriedVector.push_back(pFriedBacon);
                    
                    _doFryBacon(pFriedBacon);
                    
                    m_pGuideLayer->removeGuide();
                    if (m_pBaconVector.size()==0) {
                        m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
//                        m_pClip->setTouchEnabled(false);
//                        m_pClip->runAction(MoveBy::create(1, Vec2(visibleSize.width*1.5, 0)));
                        AudioHelp::getInstance()->_playEffectNiceWork();
                    }
                }),
                                                      MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
            }
            
        }else if (index==3) {
#pragma mark            夹起锅里bacon （翻面或放入盘子）
            for (auto pFriedBacon:m_pBaconFriedVector) {
                if (pFriedBacon->flag==0) {
                    continue;
                }
                Rect rect = pFriedBacon->getRectWorld();
                if (rect.containsPoint(worldPoint)) {
                    if (pFriedBacon->flag == 1) {
                        pDragNode->index = 5;
                        
                    }else if(pFriedBacon->flag == 2) {
                        if (!pFriedBacon->isFriedOK) {
                            continue;
                        }
                        pDragNode->index = 4;
                    }
                    AudioHelp::getInstance()->playEffect("drag_corn.mp3");
                    pDragNode->setTouchEnabled(false);
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pFriedBacon->getPosition()+Vec2(100, 100)),
                                                          CallFunc::create([=](){
                        pDragNode->index = 4;
                        Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                        if (pOn) {
                            pOn->setTexture(localPath("clip2_on.png"));
                        }
                        Sprite* pBacon;
                        m_pGuideLayer->removeGuide();
                        if (pFriedBacon->flag == 1) {
                            pDragNode->index = 5;
                            pBacon = Sprite::create(localPath("jam2.png"));
                            
                        }else if(pFriedBacon->flag == 2) {
                            pDragNode->index = 4;
                            pBacon = Sprite::create(localPath("jam5.png"));
                            
                            auto iter = std::find(m_pBaconFriedVector.begin(), m_pBaconFriedVector.end(), pFriedBacon);
                            if (iter != m_pBaconFriedVector.end())
                            {
                                m_pBaconFriedVector.erase(iter);
                            }
                            m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPlate->getPosition());
                        }
                        pDragNode->addChild(pBacon);
                        pBacon->setName("jam");
                        pBacon->setTag(pFriedBacon->getTag());
                        pBacon->setPosition(pDragNode->convertToNodeSpace(worldPoint));
                        
                        pFriedBacon->stopFry();
                        pFriedBacon->setVisible(false);
                        
                        pDragNode->setTouchEnabled(true);
                    }), NULL));
                    break;
                }
            }
            
        }else if (index==4) {
#pragma mark            将煎好的bacon放盘子
            Rect rect = m_pPlate->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                Node* pBacon = pDragNode->getChildByName("jam");
                int tag = pBacon->getTag();
                pBacon->removeFromParent();
                AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
                
                int count = m_pBaconVector.size();
                
                Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                if (pOn) {
                    pOn->setTexture(localPath("clip1_on.png"));
                }
                
                Sprite* pBaconInPlate = Sprite::create(localPath("jam5.png"));
                m_pPlate->addChild(pBaconInPlate);
                
                pBaconInPlate->setPosition(m_pPlate->getAnchorPointInPoints()+Vec2(0, 60-20*(2-count)));
                pBaconInPlate->setScale(0.6);
                pDragNode->index = 1;
                m_pGuideLayer->removeGuide();
                
                auto pParticle = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
                if (pParticle) {
                    pParticle->stopSystem();
                    pParticle->removeFromParent();
                }
                if (m_pBaconVector.size() == 0) {
                    //finish fry bacon
                    m_pClip->setTouchEnabled(false);
                    m_pClip->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
//                    m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                    _finish();
                }else{
                    m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pBaconPlate->getPosition());
                    m_pClip->setTouchEnabled(true);
                }
            }
        }
    }
}

void BaconHamFryHamScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        AudioHelp::getInstance()->stopLoopEffect();
        Rect rectBowl = m_pPan->getBoundingBox();
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)){
            m_pGuideLayer->removeGuide();
            _addButter(pDragNode);
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),[=](){
                IngredientManager::getInstance()->showShadow(pDragNode->getTag(),true);
                pDragNode->setTouchEnabled(true);
            });
        }
        
    }else if(name == "clip"){
#pragma mark        bacon翻面
        if (pDragNode->index==5) {
            pDragNode->setTouchEnabled(false);
            Sprite* pBacon = (Sprite*)pDragNode->getChildByName("jam");
            for (auto pFriedBacon:m_pBaconFriedVector){
                int tag = pFriedBacon->getTag();
                if (tag == pBacon->getTag()) {
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pFriedBacon->getPosition()+Vec2(60, 100)),
                                                          CallFunc::create([=](){
                        Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                        if (pOn) {
                            pOn->setTexture(localPath("clip1_on.png"));
                        }
                        pFriedBacon->setVisible(true);
                        pFriedBacon->flag = 2;
                        pFriedBacon->setLocalZOrder(5);
                        pFriedBacon->getFood()->runAction(JumpBy::create(1, Vec2::ZERO, 100, 1));
                        pFriedBacon->getFood()->runAction(Sequence::create(RotateBy::create(0.5, Vec3(0, 90, 0)),
                                                                           CallFunc::create([=](){
                            pFriedBacon->getFood()->setTexture(localPath("jam3.png"));
                        }),
                                                                           RotateBy::create(0.5, Vec3(0, 90, 0)),
                                                                           CallFunc::create([=](){
                            pFriedBacon->setLocalZOrder(0);
                            _doFryBacon(pFriedBacon);
                        }),
                                                                           NULL));
                        if (pBacon) {
                            pBacon->removeFromParent();
                        }
                        
                        pDragNode->index = 3;
//                        pDragNode->setTouchEnabled(true);
                    }),
                                                          MoveBy::create(1, Vec2(visibleSize.width, 0)),  NULL));
                    AudioHelp::getInstance()->playEffect("vo_fabulous.mp3");
                    break;
                }
            }
        }
    }
}

void BaconHamFryHamScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
    dragNodeTouchEnded(pDragNode,worldPoint);
}

#pragma mark - initData
void BaconHamFryHamScene::_initData()
{
    setExPath("content/make/bacon/");
}
void BaconHamFryHamScene::_showStove()
{
    m_pStove = Sprite::create("content/make/common/stove/stove.png");
    this->addChildToContentLayer(m_pStove);
    CMVisibleRect::setPositionAdapted(m_pStove, 320, 320);
    
    m_pFire = Sprite::create("content/make/common/stove/stove_1.png");
    this->addChildToContentLayer(m_pFire);
    m_pFire->setPosition(m_pStove->getPosition()+Vec2(0,-10));
    
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/common/stove/stove_", 1, 2)));
}


void BaconHamFryHamScene::_showPan()
{
    m_pPan = Sprite::create("content/make/common/stove/pan_down.png");
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320+150+visibleSize.width, 420);
    
    m_pPanFront = Sprite::create("content/make/common/stove/pan_on.png");
    this->addChildToContentLayer(m_pPanFront);
    m_pPanFront->setPosition(m_pPan->getPosition());
    m_pPanFront->setLocalZOrder(5);
    
    m_pPan->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    m_pPanFront->runAction(Sequence::create(DelayTime::create(0.5),
                                            MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    
    m_pPanIn = Node::create();
    this->addChildToContentLayer(m_pPanIn);
    CMVisibleRect::setPositionAdapted(m_pPanIn, 320, 400);
    
    Sprite* pButter = Sprite::create("content/make/egg/make1/pan_butter0.png");
    pButter->setPosition(pButter->getContentSize()*0.5);
    m_pButterScribble = ScribbleNode::create(pButter->getContentSize());
    m_pButterScribble->useBrush("content/brush.png",Scribble::BrushType::eBrush);
    m_pPanIn->addChild(m_pButterScribble);
    m_pButterScribble->antiAliasing();
    m_pButterScribble->useTarget(pButter);
}

void BaconHamFryHamScene::_showButter()
{
    //    back shadow front food food2
    static std::string sIngredent[5] =
    {"butter_plate1.png","butter_plate2.png","","butter.png","butter_shadow.png"
        //batter
    };
    AudioHelp::getInstance()->playEffect("vo_add_butter_pan.mp3");
    
    DragNode* pDragNode = _createDrageNode("content/make/egg/make1/butter_plate1.png");
    this->addChildToContentLayer(pDragNode);
    pDragNode->setName("ingredient");
    pDragNode->setTouchEnabled(true);
    pDragNode->setActionPoint(Vec2(0, 0));
    CMVisibleRect::setPositionAdapted(pDragNode, Vec2(320, 690)+Vec2(visibleSize.width,0));
    
    IngredientManager::getInstance()->addIngredient(pDragNode);
    IngredientManager::getInstance()->addContainer(pDragNode,"content/make/egg/make1/", sIngredent);
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    pDragNode->runAction(action);
}

void BaconHamFryHamScene::_showHams()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    AudioHelp::getInstance()->playEffect("vo_fry3hams.mp3");
    m_pBaconPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pBaconPlate);
    CMVisibleRect::setPositionAdapted(m_pBaconPlate, 200+visibleSize.width, 680);
    m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    for (int i = 0; i<3; i++) {
        Sprite* pBacon = Sprite::create(localPath("jam.png"));
        this->addChildToContentLayer(pBacon);
        CMVisibleRect::setPositionAdapted(pBacon, 200+visibleSize.width, 720-20*i);
        pBacon->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        pBacon->setScale(0.6);
        m_pBaconVector.push_back(pBacon);
    }
    m_pClip = _createDrageNode(localPath("clip.png"));
    this->addChildToContentLayer(m_pClip);
    CMVisibleRect::setPositionAdapted(m_pClip, 450+visibleSize.width, 700);
    m_pClip->setTouchEnabled(false);
    m_pClip->setActionPoint(Vec2(85, 70));
    m_pClip->setName("clip");
    m_pClip->setLocalZOrder(20);
    m_pClip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pClip->getPosition(), m_pBaconPlate->getPosition());
        m_pClip->setTouchEnabled(true);
    }), NULL));
}

void BaconHamFryHamScene::_addButter(DragNode* pIngredient)
{
    pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pPan->getPosition()+Vec2(-60, 60)),
                                            RotateTo::create(0.5,-60),
                                            CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
        
        ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
        if (pContainer) {
            pContainer->pourFood(true,0.5);
        }
        if (!m_pButter) {
            m_pButter = _createDrageNode("content/make/egg/make1/pan_butter1.png");
            m_pPanIn->addChild(m_pButter,5);
            m_pButter->setVisible(false);
            m_pButter->setTouchEnabled(false);
            m_pButter->setName("butter");
            m_pButter->runAction(Sequence::create(DelayTime::create(0.5),
                                                  Show::create(),
                                                  CallFunc::create([=](){
                m_pButter->getDragSprite()->runAction(AnimationHelp::createAnimate("content/make/egg/make1/pan_butter", 1, 2,false,true,0.3));
            }), NULL));
        }
    }),
                                            MoveTo::create(0.5,m_pPan->getPosition()+Vec2(40, 100)),
                                            RotateTo::create(1, 0),
                                            CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
        
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideMove(m_pPanIn->getPosition(), m_pPanIn->getPosition()+Vec2(100, 80));
        m_pButter->setTouchEnabled(true);
    }),
                                            MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                            CallFunc::create([=](){
        
        MenuLayer::addIngredient(MenuLayer::ePackageBacon, MenuLayer::eBacon_butter);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        IngredientManager::getInstance()->removeIngredient(pIngredient);
        pIngredient->removeFromParent();
    }), NULL));
}

void BaconHamFryHamScene::_paintButter()
{
    m_nMoveCount++;
    if (m_nMoveCount==0) {
        
        AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
    }
    if (m_nMoveCount%50 == 0) {
        int percent = m_pButterScribble->getCanvas()->getPercentageTransparent();
        log("====percent===%d",percent);
        int step = m_nMoveCount/50;
        if (step<5) {
            m_pButter->getDragSprite()->setTexture("content/make/egg/make1/pan_butter"+std::to_string(step+1)+".png");
        }
        if (percent<=28) {
            m_pGuideLayer->removeGuide();
            m_pButterScribble->resetCanvas();
            m_pButterScribble->paint("content/make/egg/make1/pan_butter0.png");
            m_pButter->ignoreMoving = true;
            
            m_pButter->runAction(Sequence::create(DelayTime::create(0.5),
                                                  CallFunc::create([=](){
                AudioHelp::getInstance()->stopLoopEffect();
                m_pButter->setTouchEnabled(false);
                m_pButter->setVisible(false);
            }),
                                                  DelayTime::create(0.5),
                                                  CallFunc::create([=](){
                AudioHelp::getInstance()->stopLoopEffect();
                AudioHelp::getInstance()->playEffect("ding.mp3");
                AudioHelp::getInstance()->_playEffectNiceWork();
                //                finish Scribble Butter
                _showHams();
            }), NULL));
        }
    }
}
void BaconHamFryHamScene::_doFryBacon(FriedUnit* pBacon)
{
    pBacon->doFry();
    if (pBacon->flag == 0) {
        pBacon->getFood()->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("jam"), 1, 3,false,true,3),
                                                      CallFunc::create([=](){
            if (!m_bEffectTurn) {
                m_bEffectTurn = true;
//                AudioHelp::getInstance()->playEffect("vo_turn_over_bacon.mp3");
            }
            pBacon->flag = 1;
            if (m_pClip->getPositionX()>visibleSize.width) {
                m_pClip->setTouchEnabled(false);
                m_pClip->index = 3;
                m_pClip->runAction(Sequence::create(MoveTo::create(1, m_pPan->getPosition()+Vec2(0, 100)),
                                                    CallFunc::create([=](){
                    m_pClip->setTouchEnabled(true);
                }), NULL));
            }
        }), NULL));
    }else if (pBacon->flag==2) {
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            if (!m_bEffectBaconOK) {
                m_bEffectBaconOK = true;
                AudioHelp::getInstance()->playEffect("vo_ham_ready_soon.mp3");
            }
            
        }), NULL));
        pBacon->getFood()->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("jam"), 3, 5,false,true,3),
                                                      CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("ding.mp3");
            AudioHelp::getInstance()->_playEffectNiceWork();
            pBacon->isFriedOK = true;
            if (m_pBaconVector.size()==2) {
                if (!m_pPlate) {
                    m_pPlate = Sprite::create(localPath("plate.png"));
                    this->addChildToContentLayer(m_pPlate);
                    CMVisibleRect::setPositionAdapted(m_pPlate, 350+visibleSize.width, 680);
                    m_pPlate->runAction(Sequence::create(DelayTime::create(1),
                                                         MoveBy::create(1, Vec2(-visibleSize.width+100, 0)), NULL));
                }
            }
            if (m_pClip->getPositionX()>visibleSize.width) {
                m_pClip->setTouchEnabled(false);
                m_pClip->index = 3;
                m_pClip->runAction(Sequence::create(MoveTo::create(1, m_pPan->getPosition()+Vec2(0, 100)),
                                                    CallFunc::create([=](){
                    m_pGuideLayer->showGuideMove(m_pClip->getPosition(), m_pPanIn->getPosition());
                    m_pClip->setTouchEnabled(true);
                }), NULL));
            }
        }), NULL));
    }
}
void BaconHamFryHamScene::_finishFryBacon()
{
    
}
void BaconHamFryHamScene::_finish()
{
    MenuLayer::addIngredient(MenuLayer::ePackageBacon, MenuLayer::eBacon_ham);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<BaconHamEatScene>();
    }), NULL));
}