//
//  ShareFunctionManager
//  MysteryGuardians
//
//  Created by liyang on 13-7-22.
//
//

#import <Foundation/Foundation.h>
#import <MessageUI/MessageUI.h>
#include "cocos2d.h"

@interface ShareFunctionManager : NSObject<MFMailComposeViewControllerDelegate>
{
}

+(ShareFunctionManager*)sharedMyManager;
+(void)purgeShareManager;



-(void)goImageES;


@end
