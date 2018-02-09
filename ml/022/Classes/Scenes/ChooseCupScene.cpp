
#include "ChooseCupScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"
#include "SceneManager.h"

ChooseCupScene::ChooseCupScene()
{
    m_pCupLayer = nullptr;
}

ChooseCupScene::~ChooseCupScene()
{
    
}
bool ChooseCupScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg_2.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pTable = Sprite::create(localPath("bg/bg_table4.png"));
    CMVisibleRect::setPosition(pTable, 320, 350-200);
    this->addChildToBGLayer(pTable);
    
    _showCups();
    
    m_pGameUI->showNormalLayout();
    return true;
}

void ChooseCupScene::onEnter()
{
    ExtensionScene::onEnter();
    
}

void ChooseCupScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ChooseCupScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        m_pOK->setVisible(false);
        for_each(m_Cups.begin(), m_Cups.end(), [=](LockItem* pCurItem){
            Vec2 pos = pCurItem->getParent()->convertToWorldSpace(pCurItem->getPosition());
            if (pos.x<visibleSize.width/2) {
                pCurItem->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
            }else{
                pCurItem->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
            }
        });
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            SceneManager::replaceTheScene<PourJuiceScene>();
        }), NULL));
        return;
    }
    ExtensionScene::onButtonCallback(btn);
}
#pragma mark - initData
void ChooseCupScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagPourJuice;
    m_bItemTouchEnable = false;
    m_pOK = nullptr;
}


void ChooseCupScene::_showCups()
{
    m_pCupScrollView = ExpandScrollView::create();
    m_pCupScrollView->setContentSize(Size(visibleSize.width, 400));
    this->addChildToContentLayer(m_pCupScrollView);
    CMVisibleRect::setPositionAdapted(m_pCupScrollView, visibleSize.width, 480+100,kBorderLeft,kBorderBottom);
    m_pCupScrollView->setClippingEnabled(false);
    
    m_pCupScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pCupScrollView->setScrollBarEnabled(false);
    m_pCupScrollView->setTouchEnabled(true);
    int shapeCount = 20;
    for (int i = 0; i<shapeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(1)){
            if (i%2==1){
                if (ConfigManager::getInstance()->getVideoUnLocked("cup", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        
        pItem->initWithName("content/make/cup/pattern_icon/"+std::to_string(i+1)+".png", lockName,locked);
        
        if (locked) {
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
                pLock->setTag(TAG_REWARD_LOCK);
                LockUIManager::getInstance()->registerLock("cup", i, pLock->getParent());
                LockUIManager::getInstance()->setLockPos(Vec2(30, -80));
            }
        }
        
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(ChooseCupScene::_onItemCallback, this);
        m_pCupScrollView->getInnerContainer()->addChild(pItem);
        CMVisibleRect::setPositionAdapted(pItem, 167+200*i, 250);
        m_Cups.push_back(pItem);
        
    }
    
    AudioHelp::getInstance()->playEffect("vo_choose_cup.mp3");
    m_pCupScrollView->setInnerContainerSize(Size(167+200*shapeCount, 500));
    m_pCupScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                 CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("slide_in_out.mp3");
    }),
                                                 MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                                 CallFunc::create([=](){
        m_pCupScrollView->scrollToRight(1, true);
    }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
        m_pCupScrollView->scrollToLeft(0.5, true);
    }), NULL));
}


void ChooseCupScene::_onItemCallback(LockItem* item)
{
//    if (!m_bItemTouchEnable) {
//        return;
//    }
    if(m_pCupScrollView->isScrolled()){
        return;
    }
    int tag = item->getTag();
    if (item->getChildByTag(TAG_REWARD_LOCK))  {
        RewardManager::getInstance()->showRewardAds("cup", tag);
        return;
    }
    AudioHelp::getInstance()->playEffect("select_cup.mp3");
    AudioHelp::getInstance()->playEffect("vo_great.mp3");
    m_bItemTouchEnable = false;
    
    if (!m_pOK) {
        m_pOK = Sprite::create("content/common/ok.png");
        m_pCupScrollView->getInnerContainer()->addChild(m_pOK);
    }
    m_pOK->setPosition(item->getPosition());
    
    GameDataManager::getInstance()->m_nCupIndex = tag;
    
    if (m_pCupLayer) {
        CupLayer* preCup = m_pCupLayer;
        preCup->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -visibleSize.height)),
                                           CallFunc::create([=](){
            preCup->removeFromParent();
        }), NULL));
    }else{
        m_pGameUI->showNextLayout();
    }
    m_pCupLayer = CupLayer::create();
    m_pCupLayer->initWithIndex(tag);
    this->addChildToContentLayer(m_pCupLayer);
    CMVisibleRect::setPositionAdapted(m_pCupLayer, 320, 300-visibleSize.height);
    m_pCupLayer->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height)));
}