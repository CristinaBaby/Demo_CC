//
//  MakeOtherDec.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#ifndef MakeOtherDec_hpp
#define MakeOtherDec_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"

//make步骤的模板
class MakeOtherDec : public BaseLayer  {
public:
    MakeOtherDec();
    ~MakeOtherDec();
public:
    bool init();
    CREATE_FUNC(MakeOtherDec);
    CREATE_SCENE(MakeOtherDec);
    
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
    Sprite* m_pBowl;
};


#endif 
