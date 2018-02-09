//
//  PurchaseHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#include "PurchaseHelper.h"
#include "EncryptDataHelper.h"
#include "../AppConfig.h"
#include "../AppGlobal.h"


USING_NS_CC;


bool PurchaseHelper::isPurchased = false;
bool PurchaseHelper::isAdPurchased = false;


static PurchaseHelper* s_m_PurchaseHelper = NULL;

PurchaseHelper* PurchaseHelper::getInstance()
{
    if (NULL == s_m_PurchaseHelper)
    {
        s_m_PurchaseHelper = new PurchaseHelper();
    }
    
    return s_m_PurchaseHelper;
}

void PurchaseHelper::destroy()
{
    CC_SAFE_DELETE(s_m_PurchaseHelper);
}


PurchaseHelper::PurchaseHelper()
: _delegate(NULL)
{
}

void PurchaseHelper::setDelegate(STIABDelegate *delegate)
{
    _delegate = delegate;
}

void PurchaseHelper::purchasing(const char* pid)
{
    if (strcmp(pid, st_iap_sku_unlock_all) == 0)
    {
        // 全部解锁
        isPurchased   = true;
        isAdPurchased = true;
        EncryptDataHelper::setPurchaseFlag(key_iap_purchased_flag, isPurchased);
        EncryptDataHelper::setPurchaseFlag(key_iap_ad_purchased_flag, isAdPurchased);
    }
    else if(strcmp(pid, st_iap_sku_hint_5) == 0)
    {
        // 5个hint道具
        unsigned hintQuantity = EncryptDataHelper::getPorpsQuantity(key_hint_quantity, 3);
        hintQuantity += 5;
        EncryptDataHelper::setPorpsQuantity(key_hint_quantity, hintQuantity);
    }
    else if(strcmp(pid, st_iap_sku_add_time_5) == 0)
    {
        // 5个add time道具
        unsigned quantity = EncryptDataHelper::getPorpsQuantity(key_addtiome_quantity, 1);
        quantity += 5;
        EncryptDataHelper::setPorpsQuantity(key_addtiome_quantity, quantity);
    }
    else if(strcmp(pid, st_iap_sku_freeze_5) == 0)
    {
        // 5个freeze道具
        unsigned quantity = EncryptDataHelper::getPorpsQuantity(key_freeze_quantity, 2);
        quantity += 5;
        EncryptDataHelper::setPorpsQuantity(key_freeze_quantity, quantity);
    }
}


void PurchaseHelper::purchaseSuccessful(const char* pid)
{
    purchasing(pid);
    
    // 做代理，转发事件，上面只处理共同事件
    if (_delegate)
    {
        _delegate->purchaseSuccessful(pid);
    }
}

void PurchaseHelper::purchaseFailed(const char *pid, int errorCode)
{
    if (_delegate)
    {
        _delegate->purchaseFailed(pid, errorCode);
    }
}

void PurchaseHelper::restoreSuccessful(const char* pid)
{
    purchasing(pid);
    
    if (_delegate)
    {
        _delegate->restoreSuccessful(pid);
    }
}

void PurchaseHelper::restoreFailed(const char* pid, int errorCode)
{
    if (_delegate)
    {
        _delegate->restoreFailed(pid, errorCode);
    }
}

