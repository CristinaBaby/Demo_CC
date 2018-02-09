#include "IAPManager.h"
#include "AppConfigs.h"
#include "AudioHelper.h"
#include "CBPackScene.h"
#include "GameScene.h"
#include "PensManager.h"

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
    
    vector<string> ids;
    for (int i = 3; i < 5; i ++) {
        ids.push_back(xPack->getPackPurchasedIdentifier(i));
    }
    for (int i = 0; i < 6; i ++) {
        ids.push_back(xPens->getParchaseInfoFromIndex(i));
    }
    
    for (auto id : ids ) {
        string identifier = UserDefaultKey_HasPurchasedPack + id;
        if(_userDefault->getBoolForKey(identifier.c_str(), false))
        {
            _purchased.push_back(id);
        }
        _identifiers.push_back(id);
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

void IAPManager::purchaseSuccessful(const char* identifier)
{
    string iap = identifier;
    
    if (!isPurchased(iap)) {
        _purchased.push_back(iap);
        
        //写如文件保存, 后面可以来做加密
        string identifier = UserDefaultKey_HasPurchasedPack + iap;
        _userDefault->setBoolForKey(identifier.c_str(), true);
        _userDefault->flush();
        
        std::string temp(identifier);
        int index = std::stoi(temp.substr(temp.length()- 1, 1));
        
        
        if (xPack->isPackPurchasedIdentifier(iap)) {
            // pack
            _userDefault->setBoolForKey(string(RewardedAds_record_packs + std::to_string(index - 1)).c_str(), true);
            auto scene = Director::getInstance()->getRunningScene();
            if (scene->getName().compare("PackScene") == 0) {
                ((PackScene*)scene)->updatePacks(index - 1);
            }
        }
        else
        {
            //pen
            _userDefault->setBoolForKey(string(RewardedAds_record_pens + std::to_string(index - 1)).c_str(), true);
        }
        
        _userDefault->flush();
    }
    
    Value id(identifier);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_IAP, (void*)&id);
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
    for (auto id : _purchased) {
        if (id.compare(iapid) == 0) {
            return true;
        }
    }

    return false;
}

string IAPManager::getIdentifierForPen(int index)
{
    return _identifiers.at(index + 2);
}
