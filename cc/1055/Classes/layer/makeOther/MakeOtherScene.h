//
//  MakeOtherScene.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/12.
//
//

#ifndef MakeOtherScene_hpp
#define MakeOtherScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Configure.h"
#include "MainScene.h"

USING_NS_CC;

class MakeOtherScene : public Layer {
    MakeOtherScene();
    ~MakeOtherScene();
    
public:
    bool init();
    CREATE_FUNC(MakeOtherScene);
    CREATE_SCENE(MakeOtherScene);
public:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    //轮换展示Layer
    void showLayer1();
    void showLayer2(Texture2D* spriteTexture2D);
    void showLayer3();
    void showLayer4();
};


#endif /* MakeOtherScene_hpp */
