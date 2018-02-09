//
//  VersionChecker.m
//  VersionDectorDemo
//
//  Created by tang.wei on 13-10-9.
//  Copyright (c) 2013年 tang.wei. All rights reserved.
//

#import "VersionChecker.h"
//#import "CJSONDeserializer.h"

#define AppStoreURL(appleID)    [NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=%@", appleID]

NSString *const kVersionCheckerReminderRequestDate = @"kVersonCheckerReminderRequestDate";
NSString *const kVersionCheckerAllowCheck = @"kVersionCheckerAllowCheck";

@interface VersionChecker()

@property (nonatomic, copy) NSString *appId;
@property (nonatomic, retain) NSMutableData *resultData;

@end

@implementation VersionChecker
{
    BOOL _checking;
    BOOL _hasAddedNotification;
}

@synthesize delegate, remindDays, appId, resultData;
@synthesize allowCheckWhenAppResume = _allowCheckWhenAppResume;

static VersionChecker *instance = nil;
+ (VersionChecker *)sharedVersionChecker
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (instance == nil) {
            instance = [[VersionChecker alloc] init];
        }
    });
    return instance;
}

- (id)init
{
    if (self = [super init]) {
        _checking = NO;
        
        _hasAddedNotification = NO;
        _allowCheckWhenAppResume = NO;
        
//        id object = [[NSUserDefaults standardUserDefaults] objectForKey:kVersionCheckerAllowCheck];
//        if (!object) {
//            [self setAllowCheck:YES];
//        }
        
        self.remindDays = 1;
    }
    return self;
}

- (void)startChecking
{
    [self startCheckingRemindDays:1];
}

- (void)startChecking:(id)delegate_
{
    self.delegate = delegate_;
    [self startChecking];
}

- (void)startCheckingRemindDays:(NSUInteger)days
{
    // 如果不再显示版本检查，直接返回
//    if (![self allowCheck]) {
//        [self didNotNeedShowing];
//        return;
//    }
    
    // 如果当前不需要弹出那么直接返回
    if (![self canShowConfirmView]) {
        [self didNotNeedShowing];
        return;
    }
    
    // 如果正在请求网络，直接返回
    if (_checking) {
        return;
    }
    _checking = YES;
    
    [self addNotification];
    
    self.remindDays = days;
    self.resultData = [NSMutableData data];
    
    NSString *bundleId = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"];
    NSURL *url = [NSURL URLWithString:
                  [NSString stringWithFormat:@"https://itunes.apple.com/lookup?bundleId=%@",bundleId]];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    
    [[[NSURLConnection alloc] initWithRequest:request delegate:self] autorelease];
    
    /* iOS5.0 and later
    [NSURLConnection sendAsynchronousRequest:request
                                       queue:[NSOperationQueue mainQueue]
                           completionHandler:^(NSURLResponse *response, NSData *data, NSError *error) {
                               if (!error) {
                                   NSError* parseError = nil;
                                   id appMetadataDictionary =
                                   [[CJSONDeserializer deserializer] deserialize:data error:&parseError];
                                   
                                   if (parseError) {
                                       NSLog(@"Json parse error");
                                       [self didNotNeedShowing];
                                       return;
                                   }
                                   
                                   if (appMetadataDictionary) {
                                       // compare version with your apps local version
                                       id result = [appMetadataDictionary objectForKey:@"results"];
                                       
                                       if ([result count] == 0) {
                                           NSLog(@"No data error.");
                                           [self didNotNeedShowing];
                                       } else {
                                           
                                           NSDictionary *dict = [result objectAtIndex:0];
                                           NSString *iTunesVersion = [dict objectForKey:@"version"];
                                           self.appId = [dict objectForKey:@"trackId"];
                                           NSString *appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString*)@"CFBundleShortVersionString"];
                                           
                                           if (iTunesVersion && [appVersion compare:iTunesVersion] != NSOrderedSame) {
                                               [self showConfirmView];
                                           } else {
                                               [self didNotNeedShowing];
                                           }
                                       }
                                   }
                               } else {
                                   // error occurred with http(s) request
                                   NSLog(@"Version Checking error occurred communicating with iTunes");
                                   [self didNotNeedShowing];
                               }
                           }];*/
}

#pragma mark NSURLConnection delegate Method
- (void)connection:(NSURLConnection*)connection didReceiveData:(NSData*)data
{
    [self.resultData appendData:data];
}

