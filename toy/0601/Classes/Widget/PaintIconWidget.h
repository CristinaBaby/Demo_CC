//
//  PaintIconWidget.h
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#ifndef __PaintDress__PaintIconWidget__
#define __PaintDress__PaintIconWidget__

#include "cocos2d.h"
#include "cocosGUI.h"
#include "BaseIconWidget.h"
#include "../Model/AppConfigs.h"
#include "../Model/ResManager.h"

class PaintIconWidget : public BaseIconWidget
{
public:
    static PaintIconWidget* create(Res* res);
    virtual bool init(Res* res);
    virtual void changeToUnSelectStyle() override;
    PaintIconWidget():_isBuyStyle(false){};
    
    bool isBuyStyle(){return _isBuyStyle;};
    
    void changeToBuyStyle();
    void changeToPriceStyle();
    
    void showPriceButton(){_priceButton->setVisible(true);};
    void hidePriceButton(){_priceButton->setVisible(false);};
    
    void showNewLogo(){_newLogo->setVisible(true);};
    void hideNewLogo(){_newLogo->setVisible(false);};
    
protected:
    cocos2d::Label *_priceLabel;
    cocos2d::Sprite *_priceButton;
    bool _isBuyStyle;
};


#endif /* defined(__PaintDress__PaintIconWidget__) */
