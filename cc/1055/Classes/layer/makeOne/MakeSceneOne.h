//
//  MakeSceneOne.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#ifndef MakeSceneOne_hpp
#define MakeSceneOne_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Configure.h"
#include "MainScene.h"

USING_NS_CC;

class MakeSceneOne : public Layer {
    MakeSceneOne();
    ~MakeSceneOne();
    
public:
    bool init();
    CREATE_FUNC(MakeSceneOne);
    CREATE_SCENE(MakeSceneOne);
public:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    //轮换展示Layer
    void showLayer1();
    void showLayer2(Texture2D* texture);
    void showLayer3();
    void showLayer4(Texture2D* texture);
    
};

#endif /* MakeSceneOne_hpp */
