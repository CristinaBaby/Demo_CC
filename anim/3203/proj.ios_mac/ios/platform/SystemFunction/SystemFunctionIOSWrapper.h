//
//  SystemFunctionIOSWrapper.h
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import <Foundation/Foundation.h>
#import <MessageUI/MessageUI.h>
#include "STSystemFunction.h"
#import "NewsBlast.h"

@interface SystemFunctionManager : NSObject<MFMailComposeViewControllerDelegate>
{
    bool isTakingPhoto;
}

+(SystemFunctionManager*)sharedManager;
+(void)purgeManager;

/**
 *  @brief Send Email by system default.
 *
 *  @param subject email subject.
 *  @param content email subject.
 */
-(void)sendEmail:(NSString*)subject Content:(NSString*) content;

-(void)popAlertDialog:(NSString *)message;

-(void)showMorePage;
-(void)goToPrivacy;

- (void)showNewsBlast;
- (void)showNewsBlast:(InvokeMode)mode;

-(void)saveLocalImage:(NSString *)image_path;

-(void)saveLocalImageWithImage:(UIImage *)image;

-(void)rateUs:(NSString*)appleID Message:(NSString*) message;

- (BOOL)isRateUs;

@end
