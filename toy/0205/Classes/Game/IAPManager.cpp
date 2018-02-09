#include "IAPManager.h"
#include "AppConfigs.h"
#include "AudioHelper.h"

static IAPManager *_sharedIAP = nullptr;

IAPManager* IAPManager::getInstance()
{
    if (! _sharedIAP)
    {
        _sharedIAP = new IAPManager();
    }
    
    return _sharedIAP;
}

void IAPManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedIAP);
}

IAPManager::IAPManager()
{
    _userDefault = UserDefault::getInstance();
    _appBilling.setIABDelegate(this);
    
    //加载3个iap
    vector<string> ids = {iap_1, iap_2};
    
    for (auto id : ids ) {
        string identifier = UserDefaultKey_HasPurchasedPack + id;
        if(_userDefault->getBoolForKey(identifier.c_str(), false))
        {
            _purchased.push_back(id);
        }
    }
}

void IAPManager::purchase(const string& iapid)
{
    _appBilling.purchase(iapid.c_str());
    xAudio->playSound("ui/button_shop_buy.mp3");
}

void IAPManager::restore()
{
    _appBilling.restore();
    xAudio->playSound("ui/button_shop_restore.mp3");
}

void IAPManager::purchaseSuccessful(const char* pid)
{
    string iap = pid;
    
    if (!isPurchased(iap)) {
        _purchased.push_back(iap);
        
        //购买All, 解锁所有的东西
//        if (iap.compare(iap_2) == 0) {
//            purchaseSuccessful(iap_1);
//            purchaseSuccessful(iap_2);
//        }
        
        //写如文件保存, 后面可以来做加密
        string identifier = UserDefaultKey_HasPurchasedPack + iap;
        _userDefault->setBoolForKey(identifier.c_str(), true);
        _userDefault->flush();
    }
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_IAP);
}

void IAPManager::purchaseFailed(const char *pid, int errorCode)
{
}

void IAPManager::restoreSuccessful(const char* pid)
{
    purchaseSuccessful(pid);
}

void IAPManager::restoreFailed(const char* pid, int errorCode)
{
}

bool IAPManager::isPurchased(const string& iapid)
{
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
//    return true;
//#endif
    
    for (auto id : _purchased) {
        if (id.compare(iapid) == 0) {
            return true;
        }
    }

    return false;
}

bool IAPManager::noAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return isPurchased(iap_2);
#else
    return false;
#endif
}
