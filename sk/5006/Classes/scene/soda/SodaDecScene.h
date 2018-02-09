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
class AddSprinkle;
class ComponentTouchMove;
class SodaDecScene : public DecorationBaseLayer
{
public:
    MY_SCENE(SodaDecScene);
    CREATE_FUNC(SodaDecScene);
    virtual bool init() override;
    SodaDecScene();
    ~SodaDecScene(){};
    
    static Scene* createSceneWithStepIndex(int index);
    static SodaDecScene* createWithStepIndex(int index);
    
    
    
protected:
    JuiceBottleNode* _juice;
    ComponentTouchMove *_sprinkleTouch;
    bool _first;

    Widget *_selectMachineWidget;
    bool _isAnimation;

    bool _firstCup = false;
    float _stickerDeltaY;
    AddSprinkle* currentSprinkle;
    Sprite* _temp;
    int _stepIndex;
private:
    void chooseCream();
    void juiceMachineSelect(Widget *widget);
    void juiceMachineUnSelect();

    void showOtherDec();
    void showScrollView(int index);
    void onEnter()override ;
    void setTypeEnable(bool enable);
    
    void chooseStraw();
};

#endif /* SodaDecScene_h */
