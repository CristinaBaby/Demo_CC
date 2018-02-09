//
//  Email.h
//  Chups006
//
//  Created by QAMAC01 on 14-9-24.
//
//

#import <Foundation/Foundation.h>
#include <string>
#import <UIKit/UIKit.h>
#include "cocos2d.h"
#include "STSystemFunction.h"
#import <MessageUI/MessageUI.h>

USING_NS_CC;

@interface Email : NSObject<MFMailComposeViewControllerDelegate>

- (void)sendEmailWithTitle:(const char* )title  withContent:(const char *)content withImage:(const char *)image withDelegate:(id)delegate;

@end