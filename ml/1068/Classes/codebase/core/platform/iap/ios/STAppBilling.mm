//
//  STAppBilling.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-15.
//
//

#include "STAppBilling.h"
#include "IAPBillingManager_IOS.h"

STAppBilling::STAppBilling()
{
}

STAppBilling::~STAppBilling()
{
}

void STAppBilling::setIABDelegate(STAppBillingDelegate* pDelegate)
{
    [[IOSIAPManager sharedManager] setSTIABDelegate:pDelegate];
}

void STAppBilling::purchase(const char *pid)
{
    [[IOSIAPManager sharedManager]purchaseIAPWithPID:[NSString stringWithCString:pid encoding:NSUTF8StringEncoding]];
}

void STAppBilling::purchaseUnmanaged(const char *pid)
{
    // on IOS platform, this method the same as purchase(const char *)
    purchase(pid);
}

void STAppBilling::restore()
{
    [[IOSIAPManager sharedManager] restoreIAP];
}

void STAppBilling::query()
{
    /* Only For Android platform */
}