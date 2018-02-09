//
//  PhotoScene.cpp
//  PaintDress
//
//  Created by maxiang on 6/29/15.
//
//

#include "PhotoScene.h"
#include "../Model/STVisibleRect.h"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"
#include "../Widget/PhotoPage.h"
#include "HomeScene.h"
#include "../Model/AppManager.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace extension;

#define RES_COCOSTUDIO_JSON          "ui06_photo_album.json"
#define RES_BG                       "ui00_loading_bg.jpg"
#define RES_CONTROL_NORMAL           "ui06_dot2.png"
#define RES_CONTROL_SELECT           "ui06_dot1.png"

#define NAME_BUTTON_HOME             "ui08_btn_home"
#define NAME_BUTTON_DELETE           "ui06_delete"

PhotoScene::~PhotoScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool PhotoScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    
    do {
        //init bg
        auto bg = ImageView::create(RES_BG);
        bg->setPosition(getContentSize()/2);
        addChild(bg);
        
        _pageController = STPageController::create(this, getContentSize());
        CC_BREAK_IF(!_pageController);
        _pageController->ignoreAnchorPointForPosition(false);
        _pageController->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
        addChild(_pageController, 1);
        
        //page control
        _pageControl = PageControl::create(RES_CONTROL_NORMAL, RES_CONTROL_SELECT);
        int count = (int)ResManager::getInstance()->getSavedPhotos().size();
        if (count == 0) {
            _pageControl->setVisible(false);
        }
        _pageControl->setPagesCount(count);
        _pageControl->ignoreAnchorPointForPosition(false);
        namespaceST::STVisibleRect::setPosition(_pageControl, 320, 200);
        addChild(_pageControl, 1);
        
        _pageController->setPageControl(_pageControl);
        _pageController->reloadData();
        
        //init ui
        auto ui = GUIReader::getInstance()->widgetFromJsonFile(RES_COCOSTUDIO_JSON);
        addChild(ui, 2);

        auto homeButton = Helper::seekWidgetByName(ui, NAME_BUTTON_HOME);
        homeButton->setZOrder(100);
        homeButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                homeAction();
            }
        });
        
        _deleteButton = Helper::seekWidgetByName(ui, NAME_BUTTON_DELETE);
        if (count == 0) {
            _deleteButton->setVisible(false);
        }
        _deleteButton->setZOrder(100);
        _deleteButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                deleteAction();
            }
        });

        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        //show banner ads
        AppManager::getInstance()->showBannerAd();
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void PhotoScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    if (!AppManager::getInstance()->isFirstTimeLaunchApp() && _needShowFullScreenAds)
    {
        AppManager::getInstance()->requestFullScreenAd();
    }
}

#pragma mark- Action methods

void PhotoScene::homeAction()
{
    AppManager::getInstance()->playButtonEffect();

    auto scene = HomeScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void PhotoScene::deleteAction()
{
    AppManager::getInstance()->playButtonEffect();

    int index = _pageController->getFocusPageIndex();
    if (ResManager::getInstance()->getSavedPhotos().size() <= index) {
        return;
    }
    
    int count = (int)ResManager::getInstance()->getSavedPhotos().size();
    std::string savedPhoto = ResManager::getInstance()->getSavedPhotos().at(count - 1 -index).asString();
    ResManager::getInstance()->deletePhoto(savedPhoto);
    
    //delete from document
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    std::string path = FileUtils::getInstance()->getWritablePath() + savedPhoto;
    if (FileUtils::getInstance()->isFileExist(path))
    {
        remove(path.c_str());
    }
#endif
    
    if (count - 1 <= 0)
    {
        _pageControl->setVisible(false);
        _deleteButton->setVisible(false);
    }
    else
    {
        _pageControl->setVisible(true);
    }
    _pageControl->setPagesCount(count - 1);
    _pageController->reloadData();
}

#pragma mark- TableViewDataSource

Size PhotoScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return getContentSize();
}

TableViewCell* PhotoScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto page = (PhotoPage *)table->dequeueCell();
    if (page)
    {
        page->reset();
    }
    else
    {
        page = PhotoPage::create();
        page->setContentSize(getContentSize());
    }
    
    int count = (int)ResManager::getInstance()->getSavedPhotos().size();
    if (count > idx)
    {
        std::string savedPhoto = ResManager::getInstance()->getSavedPhotos().at(count - 1 - idx).asString();
        std::string path = FileUtils::getInstance()->getWritablePath() + savedPhoto;
        page->setImage(path);
    }

    return page;
}

ssize_t PhotoScene::numberOfCellsInTableView(TableView *table)
{
    return ResManager::getInstance()->getSavedPhotos().size();
}

