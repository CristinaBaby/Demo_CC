//
//  CBPatternPenSprite.h
//  ColorBook
//
//  Created by maxiang on 4/28/15.
//
//

#ifndef __ColorBook__CBPatternPenSprite__
#define __ColorBook__CBPatternPenSprite__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"
#include "../Model/CBColouringManager.h"
#include "../Layer/CBPenSprite.h"

CB_BEGIN_NAMESPACE

class PatternPenSprite : public PenSprite
{
public:
    static PatternPenSprite* createWithPatternData(Pattern *pattern);
    bool initWithPatternData(Pattern *pattern);
    
    void setPatternPenData(Pattern *pattern){_pattern = pattern;};
    Pattern* getPatternPenData(){return _pattern;};
    
    bool isLoced(){return _isLocked;};
    void removeLock();
protected:
    Pattern *_pattern;
    cocos2d::Sprite *_lock;
    bool _isLocked;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBPatternPenSprite__) */
