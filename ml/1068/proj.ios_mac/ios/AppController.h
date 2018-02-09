#import <UIKit/UIKit.h>

#if __has_include(<AppRootController.h>)
#import "AppRootController.h"
#define ENABLE_COMPONENT_IOS  1
#else
#define ENABLE_COMPONENT_IOS  0
#endif

@class RootViewController;

@interface AppController :
#if ENABLE_COMPONENT_IOS
AppRootController
#else
NSObject
#endif
<UIApplicationDelegate> {
    UIWindow *window;
}
@property(nonatomic, readonly) RootViewController* viewController;
@property(nonatomic, retain) UIWindow *window;

+ (AppController *)sharedAppController;

@end

