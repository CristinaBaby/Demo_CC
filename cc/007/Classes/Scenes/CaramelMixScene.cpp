
#include "CaramelMixScene.h"
#include "SceneManager.h"
#include "FrameStatus.h"
#include "PercentChangeComponent.h"
#include "InertiaComponent.h"
#include "DeviceInfo.h"

CaramelMixScene::CaramelMixScene()
{
    m_nStep = 0;
    m_nMixCount = 0;
    m_nRotateCount = 0;
    m_pBar = nullptr;
    m_nMixTimeCount = 0;
}

CaramelMixScene::~CaramelMixScene()
{
    
}
bool CaramelMixScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("bg2.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _showBowl();
        
        _showIngredient(0);
        AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
    }),
                                     DelayTime::create(1.5),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pFlour->getPosition(), m_pBowl->getPosition());
        m_pGuideLayer->showGuideRotateLeft(m_pFlour->getPosition()+Vec2(-100, -100), 0.2);
    }),  NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void CaramelMixScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CaramelMixScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(2)) {
        ConfigManager::getInstance()->setVidioUnlocked("classic_mixtool", 1);
    }
}

void CaramelMixScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "ingredient"){
        _showShadow(pDragNode->getTag(),false);
    }else if (name == "tool_normal"){
        int index = pDragNode->index;
        if (index == 0) {
            pDragNode->index = 1;
            m_pCurTool = pDragNode;
            _showProgressBar();
            pDragNode->setTouchEnabled(false);
            
            ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
            if(pContainer){
                pContainer->showShadow(false);
            }
        
            pDragNode->runAction(Sequence::create(Spawn::create(RotateBy::create(1, -90),
                                                                MoveTo::create(1, Vec2(visibleSize.width/2, m_pBowl->getPositionY())), NULL),
                                                  CallFunc::create([=](){
                pDragNode->setRotation(0);
                pDragNode->setDragSprite(Sprite::create(localPath("spoon2.png")));
                
                Rect rect = m_pBowl->getBoundingBox();
                rect.origin.x += 170*1.5;
                rect.origin.y += 170*1.5;
                rect.size = Size(150*1.5, 230*1.5);
                pDragNode->setLimitRect(rect);
                
                pDragNode->resetOffset();
                pDragNode->setTouchEnabled(true);
            }), NULL));
            pDragNode->setActionPoint(Vec2(51, 54));
            
            m_bTemp = false;
            
            m_pBowl->runAction(MoveTo::create(0.8, Vec2(visibleSize.width/2, m_pBowl->getPositionY())));
            m_pBowl->runAction(ScaleTo::create(0.8, 1.5));
            m_pMixTool_electric->setTouchEnabled(false);
            m_pMixTool_electric->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }
    }else if (name == "tool_electric"){
        int index = pDragNode->index;
        if (index == 0){
            if (!IAPManager::getInstance()->getItemIsBought(2)) {
                if (!ConfigManager::getInstance()->getVideoUnLocked("caramel_mixtool", 1)){
                    ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
                    this->addChildToUILayer(pLayer);
                    pLayer->setLocalZOrder(100);
                    pDragNode->setTouchEnabled(false);
                    pLayer->shopDismissed = [=](){
                        if (IAPManager::getInstance()->getItemIsBought(2)) {
                            Node* pLock = pDragNode->getChildByName(LOCK_NAME_REWARD);
                            if (pLock) {
                                pLock->setVisible(false);
                            }
                        }
                        pDragNode->setTouchEnabled(true);
                    };
                    pLayer->shopGetFree = [=](){
                        pDragNode->setTouchEnabled(true);
                        LockUIManager::getInstance()->onRewardSuccess = [=](){
                            AudioHelp::getInstance()->playEffect("basic/start.mp3");
                            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                            pDragNode->addChild(pParticle,100);
                            pParticle->setPosition(Vec2::ZERO);
                        };
                        RewardManager::getInstance()->showRewardAds("caramel_mixtool", 1);
                        pLayer->removeFromParent();
                    };
                    return;
                }
            }
            pDragNode->index = 1;
            m_pCurTool = pDragNode;
            _showProgressBar();
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(Spawn::create(RotateBy::create(1, -30),
                                                                MoveTo::create(1, Vec2(visibleSize.width/2+50, m_pBowl->getPositionY()+150)), NULL),
                                                  CallFunc::create([=](){
                pDragNode->setRotation(0);
                pDragNode->setDragSprite(Sprite::create("content/make/common/agitator1.png"));
                
                Sprite* pStick = Sprite::create("content/make/common/agitator1_1.png");
                pDragNode->addChild(pStick);
                pStick->setName("stick");
                
                Rect rect = m_pBowl->getBoundingBox();
                rect.origin.x += 160*1.5;
                rect.origin.y += 230*1.5;
                rect.size = Size(170*1.5, 230*1.5);
                pDragNode->setLimitRect(rect);
                
                pDragNode->resetOffset();
                pDragNode->setTouchEnabled(true);
            }), NULL));
            pDragNode->setActionPoint(Vec2(91, 56));
            
            m_bTemp = false;
            m_pBowl->runAction(MoveTo::create(0.8, Vec2(visibleSize.width/2, m_pBowl->getPositionY())));
            m_pBowl->runAction(ScaleTo::create(0.8, 1.5));
            m_pMixTool_normal->setTouchEnabled(false);
            m_pMixTool_normal->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }else{
            _toolElectricWork();
        }
    }
}

void CaramelMixScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "tool_normal" || name == "tool_electric"){
        int index = pDragNode->index;
        if(index == 1){
            Rect bowlRect = m_pBowl->getBoundingBox();
            if (bowlRect.containsPoint(worldPoint)) {
                if (name == "tool_electric"){
                    AudioHelp::getInstance()->playLoopEffect("stir_machine1.mp3");
                }else{
                    AudioHelp::getInstance()->playLoopEffect("stir_spoon.mp3");
                    
                }
                _toolElectricWork();
                if (!m_bTemp) {
                    m_bTemp = true;
                    return;
                }
                auto distance = pDragNode->getOffset();
                auto diagonal = DeviceInfo::getDiagonal();
                if(diagonal>7.9)
                    diagonal = 7.9;
                else if(diagonal<5)
                    diagonal = 5;
                float percent = distance.getLength()/80*diagonal/6;
                
                float time = 0.4/percent;
                if(time<0.7)
                    time = 0.7;
                else if(time>1.4)
                    time = 1.4;
                _inertia->Invoke(percent);
                
            }else{
                AudioHelp::getInstance()->stopLoopEffect();
                m_pMixture->stopCountMove();
                m_pBowl->stopAllActions();
            }
        }
    }
}

void CaramelMixScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        Rect rectBowl = m_pBowl->getBoundingBox();
        if (rectBowl.containsPoint(pDragNode->getPosition())) {
            m_pGuideLayer->removeGuide();
            _addIngredient(pDragNode);
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),[=](){
                _showShadow(pDragNode->getTag(),true);
                pDragNode->setTouchEnabled(true);
            });
        }
    }else if (name == "tool_normal" || name == "tool_electric"){
        int index = pDragNode->index;
        if(index == 1){
            AudioHelp::getInstance()->stopLoopEffect();
            
            m_pBowl->stopAllActions();
            m_pMixture->stopCountMove();
            m_pMixture->stop();
            m_nMixCount = 0;
            
            _inertia->stopInvoke();
            unschedule(schedule_selector(CaramelMixScene::caculateShakeBad));
        }
        _toolElectricWork(false);
    }
}

void CaramelMixScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CaramelMixScene::_initData()
{
    setExPath("content/make/caramel/make2/");
}

void CaramelMixScene::_showShadow(int index,bool show)
{
    for_each(m_IngredientVector.begin(), m_IngredientVector.end(), [=](DragNode* pNode){
        if(index == pNode->getTag()){
            ContainerNode* pContainer = (ContainerNode*)pNode->getUserData();
            if(pContainer){
                pContainer->showShadow(show);
            }
        }
    });
    
}
void CaramelMixScene::_frozeIngredients(int index,bool froze)
{
    for_each(m_IngredientVector.begin(), m_IngredientVector.end(), [=](DragNode* pNode){
        if(froze){
            pNode->setTouchEnabled(false);
        }else{
            if(index != pNode->getTag()){
                pNode->setTouchEnabled(true);
            }
        }
    });
    
}
void CaramelMixScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 430);
    m_pBowl->runAction(Spawn::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                     EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL));
    
    m_pMixture = MixPoint::create();
    m_pBowl->addChild(m_pMixture);
    m_pMixture->setPosition(m_pBowl->getContentSize()*0.5);
    
    m_pBatter = Node::create();
    m_pMixture->addChild(m_pBatter);
}

