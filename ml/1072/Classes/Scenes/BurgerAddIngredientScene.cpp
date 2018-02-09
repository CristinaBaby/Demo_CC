
#include "BurgerAddIngredientScene.h"
#include "SceneManager.h"

BurgerAddIngredientScene::BurgerAddIngredientScene()
{
    m_nAddCount = 0;
    m_nMilkAddCount = 0;
}

BurgerAddIngredientScene::~BurgerAddIngredientScene()
{
    
}
bool BurgerAddIngredientScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_make1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showBowl();
    AudioHelp::getInstance()->playEffect("vo_make_beef_patty.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        _showIngredient(tagBeaf);
    }),DelayTime::create(2),
                                     CallFunc::create([=](){
        _showIngredient(tagMilk);
    }),  NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    m_pGameUI->showNormalLayout();
    return true;
}

void BurgerAddIngredientScene::onEnter()
{
    ExtensionScene::onEnter();
}

void BurgerAddIngredientScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BurgerAddIngredientScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    _hideShadow(pDragNode);
}

void BurgerAddIngredientScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    
    if (pDragNode->index==1){
        Rect rect = m_pBowl->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())){
            pDragNode->setTouchEnabled(false);
            
            _pourIngredient(pDragNode,pDragNode->getTag());
        }
    }else if (pDragNode->index == 0) {
        Node* pNode = (Node*)pDragNode->getUserObject();
        if (pNode) {
            Rect rect = pNode->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                _getIngredient(pDragNode,pDragNode->getTag());
                pDragNode->index = 1;
            }
        }else{
            pDragNode->index = 1;
        }
    }
}

void BurgerAddIngredientScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void BurgerAddIngredientScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void BurgerAddIngredientScene::_initData()
{
    setExPath("content/make/burger/1/");
}

void BurgerAddIngredientScene::_hideShadow(DragNode* pNode)
{
    Node* pShadow = pNode->getChildByName("shadow");
    if (pShadow) {
        pShadow->setVisible(false);
    }
}

void BurgerAddIngredientScene::_showBowl()
{    
    m_pBowl = Sprite::create(localPath("bowl.png"));
    CMVisibleRect::setPosition(m_pBowl, 320+visibleSize.width, 330);
    this->addChildToContentLayer(m_pBowl);
    m_pBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    m_pMixture->setLocalZOrder(5);
    CMVisibleRect::setPosition(m_pMixture, 320+30, 330+30);
}

