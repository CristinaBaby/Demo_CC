//
//  ImageTool.h
//  KidsFrameWorkTest
//
//  Created by zhangguangzong1 on 5/5/14.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#include "cocos2d.h"
#import <UIKit/UIKit.h>
using namespace cocos2d;

@interface ImageTool : NSObject

+(ImageTool *)sharedImageTool;

-(UIImage*)CCImageToUIImage:(CCImage*) apImg;
@end

#endif