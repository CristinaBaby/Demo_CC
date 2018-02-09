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
#include "cocosgui.h"

CB_BEGIN_NAMESPACE

enum buttonTag{
    contineTag = 111,
    resetTag
};

class ContinueLayer : public cocos2d::LayerColor
{
public:
    static ContinueLayer* create(const int packIndex, const int pictureIndex);
    virtual bool init(const int packIndex, const int pictureIndex);
    
    void continueAction();
    void resetAction();
    void touchDown(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
protected:
    int _packIndex;
    int _pictureIndex;
};

CB_END_NAMESPACE
#endif /* defined(__ColorBook__CBContinueLayer__) */
