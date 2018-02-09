//
//  ComponentTouchMove.hpp
//
//  Created by tanshoumei on 1/21/16.
//
//

#ifndef ComponentTouchMove_hpp
#define ComponentTouchMove_hpp

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

typedef enum
{
    kComponentTouchEventBegan,
    kComponentTouchEventMoved,
    kComponentTouchEventEnded,
    kComponentTouchEventCancelled,
    kComponentTouchEventClicked,
//    kComponentTouchEventEndInSide,
//    kComponentTouchEventEndOutSide,
}
kComponentTouchEventType;

/// 触摸移动
class ComponentTouchMove: public ComponentBase
{
public:
    CREATE_FUNC(ComponentTouchMove);
    
public:
    //是否限制移动区域。默认是，且默认区域为屏幕内
    CC_SYNTHESIZE(bool, _bRestrictMoveArea, IsRestrictMoveArea);
    CC_SYNTHESIZE(bool, _needTouchTarget, NeedTouchTarget);
    
    //移动限制区域(当开启限制功能时有效), 默认为屏幕内
    CC_SYNTHESIZE(Rect, _moveArea, MoveArea);
    
    //是否会在move中发生位移。默认为yes. 如果移动到某个目标点到不需要在touch过程再Move,可设为false;
    CC_SYNTHESIZE(bool, _bCanMove, IsCanMove);
    
    /**
     *  当作touch算作move的最小距离，即如果move的距离小于此值，touch结束是算作点击事件的，否则算move事件。
     *  默认值为5
     */
    CC_SYNTHESIZE(float, _fMinDistanceForMoveStart, MinDistanceForMoveStart);
    
    float getMovedDistance();
    
    void setBackToOriginal(bool isBack = false, Vec2 oriPos = Vec2::ZERO);
    void back(std::function<void()>  backFunc = nullptr);
    
    //触摸相关回调
    std::function<void(Touch* touch)> touchBeganCall = nullptr;
    
    std::function<void(Touch* touch)> touchMovedCall = nullptr;
    
    std::function<void(Touch* touch)> touchEndedCall = nullptr;
    
    std::function<void(Touch* touch)> touchCancelledCall = nullptr;
    
    std::function<void(Touch* touch)> touchClickedCall = nullptr;
    
public:
    ComponentTouchMove();
    
    ~ComponentTouchMove();
    
    virtual void pause();
    
    virtual void resume();
    
    virtual void stop();
    
protected:
    virtual void onAdd();
    
    virtual void onRemove();
    
private:
    bool onTouchBegan(Touch* touch, Event* event);
    
    void onTouchMoved(Touch* touch, Event* event);
    
    void onTouchEnded(Touch* touch, Event* event);
    
    void onTouchCancelled(Touch* touch, Event* event);
    
    bool isTouchInRecursionChildren(Node* node, Touch* t);
private:
    EventListenerTouchOneByOne* _eventListenerTouchOneByOne = nullptr;
    
    float _fMoveDistance = 0;
    
    bool _paused = false;
    
    bool _isBack = false;

    Vec2 _originalPos = Vec2::ZERO;
};

#endif /* ComponentTouchMove_hpp */
