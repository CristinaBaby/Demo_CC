//
//  RecordItem.cpp
//  BedtimeStory
//
//  Created by maxiang on 10/22/15.
//
//

#include "RecordItem.hpp"
#include "AppConfigs.h"
#include "STVisibleRect.h"

USING_NS_CC;
using namespace ui;

#define DURATION 0.2

RecordItem::RecordItem():
_dateLabel(nullptr),
_timeLabel(nullptr),
_playButton(nullptr),
_bottomWhiteLine(nullptr),
_titleLabel(nullptr),
_colorLayer(nullptr),
_index(-1),
_isSelected(false),
_recordInfo(nullptr),
_currentState(State::Pause)
{
    
}

RecordItem::~RecordItem()
{
    
}

bool RecordItem::init()
{
    if (!Layer::init())
        return false;
    
    setContentSize(Size(278.0, 128.0));
 
    _colorLayer= LayerColor::create(Color4B(97.0, 172.0, 174.0, 255.0));
    _colorLayer->setContentSize(getContentSize());
    _colorLayer->setOpacity(0.0);
    addChild(_colorLayer);
    
    _titleLabel = Label::createWithTTF("New Record 1", "TitanOne-Regular.ttf", 22.0);
    _titleLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
    auto titlePosition = Vec2(100.0, 90.0);
    _titleLabel->setPosition(titlePosition);
    addChild(_titleLabel);
    
    _dateLabel = Label::createWithTTF("5/7/9", "TitanOne-Regular.ttf", 15.0);
    auto dateLabelPosition = Vec2(65.0, 40.0);
    _dateLabel->setPosition(dateLabelPosition);
    addChild(_dateLabel);
    
    _timeLabel = Label::createWithTTF("00:00:00", "TitanOne-Regular.ttf", 15.0);
    auto timeLabelPosition = Vec2(165.0, 40.0);
    _timeLabel->setPosition(timeLabelPosition);
    addChild(_timeLabel);
    
    _playButton = Button::create("play.png");
    _playButton->setVisible(false);
    auto playButtonPosition = Vec2(240.0, 40.0);
    _playButton->setPosition(playButtonPosition);
    _playButton->addTouchEventListener([&](Ref*, Widget::TouchEventType eventType){
        
        if (eventType == Widget::TouchEventType::ENDED)
        {
            if (_currentState == State::Play)
            {
                stopPlay();
            }
            else
            {
                startPlay();
            }
        }
    });
    addChild(_playButton);
    
    //top white line
    _bottomWhiteLine = LayerColor::create(Color4B(200.0, 200.0, 200.0, 100.0));
    _bottomWhiteLine->setContentSize(Size(getContentSize().width - 40.0, 1.0));
    auto bottomWhiteLinePosition = Vec2(20.0, 0.0);
    _bottomWhiteLine->setPosition(bottomWhiteLinePosition);
    addChild(_bottomWhiteLine);
    
    _setComponentToOriginal = [=](bool animation){
        
        if (animation)
        {
            _titleLabel->runAction(MoveTo::create(DURATION, titlePosition));
            _dateLabel->runAction(MoveTo::create(DURATION, dateLabelPosition));
            _timeLabel->runAction(MoveTo::create(DURATION, timeLabelPosition));
            _playButton->runAction(MoveTo::create(DURATION, playButtonPosition));
            _colorLayer->runAction(MoveTo::create(DURATION, Vec2::ZERO));
            _bottomWhiteLine->runAction(MoveTo::create(DURATION, bottomWhiteLinePosition));
        }
        else
        {
            _titleLabel->setPosition(titlePosition);
            _dateLabel->setPosition(dateLabelPosition);
            _timeLabel->setPosition(timeLabelPosition);
            _playButton->setPosition(playButtonPosition);
            _colorLayer->setPosition(Vec2::ZERO);
            _bottomWhiteLine->setPosition(bottomWhiteLinePosition);

        }
    };

    return true;
}

void RecordItem::stopPlay()
{
    if (_currentState == State::Pause)
        return;
    
    _currentState = State::Pause;
    _playButton->loadTextureNormal("play.png");
    if (_pauseCallback)
        _pauseCallback(this);
}

void RecordItem::startPlay()
{
    if (_currentState == State::Play)
        return;

    _currentState = State::Play;
    _playButton->loadTextureNormal("pause.png");
    if (_playCallback)
        _playCallback(this);
}

void RecordItem::changeToSeletedStyle()
{
    if (_isSelected)
        return;
    
    _isSelected = true;
    setColorLayerVisible(true, true);
    _playButton->setVisible(true);
}

void RecordItem::changeToUnselectedStyle()
{
    if (!_isSelected)
        return;
    
    _isSelected = false;
    setColorLayerVisible(false, true);
    _playButton->setVisible(false);
    
    if (_currentState == State::Play)
        stopPlay();
}

void RecordItem::setPlayCallback(const std::function<void(RecordItem *)>& callback)
{
    _playCallback = callback;
}

void RecordItem::setPauseCallback(const std::function<void(RecordItem *)>& callback)
{
    _pauseCallback = callback;
}

void RecordItem::setBottomWhiteLineVisible(bool visible)
{
    _bottomWhiteLine->setVisible(visible);
}

void RecordItem::setTitle(const std::string& title)
{
    _titleLabel->setString(title);
}

void RecordItem::setDate(const std::string& date)
{
    _dateLabel->setString(date);
}

void RecordItem::setTime(const std::string& time)
{
    _timeLabel->setString(time);
}

void RecordItem::setComponentOffset(const cocos2d::Vec2& offset,
                                    bool withWhiteLine,
                                    bool animation)
{
    if (animation)
    {
        _titleLabel->runAction(MoveBy::create(DURATION, offset));
        _dateLabel->runAction(MoveBy::create(DURATION, offset));
        _timeLabel->runAction(MoveBy::create(DURATION, offset));
        _playButton->runAction(MoveBy::create(DURATION, offset));
        _colorLayer->runAction(MoveBy::create(DURATION, offset));
        
        if (withWhiteLine)
            _bottomWhiteLine->runAction(MoveBy::create(DURATION, offset));
    }
    else
    {
        _titleLabel->setPosition(_timeLabel->getPosition() + offset);
        _dateLabel->setPosition(_dateLabel->getPosition() + offset);
        _timeLabel->setPosition(_timeLabel->getPosition() + offset);
        _playButton->setPosition(_playButton->getPosition() + offset);
        _colorLayer->setPosition(_colorLayer->getPosition() + offset);
        if (withWhiteLine)
            _bottomWhiteLine->setPosition(_bottomWhiteLine->getPosition() + offset);
    }
}

void RecordItem::backToOriginalPosition(bool animation)
{
    if (_setComponentToOriginal)
        _setComponentToOriginal(animation);
    
    auto postion = Vec2(0.0, DESIGN_HEIGHT - getContentSize().height * _index + namespaceST::STVisibleRect::getDesignOffset().y);
    if (animation)
        runAction(MoveTo::create(0.3, postion));
    else
        setPosition(postion);
}

void RecordItem::setComponentToOriginal()
{
    
}

void RecordItem::setColorLayerVisible(bool visible, bool animation)
{
    if (animation)
    {
        if (visible)
            _colorLayer->runAction(FadeIn::create(0.3));
        else
            _colorLayer->runAction(FadeOut::create(0.3));
    }
    else
    {
        if (visible)
            _colorLayer->setOpacity(255.0);
        else
            _colorLayer->setOpacity(0.0);
    }
}






