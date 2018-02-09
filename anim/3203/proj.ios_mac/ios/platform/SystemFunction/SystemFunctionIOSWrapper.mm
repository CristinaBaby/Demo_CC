//
//  SystemFunctionIOSWrapper.m
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import "SystemFunctionIOSWrapper.h"
#import "AppController.h"
#import "AboutUsPage.h"
#import "RootViewController.h"
#import "RateTool.h"


#if !KIDS
#endif

static SystemFunctionManager *s_instance=nil;

@implementation SystemFunctionManager

+(SystemFunctionManager*)sharedManager
{
    if(s_instance==nil){
        s_instance=[[SystemFunctionManager alloc] init];
    }
    return s_instance;
}

-(SystemFunctionManager *)init
{
    self = [super init];
    if (self != nil) {
        isTakingPhoto = false;
    }
    return self;
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
    NSMutableString *emailBody = [[NSMutableString alloc] initWithString:@"<html><body>"];
    [emailBody appendString:[NSString stringWithFormat:@"%@%@%@%@%@",
                             @"<p>Hey!</p>",
                             @"<p>I am playing this awesome hidden object game - Hidden Objects:  Kitchen from Hell!</p>",
                             @"<p>I think you will like this also!</p>",
                             @"<p>Get it NOW!</p>",
                             @"<p><a href=‘https://itunes.apple.com/app/id694502530’>https://itunes.apple.com/app/id694502530</a></p>"]];
    
//    [emailBody appendString:content];
    
    //NSData *imageData = [NSData dataWithData:UIImagePNGRepresentation(self.shareImage)];
    [emailBody appendString:@"</body></html>"];
    Class mailClass = (NSClassFromString(@"MFMailComposeViewController"));
    if (nil != mailClass) {
        MFMailComposeViewController* emailDialog = [[MFMailComposeViewController alloc] init];
        if (nil != emailDialog) {
            emailDialog.mailComposeDelegate = self;
            // Set the subject of email
            //[emailDialog addAttachmentData:imageData mimeType:@"image/png" fileName:@"nails"];
            [emailDialog setSubject:subject];
            [emailDialog setMessageBody:emailBody isHTML:YES];
            
            RootViewController *vc=[AppController sharedAppController].viewController;
            
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

- (void)showNewsBlast
{
//    [[AdsPopupManager sharedManager]showNewsBlast];
    [self showNewsBlast:InvokeModeLaunch];
}

- (void)showNewsBlast:(InvokeMode)mode{
//    [[AdsPopupManager sharedManager]showNewsBlast:(InvokeMode)mode];
    [[NewsBlast shareNews] setInvokeMode:mode];
    [[NewsBlast shareNews] sendNews];
}


-(void)goToPrivacy
{
//    PrivacyPage *page = [PrivacyPage privacyPage];
//    
//    RootViewController *vc=[AppController sharedAppController].viewController;
//    [page showInView:vc.view];
    //    [page release];
}

-(void)rateUs:(NSString*)appleID Message:(NSString*) message
{
    // rate us
    [RateTool rateForAppleID:appleID alertMessage:message];
    
    RootViewController *vc=[AppController sharedAppController].viewController;
    UIWindow *window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    
    window.rootViewController = vc;
    [window makeKeyAndVisible];
}

-(BOOL)isRateUs
{
    return [RateTool hasRated];
}

-(void)saveLocalImage:(NSString *)image_path
{
    if(false == isTakingPhoto) {
        isTakingPhoto = true;
        UIImage *image = [UIImage imageWithContentsOfFile:image_path];
        UIImageWriteToSavedPhotosAlbum(image, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
    }
}

-(void)saveLocalImageWithImage:(UIImage *)image
{
    if(false == isTakingPhoto) {
        isTakingPhoto = true;
        UIImageWriteToSavedPhotosAlbum(image, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
    }
}

//保存到本地回调
- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo
{
    if (error != NULL)
    {
        if ([@"Data unavailable" isEqualToString:[error localizedDescription]]) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"This app does not have access to your photos.\nYou can enable access in Privacy Setting." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil, nil];
            [alert show];
            [alert release];
        }
        
    } else {
        UIAlertView *alert = [[[UIAlertView  alloc] initWithTitle:nil
                                                          message:@"Your image has been saved to Photos!"
                                                         delegate:nil
                                                cancelButtonTitle:nil
                                                otherButtonTitles:@"OK",nil] autorelease];
        [alert show];
    }
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ImageSaveComplete");
    isTakingPhoto = false;
}

@end
