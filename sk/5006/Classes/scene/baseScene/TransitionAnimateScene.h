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
class BowlNode;
class TransitionAnimateScene : public ESMakeBaseScene
{
public:
    
    TransitionAnimateScene();
    ~TransitionAnimateScene();
    
    MY_SCENE(TransitionAnimateScene);
    CREATE_FUNC(TransitionAnimateScene);
    virtual bool init() override;
    
protected:
    vector<Sprite*> _shadowVec;
    int _animationIndex;
protected:
    virtual void onEnterTransitionDidFinish() override;
    void showStore();
    void showStory();
    
    void showAnimation();
    
};

#endif /* TransitionAnimateScene_h */
