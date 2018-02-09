#import <UIKit/UIKit.h>
#import "AppRootController.h"

@class RootViewController;

@interface AppController : AppRootController <UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *_viewController;
    
}

@property(nonatomic, readonly) RootViewController* viewController;
+ (AppController *)sharedAppController;

@end

