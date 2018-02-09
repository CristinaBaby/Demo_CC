#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

+ (AppController *)sharedAppController;

@property(nonatomic, readonly) RootViewController* viewController;

@end

