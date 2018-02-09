//
//  SystemFunctionIOSWrapper.m
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import <AssetsLibrary/AssetsLibrary.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <CoreFoundation/CoreFoundation.h>
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#import <netinet/in.h>
#import <netinet6/in6.h>
#import <arpa/inet.h>
#import <ifaddrs.h>
#import <netdb.h>
#import "SystemFunctionManager_IOS.h"
#import "AppController.h"
#import "AboutUsPage.h"
#import "RootViewController.h"
#import "NewsBlast.h"


static SystemFunctionManager *s_instance=nil;

@implementation SystemFunctionManager

+(SystemFunctionManager*)sharedManager
{
    if(s_instance==nil){
        s_instance=[[SystemFunctionManager alloc] init];
    }
    return s_instance;
}

+(void)purgeManager
{
    if (s_instance)
    {
        [s_instance release], s_instance=nil;
    }
}

-(void)dealloc
{
    s_instance = 0;
    [super dealloc];
}

-(void)sendEmail:(NSString*)subject Content:(NSString*) content
{
//    NSMutableString *emailBody = [[NSMutableString alloc] initWithString:@"<html><body>"];
//    [emailBody appendString:[NSString stringWithFormat:@"%@%@%@%@%@",
//                             @"<p>Hey!</p>",
//                             @"<p>I am playing this awesome hidden object game - Hidden Objects:  Kitchen from Hell!</p>",
//                             @"<p>I think you will like this also!</p>",
//                             @"<p>Get it NOW!</p>",
//                             @"<p><a href=‘https://itunes.apple.com/app/id694502530’>https://itunes.apple.com/app/id694502530</a></p>"]];
//    
////    [emailBody appendString:content];
//    
//    //NSData *imageData = [NSData dataWithData:UIImagePNGRepresentation(self.shareImage)];
//    [emailBody appendString:@"</body></html>"];
//    Class mailClass = (NSClassFromString(@"MFMailComposeViewController"));
//    if (nil != mailClass) {
//        MFMailComposeViewController* emailDialog = [[MFMailComposeViewController alloc] init];
//        if (nil != emailDialog) {
//            emailDialog.mailComposeDelegate = self;
//            // Set the subject of email
//            //[emailDialog addAttachmentData:imageData mimeType:@"image/png" fileName:@"nails"];
//            [emailDialog setSubject:subject];
//            [emailDialog setMessageBody:emailBody isHTML:YES];
//            
//            RootViewController *vc=[AppController sharedAppController].viewController;
//            
//            if ([vc respondsToSelector:@selector(presentViewController:animated:completion:)]) {
//                [vc presentViewController:emailDialog animated:YES completion:NULL];
//            }else
//            {
//                [vc presentModalViewController:emailDialog animated:YES];
//            }
//            
//            [emailDialog release];
//        }
//    }
//    [emailBody release];
}
-(void)sendEmailWithPic:(NSString*)subject Content:(NSString*) content Path:(NSString*) imagePath
{
    NSMutableString *emailBody = [[NSMutableString alloc] initWithString:@"<html><body>"];
    [emailBody appendString:[NSString stringWithFormat:@"%@%@%@",
                             @"<p>Hey!</p>",
                             content,
                             @"<p><a href=‘https://itunes.apple.com/app/id1068846834’>https://itunes.apple.com/app/id1068846834</a></p>"]];
    
    //    [emailBody appendString:content];
    
    NSData *imageData = [NSData dataWithData:UIImagePNGRepresentation([UIImage imageWithContentsOfFile:imagePath])];
    [emailBody appendString:@"</body></html>"];
    Class mailClass = (NSClassFromString(@"MFMailComposeViewController"));
    if (nil != mailClass) {
        MFMailComposeViewController* emailDialog = [[MFMailComposeViewController alloc] init];
        if (nil != emailDialog) {
            emailDialog.mailComposeDelegate = self;
            // Set the subject of email
            [emailDialog addAttachmentData:imageData mimeType:@"image/png" fileName:@"nails"];
            [emailDialog setSubject:subject];
            [emailDialog setMessageBody:emailBody isHTML:YES];
            
            RootViewController *vc=[AppController sharedAppController].viewController;
            
            if ([vc respondsToSelector:@selector(presentViewController:animated:completion:)]) {
                [vc presentViewController:emailDialog animated:YES completion:NULL];
            }else
            {
//                [vc presentModalViewController:emailDialog animated:YES];
//                presentViewController:animated:completion:
                [vc presentViewController:emailDialog animated:YES completion:^(){}];
            }
            
            [emailDialog release];
        }
    }
    [emailBody release];
}

//-(void)saveToAlbum:(Image*)img withTarget:(Ref*) target withCallFun : (CallFunction)callFun
//{
//    UIImage *lImage = [self CCImageToUIImage:img];
//    ALAssetsLibrary* library = [[ALAssetsLibrary alloc] init];
//    [library writeImageToSavedPhotosAlbum: lImage.CGImage metadata:nil completionBlock:^(NSURL *assetURL, NSError *error2)
//     {
//         if (error2)
//         {
//             (target->*callFun)(false,(void*)[[error2 localizedDescription] UTF8String]);
//         }
//         else
//         {
//             (target->*callFun)(true,(void*)[[error2 localizedDescription] UTF8String]);
//         }
//     }];
//}

