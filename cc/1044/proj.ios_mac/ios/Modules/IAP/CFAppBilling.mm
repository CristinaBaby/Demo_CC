//
//  CFAppBilling.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-15.
//
//

#include "CFAppBilling.h"
#include "IAPBillingManager_IOS.h"

CFAppBilling::CFAppBilling()
{
}

CFAppBilling::~CFAppBilling()
{
}

void CFAppBilling::setIABDelegate(CFAppBillingDelegate* pDelegate)
{
    [[IOSIAPManager sharedManager] setSTIABDelegate:pDelegate];
}

void CFAppBilling::purchase(const char *pid)
{
    [[IOSIAPManager sharedManager]purchaseIAPWithPID:[NSString stringWithCString:pid encoding:NSUTF8StringEncoding]];
}

void CFAppBilling::purchaseUnmanaged(const char *pid)
{
    // on IOS platform, this method the same as purchase(const char *)
    purchase(pid);
}

void CFAppBilling::restore()
{
    [[IOSIAPManager sharedManager] restoreIAP];
}

void CFAppBilling::query()
{
    /* Only For Android platform */
}