void CaramelMixScene::_showMixTools()
{
    m_pMixTool_normal = _createDrageNode(localPath("spoon1.png"));
    this->addChildToContentLayer(m_pMixTool_normal);
    CMVisibleRect::setPositionAdapted(m_pMixTool_normal, 520, 680);
    m_pMixTool_normal->setName("tool_normal");
    
    ContainerNode* pNode = ContainerNode::create(m_pMixTool_normal);
    pNode->addShadow(localPath("spoon1_1.png"));
    m_pMixTool_normal->setUserData(pNode);
    
    m_pMixTool_normal->setTouchEnabled(false);
    ActionHelper::showBezier(m_pMixTool_normal, m_pMixTool_normal->getPosition(), ActionHelper::ShowDirection::show_from_right,1,[=](){
        m_pMixTool_normal->setOrgPositionDefault();
        m_pMixTool_normal->setTouchEnabled(true);
    });
    
    m_pMixTool_electric = _createDrageNode("content/make/common/agitator.png");
    this->addChildToContentLayer(m_pMixTool_electric);
    CMVisibleRect::setPositionAdapted(m_pMixTool_electric, 224, 760);
    m_pMixTool_electric->setName("tool_electric");
    
    bool bLocked = false;
    if (!IAPManager::getInstance()->getItemIsBought(2)) {
        if (!ConfigManager::getInstance()->getVideoUnLocked("caramel_mixtool", 1)){
            LockUIManager::getInstance()->registerLock("caramel_mixtool", 1, m_pMixTool_electric);
            LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
            bLocked = true;
        }
    }
    m_pMixTool_electric->setTouchEnabled(false);
    ActionHelper::showBezier(m_pMixTool_electric, m_pMixTool_electric->getPosition(), ActionHelper::ShowDirection::show_from_right,0.5,[=](){
        m_pMixTool_electric->setOrgPositionDefault();
        m_pMixTool_electric->setTouchEnabled(true);
        if (!bLocked) {
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
            m_pMixTool_electric->addChild(pParticle,20);
            pParticle->setPosition(Vec2::ZERO);
        }
    });
    
    AudioHelp::getInstance()->playEffect("present_in.mp3");
}

void CaramelMixScene::_showProgressBar()
{
    if (m_pBar) {
        m_pBar->beginProgress(5,false);
        _showBar();
        return;
    }
    m_pBar = ProgressBar::create("content/common/progress/bar_h.png", "content/common/progress/face1.png", "content/common/progress/face2.png", "content/common/progress/face3.png","content/common/progress/bar_mash_h.png",ProgressBar::Direction::eDirectionH);
    this->addChildToUILayer(m_pBar);
    CMVisibleRect::setPositionAdapted(m_pBar, 320, 550,kBorderNone,kBorderTop);
    m_pBar->progressNewState = [=](int state){
        if (state == ProgressBar::eStateOK) {
            //            AudioHelp::getInstance()->playEffect("princess_win.mp3");
            //            m_pGameUI->showNextLayout();
            //            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            //            this->addChildToUILayer(pParticle);
            //            pParticle->setPosition(visibleSize*0.5);
        }else if (state == ProgressBar::eStateBad) {
            log("===mix bad");
            //            m_pCurTool->ignoreMoving = true;
            //            _finishMix();
        }else if (state == ProgressBar::eStateOver) {
            log("===mix Over");
            m_nMixTimeCount++;
            if (m_nMixTimeCount == 2) {
                m_pMixture->stopAllActions();
                _finishMix();
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }
    };
    m_pBar->stopProgress();
    _showBar();
    m_pBar->setPercentStage(180.0*100/534, 360.0*100/534, 100);
    m_pBar->beginProgress(5,false);
}
void CaramelMixScene::_showBar(bool show )
{
    if (!m_pBar) {
        return;
    }
    if (show) {
        
        m_pBar->stopAllActions();
        m_pBar->setPosition(Vec2(visibleSize.width/2, visibleSize.height-150+500));
        m_pBar->setVisible(true);
        m_pBar->runAction(EaseBackOut::create(MoveBy::create(0.5, Vec2(0, -500))));
    }else{
        m_pBar->stopAllActions();
        m_pBar->setPosition(Vec2(visibleSize.width/2, visibleSize.height-150));
        m_pBar->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, 500))),
                                           CallFunc::create([=]()
                                                            {
                                                                m_pBar->setVisible(false);
                                                            }), NULL));
    }
}

