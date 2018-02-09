//
//  TransitionSceneEx.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__TransitionSceneEx__
#define __ColorBook__TransitionSceneEx__

#include "cocos2d.h"
#include "BaseMode.h"


class TransitionFadeEx : public TransitionFade
{
public:
    virtual void onEnter() override;
    
    void myHideOutShowIn();
    
    static TransitionFadeEx * create(float duration, Scene *scene, const Color3B& color);
protected:
    bool    _canChange;
};

#endif /* defined(__ColorBook__TransitionSceneEx__) */
