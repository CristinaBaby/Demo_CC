//
//  RateTool.m
//  Dentist
//
//  Created by fushaobing on 13-7-8.
//  Copyright (c) 2013年 fushaobing. All rights reserved.
//

#import "RateTool.h"

//#define RATE_MESSAGE                @"If you like Celebrity Dentist, rate our game!"

#define KEY_RATED                   @"Rated"

@interface RateAlertView : UIAlertView  <UIAlertViewDelegate>

@property (nonatomic, retain)NSString *URL;

@end

@implementation RateAlertView

- (void)dealloc {
    [_URL release];
    [super dealloc];
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    if (buttonIndex == 1) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:_URL]];
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:KEY_RATED];
        [[NSUserDefaults standardUserDefaults] synchronize];
    }
}

@end

@implementation RateTool

+ (void)rateForAppleID:(NSString *)ID alertMessage:(NSString *)alertMessage {
    if ([[NSUserDefaults standardUserDefaults] boolForKey:KEY_RATED]) return;    
    RateAlertView *alertView = [[[RateAlertView alloc] initWithTitle:@"Rate Us" message:alertMessage delegate:nil cancelButtonTitle:nil otherButtonTitles:@"No Thanks", @"Rate Now", nil] autorelease];
    alertView.delegate = alertView;
    [alertView show];
    if([UIDevice currentDevice].systemVersion.floatValue<6.9){
        alertView.URL = [NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%@", ID];
    }else{
        alertView.URL = [NSString stringWithFormat:@"http://itunes.apple.com/app/id%@", ID];
    }
}

+ (BOOL)hasRated
{
    return [[NSUserDefaults standardUserDefaults] boolForKey:KEY_RATED];
}

@end
