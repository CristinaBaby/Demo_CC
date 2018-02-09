//
//  NoneMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "NoneMode.h"
#include "CBGameScene.h"

NoneMode::NoneMode(GameScene * layer) : BaseMode(layer)
{
}

void NoneMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesBegan(touches, unused_event);
    
    //如果收到一个began消息, 立刻进入wait mode
    if(_gameLayer->getFingerCount() == 1)
    {
        _gameLayer->changeColor();
        
        //收集paint
        _gameLayer->_paints.clear();
        
        GameScene::Paint paint;
        paint.pos = _gameLayer->convertToWorldSpace(touches[0]->getLocation());
        _gameLayer->_paints.push_back(paint);
        
        _gameLayer->switchMode(Mode::WAIT);
    }
    else if (_gameLayer->getFingerCount() == 2)
    {
        _gameLayer->switchMode(Mode::ZOOM);
    }
    else
    {
        _gameLayer->switchMode(Mode::ZOOM);
        //_gameLayer->switchMode(Mode::MOVE);
    }
}