//
//  ColorPenSprite.h
//  BedtimeStory
//
//  Created by maxiang on 9/16/15.
//
//

#ifndef __BedtimeStory__ColorPenSprite__
#define __BedtimeStory__ColorPenSprite__

#include "BasePenSprite.h"

class ColorPenSprite : public BasePenSprite
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

#endif /* defined(__BedtimeStory__ColorPenSprite__) */
