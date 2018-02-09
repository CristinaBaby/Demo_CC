
#include "NachoAddIngredientScene.h"
#include "SceneManager.h"

static std::string sgIngredient[][3] = {
    {"bowl_n","bowl_nachos","nachos"},
    {"bowl_b","bowl_beans","bean"},
    {"bowl_t","bowl_tomato","tomoto"},
    {"bowl_o","bowl_onion","onion"},
    {"bowl_p","bowl_pepper","pepper"},
    {"bowl_c","bowl_cheese","cheese"},
    //    {"",""},
};
NachoAddIngredientScene::NachoAddIngredientScene()
{
    
}

NachoAddIngredientScene::~NachoAddIngredientScene()
{
    
}
bool NachoAddIngredientScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/nachos_bg2.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_drag_pan.mp3");
        
        _showPan();
        _showIngredient(0);
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void NachoAddIngredientScene::onEnter()
{
    ExtensionScene::onEnter();
}

void NachoAddIngredientScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void NachoAddIngredientScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    if (!m_bBtnEnable) {
        return;
    }
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagNext) {
        m_pGameUI->hideNext();
        
        int ingredient = MenuLayer::eNachos_bean;
        switch (m_nStep) {
            case 0:
                ingredient = MenuLayer::eNachos_chip;
                break;
            case 1:
                ingredient = MenuLayer::eNachos_bean;
                break;
            case 2:
                ingredient = MenuLayer::eNachos_tomato;
                break;
            case 3:
                ingredient = MenuLayer::eNachos_onion;
                break;
            case 4:
                ingredient = MenuLayer::eNachos_pepper;
                break;
            case 5:
                ingredient = MenuLayer::eNachos_cheese;
                break;
                
            default:
                break;
        }
        MenuLayer::addIngredient(MenuLayer::ePackageNachos, ingredient);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        
        m_nStep++;
        if (m_nStep<6) {
            m_pCurFood->removeFromParent();
            m_pBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                CallFunc::create([=](){
                m_pBowl->removeFromParent();
                m_pBowl = nullptr;
                _showIngredient(m_nStep);
            }), NULL));
        }else{
            _finish();
        }
        return;
    }
}

void NachoAddIngredientScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setVisible(true);
    int tag = pDragNode->getTag();
    std::string name = sgIngredient[tag][2];
    if (name == "bean") {
        ContainerNode* pContainer = (ContainerNode*)m_pBowl->getUserData();
        if (pContainer) {
            int tag = pDragNode->getTag();
            if (sgIngredient[tag][1] != "") {
                pContainer->addFood(localPath("bowl_beans2.png"));
            }
        }
    }
}

void NachoAddIngredientScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void NachoAddIngredientScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    Rect IngredientRect = pDragNode->getDragSprite()->getBoundingBox();
    IngredientRect.origin = pDragNode->convertToWorldSpace(IngredientRect.origin);
    Rect panRect = m_pPan->getBoundingBox();
    if (panRect.intersectsRect(IngredientRect)) {
        int tag = pDragNode->getTag();
        std::string name = sgIngredient[tag][2];
        int count = 1;
        if (name == "tomoto" || name == "onion") {
            count = 2;
        }
        
        m_nAddCount++;
        float radius = 40;
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/nachos.plist");
        
        Sprite* pTexture = Sprite::create(localPath(name+"0.png"));
        float textureSize = MIN(pTexture->getContentSize().width, pTexture->getContentSize().height);
        pParticle->setTexture(pTexture->getTexture());
        
        pParticle->setStartSize(textureSize);
        pParticle->setEndSize(textureSize+2);
        this->addChildToContentLayer(pParticle);
        pParticle->setPosition(Vec2(20, 250)+m_pPan->getPosition());
        
        Sprite* pUnit = Sprite::create(localPath(name+"0.png"));
        m_pMixture->addChild(pUnit);
        pUnit->setRotation((arc4random()%20)*18);
        pUnit->setPosition(Vec2::ZERO);
        pUnit->setOpacity(0);
        pUnit->runAction(Sequence::create(DelayTime::create(1),
                                          FadeIn::create(0.3),NULL));
        IngredientData lData = IngredientData::convertToIngredientData(tag, pUnit);
        GameDataManager::getInstance()->m_VectorIngredient.push_back(lData);
        
        for (int i = 2; i>=0; i--) {
            float angle = 0.1*tag+M_PI*0.2*(arc4random()%10);
            for (int k = 0; k<15-7*(2-i); k++) {
                Sprite* pUnit = Sprite::create(localPath(name+std::to_string(i*k%count)+".png"));
                m_pMixture->addChild(pUnit);
                pUnit->setRotation((arc4random()%20)*18);
                pUnit->setPosition(Vec2(30, 60));
                Vec2 p;
                
                int delt = (arc4random()%10)*0.6;
//                int delt = tag*1.2;
                p.x = (delt+radius)*(i+1) * cos(angle);
                p.y = (delt+radius)*(i+1) * sin(angle);
                log("======%f,%f",p.x,p.y);
                angle = angle+(k+1)*0.5;
                
                pUnit->setPosition(p);
                pUnit->setOpacity(0);
                pUnit->runAction(Sequence::create(DelayTime::create(1),
                                                  FadeIn::create(0.3),NULL));
                
                IngredientData lData = IngredientData::convertToIngredientData(tag, pUnit);
                lData.index = i%count;
                GameDataManager::getInstance()->m_VectorIngredient.push_back(lData);
            }
        }
        float duration = 1;
        if (name == "bean"){
            duration = 2;
            pDragNode->setPosition(Vec2(50, 280)+m_pPan->getPosition());
            pDragNode->runAction(Sequence::create(DelayTime::create(0.5),
                                                  CallFunc::create([=](){
                Sprite* pSpoon = (Sprite*)pDragNode->getChildByName(name);
                if (pSpoon) {
                    pSpoon->setTexture(localPath("spoon_b.png"));
                }
            }),
                                                  DelayTime::create(0.5),
                                                  MoveTo::create(0.5, pDragNode->getOrgPosition()),
                                                  CallFunc::create([=](){
                Sprite* pSpoon = (Sprite*)pDragNode->getChildByName(name);
                if (pSpoon) {
                    pSpoon->setTexture(localPath("spoon_beans.png"));
                }
                pDragNode->setVisible(false);
                ContainerNode* pContainer = (ContainerNode*)m_pBowl->getUserData();
                int tag = pDragNode->getTag();
                if (sgIngredient[tag][1] != "") {
                    pContainer->addFood(localPath("bowl_beans1.png"));
                }
            }), NULL));
        }else{
            pDragNode->setVisible(false);
            pDragNode->setPosition(pDragNode->getOrgPosition());
        }
        pDragNode->setTouchEnabled(false);
        m_bBtnEnable = false;
        pDragNode->runAction(Sequence::create(DelayTime::create(duration),
                                         CallFunc::create([=](){
            pDragNode->setTouchEnabled(true);
            if (name == "nachos") {
                if (m_nAddCount >=3) {
                    m_pGameUI->showNextLayout();
                    m_bBtnEnable = true;
                }
                GameDataManager::getInstance()->m_nNachosCount = m_nAddCount;
            }else{
                m_pGameUI->showNextLayout();
                m_bBtnEnable = true;
            }
        }), NULL));
        
        m_pGuideLayer->removeGuide();
    }else{
        pDragNode->setTouchEnabled(false);
        pDragNode->setVisible(false);
        int tag = pDragNode->getTag();
        std::string name = sgIngredient[tag][2];
        if (name == "bean") {
            ContainerNode* pContainer = (ContainerNode*)m_pBowl->getUserData();
            if (pContainer) {
                int tag = pDragNode->getTag();
                if (sgIngredient[tag][1] != "") {
                    pContainer->addFood(localPath("bowl_beans1.png"));
                }
            }
        }
        pDragNode->back(1,[=](){
            pDragNode->setTouchEnabled(true);
        });
    }
}

void NachoAddIngredientScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void NachoAddIngredientScene::_initData()
{
    setExPath("content/make/nachos/0/");
    m_nStep = 0;
}


void NachoAddIngredientScene::_showPan()
{
    m_pPan = Sprite::create(localPath("pan.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPositionAdapted(m_pPan, 320-visibleSize.width, 320);
    m_pPan->setScale(1.1);
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 320-10, 320+16);
    
    m_pPan->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
}

void NachoAddIngredientScene::_showIngredient(int index)
{
    m_nAddCount = 0;
    
    Node* pIngredient = Node::create();
    this->addChildToContentLayer(pIngredient);
    m_pBowl = pIngredient;
    
    ContainerNode* pContainer = ContainerNode::create(pIngredient);
    if (sgIngredient[index][1] != "") {
        pContainer->addFood(localPath(sgIngredient[index][1]+"1.png"));
    }
    pContainer->addBody(localPath(sgIngredient[index][0]+".png"));
    CMVisibleRect::setPositionAdapted(pIngredient, 260-visibleSize.width, 760);
    
    
    std::string name = sgIngredient[index][2];
    std::string foodPath = localPath(sgIngredient[index][1]+"1.png");
    
    DragNode* pFood = _createDrageNode(foodPath);
    this->addChildToContentLayer(pFood);
    pFood->getDragSprite()->setVisible(false);
    pFood->setVisible(false);
    pFood->setTouchEnabled(false);
    pFood->setName(sgIngredient[index][2]);
    pFood->setTag(index);
    m_pCurFood = pFood;
    
    m_pBowl->setUserData(pContainer);
    if (name == "bean") {
        Sprite* pUnit = Sprite::create(localPath("spoon_beans.png"));
        pFood->addChild(pUnit);
        pUnit->setName(name);
    }else{
        int count = 1;
        if (name == "tomoto" || name == "onion") {
            count = 2;
        }
        
        float radius = 10;
        for (int i = 4; i>=0; i--) {
            float angle = M_PI*0.4*(arc4random()%10);
            for (int k = 0; k<4; k++) {
                Sprite* pUnit = Sprite::create(localPath(name+std::to_string(i%count)+".png"));
                pFood->addChild(pUnit);
                pUnit->setRotation((arc4random()%20)*18);
                
                Vec2 p;
                p.x = radius*(i+1) * cos(angle);
                p.y = radius*(i+1) * sin(angle);
                angle = angle+(k+1)*0.5;
                pUnit->setPosition(p);
            }
        }
    }
    
    pIngredient->runAction(Sequence::create(DelayTime::create(0.5),
                                            EaseSineOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                            CallFunc::create([=](){
        pFood->setPosition(pIngredient->getPosition());
        pFood->setOrgPositionDefault();
        pFood->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(pIngredient->getPosition(), m_pPan->getPosition());
        switch (index) {
            case 0:
            {
                AudioHelp::getInstance()->playEffect("vo_add_nacho_chips.mp3");
            }
                break;
            case 1:
            {
                AudioHelp::getInstance()->playEffect("vo_add_other_ingredients0.mp3");
                this->runAction(Sequence::create(DelayTime::create(3),
                                                 CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("vo_add_other_ingredients1.mp3");
                }), NULL));
            }
                break;
            case 2:
            {
            }
                break;
                
            default:
                break;
        }
    }), NULL));
}
void NachoAddIngredientScene::_finish()
{
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<NachoBakeScene>();
    }), NULL));
}