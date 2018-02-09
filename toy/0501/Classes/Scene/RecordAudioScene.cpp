//
//  RecordAudioScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "RecordAudioScene.h"

#include "HomeScene.h"
#include "ChoosePageScene.h"
#include "JigsawPuzzleScene.h"
#include "ColorBookScene.h"
#include "AudioRecorder.h"

USING_NS_CC;
using namespace extension;
using namespace ui;

RecordAudioScene::~RecordAudioScene()
{
    unschedule(schedule_selector(RecordAudioScene::timer));
    _playRecordItemBar->release();
}

RecordAudioScene::RecordAudioScene() :
_recordButton(nullptr),
_beginTimeLabel(nullptr),
_endTimeLabel(nullptr),
_timeSilder(nullptr),
_soundLevelIndicator1(nullptr),
_soundLevelIndicator2(nullptr),
_soundLevelIndicator3(nullptr),
_soundLevelIndicator4(nullptr),
_playRecordItemBar(nullptr),
_isShowPlayRecordBar(nullptr),
_leftStoryImage(nullptr),
_rightStoryImage(nullptr),
_storyIndex(0),
_bookSprite(nullptr),
_isAnimation(false),
_isRecording(false),
_recordTime(0.0),
_playTime(0.0),
_rightLayer(nullptr)

{};

