//
//  RainbowMakeScene.h
//  shake5008
//
//  Created by liji on 17/1/3.
//
//

#ifndef RainbowMakeScene_h
#define RainbowMakeScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"

//cakepop and cupcake share the same add scene

class BowlNode;
class RainbowMakeScene : public ESMakeBaseScene
{
public:
    
    RainbowMakeScene();
    ~RainbowMakeScene();
    
    MY_SCENE(RainbowMakeScene);
    CREATE_FUNC(RainbowMakeScene);
    bool init();
    
protected:
    BowlNode* _bowl;
    BowlNode* _sifter;
    int _saltIdd;
    int _eggCount;
    int _butterTipIndex;

private:
    void initBowl();
    void stepAddIngredients1();
    void stepAddIngredients2();
    void stepAddCornPowder();
    void stepMix();
    void stepOver();
    
    void stepMixAbanded();

    
    void addMilk();
    void addPowder();
    void addSalt();
    
    void addEgg();
    void addButter();
    void addSugar();

    void shakeSifter();
    
    void onEnterTransitionDidFinish();
    
};

#endif /* RainbowMakeScene_h */
