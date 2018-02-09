//
//  RainbowCupcakeShareScene.h
//  shake5008
//
//  Created by liji on 17/1/12.
//
//

#ifndef RainbowCupcakeShareScene_h
#define RainbowCupcakeShareScene_h

#include <stdio.h>
#include "ShareBaseScene.h"
class RainbowCupcakeShareScene : public ShareBaseScene
{
public:
    
    RainbowCupcakeShareScene();
    ~RainbowCupcakeShareScene();
    
    MY_SCENE(RainbowCupcakeShareScene);
    CREATE_FUNC(RainbowCupcakeShareScene);
    bool init();
    virtual void initFood();
    
protected:
private:
    
};


#endif /* RainbowCupcakeShareScene_h */
