//
//  CBAppConfigs.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef ColorBook_CBAppConfig_h
#define ColorBook_CBAppConfig_h


#ifdef __cplusplus
    #define CB_BEGIN_NAMESPACE                  namespace colorbook {
    #define CB_END_NAMESPACE                    }
    #define USING_CB_NAMESPACE                  using namespace colorbook
#else
    #define CB_BEGIN_NAMESPACE
    #define CB_END_NAMESPACE
    #define USING_CB_NAMESPACE
#endif

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
#define DESIGN_WIDTH       960.0
#define DESIGN_HEIGHT      640.0

// 方块广告，如果不需要支持可以留空
#define IPHONE_MEDIUM_RECT_ID    @""
#define IPAD_MEDIUM_RECT_ID      @""

// Chartboost账号，如果不需要支持开启chartboost会话，可以留空
#define CHARTBOOST_APP_ID        @""
#define CHARTBOOST_APP_SIGNATURE @""

#define IPHONE_FULLSCREEN_ID_CROSSPROMO @"51e2f964d3454ed3a1c969bd322783a2"
#define IPAD_FULLSCREEN_ID_CROSSPROMO   @"9bb0cb869c4a4ab4ad9fea8751e01ea3"


/* ----------------Platform config---------------- */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    /* Apple app id */
    #define APP_ID                      "1025603039"

    /* mupub ads id */
    #define IPHONE_BANNER_ID            @"06acac2d811b447ab95c36560a381335"
    #define IPHONE_FULLSCREEN_ID        @"e040eb29f7d843ad810a71306eb821d1"
    #define IPAD_BANNER_ID              @"cea4ba9c4d9a485c825d0d0152a58360"
    #define IPAD_FULLSCREEN_ID          @"ff8fbc62fed94bb288cd5f36b0dd1ed4"
    #define IPHONE_FULLSCREEN_ID_REWARD @""
    #define IPAD_FULLSCREEN_ID_REWARD   @""

    /* iap id */
    #define IAP_UNLOCK_PATTERN_PEN      "com.toymakermedia.colorbookmonstertrucks.patterns"
    #define IAP_UNLOCK_SPECIAL_PACK     "com.toymakermedia.colorbookmonstertrucks.specialpack"

    /* flurry id */
    #define FLURRY_KEY                  @""
    #define FLURRY_ID                   "4JZKRZJ3XPN9RXVBCFDW"

    #define IAP_DEFAULT                 false

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    #define IPHONE_BANNER_ID            "b14907fb1b1d4697978f86cf65ca2b12"
    #define IPHONE_INTERSTITIAL_ID      "1462f4ae697243c1b8fd98fb15c863f5"
    #define IPAD_BANNER_ID              "641d7122ead74a9bb624923bf96c93d1"
    #define IPAD_INTERSTITIAL_ID        "e0227e83727e4cbab4034232b57ecbb9"
    #define IPHONE_RECT_ID              ""
    #define IPAD_RECT_ID                ""

    #define IAP_UNLOCK_PATTERN_PEN      "com.toyboxmediainc.android_colorbookmonstertrucks.billing_patterns"
    #define IAP_UNLOCK_SPECIAL_PACK     "com.toyboxmediainc.android_colorbookmonstertrucks.billing_specialpack"

    /* flurry id */
    #define FLURRY_ID                   "7C869F2BJMJ2DF9JSKCB"

    #define IAP_DEFAULT                 true
#endif


#endif
