//
//  StepLayer.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-24.
//
//

#ifndef __OreoMaker__StepLayer__
#define __OreoMaker__StepLayer__

#include <iostream>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "GameLayer.h"
#include "ToolSprite.h"
#include "AreaLayer.h"
#include "AudioHelp.h"
USING_NS_CC;


class GameLayer;

class StepLayer: public BaseLayer{
    
public:
    StepLayer(GameLayer* gameLayer);
    StepLayer();
    ~StepLayer();
    static StepLayer* create(GameLayer* gameLayer);
    
public:
    virtual bool init();
public:
    bool initUI();
    virtual void onEnter();
   
    
protected:
    GameLayer* m_pGameLayer;
};
#endif /* defined(__OreoMaker__StepLayer__) */
