//
//  MakeStep33.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#ifndef MakeStep33_hpp
#define MakeStep33_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

//make步骤的模板
class MakeStep3 : public BaseLayer  {
public:
    MakeStep3();
    ~MakeStep3();
public:
    bool init();
    CREATE_FUNC(MakeStep3);
    CREATE_SCENE(MakeStep3);
    
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
    
    Sprite* m_pBowl;
    
    void showBowl();
    void showShowTool();
    int m_iStepNum;
    Vector<Sprite*> m_vTool;
    
    Vec2 _bowlPos;
    Vec2 _LayerPos;
public:
     std::function<void()> nextStep = nullptr;
};


#endif /* MakeStep33_hpp */
