//
//  CBContinueLayer.h
//  ColorBook
//
//  Created by maxiang on 5/7/15.
//
//

#ifndef __ColorBook__CBContinueLayer__
#define __ColorBook__CBContinueLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class ContinueLayer : public cocos2d::LayerColor
{
public:
    static ContinueLayer* create(const int packIndex, const int pictureIndex);
    virtual bool init(const int packIndex, const int pictureIndex);
    
    void continueAction();
    void resetAction();
    
protected:
    int _packIndex;
    int _pictureIndex;
};

CB_END_NAMESPACE
#endif /* defined(__ColorBook__CBContinueLayer__) */
