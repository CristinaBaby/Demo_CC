//
//  PatternPenSprite.h
//  BedtimeStory
//
//  Created by maxiang on 9/16/15.
//
//

#ifndef __BedtimeStory__PatternPenSprite__
#define __BedtimeStory__PatternPenSprite__

#include "BasePenSprite.h"

class PatternPenSprite : public BasePenSprite
{
public:
    static PatternPenSprite* createWithPatternData(Pattern *pattern);
    bool initWithPatternData(Pattern *pattern);
    
    void setPatternPenData(Pattern *pattern){_pattern = pattern;};
    Pattern* getPatternPenData(){return _pattern;};
    
protected:
    Pattern *_pattern;
};



#endif /* defined(__BedtimeStory__PatternPenSprite__) */
