//
//  RainbowCakePopPressScene.h
//  shake5008
//
//  Created by liji on 17/1/5.
//
//

#ifndef RainbowCakePopPressScene_h
#define RainbowCakePopPressScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

class BowlNode;
class ComponentTouchMove;
class RainbowCakePopPressScene : public ESMakeBaseScene
{
public:
    
    RainbowCakePopPressScene();
    ~RainbowCakePopPressScene();
    
    MY_SCENE(RainbowCakePopPressScene);
    CREATE_FUNC(RainbowCakePopPressScene);
    bool init();
    
protected:
    int _cupIndex;
    float _moveDis;
    int _bagIndex;
    BowlNode* _bag;
    vector<Sprite*> _cakeVec;
    ComponentTouchMove* _cTouch;
    float _totalDis;
    Sprite* _stick;
    const int _maxLength = 20000;
private:
    void stepPress();
    void stepRoll();
    void stepChooseStick();

    void packBag();
    void updateStick();
    
};

#endif /* RainbowCakePopPressScene_h */
