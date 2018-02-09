//
//  IOSIAPWarapper.cpp
//  ShopDemo
//
//  Created by 吴有全 on 13-7-11.
//
//

#include "IAPBillingManager_IOS.h"

#import "AppController.h"

static IOSIAPManager *s_instance=nil;

@implementation IOSIAPManager

+(IOSIAPManager*)sharedManager
{
    if(s_instance==nil){
        s_instance=[[IOSIAPManager alloc] init];
    }
    return s_instance;
}

+(void)purgeManager
{
    if (s_instance) {
        [s_instance release],s_instance=nil;
    }
}

-(void)dealloc
{
    [[IAPurchase sharedPurchase] setDelegate:nil];
    ccDelegate_=0;
    s_instance=0;
    [super dealloc];
}

-(void)setSTIABDelegate:(STAppBillingDelegate*)delegate
{
    ccDelegate_=delegate;
    isPurchasing = NO;
}

-(void)purchaseIAPWithPID:(NSString*)pid
{
    if (!isPurchasing)
    {
        isPurchasing = YES;
        
        //调用公司的公共库代码去购买
        [[IAPurchase sharedPurchase] setDelegate:self];
        [[IAPurchase sharedPurchase] startRequestWithProductIdentifier:pid];
    }
}

-(void)restoreIAP
{
    if (!isPurchasing)
    {
        _showRestore = FALSE;
        isPurchasing = YES;
        
        //调用公司的公共库代码去restore
        [[IAPurchase sharedPurchase] setDelegate:self];
        [[IAPurchase sharedPurchase] restorePurchase];
    }
}

- (void)alertMessage:(NSString *)message
{
    
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:message delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

#pragma mark 实现公司的组件代理
- (void)purchaseSuccess:(IAPurchase *)purchase
{
    // alert dialog
    [self alertMessage:@"Thank you for your purchase."];
    
    //通知公共
    if (ccDelegate_)
    {
        ccDelegate_->purchaseSuccessful([purchase.curProductID UTF8String]);
    }
}

- (void)purchaseFailed:(IAPurchase *)purchase
{
    // alert dialog
    [self alertMessage:@"Purchase failed."];
    
    //
    if (ccDelegate_) {
        ccDelegate_->purchaseFailed([purchase.curProductID UTF8String], 0);
    }
}

- (void)restoreFinishedWithPaymentTransitions:(NSArray*)transitions
{
    if (transitions.count == 0) {
      [self alertMessage:@"We could not find any previous purchases. No purchases were restored."];
    }
}

- (void)purchaseRestored:(IAPurchase *)purchase
{
    // alert dialog
    if (!_showRestore) {
        [self alertMessage:@"Restore successfully!"];
        _showRestore = TRUE;
    }
    
    NSLog(@"[purchase.curProductID UTF8String] %s", [purchase.curProductID UTF8String]);
    if (ccDelegate_)
    {
        ccDelegate_->restoreSuccessful([purchase.curProductID UTF8String]);
    }
}
- (void)restoreCompletedWithProductIdentifiers:(NSArray *)productIdentifiers
{
    if (ccDelegate_)
    {
        for (int i=0; i<[productIdentifiers count]; i++) {
            NSString* productKey = [productIdentifiers objectAtIndex:i];
            
            ccDelegate_->restoreSuccessful([productKey UTF8String]);
        }
        if([productIdentifiers count]==0){
            [self alertMessage:@"We could not find any previous purchases. No purchases were restored."];
            ccDelegate_->restoreSuccessfulNotify(false);
        }else{
            ccDelegate_->restoreSuccessfulNotify(true);
        }
        
    }
    
}

- (void)restoreFailed:(IAPurchase *)purchase
{
    // alert dialog
    [self alertMessage:@"We have detected a problem restoring your purchases. No purchases were restored. Please check your device settings and storage and try again later.."];
    
    if (ccDelegate_)
    {
        ccDelegate_->restoreFailed([purchase.curProductID UTF8String], 0);
    }
}

- (void)purchaseCanceled:(IAPurchase *)purchase
{
    // alert dialog
    [self alertMessage:@"Your purchase was canceled. No purchase was made and your account was not charged."];
    if (ccDelegate_)
    {
        ccDelegate_->purchaseFailed([purchase.curProductID UTF8String], 0);
    }
}

- (void)productRequestBegin:(IAPurchase *)purchase
{
    UIWindow *curWindow=[[[UIApplication sharedApplication] windows] objectAtIndex:0];
    CGRect screenRect=[UIScreen mainScreen].applicationFrame;
    UIView *bgView=[[UIView alloc] initWithFrame:CGRectMake(0, 0,  screenRect.size.width,  screenRect.size.height)];
    [bgView setBackgroundColor:[UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:0.5]];
    [bgView setTag:100000];
    UIActivityIndicatorView *acView=[[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    acView.center=CGPointMake(screenRect.size.width/2.0f, screenRect.size.height/2.0f);
    [acView setTag:200000];
    [bgView addSubview:acView];
    [acView release];
    [curWindow addSubview:bgView];
    [curWindow bringSubviewToFront:bgView];
    [bgView release];
    [acView startAnimating];
}

- (void)productRequestEnd:(IAPurchase *)purchase
{
    UIWindow *curWindow=[[[UIApplication sharedApplication] windows] objectAtIndex:0];
    UIView *bgView=[curWindow viewWithTag:100000];
    UIActivityIndicatorView *acView=(UIActivityIndicatorView*)[bgView viewWithTag:200000];
    if (acView) {
        [acView stopAnimating];
        [acView removeFromSuperview];
    }
    if (bgView) {
        [bgView removeFromSuperview];
    }
    
    //[[IAPurchase sharedPurchase] setDelegate:nil];
    
    isPurchasing = NO;
}

- (void)productsNotReady:(IAPurchase *)purchase
{
    // alert dialog
    [self alertMessage:@"Purchase not ready."];
}

@end