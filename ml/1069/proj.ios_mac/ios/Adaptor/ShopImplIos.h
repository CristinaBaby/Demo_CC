//
//  ShopImplIos.h
//  Doctor2
//
//  Created by tanshoumei on 13-9-16.
//
//

#import <Foundation/Foundation.h>

@interface ShopImplIos : NSObject

/*
 请求购买
 key: iap id.
 **/
- (void) requestBuy: (NSString*) key;
/*
 请求restore.
 **/
- (void) requestRestore;

+ (ShopImplIos*) sharedShopImplIos;

@end
