//
//  DressIconWidget.h
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#ifndef __PaintDress__DressIconWidget__
#define __PaintDress__DressIconWidget__

#include "cocos2d.h"
#include "cocosGUI.h"
#include "BaseIconWidget.h"
#include "../Model/AppConfigs.h"
#include "../Model/ResManager.h"

class DressIconWidget : public BaseIconWidget
{
public:
    static DressIconWidget* create(Res* res);
    virtual bool init(Res* res);

    void showAddButton(){_addButton->setVisible(true);};
    void hideAddButton(){_addButton->setVisible(false);};
    
    cocos2d::Sprite* getPaintButton(){return _paintButton;};
    
protected:
    cocos2d::Sprite *_paintButton;
    cocos2d::Sprite *_addButton;
};



#endif /* defined(__PaintDress__DressIconWidget__) */
