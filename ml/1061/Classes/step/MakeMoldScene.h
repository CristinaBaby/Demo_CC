//
//  MakeMoldScene.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/21.
//
//

#ifndef MakeMoldScene_hpp
#define MakeMoldScene_hpp

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"
#include "SelectMoldLayer.h"
#include "PourMoldLayer.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class MakeMoldScene:public GameBaseScene{
private:
    MakeMoldScene();
    ~MakeMoldScene();
    
    virtual bool init();
    CREATE_FUNC(MakeMoldScene);
public:
    CREATE_SCENE(MakeMoldScene);
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    
    void initUI();
    //下一步
    void nextStep();
private:
    Sprite* m_pBG;
    SelectMoldLayer* m_pSelectMoldLayer;
    PourMoldLayer*   m_pPourMoldLayer;
  
};

#endif /* MakeMoldScene_hpp */
