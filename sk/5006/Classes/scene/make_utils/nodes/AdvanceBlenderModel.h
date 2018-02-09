//
//  AdvanceBlenderModel.h
//  BHUG1062
//
//  Created by tanshoumei on 4/29/16.
//
//

#ifndef AdvanceBlenderModel_hpp
#define AdvanceBlenderModel_hpp

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

enum AdvanceBlenderStatus
{
    kAdvanceBlenderStatusIdle,
    kAdvanceBlenderStatusTurningOut, //在容器中转
    kAdvanceBlenderStatusTurningIn   //空转
};

class AdvanceBlenderModel: public Node
{
public:
    CREATE_FUNC(AdvanceBlenderModel);
    
    bool init();
    
    void setStatus(AdvanceBlenderStatus status);
    
    Sprite* getBody();
private:
    //在搅拌容器中转
    void runTurningAnimationIn();
    //空转
    void runTurningAniamtionOut();
    
    void stopAnimation();
    
private:
    AdvanceBlenderStatus _status;
    
    Sprite* _head = nullptr;
    
    Sprite* _body = nullptr;
    
    Sprite* _particleSprite = nullptr;
    
    std::string resRootPath = "donut/make/tool/blender/";
    
    const int _animationTag = INT_MAX;
};

#endif /* AdvanceMixerModel_hpp */
