//
//  CBAppConfig.h
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

/* ----------------Platform config---------------- */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    /* Apple app id */
    #define APP_ID                      "1017624025"

    /* mupub ads id */
#define IPHONE_BANNER_ID                    @"6c55b4ba75774c6d95ccf1566761031c"
#define IPHONE_FULLSCREEN_ID                @"78355cdde2fb4b659cb3c29e8292f421"

#define IPAD_BANNER_ID                      @"8a0cf85dd6744d359cdd62750414b29d"
#define IPAD_FULLSCREEN_ID                  @"21bc76dbdabb4195afcfd86bc4b316bd"

#define IPHONE_MEDIUM_RECT_ID               @""
#define IPAD_MEDIUM_RECT_ID                 @""

#define IPHONE_FULLSCREEN_ID_CROSSPROMO     @"80bd19a1bfd44759b926c892477f5b57"
#define IPAD_FULLSCREEN_ID_CROSSPROMO       @"542f63becd7d4f55b4ef7cbd458767c4"

/*  */
#define IPHONE_FULLSCREEN_ID_REWARD         @""
#define IPAD_FULLSCREEN_ID_REWARD           @""

    /* iap id */
    #define IAP_UNLOCK_PATTERN_PEN      "com.animalhouseltd.colorbookrainbowpony.patterns"
    #define IAP_UNLOCK_SPECIAL_PACK     "com.animalhouseltd.colorbookrainbowpony.specialpack"

    /* flurry id */
    #define FLURRY_ID                   "2MP4DCZDZ759W4BDSHHF"
    #define FLURRY_KEY                       @""

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    #define IPHONE_BANNER_ID            ""
    #define IPHONE_INTERSTITIAL_ID      ""
    #define IPAD_BANNER_ID              ""
    #define IPAD_INTERSTITIAL_ID        ""
    #define IPHONE_RECT_ID              ""
    #define IPAD_RECT_ID                ""

    #define IAP_UNLOCK_PATTERN_PEN      "com.toyboxmediainc.android_toysoldiercolorbook.billing_patterns"
    #define IAP_UNLOCK_SPECIAL_PACK     "com.toyboxmediainc.android_toysoldiercolorbook.billing_specialpack"

    /* flurry id */
    #define FLURRY_ID                   ""
#endif


#endif
