#import <UIKit/UIKit.h>
#import <SSCAppRootControllerLib/AppRootController.h>

@class RootViewController;

@interface AppController : AppRootController <UIApplicationDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;
@property(nonatomic, retain) UIWindow *window;

+ (AppController *)sharedAppController;

@end

