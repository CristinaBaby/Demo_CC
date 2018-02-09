//
//  MixToolOperate.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/21.
//
//

#ifndef MixToolOperate_hpp
#define MixToolOperate_hpp

#include "MoveHideShade.h"
#include "TimerChangeComponent.h"

class MixToolOperate : public LQComponent {
    DECLARE_COMPONENT_DYNCLASS(MixToolOperate);
private:
    void start();
    void stop();
public:
    MixToolOperate();
    ~MixToolOperate();
    virtual bool init();
public:
    void setEndRote(float);
    void setEndScale(float);
    void setMixLayer(Node*);
    
    void setTarget(const Rect& _rect);
   void setCheckPoint(Vec2);
    void setOperating(ActionInterval*);
protected:
   float _startRote,_endRote,_startScale,_endScale;
   Node* mixLayer;
   ActionInterval* operating;
    bool isPlaying;
public:
    static const string operatemixing;
   static  const string operatemixpause;
};
#endif /* MixToolOperate_hpp */
