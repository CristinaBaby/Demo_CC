
#include "HotdogMixSauceScene.h"
#include "SceneManager.h"

HotdogMixSauceScene::HotdogMixSauceScene()
{
    m_nAddOilCount = 0;
    m_nStep = 0;
    m_nMixCount = 0;
}

HotdogMixSauceScene::~HotdogMixSauceScene()
{
    
}
bool HotdogMixSauceScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_make3.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showOil();
    _showMustard();
    _showBowl();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void HotdogMixSauceScene::onEnter()
{
    ExtensionScene::onEnter();
}

void HotdogMixSauceScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void HotdogMixSauceScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name=="oil_spoon"){
        if (pDragNode->index==0){
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("oil_spoon1_1.png")));
            Sprite* pOil = (Sprite*)pDragNode->getChildByName("oil");
            if (!pOil) {
                pOil = Sprite::create(localPath("oil_spoon1_2.png"));
                pDragNode->addChild(pOil);
                pOil->setName("oil");
            }
            pOil->setPosition(Vec2::ZERO);
            pOil->setScale(1);
            pOil->setOpacity(255);
        }
        
    }else if (name=="mustard"){
        pDragNode->setDragSprite(Sprite::create(localPath("mustard_plate1.png")));
    }else if (name=="chilli_spoon") {
        if (pDragNode->index==0){
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("chilli_spoon1.png")));
        }
    }else if (name=="paprika_spoon") {
        if (pDragNode->index==0){
            pDragNode->index = 1;
            Sprite* pShadow = (Sprite*)pDragNode->getChildByName("shadow");
            if (pShadow) {
                pShadow->setVisible(false);
            }
        }
    }if (name == "whrisk"){
        if (pDragNode->index ==1){
            AudioHelp::getInstance()->playLoopEffect("stir.mp3");
            pDragNode->setLocalZOrder(5);
            
            Rect rect = m_pBowl->getBoundingBox();
            Rect limitRect = Rect(rect.origin.x+200, rect.origin.y+250, rect.size.width-220, rect.size.height*0.3);
            pDragNode->setLimitRect(limitRect);
            pDragNode->setOrgPositionDefault();
            pDragNode->setTouchEnabled(true);
            
        }
    }
}

void HotdogMixSauceScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="oil_spoon") {
        if (pDragNode->index==1){
            Rect rect = m_pBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pBowl->getPosition()+Vec2(50,200));
                
                _addOil();
            }
        }
    }else if (name=="mustard"){
        Rect rect = m_pBowl->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition())){
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pBowl->getPosition()+Vec2(50,200));
            
            _addMustard();
        }
    }else if (name=="chilli_spoon") {
        if (pDragNode->index==1){
            Rect rect = m_pChilliBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->index = 2;
                Sprite* pOil = (Sprite*)pDragNode->getChildByName("chilli");
                if (!pOil) {
                    pOil = Sprite::create(localPath("chilli_spoon2.png"));
                    pDragNode->addChild(pOil);
                    pOil->setName("chilli");
                }
                pOil->setPosition(Vec2::ZERO);
                pOil->setScale(1);
                pOil->setOpacity(255);
            }
        }else if (pDragNode->index==2){
            Rect rect = m_pBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pBowl->getPosition()+Vec2(50,200));
                
                _addChilli();
            }
        }
    }else if (name=="paprika_spoon") {
        if (pDragNode->index==1){
            Rect rect = m_pPaprikaBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->index = 2;
                Sprite* pOil = (Sprite*)pDragNode->getChildByName("paprika");
                if (!pOil) {
                    pOil = Sprite::create(localPath("smoked_paprika1.png"));
                    pDragNode->addChild(pOil);
                    pOil->setName("paprika");
                }
                pOil->setPosition(Vec2::ZERO);
                pOil->setScale(1);
                pOil->setOpacity(255);
            }
        }else if (pDragNode->index==2){
            Rect rect = m_pBowl->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pBowl->getPosition()+Vec2(50,200));
                
                _addPaprika();
            }
        }
    }else if (name == "whrisk"){
        Rect rect = m_pBowl->getBoundingBox();
        rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            _mixBatter();
        }
        
    }
}

void HotdogMixSauceScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    AudioHelp::getInstance()->stopLoopEffect();
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name==""){
        
    }
}

void HotdogMixSauceScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void HotdogMixSauceScene::_initData()
{
    setExPath("content/make/hotdog/4/");
}

void HotdogMixSauceScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl.png"));
    CMVisibleRect::setPosition(m_pBowl, 320+visibleSize.width, 330);
    this->addChildToContentLayer(m_pBowl);
    m_pBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    m_pMixture->setLocalZOrder(5);
    CMVisibleRect::setPosition(m_pMixture, 320+10, 330);
    
}

void HotdogMixSauceScene::_showOil()
{
    m_pOilBowl = Sprite::create(localPath("oil.png"));
    CMVisibleRect::setPosition(m_pOilBowl, 240+visibleSize.width, 790);
    this->addChildToContentLayer(m_pOilBowl);
    m_pOilBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pOilSpoon = _createDrageNode(localPath("oil_spoon_1.png"));
    CMVisibleRect::setPosition(m_pOilSpoon, 260+visibleSize.width, 770);
    this->addChildToContentLayer(m_pOilSpoon);
    m_pOilSpoon->setName("oil_spoon");
    
    m_pOilSpoon->setTouchEnabled(false);
    m_pOilSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pOilSpoon->setOrgPositionDefault();
        m_pOilSpoon->setTouchEnabled(true);
    }), NULL));
}

void HotdogMixSauceScene::_showMustard()
{
    m_pMustardPlate = _createDrageNode(localPath("mustard_plate.png"));
    CMVisibleRect::setPosition(m_pMustardPlate, 490+visibleSize.width, 790);
    this->addChildToContentLayer(m_pMustardPlate);
    m_pMustardPlate->setName("mustard");
    
    m_pMustard = Sprite::create(localPath("mustard.png"));
    m_pMustardPlate->addChild(m_pMustard);
    
    m_pMustardPlate->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
}

void HotdogMixSauceScene::_showChilli()
{
    m_pChilliBowl = Sprite::create(localPath("chilli.png"));
    CMVisibleRect::setPosition(m_pChilliBowl, 340+visibleSize.width, 760);
    this->addChildToContentLayer(m_pChilliBowl);
    m_pChilliBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pChilliSpoon = _createDrageNode(localPath("chilli_spoon.png"));
    CMVisibleRect::setPosition(m_pChilliSpoon, 400+visibleSize.width, 770);
    this->addChildToContentLayer(m_pChilliSpoon);
    m_pChilliSpoon->setName("chilli_spoon");
    
    m_pChilliSpoon->setTouchEnabled(false);
    m_pChilliSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pChilliSpoon->setOrgPositionDefault();
        m_pChilliSpoon->setTouchEnabled(true);
    }), NULL));
}

void HotdogMixSauceScene::_showPaprika()
{
    m_pPaprikaBowl = Sprite::create(localPath("smoked_paprika.png"));
    CMVisibleRect::setPosition(m_pPaprikaBowl, 260+visibleSize.width, 790);
    this->addChildToContentLayer(m_pPaprikaBowl);
    m_pPaprikaBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pPaprikaSpoon = _createDrageNode(localPath("smoked_paprika_spoon.png"));
    CMVisibleRect::setPosition(m_pPaprikaSpoon, 260+visibleSize.width, 770);
    this->addChildToContentLayer(m_pPaprikaSpoon);
    m_pPaprikaSpoon->setName("paprika_spoon");
    
    Sprite* pShadow = Sprite::create(localPath("smoked_paprika_shadow.png"));
    m_pPaprikaSpoon->addChild(pShadow,-1);
    pShadow->setVisible(false);
    pShadow->setName("shadow");
    
    m_pPaprikaSpoon->setTouchEnabled(false);
    m_pPaprikaSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pPaprikaSpoon->setOrgPositionDefault();
        m_pPaprikaSpoon->setTouchEnabled(true);
    }), NULL));
}

