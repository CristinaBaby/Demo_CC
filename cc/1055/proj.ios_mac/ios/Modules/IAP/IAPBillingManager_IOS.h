//
//  IOSIAPWrapper.h
//
//  Created by 吴有全 on 13-7-11.
//
//

#import <Foundation/Foundation.h>


    //公司的公共库文件
#import "IAPurchase.h"
    //
#include "CFAppBilling.h"

@interface IOSIAPManager : NSObject<IAPurchaseDelegate>
{
    CFAppBillingDelegate* ccDelegate_;
    
    // this is running purchase flow, can't respons any game event.
    BOOL isPurchasing;
    bool  _showRestore;
}
+(IOSIAPManager*)sharedManager;
+(void)purgeManager;

-(void)setSTIABDelegate:(CFAppBillingDelegate*)delegate;
-(void)purchaseIAPWithPID:(NSString*)pid;
-(void)restoreIAP;

@end

