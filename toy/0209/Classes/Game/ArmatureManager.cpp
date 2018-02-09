//
//  ArmatureManager.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/8/24.
//
//

#include "ArmatureManager.h"
#include "UITools.h"

void ArmatureManager::setScale(float scale)
{
    _scale = scale;
}

void ArmatureManager::setAnchorPoint(const Vec2& point)
{
    _anchorPoint = point;
}

void ArmatureManager::setPosition(const Vec2& point)
{
    _position = point;
}

void ArmatureManager::setLocalZOrder(int localZOrder)
{
    _localZOrder = localZOrder;
}

Armature* ArmatureManager::add(const string& armatureFile, Node * parent)
{
    return add(armatureFile, parent, Vec2::ZERO);
}

Armature* ArmatureManager::add(const string& armatureFile, Node * parent, const Vec2& offset)
{
    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo(armatureFile);
    
    size_t pos = armatureFile.find(".ExportJson");
    auto armature = Armature::create(armatureFile.substr(0, pos));
    
    armature->setScale(_scale);
    armature->setAnchorPoint(_anchorPoint);
    armature->setPosition(_position + offset);
    parent->addChild(armature);
    armature->setLocalZOrder(_localZOrder);
    
    _model.push_back(armature);
    
    return armature;
}

void ArmatureManager::switchArmature()
{
    for (auto armature : _model) {
        armature->getAnimation()->stop();
        armature->setVisible(false);
    }
    
    if (_keepTalking)
    {
        auto armature = _model.at(4);
        armature->setVisible(true);
        armature->getAnimation()->playWithIndex(0);
        return;
    }
    
    int start, end;
    int *lastIndex = nullptr;
    if (_voiceStart != 0)
    {
        if (_noneVoicePlayCount < _nextNoneVoicePlayCount)
        {
            _noneVoicePlayCount++;
            start = 0;
            end = _voiceStart - 1;
            
            lastIndex = &_lastIndex;
        }
        else
        {
            _noneVoicePlayCount = 0;
            _nextNoneVoicePlayCount = quickRandom(1, 3);
            start = _voiceStart;
            
            if (rand() % 100 < 70) {    //有70%的概率都不唱歌
                start++;
            }
            
            end = (int)_model.size() - 1;
            _lastVoiceIndex = -1;
            lastIndex = &_lastVoiceIndex;
        }
    }
    else
    {
        start = 0;
        end = (int)_model.size() - 1;
        
        lastIndex = &_lastIndex;
    }
    
    int index = quickRandom(start, end);
    while(*lastIndex == index)
    {
        index = quickRandom(start, end);
    }
    *lastIndex = index;
    
    //为suggest增加的修正
    if(*lastIndex == 5)
    {
        _suggestCount++;
        if (_suggestCount > 2) {
            _suggestCount = 0;
            *lastIndex = 4;
        }
    }
    
    auto armature = _model.at(*lastIndex);
    armature->setVisible(true);
    armature->getAnimation()->playWithIndex(0);
}

const vector<cocostudio::Armature *> & ArmatureManager::getArmatures()
{
    return _model;
}

void ArmatureManager::setKeepTalking(bool keep)
{
    _keepTalking = keep;
}
