//
//  SodaChooseStepScene.h
//  CCATS022
//
//  Created by liji on 16/11/16.
//
//

#ifndef SodaChooseStepScene_h
#define SodaChooseStepScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
#include "Charactor.h"

class SodaChooseStepScene : public ESMakeBaseScene
{
public:
    
    SodaChooseStepScene();
    ~SodaChooseStepScene();
    
    MY_SCENE(SodaChooseStepScene);
    CREATE_FUNC(SodaChooseStepScene);
    bool init();
    
protected:
    Charactor* _npc;

private:
    void showContentsInBtns(Widget* item, bool unlocked);
    
};

#endif /* SodaChooseStepScene_h */
