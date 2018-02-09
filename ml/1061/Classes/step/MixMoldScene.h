//
//  MixMoldScene.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/6.
//
//

#ifndef MixMoldScene_hpp
#define MixMoldScene_hpp

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"
#include "MySpriteButton.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class MixMoldScene:public GameBaseScene{
private:
    MixMoldScene();
    ~MixMoldScene();
public:
    virtual bool init();
    CREATE_FUNC(MixMoldScene);
    CREATE_SCENE(MixMoldScene);
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    void initUI();

    void nextStep();
    void switchBtn(Ref *sender);
    
    void showPotMix(float dt);
    void showPotLid(float dt);
    void showPotFire(float dt);
    void showPotStir(int index);
    void potLidBtn(Ref *sender);
    void potLidAction(float dt);
    int  randVectorNum();
    void showHand();
    void showParticle(float dt);
    
    void showHandClickPot(int index);
    
    
    //锅里动画的三个阶段
    void potActionStepOne(MySpriteButton* btn);
    void potActionStepTwo(MySpriteButton* btn);
    void potActionStepThree(MySpriteButton* btn);
    
    void potCoverClick(Ref *sender);
    void showFont(float dt);
private:
    Sprite* m_pBG;
    Sprite* m_pStoven;
    Sprite* _pStoven;
    MySpriteButton* m_pSwitch;
    Vector<Sprite*> m_vPot;
    Vector<MySpriteButton*> m_vPotCover;
    Vector<Sprite*> m_vFire;
    Vector<MySpriteButton*> m_vPotLid;
    vector<int> m_vPotLidNum;
    int m_iStep;
    bool m_bSwitchOn;
    Sprite* m_pHand;
    bool m_bStirIsShow;
    int m_iSuccessPot;
    int m_iFailPot;
    vector<int> m_vOverPotIndex;
    bool m_bIsShowHand1;
    bool m_bIsShowHand2;
};

#endif /* MixMoldScene_hpp */