void BurgerAddIngredientScene::_showIngredient(int index)
{
    switch (index) {
        case tagBeaf:
        {
            AudioHelp::getInstance()->playEffect("vo_add_beef.mp3");
            m_pBeafPlate = _createDrageNode(localPath("beef_plate.png"));
            CMVisibleRect::setPosition(m_pBeafPlate, 215+visibleSize.width, 780);
            this->addChildToContentLayer(m_pBeafPlate);
            m_pBeafPlate->setName("beaf");
            m_pBeafPlate->setTag(index);
            
            Sprite* pShadow = Sprite::create(localPath("beef_plate1.png"));
            m_pBeafPlate->addChild(pShadow,-1);
            pShadow->setName("shadow");
            
            m_pBeaf = Sprite::create(localPath("beef_plate2.png"));
            m_pBeafPlate->addChild(m_pBeaf);
            
            m_pBeafPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
            break;
        case tagMilk:
        {
            AudioHelp::getInstance()->playEffect("vo_add_milk.mp3");
            m_pMilkBowl = Sprite::create(localPath("milk_1.png"));
            CMVisibleRect::setPosition(m_pMilkBowl, 450+visibleSize.width, 760);
            this->addChildToContentLayer(m_pMilkBowl);
            m_pMilkBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            
            m_pMilkSpoon = _createDrageNode(localPath("milk_spoon.png"));
            CMVisibleRect::setPosition(m_pMilkSpoon, 580+visibleSize.width, 770);
            this->addChildToContentLayer(m_pMilkSpoon);
            m_pMilkSpoon->setName("milk_spoon");
            m_pMilkSpoon->setTag(index);
            m_pMilkSpoon->setUserObject(m_pMilkBowl);
            
            Sprite* pShadow = Sprite::create(localPath("milk_spoon_shadow.png"));
            m_pMilkSpoon->addChild(pShadow,-1);
            pShadow->setName("shadow");
            
            m_pMilkSpoon->setTouchEnabled(false);
            m_pMilkSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                       CallFunc::create([=](){
                m_pMilkSpoon->setOrgPositionDefault();
                m_pMilkSpoon->setTouchEnabled(true);
            }), NULL));
        }
            break;
        case tagOnion:
        {
            AudioHelp::getInstance()->playEffect("vo_add_onion.mp3");
            m_pOnionBowl = _createDrageNode(localPath("onions_bowl.png"));
            CMVisibleRect::setPosition(m_pOnionBowl, 215+visibleSize.width, 780);
            this->addChildToContentLayer(m_pOnionBowl);
            m_pOnionBowl->setName("onion");
            m_pOnionBowl->setTag(index);
            
            Sprite* pShadow = Sprite::create(localPath("onions_bowl1.png"));
            m_pOnionBowl->addChild(pShadow,-1);
            pShadow->setName("shadow");
            
            m_pOnion = Sprite::create(localPath("onions.png"));
            m_pOnionBowl->addChild(m_pOnion);
            
            m_pOnionBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
            break;
        case tagSauce:
        {
            AudioHelp::getInstance()->playEffect("vo_add_sauce.mp3");
            m_pSauceBowl = Sprite::create(localPath("sauce.png"));
            CMVisibleRect::setPosition(m_pSauceBowl, 450+visibleSize.width, 780);
            this->addChildToContentLayer(m_pSauceBowl);
            m_pSauceBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            
            m_pSauceSpoon = _createDrageNode(localPath("sauce1.png"));
            CMVisibleRect::setPosition(m_pSauceSpoon, 500+visibleSize.width, 780);
            this->addChildToContentLayer(m_pSauceSpoon);
            m_pSauceSpoon->setName("sauce_spoon");
            m_pSauceSpoon->setTag(index);
            m_pSauceSpoon->setUserObject(m_pSauceBowl);
            
            Sprite* pShadow = Sprite::create(localPath("sauce2.png"));
            m_pSauceSpoon->addChild(pShadow,-1);
            pShadow->setName("shadow");
            
            m_pSauceSpoon->setTouchEnabled(false);
            m_pSauceSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                       CallFunc::create([=](){
                m_pSauceSpoon->setOrgPositionDefault();
                m_pSauceSpoon->setTouchEnabled(true);
            }), NULL));
        }
            break;
        case tagSalt:
        {
            AudioHelp::getInstance()->playEffect("vo_add_salt.mp3");
            m_pSaltBowl = Sprite::create(localPath("slat.png"));
            CMVisibleRect::setPosition(m_pSaltBowl, 215+visibleSize.width, 780);
            this->addChildToContentLayer(m_pSaltBowl);
            m_pSaltBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            
            m_pSaltSpoon = _createDrageNode(localPath("salt_spoon_1.png"));
            CMVisibleRect::setPosition(m_pSaltSpoon, 230+visibleSize.width, 750);
            this->addChildToContentLayer(m_pSaltSpoon);
            m_pSaltSpoon->setName("salt_spoon");
            m_pSaltSpoon->setTag(index);
            m_pSaltSpoon->setUserObject(m_pSaltBowl);
            m_pSaltSpoon->setActionPoint(Vec2(33, 80));
            
            Sprite* pShadow = Sprite::create(localPath("salt_spoon_2.png"));
            m_pSaltSpoon->addChild(pShadow,-1);
            pShadow->setName("shadow");
            
            m_pSaltSpoon->setTouchEnabled(false);
            m_pSaltSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                       CallFunc::create([=](){
                m_pSaltSpoon->setOrgPositionDefault();
                m_pSaltSpoon->setTouchEnabled(true);
            }), NULL));
        }
            break;
        case tagGarlic:
        {
            AudioHelp::getInstance()->playEffect("vo_add_garlic.mp3");
            m_pGarlicBowl = _createDrageNode(localPath("garlic_bowl.png"));
            CMVisibleRect::setPosition(m_pGarlicBowl, 450+visibleSize.width, 780);
            this->addChildToContentLayer(m_pGarlicBowl);
            m_pGarlicBowl->setName("grill");
            m_pGarlicBowl->setTag(index);
            
            Sprite* pShadow = Sprite::create(localPath("garlic_bowl1.png"));
            m_pGarlicBowl->addChild(pShadow,-1);
            pShadow->setName("shadow");
            
            m_pGarlic = Sprite::create(localPath("garlic.png"));
            m_pGarlicBowl->addChild(m_pGarlic);
            
            m_pGarlicBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
            break;
        case tagPepper:
        {
            AudioHelp::getInstance()->playEffect("vo_add_pepper.mp3");
            m_pPepperBowl = Sprite::create(localPath("pepper.png"));
            CMVisibleRect::setPosition(m_pPepperBowl, 215+visibleSize.width, 780);
            this->addChildToContentLayer(m_pPepperBowl);
            m_pPepperBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            
            m_pPepperSpoon = _createDrageNode(localPath("pepper_1.png"));
            CMVisibleRect::setPosition(m_pPepperSpoon, 260+visibleSize.width, 760);
            this->addChildToContentLayer(m_pPepperSpoon);
            m_pPepperSpoon->setName("pepper_spoon");
            m_pPepperSpoon->setTag(index);
            m_pPepperSpoon->setUserObject(m_pPepperBowl);
            m_pPepperSpoon->setActionPoint(Vec2(34, 15));
            
            m_pPepperSpoon->setTouchEnabled(false);
            m_pPepperSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                       CallFunc::create([=](){
                m_pPepperSpoon->setOrgPositionDefault();
                m_pPepperSpoon->setTouchEnabled(true);
            }), NULL));
        }
            break;
        case tagEgg:
        {
            AudioHelp::getInstance()->playEffect("vo_add_egg.mp3");
            m_pEgg = _createDrageNode(localPath("egg.png"));
            CMVisibleRect::setPosition(m_pEgg, 450+visibleSize.width, 780);
            this->addChildToContentLayer(m_pEgg);
            m_pEgg->setName("egg");
            m_pEgg->setTag(index);
            
            Sprite* pShadow = Sprite::create(localPath("egg_shadow.png"));
            m_pEgg->addChild(pShadow,-1);
            pShadow->setName("shadow");
            
            m_pEgg->setTouchEnabled(false);
            m_pEgg->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                       CallFunc::create([=](){
                m_pEgg->setOrgPositionDefault();
                m_pEgg->setTouchEnabled(true);
            }), NULL));
        }
            break;
            
        default:
            break;
    }
}

