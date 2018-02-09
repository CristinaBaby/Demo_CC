//
//  AlumnImpIos.h
//  Doctor2
//
//  Created by tanshoumei on 13-9-18.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import <MessageUI/MessageUI.h>

@interface AlumnImpIos : NSObject<UINavigationControllerDelegate, UIImagePickerControllerDelegate,UIPopoverControllerDelegate>{
    bool isTakingPhoto;
    UIPopoverController * photoPop;
}

+ (AlumnImpIos*) shared;
+ (void) purge;
//请求保存图片到相册
- (void) saveToAlumn:(UIImage*) apImg;
-(void) openAlumn;

@end

#endif
