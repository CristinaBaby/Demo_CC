//
//  SystemFunctionIOSWrapper.h
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
#import <iostream>
#include "STSystemFunction.h"

@interface SystemFunctionManager : NSObject<MFMailComposeViewControllerDelegate>
{
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

-(void)sendEmailWithPic:(NSString*)subject Content:(NSString*) content Path:(NSString*) imagePath;

-(void)popAlertDialog:(NSString *)message;

-(BOOL)isNetworkAvaliable;

-(void)showMorePage;
-(void)showPrivacy;

- (void)showNewsBlast;
- (void)showNewsBlast:(NewsBlastMode)mode;

-(UIImage*)CCImageToUIImage:(Image*)apImg;
-(void)saveToAlbum:(Image*)img withCallback:(std::function<void(bool)>) call;

-(void)rateUs;

@end
