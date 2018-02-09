//
//  CBAppConfigs.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef ColorBook_CBAppConfig_h
#define ColorBook_CBAppConfig_h

/* DEBUG: unlock pattern pen */
#define DEBUG_UNLOCK_PATTERN_PEN        0

/* DEBUG: staging server, used to test new pack */
#define DEBUG_STAGING_SERVER            0

/* design width & height */
#define DESIGN_WIDTH       960.0
#define DESIGN_HEIGHT      640.0

// 方块广告，如果不需要支持可以留空
#define IPHONE_MEDIUM_RECT_ID    @""
#define IPAD_MEDIUM_RECT_ID      @""

// Chartboost账号，如果不需要支持开启chartboost会话，可以留空
#define CHARTBOOST_APP_ID        @""
#define CHARTBOOST_APP_SIGNATURE @""

// Layer Level
#define UI_LAYER 100
#define BG_LAYER 99

#define UserDefaultKey_HasPurchasedPack             "UserDefaultKey_HasPurchasedPack_"
/* ----------------Platform config---------------- */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    /* Apple app id */
    #define APP_ID                                  "1040038129"

    /* iap id */
    #define iap_1                                   "com.toyboxmediainc.halloween.pack1"
    #define iap_2                                   "com.toyboxmediainc.halloween.pack2"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /* iap id */
    #define iap_1                                   "com.toyboxmediainc.android_color_halloween.pack1"
    #define iap_2                                   "com.toyboxmediainc.android_color_halloween.pack2"
#endif

#endif
