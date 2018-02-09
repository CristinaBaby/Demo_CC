//
//  MakePowder.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang on 16/1/22.
//
//

#ifndef MakePowder_hpp
#define MakePowder_hpp

#include "BaseMakeLayer.h"
class MakePowder: public BaseMakeLayer {
public:
       CREATE_SECENE(MakePowder);
protected:
    bool init();
    virtual void onEnterTransitionDidFinish();
    virtual void fallEnd(Node*node,Component*, OperateListner* _lis);
    std::unordered_map<string,Vec2> endPos;
    std::unordered_map<string,float> fallTime;
    std::unordered_map<string,float> endRote;
    DragEndPosFall* createDraOperate(const string& nodeName);
     virtual void mixEnd(Node*node,Component*, OperateListner* _lis);
};

#endif /* MakePowder_hpp */
