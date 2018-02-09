//
//  GetSnowLayer.h
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#ifndef __Make__GetSnowLayer__
#define __Make__GetSnowLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class GetSnowLayer :public BaseLayer
{
public:
    static Scene* scene();
    CREATE_FUNC(GetSnowLayer);
    bool init();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual void nextBtnClicked();
    void initUI();
    void fillSnow();

private:
    Sprite* boxCover_;
    Sprite* boxBottom_;
    Sprite* snowInMachine_;
    Sprite* snowInCup_;
    Sprite* spoon_;
    Sprite* snowInSpoon_;
    Sprite* cup_;

    bool canFillSnow_;
    int snowCount_;
};
#endif /* defined(__Make__GetSnowLayer__) */
