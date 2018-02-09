//
//  MoveForeverComponent.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/7.
//
//

#ifndef MoveForeverComponent_hpp
#define MoveForeverComponent_hpp

#include "LQComponent.h"
typedef function<void(Node*,Vec2,Vec2)> PosiontChangeCallBack;

class PositionChange : public ActionInterval{
    CC_SYNTHESIZE(PosiontChangeCallBack, _callBack, CallBack)
public:
    PositionChange();
    virtual ~PositionChange();
    static PositionChange* create(ActionInterval*);
       virtual ActionInterval* getInnerAction();
    virtual PositionChange* clone() const override;
    virtual PositionChange* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void stop() override;
    virtual void update(float time) override;
    bool initWithAction(ActionInterval *action);
protected:
        ActionInterval *_inner;
private:
    CC_DISALLOW_COPY_AND_ASSIGN(PositionChange);
};

class MoveForeverComponent : public LQComponent {
    CC_SYNTHESIZE(float, _speed, Speed);
    DECLARE_COMPONENT_DYNCLASS(MoveForeverComponent);
public:
    MoveForeverComponent();
    ~MoveForeverComponent();
    virtual bool init();
    virtual void onAdd();
    void setDirection(Vec2  _direction);
    
    void resumeAction();
    void pauseAction();
protected:
    bool _playing;
    ActionInterval *_moveAction;
    Vec2  _direction;
    Node* _temp1,*_temp2;
    Vec2 _moveDeta;
public:
   static const  Vec2 leftToRigth;
   static const  Vec2 rightToLeft;
   static const  Vec2 bottomToTop;
   static const  Vec2 TopToBottom;

};

#endif /* MoveForeverComponent_hpp */
