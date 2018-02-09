//
//  ChoosePageScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "ChoosePageScene.h"
#include "MiniGameSelectScene.h"
#include "SpotDifferenceScene.h"
#include "JigsawPuzzleScene.h"
#include "ColorBookScene.h"
#include "HomeScene.h"
#include "PlayAudioScene.h"
#include "RecordAudioScene.h"

USING_NS_CC;
using namespace extension;
using namespace ui;

bool ChoosePageScene::needShowCross = false;
bool ChoosePageScene::needShowFullScreen = false;

ChoosePageScene::ChoosePageScene():
_gameType(GameType::None),
_pageView(nullptr),
_pageControl(nullptr),
_copyPage(nullptr)
{};

ChoosePageScene::~ChoosePageScene()
{
    CC_SAFE_RELEASE(_copyPage);
}

bool ChoosePageScene::init()
{
    if (!BaseScene::init()) {
        return false;
    }
    
    //init ui
    _rootLayout = GUIReader::getInstance()->widgetFromJsonFile("level_select(2).json");
    addChild(_rootLayout, 1);
    
    //page
    _copyPage = (Layout *)Helper::seekWidgetByName(_rootLayout, "level_page");
    _copyPage->retain();
    _copyPage->removeFromParent();
    
    //home
    Utils::addEventToButton(_rootLayout, "back_button", Widget::TouchEventType::ENDED, [&](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        if (_gameType == GameType::Story ||
            _gameType == GameType::Record)
        {
            auto scene = BaseScene::createScene<HomeScene>();
            auto fadeTransition = TransitionFade::create(0.3, scene);
            
            Director::getInstance()->replaceScene(fadeTransition);
        }
        else
        {
            auto scene = BaseScene::createScene<MiniGameSelectScene>();
            auto fadeTransition = TransitionFade::create(0.3, scene);
            
            Director::getInstance()->replaceScene(fadeTransition);
        }
    });
    
    runAction(CallFunc::create([=](){
        
        if (needShowCross)
        {
            AppManager::getInstance()->requestCrossPromoAd();
        }
        
        if (needShowFullScreen)
        {
            AppManager::getInstance()->requestFullScreenAd();
        }
        
        needShowCross = false;
        needShowFullScreen = false;
    }));
    
    //iap delegate
    _iap.setIABDelegate(this);
    
    return true;
}

void ChoosePageScene::onExit()
{
    AppManager::getInstance()->setBannerAdVisible(false);
    Layer::onExit();
}

void ChoosePageScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    AppManager::getInstance()->setBannerAdVisible(true);
}

void ChoosePageScene::setupGameType(GameType type)
{
    CCASSERT(type != GameType::None, "Game type not support!");
    CCASSERT(_gameType == GameType::None, "Called too many times!");
    
    _gameType = type;

    _pageView = MXPageView::create();
    _pageView->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _pageView->setPosition(getContentSize()/2);
    _pageView->setContentSize(_copyPage->getContentSize());
    _pageView->addEventListener([&](Ref*, PageView::EventType type){
        
        _pageControl->setSelectPage((int)_pageView->getCurPageIndex());
    });
    addChild(_pageView, 2);
    
    int dataCount = 0;
    if (_gameType == GameType::ColorBook)
        dataCount = (int)MiniGameManager::getInstance()->getColorBookDataVector().size();
    else if (_gameType == GameType::Spot)
        dataCount = (int)MiniGameManager::getInstance()->getSpotDataVector().size();
    else if (_gameType == GameType::Jigsaw)
        dataCount = (int)MiniGameManager::getInstance()->getJigsawDataVector().size();
    else if (_gameType == GameType::Story ||
             _gameType == GameType::Record)
        dataCount = (int)MiniGameManager::getInstance()->getRecordDataVector().size();
    
    int pageCount = ceil(dataCount / 8.0);
    if (pageCount > 0)
    {
        auto setupPageButtons = [=](int pageIndex, Layout *page){
            
            for (int i = 1; i <= 8; ++i)
            {
                std::string buttonName = "button" + std::to_string(i);
                auto button = Helper::seekWidgetByName(page, buttonName);
                
                int index = (pageIndex - 1) * 8 + i;
                if (index > dataCount)
                {
                    button->removeFromParent();
                    continue;
                }
                
                auto resLogo = (ImageView *)Helper::seekWidgetByName(button, "res_logo");
                resLogo->setPosition(Vec2(resLogo->getPosition().x, resLogo->getPosition().y + 2.0));
                
                auto lock = Helper::seekWidgetByName(button, "lock");
                lock->setVisible(false);
                if (index > 15 && !AppManager::getInstance()->getHasUnlockAll())
                {
                    _locksVector.push_back(lock);
                    lock->setVisible(true);
                }
  
                std::string textureFile = std::to_string(index) + ".png";
                
                if (_gameType == GameType::ColorBook)
                {
                    //check if has histroy, begin with index 0
                    std::string historyFile = FileUtils::getInstance()->getWritablePath() + std::to_string(index - 1) + ".png";
                    if (FileUtils::getInstance()->isFileExist(historyFile))
                    {
                        Director::getInstance()->getTextureCache()->removeTextureForKey(historyFile);
                        textureFile = historyFile;
                    }
                }
      
                resLogo->loadTexture(textureFile);
                Utils::addEventToButton(page, buttonName, Widget::TouchEventType::ENDED, [=](){
                    
                    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

                    chooseLevelAction(index-1);
                });
            }
        };
        
        setupPageButtons(1, _copyPage);
        _pageView->addPage(_copyPage);
        
        for (int i = 2; i <= pageCount; ++i)
        {
            auto newPage = (Layout *)_copyPage->clone();
            
            setupPageButtons(i, newPage);
            _pageView->addPage(newPage);
        }
    }
    
    //page control
    _pageControl = PageControl::create("page_2.png", "page_1.png");
    _pageControl->setPagesCount(pageCount);
    _pageControl->setAnchorPoint(Point::ANCHOR_MIDDLE);
    namespaceST::STVisibleRect::setPosition(_pageControl, 480.0, 110.0);
    addChild(_pageControl, 3);
}

