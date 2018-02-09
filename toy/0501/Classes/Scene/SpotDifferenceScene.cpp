//
//  SpotDifferenceScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "SpotDifferenceScene.h"
#include "ChoosePageScene.h"
#include "HomeScene.h"
#include "AppManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#define TAG_RIGHT_MARK 123

SpotDifferenceScene::SpotDifferenceScene():
_gameLevel(-1),
_tmxMap(nullptr),
_leftPicture(nullptr),
_rightPicture(nullptr),
_usedTime(0),
_hintCooldownTime(0),
_pause(true)
{};

SpotDifferenceScene::~SpotDifferenceScene()
{
    unschedule(schedule_selector(SpotDifferenceScene::timerUpdate));
    CC_SAFE_RELEASE(_tmxMap);
    
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool SpotDifferenceScene::init()
{
    if (!BaseScene::init()) {
        return false;
    }
    
    //init ui
    _rootLayout = GUIReader::getInstance()->widgetFromJsonFile("spot(2).json");
    addChild(_rootLayout, 1);
    
    _timeLabel = (Text *)Helper::seekWidgetByName(_rootLayout, "time_label");
    _timeLabel->setString(AppManager::getInstance()->getFormatTimeString(0));
    
    _progressBar = (LoadingBar *)Helper::seekWidgetByName(_rootLayout, "progressbar");
    _progressImage1 = (ImageView *)Helper::seekWidgetByName(_rootLayout, "progress1");
    _progressImage2 = (ImageView *)Helper::seekWidgetByName(_rootLayout, "progress2");
    _progressImage3 = (ImageView *)Helper::seekWidgetByName(_rootLayout, "progress3");
    _progressImage4 = (ImageView *)Helper::seekWidgetByName(_rootLayout, "progress4");

    Utils::addEventToButton(_rootLayout, "back_button", Widget::TouchEventType::ENDED, [](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        auto scene = BaseScene::createScene<ChoosePageScene>();
        ChoosePageScene::needShowFullScreen = true;
        auto fadeTransition = TransitionFade::create(0.3, scene);
        auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
        layer->setupGameType(GameType::Spot);
        
        Director::getInstance()->replaceScene(fadeTransition);
    });
    
    _hintButton = Utils::addEventToButton(_rootLayout, "hint_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        if (_iconsVector.size() > 0)
        {
            _hintButton->loadTextureNormal("time2_disable.png");
            _hintButton->setTouchEnabled(false);

            auto sprite = Sprite::create("time2.png");
            auto progressTimer = ProgressTimer::create(sprite);
            progressTimer->runAction(Sequence::create(ProgressFromTo::create(SPOT_GAME_HINT_COOL_DOWN, 0, 100), CallFunc::create([=](){

                progressTimer->removeFromParent();
                _hintButton->loadTextureNormal("time2.png");
                _hintButton->setTouchEnabled(true);
            }), NULL));
            progressTimer->setPosition(_hintButton->getPosition());
            progressTimer->setType(cocos2d::ProgressTimer::Type::BAR);
            progressTimer->setMidpoint(Vec2(0.5, 0.0));
            progressTimer->setBarChangeRate(Vec2(0.0, 1.0));
            _gameContentLayer->addChild(progressTimer);
            
            correctTouch(_iconsVector.front());
        }
    });
    
    //init game content layer
    _gameContentLayer = Layer::create();
    addChild(_gameContentLayer, 2);

//    //schedule timer
    schedule(schedule_selector(SpotDifferenceScene::timerUpdate), 1);

    //set touch events
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = CC_CALLBACK_2(SpotDifferenceScene::onTouchBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(SpotDifferenceScene::onTouchMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(SpotDifferenceScene::onTouchEnded, this);
    touchEventListener->onTouchCancelled = CC_CALLBACK_2(SpotDifferenceScene::onTouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, _gameContentLayer);

    return true;
}

#pragma mark- init

void SpotDifferenceScene::startGameWithGameLevel(const int level)
{
    //reset time label
    _timeLabel->setString(AppManager::getInstance()->getFormatTimeString(0));

    //setup tmx map
    setupTMXMap(level);
    
    //setup picture
    setupPicture(level);
    
    //setup icons
    setupIcons(level);
    
    //begin update time label
    _pause = false;
}

#pragma mark- Inner Functions

void SpotDifferenceScene::showPurchaseDialog()
{
    auto maskLayer = Utils::createMaskLayer(200.0);
    addChild(maskLayer, 200);
    
    AppManager::getInstance()->setBannerAdVisible(true);
    
    auto dialogBg = Sprite::create("popup.png");
    dialogBg->setPosition(maskLayer->getContentSize()/2);
    dialogBg->setScale(0.0);
    maskLayer->addChild(dialogBg);
    dialogBg->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7), CallFunc::create([=](){
        
    }), NULL));
    
    auto text = Sprite::create("text(3).png");
    text->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    text->setPosition(Vec2(70.0, 187.0));
    dialogBg->addChild(text);
    
    auto yesButton = Button::create("buy.png");
    yesButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    yesButton->setPosition(Vec2(110.0, 52.0));
    yesButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            AppManager::getInstance()->setBannerAdVisible(false);
            
            dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                
                maskLayer->removeFromParent();
            }), NULL));
            
            _iap.purchase(IAP_UNLOCK_All);
        }
    });
    dialogBg->addChild(yesButton);
    
    auto noButton = Button::create("later.png");
    noButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    noButton->setPosition(Vec2(320.0, 52.0));
    noButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            AppManager::getInstance()->setBannerAdVisible(false);
            
            dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                
                maskLayer->removeFromParent();
            }), NULL));
        }
    });
    dialogBg->addChild(noButton);
}

