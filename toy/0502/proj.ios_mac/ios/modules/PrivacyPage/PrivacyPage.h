//
//  PrivacyPage.h
//
//
//  Created by tanbo on 9/3/13.
//  Copyright (c) 2013 . All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol PrivacyDelegate <NSObject>

@optional

- (void)privacyPageWillAppear;

- (void)privacyPageDidDismiss;

@end

@interface PrivacyPage : UIView <UIWebViewDelegate>
{
    UINavigationBar *navigationBar;
    UIWebView *_contentView;
    UIActivityIndicatorView *_indicator;
}

@property(nonatomic,assign) id<PrivacyDelegate> delegate;

+ (PrivacyPage *)privacyPage;

- (void)show;

- (void)showInView:(UIView*)view;

@end
