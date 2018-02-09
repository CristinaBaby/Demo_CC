//
//  HomeScene.h
//  BHUG1062
//
//  Created by liji on 16/5/3.
//
//

#ifndef HomeScene_h
#define HomeScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "depends/component/layerext/StudioLayer.h"
#include "component/makeComponent/basicComponent/ComponentBase.h"
#include "ESMakeBaseScene.h"
using namespace cocos2d;
using namespace std;

class HomeScene : public ESMakeBaseScene
{
public:
    
    MY_SCENE(HomeScene);
    CREATE_FUNC(HomeScene);
    bool init() override;
    
    HomeScene();
    ~HomeScene();
    
    virtual void onButtonClick(Ref* ref,Widget::TouchEventType type) override;

    
    void onEnterTransitionDidFinish() override;
    void onExit() override;
private:
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
    
    bool _isFirst = true;
};

#endif /* HomeScene_h */
