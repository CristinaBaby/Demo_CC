//
//  ComponentMixer.hpp
//
//  Created by tanshoumei on 2/2/16.
//
//

#pragma once

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"
#include "cocos2d.h"

enum kMixerType
{
    //移动距离搅拌(默认类型)
    kMixerTypeDistance,
    //时间搅拌(可不移动)
    kMixerTypeTimed
};

class ComponentMixer: public ComponentBase
{
public:
    CREATE_FUNC(ComponentMixer);
    
    void start() override;
    
    void stop() override;
    
    void setEffectiveArea(const Rect& world);
public:
    /**
     *  搅拌回调
     *  @pram completePercent, 搅拌进度(0-100)
     */
    std::function<void(float completePercent)> componentMixCallback = nullptr;
    
    /**
     *  搅拌回调 松开点击时的回调
     */
    std::function<void()> _componentMixPaused = nullptr;

    
    /**
     *  搅拌类型(移动，或者时间)
     */
    CC_SYNTHESIZE(int, _mixerType, MixerType);
    
    /**
     *  搅拌完成需要的移动距离(或者时间)
     */
    CC_SYNTHESIZE(float, _mixDoneLength, MixDoneLength);
    
    //owner上以哪个位置作为判断是否相应, 默认为Vec2::ZERO
    CC_SYNTHESIZE(Vec2, _effectLocalPosition, EffectLocalPosition);
    
    /**
     *  完成度(0-100)
     */
    CC_SYNTHESIZE_READONLY(float, _mixPercent, MixPercent);
    
public:
    ComponentMixer();
    
    ~ComponentMixer();
    
private:
    //owner的zero点是否在有效区域内
    bool isOwnerInEffectArea();
    
    void computeCompleteAndCallback();
    
    void timerSchedule(float dt);
    
    bool isTouchInRecursionChildren(Node* node, Touch* t);
private:
    //
    EventListenerTouchOneByOne* _eventListenerTouchOneByOne = nullptr;
    //总移动距离
    float _moveDistanceAll = 0;
    //总使用时间
    float _touchTimeAll = 0;
    //当前touch的时间
    float _touchTimeCurrent = 0;
    
    Vec2 _previousPosition ;
    //搅拌有效区域，默认全屏
    Rect _effectEffectWorld;
};