DragNode* CaramelMixScene::_createContainer(int index,bool isWater)
{
    //    back shadow front food food2
    static std::string sIngredents[][5] ={
        {"flour_down.png","flour_shadow.png","flour_on.png"},
        {"pan.png","pan_down.png","","pan_sugar.png"},
        {},
    };
    int tag = index;
    
    DragNode* pDragNode = _createDrageNode(localPath(sIngredents[index][0]));
    this->addChildToContentLayer(pDragNode);
    pDragNode->setTag(tag);
    pDragNode->setLocalZOrder(tag);
    pDragNode->setName("ingredient");
    if(!isWater) {
        ContainerNode* pNode = ContainerNode::create(pDragNode);
        if(sIngredents[index][1]!=""){
            pNode->addShadow(localPath(sIngredents[index][1]));
        }
        if(sIngredents[index][2]!=""){
            pNode->addFront(localPath(sIngredents[index][2]));
        }
        if(sIngredents[index][3]!=""){
            pNode->addFood(localPath(sIngredents[index][3]));
        }
        pDragNode->setUserData(pNode);
    }else{
        KettleNode* pNode = KettleNode::create(pDragNode);
        if(sIngredents[index][1]!=""){
            pNode->addShadow(localPath(sIngredents[index][1]));
        }
        if(sIngredents[index][2]!=""){
            pNode->addFront(localPath(sIngredents[index][2]));
        }
        if(sIngredents[index][3]!=""){
            pNode->addWater(localPath(sIngredents[index][3]),localPath(sIngredents[index][3]));
        }
        if(sIngredents[index][4]!=""){
            pNode->addPourWater(localPath(sIngredents[index][4]));
        }
        pDragNode->setUserData(pNode);
    }
    
    return pDragNode;
}
void CaramelMixScene::_showIngredient(int index)
{
    bool bIsWater = false;
    if (index == 1) {
        bIsWater = true;
    }
    DragNode* pIngredient = _createContainer(index,bIsWater);
    if (!pIngredient) {
        return;
    }
    m_IngredientVector.pushBack(pIngredient);
    float dr = 0.5;
    switch (index) {
        case 0:
        {
            m_pFlour = pIngredient;
            pIngredient->setActionPoint(Vec2(28, 308));
            CMVisibleRect::setPositionAdapted(pIngredient, 450, 790);
            dr = 0.5;
        }
            break;
        case 1:
        {
            m_pSugar = pIngredient;
            pIngredient->setActionPoint(Vec2(30, 168));
            CMVisibleRect::setPositionAdapted(pIngredient, 430, 780);
            dr = 1;
        }
            break;
       
        default:
            break;
    }
    this->runAction(Sequence::create(DelayTime::create(dr),
                                     CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    }), NULL));
    ActionHelper::showBezier(pIngredient, pIngredient->getPosition(), ActionHelper::ShowDirection::show_from_right,dr,[=](){
        pIngredient->setTouchEnabled(false);
        pIngredient->setOrgPositionDefault();
        pIngredient->runAction(Sequence::create(DelayTime::create(1-dr),
                                                CallFunc::create([=](){
            pIngredient->setTouchEnabled(true);
        }), NULL));
    });
}

void CaramelMixScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "bowl_flour.png",
        "bowl_sugar.png",
    };
    int index = pIngredient->getTag();
    _frozeIngredients(index);
    int k = index;
    
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    //    pIngredient->setLocalZOrder(5);
    
    pIngredient->setTouchEnabled(false);
    switch (index) {
#pragma mark =========  add flour ==========
        case 0:
        {
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 150)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_flour.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    MoveBy::create(2,Vec2(100, 200)),
//                                                    MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(50, 100)),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 1) {
                    _showIngredient(1);
                }
                m_IngredientVector.eraseObject(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add Sugar ==========
        case 1:
        {
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 180)),
                                                    RotateBy::create(0.5, -40),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourMilkEffect();
                
                Sprite* pStream = Sprite::create(localPath("pan_sugar_1.png"));
                pIngredient->addChild(pStream);
                pStream->setLocalZOrder(20);
                pStream->setRotation(40);
                pStream->setAnchorPoint(Vec2(0.5,0.8));
                pStream->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pStream->setName("stream");
                pStream->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("pan_sugar_"), 1, 2)));
                
                KettleNode* pContainer = (KettleNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourWater();
                }
            }),
                                                    CircleMove::create(2,m_pBowl->getPosition()+Vec2(60, 200),40,360),
//                                                    MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 100)),
                                                    CallFunc::create([=](){
                auto pStream = pIngredient->getChildByName("stream");
                if (pStream) {
                    pStream->stopAllActions();
                    pStream->runAction(FadeOut::create(0.5));
                }
                
            }),
                                                    DelayTime::create(0.5),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 2) {
                    _saveMixture();
                    _showMixTools();
                    AudioHelp::getInstance()->playEffect("vo_mix_ingredients.mp3");
                    _finishAdd();
                }
                m_IngredientVector.eraseObject(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
        default:
            break;
    }
    Sprite* pAdd = Sprite::create(localPath(lNameStr));
    pAdd->setPosition(posAdd);
    if (pAdd) {
        pAdd->setOpacity(0);
        m_pBatter->addChild(pAdd,5);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         FadeIn::create(1),
                                         CallFunc::create([=](){
            
//            _frozeIngredients(index,false);
        }), NULL));
    }
}

void CaramelMixScene::_saveMixture()
{
    Size candySize = m_pBowl->getContentSize();
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pMixture->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"carame_mixture.png", false);
    pImage->autorelease();
}


