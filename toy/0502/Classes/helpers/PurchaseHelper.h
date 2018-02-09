//
//  PurchaseHelper.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#ifndef __LVUP002__PurchaseHelper__
#define __LVUP002__PurchaseHelper__

#include <cocos2d.h>
#include "../modules/STInAppBilling.h"


class PurchaseHelper : public cocos2d::Ref, public STIABDelegate
{
    PurchaseHelper();
    STIABDelegate   *_delegate;
    
public:
    static PurchaseHelper* getInstance();
    static void destroy();

    static bool isPurchased;
    static bool isAdPurchased;
    
public:
    void setDelegate(STIABDelegate*);
    
    void purchaseSuccessful(const char* pid);
    void purchaseFailed(const char* pid, int errorCode);
    void restoreSuccessful(const char* pid);
    void restoreFailed(const char* pid, int errorCode);
    
private:
    void purchasing(const char* pid);
};

#endif /* defined(__LVUP002__PurchaseHelper__) */