bool RecordAudioScene::init()
{
    if (!BaseScene::init()) {
        return false;
    }
    
    //init ui
    auto background = Sprite::create("bg.jpg");
    background->setPosition(getContentSize()/2);
    addChild(background);
    
    //back button
    auto backButton = Button::create("back.png");
    namespaceST::STVisibleRect::setPosition(backButton, 10.0+backButton->getContentSize().width/2, DESIGN_HEIGHT-10.0-backButton->getContentSize().height/2, true, false, true, false);
    backButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            backAction();
        }
    });
    addChild(backButton, 20);
    
    //right container
    _rightLayer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 200.0));
    _rightLayer->setContentSize(Size(278.0, getContentSize().height));
    _rightLayer->setPosition(Vec2(getContentSize().width - 278.0, 0.0));
    addChild(_rightLayer, 5);

    //play record item bar
    _playRecordItemBar = PlayRecordItemBar::create();
    _playRecordItemBar->setPosition(Vec2(-100, -100));
    _playRecordItemBar->setDeleteCallback([&](){
        
        deleteRecordItemAction();
    });
    _playRecordItemBar->setRefreshCallback([&](){
        
        refreshRecordItemAction();
    });
    
    _playRecordItemBar->retain();
    _rightLayer->addChild(_playRecordItemBar);
    
    //bottom container
    auto bottomLayer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 160.0));
    bottomLayer->setContentSize(Size(getContentSize().width - 278.0, 114.0));
    bottomLayer->setPosition(Vec2(0.0, 0.0));
    addChild(bottomLayer, 1);
    
    //record button bg
    auto bg = Sprite::create("voice-7.png");
    bg->setPosition(Vec2(bottomLayer->getContentSize().width/2, bottomLayer->getContentSize().height/2 + 25.0));
    bottomLayer->addChild(bg);
    
    _recordButton = Button::create("voice-2.png", "voice-2.png");
    _recordButton->setPosition(Vec2(bottomLayer->getContentSize().width/2, bottomLayer->getContentSize().height/2 + 25.0));
    _recordButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            if (_isRecording)
                stopRecordAction();
            else
                beginRecordAction();
        }
    });
    bottomLayer->addChild(_recordButton);
    
    _beginTimeLabel = Text::create("00:00:00", "TitanOne-Regular.ttf", 15.0);
    _beginTimeLabel->setPosition(Vec2(78.0, 30.0));
    bottomLayer->addChild(_beginTimeLabel);
    
    _endTimeLabel = Text::create("00:05:00", "TitanOne-Regular.ttf", 15.0);
    _endTimeLabel->setPosition(Vec2(bottomLayer->getContentSize().width - 90.0, 30.0));
    bottomLayer->addChild(_endTimeLabel);
    
    _timeSilder = Slider::create();
    _timeSilder->loadSlidBallTextureNormal("article_4.png");
    _timeSilder->loadSlidBallTexturePressed("article_4.png");
    _timeSilder->loadBarTexture("article_2.png");
    _timeSilder->loadProgressBarTexture("article2.png");
    _timeSilder->setPosition(Vec2(bottomLayer->getContentSize().width/2, 30.0));
    _timeSilder->setTouchEnabled(false);
    bottomLayer->addChild(_timeSilder);

    _soundLevelIndicator1 = Sprite::create("voice-3.png");
    _soundLevelIndicator1->setVisible(false);
    _soundLevelIndicator1->setPosition(Vec2(bottomLayer->getContentSize().width/2, bottomLayer->getContentSize().height/2 + 25.0));
    bottomLayer->addChild(_soundLevelIndicator1);
    
    _soundLevelIndicator2 = Sprite::create("voice-4.png");
    _soundLevelIndicator2->setVisible(false);
    _soundLevelIndicator2->setPosition(Vec2(bottomLayer->getContentSize().width/2, bottomLayer->getContentSize().height/2 + 25.0));
    bottomLayer->addChild(_soundLevelIndicator2);
    
    _soundLevelIndicator3 = Sprite::create("voice-5.png");
    _soundLevelIndicator3->setVisible(false);
    _soundLevelIndicator3->setPosition(Vec2(bottomLayer->getContentSize().width/2, bottomLayer->getContentSize().height/2 + 25.0));
    bottomLayer->addChild(_soundLevelIndicator3);
    
    _soundLevelIndicator4 = Sprite::create("voice-6.png");
    _soundLevelIndicator4->setOpacity(0.0);
    _soundLevelIndicator4->setPosition(Vec2(bottomLayer->getContentSize().width/2, bottomLayer->getContentSize().height/2 + 25.0));
    bottomLayer->addChild(_soundLevelIndicator4);
   
    //book
    _bookSprite = Sprite::create("book.png");
    _bookSprite->setPosition(Vec2((getContentSize().width - _rightLayer->getContentSize().width)/2, (getContentSize().height - bottomLayer->getContentSize().height)/2 + bottomLayer->getContentSize().height));
    addChild(_bookSprite);
    
    auto leftButton = Button::create("page_03.png");
    leftButton->setPosition(Vec2(10.0, _bookSprite->getContentSize().height/2));
    leftButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            previousStoryAction();
        }
    });
    _bookSprite->addChild(leftButton);
    
    auto rightButton = Button::create("page_04.png");
    rightButton->setPosition(Vec2(_bookSprite->getContentSize().width - 10.0, _bookSprite->getContentSize().height/2));
    rightButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            nextStoryAction();
        }
    });
    _bookSprite->addChild(rightButton);
    
    //current story
    _leftStoryImage = Sprite::create();
    _leftStoryImage->setScale(STORY_IMAGE_SCALE);
    _leftStoryImage->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
    _leftStoryImage->setPosition(Vec2(276.0, 203.0));
    _bookSprite->addChild(_leftStoryImage, 2);
    
    _rightStoryImage = Sprite::create();
    _rightStoryImage->setScale(STORY_IMAGE_SCALE);
    _rightStoryImage->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    _rightStoryImage->setPosition(Vec2(276.0, 203.0));
    _bookSprite->addChild(_rightStoryImage, 2);
    
    //set touch events
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = [](Touch* touch, Event* pEvent){
        
        return true;
    };
    touchEventListener->onTouchEnded = [&](Touch* touch, Event* pEvent){
        
        if(_isAnimation || _isRecording)
            return;
  
        if (Utils::hitTest(_playRecordItemBar, _playRecordItemBar->convertToNodeSpace(touch->getLocation())))
        {
            if (!_playRecordItemBar->getIsAnimation())
                touchedRecordItemBar();
            
            return;
        }
   
#pragma mark- MARK1
        for (auto item : _recordItemVector)
        {
            if (item->getBoudingBox().containsPoint(item->convertToNodeSpace(touch->getLocation())))
            {
                touchedRecordItem(item);
                return;
            }
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    //timer
    schedule(schedule_selector(RecordAudioScene::timer), TIMER_UPDATE_INTERVAL);
    
    //
    refreshRecords();
    
    //stop music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

    //iap delegate
    _iap.setIABDelegate(this);
    
    return true;
}

#pragma mark- Start 

void RecordAudioScene::startWithStoryIndex(const int index)
{
    _storyIndex = index;
    
    auto dataVector = MiniGameManager::getInstance()->getRecordDataVector();
    if (_storyIndex < 0 || _storyIndex >= dataVector.size())
        return;

    auto data = dataVector.at(_storyIndex);
    std::string leftStoryImage = data->leftStoryImageFile;
    std::string rightStoryImage = data->rightStoryImageFile;
    
    //current story
    _leftStoryImage->setTexture(leftStoryImage);
    _rightStoryImage->setTexture(rightStoryImage);
    
    refreshRecords();
}

#pragma mark- Timer

void RecordAudioScene::timer(float time)
{
    if (_isRecording)
    {
        _recordTime += TIMER_UPDATE_INTERVAL;
        if (_recordTime >= RECORD_MAX_TIME)
        {
            stopRecordAction();
            return;
        }
        
        _timeSilder->setPercent(_recordTime/300.0 * 100);//five minutes
        _beginTimeLabel->setString(AppManager::getInstance()->getFormatTimeString((int)_recordTime));
        
        float power = AudioRecorder::getInstance()->getAveragePower();
        //0->max, -160->min
        log("Power: %f", power);
        
        if (power <= -70)
        {
            _soundLevelIndicator1->setVisible(false);
            _soundLevelIndicator2->setVisible(false);
            _soundLevelIndicator3->setVisible(false);
            
            _soundLevelIndicator4->setOpacity(0.0);
        }
        else if (power <= -35)
        {
            _soundLevelIndicator1->setVisible(true);
            _soundLevelIndicator2->setVisible(false);
            _soundLevelIndicator3->setVisible(false);
            
            _soundLevelIndicator4->setOpacity(50.0);
        }
        else if (power <= -15)
        {
            _soundLevelIndicator1->setVisible(true);
            _soundLevelIndicator2->setVisible(true);
            _soundLevelIndicator3->setVisible(false);
            
            _soundLevelIndicator4->setOpacity(150.0);
        }
        else if (power <= -5)
        {
            _soundLevelIndicator1->setVisible(true);
            _soundLevelIndicator2->setVisible(true);
            _soundLevelIndicator3->setVisible(true);
            
            _soundLevelIndicator4->setOpacity(255.0);
        }
    }
    
    auto item = _playRecordItemBar->getCurrentRecordItem();
    if (item && item->getCurrentState() == RecordItem::State::Play)
    {
        _playTime += TIMER_UPDATE_INTERVAL;
 
        auto recordInfo = item->getRecordInfo();
        
        int totalTime = recordInfo->recordTime;
        if (_playTime > totalTime)
        {
            _playTime = 0;
        }
        
        _playRecordItemBar->setCurrentTime(AppManager::getInstance()->getFormatTimeString((int)_playTime));
        _playRecordItemBar->setSliderPercent(_playTime/recordInfo->recordTime * 100);
    }
}

#pragma mark- Inner function

void RecordAudioScene::setBackCallback(const std::function<void()>& callback)
{
    _backCallback = callback;
}

std::string RecordAudioScene::getCurrentFormatTime(const std::string& seperator)
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    
    struct tm *tm = localtime(&tv.tv_sec);
    
    std::string time = std::to_string(tm->tm_mon + 1) + seperator + std::to_string(tm->tm_mday) + seperator + std::to_string(tm->tm_year + 1900);
    
    return time;
}

