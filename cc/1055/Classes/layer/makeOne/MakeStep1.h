//
//  MakeStep1.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/29.
//
//

#ifndef MakeStep1_hpp
#define MakeStep1_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

class MakeStep1 : public BaseLayer  {
public:
    MakeStep1();
    ~MakeStep1();
public:
    bool init();
    CREATE_FUNC(MakeStep1);
    CREATE_SCENE(MakeStep1);
    
public:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    void goToNextStep(float dt);
    
    void showHand();
public:
    std::function<void()> nextStep = nullptr;
    int m_iStepNum;
    Sprite* m_pBowl;
    Vec2 _bowlPos;
    Vec2 _LayerPos;
    
    bool m_bIsShowHand;
    
    
};

#endif /* MakeStep1_hpp */
