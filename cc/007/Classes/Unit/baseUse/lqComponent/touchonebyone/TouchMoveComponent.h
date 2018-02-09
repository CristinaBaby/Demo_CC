//
//  TouchMoveComponent.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/5.
//
//

#ifndef TouchMoveComponent_hpp
#define TouchMoveComponent_hpp

#include "TouchComponent.h"
#include "LQMath.h"
/*
可以移动的Component
 */
enum class MoveComponentType{
    kTypeBack,
    kTypeInRect,
    kTypeNull
};
class TouchMoveComponent : public TouchComponent{
    DECLARE_COMPONENT_DYNCLASS(TouchMoveComponent);
    /**移动时是否reorder到最顶层,默认为true*/
    CC_SYNTHESIZE(bool, _brintTop, BrintTop);
    CC_SYNTHESIZE(MoveComponentType,_moveType,MoveType);
    /**检测点0~1,默认为Vec2(0.5,0.5)*/
    CC_SYNTHESIZE_RETAIN(LQPolygon*, _checkPolygon, CheckPolygon);
    CC_SYNTHESIZE(Vec2,_checkPoint,CheckPoint);
public:
    virtual void onAdd();
    TouchMoveComponent();
    virtual ~TouchMoveComponent();
    virtual void onResume();
    virtual void onRemove();
    virtual Vec2 getCheckPointInPoints(Vec2);
    void setOrnagnalPos(Vec2);
    const Vec2 getOrnagnalPos();
    bool checkInlimit();
    virtual void backToStart();
    void setTargets(const Vector<LQPolygon*>& rects);
    void removeTargets(size_t index);
    void removeTargets(LQPolygon*);
    void setTarget(LQPolygon*);
    int getOldZorder();
protected:
    virtual bool touchBeganLis(Touch*,Event*);
    virtual void touchMoveLis(Touch*,Event*);
    virtual void touchEndLis(Touch*,Event*);
    virtual void touchCanceLis(Touch*,Event*);
    virtual void moveBackSchedle(float);
    virtual bool isCotain(Touch* pTouch);
    int getTargetIndex(Vec2 pos);

protected:
    int *_oldZOrder;
    Vec2* oragnalPos;
    bool isTouch;
    float _backTime;
    Vec2 _backDistance;
    static const int backActionTag;
    Vector<LQPolygon*> _limits;
};

#endif /* TouchMoveComponent_hpp */
