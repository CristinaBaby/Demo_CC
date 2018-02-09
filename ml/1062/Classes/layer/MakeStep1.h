//
//  MakeStep1.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/21.
//
//

#ifndef MakeStep1_hpp
#define MakeStep1_hpp

#include "BaseMakeLayer.h"
class MakeStep1: public BaseMakeLayer{
public:
    CREATE_SECENE(MakeStep1);
protected:
    bool init();
    virtual void onEnterTransitionDidFinish();
    void showFood();
    void mixEnd(Node*node,Component*, OperateListner* _lis);
    void dipping(Node*node,Component*, OperateListner* _lis);
    void dipEnd(Node*,Component*);
    int dipCount;
    virtual void fallEnd(Node*node,Component*, OperateListner* _lis);
    void endAllStep();
    int moveCount;
    int inBowlCount;
};


#endif /* MakeStep1_hpp */
