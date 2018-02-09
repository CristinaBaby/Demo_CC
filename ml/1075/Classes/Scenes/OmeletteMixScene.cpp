
#include "OmeletteMixScene.h"
#include "SceneManager.h"

OmeletteMixScene::OmeletteMixScene()
{
    m_nStep = 0;
    m_nEggCount = 0;
    m_pMixTool = nullptr;
}

OmeletteMixScene::~OmeletteMixScene()
{
    
}
bool OmeletteMixScene::init()
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
    
    _showBowl();
    
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        _showIngredient(0);
        _showIngredient(1);
        _showIngredient(2);
        AudioHelp::getInstance()->playEffect("vo_add3eggs_bowl.mp3");
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void OmeletteMixScene::onEnter()
{
    ExtensionScene::onEnter();
}

void OmeletteMixScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void OmeletteMixScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    
    if (name == "ingredient"){
        int tag = pDragNode->getTag();
        IngredientManager::getInstance()->showShadow(tag,false);
    }else if (name == "tool_normal"){
        int index = pDragNode->index;
        if (index == 0) {
            pDragNode->index = 1;
            AudioHelp::getInstance()->playEffect("drag_corn.mp3");
            
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(Spawn::create(RotateBy::create(1, -90),
                                                                MoveTo::create(1, Vec2(visibleSize.width/2, m_pBowl->getPositionY()+100)), NULL),
                                                  CallFunc::create([=](){
                pDragNode->setRotation(0);
                pDragNode->setDragSprite(Sprite::create(localPath("agitator1.png")));
                
                Rect rect = m_pBowl->getBoundingBox();
                rect.origin.x += 92;
                rect.origin.y += 110;
                rect.size = Size(240, 110);
                pDragNode->setLimitRect(rect);
                
                pDragNode->setTouchEnabled(true);
                
//                AudioHelp::getInstance()->_playEffectGoodChooice();
//                _showProgressStar();
            }), NULL));
        }else{
            pDragNode->setLocalZOrder(8);
        }
    }
}

void OmeletteMixScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "tool_normal"){
        int index = pDragNode->index;
        if (index == 1){
            m_pMixTool->getDragSprite()->setTexture(localPath("agitator2.png"));
            pDragNode->index = 2;
        }else if (index == 2) {
            pDragNode->setLocalZOrder(8);
            Rect bowlRect = m_pBowl->getBoundingBox();
            if (bowlRect.containsPoint(worldPoint)) {
                AudioHelp::getInstance()->playLoopEffect("liquid_mixing.mp3");
                MixManager::getInstance()->mix();
            }else{
                AudioHelp::getInstance()->stopLoopEffect();
                m_pBowl->stopAllActions();
            }
            
        }
    }
}

void OmeletteMixScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        int tag = pDragNode->getTag();
        Rect rectBowl = m_pBowl->getBoundingBox();
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)) {
            m_pGuideLayer->removeGuide();
            _addIngredient(pDragNode);
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),[=](){
                IngredientManager::getInstance()->showShadow(pDragNode->getTag(),true);
                pDragNode->setTouchEnabled(true);
            });
        }
        
    }else if (name == "tool_normal"){
        int index = pDragNode->index;
        AudioHelp::getInstance()->stopLoopEffect();
        if(index == 1){
            AudioHelp::getInstance()->stopLoopEffect();
            m_pBowl->stopAllActions();
        }
//        _toolElectricWork(false);
    }
}

void OmeletteMixScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

#pragma mark - initData
void OmeletteMixScene::_initData()
{
    setExPath("content/make/omelette/make1/");
}

void OmeletteMixScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl_down.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320+visibleSize.width, 380);
    
    m_pBowlFront = Sprite::create(localPath("bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    
    m_pBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    m_pBowlFront->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pMixtureClipping = ClippingNode::create(Sprite::create(localPath("bowl_mask.png")));
    this->addChildToContentLayer(m_pMixtureClipping);
    CMVisibleRect::setPositionAdapted(m_pMixtureClipping, 320, 380);
    m_pMixtureClipping->setAlphaThreshold(0.5);
    m_pMixtureClipping->setLocalZOrder(5);
    
    m_pMixture = Node::create();
    m_pMixtureClipping->addChild(m_pMixture);
    
    
    MixManager::getInstance()->reset();
    MixManager::getInstance()->_mixtureStartIndex = 1;
    MixManager::getInstance()->_mixCount = 3;
    MixManager::getInstance()->_mixSpeed = 100;
    MixManager::getInstance()->_mixRotateSpeed = 0;
    MixManager::getInstance()->_mixturePathEX = localPath("stir");
    MixManager::getInstance()->_mixtureNode = m_pMixture;
    MixManager::getInstance()->_mixtureSize = m_pBowl->getContentSize();
    MixManager::getInstance()->onMixCallback = [=](int index){
        if (index==2) {
            AudioHelp::getInstance()->playEffect("vo_keep_stirring.mp3");
        }
    };
    MixManager::getInstance()->onScheduleCallback = [=](int index){
//        m_pProgressStar->addStar();
    };
    MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(OmeletteMixScene::_finishMix, this);
}

void OmeletteMixScene::_showMixTool()
{
    if (!m_pMixTool) {
        m_pMixTool = _createDrageNode(localPath("agitator.png"));
        this->addChildToContentLayer(m_pMixTool);
        m_pMixTool->setName("tool_normal");
    }else{
        m_pMixTool->setDragSprite(Sprite::create(localPath("agitator.png")));
    }
    m_pMixTool->index = 0;
    m_pMixTool->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(m_pMixTool, 320+visibleSize.width, 700);
    
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    AudioHelp::getInstance()->playEffect("vo_whip_eggs_blender.mp3");
    m_pMixTool->setTag(0);
    m_pMixTool->setActionPoint(Vec2(50, 50));
    Rect rect = m_pBowl->getBoundingBox();
    rect.origin.x += 100;
    rect.origin.y += 70;
    rect.size = Size(70, 80);
    m_pMixTool->setLimitRect(rect);
    
    auto action = Sequence::create(JumpBy::create(1, Vec2(-visibleSize.width, 0),200,1),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pMixTool->runAction(action);
}

void OmeletteMixScene::_showIngredient(int index)
{
    //    back shadow front food food2
    static std::string sIngredents[][5] ={
        {"egg.png","egg_shadow.png"}, //egg
        {"egg.png","egg_shadow.png"}, //egg
        {"egg.png","egg_shadow.png"}, //egg
        {"pepper.png","pepper_shadow.png"}, //pepper
        {"salt.png","salt_shadow.png"}, //salt
        {},
    };
    
    
    Vec2 actionPoint[] = {
        Vec2(0, 0),
        Vec2(0, 0),
        Vec2(0, 0),
        Vec2(26, 165),
        Vec2(38, 155),
    };
    Vec2 pos[] = {
        Vec2(370, 700),
        Vec2(300, 690),
        Vec2(440, 690),
        Vec2(220, 750),
        Vec2(480, 740),
    };
    DragNode* pDragNode = _createDrageNode(localPath(sIngredents[index][0]));
    this->addChildToContentLayer(pDragNode);
    pDragNode->setTag(index);
    pDragNode->setLocalZOrder(index);
    pDragNode->setName("ingredient");
    pDragNode->setTouchEnabled(true);
    pDragNode->setActionPoint(actionPoint[index]);
    CMVisibleRect::setPositionAdapted(pDragNode, pos[index]+Vec2(visibleSize.width,0));
    
    IngredientManager::getInstance()->addIngredient(pDragNode);
    IngredientManager::getInstance()->addContainer(pDragNode,localPath(""), sIngredents[index]);
    
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
void OmeletteMixScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "bowl_egg.png",
        "bowl_egg.png",
        "bowl_egg.png",
        "bowl_pepper.png",
        "bowl_salt.png",
    };
    int index = pIngredient->getTag();
    IngredientManager::getInstance()->frozeIngredients(index);
    int k = index;
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setTouchEnabled(false);
    if (index != 4 || index != 3) {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            pIngredient->setLocalZOrder(5);
        }), NULL));
    }
    
    switch (index) {
#pragma mark =========  add Egg ==========
        case 0:
        case 1:
        case 2:
        {
            dt = 4;
            posAdd = Vec2(-60+60*(m_nEggCount/2)+120*(m_nEggCount%2), -50-50*(m_nEggCount/2));
            Vec2 posEgg = m_pMixtureClipping->getPosition()+posAdd+Vec2(0, 100);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(140, 120)),
                                                    JumpBy::create(1,Vec2::ZERO,20,2),
                                                    CallFunc::create([=](){
                pIngredient->setVisible(false);
                
                Sprite* pEgg = Sprite::create(localPath("egg1.png"));
                this->addChildToContentLayer(pEgg);
                pEgg->setPosition(pIngredient->getWorldSpaceActionPoint());
                pEgg->setLocalZOrder(5);
                
                pEgg->runAction(Sequence::create(MoveTo::create(0.5, posEgg),
                                                 CallFunc::create([=](){
                    AudioHelp::getInstance()->playAddEggEffect();
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    pEgg->setTexture(localPath("egg2.png"));
                    if (m_nEggCount==1) {
                        AudioHelp::getInstance()->playEffect(arc4random()%100>0?"vo_sweet.mp3":"vo_fabulous.mp3");
                    }
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    IngredientManager::getInstance()->frozeIngredients(index,false);
                    
                    m_nStep++;
                    if (m_nStep == 3) {
                        _showMixTool();
                    }
                    m_nEggCount++;
                    if (m_nEggCount==3) {
                        MenuLayer::addIngredient(MenuLayer::ePackageOmelette, MenuLayer::eOmelette_egg);
                        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                    }
                    IngredientManager::getInstance()->removeIngredient(pIngredient);
                    pIngredient->removeFromParent();
                    pEgg->removeFromParent();
                    if (m_nEggCount==3) {
                        m_pMixture->removeAllChildren();
                        Sprite* pAdd = Sprite::create(localPath("stir1.png"));
                        if (pAdd) {
                            pAdd->setPosition(Vec2::ZERO);
                            m_pMixture->addChild(pAdd,5);
                        }
                    }
                }), NULL));
            }), NULL));
        }
            break;
