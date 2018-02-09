//
//  InternetTool.h
//  MAKE-JJ06
//
//  Created by fushaobing on 13-3-27.
//  Copyright (c) 2013年 fushaobing. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <SystemConfiguration/SystemConfiguration.h>

@interface InternetTool : NSObject

+ (BOOL)isInternetAvailable;

+ (BOOL)isConnectionAvailable;

@end