void HotdogMixSauceScene::_showWhisk()
{
    m_pWhrisk = _createDrageNode(localPath("spoon.png"));
    this->addChildToContentLayer(m_pWhrisk);
    CMVisibleRect::setPositionAdapted(m_pWhrisk, 320+visibleSize.width, 400);
    m_pWhrisk->setActionPoint(Vec2(58, 40));
    m_pWhrisk->setLocalZOrder(10);
    m_pWhrisk->setName("whrisk");
    
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                               m_pWhrisk->setOrgPositionDefault();
                                                               m_pWhrisk->setTouchEnabled(true);
                                                           }),
                                          NULL));

}

void HotdogMixSauceScene::_addOil()
{
    AudioHelp::getInstance()->playEffect("add_water.mp3");
    DragNode* pDragNode = m_pOilSpoon;
    Sprite* pOilStream = Sprite::create(localPath("oil_spoon1_3.png"));
    pDragNode->addChild(pOilStream);
    pOilStream->setName("stream");
    pOilStream->runAction(AnimationHelp::createAnimate(localPath("oil_spoon1_"), 3, 4,false));
    
    Sprite* pAdd = Sprite::create(localPath("bowl_oil.png"));
    m_pMixture->addChild(pAdd);
    pAdd->setOpacity(0);
    pAdd->setScale(0.2);
    pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                     Spawn::create(ScaleTo::create(2, 1),
                                                   FadeIn::create(1), NULL), NULL));
    Node* pOil = pDragNode->getChildByName("oil");
    if (pOil) {
        pOil->runAction(Sequence::create(DelayTime::create(0.5),
                                         Spawn::create(MoveBy::create(2, Vec2(-60, -20)),
                                                       ScaleTo::create(2, 0.3), NULL),
                                         FadeOut::create(0.3),
                                         CallFunc::create([=](){
            pOilStream->removeFromParent();
            
            _finishAddOil();
        }), NULL));
    }
}

void HotdogMixSauceScene::_addMustard()
{
    AudioHelp::getInstance()->playEffect("buter.mp3");
    m_pMustard->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -100)),
                                           CallFunc::create([=](){
        m_pMustard->setVisible(false);
        Sprite* pAdd = Sprite::create(localPath("bowl_mustard.png"));
        m_pMixture->addChild(pAdd);
        _finishAddMustard();
    }), NULL));
}
void HotdogMixSauceScene::_addChilli()
{
    DragNode* pDragNode = m_pChilliSpoon;
    AudioHelp::getInstance()->playPourPowderEffect();
    
    for (int i = 0; i<3; i++) {
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/garlic.plist");
        pParticle->setTexture(Sprite::create(localPath("chilli_granule"+std::to_string(i+1)+".png"))->getTexture());
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
        pParticle->setLocalZOrder(10);
    }
    Sprite* pAdd = Sprite::create(localPath("bowl_chilli.png"));
    m_pMixture->addChild(pAdd);
    pAdd->setOpacity(0);
    pAdd->setScale(0.2);
    pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                     Spawn::create(ScaleTo::create(2, 1),
                                                   FadeIn::create(1), NULL), NULL));
    
    Node* pChilli = pDragNode->getChildByName("chilli");
    if (pChilli) {
        pChilli->runAction(Sequence::create(DelayTime::create(0.5),
                                         Spawn::create(MoveBy::create(1, Vec2(-60, 20)),
                                                       ScaleTo::create(1, 0.3), NULL),
                                         FadeOut::create(0.3),
                                         CallFunc::create([=](){
            _finishAddChilli();
        }), NULL));
    }
}

