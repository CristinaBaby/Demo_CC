#import <UIKit/UIKit.h>
#include "AppRootController.h"

@class RootViewController;

@interface AppController : AppRootController <UIApplicationDelegate> {
    UIWindow *window;
}

+ (AppController *)sharedAppController;

@property(nonatomic, readonly) RootViewController* viewController;

@end

