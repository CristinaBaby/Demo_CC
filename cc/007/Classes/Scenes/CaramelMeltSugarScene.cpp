
#include "CaramelMeltSugarScene.h"
#include "SceneManager.h"

CaramelMeltSugarScene::CaramelMeltSugarScene()
{
    m_nStep = 0;
    m_nMixCount = 0;
    m_nRotateCount = 0;    
}

CaramelMeltSugarScene::~CaramelMeltSugarScene()
{
    
}
bool CaramelMeltSugarScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showStove();
    _showBowl();
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _showIngredient(0);
        _showIngredient(1);
        _showIngredient(2);
        AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pBrownSugar->getPosition(), m_pBowl->getPosition());
        m_pGuideLayer->showGuideRotateLeft(m_pBrownSugar->getPosition()+Vec2(-50, -100), 0.2);
    }),  NULL));
    
    m_pGameUI->showNormalLayout();
    AudioHelp::getInstance()->playBackGroundMusic("music_make.mp3");
    return true;
}

void CaramelMeltSugarScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_CRAMEL);
}

void CaramelMeltSugarScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CaramelMeltSugarScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    
    if (name == "ingredient"){
        _showShadow(pDragNode->getTag(),false);
    }else if (name == "tool_normal"){
        int index = pDragNode->index;
        if (index == 0) {
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("spoon1_1.png")));
        }
    }
}

void CaramelMeltSugarScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "tool_normal"){
        int index = pDragNode->index;
        if(index == 1){
            Rect rect = m_pBowl->getBoundingBox();
            if (rect.containsPoint(worldPoint)) {
                pDragNode->index = 2;
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(Sequence::create(Spawn::create(RotateBy::create(1, -60),
                                                                    MoveTo::create(1, Vec2(m_pBowl->getPositionX()+50, m_pBowl->getPositionY()+100)), NULL),
                                                      CallFunc::create([=](){
//                    pDragNode->setRotation(0);
//                    pDragNode->setDragSprite(Sprite::create(localPath("spoon2.png")));
                    
                    pDragNode->setTouchEnabled(true);
                }), NULL));
                pDragNode->setActionPoint(Vec2(49, 73));
//                pDragNode->setActionPoint(Vec2(45, 27));
                rect.origin.x += 200;
                rect.origin.y += 250;
                rect.size = Size(160, 160);
                pDragNode->setLimitRect(rect);
            }
        }else if (index == 2) {
            Rect bowlRect = m_pBowl->getBoundingBox();
            if (bowlRect.containsPoint(worldPoint)) {
                AudioHelp::getInstance()->playLoopEffect("stir_spoon.mp3");
                
                _mixBatter();
            }else{
                AudioHelp::getInstance()->stopLoopEffect();
            }
            
        }
    }
}

void CaramelMeltSugarScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        Rect rectBowl = m_pBowl->getBoundingBox();
        if (rectBowl.containsPoint(pDragNode->getPosition())) {
            _addIngredient(pDragNode);
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),[=](){
                _showShadow(pDragNode->getTag(),true);
                pDragNode->setTouchEnabled(true);
            });
        }
    }else if (name == "tool_normal"){
        AudioHelp::getInstance()->stopLoopEffect();
    }
}

void CaramelMeltSugarScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CaramelMeltSugarScene::_initData()
{
    setExPath("content/make/caramel/make1/");
    GameDataManager::getInstance()->m_nFlavorIndex = 5;
}

void CaramelMeltSugarScene::_showShadow(int index,bool show)
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
void CaramelMeltSugarScene::_frozeIngredients(int index,bool froze)
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
void CaramelMeltSugarScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("pan.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 365-visibleSize.width, 400);
    m_pBowl->runAction(Spawn::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                     EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL));
    
    m_pMixture = MixPoint::create();
    m_pBowl->addChild(m_pMixture);
    m_pMixture->setPosition(Vec2(192, 245));
    
}

