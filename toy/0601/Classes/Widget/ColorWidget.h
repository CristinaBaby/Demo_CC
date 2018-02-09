//
//  ColorWidget.h
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#ifndef __PaintDress__ColorWidget__
#define __PaintDress__ColorWidget__


#include "cocos2d.h"
#include "BasePenWidget.h"
#include "../Model/AppConfigs.h"
#include "../Model/PaintingManager.h"

class ColorWidget : public BasePenWidget
{
public:
    static ColorWidget* create(const Pen& pen);
    virtual bool init(const Pen& pen);

    Pen& getPen(){return _pen;};
    
protected:
    Pen _pen;
};



#endif /* defined(__PaintDress__ColorWidget__) */
