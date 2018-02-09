//
//  TexasPizzaShareScene.h
//  shake004
//
//  Created by liji on 16/10/31.
//
//

#ifndef TexasPizzaShareScene_h
#define TexasPizzaShareScene_h

#include <stdio.h>
#include "ShareBaseScene.h"
class TexasPizzaShareScene : public ShareBaseScene
{
public:
    
    TexasPizzaShareScene();
    ~TexasPizzaShareScene();
    
    MY_SCENE(TexasPizzaShareScene);
    CREATE_FUNC(TexasPizzaShareScene);
    bool init();
    virtual void initFood();

protected:
private:

    
};

#endif /* TexasPizzaShareScene_h */