-(void)saveToAlbum:(Image*)img withCallback:(std::function<void(bool)>) call
{
    UIImage *lImage = [self CCImageToUIImage:img];
    ALAssetsLibrary* library = [[ALAssetsLibrary alloc] init];
    [library writeImageToSavedPhotosAlbum: lImage.CGImage metadata:nil completionBlock:^(NSURL *assetURL, NSError *error2)
     {
         if (error2)
         {
             call(false);
         }
         else
         {
             call(true);
         }
     }];
}

-(UIImage *)CCImageToUIImage:(Image*)apImg
{
    int bitsPerComponent    =   8;
    int bitsPerPixel        =   32;
    int bytesPerRow         =   (bitsPerPixel/8) * apImg->getWidth();
    int myDataLength        =   bytesPerRow * apImg->getHeight();
    unsigned char *pixels   =   apImg->getData();
    // make data provider with data.
    CGBitmapInfo bitmapInfo         = kCGBitmapByteOrderDefault;
    CGDataProviderRef provider      = CGDataProviderCreateWithData(NULL, pixels, myDataLength, NULL);
    CGColorSpaceRef colorSpaceRef   = CGColorSpaceCreateDeviceRGB();
    CGImageRef iref                 = CGImageCreate(apImg->getWidth(), apImg->getHeight(),
                                                     bitsPerComponent, bitsPerPixel, bytesPerRow,
                                                     colorSpaceRef, bitmapInfo, provider,
                                                     NULL, false,
                                                     kCGRenderingIntentDefault);
    UIImage* image                   = [[[UIImage alloc] initWithCGImage:iref] autorelease];
    CGImageRelease(iref);
    
    return image;
}


/*--------------------------------*/
#pragma mark ios的邮件代理回调

- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error
{
    // Notifies users about errors associated with the interface
    switch (result)
    {
        case MFMailComposeResultCancelled:
            //NSLog(@"Result: canceled");
            break;
        case MFMailComposeResultSaved:
            //NSLog(@"Result: saved");
            break;
        case MFMailComposeResultSent:
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil message:@"Mail Sent Successfully" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            [alert show];
            [alert release];
        }
            break;
        case MFMailComposeResultFailed:
            //NSLog(@"Result: failed");
            break;
        default:
            //NSLog(@"Result: not sent");
            break;
            
    }
    RootViewController *vc=[AppController sharedAppController].viewController;
    if ([vc respondsToSelector:@selector(presentViewController:animated:completion:)])
    {
        [vc dismissViewControllerAnimated:YES completion:NULL];
        
    }
    else
    {
        [vc dismissModalViewControllerAnimated:YES];
    }

}

- (void)popAlertDialog:(NSString *)message
{
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:message delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

-(BOOL)isNetworkAvaliable {
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
    
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityFlags flags;
    
    BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
    CFRelease(defaultRouteReachability);
    
    if (!didRetrieveFlags) {
        return NO;
    }
    
    BOOL isReachable = flags & kSCNetworkFlagsReachable;
    BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
    // = flags & kSCNetworkReachabilityFlagsIsWWAN;
    BOOL nonWifi = flags & kSCNetworkReachabilityFlagsTransientConnection;
    BOOL moveNet = flags & kSCNetworkReachabilityFlagsIsWWAN;
    
    return ((isReachable && !needsConnection) || nonWifi || moveNet) ? YES : NO;
}

#pragma mark 公司业务 广告 相关
-(void)showMorePage
{
    AboutUsPage *page=[[AboutUsPage alloc] init];
    
    RootViewController *vc=[AppController sharedAppController].viewController;
    [page showInView:vc.view];
    [page release];
}

- (void)showNewsBlast
{
//    [[AdsPopupManager sharedManager]showNewsBlast];
    [self showNewsBlast:NewsModeLaunch];
}

- (void)showNewsBlast:(NewsBlastMode)mode{
//    [[AdsPopupManager sharedManager]showNewsBlast:(InvokeMode)mode];
    NewsBlast *news= [NewsBlast shareNews];
    news.invokeMode=(InvokeMode)mode;
    [news sendNews];
}

-(void)showPrivacy
{
//    PrivacyPage *page = [PrivacyPage privacyPage];
    
//    RootViewController *vc=[AppController sharedAppController].viewController;
//    [page showInView:vc.view];
//    [page release];
}
-(void)rateUs
{
    NSString *url=nil;
    NSString *app_id = @"1055242277";
    if ([[UIDevice currentDevice].systemVersion floatValue] < 7.0) {
        url=[NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%@",app_id];
    }else{
        url=[NSString stringWithFormat:@"itms-apps://itunes.apple.com/app/id%@",app_id];
    }
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}


@end
