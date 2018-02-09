//
//  PaintMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "PaintMode.h"
#include "CBGameScene.h"

PaintMode::PaintMode(GameScene * layer) : BaseMode(layer)
{
    
}

PaintMode::~PaintMode()
{
    
}

void PaintMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    auto current = _gameLayer->convertToWorldSpace(touches[0]->getLocation());
    auto previous = _gameLayer->convertToWorldSpace(touches[0]->getPreviousLocation());

    _gameLayer->paint(current, previous);
}