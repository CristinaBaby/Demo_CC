//
//  PlayRecordItemBar.cpp
//  BedtimeStory
//
//  Created by maxiang on 10/22/15.
//
//

#include "PlayRecordItemBar.hpp"

USING_NS_CC;
using namespace ui;

PlayRecordItemBar::PlayRecordItemBar():
_currentTimeLabel(nullptr),
_endTimeLabel(nullptr),
_timeSlider(nullptr),
_refreshButton(nullptr),
_deleteButton(nullptr),
_colorLayer(nullptr),
_currentRecordItem(nullptr),
_toolBarLayer(nullptr),
_isShowToolBarLayer(false),
_showButton(nullptr),
_isAnimation(false)
{
    
}

PlayRecordItemBar::~PlayRecordItemBar()
{
    
}

bool PlayRecordItemBar::init()
{
    if (!Layer::init())
        return false;
    
    setContentSize(Size(278.0, 60.0));
    setCascadeOpacityEnabled(true);
    
    _colorLayer = LayerColor::create(Color4B(97.0, 172.0, 174.0, 255.0));
    _colorLayer->setContentSize(getContentSize());
    addChild(_colorLayer, 2);
    
    _currentTimeLabel = Label::createWithTTF("00:00:00", "TitanOne-Regular.ttf", 8.0);
    _currentTimeLabel->setPosition(Vec2(55.0, 40.0));
    addChild(_currentTimeLabel, 3);
    
    _endTimeLabel = Label::createWithTTF("00:00:00", "TitanOne-Regular.ttf", 8.0);
    _endTimeLabel->setPosition(Vec2(getContentSize().width - 30.0, 40.0));
    addChild(_endTimeLabel, 3);
    
    _timeSlider = Slider::create();
    _timeSlider->loadSlidBallTextureNormal("article_1.png");
    _timeSlider->loadSlidBallTexturePressed("article_1.png");
    _timeSlider->loadProgressBarTexture("article.png");
    _timeSlider->loadBarTexture("article.png");
    _timeSlider->setPosition(Vec2(getContentSize().width/2 + 15.0, 40.0));
    _timeSlider->setTouchEnabled(false);
    addChild(_timeSlider, 3);
    
    _toolBarLayer = LayerColor::create(Color4B(97.0, 172.0, 174.0, 255.0));
    _toolBarLayer->setContentSize(Size(125.0, getContentSize().height));
    _toolBarLayer->setVisible(false);
    addChild(_toolBarLayer, 1);
    
    _refreshButton = Button::create("refesh_2.png");
    _refreshButton->setPosition(Vec2(35.0, getContentSize().height/2));
    _refreshButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            if (_isAnimation || !_isShowToolBarLayer)
                return;
            
            if (_refreshCallback)
                _refreshCallback();
        }
    });
    _toolBarLayer->addChild(_refreshButton);
    
    _deleteButton = Button::create("delete_1.png");
    _deleteButton->setPosition(Vec2(90.0, getContentSize().height/2));
    _deleteButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            if (_isAnimation || !_isShowToolBarLayer)
                return;
            
            if (_deleteCallback)
                _deleteCallback();
        }
    });
    _toolBarLayer->addChild(_deleteButton);
    
    _showButton = Button::create("open.png");
    _showButton->setPosition(Vec2(17.0, getContentSize().height/2));
    _showButton->addTouchEventListener([=](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            if (_isAnimation)
                return;
            
            if (_isShowToolBarLayer)
            {
                hideToolBarLayer();
            }
            else
            {
                showToolBarLayer();
            }
        }
    });
    addChild(_showButton, 3);
    
    return true;
}

void PlayRecordItemBar::setCurrentTime(const std::string& time)
{
    _currentTimeLabel->setString(time);
}

void PlayRecordItemBar::setEndTime(const std::string& time)
{
    _endTimeLabel->setString(time);
}

void PlayRecordItemBar::setSliderPercent(const float percent)
{
    _timeSlider->setPercent(percent);
}

void PlayRecordItemBar::fadeIn(bool animation)
{
    if (animation)
        runAction(FadeIn::create(1));
    else
        setOpacity(255.0);
}

void PlayRecordItemBar::fadeOut(bool animation)
{
    if (animation)
        runAction(FadeOut::create(0.3));
    else
        setOpacity(0.0);
}

void PlayRecordItemBar::setDeleteCallback(const std::function<void()>& callback)
{
    _deleteCallback = callback;
}

void PlayRecordItemBar::setRefreshCallback(const std::function<void()>& callback)
{
    _refreshCallback = callback;
}

void PlayRecordItemBar::showToolBarLayer()
{
    _isShowToolBarLayer = true;
    _showButton->loadTextureNormal("closed.png");
    
    _isAnimation = true;
    _toolBarLayer->setVisible(true);
    _toolBarLayer->runAction(Sequence::create(MoveBy::create(0.3, Vec2(-_toolBarLayer->getContentSize().width, 0.0)), CallFunc::create([=](){
        
        _isAnimation = false;
    }), NULL));
}

void PlayRecordItemBar::hideToolBarLayer()
{
    _isShowToolBarLayer = false;
    _showButton->loadTextureNormal("open.png");
    
    _isAnimation = true;
    _toolBarLayer->runAction(Sequence::create(MoveBy::create(0.3, Vec2(_toolBarLayer->getContentSize().width, 0.0)), CallFunc::create([=](){
        
        _toolBarLayer->setVisible(false);
        _isAnimation = false;
    }), NULL));
}






