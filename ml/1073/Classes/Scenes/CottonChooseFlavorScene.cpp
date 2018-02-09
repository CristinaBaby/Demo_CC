
#include "CottonChooseFlavorScene.h"



CottonChooseFlavorScene::CottonChooseFlavorScene()
{
    m_bSelected = false;
    m_pCurItem = nullptr;
}

CottonChooseFlavorScene::~CottonChooseFlavorScene()
{
    
}
bool CottonChooseFlavorScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localRootPath("bg/bg1.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pCabinetBg = Sprite::create(localPath("cabinet.png"));
    pCabinetBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pCabinetBg);
    
    Sprite* pWindow = Sprite::create(localPath("door_0.png"));
    pWindow->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pWindow);
    
    pWindow->runAction(Sequence::create(DelayTime::create(0.5),
                                        EaseBackOut::create(ScaleTo::create(0.5, 1.05)),
                                        AnimationHelp::createAnimate(localPath("door_"), 0, 1,false),
                                        ScaleTo::create(0.2, 1),
                                        CallFunc::create([=](){
        
        _showScrollView();
        AudioHelp::getInstance()->playEffect("vo_cotton_choose_flavor.mp3");
    }), NULL));
    
    
    m_pGameUI->showNormalLayout();

    
    return true;
}

#pragma mark - initData
void CottonChooseFlavorScene::_initData()
{
    setExPath("content/make/cotton/1/");
    m_nNextSceneTag = GameUIEvent::eSceneTagCottonMake;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagCottonMake;
}

void CottonChooseFlavorScene::_showScrollView()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(450, 526));
    this->addChildToContentLayer(m_pScrollView);
    CMVisibleRect::setPosition(m_pScrollView, 95, 220+15);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    m_pScrollView->setLocalZOrder(5);
    
    int count = 14;
    int colCount = 3;
    for (int i = 0; i<count; i++) {
        int row = i/colCount;
        int col = i%colCount;
        if (col==0) {
            Sprite* pBoard = Sprite::create(localPath("cabinet2.png"));
            m_pScrollView->getInnerContainer()->addChild(pBoard);
            pBoard->setPosition(Vec2(225, 100+250*((count+1)/colCount-row-1)));
            pBoard->setRotation3D(Vec3(-90, 0, 0));
            pBoard->setAnchorPoint(Vec2(0.5, 1));
            pBoard->setOpacity(0);
            if (row<2) {
                
                AudioHelp::getInstance()->playEffect("cart_banner.mp3");
            }
            pBoard->runAction(Sequence::create(DelayTime::create(0.1+0.1*i),
                                               FadeIn::create(0.1),
                                               EaseBackOut::create(RotateBy::create(0.5, Vec3(90, 0, 0))), NULL));
        }
        
        LockItem* pItem = LockItem::create();
        if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i%2==1) {
            if (!ConfigManager::getInstance()->getVideoUnLocked("sugar", i)){
                pItem->initWithName(localPath("bottle_down.png"), "content/common/video.png",true);
                LockUIManager::getInstance()->registerLock("sugar", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
            }else{
                pItem->initWithName(localPath("bottle_down.png"), "content/common/video.png",false);
            }
        }else{
            pItem->initWithName(localPath("bottle_down.png"), "content/common/video.png",false);
        }
        Sprite* pLock = pItem->getLockSprite();
        if (pLock) {
            pLock->setPosition(Vec2(50, -50));
            pLock->setTag(TAG_REWARD_LOCK);
        }
        pItem->setTag(i);
        pItem->setScale(0.8);
        
        SugarBottle* pBottle = SugarBottle::create();
        pBottle->initSugar(i);
        pItem->addChild(pBottle);
        pBottle->setName("bottol");
        
        pItem->setScale(0);
        pItem->runAction(Sequence::create(DelayTime::create(1),
                                          CallFunc::create([=]()
                                                           {
                                                               pItem->setScale(0.3);
                                                               if (pItem->getTag()==0) {
                                                                   AudioHelp::getInstance()->playEffect("sugarcan.mp3");
                                                               }
                                                           }),
                                          Spawn::create(JumpBy::create(1.2, Vec2::ZERO, 10, 2),
                                                        EaseBackOut::create(ScaleTo::create(1, 0.8)), NULL),
                                          CallFunc::create([=]()
                                                           {
                                                               if (i==count-1) {
                                                                   m_pScrollView->scrollToBottom(0.5, true);
                                                                   this->runAction(Sequence::create(DelayTime::create(0.5),
                                                                                                    CallFunc::create([=]()
                                                                                                                     {
                                                                                                                         m_pScrollView->scrollToTop(1, true);
                                                                                                                     }), NULL));
                                                               }
                                                           }),
                                           NULL));
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(90+120*col, 160+250*((count+1)/colCount-row-1)));
        pItem->onItemClicked = CC_CALLBACK_1(CottonChooseFlavorScene::onItemClicked, this);
     
        m_ItemVector.pushBack(pItem);
    }
    m_pScrollView->setInnerContainerSize(Size(m_pScrollView->getContentSize().width, 250*((count+1)/colCount)));
    
}

void CottonChooseFlavorScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (tag==GameUILayoutLayer::eUIButtonTagReset) {
        Node*pNode = m_pContentLayer->getChildByName("colorgray");
        if (pNode) {
            pNode->removeFromParent();
        }
        Node*pBottle = m_pContentLayer->getChildByName("tempBottle");
        if (pBottle) {
            pBottle->removeFromParent();
        }
        if (m_pCurItem) {
            m_pCurItem->setVisible(true);
            
            m_pGameUI->hideReset();
            m_pGameUI->hideNext();
        }
        m_bSelected = false;
    }else{
        ExtensionScene::onButtonCallback(btn);
    }
}

void CottonChooseFlavorScene::onItemClicked(LockItem *item)
{
    if (m_pScrollView->isScrolled()) {
        return;
    }
    if (m_bSelected) {
        return;
    }
    
    int tag = item->getTag();
    if (item->isLocked()) {
        if(!ConfigManager::getInstance()->getVideoUnLocked("sugar", tag)){
            
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                RewardManager::getInstance()->showRewardAds("sugar", tag);
                pLayer->removeFromParent();
            };
            
            return;
        }
    }
    
    m_bSelected = true;
    AudioHelp::getInstance()->playSelectedEffect();
    GameDataManager::getInstance()->m_nFlavorIndex = tag;
    m_pCurItem = item;
    
    LayerColor* pColor = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChildToContentLayer(pColor);
    pColor->setLocalZOrder(5);
    pColor->setName("colorgray");
    pColor->setOpacity(0);
    
    item->setVisible(false);
    SugarBottle* pTempBottle = SugarBottle::create();
    pTempBottle->initSugar(item->getTag());
    this->addChildToContentLayer(pTempBottle);
    pTempBottle->setName("tempBottle");
    pTempBottle->setPosition(m_pContentLayer->convertToNodeSpace(item->getParent()->convertToWorldSpace(item->getPosition())));
    
    pTempBottle->runAction(Repeat::create(Sequence::create(MoveBy::create(0.1, Vec2(5, 5)),
                                                    MoveBy::create(0.1, Vec2(-10, -10)),
                                                    MoveBy::create(0.1, Vec2(5, 5)),
                                                    NULL), 5));
    pTempBottle->setLocalZOrder(5);
    Vec2 position = pTempBottle->getParent()->convertToNodeSpace(visibleSize*0.5);
    pTempBottle->runAction(Sequence::create(MoveTo::create(0.5, position),
                                     DelayTime::create(1),
                                     CallFunc::create([=]()
                                                      {
                                                          AudioHelp::getInstance()->playEffect("sugarcanchoose.mp3");
                                                          pColor->runAction(FadeTo::create(0.5,160));
                                                          
                                                              pTempBottle->openLid();
                                                              
                                                              ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugarjump.plist");
                                                              std::ostringstream ostr;
                                                              ostr<<"particle/"<<tag+1<<".png";
                                                              pParticle->setTexture(Sprite::create(localPath(ostr.str()))->getTexture());
                                                              Vec2 pos = pTempBottle->getPosition();
                                                              pParticle->setPosition(Vec2(pos.x, pos.y+50));
                                                              pColor->addChild(pParticle);
                                                              pParticle->setLocalZOrder(5);
                                                          
                                                      }),
                                     EaseBackOut::create(ScaleTo::create(1, 1.2)),
                                     CallFunc::create([=]()
                                                      {
                                                          for (int i = 0; i<4; i++) {
                                                              std::stringstream ostr;
                                                              ostr<<"particle/star"<<i<<".plist";
                                                              ParticleSystemQuad* pParticle = ParticleSystemQuad::create(ostr.str());
                                                              pColor->addChild(pParticle);
                                                              pParticle->setPosition(visibleSize*0.5);
                                                          }
                                                          
                                                          AudioHelp::getInstance()->playEffect("done.mp3");
                                                          m_pGameUI->showNextLayout();
                                                          m_pGameUI->showResetLayout();
                                                      }),
                                     NULL));

}


void CottonChooseFlavorScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(2)) {
        for_each(m_ItemVector.begin(), m_ItemVector.end(), [=](LockItem* pItem){
            pItem->unlock();
        });
    }
}