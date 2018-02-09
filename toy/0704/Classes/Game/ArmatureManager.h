//
//  ArmatureManager.h
//  ColorBook0818
//
//  Created by huangwen on 15/8/24.
//
//

#ifndef __ColorBook0818__ArmatureManager__
#define __ColorBook0818__ArmatureManager__

#include "cocos2d.h"
#include "UIHeader.h"
#include "cocostudio/CocoStudio.h"


class ArmatureManager{
public:
    void setScale(float scale);
    void setAnchorPoint(const Vec2& point);
    void setPosition(const Vec2& point);
    void setLocalZOrder(int localZOrder);
    void setKeepTalking(bool keep);
    
    Armature* add(const string& armatureFile, Node * parent);
    Armature* add(const string& armatureFile, Node * parent, const Vec2& offset);
    void switchArmature();
    
    const vector<cocostudio::Armature *> & getArmatures();
    void setVoiceStart(int voiceStart) { _voiceStart = voiceStart; };
    
private:
    float _scale = 1.0;
    Vec2 _anchorPoint = {0.5, 0.5};
    Vec2 _position = Vec2::ZERO;
    int _localZOrder = 0;

    vector<cocostudio::Armature *> _model;
    
    int _lastIndex = -1;
    int _voiceStart = 0;
    int _noneVoicePlayCount = 0;
    bool _keepTalking = false;
    int _lastVoiceIndex = 0;
    int _nextNoneVoicePlayCount = 4;
    int _suggestCount = 0;
};

typedef shared_ptr<ArmatureManager> ArmatureManagerPtr;

#endif
