//
//  PaintMode.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__PaintMode__
#define __ColorBook__PaintMode__

#include "cocos2d.h"
#include "BaseMode.h"

class PaintMode : public BaseMode
{
public:
    PaintMode(GameScene * layer);
    virtual ~PaintMode();
    
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event *unused_event);
};

#endif /* defined(__ColorBook__PaintMode__) */
