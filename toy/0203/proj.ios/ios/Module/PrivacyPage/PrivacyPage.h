//
//  PrivacyPage.h
//  Dentist
//
//  Created by tanbo on 9/3/13.
//  Copyright (c) 2013 fushaobing. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PrivacyPage : UIView <UIWebViewDelegate>
{
    UIWebView *_contentView;
    UIActivityIndicatorView *_indicator;
}

+ (PrivacyPage *)privacyPage;

//- (PrivacyPage*)initPrivacyPageWith:(NSString*)url;

- (void)showInView:(UIView*)view;

@end
