//
//  STAppBilling
//
//  Created by Steven.Xc.Tian on 13-8-29.
//
//

#include "STAppBilling.h"
#include "IAPBillingManager_Android.h"

void STAppBilling::setIABDelegate(STAppBillingDelegate* pDelegate)
{
	AndroidIAPManager::getInstance()->setIABDelegate(pDelegate);
}

void STAppBilling::purchase(const char* sku)
{
	AndroidIAPManager::getInstance()->purchase(sku, SKU_TYPE_MANAGE);
}

void STAppBilling::purchaseUnmanaged(const char* sku)
{
	AndroidIAPManager::getInstance()->purchase(sku, SKU_TYPE_UNMANAGE);
}

void STAppBilling::restore()
{
	AndroidIAPManager::getInstance()->restore();
}

void STAppBilling::query()
{
	AndroidIAPManager::getInstance()->query();
}

STAppBilling::STAppBilling()
{
}

STAppBilling::~STAppBilling()
{
}
