//
//  PackScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include <cocos2d.h>
#include "UITools.h"
#include "ArmatureManager.h"
#include "../Module/STInAppBilling.h"

class PictureLayer;


class  PackLayer : public Layer
{
public :
    CREATE_FUNC(PackLayer);
    
    virtual bool init();
    
    void onButton(Ref* pSender, Widget::TouchEventType type);
    
    Button* quickBtn(const string& name, const Widget::ccWidgetTouchCallback& callback);
    
    void playAnimation(const string& name);
    
    void onMovementEvent(cocostudio::Armature* armature, cocostudio::MovementEventType movementType);
    void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
    void onEndTurning(cocostudio::Armature *armature, cocostudio::MovementEventType movementType);
    void chooseLocalPack(const int index);
    void addParticle(float dt);
    
    void initAniamtion();
    
    void swichAnimation();
    
    void removeAarticleFromBook(Button* target);
    
    Vec2 getPriclePos();
    
    Button* getBook();
    
    void reload();
    
    void onEventIAP(EventCustom *);
    
    void onExit();
    
private:
    string::size_type _packIndex;
    
    Layout* _root;
    
    ListView * _list;
    
    Button* _back;
    
    Button* _restore;
    
    PictureLayer * _pictureLayer;
    
    float _oldInnerContainerX;
    
    int _tagVisleble;
    
    int _pageIndex;
    
    ParticleSystemQuad* _particle;
    
    cocostudio::Armature* _armatureBook;
    
    ArmatureManagerPtr _model;
    
    STInAppBilling _appBilling;
    
    bool _ispurchasing;
};