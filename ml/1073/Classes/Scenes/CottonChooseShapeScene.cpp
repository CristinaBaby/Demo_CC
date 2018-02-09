
#include "CottonChooseShapeScene.h"

static Vec2 gShapePos[5] = {
    Vec2(190,-6),
    Vec2(470,-6),
    Vec2(695,-6),
    Vec2(892,-6),
    Vec2(1100,-6),
};

CottonChooseShapeScene::CottonChooseShapeScene()
{
    m_bItemEnable = false;
}

CottonChooseShapeScene::~CottonChooseShapeScene()
{
    
}
bool CottonChooseShapeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg3.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showCottonCandy();
    _showShapeUI();
    
    m_pGameUI->showNormalLayout();
    return true;
}

void CottonChooseShapeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CottonChooseShapeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(2)) {
        for_each(m_ShapeItems.begin(), m_ShapeItems.end(), [=](LockItem* pItem){
            pItem->unlock();
        });
    }
}

void CottonChooseShapeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void CottonChooseShapeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void CottonChooseShapeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void CottonChooseShapeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CottonChooseShapeScene::_initData()
{
    setExPath("content/make/cotton/3/");
    m_nFlavor = GameDataManager::getInstance()->m_nFlavorIndex;
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecorate;
}

void CottonChooseShapeScene::_showCottonCandy()
{
    m_pStick = Sprite::create(localRootPath("2/stick.png"));
    this->addChildToContentLayer(m_pStick);
    CMVisibleRect::setPosition(m_pStick, 320, 150);
    
    m_pCotton = Sprite::create(localRootPath("2/candy/"+std::to_string(m_nFlavor+1)+"_5.png"));
    this->addChildToContentLayer(m_pCotton);
    CMVisibleRect::setPosition(m_pCotton, 320, 150+180);
    
}

void CottonChooseShapeScene::_showShapeUI()
{
    AudioHelp::getInstance()->playEffect("vo_choose_shape.mp3");
    m_pRope = Sprite::create(localPath("rope.png"));
    this->addChildToContentLayer(m_pRope);
    CMVisibleRect::setPositionAdapted(m_pRope, 320, 840+visibleSize.height/2,kBorderNone,kBorderTop);
    
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width, 500));
    this->addChildToUILayer(m_pScrollView);
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 540,kBorderLeft,kBorderTop);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setLocalZOrder(5);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->getInnerContainer()->setContentSize(Size(1500, 500));
    
    m_pRope->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height/2))),
                                        CallFunc::create([=](){
        
        int shapeCount = 5;
        for (int i = 0; i<shapeCount; i++) {
            LockItem* pItem = LockItem::create();
            std::stringstream ostr;
            ostr<<i+1<<"/"<<m_nFlavor+1<<".png";
            
            if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i%2==0){
                if (!ConfigManager::getInstance()->getVideoUnLocked("cotton_shape", i)) {
                    pItem->initWithName(localPath("sack.png"), "content/common/video.png",true);
                    LockUIManager::getInstance()->registerLock("cotton_shape", i, pItem);
                    LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                    Sprite* Lock = pItem->getLockSprite();
                    if (Lock) {
                        Lock->setTag(TAG_REWARD_LOCK);
                    }
                    pItem->lockType = 1;
                }else{
                    pItem->initWithName(localPath("sack.png"), "",false);
                }
                
            }else{
                pItem->initWithName(localPath("sack.png"), "",false);
            }
            
            pItem->getContentSprite()->setAnchorPoint(Vec2(0.5, 245.0/pItem->getContentSprite()->getContentSize().height));
            Sprite* pSprite = Sprite::create(localPath(ostr.str()));
            pItem->addChild(pSprite,-1);
            pSprite->setPosition(Vec2(0, -115));
            pSprite->setScale(0.3);
            if (i==1) {
                pSprite->setRotation(130);
            }else{
                
                pSprite->setRotation(150);
            }
            
            Sprite* pBowknot= Sprite::create(localPath("sack_1.png"));
            pItem->addChild(pBowknot);
            pBowknot->setName("bowknot");
            
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
                pLock->setPosition(Vec2(40, -60));
            }
            
            pItem->setTag(i);
            m_pScrollView->getInnerContainer()->addChild(pItem);
            Vec2 pos = gShapePos[i];
            pos = m_pScrollView->getInnerContainer()->convertToNodeSpace(m_pRope->convertToWorldSpace(pos));
            pItem->setPosition(pos+Vec2(visibleSize.width,50));
            
            float rotation = -5+0.25*(arc4random()%20);
            pItem->runAction(RotateBy::create(1, rotation));
            pItem->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, -50)),
                                              CallFunc::create([=](){
                m_bItemEnable = true;
                pItem->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5+i*0.2),
                                                                        RotateBy::create(2, -10),
                                                                        RotateBy::create(2, 10), NULL)));
            }), NULL));
            
            pItem->onItemClicked = CC_CALLBACK_1(CottonChooseShapeScene::_onShapeClicked, this);
            m_ShapeItems.pushBack(pItem);
        }
    }), NULL));
    
}

void CottonChooseShapeScene::_onShapeClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    int tag = item->getTag();
    if (item->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("cotton_shape", tag)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                RewardManager::getInstance()->showRewardAds("cotton_shape", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    AudioHelp::getInstance()->playSelectedEffect();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setPosition(visibleSize*0.5);
    m_bItemEnable = false;
    GameDataManager::getInstance()->m_nShapeIndex = tag;
    
    m_pCotton->setTexture(localPath(std::to_string(tag+1)+"/"+std::to_string(m_nFlavor+1)+".png"));
    m_pGameUI->showNextLayout();
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        m_bItemEnable = true;
    }), NULL));
}