void SpotDifferenceScene::showQuitGameLayer()
{
    _pause = true;
    auto quitLayer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 220.0));
    auto ui = GUIReader::getInstance()->widgetFromJsonFile("ui11_quitspot.json");
    auto closeButton = Helper::seekWidgetByName(ui, "Button_24");
    closeButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
        
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

            _pause = false;
            quitLayer->removeFromParent();
        }
    });
    
    auto yesButton = Helper::seekWidgetByName(ui, "Button_26");
    yesButton->addTouchEventListener([&](Ref*, ui::Widget::TouchEventType type){
        
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

            auto scene = BaseScene::createScene<ChoosePageScene>();
            auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
            
            layer->setupGameType(GameType::Spot);
            Director::getInstance()->replaceScene(scene);
        }
    });
    
    //text label
    auto label = dynamic_cast<Text*>(Helper::seekWidgetByName(ui, "Label_25"));
    label->setFontName("TitanOne-Regular.ttf");
    label->setFontSize(27);
    label->setString("You will lose your current\nprogress if you quit the game,\nare you sure?");
    label->setColor(Color3B::BLACK);
    label->setTextHorizontalAlignment(TextHAlignment::CENTER);
    label->setTextVerticalAlignment(TextVAlignment::CENTER);
    
    quitLayer->addChild(ui);
    this->addChild(quitLayer, 10);
}

#pragma mark- Action methods

void SpotDifferenceScene::setupTMXMap(const int level)
{
    CCASSERT(level >=0 && level <= MiniGameManager::getInstance()->getSpotDataVector().size(), "Invalid game level!");
    
    SpotGameData *gameData = MiniGameManager::getInstance()->getSpotDataVector().at(level);

    std::string mapPath = gameData->resPath + gameData->tmxFileName;
    auto map = experimental::TMXTiledMap::create(mapPath);
    if (!map)
    {
        CCLOG("Load map faild!");
        return;
    }
    
    CC_SAFE_RELEASE(_tmxMap);
    _tmxMap = map;
    _tmxMap->retain();
    
    _gameLevel = level;
}

void SpotDifferenceScene::setupPicture(const int level)
{
    SpotGameData *gameData = MiniGameManager::getInstance()->getSpotDataVector().at(level);

    /* load texture cache */
    std::string plist = gameData->resPath + gameData->textureFilePlist;
    std::string png = gameData->resPath + gameData->textureFileName;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, png);
    
    int newLevel = level + 1;
    std::string levelString = newLevel < 10? "0" + std::to_string(newLevel) : std::to_string(newLevel);
    std::string pictureName = "L" + levelString + "_bg.png";
    auto pictureFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pictureName);
    
    if (_leftPicture) {
        _leftPicture->removeFromParent();
    }
    _leftPicture = Sprite::create();
    _leftPicture->setSpriteFrame(pictureFrame);
    _leftPicture->setScale(SPOT_GAME_RES_SCALE);
    namespaceST::STVisibleRect::setPosition(_leftPicture, 260.0, 280.0);
    _gameContentLayer->addChild(_leftPicture);

    if (_rightPicture) {
        _rightPicture->removeFromParent();
    }
    _rightPicture = Sprite::create();
    _rightPicture->setSpriteFrame(pictureFrame);
    _rightPicture->setScale(SPOT_GAME_RES_SCALE);
    namespaceST::STVisibleRect::setPosition(_rightPicture, 702.0, 280.0);
    _gameContentLayer->addChild(_rightPicture);
}

