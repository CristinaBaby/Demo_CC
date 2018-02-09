//
//  CBGameLoadingLayer.h
//  ColorBook
//
//  Created by maxiang on 5/15/15.
//
//

#ifndef __ColorBook__CBGameLoadingLayer__
#define __ColorBook__CBGameLoadingLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class GameLoadingLayer : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(GameLoadingLayer);
    
    virtual bool init();
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBGameLoadingLayer__) */
