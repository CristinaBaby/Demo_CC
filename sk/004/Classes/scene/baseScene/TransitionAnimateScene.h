//
//  TransitionAnimateScene.h
//  kfood007
//
//  Created by liji on 16/9/8.
//
//

#ifndef TransitionAnimateScene_h
#define TransitionAnimateScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

class TransitionAnimateScene : public ESMakeBaseScene
{
public:
    
    TransitionAnimateScene();
    ~TransitionAnimateScene();
    
    MY_SCENE(TransitionAnimateScene);
    CREATE_FUNC(TransitionAnimateScene);
    bool init();
    
protected:
    Sprite* _girl;
private:
    
    
};

#endif /* TransitionAnimateScene_h */