void SpotDifferenceScene::setupIcons(const int level)
{
    int needFindCount = 5;
    _iconsVector.clear();
    
    std::vector<int> usedIndex;
    int randomIconIndex = 0;
    while (needFindCount != 0)
    {
        JUMP:while (1)
        {
            
            randomIconIndex = arc4random() % _tmxMap->getObjectGroups().at(0)->getObjects().size();
            for (auto iterator : usedIndex)
            {
                if (iterator == randomIconIndex) {
                    goto JUMP;
                }
            }
            
            usedIndex.push_back(randomIconIndex);
            break;
        }
        
        float x = getIconPositionX(randomIconIndex);
        float y = getIconPositionY(randomIconIndex);
        
        int newLevel = level + 1;
        std::string levelString = newLevel < 10? "0" + std::to_string(newLevel) : std::to_string(newLevel);
        std::string iconIndexString = randomIconIndex + 1 < 10? "0" + std::to_string(randomIconIndex + 1) : std::to_string(randomIconIndex + 1);

        std::string iconName = "L" + levelString + "_" + iconIndexString + ".png";
        auto iconFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(iconName);
        auto iconSprite = Sprite::create();
        iconSprite->setSpriteFrame(iconFrame);
        iconSprite->setPosition(x, y);
        iconSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        
        //DEBUG:
        //iconSprite->runAction(Sequence::create(ScaleTo::create(1, 3), ScaleTo::create(1, 1.0), NULL));
        
        int leftOrRight = arc4random() % 2;
        if (leftOrRight == 1)
        {
            _leftPicture->addChild(iconSprite);
        }
        else
        {
            _rightPicture->addChild(iconSprite);
        }
        
        _iconsVector.push_back(iconSprite);
        needFindCount--;
    }

}

float SpotDifferenceScene::getIconPositionX(const int iconIndex)
{
    auto groups = _tmxMap->getObjectGroups();
    auto objectGroup = groups.at(0);
    auto objects = objectGroup->getObjects();
    
    auto object = objects.at(iconIndex).asValueMap();
    return object["x"].asFloat();
}

float SpotDifferenceScene::getIconPositionY(const int iconIndex)
{
    auto groups = _tmxMap->getObjectGroups();
    auto objectGroup = groups.at(0);
    auto objects = objectGroup->getObjects();
    
    auto object = objects.at(iconIndex).asValueMap();
    return object["y"].asFloat();
}

#pragma mark- Timer 

void SpotDifferenceScene::timerUpdate(float time)
{
    if (_pause) {
        return;
    }
    
    _usedTime++;
    _timeLabel->setString(AppManager::getInstance()->getFormatTimeString(_usedTime));
}

#pragma mark- Game logic

void SpotDifferenceScene::touchInPicture(Sprite *picture, const Point& position)
{
    for (auto iconSprite : _iconsVector)
    {
        if (iconSprite->getBoundingBox().containsPoint(position))
        {
            AppManager::getInstance()->playEffect(SOUND_SPOT_CORRECT);

            //right
            correctTouch(iconSprite);
            return;
        }
    }
    
    //wrong
    AppManager::getInstance()->playEffect(SOUND_SPOT_INCORRECT);

    auto wrongSprite = Sprite::create("mistake.png");
    wrongSprite->setPosition(position);
    wrongSprite->runAction(Sequence::create(FadeOut::create(1.5), RemoveSelf::create(), NULL));
    picture->addChild(wrongSprite);
}

void SpotDifferenceScene::correctTouch(Sprite *icon)
{
    auto rightSprite = Sprite::create("right.png");
    rightSprite->setPosition(Vec2(icon->getPosition().x + icon->getContentSize().width/2, icon->getPosition().y + icon->getContentSize().height/2));
    _leftPicture->addChild(rightSprite);
    
    auto rightSprite2 = Sprite::create("right.png");
    rightSprite2->setPosition(rightSprite->getPosition());
    _rightPicture->addChild(rightSprite2);

    auto iterator = std::find(_iconsVector.begin(), _iconsVector.end(), icon);
    if (*iterator) {
        _iconsVector.erase(iterator);
    }

    //set progressbar
    _progressBar->runAction(Sequence::create(
                            Repeat::create(
                            Sequence::create(
                            DelayTime::create(1.0/60.0),
                            CallFunc::create([=](){
        
        int percent = _progressBar->getPercent() + 1;
        _progressBar->setPercent(percent);
    }), NULL), 20),
                            CallFunc::create([=](){
        
        if (_progressBar->getPercent() == 20)
            _progressImage1->loadTexture("shell_1.png");
        else if (_progressBar->getPercent() == 40)
            _progressImage2->loadTexture("shell_1.png");
        else if (_progressBar->getPercent() == 60)
            _progressImage3->loadTexture("shell_1.png");
        else if (_progressBar->getPercent() == 80)
            _progressImage4->loadTexture("shell_1.png");
        
    }), NULL));

    if (_iconsVector.size() == 0)
    {
        runAction(Sequence::create(DelayTime::create(1), CallFunc::create([&](){
            
            AppManager::getInstance()->playEffect(SOUND_OVER);

            this->finishGame();
        }), NULL));
    }
}

