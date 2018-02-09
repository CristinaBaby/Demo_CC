//
//  SodaDecScene.h
//  shake004
//
//  Created by liji on 16/10/27.
//
//

#ifndef SodaDecScene_h
#define SodaDecScene_h

#include <stdio.h>
#include "DecorationBaseLayer.h"
#include "ESMakeBaseScene.h"
#include "JuiceBottleNode.h"
using namespace std;
class ComponentTouchMove;
class SodaDecScene : public DecorationBaseLayer
{
public:
    MY_SCENE(SodaDecScene);
    CREATE_FUNC(SodaDecScene);
    virtual bool init() override;
    SodaDecScene();
    ~SodaDecScene(){};
    
protected:
    JuiceBottleNode* _juice;
    ComponentTouchMove *_sprinkleTouch;

    
    bool _firstCup = false;
    bool _firstSprinkle = true;
    float _stickerDeltaY;
private:
    void showScrollView(int index);
    void onEnter()override ;
    void setTypeEnable(bool enable);
};

#endif /* SodaDecScene_h */