- (void)connectionDidFinishLoading:(NSURLConnection*)connection
{
    NSLog(@"No data error.");
    [self didNotNeedShowing];
    
//    NSError* parseError = nil;
//    id appMetadataDictionary =
//    [[CJSONDeserializer deserializer] deserialize:self.resultData error:&parseError];
//    
//    if (parseError) {
//        NSLog(@"Json parse error");
//        [self didNotNeedShowing];
//        return;
//    }
//    
//    if (appMetadataDictionary) {
//        // compare version with your apps local version
//        id result = [appMetadataDictionary objectForKey:@"results"];
//        
//        if ([result count] == 0) {
//            NSLog(@"No data error.");
//            [self didNotNeedShowing];
//        } else {
//            
//            NSDictionary *dict = [result objectAtIndex:0];
//            NSString *iTunesVersion = [dict objectForKey:@"version"];
//            self.appId = [dict objectForKey:@"trackId"];
//            NSString *appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString*)@"CFBundleShortVersionString"];
//            
//            if (iTunesVersion && [appVersion compare:iTunesVersion] == NSOrderedAscending) {
//                [self showConfirmView];
//            } else {
//                [self didNotNeedShowing];
//            }
//        }
//    }
}

-(void)connection: (NSURLConnection *) connection didFailWithError: (NSError *) error{
    NSLog(@"Version Checking error occurred communicating with iTunes");
    [self didNotNeedShowing];
}

- (BOOL)canShowConfirmView {
    // if the user wanted to be reminded later, has enough time passed?
	NSDate *reminderRequestDate = [NSDate dateWithTimeIntervalSince1970:[[NSUserDefaults standardUserDefaults] doubleForKey:kVersionCheckerReminderRequestDate]];
	NSTimeInterval timeSinceReminderRequest = [[NSDate date] timeIntervalSinceDate:reminderRequestDate];
	NSTimeInterval timeUntilReminder = 60 * 60 * 24 * self.remindDays;
	if (timeSinceReminderRequest < timeUntilReminder)
		return NO;
    
    return YES;
}

- (void)showConfirmView {
    
    if ([self canShowConfirmView]) {
        //  popup
        NSString *displayName = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleNameKey];
        NSString *message = [NSString stringWithFormat:NSLocalizedStringFromTable(@"A new version is available for download.", @"VersionChecker", nil), displayName];
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:
                                  NSLocalizedStringFromTable(@"New Version Available", @"VersionChecker", nil)
                                                            message:message
                                                           delegate:self
                                                  cancelButtonTitle:NSLocalizedStringFromTable(@"Cancel", @"VersionChecker", nil)
                                                  otherButtonTitles:
                                  NSLocalizedStringFromTable(@"Download", @"VersionChecker", nil),nil];
        [alertView show];
        [alertView release];
        
        //  success delegate
        if ([self.delegate respondsToSelector:@selector(versionCheckerDidShow:)]) {
            [self.delegate versionCheckerDidShow:self];
        }
        
    }else {
        [self didNotNeedShowing];
    }
}

#pragma mark - UIAlertViewDelegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    
    if ( buttonIndex == 1 ) {
        [self performSelector:@selector(forwardToUpdate) withObject:nil afterDelay:.1];
    } else if ( buttonIndex == 0 ) {
        // remind later
        [self remindMeLater];
    }
    
    _checking = NO;
    if ([self.delegate respondsToSelector:@selector(versionCheckerDidDismiss:)]) {
        [self.delegate versionCheckerDidDismiss:self];
    }
}

- (void)setAllowCheck:(BOOL)yesOrNo
{
    if (!yesOrNo) {
        [self removeNotification];
        _allowCheckWhenAppResume = NO;
    }
    
    [[NSUserDefaults standardUserDefaults] setBool:yesOrNo forKey:kVersionCheckerAllowCheck];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (BOOL)allowCheck
{
    return [[NSUserDefaults standardUserDefaults] boolForKey:kVersionCheckerAllowCheck];
}

- (void)forwardToUpdate
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:AppStoreURL(self.appId)]];
}

- (void)remindMeLater
{
    [[NSUserDefaults standardUserDefaults] setDouble:[[NSDate date] timeIntervalSince1970]
                                              forKey:kVersionCheckerReminderRequestDate];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)addNotification
{
    if (!self.allowCheckWhenAppResume) {
        return;
    }
    
    if (_hasAddedNotification) {
        return;
    }
    _hasAddedNotification = YES;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(startChecking)
                                                 name:UIApplicationWillEnterForegroundNotification
                                               object:nil];
}

- (void)removeNotification
{
    _hasAddedNotification = NO;
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)didNotNeedShowing
{
    _checking = NO;
    if ([self.delegate respondsToSelector:@selector(versionCheckerDidNotShow:)]) {
        [self.delegate versionCheckerDidNotShow:self];
    }
}

@end
