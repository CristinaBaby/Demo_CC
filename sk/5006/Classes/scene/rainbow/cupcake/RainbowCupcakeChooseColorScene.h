//
//  RainbowCupcakeChooseColorScene.h
//  shake5008
//
//  Created by liji on 17/1/10.
//
//

#ifndef RainbowCupcakeChooseColorScene_h
#define RainbowCupcakeChooseColorScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

class BowlNode;
class ComponentTouchMove;
class RainbowCupcakeChooseColorScene : public ESMakeBaseScene
{
public:
    
    RainbowCupcakeChooseColorScene();
    ~RainbowCupcakeChooseColorScene();
    
    MY_SCENE(RainbowCupcakeChooseColorScene);
    CREATE_FUNC(RainbowCupcakeChooseColorScene);
    bool init();
    
protected:
    int _colorIndex; //和cupcakecolorindexVecsize相同时跳下一步
    int _scoopCount;
    int _bagIndex;
    BowlNode* _bowl;
    BowlNode* _cup;
    BowlNode* _bag;
    Sprite* _inBag;
    ComponentTouchMove* _cTouch;
    BowlNode* _mixBag;
    Sprite* _mixInCup;
    Sprite* _cakeCup;
    bool _firstTouch;
    float _totalDis;
    float _moveDis;

    
private:
    void stepChooseColors();
    void stepMixColor();
    void stepChooseCup();
    void stepAddMix();
    
    void mix();
    void pack();
    void packBag();
    
    void updateCup();
    
    void startPress();
    void resumePress();
    void pausePress();
};

#endif /* RainbowCupcakeChooseColorScene_h */
