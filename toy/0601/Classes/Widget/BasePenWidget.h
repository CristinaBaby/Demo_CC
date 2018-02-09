//
//  BasePenWidget.h
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#ifndef __PaintDress__BasePenWidget__
#define __PaintDress__BasePenWidget__


#include "cocos2d.h"
#include "../Model/AppConfigs.h"
#include "../Model/PaintingManager.h"

class BasePenWidget : public cocos2d::LayerColor
{
public:
    virtual bool init() override;
    
    void changeToSelectStyle(){_selectBg->setVisible(true);};
    void changeToUnSelectStyle(){_selectBg->setVisible(false);};
    
protected:
    cocos2d::Sprite *_selectBg;
};

#endif /* defined(__PaintDress__BasePenWidget__) */
