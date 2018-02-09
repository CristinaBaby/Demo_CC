//
//  SystemFunctionIOSWrapper.h
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import <Foundation/Foundation.h>
#import <MessageUI/MessageUI.h>
#include "cocos2d.h"

@interface SystemFunctionManager : NSObject<MFMailComposeViewControllerDelegate>
{
    BOOL _popSaveImageAlert;
}

+(SystemFunctionManager*)sharedManager;
+(void)purgeManager;


/**
 *  @brief Send Email by system default.
 *
 *  @param subject email subject.
 *  @param content email subject.
 */
-(void)sendEmail:(NSString*)subject Content:(NSString*) content : (cocos2d::CCImage *)image;

-(void)popAlertDialog:(NSString *)message;

-(void)showMorePage;

-(void)go2Market:(NSString *)url;

-(void)rateUs:(NSString*)appleID Message:(NSString*) message;

-(void)go2PrivacyPage;

-(std::string)getSavePath;

-(std::string)getTempPath;

-(void)openURL:(NSString *)url;

-(void)saveLocalImage:(cocos2d::CCImage *)image;
-(void)saveLocalImage:(cocos2d::CCImage *)image withAlert:(BOOL)alert;

-(UIImage*)covertLocalImage:(cocos2d::CCImage *)image;

- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo;

-(BOOL)isHaveInternet;

-(BOOL)isRate;

-(BOOL)isIosFiveUp;
@end
