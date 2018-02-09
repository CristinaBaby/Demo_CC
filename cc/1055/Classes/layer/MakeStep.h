//
//  MakeStep.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/30.
//
//

#ifndef MakeStep_hpp
#define MakeStep_hpp


#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

//make步骤的模板
class MakeStep : public BaseLayer  {
public:
    MakeStep();
    ~MakeStep();
public:
    bool init();
    CREATE_FUNC(MakeStep);
    CREATE_SCENE(MakeStep);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
};

#endif /* MakeStep_hpp */
