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
#include "CFSystemFunction.h"

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

-(void)popAlertDialog:(NSString *)message;
-(void)sendEmailWithPic:(NSString*)subject Content:(NSString*) content Path:(NSString*) imagePath;
-(BOOL)isNetworkAvaliable;

-(void)showMorePage;
-(void)showPrivacy;
-(void)rateUs;
- (void)showNewsBlast;
- (void)showNewsBlast:(NewsBlastMode)mode;

-(UIImage*)CCImageToUIImage:(Image*)apImg;
//-(void)saveToAlbum:(Image*)img withTarget:(Ref*) target withCallFun : (CallFunction)callFun;
-(void)saveToAlbum:(Image*)img withCallback:(std::function<void(bool)>) call;

@end
