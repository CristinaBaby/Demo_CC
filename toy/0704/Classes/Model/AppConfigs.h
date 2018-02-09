//
//  CBAppConfigs.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef ColorBook_CBAppConfig_h
#define ColorBook_CBAppConfig_h


#define CREATE_SCENE_FUNC(__TYPE__) \
static cocos2d::Scene* createScene() \
{\
auto scene = cocos2d::Scene::create();\
auto layer = __TYPE__::create();\
scene->addChild(layer);\
return scene;\
}\

/* DEBUG: unlock pattern pen */
#define DEBUG_UNLOCK_PATTERN_PEN        0

/* DEBUG: staging server, used to test new pack */
#define DEBUG_STAGING_SERVER            0

/* design width & height */
#define DESIGN_WIDTH       640.0
#define DESIGN_HEIGHT      960.0

// 方块广告，如果不需要支持可以留空
#define IPHONE_MEDIUM_RECT_ID    @""
#define IPAD_MEDIUM_RECT_ID      @""

// Chartboost账号，如果不需要支持开启chartboost会话，可以留空
#define CHARTBOOST_APP_ID        @""
#define CHARTBOOST_APP_SIGNATURE @""

// Layer Level
#define UI_LAYER 100
#define BG_LAYER 99

//RewardedAds record

#define  RewardedAds_record_pens                        "RewardedAds_record_pens_"
#define  RewardedAds_record_packs                       "RewardedAds_record_packs_"

#define UserDefaultKey_HasPurchasedPack                 "UserDefaultKey_HasPurchasedPack_"

#define PENSPACK_TIME_LOCK                              "PensPack_Time_Lock_"
#define PICTUREPACK_TIME_LOCK                           "PicturePack_Time_Lock_"

/* ----------------Platform config---------------- */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #define APP_ID                                  "1076205983"
#endif

#endif
