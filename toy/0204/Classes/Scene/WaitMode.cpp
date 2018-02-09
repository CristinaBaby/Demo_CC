//
//  WaitMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "WaitMode.h"
#include "CBGameScene.h"

WaitMode::WaitMode(GameScene * layer) : BaseMode(layer)
{
    
}

WaitMode::~WaitMode()
{
    stop();
}

void WaitMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesBegan(touches, unused_event);
    
    //收到2根手指, 等待结束后, 进入zoom模式
    if(_gameLayer->getFingerCount() == 2)
    {
        //立刻到zoom, 只在zoom, 里面判断3只手指
        _gameLayer->switchMode(Mode::ZOOM);
    }
    //收到2根手指, 立刻进入move模式
    else if (_gameLayer->getFingerCount() > 2)
    {
        _gameLayer->switchMode(Mode::ZOOM);
        //_gameLayer->switchMode(Mode::MOVE);
    }
    
}

void WaitMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    //收集paints
    GameScene::Paint paint;
    paint.pos = _gameLayer->convertToWorldSpace(touches[0]->getLocation());
    paint.previousPos = _gameLayer->convertToWorldSpace(touches[0]->getPreviousLocation());
    _gameLayer->_paints.push_back(paint);
    
    //移动距离超过200, 结束waiting
    if (isOutOfRange(touches)) {
        timeout(0);
    }
}

void WaitMode::start()
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(WaitMode::timeout), this, 0, 0, 0.25, false);
}

void WaitMode::stop()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(WaitMode::timeout), this);
}

void WaitMode::timeout(float dt)
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(WaitMode::timeout), this);
    _gameLayer->paintPrevious();
    _gameLayer->switchMode(Mode::PAINT);
}

bool WaitMode::isOutOfRange(const vector<Touch*>& touches)
{
    for (auto touch : touches) {
        Vec2 current = _gameLayer->convertToWorldSpace(touch->getLocation());
        Vec2 start = _gameLayer->convertToWorldSpace(touch->getStartLocation());
        
        Vec2 distance = current - start;
        
        if (std::abs(distance.x) + std::abs(distance.y) > 40)
        {
            log("out of range %f", std::abs(distance.x) + std::abs(distance.y));
            return true;
        }
    }
    
    return false;
}