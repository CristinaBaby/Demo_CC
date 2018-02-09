//
//  StepLayer.h
//  MLABS1047_HotDogMaker
//
//  Created by wusonglin1 on 15/8/4.
//
//

#ifndef __MLABS1047_HotDogMaker__StepLayer__
#define __MLABS1047_HotDogMaker__StepLayer__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class StepLayer:public GameBaseScene{
private:
    StepLayer();
    ~StepLayer();
public:
    virtual bool init();
    CREATE_FUNC(StepLayer);
    CREATE_SCENE(StepLayer);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
public:
    //下一步
    void nextStep();
public:
    Sprite* m_pBG;
};

#endif /* defined(__MLABS1047_HotDogMaker__StepLayer__) */
