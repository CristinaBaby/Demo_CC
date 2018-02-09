//
//  MakeStepEatEat.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/18.
//
//

#ifndef MakeStepEatEat_hpp
#define MakeStepEatEat_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"
#include "EatSprite.h"
//make步骤的模板
class MakeStepEat : public BaseLayer {
public:
    MakeStepEat();
    ~MakeStepEat();
public:
    bool init();
    CREATE_FUNC(MakeStepEat);
    CREATE_SCENE(MakeStepEat);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    
    void onNegativeClick(void*);
    void onPositiveClick(void*);

    void checkEatNodePercentage(float dt);
    
    Sprite* m_pBowl;
    EatSprite* m_pEatNode;
    Vec2 _bowlPos;
    Vec2 btnPos[4];
    bool m_bIsReward;
    bool m_bIsEat;
    int _tapNum;
    
};


#endif /* MakeStepEatEat_hpp */