void SpotDifferenceScene::finishGame()
{
    //pause the timer
    _pause = true;
    
    //reset hooks
    _progressImage1->loadTexture("shell_3.png");
    _progressImage2->loadTexture("shell_3.png");
    _progressImage3->loadTexture("shell_3.png");
    _progressImage4->loadTexture("shell_3.png");
    
    _progressBar->setPercent(0);

    //hide game content layer
    _gameContentLayer->setVisible(false);

    auto layer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 200.0));
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
    addChild(layer, 110);
    
    AppManager::getInstance()->playEffect(SOUND_POPUB);

    auto ui = GUIReader::getInstance()->widgetFromJsonFile("finish_dialog.json");
    ui->setAnchorPoint(Point::ANCHOR_MIDDLE);
    ui->setPosition(getContentSize()/2);
    layer->addChild(ui, 1);
    
    AppManager::getInstance()->setBannerAdVisible(true);
    ui->setScale(0.1);
    ui->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7), CallFunc::create([=](){
        
        /* Ads Logic */
        AppManager::getInstance()->requestFullScreenAd();
    }), NULL));

    //set label font
    auto label = static_cast<Text*>(Helper::seekWidgetByName(ui, "time_label"));
    label->setString(AppManager::getInstance()->getFormatTimeString(_usedTime));
    
    auto closeButton = Utils::addEventToButton(ui, "close_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
        AppManager::getInstance()->setBannerAdVisible(false);

        _pause = false;
        _gameContentLayer->setVisible(true);
        
        layer->removeFromParent();
    });
    closeButton->setVisible(false);
    
    Utils::addEventToButton(ui, "next_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
        AppManager::getInstance()->setBannerAdVisible(false);

        int level = _gameLevel + 1;

        if (MiniGameManager::getInstance()->getSpotDataVector().size() <= level)
        {
            level = 0;//如果已经到达最后一张，循环回到第一张
        }
        
        if (level >= 15 && !AppManager::getInstance()->getHasUnlockAll())
        {
            showPurchaseDialog();
            return;
        }
        
        _pause = false;

        startGameWithGameLevel(level);
        
        layer->removeFromParent();
        _gameContentLayer->setVisible(true);
    });
    
    Utils::addEventToButton(ui, "replay_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
        AppManager::getInstance()->setBannerAdVisible(false);

        _pause = false;

        startGameWithGameLevel(_gameLevel);
        
        layer->removeFromParent();
        _gameContentLayer->setVisible(true);
    });
    
    Utils::addEventToButton(ui, "level_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->setBannerAdVisible(false);

        auto scene = BaseScene::createScene<ChoosePageScene>();
        auto fadeTransition = TransitionFade::create(0.3, scene);
        auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
        layer->setupGameType(GameType::Spot);

        Director::getInstance()->replaceScene(fadeTransition);
    });
}

#pragma mark- Touch event

bool SpotDifferenceScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    Point leftTouchPoint = _leftPicture->convertToNodeSpace(touch->getLocation());
    Rect leftBounding(0.0, 0.0, _leftPicture->getContentSize().width, _leftPicture->getContentSize().height);
    if (leftBounding.containsPoint(leftTouchPoint))
    {
        touchInPicture(_leftPicture, leftTouchPoint);
        return true;
    }
    
    Point rightTouchPoint = _rightPicture->convertToNodeSpace(touch->getLocation());
    Rect rightBounding(0, 0, _rightPicture->getContentSize().width, _rightPicture->getContentSize().height);
    if (rightBounding.containsPoint(rightTouchPoint))
    {
        touchInPicture(_rightPicture, rightTouchPoint);
        return true;
    }
    
    return false;
}


void SpotDifferenceScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}


void SpotDifferenceScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    
}


void SpotDifferenceScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}


#pragma mark- STIABDelegate

void SpotDifferenceScene::purchaseSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void SpotDifferenceScene::purchaseFailed(const char *pid, int errorCode)
{
    
}

void SpotDifferenceScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void SpotDifferenceScene::restoreFailed(const char* pid, int errorCode)
{
    
}


















