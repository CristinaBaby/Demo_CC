//
//  CBColorPenSprite.h
//  ColorBook
//
//  Created by maxiang on 4/28/15.
//
//

#ifndef __ColorBook__CBColorPenSprite__
#define __ColorBook__CBColorPenSprite__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"
#include "../Model/CBColouringManager.h"
#include "../Layer/CBPenSprite.h"

CB_BEGIN_NAMESPACE

class ColorPenSprite : public PenSprite
{
public:
    
    static ColorPenSprite* createWithPenData(Pen *pen);
    bool initWithPenData(Pen *pen);

    void setColorPenData(Pen *pen){_pen = pen;};
    Pen* getColorPenData(){return _pen;};

protected:
    Pen *_pen;
    cocos2d::Vec2 _originalPosition;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBColorPenSprite__) */