void HotdogMixSauceScene::_addPaprika()
{
    AudioHelp::getInstance()->playPourPowderEffect();
    DragNode* pDragNode = m_pPaprikaSpoon;
    
    pDragNode->runAction(RotateBy::create(0.3, 60));
    pDragNode->setLocalZOrder(5);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/garlic.plist");
    pParticle->setTexture(Sprite::create(localPath("smoked paprika_granule.png"))->getTexture());
    pParticle->setTotalParticles(60);
    pParticle->cocos2d::Node::setScaleX(-1);
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
    pParticle->setLocalZOrder(10);
    
    Sprite* pAdd = Sprite::create(localPath("bowl_smoked-paprika.png"));
    m_pMixture->addChild(pAdd);
    pAdd->setOpacity(0);
    pAdd->setScale(0.2);
    pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                     Spawn::create(ScaleTo::create(2, 1),
                                                   FadeIn::create(1), NULL), NULL));
    
    Node* pPaprika = pDragNode->getChildByName("paprika");
    if (pPaprika) {
        pPaprika->runAction(Sequence::create(DelayTime::create(0.5),
                                            Spawn::create(MoveBy::create(1, Vec2(20, 30)),
                                                          ScaleTo::create(1, 0.3), NULL),
                                            FadeOut::create(0.3),
                                            CallFunc::create([=](){
            
            _finishAddPaprika();
        }), NULL));
    }
}

void HotdogMixSauceScene::_mixBatter()
{
    m_pMixture->setRotation(m_pMixture->getRotation()+1);
    if (m_pWhrisk->index == 1){
        m_nMixCount++;
        int count = 200;
        if (m_nMixCount%count==0) {
            int mixtureStep = m_nMixCount/count;
            int mixTotal = 4;
            if (mixtureStep<=mixTotal){
                std::stringstream ostr;
                ostr<<"stir"<<mixtureStep<<".png";
                Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                m_pMixture->removeAllChildren();
                m_pMixture->addChild(pMixture);
            }
            if (mixtureStep>=mixTotal){
                //mix finish
                m_pMixture->stopAllActions();
                m_pWhrisk->setDragSprite(Sprite::create(localPath("spoon.png")));
                m_pMixture->setScaleX(1);
                
                _finishMix();
                m_nMixCount = 0;
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }
    }else{
        Sprite *pShadow = (Sprite*)m_pWhrisk->getChildByName("shadow");
        if (pShadow){
            pShadow->removeFromParent();
        }
        
        m_pWhrisk->index = 1;
        AudioHelp::getInstance()->playLoopEffect("stir.mp3");
        m_pWhrisk->setDragSprite(Sprite::create(localPath("spoon1.png")));
        m_pWhrisk->setLocalZOrder(5);
        m_pWhrisk->setRotation(0);
        
        Rect rect = m_pBowl->getBoundingBox();
        Rect limitRect = Rect(rect.origin.x+190+80, rect.origin.y+250+40, rect.size.width-200-160, rect.size.height*0.4-80);
        m_pWhrisk->setLimitRect(limitRect);
        m_pWhrisk->setOrgPositionDefault();
        //        m_pWhrisk->setTouchEnabled(false);
        m_pWhrisk->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+100));
        
    }
}


void HotdogMixSauceScene::_finishAddOil()
{
    MenuLayer::addIngredient(MenuLayer::ePackageHotdog, MenuLayer::eHotdog_olive_oil);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    m_pOilBowl->setLocalZOrder(10);
    m_pOilSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pOilBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_nStep++;
    if(m_nStep==2){
        _showChilli();
    }
}

void HotdogMixSauceScene::_finishAddMustard()
{
    MenuLayer::addIngredient(MenuLayer::ePackageHotdog, MenuLayer::eHotdog_mustard);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    m_pMustardPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_nStep++;
    if(m_nStep==2){
        _showChilli();
    }
}

void HotdogMixSauceScene::_finishAddChilli()
{
    MenuLayer::addIngredient(MenuLayer::ePackageHotdog, MenuLayer::eHotdog_chili);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    m_pChilliBowl->setLocalZOrder(10);
    m_pChilliSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pChilliBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_nStep++;
    _showPaprika();
}

void HotdogMixSauceScene::_finishAddPaprika()
{
    MenuLayer::addIngredient(MenuLayer::ePackageHotdog, MenuLayer::eHotdog_paprika);
    MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    
    m_pPaprikaBowl->setLocalZOrder(10);
    m_pPaprikaSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pPaprikaBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_nStep++;
    _showWhisk();
}

void HotdogMixSauceScene::_finishMix()
{
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                           }),
                                          NULL));
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<HotdogCutBreadScene>();
    }), NULL));
}