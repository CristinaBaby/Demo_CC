##统计SDK 跨平台文件说明

此仓库下的文件用于Cocos2dx 跨平台开发,使用是添加子模块到Cocos2dx 的Class 文件夹中.

如果编译iOS 版本程序,需要在Xcode 中去掉对android 文件夹的引用.

####使用说明

* 首先需要在AppController.mm文件引入`#import <AnalyticsSDK/AnalyticsSDK.h>`,并在didFinishLaunchingWithOptions方法中
调用`[[Analytics getInstance] startSession:AT_FLURRY|AT_GOOGLE|AT_NATIVE];`初始化iOS原生的统计sdk
    	*  `- (void)startSession:(int)types`方法说明
    
    		参数types：为统计平台类型，取值包括
    		AT_FLURRY:flurry 统计
    		AT_GOOGLE:GoogleAnalytics 统计
    		AT_NATIVE:自家统计
    		
    		例如，启动全部统计：`[[Analytics getInstance] AT_FLURRY|AT_NATIVE|AT_GOOGLE]`.（如果只用某种统计可以自己取舍。）

* 最后在cocos2d-x使用引入`#include "Analytics.h"`,并调用其方法来发送统计。
		
		常用方法说明：

    	*  使用`Analytics::getInstance()`方法可以获取单例对象.

	
    	*  调用方法`void sendEvent(std::string eventName, std::string value)`发送事件统计
    		
    		参数eventName：为事件名，必传
    		参数value：为值，如不需要可传空字符串
    		调用示例：Analytics::getInstance()->sendEvent("MyEventName","");
    		
    	*  调用方法`void sendScreenEvent(std::string sreenName)`发送屏幕事件统计
    		
    		参数screenName：为屏幕名，必传
    		调用示例：Analytics::getInstance()->sendScreenEvent("MyScreenEvent");
    
    	*  调用方法`setDebugMode`和`getDebugMode`设置debug模式和获取当前是否为debug模式状态