void RecordAudioScene::showPurchaseDialog()
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

#pragma mark- Record Item

void RecordAudioScene::touchedRecordItem(RecordItem *item)
{
    if (!item->getIsSelected())
    {
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        
        selectRecordItem(item);
    }
    else
    {
        if (item->getCurrentState() == RecordItem::State::Play)
        {
            item->stopPlay();
        }
        else
        {
            item->startPlay();
        }
    }
}

void RecordAudioScene::selectRecordItem(RecordItem *item)
{
    for (auto iterator : _recordItemVector)
    {
        iterator->changeToUnselectedStyle();
        iterator->backToOriginalPosition(true);
        iterator->setBoudingBox(Rect(0.0, 0.0, iterator->getContentSize().width, iterator->getContentSize().height));
    }
    
    Vec2 playRecordItemBarPosition;
    if (_recordItemVector.size() < 5)
    {
        auto iterator = std::find(_recordItemVector.begin(), _recordItemVector.end(), item);
        while ((++iterator) != _recordItemVector.end()) {
            
            (*iterator)->setComponentOffset(Vec2(0.0, -_playRecordItemBar->getContentSize().height), true);
            (*iterator)->setBoudingBox(Rect(0.0, -_playRecordItemBar->getContentSize().height, item->getContentSize().width, item->getContentSize().height));
        }
        
        playRecordItemBarPosition = Vec2(0.0, item->getPosition().y - _playRecordItemBar->getContentSize().height);
    }
    else
    {
        float interval = _playRecordItemBar->getContentSize().height / 4;

        int index = item->getIndex();
        int topItemCount = index - 1;
        int bototmItemCount = 5 - index;
        
        float topItemBeginOffset = topItemCount * interval;
        float bottomItemBeginOffset = (bototmItemCount - 1) * interval;

        item->setComponentOffset(Vec2(0.0, topItemBeginOffset), true);
        item->setBoudingBox(Rect(0.0, topItemBeginOffset, item->getContentSize().width, item->getContentSize().height));

        playRecordItemBarPosition = Vec2(0.0, item->getPosition().y - _playRecordItemBar->getContentSize().height + topItemBeginOffset);
        for (int i = index - 1; i >= 1; --i)
        {
            auto recordItem = _recordItemVector.at(i - 1);
            recordItem->setComponentOffset(Vec2(0.0, topItemBeginOffset), true);
            recordItem->setBoudingBox(Rect(0.0, topItemBeginOffset, recordItem->getContentSize().width, recordItem->getContentSize().height - interval));

            topItemBeginOffset -= interval;
        }
        
        for (int i = index + 1; i <= 5; ++i)
        {
            auto recordItem = _recordItemVector.at(i - 1);
            recordItem->setComponentOffset(Vec2(0.0, -bottomItemBeginOffset), true);
            recordItem->setBoudingBox(Rect(0.0, -bottomItemBeginOffset, recordItem->getContentSize().width, recordItem->getContentSize().height - interval));

            bottomItemBeginOffset -= interval;
        }
    }
    
    //
    auto currentItem = _playRecordItemBar->getCurrentRecordItem();
    if (currentItem)
    {
        currentItem->getRecordInfo()->isSelected = false;
    }
    item->changeToSeletedStyle();
    item->getRecordInfo()->isSelected = true;
 
    _playRecordItemBar->setCurrentRecordItem(item);
    _playRecordItemBar->setSliderPercent(0.0);
    _playRecordItemBar->setCurrentTime(AppManager::getInstance()->getFormatTimeString(0));
    _playRecordItemBar->setEndTime(AppManager::getInstance()->getFormatTimeString(item->getRecordInfo()->recordTime));
    
    //pause record, ready to play
    _playTime = 0;
    std::string path = FileUtils::getInstance()->getWritablePath() + item->getRecordInfo()->recordFilePath;
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(path.c_str(), true);
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    //show
    if (!_isShowPlayRecordBar)
    {
        _isShowPlayRecordBar = true;
        _playRecordItemBar->setPosition(playRecordItemBarPosition);
    }
    else
    {
        _isAnimation = true;
        _playRecordItemBar->runAction(Sequence::create(MoveTo::create(0.3, playRecordItemBarPosition), CallFunc::create([=](){
            
            _isAnimation = false;
        }), NULL));
    }
    
}

