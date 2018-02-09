//
//  ShaveIceLayer.h
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#ifndef __Make__ShaveIceLayer__
#define __Make__ShaveIceLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class ShaveIceLayer :public BaseLayer
{
public:
    static Scene* scene();
    CREATE_FUNC(ShaveIceLayer);
    bool init();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual void nextBtnClicked();
    
    void initSnowCone();
    void showShaveSnowEffect();
    void stopShaveSnowEffect();
    
private:
    Sprite* pile_;
    Sprite* basicPile_;
    Sprite* knife_;
    ParticleSystemQuad* shaveSnow_;
    
    int shaveCount_;

};
#endif /* defined(__Make__ShaveIceLayer__) */
