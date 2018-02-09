//
//  UndoManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "UndoManager.h"
#include "UICpp.h"
#include "FillNode.h"

//通常, 可以undo的次数是 RECORD_COUNT - 1
#define RECORD_COUNT          (11)

static UndoManager *_sharedUndo = nullptr;

UndoManager* UndoManager::getInstance()
{
    if (! _sharedUndo)
    {
        _sharedUndo = new UndoManager();
    }
    
    return _sharedUndo;
}

void UndoManager::destroyInstance()
{
    xScheduler->unscheduleAllForTarget(this);
    CC_SAFE_DELETE(_sharedUndo);
}

UndoManager::UndoManager()
{
    xScheduler->scheduleUpdate(this, 0, false);
}

void UndoManager::init(FillNode * fillSVGNode)
{
    _fillSVGNode = fillSVGNode;
    record();
}

void UndoManager::update(float dt)
{
    if(_acc > 0.4 && _painted)
    {
        _acc = 0;
        _painted = false;
        record();
    }
    else
    {
        _acc += dt;
    }
}

void UndoManager::undo()
{
    if (_recordCount <= 1) return;
    _recordCount--;
    
    _fillSVGNode->resetCanvas();
    
    cycleNum(RECORD_COUNT, false, &_recordIndex);
    string path = FileUtils::getInstance()->getWritablePath() + getSaveFileName();
    auto undo = Sprite::create(path);
    if (undo != nullptr) {
        undo->setPosition(_fillSVGNode->getContentSize()/2);
        _fillSVGNode->getCanvas()->begin();
        undo->visit();
        _fillSVGNode->getCanvas()->end();
        Director::getInstance()->getRenderer()->render();
    }
    Director::getInstance()->getTextureCache()->removeTextureForKey(path);
}

void UndoManager::painted()
{
    _painted = true;
}

void UndoManager::record()
{
    return;
    
    cycleNum(RECORD_COUNT, true, &_recordIndex);
    _fillSVGNode->getCanvas()->saveToFile(getSaveFileName());
    
    _recordCount++;
    if (_recordCount > RECORD_COUNT) {
        _recordCount = RECORD_COUNT;
    }
}

string UndoManager::getSaveFileName()
{
    string str = "undo_" + std::to_string(_recordIndex) + ".png";
    return std::move(str);
}