void CaramelMixScene::_checkMoveSpeed()
{
    log("========m_nMixCount=%d==",m_nMixCount);
    std::string name = m_pCurTool->getName();
    int fast = 170;
    if (name == "tool_electric") {
        fast = 150;
    }
    if(m_nMixCount<60){
        //        慢
        log("========slow");
        //        m_pMixture->run(m_nMixCount/100.0);
        
        int count = 2;
        for (int i = 0; i<count; i++) {
            this->runAction(Sequence::create(DelayTime::create(i*0.3/count),
                                             CallFunc::create([=](){
                _mixBatter();
            }), NULL));
        }
        
        m_pBowl->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
    }else if(m_nMixCount<fast) {
        //        适中
        log("========perfect");
        //        m_pMixture->run(0.3+m_nMixCount/200.0);
        
        int count = 2+m_nMixCount/60;
        for (int i = 0; i<count; i++) {
            this->runAction(Sequence::create(DelayTime::create(i*0.3/count),
                                             CallFunc::create([=](){
                _mixBatter();
            }), NULL));
        }
        
        auto action0 = m_pBowl->getActionByTag(1002);
        if (!action0){
            m_pBowl->stopAllActions();
            CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
        }
        auto action = m_pBowl->getActionByTag(1001);
        if (!action) {
            m_pBowl->stopAllActions();
            CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
            action = RepeatForever::create(ActionHelper::createShakeAction(11,0.07));
            action->setTag(1001);
            m_pBowl->runAction(action);
        }
        if (m_nMixCount>fast-40) {
            static bool bVoicePlay = false;
            if (!bVoicePlay) {
                bVoicePlay = true;
                AudioHelp::getInstance()->playEffect("vo_watch_out.mp3");
                this->runAction(Sequence::create(DelayTime::create(4),
                                                 CallFunc::create([=](){
                    bVoicePlay = false;
                    
                }), NULL));
            }
        }
    }else{
        //        快
        log("========fast");
        //        m_pMixture->run(0.6+m_nMixCount/300.0);
        AudioHelp::getInstance()->stopLoopEffect();
        m_pCurTool->ignoreMoving = true;
        
        int count = 4+m_nMixCount/60;
        for (int i = 0; i<count; i++) {
            this->runAction(Sequence::create(DelayTime::create(i*0.3/count),
                                             CallFunc::create([=](){
                _mixBatter();
            }), NULL));
        }
        
        auto action0 = m_pBowl->getActionByTag(1001);
        if (!action0){
            m_pBowl->stopAllActions();
            CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
        }
        auto action = m_pBowl->getActionByTag(1002);
        if (!action) {
            m_pBowl->stopAllActions();
            CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
            action = RepeatForever::create(ActionHelper::createShakeAction(15,0.04));
            action->setTag(1002);
            m_pBowl->runAction(action);
        }
        m_pCurTool->setTouchEnabled(false);
        m_pMixture->stopCountMove();
        _mixBad();
    }
    m_pMixture->runAction(RotateBy::create(0.3, m_nMixCount));
    m_nMixCount = 0;
}
void CaramelMixScene::_mixBatter()
{
    if (m_nRotateCount>150*5) {
        return;
    }
    
    m_nRotateCount++;
    //    m_pMixture->setRotation(m_pMixture->getRotation()+5);
    if (m_nRotateCount%150==0) {
        int mixtureStep = m_nRotateCount/150;
        int mixTotal = 5;
        if (mixtureStep<=mixTotal){
            std::stringstream ostr;
            ostr<<"batter"<<mixtureStep<<".png";
            Sprite* pMixture = Sprite::create(localPath(ostr.str()));
            m_pBatter->removeAllChildren();
            m_pBatter->addChild(pMixture);
        }
        if (mixtureStep>=mixTotal){
            //mix finish
            m_nMixTimeCount++;
            if (m_nMixTimeCount == 2) {
                m_pMixture->stopAllActions();
                _finishMix();
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }
    }
}

void CaramelMixScene::_mixBad()
{
    AudioHelp::getInstance()->stopLoopEffect();
    _toolElectricWork(false);
    for (int i = 0; i<3; i++) {
        Sprite* pBatter = Sprite::create("content/make/common/mix/splatter4.png");
        this->addChildToContentLayer(pBatter);
        pBatter->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2)+Vec2(-300.0+30*(arc4random()%20), -300.0+30*(arc4random()%20)));
        pBatter->setOpacity(0);
        pBatter->setScale(0.3);
        pBatter->runAction(Sequence::create(DelayTime::create(0.1+i*0.2),
                                            FadeIn::create(0.3),
                                            CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("splash.mp3");
        }),
                                            ScaleTo::create(0.5, 0.8+0.04*(arc4random()%10)),
                                            DelayTime::create(1),
                                            FadeOut::create(1), NULL));
    }
    
    m_pBg->runAction(Sequence::create(ScaleTo::create(1, 1.5),
                                      CallFunc::create([=](){
        m_pBowl->stopAllActions();
        m_pBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, visibleSize.height/2)));
        m_pCurTool->runAction(MoveBy::create(1, Vec2(visibleSize.width, 50)));
    }),
                                      DelayTime::create(1),
                                      ScaleTo::create(1, 1),
                                      CallFunc::create([=](){
        CMVisibleRect::setPositionAdapted(m_pBowl, 210-visibleSize.width, 430);
        m_pBowl->setScale(1);
        m_pBowl->runAction(Spawn::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                         EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL));
        m_pMixTool_normal->removeFromParent();
        m_pMixTool_electric->removeFromParent();
        m_pBatter->removeAllChildren();
        
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"carame_mixture.png");
        Sprite* pMix = Sprite::create(FileUtils::getInstance()->getWritablePath()+"carame_mixture.png");
        if (pMix) {
            m_pBatter->addChild(pMix);
        }
        _showMixTools();
    }), NULL));
    _showBar(false);
    
    m_nMixTimeCount = 0;
    m_nMixCount = 0;
    m_nRotateCount =0;
    loadingPercent = 0;
    isShake = false;
    isFastShake = false;
    isFinish = false;
    isPlayFast = false;
}
void CaramelMixScene::_finishMix()
{
    m_pBowl->stopAllActions();
    AudioHelp::getInstance()->playEffect("present_out.mp3");
    m_pCurTool->setTouchEnabled(false);
    m_pCurTool->stopAllActions();
    m_pCurTool->setLocalZOrder(20);
    _toolElectricWork(false);
    Sprite* pStick = (Sprite*)m_pCurTool->getChildByName("stick");
    if (pStick) {
        pStick->stopAllActions();
        pStick->setTexture("content/make/common/agitator1_1.png");
    }
    m_pCurTool->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    static std::string voName[] = {
        "vo_wonderful.mp3",
        "vo_well_done.mp3",
        "vo_fantastic.mp3",
        "vo_done.mp3",
        "vo_fantastic.mp3",
        "vo_great.mp3",
    };
    AudioHelp::getInstance()->playEffect(voName[arc4random()%6]);
    
    Sprite* pAnimation = Sprite::create("content/make/common/animation/donhua_1.png");
    this->addChildToUILayer(pAnimation);
    pAnimation->setAnchorPoint(Vec2(0.5, 0));
    CMVisibleRect::setPositionAdapted(pAnimation, 320, 0,kBorderNone,kBorderBottom);
    pAnimation->runAction(Sequence::create(AnimationHelp::createAnimate("content/make/common/animation/donhua_", 1, 8),
                                           CallFunc::create([=](){
        pAnimation->removeFromParent();
    }), NULL));
    AudioHelp::getInstance()->playEffect("success.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<CaramelMakeBallScene>();
    }), NULL));
}


