

#include "ShopAdapter.h"
#include "SSCIAPManager.h"
#include "ConfigManager.h"
#include "IAPManager.h"
#include "cocos2d.h"
#include "AdsManager.h"

USING_NS_CC;

static int gIndex = -1;
int getAndroidPlatformCode()
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "com.blackbeltclown.slushy.AppActivity",
                                                 "getAndroidPlatformCode",
                                                 "()I");
    if (!isHave)
    {
        log("jni:getAndroidPlatformCode not exist!");
    }
    else
    {
        jint platformCode;
        platformCode = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
        
        return (int)platformCode;
    }
    
    return -1;
}

void ShopAdapter::registerAndroidIAP()
{
    int plateformFlag = getAndroidPlatformCode();
    log("plateformFlag  %d",plateformFlag);
//    google
    if (plateformFlag==32) {
        SSCIAPManager::getInstance()->initIAP("config/IAP_G.csv");
    }else if (plateformFlag==33){
//        amazon
        SSCIAPManager::getInstance()->initIAP("config/IAP_A.csv");
    }
    SSCIAPManager::getInstance()->setAfterPurchaseCallback([=](bool success){
        if(success){
            buyItemSuccess(gIndex);
        }
    });
    SSCIAPManager::getInstance()->setAfterRestoreCallback([=](bool suc , bool flag){
        
        if (suc)
        {
            for (int i = 1; i<=3; i++) {
                if (SSCIAPManager::getInstance()->isPackagePurchased(i))
                {
                    ShopAdapter::getInstance()->buyItemSuccess(i);
                }
            }
            if (flag)
            {
                MessageBox("Your content has been restored!", "");
//                MessageBox("Restore success!", "");
            }else{
                
                MessageBox("Sorry! It looks like you haven't purchased anything yet!", "");            }
        }else{
            MessageBox("Sorry, restore transaction failed", "");
        }
    });
}
void ShopAdapter::requestBuyItem(int index)
{
    if(IAPManager::getInstance()->getItemIsBought(index-1)){
        MessageBox("You have already purchased this package!", "");
    }else{
        gIndex = index;
        SSCIAPManager::getInstance()->purchase(index);
    }
}

void ShopAdapter::buyItemSuccess(int index)
{
    IAPManager::getInstance()->setItemBoughtWithIndex(index-1);
    ConfigManager::getInstance()->unlock(index-1);
    __NotificationCenter::getInstance()->postNotification(ItemBuySuccessNotify);
}

void ShopAdapter::buyItemSuccess(std::string key)
{
    IAPManager::getInstance()->setItemBoughtWithID(key);
    ConfigManager::getInstance()->unlock(key);
    __NotificationCenter::getInstance()->postNotification(ItemBuySuccessNotify);
}

//先检测是否已购买，没有再发出请求。
void ShopAdapter::requestBuyItem(std::string key)
{
}

void ShopAdapter::requestRestore()
{
    SSCIAPManager::getInstance()->restore();
}