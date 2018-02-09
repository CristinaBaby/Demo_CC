//
//  CBPackScene.cpp
//  ColorBook
//
//  Created by maxiang on 4/22/15.
//
//

#include "CBPackScene.h"
#include "CBHomeScene.h"
#include "../Model/AppConfigs.h"
#include "../Model/CBPackManager.h"
#include "../Widget/CBPackPage.h"
#include "../Model/STVisibleRect.h"
#include "../Model/CBAppManager.h"
#include "../Model/CBUtils.h"
#include "../ad_3.2/AdCross/AdLoadingLayerBase.h"
#include "../Layer/AdsLoadingLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
CB_BEGIN_NAMESPACE

#define TAG_PAGE_CONTROLLER  1234

#define RES_BG               "ui03_pack_bg.jpg"
#define RES_PEN_BG           "ui03_pen.png"

#define RES_CONTROL_NORMAL   "ui03_page_dot2.png"
#define RES_CONTROL_SELECT   "ui03_page_dot1.png"

#define RES_BACK_BUTTON      "ui07_public_back.png"
#define RES_BACK_BUTTON_H    "ui07_public_back_h.png"

PackScene::~PackScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool PackScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        
        /* ---------------------IMPORTANT---------------------
         refresh packs here
         */
        PackManager::getInstance()->refreshPacks();
        
        //back ground
        Sprite *background = Sprite::create(RES_BG);
        CC_BREAK_IF(!background);
        background->setPosition(Utils::getInstance()->getScreenCenter());
        addChild(background);
        
        //back
        Sprite *back = Sprite::create(RES_BACK_BUTTON);
        back->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        back->setPosition(Vec2(back->getContentSize().width/5, Utils::SCREEN_HEIGHT - back->getContentSize().width/5));
        addChild(back);
        
        //pen
        Sprite *pen = Sprite::create(RES_PEN_BG);
        namespaceST::STVisibleRect::setPosition(pen, Vec2(860, 630));
//        addChild(pen);
        
        //page controller
        _pageSize = Size(Utils::SCREEN_WIDTH, Utils::SCREEN_HEIGHT/2*1.2);
        
        _pageController = STPageController::create(this, _pageSize);
        CC_BREAK_IF(!_pageController);
        _pageController->ignoreAnchorPointForPosition(false);
        _pageController->setPosition(Vec2(Utils::SCREEN_WIDTH/2, Utils::SCREEN_HEIGHT/2 + 20));
        addChild(_pageController, 1);
        
        //page control
        _pageControl = PageControl::create(RES_CONTROL_NORMAL, RES_CONTROL_SELECT);
        _pageControl->setPagesCount(getNumberOfCells());
        _pageControl->ignoreAnchorPointForPosition(false);
        _pageControl->setPosition(Vec2(getContentSize().width/2, _pageController->getBoundingBox().getMinY()-20));
        _pageController->setTag(TAG_PAGE_CONTROLLER);
        addChild(_pageControl);
        
        _pageController->setPageControl(_pageControl);
        _pageController->reloadData();
        
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [back](Touch* touch, Event* event) {
            
            Point point = touch->getLocation();
            if (back->getBoundingBox().containsPoint(point))
            {
                back->setTexture(RES_BACK_BUTTON_H);
            }
            
            return true;
        };
        
        touchEventListener->onTouchMoved = [back](Touch* touch, Event* event) {
            
            Point point = touch->getLocation();
            
            back->setTexture(RES_BACK_BUTTON);
            if (back->getBoundingBox().containsPoint(point))
            {
                back->setTexture(RES_BACK_BUTTON_H);
            }
        };
        
        touchEventListener->onTouchEnded = [back, this](Touch* touch, Event* event) {
            
            Point point = touch->getLocation();
            back->setTexture(RES_BACK_BUTTON);
            if (back->getBoundingBox().containsPoint(point))
            {
                backAction();
            }
        };
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void PackScene::reloadPacks()
{
    _pageControl->setPagesCount(getNumberOfCells());
    _pageController->reloadData();
}

void PackScene::turnToPage(int pageIndex)
{
    _pageControl->setSelectPage(pageIndex);
    _pageController->turnToPage(pageIndex, false);
}

#pragma mark- TableViewDataSource

Size PackScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return _pageSize;
}

TableViewCell* PackScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto page = (PackPage *)table->dequeueCell();
    if (page)
    {
        page->reset();
    }
    else
    {
        page = PackPage::create();
        page->setContentSize(_pageSize);
    }
    
    page->reload((int)idx);
    
    return page;
}

ssize_t PackScene::numberOfCellsInTableView(TableView *table)
{
    return getNumberOfCells();
}

int PackScene::getNumberOfCells()
{
    bool hasSpecialPack = PackManager::getInstance()->checkHasSpecialPack();
    int allPacksCount = PackManager::getInstance()->getPackCount();
    
    int numberOfCells = ceilf((hasSpecialPack? allPacksCount + 1 : allPacksCount) / 3.0);
    
    return numberOfCells;
}

#pragma mark- Action methods

void PackScene::backAction()
{
    AppManager::getInstance()->playButtonEffect();
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
    auto scene = HomeScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void PackScene::onExit()
{
    Layer::onExit();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
}

void PackScene::onEnter()
{
    Layer::onEnter();
}

void PackScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    bool isTipPlay = UserDefault::getInstance()->getBoolForKey("tipPlay");
    bool isFirstIntoApp = UserDefault::getInstance()->getBoolForKey("firstIntoApp");
    if (isTipPlay && !isFirstIntoApp)
    {
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
        UserDefault::getInstance()->setBoolForKey("tipPlay", false);
        UserDefault::getInstance()->flush();
        AdLoadingLayerBase::loadingDoneCallback = [](){
            AppManager::getInstance()->showBannerAd();
        };

    }
}
CB_END_NAMESPACE
