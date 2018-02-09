//
//  PopcornShareScene.h
//  shake004
//
//  Created by liji on 16/10/31.
//
//

#ifndef PopcornShareScene_h
#define PopcornShareScene_h

#include <stdio.h>
#include "ShareBaseScene.h"
#include "PopcornNode.h"
class PopcornShareScene : public ShareBaseScene
{
public:
    
    PopcornShareScene();
    ~PopcornShareScene();
    
    MY_SCENE(PopcornShareScene);
    CREATE_FUNC(PopcornShareScene);
    bool init();
    virtual void initFood();

protected:
    PopcornNode* _popcorn;
private:

    
};

#endif /* PopcornShareScene_h */
