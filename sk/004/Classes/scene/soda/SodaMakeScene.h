//
//  SodaMakeScene.h
//  shake004
//
//  Created by liji on 16/10/11.
//
//

#ifndef SodaMakeScene_h
#define SodaMakeScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
class BowlNode;
class SodaMakeScene : public ESMakeBaseScene
{
public:
    
    SodaMakeScene();
    ~SodaMakeScene();
    
    MY_SCENE(SodaMakeScene);
    CREATE_FUNC(SodaMakeScene);
    bool init();
    
protected:
    BowlNode* _cup;
    Widget *_selectMachineWidget;
    bool _isAnimation;
    vector<Sprite*> _iceVec;
    Sprite* _cupTop;

private:
    void stepChooseCup();
    void stepChooseFlavor();
    void juiceMachineSelect(Widget *widget);
    void juiceMachineUnSelect();
    void stepAddIce();
    void addIce();
    
};

#endif /* SodaMakeScene_h */
