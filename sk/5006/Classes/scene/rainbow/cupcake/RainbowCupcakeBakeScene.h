//
//  RainbowCupcakeBakeScene.h
//  shake5008
//
//  Created by liji on 17/1/11.
//
//

#ifndef RainbowCupcakeBakeScene_h
#define RainbowCupcakeBakeScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
class OvenNode;
class BowlNode;
class CupCakeOvenPlateNode;

class RainbowCupcakeBakeScene : public ESMakeBaseScene
{
public:
    
    RainbowCupcakeBakeScene();
    ~RainbowCupcakeBakeScene();
    
    MY_SCENE(RainbowCupcakeBakeScene);
    CREATE_FUNC(RainbowCupcakeBakeScene);
    bool init();
    
protected:
    CupCakeOvenPlateNode* _plate;
    OvenNode* _oven;

private:
    void initBake();
    void bake();
    
};

#endif /* RainbowCupcakeBakeScene_h */
