//
//  DragEndPosFall.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#ifndef DragEndPosFall_hpp
#define DragEndPosFall_hpp

#include "LQComponent.h"

/**拖动倒材料*/
class DragEndPosFall : public LQComponent {
      DECLARE_COMPONENT_DYNCLASS(DragEndPosFall);
public:
    DragEndPosFall();
	~DragEndPosFall();
    virtual bool init();

	void setTargetRect(const Rect&);
	void setCheckPoint(const Vec2&);
	void setInbow(Node*,ActionInterval*);
    void setEndRote(float);
    void setEndPos(const Vec2& _endRote);
protected:
    float _endRote;
    Vec2 endPos;
    Node* _inbow;
	ActionInterval* _inbowAction;
    float _startRote;
	void fallEnd();
    void showPouring();
    void hidePouring();
    void pouring();
};

#endif /* DragEndPosFall_hpp */
