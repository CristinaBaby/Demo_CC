//
//  PrivacyPage.m
//  
//
//  Created by tanbo on 9/3/13.
//  Copyright (c) 2013 . All rights reserved.
//

#import "PrivacyPage.h"
#import "AppConfigs.h"

#define kBarHeight 44

@implementation PrivacyPage

+ (PrivacyPage *)privacyPage{
    return [[[PrivacyPage alloc] initPrivacyPage] autorelease];
}

- (void)dealloc{
    [_contentView release],_contentView = nil;
    [_indicator release],_indicator = nil;
    [super dealloc];
}

- (PrivacyPage*)initPrivacyPage{
    CGRect rect = [self getCurrentFrame];
    self = [super initWithFrame:rect];
    if (self) {
        
        navigationBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, rect.size.height, rect.size.width, kBarHeight)];
        if ([[UIDevice currentDevice].systemVersion floatValue] >= 5.0) {
            if ([navigationBar respondsToSelector:@selector(setBackgroundImage:forBarMetrics:)]) {
                [navigationBar setBackgroundImage:[UIImage imageNamed:@"navigationbarBg.png"] forBarMetrics:UIBarMetricsDefault];
            }
        } else {
            UIImageView *bgView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"navigationbarBg.png"]];
            bgView.frame = navigationBar.bounds;
            [navigationBar insertSubview:bgView atIndex:0];
            [bgView release];
        }
        UINavigationItem *navigationItem = [[UINavigationItem alloc] init];
        UILabel *titleView = (UILabel *)navigationItem.titleView;
        if (!titleView) {
            titleView = [[UILabel alloc] initWithFrame:CGRectZero];
            titleView.backgroundColor = [UIColor clearColor];
            titleView.font = [UIFont boldSystemFontOfSize:20.0];
            //titleView.shadowColor = [UIColor colorWithWhite:0.0 alpha:0.5];
            titleView.textColor = [UIColor whiteColor]; // Change to desired color
            navigationItem.titleView = titleView;
            [titleView release];
        }
        titleView.text = @"Privacy Policy";
        [titleView sizeToFit];
        
        UIBarButtonItem *rightButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(dismiss)];
        [navigationItem setRightBarButtonItem:rightButton];
        [navigationBar pushNavigationItem:navigationItem animated:NO];
        [self addSubview:navigationBar];
        [navigationBar release];
        [navigationItem release];
        [rightButton release];
        
        
        NSString *urlString = PRIVACY_PAGE_URL;
        if (urlString.length == 0) {
            NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
            NSString *companyName = [[bundleID componentsSeparatedByString:@"."] objectAtIndex:1];
            urlString = [NSString stringWithFormat:@"http://www.%@.com/privacy.html",companyName];
        }
        NSURL *url = [NSURL URLWithString:urlString];
        // init content view
        
		_contentView = [[UIWebView alloc] initWithFrame:CGRectMake(0, kBarHeight + rect.size.height, rect.size.width, rect.size.height - kBarHeight)];
        _contentView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        _contentView.scalesPageToFit = YES;
		_contentView.delegate = self;
		_contentView.backgroundColor = [UIColor clearColor];
		[self addSubview:_contentView];
        //_contentView.opaque = isOpacity?NO:YES;
        
        _indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
        _indicator.autoresizingMask = UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleRightMargin;
        _indicator.hidesWhenStopped = YES;
        _indicator.center = CGPointMake(self.bounds.size.width/2, self.bounds.size.height/2 + rect.size.height);
        [self addSubview:_indicator];
        
        [self startWithURL:url];
    }
    return self;
}

// 显示到window
- (void)show
{
    UIWindow *window = [[UIApplication sharedApplication].windows objectAtIndex:0];
    UIViewController *controller = [window rootViewController];
    if ([controller isKindOfClass:[UINavigationController class]]) {
        controller = [(UINavigationController *)controller visibleViewController];
    }
    [self showInView:controller.view];
}

- (void)showInView:(UIView *)view
{
    if (!self.superview) {
        [view addSubview:self];
        [view bringSubviewToFront:self];
    }
    
    [[UIApplication sharedApplication] beginIgnoringInteractionEvents];
    
    if (_delegate && [_delegate respondsToSelector:@selector(privacyPageWillAppear)]) {
        [_delegate performSelector:@selector(privacyPageWillAppear)];
    }
    
    [UIView animateWithDuration:0.5f animations:^{
        CGRect rect = [self getCurrentFrame];
        navigationBar.center = CGPointMake(navigationBar.center.x, navigationBar.center.y - rect.size.height);
        _contentView.center = CGPointMake(_contentView.center.x, _contentView.center.y - rect.size.height);
        _indicator.center = CGPointMake(_indicator.center.x, _indicator.center.y - rect.size.height);
    } completion:^(BOOL finished) {
        [[UIApplication sharedApplication] endIgnoringInteractionEvents];
        self.backgroundColor = [UIColor whiteColor];
    }];
}

- (void)startWithURL:(NSURL*)url{
    [_contentView loadRequest:[NSURLRequest requestWithURL:url]];
    [_indicator startAnimating];
}

#pragma mark - Utility methods
- (CGRect)getCurrentFrame {
    
	CGRect rect = [UIScreen mainScreen].applicationFrame;
    UIInterfaceOrientation orit = [UIApplication sharedApplication].statusBarOrientation;
	if (UIInterfaceOrientationIsLandscape(orit)&&[UIDevice currentDevice].systemVersion.floatValue<7.9) {
		rect = CGRectMake(0, 0, rect.size.height, rect.size.width);
	}
	return rect;
}

#pragma mark - UIWebViewDelegate Method

- (void)webViewDidFinishLoad:(UIWebView *)webView{
    [_indicator stopAnimating];
}

- (void)dismiss{
    self.backgroundColor = [UIColor clearColor];
//    [UIView animateWithDuration:0.5f animations:^{
        CGRect rect = [self getCurrentFrame];
        navigationBar.center = CGPointMake(navigationBar.center.x, navigationBar.center.y + rect.size.height);
        _contentView.center = CGPointMake(_contentView.center.x, _contentView.center.y + rect.size.height);
        _indicator.center = CGPointMake(_indicator.center.x, _indicator.center.y + rect.size.height);
//    } completion:^(BOOL finished) {
        if (_delegate && [_delegate respondsToSelector:@selector(privacyPageDidDismiss)]) {
            [_delegate performSelector:@selector(privacyPageDidDismiss)];
        }
        [self removeFromSuperview];
//    }];
}

@end
