//
//  DragFall.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#ifndef DragFall_hpp
#define DragFall_hpp

#include "LQComponent.h"
#include "FrameStatus.h"
class DragFall : public LQComponent {
    DECLARE_COMPONENT_DYNCLASS(DragFall);
    CC_SYNTHESIZE_RETAIN(FrameStatus*, _frameStatus, FrameStatus);
public:
    DragFall();
    ~DragFall();
    virtual bool init();
    void setTargetRect(const Rect&);
    void setCheckPoint(const Vec2&);
    void setInbow(Node*,ActionInterval*);
    virtual void onAdd();
protected:
    Node* _inbow;
    ActionInterval* _inbowAction;
    float _startRote;
    static const std::string fallingAnimation;
protected:
    void fallEnd();
    void showPouring();
    void hidePouring();
    void pouring();
};

#endif /* DragFall_hpp */
