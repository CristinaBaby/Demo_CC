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
    #define APP_ID                      "1003675118"

    /* mupub ads id */
//#define IPHONE_BANNER_ID                    @"4e5164f529ba4e7c91d39b6695a4f87d"
//#define IPHONE_FULLSCREEN_ID                @"fae3a524c10a48a8b5df67680e3e1ad1"
//
//#define IPAD_BANNER_ID                      @"ed7bf274bd8440eeb5c2d8e3b42fe7d5"
//#define IPAD_FULLSCREEN_ID                  @"c1739e12bc3041d19488fb98bf749da4"

//#define IPHONE_MEDIUM_RECT_ID               @""
//#define IPAD_MEDIUM_RECT_ID                 @""
//
//#define IPHONE_FULLSCREEN_ID_CROSSPROMO     @"5f0fb59994a0434bb3009b1a74921773"
//#define IPAD_FULLSCREEN_ID_CROSSPROMO       @"c340e28536fd4785be9cf95e4aa41fb9"

/*  */
//#define IPHONE_FULLSCREEN_ID_REWARD         @""
//#define IPAD_FULLSCREEN_ID_REWARD           @""

    /* iap id */
    #define IAP_UNLOCK_PATTERN_PEN      "com.toyboxmediainc.colorbooktoysoldier.patterns"
    #define IAP_UNLOCK_SPECIAL_PACK     "com.toyboxmediainc.colorbooktoysoldier.specialpack"

    /* flurry id */
//    #define FLURRY_ID                   "CT7G9B8Q7XTK7H8FGJVH"
//    #define FLURRY_KEY                       @""

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
