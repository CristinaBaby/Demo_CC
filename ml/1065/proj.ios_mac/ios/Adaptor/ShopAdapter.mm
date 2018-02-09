//
//  ShopAdapter.cpp
//  Doctor
//
//  Created by tanshoumei on 13-8-20.
//
//

#include "ShopAdapter.h"
#include "cocos2d.h"
#include "NotificationDef.h"
#include "IAPManager.h"
#include "ShopImplIos.h"
#include "ConfigManager.h"

USING_NS_CC;

void ShopAdapter::buyItemSuccess(std::string key)
{
    IAPManager::getInstance()->setItemBoughtWithID(key);
    ConfigManager::getInstance()->unlock(key);
    __NotificationCenter::getInstance()->postNotification(ItemBuySuccessNotify);
}

//先检测是否已购买，没有再发出请求。
void ShopAdapter::requestBuyItem(std::string key)
{
    
////**************测试
//    buyItemSuccess(key);
//    return;
////****************************

    if(IAPManager::getInstance()->getItemIsBought(key)){
        //已经买过
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"You have already purchased this package!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alert show];
        [alert release];
    }else{
        
#if TARGET_IPHONE_SIMULATOR || COCOS2D_DEBUG
        buyItemSuccess(key);
#else
        NSString* sId = [NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding];
        [[ShopImplIos sharedShopImplIos] requestBuy:sId];
#endif
    }
}

void ShopAdapter::requestRestore()
{
    [[ShopImplIos sharedShopImplIos] requestRestore];
}