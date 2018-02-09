//
//  PrivacyPage.m
//  Dentist
//
//  Created by tanbo on 9/3/13.
//  Copyright (c) 2013 fushaobing. All rights reserved.
//

#import "PrivacyPage.h"

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
        self.backgroundColor = [UIColor whiteColor];
        
        UINavigationBar *navigationBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, 0, rect.size.width, kBarHeight)];
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
        
//        http://www.bluegiantinc.com/privacy_in_game.html
        
        NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
        NSString *companyName = [[bundleID componentsSeparatedByString:@"."] objectAtIndex:1];
        NSURL *_url = [NSURL URLWithString:[NSString stringWithFormat:@"http://www.%@.com/privacy_in_game.html",companyName]];
//
//        NSURL *_url = [NSURL URLWithString:@"http://www.bluegiantinc.com/privacy_in_game.html"];
        // init content view
        
		_contentView = [[UIWebView alloc] initWithFrame:CGRectMake(0, kBarHeight, rect.size.width, rect.size.height - kBarHeight)];
        _contentView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
		_contentView.delegate = self;
		_contentView.backgroundColor = [UIColor clearColor];
		[self addSubview:_contentView];
        //_contentView.opaque = isOpacity?NO:YES;
        
        _indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
        _indicator.autoresizingMask = UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleRightMargin;
        _indicator.hidesWhenStopped = YES;
        _indicator.center = CGPointMake(self.bounds.size.width/2, self.bounds.size.height/2);
        [self addSubview:_indicator];
        
        [self startWithURL:_url];
    }
    return self;
}

- (void)showInView:(UIView *)view
{
    if (!self.superview) {
        [view addSubview:self];
        [view bringSubviewToFront:self];
    }
    
    CGRect frame = self.frame;
    frame.origin.y = 0;
    
    [UIView animateWithDuration:0.5f animations:^{
        self.frame = frame;
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
	if (UIInterfaceOrientationIsLandscape(orit)) {
		rect = CGRectMake(0, rect.size.width, rect.size.height, rect.size.width);
	}
    rect.origin.y = CGRectGetHeight(rect);
	return rect;
}

#pragma mark - UIWebViewDelegate Method

- (void)webViewDidFinishLoad:(UIWebView *)webView{
    [_indicator stopAnimating];
}

- (void)dismiss{
    CGRect rect = self.frame;
    rect.origin.y = rect.size.height;
    [UIView animateWithDuration:0.5f animations:^{
        self.frame = rect;
    } completion:^(BOOL finished) {
        [self removeFromSuperview];
    }];
}

@end
