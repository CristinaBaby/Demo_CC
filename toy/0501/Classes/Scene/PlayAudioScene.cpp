//
//  PlayAudioScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "PlayAudioScene.h"

#include "HomeScene.h"
#include "ChoosePageScene.h"
#include "RecordAudioScene.h"
#include "MXPageView.hpp"

USING_NS_CC;
using namespace extension;
using namespace ui;

PlayAudioScene::~PlayAudioScene()
{
    
}

PlayAudioScene::PlayAudioScene():
_index(0),
_leftStoryImage(nullptr),
_rightStoryImage(nullptr),
_isAnimation(false),
_background(nullptr),
_readedStoryCount(1)
{};

bool PlayAudioScene::init()
{
    if (!BaseScene::init()) {
        return false;
    }
    
    //init ui
    _background = Sprite::create("bg (2).jpg");
    _background->setPosition(getContentSize()/2);
    addChild(_background);
    
    //back button
    auto backButton = Button::create("back.png");
    namespaceST::STVisibleRect::setPosition(backButton, 10.0+backButton->getContentSize().width/2, DESIGN_HEIGHT-10.0-backButton->getContentSize().height/2, true, false, true, false);
    backButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            backAction();
        }
    });
    addChild(backButton, 10);
    
    //left button
    auto leftButton = Button::create("page_01 (2).png");
    leftButton->setPosition(Vec2(173.0, 389.0));
    leftButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            leftAction();
        }
    });
    _background->addChild(leftButton);
    
    //right button
    auto rightButton = Button::create("page_02 (2).png");
    rightButton->setPosition(Vec2(980.0, 389.0));
    rightButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            rightAction();
        }
    });
    _background->addChild(rightButton);
    
    //play button
    auto playButton = Button::create("play_1.png");
    playButton->setPosition(Vec2(960.0, 143.0));
    playButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            playAction();
        }
    });
    _background->addChild(playButton);
    
    //
    _leftStoryImage = Sprite::create();
    _leftStoryImage->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
    _leftStoryImage->setPosition(Vec2(582.0, 385.0));
    _background->addChild(_leftStoryImage, 2);
    
    _rightStoryImage = Sprite::create();
    _rightStoryImage->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    _rightStoryImage->setPosition(Vec2(582.0, 385.0));
    _background->addChild(_rightStoryImage, 2);
    
    //stop music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    //iap delegate
    _iap.setIABDelegate(this);
    
    return true;
}

#pragma mark- Play

void PlayAudioScene::startPlayWithStoryIndex(const int index)
{
    auto dataVector = MiniGameManager::getInstance()->getRecordDataVector();
    if (dataVector.size() > index)
    {
        auto data = dataVector.at(index);
        
        _rightStoryImage->setTexture(data->rightStoryImageFile);
        _leftStoryImage->setTexture(data->leftStoryImageFile);
        
        _index = index;
    }
}

#pragma makr- Action methods

void PlayAudioScene::backAction()
{
    //bgm
    AppManager::getInstance()->playBgm(SOUND_BGM_STORY);
    
    auto scene = BaseScene::createScene<ChoosePageScene>();
    auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
    layer->setupGameType(GameType::Story);
    
    auto fadeTransition = TransitionFade::create(0.3, scene);
    Director::getInstance()->replaceScene(fadeTransition);
}

void PlayAudioScene::rightAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto dataVector = MiniGameManager::getInstance()->getRecordDataVector();
    
    int index = _index + 1;
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

    _index = index;
    auto data = dataVector.at(_index);
    std::string nextLeftStoryImage = data->leftStoryImageFile;
    std::string nextRightStoryImage = data->rightStoryImageFile;
    
    auto rightImage = Sprite::create(nextRightStoryImage);
    rightImage->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    rightImage->setPosition(_rightStoryImage->getPosition());
    _background->addChild(rightImage, 1);
    
    _isAnimation = true;
    OrbitCamera* orbit = OrbitCamera::create(BOOK_ANIMATION_DURATION, 0.3, 0.0, 0.0, -90.0, 0.0, -10.0);
    _rightStoryImage->runAction(Sequence::create(orbit, CallFunc::create([=](){
        
        _rightStoryImage->setAdditionalTransform(nullptr);
        _rightStoryImage->setTexture(nextRightStoryImage);
        rightImage->removeFromParent();
        
        auto leftImage = Sprite::create(nextLeftStoryImage);
        leftImage->setPosition(_leftStoryImage->getPosition());
        leftImage->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
        _background->addChild(leftImage, 3);
        
        OrbitCamera* orbit = OrbitCamera::create(BOOK_ANIMATION_DURATION, 0.3, 0.0, -90.0, 90.0, 0.0, 10.0);
        leftImage->runAction(Sequence::create(orbit, CallFunc::create([=](){
            
            _leftStoryImage->setTexture(nextLeftStoryImage);
            leftImage->removeFromParent();
            _isAnimation = false;
            
            //用户每看5页，显示一次全屏广告
            _readedStoryCount++;
            if (_readedStoryCount % 5 == 0)
            {
                AppManager::getInstance()->requestFullScreenAd();
            }
        }), NULL));
        
    }), NULL));

}

