//
//  PatternWidget.h
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#ifndef __PaintDress__PatternWidget__
#define __PaintDress__PatternWidget__

#include "cocos2d.h"
#include "BasePenWidget.h"
#include "../Model/AppConfigs.h"
#include "../Model/PaintingManager.h"

class PatternWidget : public BasePenWidget
{
public:
    static PatternWidget* create(const Pattern& pen);
    virtual bool init(const Pattern& pen);
    
    Pattern& getPattern(){return _pattern;};
    
protected:
    Pattern _pattern;
};

#endif /* defined(__PaintDress__PatternWidget__) */
