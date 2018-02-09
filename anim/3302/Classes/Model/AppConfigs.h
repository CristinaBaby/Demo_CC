//
//  AppConfigs.h
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
    #define APP_ID                      "1040347670"

    /* mupub ads id */
    #define IPHONE_BANNER_ID                    @""
    #define IPHONE_FULLSCREEN_ID                @""

    #define IPAD_BANNER_ID                      @""
    #define IPAD_FULLSCREEN_ID                  @""

    #define IPHONE_MEDIUM_RECT_ID               @""
    #define IPAD_MEDIUM_RECT_ID                 @""

    #define IPHONE_FULLSCREEN_ID_CROSSPROMO     @""
    #define IPAD_FULLSCREEN_ID_CROSSPROMO       @""

    #define IPHONE_FULLSCREEN_ID_REWARD         @""
    #define IPAD_FULLSCREEN_ID_REWARD           @""

    /* iap id */
    #define IAP_UNLOCK_PATTERN_PEN      "com.animalhouseltd.doodleanimalpark.patterns"
    #define IAP_UNLOCK_SPECIAL_PACK     "com.animalhouseltd.doodleanimalpark.specialpack"

    /* flurry id */
    #define FLURRY_KEY                  @""

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    #define IPHONE_BANNER_ID            ""
    #define IPHONE_INTERSTITIAL_ID      ""
    #define IPAD_BANNER_ID              ""
    #define IPAD_INTERSTITIAL_ID        ""
    #define IPHONE_RECT_ID              ""
    #define IPAD_RECT_ID                ""

    #define IAP_UNLOCK_PATTERN_PEN      ""
    #define IAP_UNLOCK_SPECIAL_PACK     ""

    /* flurry id */
    #define FLURRY_KEY                   ""
#endif


#endif
