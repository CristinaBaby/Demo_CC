//
//  ShopLayer.h
//  PaintDress
//
//  Created by maxiang on 6/25/15.
//
//

#ifndef __PaintDress__ShopLayer__
#define __PaintDress__ShopLayer__

#include "cocos2d.h"
#include "cocosGUI.h"
#include "../Model/AppConfigs.h"
#include "../Module/STInAppBilling.h"

class ShopLayer : public cocos2d::LayerColor, public STIABDelegate
{
public:
    CREATE_FUNC(ShopLayer);
    virtual bool init() override;
    virtual ~ShopLayer();

    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid);
    virtual void purchaseFailed(const char *pid, int errorCode);
    virtual void restoreSuccessful(const char* pid);
    virtual void restoreFailed(const char* pid, int errorCode);
    
protected:
    void closeAction();
    void purchase1Action();
    void purchase2Action();
    void purchase3Action();
    void purchase4Action();

    void restoreAction();
    
protected:
    cocos2d::ui::Widget *_ui;
    STInAppBilling _appBilling;
    bool _ispurchasing;
};

#endif /* defined(__PaintDress__ShopLayer__) */
