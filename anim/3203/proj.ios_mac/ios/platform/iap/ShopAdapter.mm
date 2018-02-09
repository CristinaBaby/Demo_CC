//
//  ShopAdapter.cpp
//  Doctor
//
//  Created by tanshoumei on 13-8-20.
//
//

#include "ShopAdapter.h"
#include "AppController.h"
#include "cocos2d.h"
#include "IAPManager.h"
#include "ShopImplIos.h"

USING_NS_CC;

void ShopAdapter::buyItemSuccess(std::string key)
{
    IAPManager::getInstance()->setItemBoughtWithID(key);
//    CCNotificationCenter::getInstance()->postNotification("ShopBuySuccess");
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ShopBuySuccess");
}

//先检测是否已购买，没有再发出请求。
void ShopAdapter::requestBuyItem(std::string key)
{
    printf("====%s", key.c_str());
#if TARGET_IPHONE_SIMULATOR == 1
    buyItemSuccess(key);
    return;
#endif
    if(IAPManager::getInstance()->getItemIsBought(key)){
        //已经买过
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:NSLocalizedString(@"SC1030", nil) delegate:nil cancelButtonTitle:NSLocalizedString(@"SC1009", nil) otherButtonTitles:nil];
        [alert show];
        [alert release];
    }else{
        NSString* sId = [NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding];
        [[ShopImplIos sharedShopImplIos] requestBuy:sId];
    }
}

void ShopAdapter::requestRestore()
{
    [[ShopImplIos sharedShopImplIos] requestRestore];
}