#pragma mark- Action methods

void RecordAudioScene::backAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
    AudioRecorder::getInstance()->stop();
    _isRecording = false;
    
    //如果正在录音，并且用户没有点击停止，删除此录音文件
    if (_isRecording)
    {
        std::string recordFile = FileUtils::getInstance()->getWritablePath() + _currentRecordFile;
        if (FileUtils::getInstance()->isFileExist(recordFile))
        {
            if (remove(recordFile.c_str()) != 0)
            {
                CCLOG("WARNING: remove file :%s faild!", recordFile.c_str());
            }
        }
    }

    //save record info
    MiniGameManager::getInstance()->saveRecordInfo();
    
    //stop music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

    if (_backCallback)
        _backCallback();
    else
    {
        //bgm
        AppManager::getInstance()->playBgm(SOUND_BGM_STORY);
        
        auto scene = BaseScene::createScene<ChoosePageScene>();
        auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
        layer->setupGameType(GameType::Record);
        
        auto fadeTransition = TransitionFade::create(0.3, scene);
        Director::getInstance()->replaceScene(fadeTransition);
    }
}

void RecordAudioScene::nextStoryAction()
{
    if (_isAnimation || _isRecording)
        return;
    
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto dataVector = MiniGameManager::getInstance()->getRecordDataVector();
    
    int index = _storyIndex + 1;
    //如果是最后一张，循环到第一张
    if (index >= dataVector.size())
    {
        index = 0;
    }

    if(index >= 14 && !AppManager::getInstance()->getHasUnlockAll())
    {
        showPurchaseDialog();
        return;
    }
    
    //stop music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    _storyIndex = index;
    auto data = dataVector.at(_storyIndex);
    std::string nextLeftStoryImage = data->leftStoryImageFile;
    std::string nextRightStoryImage = data->rightStoryImageFile;

    auto rightImage = Sprite::create(nextRightStoryImage);
    rightImage->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    rightImage->setPosition(_rightStoryImage->getPosition());
    rightImage->setScale(STORY_IMAGE_SCALE);
    _bookSprite->addChild(rightImage, 1);
    
    _isAnimation = true;
    OrbitCamera* orbit = OrbitCamera::create(BOOK_ANIMATION_DURATION, 0.3, 0.0, 0.0, -90.0, 0.0, -10.0);
    _rightStoryImage->runAction(Sequence::create(orbit, CallFunc::create([=](){
        
        _rightStoryImage->setAdditionalTransform(nullptr);
        _rightStoryImage->setTexture(nextRightStoryImage);
        rightImage->removeFromParent();
        
        auto leftImage = Sprite::create(nextLeftStoryImage);
        leftImage->setPosition(_leftStoryImage->getPosition());
        leftImage->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
        leftImage->setScale(STORY_IMAGE_SCALE);
        _bookSprite->addChild(leftImage, 3);
        
        OrbitCamera* orbit = OrbitCamera::create(BOOK_ANIMATION_DURATION, 0.3, 0.0, -90.0, 90.0, 0.0, 10.0);
        leftImage->runAction(Sequence::create(orbit, CallFunc::create([=](){
            
            _leftStoryImage->setTexture(nextLeftStoryImage);
            leftImage->removeFromParent();
            
            refreshRecords();
            
            _isAnimation = false;
   
        }), NULL));
        
    }), NULL));
}

