//
//  RateUsNode.h
//  CCATS022
//
//  Created by liji on 16/11/21.
//
//

#ifndef RateUsNode_h
#define RateUsNode_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

class RateUsNode : public ESMakeBaseScene
{
public:
    
    RateUsNode();
    ~RateUsNode();
    
    MY_SCENE(RateUsNode);
    CREATE_FUNC(RateUsNode);
    bool init();
    
protected:
    
private:
    void onEnterTransitionDidFinish();
    
};

#endif /* RateUsNode_h */
