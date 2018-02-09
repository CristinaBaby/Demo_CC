//
//  VersionChecker.h
//  VersionDectorDemo
//
//  Created by tang.wei on 13-10-9.
//  Copyright (c) 2013年 tang.wei. All rights reserved.
//

#import <Foundation/Foundation.h>

@class VersionChecker;
@protocol VersionCheckerDelegate <NSObject>

@optional
- (void)versionCheckerDidShow:(VersionChecker *)versionChecker;
- (void)versionCheckerDidNotShow:(VersionChecker *)versionChecker;
- (void)versionCheckerDidDismiss:(VersionChecker *)versionChecker;

@end

@interface VersionChecker : NSObject

@property (nonatomic, assign) id delegate;

// 游戏resume时，是否进行版本检查，默认值为NO
@property (nonatomic, assign) BOOL allowCheckWhenAppResume;

// 稍后提醒的天数, 默认值为1天
@property (nonatomic, assign) NSUInteger remindDays;

+ (VersionChecker *)sharedVersionChecker;

// 开始检查版本
- (void)startChecking;
- (void)startChecking:(id)delegate;

@end