void RecordAudioScene::previousStoryAction()
{
    if (_isAnimation || _isRecording)
        return;
    
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto dataVector = MiniGameManager::getInstance()->getRecordDataVector();

    //stop music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    int index = _storyIndex - 1;
    //如果是第一张，循环到最后一张
    if (index < 0)
    {
        index = (int)dataVector.size() - 1;
    }
    
    if(index >= 14 && !AppManager::getInstance()->getHasUnlockAll())
    {
        showPurchaseDialog();
        return;
    }

    _storyIndex = index;
    auto data = dataVector.at(_storyIndex);
    std::string previousLeftStoryImage = data->leftStoryImageFile;
    std::string previousRightStoryImage = data->rightStoryImageFile;
    
    auto leftImage = Sprite::create(previousLeftStoryImage);
    leftImage->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
    leftImage->setPosition(_leftStoryImage->getPosition());
    leftImage->setScale(STORY_IMAGE_SCALE);
    _bookSprite->addChild(leftImage, 1);
    
    _isAnimation = true;
    OrbitCamera* orbit = OrbitCamera::create(BOOK_ANIMATION_DURATION, 0.3, 0.0, 0.0, 90.0, 0.0, 10.0);
    _leftStoryImage->runAction(Sequence::create(orbit, CallFunc::create([=](){
        
        _leftStoryImage->setAdditionalTransform(nullptr);
        _leftStoryImage->setTexture(previousLeftStoryImage);
        leftImage->removeFromParent();
        
        auto rightImage = Sprite::create(previousRightStoryImage);
        rightImage->setPosition(_rightStoryImage->getPosition());
        rightImage->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        rightImage->setScale(STORY_IMAGE_SCALE);
        _bookSprite->addChild(rightImage, 3);
        
        OrbitCamera* orbit = OrbitCamera::create(BOOK_ANIMATION_DURATION, 0.3, 0.0, -90.0, 90.0, 0.0, -10.0);
        rightImage->runAction(Sequence::create(orbit, CallFunc::create([=](){
            
            _rightStoryImage->setTexture(previousRightStoryImage);
            rightImage->removeFromParent();
            
            refreshRecords();
            
            _isAnimation = false;
 
        }), NULL));
        
    }), NULL));
}

