//
//  MakeOtherStep1.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/6.
//
//

#ifndef MakeOtherStep1_hpp
#define MakeOtherStep1_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

//make步骤的模板
class MakeOtherStep1 : public BaseLayer  {
public:
    MakeOtherStep1();
    ~MakeOtherStep1();
public:
    bool init();
    CREATE_FUNC(MakeOtherStep1);
    CREATE_SCENE(MakeOtherStep1);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    void showTool(float dt);
    void showNextTool(float dt);
    void goToNextStep(float dt);
    
    void showHand();
public:
    std::function<void()> nextStep = nullptr;
    std::function<void()> resetLayer = nullptr;
private:
    Sprite* m_pBowl;
    Vector<Sprite*> m_vTool;
    Vec2 toolPos[11];
    int m_iStepNum;
    
    Vec2 _bowlPos;
    Vec2 _LayerPos;
    Vec2 _detalPos;
    bool m_bIsShowHand;
};

#endif /* MakeOtherStep1_hpp */
