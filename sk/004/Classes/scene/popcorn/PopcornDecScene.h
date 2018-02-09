//
//  PopcornDecScene.h
//  shake004
//
//  Created by liji on 16/10/27.
//
//

#ifndef PopcornDecScene_h
#define PopcornDecScene_h

#include <stdio.h>
#include "DecorationBaseLayer.h"
#include "ESMakeBaseScene.h"
#include "PopcornNode.h"
using namespace std;
class ComponentTouchMove;
class PopcornDecScene : public DecorationBaseLayer
{
public:
    MY_SCENE(PopcornDecScene);
    CREATE_FUNC(PopcornDecScene);
    virtual bool init() override;
    PopcornDecScene();
    ~PopcornDecScene(){};
    
protected:
    PopcornNode* _popcorn;
    ComponentTouchMove* _sprinkleTouch;
    bool _firstSticker = true;
    bool _firstCup = false;
    bool _firstStraw = true;
    bool _firstBg = true;
private:
    void showScrollView(int index);
    void onEnter();
    void setTypeEnable(bool enable);
};

#endif /* PopcornDecScene_h */