void BurgerAddIngredientScene::_getIngredient(DragNode* pDragNode,int index)
{
    switch (index) {
        case tagMilk:
        {
            pDragNode->setDragSprite(Sprite::create(localPath("milk_spoon1_1.png")));
            Sprite* pSpoonIn = (Sprite*)pDragNode->getChildByName("milk");
            if (!pSpoonIn) {
                pSpoonIn = Sprite::create(localPath("milk_spoon1_2.png"));
                pDragNode->addChild(pSpoonIn);
                pSpoonIn->setName("milk");
            }
            pSpoonIn->setScale(1);
            pSpoonIn->setOpacity(255);
            pSpoonIn->setPosition(Vec2::ZERO);
        }
            break;
        case tagSauce:
        {
            pDragNode->setDragSprite(Sprite::create(localPath("sauce_spoon.png")));
            Sprite* pSpoonIn = (Sprite*)pDragNode->getChildByName("sauce");
            if (!pSpoonIn) {
                pSpoonIn = Sprite::create(localPath("sauce_spoon3.png"));
                pDragNode->addChild(pSpoonIn);
                pSpoonIn->setName("sauce");
            }
            pSpoonIn->setScale(1);
            pSpoonIn->setOpacity(255);
            pSpoonIn->setPosition(Vec2::ZERO);
        }
            break;
        case tagSalt:
        {
            pDragNode->setDragSprite(Sprite::create(localPath("salt_spoon.png")));
            Sprite* pSpoonIn = (Sprite*)pDragNode->getChildByName("salt");
            if (!pSpoonIn) {
                pSpoonIn = Sprite::create(localPath("salt_spoon1.png"));
                pDragNode->addChild(pSpoonIn);
                pSpoonIn->setName("salt");
            }
            pSpoonIn->setScale(1);
            pSpoonIn->setOpacity(255);
            pSpoonIn->setPosition(Vec2::ZERO);
        }
            break;
        case tagPepper:
        {
            pDragNode->setDragSprite(Sprite::create(localPath("pepper_spoon1.png")));
            Sprite* pSpoonIn = (Sprite*)pDragNode->getChildByName("pepper");
            if (!pSpoonIn) {
                pSpoonIn = Sprite::create(localPath("pepper_spoon2.png"));
                pDragNode->addChild(pSpoonIn);
                pSpoonIn->setName("pepper");
            }
            pSpoonIn->setScale(1);
            pSpoonIn->setOpacity(255);
            pSpoonIn->setPosition(Vec2::ZERO);
        }
            break;
            
        default:
            break;
    }
}

