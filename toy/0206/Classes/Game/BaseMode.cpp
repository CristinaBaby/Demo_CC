//
//  BaseMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "BaseMode.h"
#include "GameScene.h"

BaseMode::BaseMode(GameScene * layer)
{
    _gameScene = layer;
    _colouringLayer = layer->getColouringLayer();
    _original = _colouringLayer->getBoundingBox();
    
    _gameScene->setFingerCount(0);
    
    _whiteLayer = (LayerColor*)_colouringLayer->getChildByName("whiteLayer");
    _leftBottom = _colouringLayer->convertToWorldSpace(_whiteLayer->getPosition());
}

BaseMode::~BaseMode()
{
    
}

void BaseMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    _gameScene->setFingerCount(_gameScene->getFingerCount() + touches.size());
}

void BaseMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    
}

void BaseMode::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
    int fingerCount = _gameScene->getFingerCount() - touches.size();

    if(fingerCount <= 0)
    {
        fingerCount = 0;
        _gameScene->paintPrevious();
        _gameScene->switchMode(Mode::NONE);
    }
    
    _gameScene->setFingerCount(fingerCount);
}

void BaseMode::onTouchesCancelled(const vector<Touch*>& touches, Event *unused_event)
{
    int fingerCount = _gameScene->getFingerCount() - touches.size();
    
    if(fingerCount <= 0)
    {
        fingerCount = 0;
        _gameScene->paintPrevious();
        _gameScene->switchMode(Mode::NONE);
    }
    
    _gameScene->setFingerCount(fingerCount);
}

void BaseMode::start()
{
    
}

void BaseMode::stop()
{
    
}

void BaseMode::checkBoundingBox()
{
    Rect current = _colouringLayer->getBoundingBox();
    Vec2 offset = Vec2::ZERO;
    
    //附加的判定条件
    auto white = _colouringLayer->convertToWorldSpace(_whiteLayer->getPosition());
    if (white.x > _leftBottom.x) {
        offset.x = _leftBottom.x - white.x;
    }
    if (white.y > _leftBottom.y) {
        offset.y = _leftBottom.y - white.y;
    }
    
    if (current.getMinX() > _original.getMinX()) {
        offset.x =  _original.getMinX() - current.getMinX();
    }
    else if (current.getMaxX() < _original.getMaxX()) {
        offset.x =  _original.getMaxX() - current.getMaxX();
    }
    
    if (current.getMinY() > _original.getMinY()) {
        offset.y =  _original.getMinY() - current.getMinY();
    }
    else if (current.getMaxY() < _original.getMaxY()) {
        offset.y =  _original.getMaxY() - current.getMaxY();
    }
    
    _colouringLayer->setPosition(_colouringLayer->getPosition() + offset);
}