#pragma mark- Action methods

void ChoosePageScene::chooseLevelAction(const int index)
{
    CCLOG("choose leve: %d", index);
    if (index >= 15 && !AppManager::getInstance()->getHasUnlockAll())//begin at 0
    {
        _iap.purchase(IAP_UNLOCK_All);
        return;
    }
    
    if (_gameType == GameType::ColorBook)
        colorbookGame(index);
    else if (_gameType == GameType::Spot)
        spotGame(index);
    else if (_gameType == GameType::Jigsaw)
        jigsawGame(index);
    else if (_gameType == GameType::Story)
        storyGame(index);
    else if (_gameType == GameType::Record)
        recordGame(index);
}

void ChoosePageScene::spotGame(const int level)
{
    auto scene = BaseScene::createScene<SpotDifferenceScene>();
    auto fadeTransition = TransitionFade::create(0.3, scene);
    auto layer = dynamic_cast<SpotDifferenceScene*>(scene->getChildren().at(0));
    
    layer->startGameWithGameLevel(level);
    Director::getInstance()->replaceScene(fadeTransition);
}

void ChoosePageScene::jigsawGame(const int level)
{
    auto scene = BaseScene::createScene<JigsawPuzzleScene>();
    auto fadeTransition = TransitionFade::create(0.3, scene);
    auto layer = dynamic_cast<JigsawPuzzleScene*>(scene->getChildren().at(0));
    
    layer->startGameWithGameLevel(level);
    Director::getInstance()->replaceScene(fadeTransition);
}

void ChoosePageScene::colorbookGame(const int level)
{
    auto scene = BaseScene::createScene<ColorBookScene>();
    auto fadeTransition = TransitionFade::create(0.3, scene);
    auto layer = dynamic_cast<ColorBookScene*>(scene->getChildren().at(0));
    
    layer->startGameWithGameLevel(level);
    Director::getInstance()->replaceScene(fadeTransition);
}

void ChoosePageScene::storyGame(const int level)
{
    auto scene = BaseScene::createScene<PlayAudioScene>();
    auto fadeTransition = TransitionFade::create(0.3, scene);
    auto layer = dynamic_cast<PlayAudioScene*>(scene->getChildren().at(0));
    
    layer->startPlayWithStoryIndex(level);
    Director::getInstance()->replaceScene(fadeTransition);
}

void ChoosePageScene::recordGame(const int level)
{
    auto scene = BaseScene::createScene<RecordAudioScene>();
    auto fadeTransition = TransitionFade::create(0.3, scene);
    auto layer = dynamic_cast<RecordAudioScene*>(scene->getChildren().at(0));
    
    layer->startWithStoryIndex(level);
    Director::getInstance()->replaceScene(fadeTransition);
}

#pragma mark- STIABDelegate

void ChoosePageScene::purchaseSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
        
        for (auto lock : _locksVector)
        {
            lock->setVisible(false);
        }
    }
}

void ChoosePageScene::purchaseFailed(const char *pid, int errorCode)
{
    
}

void ChoosePageScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
        
        for (auto lock : _locksVector)
        {
            lock->setVisible(false);
        }
    }
}

void ChoosePageScene::restoreFailed(const char* pid, int errorCode)
{
    
}

