//
//  RainbowCupcakeDecScene.h
//  shake5008
//
//  Created by liji on 17/1/12.
//
//

#ifndef RainbowCupcakeDecScene_h
#define RainbowCupcakeDecScene_h

#include <stdio.h>
#include "DecorationBaseLayer.h"
#include "ESMakeBaseScene.h"
#include "RainbowCupcakeNode.h"
using namespace std;
class ComponentTouchMove;
class RainbowCupcakeDecScene : public ESMakeBaseScene
{
public:
    MY_SCENE(RainbowCupcakeDecScene);
    CREATE_FUNC(RainbowCupcakeDecScene);
    virtual bool init() override;
    RainbowCupcakeDecScene():
    _food(nullptr),
    _bag(nullptr),
    _drawTopping(nullptr),
    _shadow(nullptr)
    {
        
    };
    ~RainbowCupcakeDecScene(){};
    
protected:
    RainbowCupcakeNode* _food;
    Sprite* _bag;
    ComponentTouchMove *_drawTopping;
    Sprite* _shadow;
private:
    void showToppingDec();
    void showOtherDec();
    void showAddSprinkle();
    bool isOtherDecOver();
    
};

#endif /* RainbowCupcakeDecScene_h */
