//
// AnimalChooseScene.cpp
// ANIM3203
//
// Created by dengcheng on 15/9/14.
//
//

#include "AnimalChooseScene.h"
#include "HomeScene.h"
#include "ViewPager.h"
#include "SPAScene.h"
#include "AnimalModel.h"
#include "IAPManager.h"
#include "ShopLayer.h"
#include "LoadingLayer.h"

static const std::string g_ModelImageFilePath[] = {
    "cocoStudio/ui02_select/Tabby.png",
    "cocoStudio/ui02_select/Beagle.png",
    "cocoStudio/ui02_select/Ragdoll.png",
    "cocoStudio/ui02_select/Schnauzer.png"
};

AnimalChooseScene::AnimalChooseScene()
:m_bFirstIn(true)
,m_bDrag(false)
,m_pItem(nullptr)
,m_bCross(false)
{
    
}

void AnimalChooseScene::onEnter()
{
    BaseScene::onEnter();
    
    if (m_bCross && !IAPManager::getInstance()->isAdRemoved() && !LoadingLayer::s_currentInstance) {
        m_bCross = false;
        LoadingLayer::showLoading<LoadingLayer>(true, this, 99999);
        LoadingLayer::s_currentInstance->loadingDoneCallback = CC_CALLBACK_0(AnimalChooseScene::onAdsShowDone, this);
    } else {
        if (!IAPManager::getInstance()->isAdRemoved()) {
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
        }
        if (m_bFirstIn) {
            m_bFirstIn = false;
            
            Layer *pLockLayer = Layer::create();
            EventListenerTouchOneByOne *pTouch = EventListenerTouchOneByOne::create();
            pTouch->setSwallowTouches(true);
            pTouch->onTouchBegan = []( Touch *, Event *)->bool{
                return true;
            };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, pLockLayer);
            addChild(pLockLayer, 999999);
            
            pLockLayer->runAction(Sequence::create(DelayTime::create(2.2), CallFunc::create([this](){
                m_pPageView->scrollToPage(0);
                m_pViewPager->setVisible(true);
                this->openTouchEvent();
            }), RemoveSelf::create(), nullptr));
            
            Vector<Layout*> pages = m_pPageView->getPages();
            for (auto page : pages) {
                page->runAction(MoveBy::create(2, Vec2(Director::getInstance()->getWinSize().width * 3, 0.0)));
            }
        }
    }
}

AnimalChooseScene* AnimalChooseScene::create()
{
    auto pRet = new AnimalChooseScene();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool AnimalChooseScene::init()
{
    if ( !BaseScene::init("bg/select_bg.jpg") )
    {
        return false;
    }
    
    setName("AnimalChooseScene");
    
    auto *pUI = GUIReader::getInstance()->widgetFromJsonFile("cocoStudio/select.json");
    m_pUILayer->addChild(pUI);
    
    auto *pBack = dynamic_cast<Button *>(Helper::seekWidgetByName(pUI, "public_btn_back"));
    pBack->addTouchEventListener(CC_CALLBACK_2(AnimalChooseScene::onButtonCallback, this));
    
    m_pPageView = PageView::create();
    m_pPageView->setContentSize(Director::getInstance()->getWinSize());
    m_pPageView->addEventListener(CC_CALLBACK_2(AnimalChooseScene::onPageScrollDone, this));
    m_pGameLayer->addChild(m_pPageView, kGameZorder);
    
    for (int i=0; i<4; i++) {
        
        Layout *pLayout = Layout::create();
        ImageView *pImage = ImageView::create(g_ModelImageFilePath[i]);
        pImage->setTag(i);
        pImage->setPosition(XCVisibleRect::getPosition(320, 567));
        pLayout->addChild(pImage);
        m_pPageView->insertPage(pLayout, i);
        
        if (i >= 2 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockPets) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
            pImage->setUserData(new short(1));
            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
            pLock->setPosition(Vec2(pImage->getContentSize().width / 2 + 20, pImage->getContentSize().height / 2 - 100));
            pImage->addChild(pLock, 10);
        }
    }
    
    m_pViewPager = ViewPager::create("cocoStudio/public/public_point.png", "cocoStudio/public/public_point_h.png");
    m_pViewPager->setPosition(XCVisibleRect::getPosition(320, 140));
    m_pViewPager->setDotCount(4);
    m_pUILayer->addChild(m_pViewPager);
    m_pViewPager->setVisible(false);
    
    m_pPageView->scrollToPage(3);
    
    if (Director::getInstance()->getRunningScene()->getName() == "SPAScene") {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/menu.mp3", true);
    }
    
    m_pCustom = _eventDispatcher->addCustomEventListener("ShopBuySuccess", [this](EventCustom *){
        
        if (IAPManager::getInstance()->getItemIsBought(kIAPUnlockPets) || IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
            Vector<Layout*> pages = m_pPageView->getPages();
            for (auto page : pages) {
                ImageView *pTemp = dynamic_cast<ImageView *>(page->getChildren().at(0));
                if (pTemp) {
                    pTemp->removeAllChildren();
                    delete (short *)pTemp->getUserData();
                    pTemp->setUserData(nullptr);
                }
            }
        }
        
        if (IAPManager::getInstance()->isAdRemoved()) {
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        }

    });
    
    return true;
}

