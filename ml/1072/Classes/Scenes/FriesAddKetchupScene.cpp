
#include "FriesAddKetchupScene.h"
#include "SceneManager.h"

FriesAddKetchupScene::FriesAddKetchupScene()
{
    m_bAddKetchup = false;
}

FriesAddKetchupScene::~FriesAddKetchupScene()
{
    
}
bool FriesAddKetchupScene::init()
{
    if ( !ExtensionScene::init() )
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
    
    _showFries();
    
    m_pGameUI->showNormalLayout();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    std::memset(GameDataManager::getInstance()->m_bFriesKetchup, 0, sizeof(GameDataManager::getInstance()->m_bFriesKetchup));
    return true;
}

void FriesAddKetchupScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FriesAddKetchupScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FriesAddKetchupScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagNext) {
        btn->setEnabled(false);
        SceneManager::replaceTheScene<FriesEatScene>();
        
    }else if (tag==GameUILayoutLayer::eUIButtonTagReset){
        m_pGameUI->hideButton(GameUILayoutLayer::eUIButtonTagReset);
        _resetKetchup();
    }
    ExtensionScene::onButtonCallback(btn);
}

void FriesAddKetchupScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "ketchup"){
        if(pDragNode->index == 0){
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("ketchup2.png")));
        }
        pDragNode->getDragSprite()->setRotation(-60);;
    }
}

void FriesAddKetchupScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "ketchup"){
        if(pDragNode->index == 1){
            for (int i = 0; i<m_FriesVector.size(); i++) {
                DragNode* pFries = m_FriesVector.at(i);
                if (pFries->index==0) {
                    Rect rect = Rect(-30+pFries->getWorldSpaceActionPoint().x, -30+pFries->getWorldSpaceActionPoint().y, 60, 60);
                    //                    rect.origin = pDragNode->convertToWorldSpace(rect.origin);
                    if (rect.containsPoint(worldPoint)) {
                        if (!m_bAddKetchup) {
                            m_pGameUI->showResetLayout();
                            MenuLayer::addIngredient(MenuLayer::ePackageFries, MenuLayer::eFries_ketchup);
                            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                        }
                        m_bAddKetchup = true;
                        int tag = pFries->getTag();
                        GameDataManager::getInstance()->m_bFriesKetchup[tag] = true;
                        
                        pFries->index = 1;
                        Sprite* pSauce = Sprite::create(localPath("ketchup2_"+std::to_string(4-tag%4)+".png"));
                        pSauce->setScale(0.2);
                        pSauce->setOpacity(0);
                        pSauce->setRotation(pFries->getRotation());
                        pSauce->setAnchorPoint(Vec2(0.5, 1));
                        this->addChildToContentLayer(pSauce);
                        pSauce->setLocalZOrder(tag);
                        pSauce->setPosition(pFries->getWorldSpaceActionPoint());
                        
                        m_pGuideLayer->removeGuide();
                        pSauce->runAction(Sequence::create(Spawn::create(FadeIn::create(0.3),
                                                                         ScaleTo::create(0.8, 1), NULL),
                                                           CallFunc::create([=](){
                            pFries->getDragSprite()->setTexture(localPath("french_fries"+std::to_string(i%4+1)+"_1.png"));
                            pSauce->removeFromParent();
                        }), NULL));
                        break;
                    }
                }
            }
        }
        
    }

}

void FriesAddKetchupScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name == "ketchup"){
        pDragNode->back();
        pDragNode->getDragSprite()->setRotation(0);
    }
}

void FriesAddKetchupScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void FriesAddKetchupScene::_initData()
{
    setExPath("content/make/fries/3/");
}

void FriesAddKetchupScene::_resetKetchup()
{
    m_bAddKetchup = false;
    for (int i = 0; i<m_FriesVector.size(); i++) {
        DragNode* pFries = m_FriesVector.at(i);
        pFries->index = 0;
        pFries->getDragSprite()->setTexture(localPath("french_fries"+std::to_string(i%4+1)+".png"));
        
        pFries->setPosition(pFries->getOrgPosition());
    }
    std::memset(GameDataManager::getInstance()->m_bFriesKetchup, 0, sizeof(GameDataManager::getInstance()->m_bFriesKetchup));
    
    MenuLayer::addIngredient(MenuLayer::ePackageFries, MenuLayer::eFries_ketchup,false);
}


void FriesAddKetchupScene::_showFries()
{
    int index = GameDataManager::getInstance()->m_nFriesBoxIndex;
    
    if (index<10) {
        m_pBox = Sprite::create("content/category/box/down/1_10.png");
    }else{
        m_pBox = Sprite::create("content/category/box/down/"+std::to_string(index)+".png");
    }
    this->addChildToContentLayer(m_pBox);
    CMVisibleRect::setPositionAdapted(m_pBox, 320-visibleSize.width, 320+200);
    m_pBox->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
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
        float rotation =0;
        Vec2 pos =Vec2::ZERO;
        if (i<8) {
            rotation = -30+i*7;
            pos = Vec2(-130+40*i, 100);
        }else if (i<12){
            rotation = -35+(i-7)*10;
            pos = Vec2(-100+70*(i-8), 60);
        }else{
            rotation = -20+(i-12)*12;
            pos = Vec2(-60+70*(i-12), 40);
        }
        
        DragNode* pFries = _createDrageNode(localPath("french_fries"+std::to_string(i%4+1)+".png"));
        pFries->setRotation(rotation);
        pFries->setPosition(pos);
        m_pFriesNode->addChild(pFries);
        pFries->setTouchEnabled(false);
        pFries->runAction(Sequence::create(DelayTime::create(1),
                                           CallFunc::create([=](){
            pFries->setOrgPositionDefault();
        }), NULL));
        pFries->setName("fries");
        pFries->setTag(i);
        pFries->setActionPoint(Vec2(pFries->getContentSize().width/2, pFries->getContentSize().height));
        
        m_FriesVector.push_back(pFries);
    }
    
    m_pKetchup = _createDrageNode(localPath("ketchup1.png"));
    this->addChildToContentLayer(m_pKetchup);
    CMVisibleRect::setPositionAdapted(m_pKetchup, 550+visibleSize.width, 320+50);
    m_pKetchup->setTouchEnabled(false);
    m_pKetchup->setName("ketchup");
    m_pKetchup->setActionPoint(Vec2(16, 207));
    m_pKetchup->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           CallFunc::create([=](){
        m_pKetchup->setOrgPositionDefault();
        m_pKetchup->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pKetchup->getPosition(), m_pBox->getPosition());
        m_pGameUI->showNextLayout();
    }), NULL));
}
