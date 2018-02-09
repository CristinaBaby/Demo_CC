//
//  CutDountComplent.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/14.
//
//

#ifndef CutDountComplent_hpp
#define CutDountComplent_hpp
#include "LQComponent.h"
#include "TouchMoveComponent.h"
#include "DistanceToTarget.h"
/**加在工具上*/
class CutDountComplent : public LQComponent {//
    DECLARE_COMPONENT_DYNCLASS(CutDountComplent);
public:
    CutDountComplent();
    ~CutDountComplent();
    virtual bool init();
    virtual void onResume();
public:
    void reset();
    void setDough(Node*);
    void setDountPath(const string&);
    void setMaskPath(const string&);
    void setTargetNum(int num);
    const  Vector<Sprite*>& getCutDounts();
protected:
    void touchOperate(Node* _node,Component* _move,OperateListner* _lis);
private:
    Node* dough;
    TouchMoveComponent* moveOperate;
    DistanceToTarget *distanceOperate;
    string dountPath,maskPath;
    int oragnalNum;
    int targetNum;
    Vector<Sprite*> cutDounts;
    bool isCutEnd;
     ClippingNode* containerNode;
};

#endif /* CutDountComplent_hpp */