void AnimalChooseScene::onButtonCallback(cocos2d::Ref *aButton, Widget::TouchEventType aType)
{
    
    if (aType != Widget::TouchEventType::ENDED || m_bBackKeyEvent) {
        return;
    }
    
    m_bBackKeyEvent = true;
    Director::getInstance()->replaceScene(HomeScene::create());
}

void AnimalChooseScene::onPageScrollDone(cocos2d::Ref *aRef, PageView::EventType aType)
{
    m_pViewPager->setCurrentDot(m_pPageView->getCurPageIndex());
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/pets.mp3");
}

void AnimalChooseScene::onAdsShowDone()
{
    if (!IAPManager::getInstance()->isAdRemoved()) {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
    
    if (m_bFirstIn) {
        m_bFirstIn = false;
        
        Layer *pLockLayer = Layer::create();
        EventListenerTouchOneByOne *pTouch = EventListenerTouchOneByOne::create();
        pTouch->setSwallowTouches(true);
        pTouch->onTouchBegan = []( Touch *, Event *)->bool{
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, pLockLayer);
        addChild(pLockLayer, 999999);
        
        pLockLayer->runAction(Sequence::create(DelayTime::create(2.2), CallFunc::create([this](){
            m_pPageView->scrollToPage(0);
            m_pViewPager->setVisible(true);
            this->openTouchEvent();
        }), RemoveSelf::create(), nullptr));
        
        Vector<Layout*> pages = m_pPageView->getPages();
        for (auto page : pages) {
            page->runAction(MoveBy::create(2, Vec2(Director::getInstance()->getWinSize().width * 3, 0.0)));
        }
    }
}

bool AnimalChooseScene::onTouchBegan(cocos2d::Touch *aTouch, cocos2d::Event *)
{
    
    if (m_pUILayer->getChildByTag(4652)) {
        return false;
    }
    
    m_bDrag = false;
    m_pItem = nullptr;
    
    Vector<Layout*> pages = m_pPageView->getPages();
    for (auto page : pages) {
        ImageView *pTemp = dynamic_cast<ImageView *>(page->getChildren().at(0));
        if (pTemp->getBoundingBox().containsPoint(page->convertToNodeSpace(aTouch->getLocation()))) {
            m_pItem = pTemp;
            return true;
        }
    }
    
    return false;
}

void AnimalChooseScene::onTouchMoved(cocos2d::Touch *aTouch, cocos2d::Event *)
{
    if (aTouch->getLocation().distance(aTouch->getStartLocation()) > 20) {
        m_bDrag = true;
    }
}

void AnimalChooseScene::onTouchEnded(cocos2d::Touch *aTouch, cocos2d::Event *)
{
    if (!m_bDrag && m_pItem->getBoundingBox().containsPoint(m_pItem->getParent()->convertToNodeSpace(aTouch->getLocation()))) {
        
        if ((short *)m_pItem->getUserData()) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/popup.mp3");
            ShopLayer *pShopLayer = ShopLayer::create();
            pShopLayer->setTag(4652);
            m_pUILayer->addChild(pShopLayer, kPopZorder);
            return;
        }
        
        Layer *pLockLayer = Layer::create();
        EventListenerTouchOneByOne *pTouch = EventListenerTouchOneByOne::create();
        pTouch->setSwallowTouches(true);
        pTouch->onTouchBegan = []( Touch *, Event *)->bool{
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, pLockLayer);
        addChild(pLockLayer, 999999);
        
        m_pItem->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.2, 0.9)),  EaseBackOut::create(ScaleTo::create(0.2, 1.1)), CallFunc::create([this](){
            AnimalModel::Animal animal = (AnimalModel::Animal)(m_pItem->getTag() + 1);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/select_pet.mp3");
            Director::getInstance()->replaceScene(SPAScene::create(AnimalModel::create(animal)));
        }), nullptr));
    }
}

void AnimalChooseScene::onExit()
{
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    BaseScene::onExit();
}

AnimalChooseScene::~AnimalChooseScene()
{
    _eventDispatcher->removeEventListener(m_pCustom);
}