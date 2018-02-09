//
//  DragFallComponent.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#ifndef DragFallComponent_hpp
#define DragFallComponent_hpp

#include "LQComponent.h"

/**拖动倒材料*/
class DragFallComponent : public LQComponent {
      DECLARE_COMPONENT_DYNCLASS(DragFallComponent);
public:
    DragFallComponent();
	~DragFallComponent();
    virtual bool init();

	void setTargetRect(const Rect&);
	void setCheckPoint(const Vec2&);
	void setInbow(Node*,ActionInterval*);
    void setEndRote(float);
protected:
    bool isPouring;
    float _endRote;
    Node* _inbow,*innerNode;
	ActionInterval* _inbowAction;
	ActionInterval *_action;
    float _startRote;
    void pouring();
	void fallEnd();
    void showPouring();
    void hidePouring();
};

#endif /* DragFallComponent_hpp */