void BurgerAddIngredientScene::_pourIngredient(DragNode* pDragNode,int index)
{
    pDragNode->setPosition(m_pBowl->getPosition()+Vec2(80,200));
    
    pDragNode->setLocalZOrder(5);
    
    switch (index) {
        case tagBeaf:
        {
            AudioHelp::getInstance()->playEffect("buter.mp3");
            pDragNode->setPosition(m_pBowl->getPosition()+Vec2(-50, 150));
            m_pBeaf->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -100)),
                                                   CallFunc::create([=](){
                m_pBeaf->setVisible(false);
                Sprite* pAdd = Sprite::create(localPath("bowl_beef.png"));
                m_pMixture->addChild(pAdd);
                _finishAdd(index);
                MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_beaf);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }), NULL));
        }
            break;
        case tagMilk:
        {
            AudioHelp::getInstance()->playEffect("add_water.mp3");
            pDragNode->setPosition(m_pBowl->getPosition()+Vec2(130, 150));
            Sprite* pMilkStream = Sprite::create(localPath("milk1_2.png"));
            pDragNode->addChild(pMilkStream);
            pMilkStream->setName("stream");
            pMilkStream->runAction(AnimationHelp::createAnimate(localPath("milk1_"), 2, 3,false));
            
            Sprite* pAdd = Sprite::create(localPath("bowl_milk.png"));
            m_pMixture->addChild(pAdd);
            pAdd->setOpacity(0);
            pAdd->setScale(0.2);
            if (m_nMilkAddCount==0) {
                pAdd->setPosition(Vec2(50, 10));
                pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                                 Spawn::create(ScaleTo::create(2, 0.5),
                                                               FadeIn::create(1), NULL), NULL));
            }else if (m_nMilkAddCount==1) {
                pDragNode->setPosition(m_pBowl->getPosition()+Vec2(160, 150));
                pAdd->setPosition(Vec2(80, 40));
                pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                                 Spawn::create(ScaleTo::create(2, 1),
                                                               FadeIn::create(1), NULL), NULL));
            }
            Node* pMilk = pDragNode->getChildByName("milk");
            if (pMilk) {
                pMilk->runAction(Sequence::create(DelayTime::create(0.5),
                                                 Spawn::create(MoveBy::create(2, Vec2(-60, -20)),
                                                               ScaleTo::create(2, 0.3), NULL),
                                                 FadeOut::create(0.3),
                                                 CallFunc::create([=](){
                    pMilkStream->removeFromParent();
                    if (m_nMilkAddCount==0){
                        pDragNode->back(1,[=](){
                            pDragNode->index = 0;
                            pDragNode->setTouchEnabled(true);
                        });
                    }else if (m_nMilkAddCount==1) {
                        _finishAdd(index);
                        MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_milk);
                        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                    }
                    m_nMilkAddCount++;
                }), NULL));
            }
        }
            break;
        case tagOnion:
        {
            AudioHelp::getInstance()->playEffect("buter.mp3");
            pDragNode->setPosition(m_pBowl->getPosition()+Vec2(-150, 150));
            m_pOnion->runAction(Sequence::create(MoveBy::create(0.5, Vec2(100, -100)),
                                                CallFunc::create([=](){
                m_pOnion->setVisible(false);
                Sprite* pAdd = Sprite::create(localPath("bowl_onions.png"));
                m_pMixture->addChild(pAdd);
                pAdd->setPosition(Vec2(-30, 20));
                _finishAdd(index);
                MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_onion);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }), NULL));

        }
            break;
        case tagSauce:
        {
            AudioHelp::getInstance()->playEffect("buter.mp3");
            pDragNode->setPosition(m_pBowl->getPosition()+Vec2(100, 120));
            Sprite* pOilStream = Sprite::create(localPath("sauce_spoon1.png"));
            pDragNode->addChild(pOilStream);
            pOilStream->setName("stream");
            pOilStream->runAction(AnimationHelp::createAnimate(localPath("sauce_spoon"), 1, 2,false));
            
            Sprite* pAdd = Sprite::create(localPath("bowl_sauce.png"));
            m_pMixture->addChild(pAdd);
            pAdd->setOpacity(0);
            pAdd->setScale(0.2);
            pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                             Spawn::create(ScaleTo::create(2, 1),
                                                           FadeIn::create(1), NULL), NULL));
            Node* pOil = pDragNode->getChildByName("sauce");
            if (pOil) {
                pOil->runAction(Sequence::create(DelayTime::create(0.5),
                                                 Spawn::create(MoveBy::create(2, Vec2(-50, 0)),
                                                               ScaleTo::create(2, 0.3), NULL),
                                                 FadeOut::create(0.3),
                                                 CallFunc::create([=](){
                    pOilStream->removeFromParent();
                    
                    _finishAdd(index);
                    MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_sauce);
                    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                }), NULL));
            }
        }
            break;
        case tagSalt:
        {
            AudioHelp::getInstance()->playPourPowderEffect();
            pDragNode->runAction(Sequence::create(RotateBy::create(0.3, -50),
                                                  CallFunc::create([=](){
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/salt.plist");
                pParticle->setTotalParticles(60);
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
            }), NULL));
            
            Sprite* pAdd = Sprite::create(localPath("bowl_salt.png"));
            m_pMixture->addChild(pAdd);
            pAdd->setOpacity(0);
            pAdd->setScale(0.2);
            pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                             Spawn::create(ScaleTo::create(2, 1),
                                                           FadeIn::create(1), NULL), NULL));
            
            Node* pPaprika = pDragNode->getChildByName("salt");
            if (pPaprika) {
                pPaprika->runAction(Sequence::create(DelayTime::create(0.5),
                                                     Spawn::create(MoveBy::create(2, Vec2(-60, -10)),
                                                                   ScaleTo::create(2, 0.3), NULL),
                                                     FadeOut::create(0.3),
                                                     CallFunc::create([=](){
                    
                    _finishAdd(index);
                    MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_salt);
                    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                }), NULL));
            }

        }
            break;
        case tagGarlic:
        {
            AudioHelp::getInstance()->playPourPowderEffect();
            for (int i = 0; i<4; i++) {
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/garlic.plist");
                pParticle->setTexture(Sprite::create(localPath("garlic_particle"+std::to_string(i+1)+".png"))->getTexture());
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pGarlicBowl->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
            }
            pDragNode->setLocalZOrder(5);
            Sprite* pGarlic = (Sprite*)m_pGarlicBowl->getChildByName("garlic");
            if (pGarlic) {
                pGarlic->runAction(FadeOut::create(1));
            }
            
            Sprite* pAdd = Sprite::create(localPath("bowl_garlic.png"));
            m_pMixture->addChild(pAdd);
            pAdd->setPosition(Vec2(-50, -10));
            pAdd->setOpacity(0);
            pAdd->runAction(Sequence::create(FadeIn::create(1),
                                                       DelayTime::create(1),
                                             CallFunc::create([=](){
                _finishAdd(index);
                MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_garlic);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }), NULL));
        }
            break;
        case tagPepper:
        {
            AudioHelp::getInstance()->playPourPowderEffect();
            pDragNode->runAction(Sequence::create(RotateBy::create(0.3, -20),
                                                  CallFunc::create([=](){
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pepper.plist");
                pParticle->setScaleX(-1);
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
            }), NULL));
            
            Sprite* pAdd = Sprite::create(localPath("bowl_pepper.png"));
            m_pMixture->addChild(pAdd);
            pAdd->setOpacity(0);
            pAdd->setScale(0.2);
            pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                             Spawn::create(ScaleTo::create(2, 1),
                                                           FadeIn::create(1), NULL), NULL));
            pDragNode->getDragSprite()->setTexture(localPath("pepper_spoon1.png"));
            Node* pPaprika = pDragNode->getChildByName("pepper");
            if (pPaprika) {
                pPaprika->runAction(Sequence::create(DelayTime::create(0.5),
                                                     Spawn::create(MoveBy::create(2, Vec2(-40, -10)),
                                                                   ScaleTo::create(2, 0.3), NULL),
                                                     FadeOut::create(0.3),
                                                     CallFunc::create([=](){
                    _finishAdd(index);
                    MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_pepper);
                    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                }), NULL));
            }        }
            break;
        case tagEgg:
        {
            AudioHelp::getInstance()->playEffect("add_egg.mp3");
            pDragNode->setVisible(false);
            Sprite* pEgg = Sprite::create(localPath("egg.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setLocalZOrder(20);
            pEgg->setPosition(m_pBowl->getPosition()+Vec2(30, 100));
            pEgg->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("bowl_egg"), 1, 4,false,true,0.8),
                                             CallFunc::create([=](){
                Sprite* pAdd = Sprite::create(localPath("bowl_egg.png"));
                m_pMixture->addChild(pAdd);
                pAdd->setPosition(Vec2(30, -50));
                _finishAdd(index);
                MenuLayer::addIngredient(MenuLayer::ePackageBurger, MenuLayer::eBurger_egg);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                pEgg->removeFromParent();
            }), NULL));
        }
            break;
            
        default:
            break;
    }
}

void BurgerAddIngredientScene::_finishAdd(int index)
{
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_nAddCount++;
        if (m_nAddCount == 8) {
            _finish();
        }else{
            if (m_nAddCount%2==0) {
                _showIngredient(m_nAddCount);
                this->runAction(Sequence::create(DelayTime::create(2),
                                                 CallFunc::create([=](){
                    _showIngredient(m_nAddCount+1);
                }), NULL));
            }
        }
    }), NULL));
    switch (index) {
        case tagBeaf:
        {
            m_pBeafPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
            break;
        case tagMilk:
        {
            m_pMilkBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
            m_pMilkSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }
            break;
        case tagOnion:
        {
            m_pOnionBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
            break;
        case tagSauce:
        {
            m_pSauceBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
            m_pSauceSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }
            break;
        case tagSalt:
        {
            m_pSaltBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            m_pSaltSpoon->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
            break;
        case tagGarlic:
        {
            
            m_pGarlicBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }
            break;
        case tagPepper:
        {
            m_pPepperBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            m_pPepperSpoon->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        }
            break;
        case tagEgg:
        {
        }
            break;
            
        default:
            break;
    }
}

void BurgerAddIngredientScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<BurgerWearGloveScene>();
    }), NULL));
}