void CaramelMixScene::_ingredientOut(DragNode* pNode)
{
    AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
    for (int i = 0; i<2; i++) {
        m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),
                                            ScaleTo::create(0.17,1.0),nullptr));
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/heartParticle.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pBowl->getPosition()+Vec2(-50, 100));
        pParticle->setLocalZOrder(20);
        if(i == 0){
            pParticle->setScaleX(-1);
        }
    }
    _frozeIngredients(pNode->getTag(),false);
}

void CaramelMixScene::_toolElectricWork(bool working)
{
    if(!m_pMixTool_electric){
        return;
    }
    if (working) {
        auto action = m_pMixTool_electric->getActionByTag(1000);
        if (!action) {
            action = RepeatForever::create(ActionHelper::createShakeAction());
            action->setTag(1000);
            m_pMixTool_electric->runAction(action);
        }
        Sprite* pStick = (Sprite*)m_pMixTool_electric->getChildByName("stick");
        if (pStick) {
            auto action = pStick->getActionByTag(1000);
            if (!action) {
                action = RepeatForever::create(AnimationHelp::createAnimate("content/make/common/agitator1_", 2, 4));
                action->setTag(1000);
                pStick->runAction(action);
            }
        }
    }else{
        m_pMixTool_electric->stopAllActions();
        Sprite* pStick = (Sprite*)m_pMixTool_electric->getChildByName("stick");
        if (pStick) {
            pStick->stopAllActions();
            //            pStick->setTexture("content/make/common/agitator1_1.png");
        }
    }
}


void CaramelMixScene::_finishAdd()
{
    _inertia = InertiaComponent::create();
    m_pMixture->addComponent(_inertia);
    _inertia->setInvokeAction(RotateBy::create(0, 60));
    _inertia->addListners(InertiaInvoking, CC_CALLBACK_2(CaramelMixScene::inertiaInvoking, this));
    _inertia->addListners(Inertiaing, CC_CALLBACK_2(CaramelMixScene::inertiaing, this));
    _inertia->addListners(InertiaStop, [this](LQComponent*,OperateListner* lis){
    });
    loadingPercent = 0;
    isShake = false;
    isFastShake = false;
    isFinish = false;
    isPlayFast = false;
}

