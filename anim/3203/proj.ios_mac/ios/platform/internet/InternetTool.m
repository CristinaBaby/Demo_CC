//
//  InternetTool.m
//  MAKE-JJ06
//
//  Created by fushaobing on 13-3-27.
//  Copyright (c) 2013年 fushaobing. All rights reserved.
//

#import "InternetTool.h"

@implementation InternetTool

+ (BOOL) isConnectionAvailable
{
    SCNetworkReachabilityFlags flags;
    BOOL receivedFlags;
    
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(CFAllocatorGetDefault(), [@"google.com" UTF8String]);
    receivedFlags = SCNetworkReachabilityGetFlags(reachability, &flags);
    CFRelease(reachability);
    
    if (!receivedFlags || (flags == 0) )
    {
        return FALSE;
    } else {
        return TRUE;
    }
}

+ (BOOL)isInternetAvailable {
    if (![InternetTool isConnectionAvailable]) {
        [[[[UIAlertView alloc] initWithTitle:nil message:@"Sorry, there is no internet! " delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil] autorelease] show];
        return NO;
    }
    return YES;
}

@end
