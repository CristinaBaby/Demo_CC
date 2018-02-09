//
//  RateTool.h
//  Dentist
//
//  Created by fushaobing on 13-7-8.
//  Copyright (c) 2013年 fushaobing. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RateTool : NSObject 

+ (void)rateForAppleID:(NSString *)ID alertMessage:(NSString *)alertMessage;
+ (BOOL)hasRated;

@end