void CaramelMixScene::inertiaInvoking(LQComponent* lqc,OperateListner* lis){
    auto data = dynAny_cast<float>(lis->getData());
    if(data>13)
        data = 13;
      CCLOG("data====%f",data);
    float fflag = 0.3;
    if(m_pCurTool->getName() == "tool_electric"){
        fflag = 0.4;
    }
    loadingPercent = loadingPercent+ data*fflag*0.6666*2/30.0;
    m_pBar->setPercent(loadingPercent);
    _mixBatter();
    CCLOG("loadingPercent====%f",loadingPercent);
    isShake = true;
    float perfect = 8;
    float fast = 8.5;
    if(data<=perfect){
        unschedule(schedule_selector(CaramelMixScene::caculateShakeBad));
        // isShake = false;
        isFastShake = false;
        unschedule(schedule_selector(CaramelMixScene::caculateShake));
        unschedule(schedule_selector(CaramelMixScene::caculateFastShake));
        _slow();
    }else if (isFastShake) {
        _fast();
        
        if(data>fast&& !isScheduled(schedule_selector(CaramelMixScene::caculateShakeBad)))
            this->scheduleOnce(schedule_selector(CaramelMixScene::caculateShakeBad), 0.6);
    } else if(isShake){
        _perfect();
        if(data>fast&& !isFastShake&& !isScheduled(schedule_selector(CaramelMixScene::caculateFastShake)))
            this->scheduleOnce(schedule_selector(CaramelMixScene::caculateFastShake), 0.6);
        
    }
    
    if(data>fast){
        if(!isShake&& !isScheduled(schedule_selector(CaramelMixScene::caculateShake)))
            this->scheduleOnce(schedule_selector(CaramelMixScene::caculateShake), 0.7);
    }
    
    
}

void CaramelMixScene::inertiaing(LQComponent*,OperateListner* lis){
    auto data = dynAny_cast<float>(lis->getData());
    if(data >13)
        data = 13;
}

void CaramelMixScene::caculateShake(float){
    isShake = true;
}
void CaramelMixScene::caculateFastShake(float){
    isFastShake = true;
}

void CaramelMixScene::caculateShakeBad(float){
    if(!isFinish) {
        m_pCurTool->setTouchEnabled(false);
        m_pMixture->stopCountMove();
        _mixBad();
    }
}

void CaramelMixScene::_slow()
{
//    int count = 2;
//    for (int i = 0; i<count; i++) {
//        this->runAction(Sequence::create(DelayTime::create(i*0.3/count),
//                                         CallFunc::create([=](){
//            _mixBatter();
//        }), NULL));
//    }
    
}
void CaramelMixScene::_perfect()
{
    
//    int count = 2+m_nMixCount/60;
//    for (int i = 0; i<count; i++) {
//        this->runAction(Sequence::create(DelayTime::create(i*0.3/count),
//                                         CallFunc::create([=](){
//            _mixBatter();
//        }), NULL));
//    }
    
    auto action0 = m_pBowl->getActionByTag(1002);
    if (!action0){
        m_pBowl->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
    }
    auto action = m_pBowl->getActionByTag(1001);
    if (!action) {
        m_pBowl->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
        action = RepeatForever::create(ActionHelper::createShakeAction(11,0.07));
        action->setTag(1001);
        m_pBowl->runAction(action);
    }
}
void CaramelMixScene::_fast()
{
    
//    if(!isPlayFast){
//        isPlayFast = true;
        static bool bVoicePlay = false;
        if (!bVoicePlay) {
            bVoicePlay = true;
            AudioHelp::getInstance()->playEffect("vo_watch_out.mp3");
            this->runAction(Sequence::create(DelayTime::create(4),
                                             CallFunc::create([=](){
                bVoicePlay = false;
                
            }), NULL));
        }
//    }
//    int count = 4+m_nMixCount/60;
//    for (int i = 0; i<count; i++) {
//        this->runAction(Sequence::create(DelayTime::create(i*0.3/count),
//                                         CallFunc::create([=](){
//            _mixBatter();
//        }), NULL));
//    }
    auto action0 = m_pBowl->getActionByTag(1001);
    if (!action0){
        m_pBowl->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
    }
    auto action = m_pBowl->getActionByTag(1002);
    if (!action) {
        m_pBowl->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pBowl, 320, 430);
        action = RepeatForever::create(ActionHelper::createShakeAction(15,0.04));
        action->setTag(1002);
        m_pBowl->runAction(action);
    }
}