#pragma mark =========  add pepper ==========
        case 3:
        {
            posAdd = Vec2(40, -30);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pMixtureClipping->getPosition()+Vec2(0, 150)),
                                                    RotateTo::create(0.5,-100),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("salt_shaker.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pepper.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(100);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                
            }),
                                                    JumpBy::create(1.5,Vec2(150,0),50,3),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_good_job.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                IngredientManager::getInstance()->frozeIngredients(index,false);
                
                m_nStep++;
                if (m_nStep == 5) {
                    m_pMixTool->setTouchEnabled(false);
                    m_pMixTool->index = 1;
                    m_pMixTool->runAction(Sequence::create(JumpBy::create(1, Vec2(-visibleSize.width, 0), 200, 1),
                                                           CallFunc::create([=](){
                        m_pMixTool->setTouchEnabled(true);
                        
                    }), NULL));
                }
                MenuLayer::addIngredient(MenuLayer::ePackageOmelette, MenuLayer::eOmelette_pepper);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add salt ==========
        case 4:
        {
            posAdd = Vec2(-90, -20);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pMixtureClipping->getPosition()+Vec2(-50, 150)),
                                                    RotateTo::create(0.5,-140),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("salt_shaker.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/salt.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(140);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
            }),
                                                    JumpBy::create(2,Vec2(150,0),50,3),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                IngredientManager::getInstance()->frozeIngredients(index,false);
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_great.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 5) {
                    m_pMixTool->setTouchEnabled(false);
                    m_pMixTool->index = 1;
                    m_pMixTool->runAction(Sequence::create(JumpBy::create(1, Vec2(-visibleSize.width, 0), 200, 1),
                                                           CallFunc::create([=](){
                        m_pMixTool->setTouchEnabled(true);
                        
                    }), NULL));
                }
                MenuLayer::addIngredient(MenuLayer::ePackageOmelette, MenuLayer::eOmelette_salt);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
        default:
            break;
    }
    Sprite* pAdd = Sprite::create(localPath(lNameStr));
    if (pAdd) {
        pAdd->setPosition(posAdd);
        pAdd->setOpacity(0);
        m_pMixture->addChild(pAdd,5);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         FadeIn::create(1),
                                         CallFunc::create([=](){
            m_pGuideLayer->removeGuide();
        }), NULL));
    }
    
}

void OmeletteMixScene::_finishMix()
{
    AudioHelp::getInstance()->stopLoopEffect();
    if (m_nStep==3) {
        m_pMixTool->setTouchEnabled(false);
        
        m_pMixTool->getDragSprite()->setTexture(localPath("agitator1.png"));
        m_pMixTool->runAction(Sequence::create(JumpBy::create(1, Vec2(visibleSize.width, 0), 200, 1),
                                               CallFunc::create([=](){
//            m_pMixTool->setTouchEnabled(true);
            
        }), NULL));
        
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        AudioHelp::getInstance()->playEffect("vo_add_salt_pepper_eggs.mp3");
        _showIngredient(3);
        _showIngredient(4);
        
        MixManager::getInstance()->reset();
        MixManager::getInstance()->_mixtureStartIndex = 4;
        MixManager::getInstance()->_mixCount = 3;
        MixManager::getInstance()->_mixSpeed = 100;
        MixManager::getInstance()->_mixRotateSpeed = 0;
        MixManager::getInstance()->_mixturePathEX = localPath("stir");
        MixManager::getInstance()->_mixtureNode = m_pMixture;
        MixManager::getInstance()->_mixtureSize = m_pBowl->getContentSize();
        MixManager::getInstance()->onMixCallback = [=](int index){
            if (index==2) {
                AudioHelp::getInstance()->playEffect("vo_keep_stirring.mp3");
            }
        };
        MixManager::getInstance()->onScheduleCallback = [=](int index){
            //        m_pProgressStar->addStar();
        };
        MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(OmeletteMixScene::_finishMix, this);
    }else{
        _finish();
    }
}

void OmeletteMixScene::_ingredientOut(DragNode* pNode)
{
    
}
void OmeletteMixScene::_finish()
{
    AudioHelp::getInstance()->playEffect("ding.mp3");
    m_pMixTool->setTouchEnabled(false);
    m_pMixTool->runAction(JumpBy::create(1, Vec2(visibleSize.width, 0), 200, 1));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<OmeletteFryScene>();
    }), NULL));
    
}