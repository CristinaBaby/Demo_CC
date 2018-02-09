//
//  LoadingTransitionScene.h
//  BHUG1062
//
//  Created by liji on 16/5/4.
//
//

#ifndef LoadingTransitionScene_h
#define LoadingTransitionScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "depends/component/layerext/StudioLayer.h"
#include "depends/component/scenemanager/SceneManager.h"
#include "depends/component/screenadapter/CMVisibleRect.h"
using namespace cocos2d;
using namespace std;

class LoadingTransitionScene : public StudioLayer
{
public:
    CREATE_FUNC(LoadingTransitionScene);
    bool init();
    
    void onEnter();
    void onExit();
    
    function<void()> _transitionScene = nullptr;
    
private:
    void loadingAnimation(float dt);
protected:
    Vector<Sprite*> _stars;
    int _index = 0;
};

#endif /* LoadingTransitionScene_h */