#pragma mark- Record

void RecordAudioScene::refreshRecords()
{
    //remove right layer all children, rebuild it
    _rightLayer->removeAllChildren();
    _recordItemVector.clear();
    _playRecordItemBar->setCurrentRecordItem(nullptr);
    
    //get records
    auto recordsVector = MiniGameManager::getInstance()->getRecordInfoVector(_storyIndex);
    if (!recordsVector || recordsVector->size() == 0)
        return;

    for (int i = 1; i <= recordsVector->size(); ++i)
    {
        auto recordInfo = recordsVector->at(i - 1);
        
        auto item = RecordItem::create();
        item->setOpacity(0.0);
        item->setIndex(i);
        item->setTime(AppManager::getInstance()->getFormatTimeString(recordInfo->recordTime));
        item->setDate(recordInfo->recordDate);
        item->setPosition(Vec2(0.0, DESIGN_HEIGHT - item->getContentSize().height * i + namespaceST::STVisibleRect::getDesignOffset().y));
        item->setRecordInfo(recordInfo);
        
        std::string title = "New Record " + std::to_string(i);
        item->setTitle(title);
        
        if (i == 5)
            item->setBottomWhiteLineVisible(false);
        
        item->setPlayCallback([&](RecordItem *item){
            
            playRecordItemAction(item);
        });
        
        item->setPauseCallback([&](RecordItem *item){
            
            pauseRecordItemAction(item);
        });
        
        _rightLayer->addChild(item);
        _recordItemVector.push_back(item);
    }

    _rightLayer->addChild(_playRecordItemBar);
    
    bool find = false;
    for (auto item : _recordItemVector)
    {
        if (item->getRecordInfo()->isSelected)
        {
            find = true;
            selectRecordItem(item);
            break;
        }
    }
    
    if (!find)
    {
        selectRecordItem(*(_recordItemVector.begin()));
    }
}

void RecordAudioScene::showAlert()
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
    
    auto text = Sprite::create("alert.png");
    text->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    text->setPosition(Vec2(100.0, 160.0));
    dialogBg->addChild(text);
    
    auto yesButton = Button::create("yes.png");
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
    
    auto noButton = Button::create("no.png");
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

