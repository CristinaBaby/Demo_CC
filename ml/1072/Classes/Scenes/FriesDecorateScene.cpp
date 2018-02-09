
#include "FriesDecorateScene.h"
#include "SceneManager.h"

FriesDecorateScene::FriesDecorateScene()
{
    m_pBoxScrollView = nullptr;
}

FriesDecorateScene::~FriesDecorateScene()
{
    
}
bool FriesDecorateScene::init()
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
    
    m_pBg = Sprite::create(localRootPath("bg/bg_decoration.png"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showFries();
    
    this->runAction(Sequence::create(DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_decorate_fries.mp3");
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        _showBoxs();
        m_pGameUI->showNextLayout();
        m_pGameUI->showResetLayout();
    }), NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void FriesDecorateScene::onEnter()
{
    ExtensionScene::onEnter();
}

void FriesDecorateScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    if(tag == GameUILayoutLayer::eUIButtonTagNext){
        SceneManager::pushTheScene<FriesAddKetchupScene>();
    }else if(tag == GameUILayoutLayer::eUIButtonTagReset){
        AudioHelp::getInstance()->playResetEffect();
        m_pBoxFront->setTexture("content/category/box/on/7.png");
        m_pBox->setTexture("content/category/box/down/1_10.png");
    }
}
void FriesDecorateScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FriesDecorateScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void FriesDecorateScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void FriesDecorateScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void FriesDecorateScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void FriesDecorateScene::_initData()
{
    setExPath("content/make/fries/3/");
}

void FriesDecorateScene::_showFries()
{
    m_pBox = Sprite::create("content/category/box/down/1_10.png");
    this->addChildToContentLayer(m_pBox);
    CMVisibleRect::setPositionAdapted(m_pBox, 320-visibleSize.width, 320+10);
    m_pBox->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    m_pBoxFront = Sprite::create("content/category/box/on/7.png");
    this->addChildToContentLayer(m_pBoxFront);
    CMVisibleRect::setPositionAdapted(m_pBoxFront, 320-visibleSize.width, 320+10);
    m_pBoxFront->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pBoxFront->setLocalZOrder(10);
    
    m_pFriesNode = ClippingNode::create(Sprite::create(localPath("mask.png")));
    this->addChildToContentLayer(m_pFriesNode);
    CMVisibleRect::setPositionAdapted(m_pFriesNode, 320-visibleSize.width, 320+10);
    m_pFriesNode->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pFriesNode->setAlphaThreshold(0.5);
    
    for (int i = 0; i<15; i++) {
        Sprite* pSprite = Sprite::create(localPath("french_fries"+std::to_string(i%4+1)+".png"));
        m_pFriesNode->addChild(pSprite);
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
        pSprite->setVisible(false);
        Sprite* pTemp = Sprite::createWithTexture(pSprite->getTexture());
        this->addChildToContentLayer(pTemp);
        pTemp->setPosition(Vec2(visibleSize.width/2, visibleSize.height+pTemp->getContentSize().height));
        pTemp->runAction(Sequence::create(DelayTime::create(1+0.2*i),
                                          EaseBackOut::create(MoveTo::create(1,CMVisibleRect::getPositionAdapted(Vec2(320-70+35*(i%5), 430)))),
                                          CallFunc::create([=](){
            pSprite->setVisible(true);
            pSprite->setPosition(m_pFriesNode->convertToNodeSpace(pTemp->getPosition()));
            pSprite->runAction(Spawn::create(EaseBackOut::create(MoveTo::create(i*0.3+0.1, pos)),
                                                RotateTo::create(0.5, rotation), NULL));
            pTemp->removeFromParent();
        }), NULL));
//        pSprite->setPosition(Vec2(0, visibleSize.height));
    }
}

void FriesDecorateScene::_showBoxs()
{
    
    std::stringstream ostr;
    ostr.str("");
    ostr<<"content/category/box_icon/";
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile("box");
    if (!m_pBoxScrollView) {
        m_pBoxScrollView = ItemScrollView::createWithSize(Size(visibleSize.width, 230),false);
        this->addChildToUILayer(m_pBoxScrollView);
        
        m_pBoxScrollView->decorationData = data;
        m_pBoxScrollView->onItemCellSelected = CC_CALLBACK_3(FriesDecorateScene::_onDecorationCallback, this);
    }else{
        m_pBoxScrollView->decorationData = data;
    }
    
    m_pBoxScrollView->setDirection(ItemScrollView::Direction::eDirectionH);
    m_pBoxScrollView->onItemCellSelected = CC_CALLBACK_3(FriesDecorateScene::_onDecorationCallback, this);
    m_pBoxScrollView->stopAllActions();
    m_pBoxScrollView->setMargin(10);
    m_pBoxScrollView->setScale(0);
    CMVisibleRect::setPositionAdapted(m_pBoxScrollView, visibleSize.width,600,kBorderLeft);
    m_pBoxScrollView->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width,0)));
    m_pBoxScrollView->runAction(ScaleTo::create(0.2, 1));
    
    m_pBoxScrollView->btnPathEX = ostr.str();
    m_pBoxScrollView->bgHighlightPath = "content/category/icon/c.png";
    m_pBoxScrollView->setMargin(30);
    m_pBoxScrollView->reloadData();
}

void FriesDecorateScene::_onDecorationCallback(int index,int type,DecorateConfigData decData)
{
    if (type==1) {
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
            };
            return;
        }
    }
    AudioHelp::getInstance()->playSelectedEffect();
    
    
    m_pBoxFront->setTexture("content/category/box/on/"+std::to_string(index)+".png");
    if (index<10) {
        m_pBox->setTexture("content/category/box/down/1_10.png");
    }else{
        m_pBox->setTexture("content/category/box/down/"+std::to_string(index)+".png");
    }
    GameDataManager::getInstance()->m_nFriesBoxIndex = index;
}