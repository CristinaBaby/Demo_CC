//
//  ChooseServeScene.h
//  shake004
//
//  Created by liji on 16/10/28.
//
//

#ifndef ChooseServeScene_h
#define ChooseServeScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

class ChooseServeScene : public ESMakeBaseScene
{
public:
    
    ChooseServeScene();
    ~ChooseServeScene();
    
    MY_SCENE(ChooseServeScene);
    CREATE_FUNC(ChooseServeScene);
    bool init();
    
protected:
    
private:
    
    
};

#endif /* ChooseServeScene_h */
