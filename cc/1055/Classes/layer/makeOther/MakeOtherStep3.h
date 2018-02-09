//
//  MakeOtherStep3.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#ifndef MakeOtherStep3_hpp
#define MakeOtherStep3_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

//make步骤的模板
class MakeOtherStep3 : public BaseLayer  {
public:
    MakeOtherStep3();
    ~MakeOtherStep3();
public:
    bool init();
    CREATE_FUNC(MakeOtherStep3);
    CREATE_SCENE(MakeOtherStep3);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
private:
    
    Sprite* m_pSpoon;
    Sprite* m_pPaste;
    string m_sFlovar;
    
    int m_iRollNum;
    bool m_bIsRoll;
    bool m_bIsSound;
public:
    std::function<void()> nextStep = nullptr;
};

#endif /* MakeOtherStep3_hpp */
