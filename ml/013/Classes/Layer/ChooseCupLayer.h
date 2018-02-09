//
//  ChooseCupLayer.h
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#ifndef __Make__ChooseCupLayer__
#define __Make__ChooseCupLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "UIUnitScrollView.h"
USING_NS_CC;

class ChooseCupLayer :public BaseLayer
{
public:
    static Scene* scene();
    CREATE_FUNC(ChooseCupLayer);
    bool init();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual void nextBtnClicked();
    
    void lockedUnitClicked();
    void freeUnitClicked(std::string pIconImageName);
    void checkUnlock();

private:
    Sprite* cup_;
    UIUnitScrollView* _unitView = nullptr;
};
#endif /* defined(__Make__ChooseCupLayer__) */
