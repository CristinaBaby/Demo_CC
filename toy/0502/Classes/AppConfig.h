//
//  AppConfig.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#ifndef __LVUP002__AppConfig__
#define __LVUP002__AppConfig__


// ------------------- debug info ------------------

// unlimited props
#define DEBUG_PROPS             1

#define UNLOCK_ALL_LEVELS       0

// for developer debug
#define DEBUG_IAP               0

// -------------------------------------------------


// ------------------------------------------------------------------
#define project_name                            "Bedtime Story: Toy Soldier Family Fun Game Design for Kids and Toddlers"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS

#define apple_id                                "1004314003"

#define st_flurry_key                           "SWD9QKGPQNT35TD79FWV"


#define st_iap_sku_unlock_all                   "com.toyboxmediainc.bedtimemermaidfamily.unlockall"
#define st_iap_sku_hint_5                       "com.toyboxmediainc.bedtimemermaidfamily.hints"
#define st_iap_sku_freeze_5                     "com.toyboxmediainc.bedtimemermaidfamily.freeze"
#define st_iap_sku_add_time_5                   "com.toyboxmediainc.bedtimemermaidfamily.addtime"


// -------------- ADs --------------
// iPhone
#define IPHONE_BANNER_ID                        ""
#define IPHONE_INTERSTITIAL_ID                  "6b5044cab23440c2a15f814aa1768486"
// iPad
#define IPAD_BANNER_ID                          ""
#define IPAD_INTERSTITIAL_ID                    "8520a5e327084ff3b38fc1889ba5a705"
// Rect ad
#define IPHONE_RECT_ID                          "51782f8ad3d847349a6573b7968d326d"
#define IPAD_RECT_ID                            "65ed18e2f1fc4bcaa8deabe24f569552"
// ---------------------------------

#define FUNCTION_IAP_NAME ("net.baby_play.easterbunnyjigsaw.upgrade")

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
/*  android os */
#define st_android_package_name                 "com.toyboxmediainc.bedtimemermaidfamily"

#define st_iap_sku_unlock_all                   "com.toyboxmediainc.bedtimemermaidfamily.billing_unlockall"
#define st_iap_sku_hint_5                       "com.toyboxmediainc.bedtimemermaidfamily.billing_hints"
#define st_iap_sku_freeze_5                     "com.toyboxmediainc.bedtimemermaidfamily.billing_freeze"
#define st_iap_sku_add_time_5                   "com.toyboxmediainc.bedtimemermaidfamily.billing_addtime"

#define FUNCTION_IAP_NAME ("com.toyboxmediainc.bedtimemermaidfamily.billing_upgrade")

#define st_flurry_key                           "8B3F4Q62SD9YB39FKD3D"

#if (ST_TARGET_MARKET == ST_TARGET_MARKET_AMAZON)

// Amazon version
#define st_flurry_key                           ""

#elif (ST_TARGET_MARKET == ST_TARGET_MARKET_GOOGLE_PLAY)


#endif


#endif
// ------------------------------------------------------------------

// add for jigsaw
static float UI_SCALE = 1.08f;
static float GAME_SCALE = 0.5546875f;
static bool IS_PURCHASED = false;
static bool IS_FIRST_RUNNING = true;
static bool IS_2048 = false;
static bool isMyTouch = false;


#endif /* defined(__LVUP002__AppConfig__) */
