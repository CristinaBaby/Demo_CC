
#include "BaconHamFryBaconScene.h"
#include "SceneManager.h"

BaconHamFryBaconScene::BaconHamFryBaconScene()
{
    
    m_bEffectTurn = false;
    m_bEffectBaconOK = false;
}

BaconHamFryBaconScene::~BaconHamFryBaconScene()
{
    
}
bool BaconHamFryBaconScene::init()
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
        _showBacons();
        AudioHelp::getInstance()->playEffect("vo_add3bacon2pan.mp3");
    }), NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void BaconHamFryBaconScene::onEnter()
{
    ExtensionScene::onEnter();
}

void BaconHamFryBaconScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BaconHamFryBaconScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    
    if(name == "clip"){
        if (pDragNode->index == 0) {
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("clip1_down.png")));
            Sprite* pClipOn = Sprite::create(localPath("clip1_on.png"));
            pDragNode->addChild(pClipOn,20);
            pClipOn->setName("clip_on");
        }
    }
}

void BaconHamFryBaconScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    
    if(name == "clip"){
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
                Sprite* pBacon = Sprite::create(localPath("bacon.png"));
                pDragNode->addChild(pBacon);
                pBacon->setName("bacon");
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
                    Sprite* pBacon = (Sprite*)pDragNode->getChildByName("bacon");
                    if (pBacon) {
                        pBacon->removeFromParent();
                    }
                    pDragNode->setTouchEnabled(true);
                    int size = m_pBaconVector.size();
                    FriedUnit* pFriedBacon = FriedUnit::create();
                    pFriedBacon->setFood(localPath("bacon.png"));
                    pFriedBacon->setOil(localPath("bacon5_"+std::to_string(size%2+1)+"_1.png"));
                    pFriedBacon->addParticle("particle/fry_bubble1.plist");
                    pFriedBacon->addParticle("particle/fry_bubble2.plist");
                    pFriedBacon->setTag(m_pBaconVector.size());
                    this->addChildToContentLayer(pFriedBacon);
                    pFriedBacon->setPosition(m_pPanIn->getPosition()+Vec2(0, 10+70*(1-size)));
                    log("=======%f,%f",pFriedBacon->getPositionX(),pFriedBacon->getPositionY());
                    m_pBaconFriedVector.push_back(pFriedBacon);
                    
                    _doFryBacon(pFriedBacon);
                    
                    if (m_pBaconVector.size()==1) {
                        m_pPlate = Sprite::create(localPath("plate.png"));
                        this->addChildToContentLayer(m_pPlate);
                        CMVisibleRect::setPositionAdapted(m_pPlate, 350+visibleSize.width, 680);
                        m_pPlate->runAction(Sequence::create(DelayTime::create(1),
                                                             MoveBy::create(1, Vec2(-visibleSize.width+100, 0)), NULL));
                    }
                    m_pGuideLayer->removeGuide();
                    if (m_pBaconVector.size()==0) {
                        m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                        m_pClip->setTouchEnabled(false);
                        m_pClip->runAction(MoveBy::create(1, Vec2(visibleSize.width*1.5, 0)));
                        AudioHelp::getInstance()->_playEffectNiceWork();
                    }
                }), NULL));
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
                    pFriedBacon->getFood()->stopAllActions();
                    pFriedBacon->getFood()->setPosition(Vec2::ZERO);
                    AudioHelp::getInstance()->playEffect("drag_corn.mp3");
                    pDragNode->setTouchEnabled(false);
                    pDragNode->runAction(Sequence::create(MoveTo::create(0.5, pFriedBacon->getPosition()+Vec2(100, 100)),
                                                          CallFunc::create([=](){
                        Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                        if (pOn) {
                            pOn->setTexture(localPath("clip2_on.png"));
                        }
                        Sprite* pBacon;
                        if (pFriedBacon->flag == 1) {
                            pDragNode->index = 5;
                            pBacon = Sprite::create(localPath("bacon2.png"));
                            
                        }else if(pFriedBacon->flag == 2) {
                            pDragNode->index = 4;
                            pBacon = Sprite::create(localPath("bacon5_"+std::to_string(pFriedBacon->getTag()%2+1)+".png"));
                            
                            auto iter = std::find(m_pBaconFriedVector.begin(), m_pBaconFriedVector.end(), pFriedBacon);
                            if (iter != m_pBaconFriedVector.end())
                            {
                                m_pBaconFriedVector.erase(iter);
                            }
                            m_pGuideLayer->removeGuide();
                            m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPlate->getPosition());
                        }
                        pDragNode->addChild(pBacon);
                        pBacon->setName("bacon");
                        pBacon->setTag(pFriedBacon->getTag());
                        pBacon->setPosition(pDragNode->convertToNodeSpace(worldPoint));
                        
                        pFriedBacon->stopFry();
                        if(m_pBaconFriedVector.size()>0){
                            AudioHelp::getInstance()->playLoopEffect("frying_food.mp3");
                        }
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
                Node* pBacon = pDragNode->getChildByName("bacon");
                int tag = pBacon->getTag();
                pBacon->removeFromParent();
                
                int count = m_pBaconFriedVector.size();
                AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
                
                Sprite* pOn = (Sprite*)pDragNode->getChildByName("clip_on");
                if (pOn) {
                    pOn->setTexture(localPath("clip1_on.png"));
                }
                
                Sprite* pBaconInPlate = Sprite::create(localPath("bacon5_"+std::to_string(tag%2+1)+".png"));
                m_pPlate->addChild(pBaconInPlate);
                
                pBaconInPlate->setPosition(m_pPlate->getAnchorPointInPoints()+Vec2(0, 60-20*(3-count)));
                pBaconInPlate->setScale(0.6);
                pDragNode->index = 3;
                m_pGuideLayer->removeGuide();
                if (count == 0) {
                    //finish fry bacon
                    m_pClip->setTouchEnabled(false);
                    m_pClip->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
//                    m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                    
                    auto pParticle = dynamic_cast<ParticleSystem*>(m_pContentLayer->getChildByName("smoke"));
                    if (pParticle) {
                        pParticle->stopSystem();
                    }
                    _finish();
                }else{
                    m_pGuideLayer->showGuideMove(pDragNode->getPosition(), m_pPan->getPosition());
                }
            }
        }
    }
}

void BaconHamFryBaconScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
//    AudioHelp::getInstance()->stopLoopEffect();
    std::string name = pDragNode->getName();
    
    if(name == "clip"){
#pragma mark        bacon翻面
        if (pDragNode->index==5) {
            pDragNode->setTouchEnabled(false);
            Sprite* pBacon = (Sprite*)pDragNode->getChildByName("bacon");
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
                            pFriedBacon->getFood()->setTexture(localPath("bacon3.png"));
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
                        pDragNode->setTouchEnabled(true);
                    }), NULL));
                    AudioHelp::getInstance()->playEffect("vo_fabulous.mp3");
                    break;
                }
            }
        }
    }
}

void BaconHamFryBaconScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

#pragma mark - initData
void BaconHamFryBaconScene::_initData()
{
    setExPath("content/make/bacon/");
}

void BaconHamFryBaconScene::_showStove()
{
    m_pStove = Sprite::create("content/make/common/stove/stove.png");
    this->addChildToContentLayer(m_pStove);
    CMVisibleRect::setPositionAdapted(m_pStove, 320, 320);
    
    m_pFire = Sprite::create("content/make/common/stove/stove_1.png");
    this->addChildToContentLayer(m_pFire);
    m_pFire->setPosition(m_pStove->getPosition()+Vec2(0,-10));
    
    m_pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/common/stove/stove_", 1, 2)));
}

void BaconHamFryBaconScene::_showPan()
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
}

void BaconHamFryBaconScene::_showBacons()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    
    m_pBaconPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pBaconPlate);
    CMVisibleRect::setPositionAdapted(m_pBaconPlate, 200+visibleSize.width, 680);
    m_pBaconPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    for (int i = 0; i<3; i++) {
        Sprite* pBacon = Sprite::create(localPath("bacon.png"));
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
void BaconHamFryBaconScene::_doFryBacon(FriedUnit* pBacon)
{
    pBacon->doFry();
    if (pBacon->flag == 0) {
        pBacon->getFood()->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("bacon"), 1, 3,false,true,3),
                                                      CallFunc::create([=](){
            if (!m_bEffectTurn) {
                m_bEffectTurn = true;
                AudioHelp::getInstance()->playEffect("vo_turn_over_bacon.mp3");
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
            pBacon->getFood()->runAction(RepeatForever::create(ActionHelper::createShakeAction(6)));
        }), NULL));
    }else if (pBacon->flag==2) {
        pBacon->getFood()->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("bacon"), 3, 4,false,true,3),
                                                      DelayTime::create(3),
                                                      CallFunc::create([=](){
            if (!m_bEffectBaconOK) {
                m_bEffectBaconOK = true;
                AudioHelp::getInstance()->playEffect("vo_bacon_ready_soon.mp3");
            }
            AudioHelp::getInstance()->playEffect("ding.mp3");
            AudioHelp::getInstance()->_playEffectNiceWork();
            pBacon->getFood()->setTexture(localPath("bacon5_"+std::to_string(pBacon->getTag()%2+1)+".png"));
            pBacon->isFriedOK = true;
            if (m_pClip->getPositionX()>visibleSize.width) {
                m_pClip->setTouchEnabled(false);
                m_pClip->index = 3;
                m_pClip->runAction(Sequence::create(MoveTo::create(1, m_pPan->getPosition()+Vec2(0, 100)),
                                                    CallFunc::create([=](){
                    m_pClip->setTouchEnabled(true);
                }), NULL));
            }
            pBacon->getFood()->runAction(RepeatForever::create(ActionHelper::createShakeAction(6)));
        }), NULL));
    }
}
void BaconHamFryBaconScene::_finishFryBacon()
{
    
}
void BaconHamFryBaconScene::_finish()
{
    MenuLayer::addIngredient(MenuLayer::ePackageBacon, MenuLayer::eBacon_bacon);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<BaconHamFryHamScene>();
    }), NULL));
}