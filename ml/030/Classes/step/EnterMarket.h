//
//  enterMarket.h
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/6.
//
//

#ifndef __MLABS030_IceCreamKids_CookingGames__enterMarket__
#define __MLABS030_IceCreamKids_CookingGames__enterMarket__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class EnterMarket:public GameBaseScene{
private:
    EnterMarket();
    ~EnterMarket();
public:
    virtual bool init();
    CREATE_FUNC(EnterMarket);
    CREATE_SCENE(EnterMarket);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
    
    void bgAction(float dt);
    void carAction(float dt);
    void _carAction(float dt);
public:
    //下一步
    void nextStep();
public:
    Sprite* m_pBG;
    Sprite* m_pCar;
};

#endif /* defined(__MLABS030_IceCreamKids_CookingGames__enterMarket__) */
