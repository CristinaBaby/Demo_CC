//
//  MyFaceBook.h
//  MAKE05
//
//  Created by jordenwu on 11/10/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FBConnect.h"
/**提供Facebook分享功能*/
@interface FacebookManager : NSObject<FBSessionDelegate, FBRequestDelegate, FBDialogDelegate>
{
    BOOL isNeedLogIn_;
    Facebook *facebook_;
    NSMutableArray *tempArray_;
    
    UIAlertView *alert_;
    UIActivityIndicatorView *_indicator;
}
+(FacebookManager *)sharedManager;

-(BOOL)handleOpenURL:(NSURL*)url;
-(void)logIn;
-(void)logOut;

-(void)postImage:(UIImage *)image;
-(void)postContent:(NSString *)caption Content:(NSString*) shareContent;

@end
