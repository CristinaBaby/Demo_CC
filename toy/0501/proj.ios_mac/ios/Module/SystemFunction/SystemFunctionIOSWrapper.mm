//
//  SystemFunctionIOSWrapper.m
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import "SystemFunctionIOSWrapper.h"
#import "AppController.h"
#import "RootViewController.h"
#import "AboutUsPage.h"
//#import "RateTool.h"
#import <SystemConfiguration/SCNetworkReachability.h>
#import <netinet/in.h>



static SystemFunctionManager *s_instance=nil;

UIActivityIndicatorView  *activityIndicatorView_;

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
//
-(void)sendEmail:(NSString*)subject Content:(NSString*) content : (cocos2d::CCImage *)image
{
    NSMutableString *emailBody = [[NSMutableString alloc] initWithString:@"<html><body>"];
    
    subject = @"Check out my donut";

    NSString *emailBodyContent = [NSString stringWithFormat:@"%@%@", @"<p>I just made this donut in the <B>Donut!</B> application!</p>",@" <p><a href=‘http://itunes.apple.com/app/id1040411010’>http://itunes.apple.com/app/id1040411010</a></p>"];
    
    [emailBody appendString:emailBodyContent];
    
    
    //    [emailBody appendString:content];
    UIImage* ui_image = [self covertLocalImage:(image)];
    NSData *imageData = [NSData dataWithData:UIImagePNGRepresentation(ui_image)];
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
            
            RootViewController *vc = [AppController sharedAppController].viewController;
            
            if ([vc respondsToSelector:@selector(presentViewController:animated:completion:)]) {
                [vc presentViewController:emailDialog animated:YES completion:NULL];
            }else
            {
                [vc presentModalViewController:emailDialog animated:YES];
            }
            
            [emailDialog release];
        }
    }
    [emailBody release];
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

#pragma mark 公司业务 广告 相关
-(void)showMorePage
{
    AboutUsPage *page=[[AboutUsPage alloc] init];
    
    RootViewController *vc=[AppController sharedAppController].viewController;
    [page showInView:vc.view];
    [page release];
}

-(void)go2Market:(NSString *)url
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

-(void)rateUs:(NSString*)appleID Message:(NSString*) message
{
    // rate us
//    [RateTool rateForAppleID:appleID alertMessage:message];
//    
//    RootViewController *vc=[AppController sharedAppController].viewController;
//    UIWindow *window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
//    
//    window.rootViewController = vc;
//    [window makeKeyAndVisible];
}

-(void)go2PrivacyPage
{
    
}

-(void)openURL:(NSString *)url
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

cocos2d::CCImage *mu_image;
-(void)saveLocalImage:(cocos2d::CCImage *)image
{
    _popSaveImageAlert = YES;
    mu_image = image;
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
    
    UIImage* m_image = [self covertLocalImage:(image)];
    UIImageWriteToSavedPhotosAlbum(m_image, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
}

-(void)saveLocalImage:(cocos2d::CCImage *)image withAlert:(BOOL)alert
{
    [self saveLocalImage:image];
    _popSaveImageAlert = alert;
}

- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo
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
    
    if (_popSaveImageAlert)
    {
        if (error == NULL){
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"Save successfully!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            [alert show];
            [alert release];
            
        }else{
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"Save failed!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            [alert show];
            [alert release];
        }
    }
}

-(BOOL)isHaveInternet
{
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
    
    // Recover reachability flags
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityFlags flags;
    
    BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
    CFRelease(defaultRouteReachability);
    
    if (!didRetrieveFlags)
    {
        return NO;
    }
    
    BOOL isReachable = flags & kSCNetworkFlagsReachable;
    BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
    return (isReachable && !needsConnection) ? YES : NO;
}


-(BOOL)isRate
{
    //return [RateTool hasRated];
}

-(BOOL)isIosFiveUp
{
    if ([[[UIDevice currentDevice] systemVersion] floatValue] < 5.1) {
        return false;
    }
    return true;
}

-(UIImage*)covertLocalImage:(cocos2d::CCImage *)ccImage{
    NSUInteger bytesPerPixel = 4;
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL,
                                                              ccImage->getData(),
                                                              ccImage->getDataLen() * bytesPerPixel,
                                                              NULL);
    
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrderDefault;
    
    
    
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    
    
    NSUInteger scanWidth = ccImage->getWidth() * bytesPerPixel;
    CGImageRef imageRef = CGImageCreate(ccImage->getWidth(),
                                        ccImage->getHeight(),
                                        8,
                                        bytesPerPixel * 8,
                                        scanWidth,
                                        colorSpaceRef,
                                        bitmapInfo,
                                        provider,
                                        NULL,
                                        NO,
                                        renderingIntent);
    
    UIImage *image = [UIImage imageWithCGImage:imageRef];
    CGColorSpaceRelease(colorSpaceRef);
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    return image;
}


-(std::string)getTempPath
{
    NSString* lStr = NSTemporaryDirectory();
    
    return [lStr UTF8String];

}

//5.0
-(std::string)getSavePath
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDir = [paths objectAtIndex:0];
    
    return [cachesDir UTF8String];
}



@end
