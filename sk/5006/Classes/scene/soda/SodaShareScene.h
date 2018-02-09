//
//  SodaShareScene.h
//  shake004
//
//  Created by liji on 16/10/31.
//
//

#ifndef SodaShareScene_h
#define SodaShareScene_h

#include <stdio.h>
#include "ShareBaseScene.h"
#include "Charactor.h"
class SodaShareScene : public ShareBaseScene
{
public:
    
    SodaShareScene();
    ~SodaShareScene();
    
    MY_SCENE(SodaShareScene);
    CREATE_FUNC(SodaShareScene);
    bool init();
    virtual void initFood();

protected:
    Charactor* _npc;
    bool _walkOut;
    bool _allCoinGet;
private:
    void checkResever();
    void resetServe();
    
};

#endif /* SodaShareScene_h */