void CaramelMeltSugarScene::_showMixTool()
{
    m_pMixTool = _createDrageNode(localPath("spoon1_1.png"));
    this->addChildToContentLayer(m_pMixTool);
    CMVisibleRect::setPositionAdapted(m_pMixTool, 320, 750);
    m_pMixTool->setName("tool_normal");
    ActionHelper::showBezier(m_pMixTool, m_pMixTool->getPosition(), ActionHelper::ShowDirection::show_from_right,1,[=](){
        m_pMixTool->setOrgPositionDefault();
        m_pMixTool->setTouchEnabled(true);
    });
    AudioHelp::getInstance()->playEffect("present_in.mp3");
}


DragNode* CaramelMeltSugarScene::_createContainer(int index,bool isWater)
{
    //    back shadow front food food2
    static std::string sIngredents[][6] ={
        {"butter_plate.png","butter_plate1.png","","butter.png","","butter_shadow.png"},
        {"brown_sugar1.png","brown_sugar1_2.png","brown_sugar1_1.png"},
        {"golden_syrup1.png","golden_syrup1_1.png","golden_syrup3.png"},
        
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
        if(sIngredents[index][5]!=""){
            pNode->addFoodShadow(localPath(sIngredents[index][5]));
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


void CaramelMeltSugarScene::_showStove()
{
    Sprite* pStove = Sprite::create(localPath("stove.png"));
    this->addChildToContentLayer(pStove);
    CMVisibleRect::setPositionAdapted(pStove, 320, 420);
    
    Sprite* pFireSmall = Sprite::create(localPath("fire2_1.png"));
    this->addChildToContentLayer(pFireSmall);
    pFireSmall->setPosition(pStove->getPosition());
    pFireSmall->runAction(Repeat::create(AnimationHelp::createAnimate(localPath("fire2_"), 1, 2),4));
    
    Sprite* pFire = Sprite::create(localPath("fire1_1.png"));
    this->addChildToContentLayer(pFire);
    pFire->setPosition(pStove->getPosition());
    pFire->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("fire1_"), 1, 2)));
}

void CaramelMeltSugarScene::_showIngredient(int index)
{
    bool bIsWater = false;
    
    DragNode* pIngredient = _createContainer(index,bIsWater);
    if (!pIngredient) {
        return;
    }
    m_IngredientVector.pushBack(pIngredient);
    float dr = 0.5;
    switch (index) {
        case 0:
        {
            m_pButter = pIngredient;
            pIngredient->setActionPoint(Vec2(40, 60));
            CMVisibleRect::setPositionAdapted(pIngredient, 150, 780);
            dr = 0.5;
        }
            break;
        case 1:
        {
            m_pBrownSugar = pIngredient;
            pIngredient->setActionPoint(Vec2(15, 211));
            CMVisibleRect::setPositionAdapted(pIngredient, 340, 830);
            dr = 1;
        }
            break;
        case 2:
        {
            m_pSyrup = pIngredient;
            pIngredient->setActionPoint(Vec2(62, 232));
            CMVisibleRect::setPositionAdapted(pIngredient, 516, 820);
            dr = 1.5;
            
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
        pIngredient->runAction(Sequence::create(DelayTime::create(2-dr),
                                                CallFunc::create([=](){
            pIngredient->setTouchEnabled(true);
        }), NULL));
    });
}

void CaramelMeltSugarScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "pan_butter2.png",
        "pan_sugar.png",
        "pan_golden_syrup.png",
    };
    int index = pIngredient->getTag();
    _frozeIngredients(index);
    int k = index;
    
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    //    pIngredient->setLocalZOrder(5);
    
    m_pGuideLayer->removeGuide();
    switch (index) {
#pragma mark =========  add Butter ==========
        case 0:
        {
            posAdd = Vec2(50, 20);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(60, 60)),
                                                    RotateTo::create(0.5,-60),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("add_buter.mp3");
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    MoveBy::create(1,Vec2(80, 100)),
                                                    MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 100)),
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
                                                    EaseElasticIn::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),1.0),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showMixTool();
                }
                m_IngredientVector.eraseObject(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add Brown sugar ==========
        case 1:
        {
            posAdd = Vec2(-40, 0);
            Node* pLid = pIngredient->getChildByName("front");
            if(pLid){
                pLid->runAction(Sequence::create(JumpBy::create(1, Vec2(100, 600), visibleSize.height, 1),
                                                 CallFunc::create([=](){
                    pLid->setVisible(false);
                }), NULL));
            }
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(150, 200)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_sugar.plist");
                pIngredient->addChild(pParticle);
                pParticle->setTexture(Sprite::create(localPath("brown_sugar_particle.png"))->getTexture());
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                pIngredient->getDragSprite()->setTexture(localPath("brown_sugar2.png"));
            }),
                                                    MoveBy::create(2,Vec2(-100, 80)),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
                pIngredient->getDragSprite()->setTexture(localPath("brown_sugar1.png"));
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                
            }),
                                                    EaseElasticIn::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),1.0),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showMixTool();
                }
                m_IngredientVector.eraseObject(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add Syrup ==========
        case 2:
        {
            posAdd = Vec2(-50, -50);
            Node* pLid = pIngredient->getChildByName("front");
            if(pLid){
                pLid->runAction(Sequence::create(JumpBy::create(1, Vec2(100, 600), visibleSize.height, 1),
                                                 CallFunc::create([=](){
                    pLid->setVisible(false);
                }), NULL));
            }
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 100)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("drop_vanilla.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_vanilla.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
            }),
                                                    JumpBy::create(2,Vec2::ZERO,50,3),
                                                    MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 100)),
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
                                                    EaseElasticIn::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),1.0),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showMixTool();
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
        m_pMixture->addChild(pAdd,5);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         FadeIn::create(1),
                                         CallFunc::create([=](){
            
//            _frozeIngredients(index,false);
        }), NULL));
        if (k == 0) {
            pAdd->runAction(Sequence::create(DelayTime::create(2+dt),
                                             CallFunc::create([=](){
                pAdd->setTexture(localPath("pan_butter1.png"));
            }), NULL));
        }
    }
}


void CaramelMeltSugarScene::_mixBatter()
{
    m_pMixture->setRotation(m_pMixture->getRotation()+5);
    if (m_nMixCount>500) {
        return;
    }
    
    m_nMixCount++;
    if (m_nMixCount%100==0) {
        int mixtureStep = m_nMixCount/100;
        int mixTotal = 5;
        if (mixtureStep<=mixTotal){
            std::stringstream ostr;
            ostr<<"pan_"<<mixtureStep<<".png";
            Sprite* pMixture = Sprite::create(localPath(ostr.str()));
            m_pMixture->removeAllChildren();
            m_pMixture->addChild(pMixture);
            if (mixtureStep == 1) {
                m_pMixTool->setActionPoint(Vec2(45, 27));
                m_pMixTool->setRotation(0);
                m_pMixTool->setDragSprite(Sprite::create(localPath("spoon2.png")));
            }
        }
        if (mixtureStep>=mixTotal){
            //mix finish
            m_pMixture->stopAllActions();
            
            _finishMix();
            AudioHelp::getInstance()->stopLoopEffect();
        }
    }
}
void CaramelMeltSugarScene::_finishMix()
{
    AudioHelp::getInstance()->playEffect("present_out.mp3");
    m_pMixTool->setTouchEnabled(false);
    m_pMixTool->stopAllActions();
    m_pMixTool->setLocalZOrder(20);
    m_pMixTool->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
    
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
        SceneManager::replaceTheScene<CaramelMixScene>();
    }), NULL));
}


void CaramelMeltSugarScene::_ingredientOut(DragNode* pNode)
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