void PlayAudioScene::leftAction()
{
    auto dataVector = MiniGameManager::getInstance()->getRecordDataVector();

    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    //stop music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

    int index = _index - 1;
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
    
    _index = index;
    auto data = dataVector.at(_index);
    std::string previousLeftStoryImage = data->leftStoryImageFile;
    std::string previousRightStoryImage = data->rightStoryImageFile;
    
    auto leftImage = Sprite::create(previousLeftStoryImage);
    leftImage->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
    leftImage->setPosition(_leftStoryImage->getPosition());
    _background->addChild(leftImage, 1);
    
    _isAnimation = true;
    OrbitCamera* orbit = OrbitCamera::create(BOOK_ANIMATION_DURATION, 0.3, 0.0, 0.0, 90.0, 0.0, 10.0);
    _leftStoryImage->runAction(Sequence::create(orbit, CallFunc::create([=](){
        
        _leftStoryImage->setAdditionalTransform(nullptr);
        _leftStoryImage->setTexture(previousLeftStoryImage);
        leftImage->removeFromParent();
        
        auto rightImage = Sprite::create(previousRightStoryImage);
        rightImage->setPosition(_rightStoryImage->getPosition());
        rightImage->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        _background->addChild(rightImage, 3);
        
        OrbitCamera* orbit = OrbitCamera::create(BOOK_ANIMATION_DURATION, 0.3, 0.0, -90.0, 90.0, 0.0, -10.0);
        rightImage->runAction(Sequence::create(orbit, CallFunc::create([=](){
            
            _rightStoryImage->setTexture(previousRightStoryImage);
            rightImage->removeFromParent();
            _isAnimation = false;
            
            //用户每看5页，显示一次全屏广告
            _readedStoryCount++;
            if (_readedStoryCount % 5 == 0)
            {
                AppManager::getInstance()->requestFullScreenAd();
            }
        }), NULL));
        
    }), NULL));
}

void PlayAudioScene::playAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto recordVector = MiniGameManager::getInstance()->getRecordInfoVector(_index);
    if (!recordVector || recordVector->size() == 0)
    {
        //no record found, show parent gate
        showParentGate();
    }
    else
    {
        for (auto record : *recordVector)
        {
            if (record->isSelected)
            {
                std::string recordPath = FileUtils::getInstance()->getWritablePath() + record->recordFilePath;
                CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(recordPath.c_str(), true);
                break;
            }
        }
    }
}

void PlayAudioScene::showPurchaseDialog()
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
            
            _iap.purchase(IAP_UNLOCK_All);
            
            dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                
                maskLayer->removeFromParent();
            }), NULL));
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

#pragma mark- Parent Gate

void PlayAudioScene::showParentGate()
{
    auto layer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 200.0));
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
    addChild(layer, 110);
    
    auto ui = GUIReader::getInstance()->widgetFromJsonFile("parentgate_dialog(1).json");
    ui->setAnchorPoint(Point::ANCHOR_MIDDLE);
    ui->setPosition(getContentSize()/2);
    layer->addChild(ui, 1);
    
    ui->setScale(0.1);
    ui->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7));
    
    int number1 = arc4random() % 10 + 1;
    int number2 = arc4random() % 10 + 1;
    
    auto answerFiled = (TextField *)Helper::seekWidgetByName(ui, "answer_textfiled");
    auto numberLabel = (Text *)Helper::seekWidgetByName(ui, "number_label");
    
    std::string numberString = std::to_string(number1) + "+" + std::to_string(number2) + "=";
    numberLabel->setString(numberString);
    
    Utils::addEventToButton(ui, "close_button", Widget::TouchEventType::ENDED, [=](){
        
        layer->removeFromParent();
    });
    
    Utils::addEventToButton(ui, "send_button", Widget::TouchEventType::ENDED, [=](){
        
        std::string answerString = answerFiled->getStringValue();
        
        if (!AppManager::getInstance()->isDigit(answerString))
            return;
        
        int answer = std::stoi(answerString);
        if (answer == number1 + number2)
        {
            auto scene = BaseScene::createScene<RecordAudioScene>();
            auto fadeTransition = TransitionFade::create(0.3, scene);
            auto layer = static_cast<RecordAudioScene*>(scene->getChildren().at(0));
            
            int index = _index;
            layer->setBackCallback([=](){
                
                auto scene = BaseScene::createScene<PlayAudioScene>();
                auto fadeTransition = TransitionFade::create(0.3, scene);
                auto layer = static_cast<PlayAudioScene*>(scene->getChildren().at(0));
                
                layer->startPlayWithStoryIndex(index);
                Director::getInstance()->replaceScene(fadeTransition);
            });
            layer->startWithStoryIndex(_index);
            Director::getInstance()->replaceScene(fadeTransition);
        }
    });
}


#pragma mark- STIABDelegate

void PlayAudioScene::purchaseSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void PlayAudioScene::purchaseFailed(const char *pid, int errorCode)
{
    
}

void PlayAudioScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void PlayAudioScene::restoreFailed(const char* pid, int errorCode)
{
    
}



