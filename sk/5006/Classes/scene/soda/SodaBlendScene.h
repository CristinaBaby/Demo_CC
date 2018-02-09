//
//  SodaBlendScene.h
//  CCATS022
//
//  Created by liji on 16/11/11.
//
//

#ifndef SodaBlendScene_h
#define SodaBlendScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
class BowlNode;
class ESScrollView;
class ComponentTouchMove;
class ComponentTurning;
class ComponentCycleDisplay;
class JuiceMachine;
class SodaBlendScene : public ESMakeBaseScene
{
public:
    
    SodaBlendScene();
    ~SodaBlendScene();
    
    MY_SCENE(SodaBlendScene);
    CREATE_FUNC(SodaBlendScene);
    bool init();
    
protected:
    BowlNode* _bowl;
    BowlNode* _blender;
    BowlNode* _cup;

    vector<Sprite* > _tempInBlenderVec;
    vector<Node* > _tempButton;
    vector<ComponentCycleDisplay* > _cycleVec;
    vector<Sprite* > _crashedFruitVec;
    ComponentTouchMove* _cTouch;
    ComponentTurning* _cTurn;
    Sprite* _straw;
    bool _isFirst;
    bool _isAnimation = false;
    ui::Widget* _selectMachineWidget;
    
    Sprite* _inBlender;
    bool _isBlenderChosen;
private:
    void showIce();
    
    void chooseJuice();
    void juiceMachineSelect(Widget *widget);
    void juiceMachineUnSelect();
    
    void chooseBlender();
    void showBlender();
    void showAutoBlender();
    void showManualBlender();
    void startBlend();
    void pauseBlend();
    void resumeBlend();
    void chooseCup();
    void pourJuice();
    void chooseStraw();
    void updateStraw();

};

#endif /* SodaBlendScene_h */
