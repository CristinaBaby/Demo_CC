//
//  MakeSignature.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/4.
//
//

#ifndef MakeSignature_hpp
#define MakeSignature_hpp
#include "StudioLayer.h"
#include "LQComponent.h"
#include "CaculateScribbleNode.h"
class MakeSignature : public LQComponent{
      DECLARE_COMPONENT_DYNCLASS(MakeSignature);
public:
    MakeSignature();
    virtual void onAdd();
     virtual void onRemove();
protected:
    bool init();
    virtual void touchEnd(Node*,Component*, OperateListner* _lis);
    StudioLayer* _operateLayer;
    bool onBack();
    Sprite* _prompt;
    void updatePos(float);
    CaculateScribbleNode * _drawNode;
    Vec2 _prePos;
    Node* _finger;
    static const string _eventName;
};

#endif /* MakeSignature_hpp */
