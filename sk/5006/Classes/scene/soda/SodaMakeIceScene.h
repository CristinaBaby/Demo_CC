//
//  SodaMakeIceScene.h
//  CCATS022
//
//  Created by liji on 16/11/10.
//
//

#ifndef SodaMakeIceScene_h
#define SodaMakeIceScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
class IceContainerNode;
class SodaMakeIceScene : public ESMakeBaseScene
{
public:
    
    SodaMakeIceScene();
    ~SodaMakeIceScene();
    
    MY_SCENE(SodaMakeIceScene);
    CREATE_FUNC(SodaMakeIceScene);
    bool init();
    
protected:
    IceContainerNode* _iceContainer;
    Sprite* _kettle;
    int _waterIdd;
private:
    void iceBoxChoosed();
    void startPour();
    void getNextPos();
    void freezeIce();
    void onEnterTransitionDidFinish();
};

#endif /* SodaMakeIceScene_h */
