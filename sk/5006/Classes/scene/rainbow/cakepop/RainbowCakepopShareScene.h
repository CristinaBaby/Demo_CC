//
//  RainbowCakepopShareScene.h
//  shake5008
//
//  Created by liji on 17/1/13.
//
//

#ifndef RainbowCakepopShareScene_h
#define RainbowCakepopShareScene_h

#include <stdio.h>
#include "ShareBaseScene.h"
class RainbowCakepopShareScene : public ShareBaseScene
{
public:
    
    RainbowCakepopShareScene();
    ~RainbowCakepopShareScene();
    
    MY_SCENE(RainbowCakepopShareScene);
    CREATE_FUNC(RainbowCakepopShareScene);
    bool init();
    virtual void initFood();
    
protected:
private:
    
};



#endif /* RainbowCakepopShareScene_h */
