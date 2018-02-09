
#include "FriesEatScene.h"

FriesEatScene::FriesEatScene()
{
    m_nEatCount = 0;
    m_bButtonEable = true;
}

FriesEatScene::~FriesEatScene()
{
    
}
bool FriesEatScene::init()
{
    if ( !ShareBaseScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    Sprite* pTable = Sprite::create(localRootPath("bg/bg_decoration1.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320,480);
    this->addChildToBGLayer(pTable);
    
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    _showFries();
    
//    this->runAction(Sequence::create(DelayTime::create(0.5),
//                                     CallFunc::create([=](){
//        AudioHelp::getInstance()->playEffect("vo_eat_fries.mp3");
//    }), NULL));
    
    m_pGameUI->hideBack();
    m_pGameUI->hideButton(GameUILayoutLayer::eUIButtonTagNext);
    return true;
}

void FriesEatScene::onEnter()
{
    ShareBaseScene::onEnter();
}

void FriesEatScene::onButtonCallback(Button* btn)
{
    if (!m_bButtonEable) {
        return;
    }
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagNext) {
    }
    ShareBaseScene::onButtonCallback(btn);
}

void FriesEatScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FriesEatScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    
    if (name == "fries") {
        pDragNode->setVisible(true);
        Sprite* pFriesIn = (Sprite*)pDragNode->getUserData();
        pFriesIn->setVisible(false);
    }else if (name == "ketchup"){
        if(pDragNode->index == 0){
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("ketchup2.png")));
        }
        pDragNode->getDragSprite()->setRotation(-60);;
    }
}

void FriesEatScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "fries") {
        
    }
}

void FriesEatScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name == "fries") {
        Rect rect = m_pBox->getBoundingBox();
        rect.origin = rect.origin+Vec2(60, 120);
        rect.size = rect.size-Size(120, 120);
        if (!rect.containsPoint(pDragNode->getPosition())) {
            pDragNode->setTouchEnabled(false);
            
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                
                m_nEatCount++;
                
                if (m_nEatCount>=m_FriesVector.size() && !m_pGameUI->eatAgainIsShow()) {
                    m_pGameUI->showEatAgainLayout();
                    m_pGameUI->showNextLayout2();
                    
                    AudioHelp::getInstance()->playEffect("cheer_star.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setLocalZOrder(10);
                    pParticle->setPosition(visibleSize*0.5);
                }
            }), NULL));
            
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
            int type = pDragNode->getTag()%4;
            int count = (type==1?2:3);
            for (int i = 0; i<count; i++) {
                this->runAction(Sequence::create(DelayTime::create(0.1+0.5*i),
                                                 CallFunc::create([=](){
                    if (i<count-1) {
                        pDragNode->getDragSprite()->setTexture(localPath("french_fries"+std::to_string(type+1)+"_"+std::to_string(i+2)+".png"));
                    }else{
                        
                        pDragNode->setVisible(false);
                    }
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
                    this->addChildToUILayer(pParticle);
                    Vec2 pos = pDragNode->getDragSprite()->convertToWorldSpace(Vec2(pDragNode->getDragSprite()->getContentSize().width/2,pDragNode->getDragSprite()->getContentSize().height*(count-0.5-i)/count));
                    pParticle->setPosition(pos);
                }), NULL));
            }
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(0,[=]{
                Sprite* pFriesIn = (Sprite*)pDragNode->getUserData();
                pFriesIn->setVisible(true);
                pDragNode->setVisible(false);
                pDragNode->setTouchEnabled(true);
            });
        }
    }
}

void FriesEatScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

#pragma mark - initData
void FriesEatScene::_initData()
{
    setExPath("content/make/fries/3/");
}

void FriesEatScene::_reset()
{
    for (int i = 0; i<m_FriesVector.size(); i++) {
        DragNode* pFries = m_FriesVector.at(i);
        pFries->setPosition(pFries->getOrgPosition());
        pFries->setVisible(false);
        pFries->setTouchEnabled(true);
        Sprite* pFriesIn = (Sprite*)pFries->getUserData();
        pFriesIn->setVisible(true);
    }

}

void FriesEatScene::_showFries()
{
    int index = GameDataManager::getInstance()->m_nFriesBoxIndex;
    
    if (index<10) {
        m_pBox = Sprite::create("content/category/box/down/1_10.png");
    }else{
        m_pBox = Sprite::create("content/category/box/down/"+std::to_string(index)+".png");
    }
    this->addChildToContentLayer(m_pBox);
    CMVisibleRect::setPositionAdapted(m_pBox, 320-visibleSize.width, 320+200);
    m_pBox->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                       CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pBox->getPosition()+Vec2(0, 200), m_pBox->getPosition()+Vec2(0, -200));
    }), NULL));
    
    m_pBoxFront = Sprite::create("content/category/box/on/"+std::to_string(index)+".png");
    this->addChildToContentLayer(m_pBoxFront);
    CMVisibleRect::setPositionAdapted(m_pBoxFront, 320-visibleSize.width, 320+200);
    m_pBoxFront->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pBoxFront->setLocalZOrder(10);
    
    m_pFriesNode = ClippingNode::create(Sprite::create(localPath("mask.png")));
    this->addChildToContentLayer(m_pFriesNode);
    CMVisibleRect::setPositionAdapted(m_pFriesNode, 320-visibleSize.width, 320+200);
    m_pFriesNode->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pFriesNode->setAlphaThreshold(0.5);
    
    for (int i = 0; i<15; i++) {
        bool bKetchup = GameDataManager::getInstance()->m_bFriesKetchup[i];
        std::string str;
        if (bKetchup) {
            str = "french_fries"+std::to_string(i%4+1)+"_1.png";
        }else{
            str = "french_fries"+std::to_string(i%4+1)+".png";
        }
        Sprite* pSprite = Sprite::create(localPath(str));
        m_pFriesNode->addChild(pSprite);
        if (i<8) {
            pSprite->setPosition(Vec2(-130+40*i, 100));
            pSprite->setRotation(-30+i*7);
        }else if (i<12){
            pSprite->setPosition(Vec2(-100+70*(i-8), 60));
            pSprite->setRotation(-35+(i-7)*10);
        }else{
            pSprite->setPosition(Vec2(-60+70*(i-12), 40));
            pSprite->setRotation(-20+(i-12)*12);
        }
        pSprite->setLocalZOrder(i);
        
        DragNode* pFries = _createDrageNode(localPath(str));
        pFries->setRotation(pSprite->getRotation());
        pFries->setPosition(m_pContentLayer->convertToNodeSpace(m_pFriesNode->convertToWorldSpace(pSprite->getPosition()))+Vec2(visibleSize.width, 0));
        this->addChildToContentLayer(pFries);
        pFries->setVisible(false);
        pFries->setTouchEnabled(false);
        pFries->runAction(Sequence::create(DelayTime::create(1),
                                           CallFunc::create([=](){
            pFries->setOrgPositionDefault();
            pFries->setTouchEnabled(true);
        }), NULL));
        pFries->setUserData(pSprite);
        pFries->setName("fries");
        pFries->setTag(i);
        pFries->setActionPoint(Vec2(pSprite->getContentSize().width/2, pSprite->getContentSize().height));
        m_FriesVector.push_back(pFries);
    }
}