void RecordAudioScene::stopRecordAction()
{
    AudioRecorder::getInstance()->stop();
    _isRecording = false;

    //reset indicator
    _soundLevelIndicator1->setVisible(false);
    _soundLevelIndicator2->setVisible(false);
    _soundLevelIndicator3->setVisible(false);
    _soundLevelIndicator4->setOpacity(0.0);
    
    _recordButton->setScale(0.5);
    _recordButton->loadTextures("voice-2.png", "voice-2.png");
    _recordButton->runAction(ScaleTo::create(0.2, 1.0));
    
    _beginTimeLabel->setString(AppManager::getInstance()->getFormatTimeString(0));
    
    _timeSilder->setPercent(0.0);
    int recordTime = _recordTime;
    _recordTime = 0.0;

    auto newRecordCall = [=]()
    {
        //
        auto recordInfo = new RecordInfo();
        recordInfo->index = _storyIndex;
        recordInfo->recordTime = recordTime;
        recordInfo->recordDate = getCurrentFormatTime("/");
        recordInfo->recordFilePath = _currentRecordFile;
        recordInfo->isSelected = false;
        
        MiniGameManager::getInstance()->addRecordInfo(_storyIndex, recordInfo);
        
        //refresh record
        refreshRecords();
        
        //if is first record, default select
        auto recordsVector = MiniGameManager::getInstance()->getRecordInfoVector(_storyIndex);
        if (recordsVector->size() == 1)
        {
            selectRecordItem(*(_recordItemVector.begin()));
        }
    };

    auto recordsVector = MiniGameManager::getInstance()->getRecordInfoVector(_storyIndex);
    if (!recordsVector || recordsVector->size() < 5)
    {
        newRecordCall();
    }
    else//>=5个，弹出提示框，提示用户是否覆盖最老的那个
    {
        //show alert
        auto maskLayer = Utils::createMaskLayer(200.0);
        addChild(maskLayer, 200);
        
        AppManager::getInstance()->setBannerAdVisible(true);
        
        auto dialogBg = Sprite::create("popup.png");
        dialogBg->setPosition(maskLayer->getContentSize()/2);
        dialogBg->setScale(0.0);
        maskLayer->addChild(dialogBg);
        dialogBg->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7), CallFunc::create([=](){
            
            
        }), NULL));
        
        auto text = Sprite::create("alert.png");
        text->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        text->setPosition(Vec2(100.0, 160.0));
        dialogBg->addChild(text);
        
        auto yesButton = Button::create("yes.png");
        yesButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        yesButton->setPosition(Vec2(110.0, 52.0));
        yesButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->setBannerAdVisible(false);
                
                dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                    
                    maskLayer->removeFromParent();
                }), NULL));
                
                newRecordCall();
            }
        });
        dialogBg->addChild(yesButton);
        
        auto noButton = Button::create("no.png");
        noButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        noButton->setPosition(Vec2(320.0, 52.0));
        noButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->setBannerAdVisible(false);
                
                std::string path = FileUtils::getInstance()->getWritablePath() + _currentRecordFile;
                if (FileUtils::getInstance()->isFileExist(path))
                {
                    if (remove(path.c_str()) != 0)
                    {
                        CCLOG("WARNING: remove file :%s faild!", path.c_str());
                    }
                }
                
                //refresh record
                refreshRecords();
                
                dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                    
                    maskLayer->removeFromParent();
                }), NULL));
            }
        });
        dialogBg->addChild(noButton);
    }
}

void RecordAudioScene::beginRecordAction()
{
    if (_isAnimation)
        return;
    
    auto recordsVector = MiniGameManager::getInstance()->getRecordInfoVector(_storyIndex);
    if (recordsVector && recordsVector->size() >= 5)
    {
        //如果>=5，提示用户删除一个再录音
        auto maskLayer = Utils::createMaskLayer(200.0);
        addChild(maskLayer, 200);
        
        AppManager::getInstance()->setBannerAdVisible(true);
        
        auto dialogBg = Sprite::create("popup.png");
        dialogBg->setPosition(maskLayer->getContentSize()/2);
        dialogBg->setScale(0.0);
        maskLayer->addChild(dialogBg);
        dialogBg->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7), CallFunc::create([=](){
            
            
        }), NULL));
        
        auto text = Sprite::create("delete5.png");
        text->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        text->setPosition(Vec2(63.0, 177.0));
        dialogBg->addChild(text);
        
        auto yesButton = Button::create("ok(2).png");
        yesButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        yesButton->setPosition(Vec2(223.0, 48.0));
        yesButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

                AppManager::getInstance()->setBannerAdVisible(false);
                
                dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                    
                    maskLayer->removeFromParent();
                }), NULL));
            }
        });
        dialogBg->addChild(yesButton);
    }
    else
    {
        auto item = _playRecordItemBar->getCurrentRecordItem();
        if (item && item->getCurrentState() == RecordItem::State::Play)
            return;
        
        AppManager::getInstance()->playEffect(SOUND_RECORD_START);
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        _currentRecordFile = std::to_string(tv.tv_sec) + ".caf";
        
        _recordButton->runAction(Sequence::create(ScaleTo::create(0.2, 0.7), CallFunc::create([&](){
            
            _recordButton->setScale(1.0);
            _recordButton->loadTextures("voice.png", "voice.png");
        }), NULL));
        
        std::string path = FileUtils::getInstance()->getWritablePath() + _currentRecordFile;
        bool rcode = AudioRecorder::getInstance()->setupNewRecord(path);
        if (!rcode)
        {
            //alert user some error occured
        }
        else
        {
            //防止将按钮音效加入到录音中
            runAction(Sequence::create(DelayTime::create(0.6), CallFunc::create([=](){
                
                _isRecording = true;
                AudioRecorder::getInstance()->record();
            }), NULL));
        }
    }
}

