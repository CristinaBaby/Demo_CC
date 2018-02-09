//
//  BaseIconWidget.h
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#ifndef __PaintDress__BaseIconWidget__
#define __PaintDress__BaseIconWidget__

#include "cocos2d.h"
#include "../Model/AppConfigs.h"
#include "../Model/ResManager.h"

class BaseIconWidget : public cocos2d::LayerColor
{
public:
    virtual bool init(Res* res);

    virtual void changeToSelectStyle();
    virtual void changeToUnSelectStyle();
    
    void showNewLogo(){_newLogo->setVisible(true);};
    void hideNewLogo(){_newLogo->setVisible(false);};
    
    Res* getRes(){return _res;};
    
protected:
    cocos2d::Sprite *_newLogo;
    cocos2d::Sprite *_iconBg;
    
    Res *_res;
};


#endif /* defined(__PaintDress__BaseIconWidget__) */
