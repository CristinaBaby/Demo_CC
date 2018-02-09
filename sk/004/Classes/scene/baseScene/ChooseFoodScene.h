//
//  ChooseFoodScene.h
//  ccats1061
//
//  Created by liji on 16/8/25.
//
//

#ifndef ChooseFoodScene_h
#define ChooseFoodScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

class ChooseFoodScene : public ESMakeBaseScene
{
public:
    
    ChooseFoodScene();
    ~ChooseFoodScene();
    
    MY_SCENE(ChooseFoodScene);
    CREATE_FUNC(ChooseFoodScene);
    bool init();
    
protected:
    
private:
    void onEnter();
    
};

#endif /* ChooseFoodScene_h */
