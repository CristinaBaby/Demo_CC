//
//  DistanceToTarget.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/10.
//
//

#ifndef DistanceToTarget_hpp
#define DistanceToTarget_hpp

#include "LQComponent.h"
/**到目标区域的距离*/
class DistanceToTarget : public LQComponent {
    DECLARE_COMPONENT_DYNCLASS(DistanceToTarget);
public:
    DistanceToTarget();
    void setTargetRect(const Rect&);
    void setCheckPoint(const Vec2&);
    void startUpdate();
    void pauseUpdate();
    void setTargetNode(Node*);
    virtual void onRemove();
protected:
    virtual void updatePos(float _time);
    Vec2 getTargetPos();
private:
    Rect _endRect;
    float _startLen;
    Vec2 _checkPos,MidRect;
    float _judgeAngle;
    Node* targetNode;
};

#endif /* DistanceToTarget_hpp */
