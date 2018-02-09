//
//  ShopImplIos.m
//  Doctor2
//
//  Created by tanshoumei on 13-9-16.
//
//

#import "ShopImplIos.h"
#import "IAPurchase.h"
#import "InternetTool.h"
#import "IAPManager.h"
#include "AppController.h"
#include "ShopAdapter.h"

static ShopImplIos* _sharedShopImplIos = NULL;

@implementation ShopImplIos

+ (ShopImplIos*) sharedShopImplIos
{
    if(!_sharedShopImplIos)
        _sharedShopImplIos = [[ShopImplIos alloc] init];
    return _sharedShopImplIos;
}

- (void) requestBuy:(NSString *)key
{
    if ([InternetTool isInternetAvailable]) {
        [[IAPurchase sharedPurchase] setDelegate:self];
        [[IAPurchase sharedPurchase] startRequestWithProductIdentifier:key];
    }
}

- (void)requestRestore
{
//    if ([InternetTool isInternetAvailable]) {
//        //检测是否已经恢复过了..避免重复请求
//        NSString * isRestored = [[NSUserDefaults standardUserDefaults] valueForKey:@"hasRestored"];
//        if([isRestored isEqualToString:@"YES"]){
//            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:nil message:@"Your content has already been purchased!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
//            [alertView show];
//            [alertView release];
//            return;
//        }        
        [[IAPurchase sharedPurchase] setDelegate:self];
        [[IAPurchase sharedPurchase] restorePurchase];
//    }
}

#pragma mark IPA协议
- (void)alertMessage:(NSString *)message {
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:message delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

- (void)purchaseSuccess:(IAPurchase *)purchase {
    if(!purchase.curProductID)
        return;
    std::string key = [purchase.curProductID cStringUsingEncoding:NSUTF8StringEncoding];
    [self performSelector:@selector(alertMessage:) withObject:@"Thank you for your purchase." afterDelay:0.2f];
    ShopAdapter::getInstance()->buyItemSuccess(key);
}

- (void)purchaseFailed:(IAPurchase *)purchase {
    [self alertMessage:@"Purchase failed."];
}

- (void)purchaseCanceled:(IAPurchase *)purchase {
    [self purchaseFailed:purchase];
}

- (void)productsNotReady:(IAPurchase *)purchase {
    UIAlertView *alertView=[[UIAlertView alloc] initWithTitle:@"Coming Soon!" message:@"There's nothing here yet but check back here after an upgrade in the near future!" delegate:nil cancelButtonTitle:@"Thanks" otherButtonTitles:nil];
    [alertView show];
    [alertView release];
}

/*
 开始请求购买，并弹出菊花等待圈。
 **/
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
//请求完成后，让菊花圈消失吧.
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
}

//请求restore失败，给出提示
- (void)restoreFailed:(IAPurchase *)purchase {
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"" message:@"Sorry, restore transaction failed!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

//restore成功
- (void)purchaseRestored:(IAPurchase *)purchase
{
    if(!purchase.curProductID)
        return;
    std::string key = [purchase.curProductID cStringUsingEncoding:NSUTF8StringEncoding];
    ShopAdapter::getInstance()->buyItemSuccess(key);
    //提示用户
    static BOOL flag = YES;
    if (flag) {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:nil message:@"Your content has been restored!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alertView show];
        [alertView release];
        //保存配置
        [[NSUserDefaults standardUserDefaults] setObject:@"YES" forKey:@"hasRestored"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        flag = NO;
    }
}

- (void)restoreCompletedWithProductIdentifiers:(NSArray *)productIdentifiers
{
    if (productIdentifiers.count <= 0) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"" message:@"Sorry! It looks like you haven't purchased anything yet." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alert show];
        [alert release];
        return;
    }
    for(int i = 0; i < productIdentifiers.count; ++i)
    {
        std::string key = [[productIdentifiers objectAtIndex:i] cStringUsingEncoding:NSUTF8StringEncoding];
        NSLog(@"------------restoreCompletedWithProductIdentifiers key : %@",[productIdentifiers objectAtIndex:i]);
        ShopAdapter::getInstance()->buyItemSuccess(key);
    }
    
    
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:nil message:@"Your content has been restored!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alertView show];
    [alertView release];
    //保存配置
//    [[NSUserDefaults standardUserDefaults] setObject:@"YES" forKey:@"hasRestored"];
//    [[NSUserDefaults standardUserDefaults] synchronize];
    
    //提示用户
//    static BOOL flag = YES;
//    if (flag) {
//        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:nil message:@"Your content has been restored!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
//        [alertView show];
//        [alertView release];
//        //保存配置
//        [[NSUserDefaults standardUserDefaults] setObject:@"YES" forKey:@"hasRestored"];
//        [[NSUserDefaults standardUserDefaults] synchronize];
//        flag = NO;
//    }
}

- (void)restoreFinishedWithPaymentTransitions:(NSArray*)transitions {
    if (transitions.count <= 0) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"" message:@"Sorry! It looks like you haven't purchased anything yet." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alert show];
        [alert release];
    }
}


@end
