//
//  NotificationDef.h
//  Doctor
//
//  Created by tanshoumei on 13-8-12.
//
//

#ifndef Doctor_NotificationDef_h
#define Doctor_NotificationDef_h
//applicationWillEnterForeground notify
#define ApplicationWillEnterForegroundNotify "ApplicationWillEnterForeground"
//glview失去焦点 用于全屏广告出现等情况。当你正在操作游戏中的东西的时候，此时弹出个全屏广告
#define NotifyGLViewWillLoseFocus "NotifyGLViewWillLoseFocus"
#define kNotifyFullAdDisMiss "kNotifyFullAdDisMiss"
#define kNotifyMoreGameDidDisappear "kNotifyMoreGameDidDisappear"
//点击banner弹出广告
#define kNotifyBannerPopShow "kNotifyBannerPopShow"
//点击banner的弹出广告消失
#define kNotifyBannerPopAdDismiss "kNotifyBannerPopAdDismiss"

//商店消失
#define kNotifyShopExit "kNotifyShopExit"

#define ShopIsCloseNotify "ShopIsCloseNotify"
#define ItemBuySuccessNotify "ItemBuySuccessNotify"
#define UnitCloseNotify "UnitCloseNotify"

#define InterstitialIsShowNotify "InterstitialIsShowNotify"
#define InterstitialIsCloseNotify "InterstitialIsCloseNotify"
#define ResumeNotify "ResumeNotify"


#define CameraFinishedNotify "CameraFinishedNotify"



#define DiaperFinishedNotify "DiaperFinishedNotify"
#endif
