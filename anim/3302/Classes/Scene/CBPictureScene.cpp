//
//  CBPictureScene.cpp
//  ColorBook
//
//  Created by maxiang on 4/23/15.
//
//

#include "CBPictureScene.h"
#include "../Model/CBUtils.h"
#include "../Model/AppConfigs.h"
#include "../Model/CBPackManager.h"
#include "../Scene/CBPackScene.h"
#include "../Model/STVisibleRect.h"
#include "../Model/CBAppManager.h"

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

PictureScene::~PictureScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

Scene* PictureScene::createSceneWithPackIndex(const int packIndex)
{
    Scene *scene = Scene::create();
    PictureScene *layer = create();
    layer->setPackIndex(packIndex);
    scene->addChild(layer);
    return scene;
}

bool PictureScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {

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

//        //pen
//        Sprite *pen = Sprite::create(RES_PEN_BG);
//        namespaceST::STVisibleRect::setPosition(pen, Vec2(860, 630));
//        addChild(pen);
        
        //page controller
        _pageSize = Size(Utils::SCREEN_WIDTH*0.85, Utils::SCREEN_HEIGHT*0.70);
        
        _pageController = STPageController::create(this, _pageSize);
        CC_BREAK_IF(!_pageController);
        _pageController->ignoreAnchorPointForPosition(false);
        _pageController->setPosition(getContentSize().width/2, getContentSize().height/2 + 20);
        _pageController->setTag(TAG_PAGE_CONTROLLER);
        addChild(_pageController, 1);

        //page control
        PackInfo *info = PackManager::getInstance()->getPackInfo(_packIndex);
        int pictureCount = info->itemsCount;
        
        if (pictureCount > 6)
        {
            auto pageControl = PageControl::create(RES_CONTROL_NORMAL, RES_CONTROL_SELECT);
            pageControl->setPagesCount(getNumberOfCells());
            pageControl->ignoreAnchorPointForPosition(false);
            pageControl->setPosition(Vec2(getContentSize().width/2, _pageController->getBoundingBox().getMinY()));
            addChild(pageControl);
            
            _pageController->setPageControl(pageControl);
        }

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
            if (back->getBoundingBox().containsPoint(point))
            {
                back->setTexture(RES_BACK_BUTTON);
                backAction();
            }
        };
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        AppManager::getInstance()->setBannerAdVisible(true);
        
        log("MX: init picture scene!");
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void PictureScene::setPackIndex(const int index)
{
    _packIndex = index;
    _pageController->reloadData();
}

#pragma mark- Load picture texture async

void PictureScene::loadPictureTexture()
{
    
}

#pragma mark- TableViewDataSource

Size PictureScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return _pageSize;
}

TableViewCell* PictureScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    log("MX: init table!");

    auto page = (PicturePage *)table->dequeueCell();
    if (page)
    {
        page->reset();
    }
    else
    {
        page = PicturePage::create();
        page->setContentSize(_pageSize);
    }
    
    page->setPackIndex(_packIndex);
    page->reload((int)idx);
    
    return page;
}

ssize_t PictureScene::numberOfCellsInTableView(TableView *table)
{
    return getNumberOfCells();
}


int PictureScene::getNumberOfCells()
{
    PackInfo *info = PackManager::getInstance()->getPackInfo(_packIndex);
    return (int)ceilf(info->itemsCount / 6.0);
}

#pragma mark- Action methods 

void PictureScene::backAction()
{
    AppManager::getInstance()->playButtonEffect();

    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);

    auto scene = PackScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

CB_END_NAMESPACE