#pragma mark- Play & Pause

void RecordAudioScene::playRecordItemAction(RecordItem *item)
{
    if (_isAnimation)
        return;
 
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void RecordAudioScene::pauseRecordItemAction(RecordItem *item)
{
    if (_isAnimation)
        return;
    
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void RecordAudioScene::deleteRecordItemAction()
{
    if (_isAnimation)
        return;
    
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto item = _playRecordItemBar->getCurrentRecordItem();
    if (!item)
        return;
    
    //stop background music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

    //remove this item
    _isAnimation = true;
    auto iterator = std::find(_recordItemVector.begin(), _recordItemVector.end(), item);
    
    if (iterator != _recordItemVector.end())
    {
        while (++iterator != _recordItemVector.end()) {
            
            (*iterator)->runAction(MoveBy::create(0.3, Vec2(0.0, item->getContentSize().height)));
        }
    }
    
    //remove this item
    item->runAction(Sequence::create(
                     Spawn::create(
                      MoveTo::create(0.3, Vec2(item->getPosition().x + _playRecordItemBar->getContentSize().width, item->getPosition().y)),
                      FadeOut::create(0.3), NULL),
                     CallFunc::create([=](){
        
        _isAnimation = false;

        //delete record info and record file
        MiniGameManager::getInstance()->deleteRecordInfo(_storyIndex, item->getRecordInfo());
        
        refreshRecords();
    }), NULL));
}

void RecordAudioScene::refreshRecordItemAction()
{
    if (_isAnimation)
        return;
    
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto item = _playRecordItemBar->getCurrentRecordItem();
    if (!item)
        return;

    _playTime = 0;
    _playRecordItemBar->setCurrentTime(AppManager::getInstance()->getFormatTimeString(0));
    _playRecordItemBar->setSliderPercent(0.0);

    std::string path = FileUtils::getInstance()->getWritablePath() + item->getRecordInfo()->recordFilePath;
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(path.c_str(), true);
}

#pragma mark- Play Record Bar

void RecordAudioScene::touchedRecordItemBar()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    if (_playRecordItemBar->getIsShowToolBarLayer())
    {
        _playRecordItemBar->hideToolBarLayer();
    }
    else
    {
        _playRecordItemBar->showToolBarLayer();
    }
}

void RecordAudioScene::showPlayRecordBar(const cocos2d::Vec2& fromePosition)
{
    if (_isShowPlayRecordBar)
        return;
    
    _isShowPlayRecordBar = true;
    _playRecordItemBar->setOpacity(255.0);
    _playRecordItemBar->setPosition(fromePosition);
}

void RecordAudioScene::hidePlayRecordBar(const cocos2d::Vec2& fromePosition)
{
    if (!_isShowPlayRecordBar)
        return;
    
    _isShowPlayRecordBar = false;
    _isAnimation = true;
    _playRecordItemBar->runAction(Sequence::create(Spawn::create(MoveTo::create(0.3, Vec2(fromePosition.x + _playRecordItemBar->getContentSize().width, fromePosition.y)), FadeOut::create(0.3), NULL), CallFunc::create([&](){
        
        _isAnimation = false;
    }), NULL));
}

#pragma mark- STIABDelegate

void RecordAudioScene::purchaseSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void RecordAudioScene::purchaseFailed(const char *pid, int errorCode)
{
    
}

void RecordAudioScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void RecordAudioScene::restoreFailed(const char* pid, int errorCode)
{
    
}

