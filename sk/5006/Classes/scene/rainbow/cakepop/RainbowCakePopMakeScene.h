//
//  RainbowCakePopMakeScene.h
//  shake5008
//
//  Created by liji on 17/1/3.
//
//

#ifndef RainbowCakePopMakeScene_h
#define RainbowCakePopMakeScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

class OvenNode;
class BowlNode;
class CakepopOvenPlateNode;
class RainbowCakePopMakeScene : public ESMakeBaseScene
{
public:
    
    RainbowCakePopMakeScene();
    ~RainbowCakePopMakeScene();
    
    MY_SCENE(RainbowCakePopMakeScene);
    CREATE_FUNC(RainbowCakePopMakeScene);
    bool init();
    
protected:
    Vector<BowlNode*> _cupVec;
    BowlNode* _bowl;
    Sprite* _inBowl;
    Sprite* _flow;
    CakepopOvenPlateNode* _plate;
    OvenNode* _oven;
    int _waterIdd = 0;
    int _cupIndex = 0;
    bool _firstChooseColor = true;
    
private:
    void stepPourInCup();
    void stepChooseColor();
    void stepPourInMould();
    void stepBake();
    void stepFinish();
    
    void startPour();
    void getNextPos();
    void fillInCup(int index);
    
    void colorChoosed(int cIndex);
    void showMixCup(int cIndex);
    
    void pourIntoMould();
    void bake();
};

#endif /* RainbowCakePopMakeScene_h */
