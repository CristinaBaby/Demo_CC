//
//  AppConfigs.h
//  PaintDress
//
//  Created by maxiang on 6/24/15.
//
//

#ifndef PaintDress_AppConfigs_h
#define PaintDress_AppConfigs_h

#define CREATE_SCENE_FUNC(__TYPE__) \
static cocos2d::Scene* createScene() \
{\
    auto scene = cocos2d::Scene::create();\
    auto layer = __TYPE__::create();\
    scene->addChild(layer);\
    return scene;\
}\

/* DEBUG: remove fullscreen ads */
#define DEBUG_REMOVE_FULLSCREEN_ADS     0

/* design width & height */
#define DESIGN_WIDTH                640.0
#define DESIGN_HEIGHT               960.0

/* ----------------User Default Keys---------------- */

/* indicated if is first time launch app */
#define UserDefaultKey_FirstTimeLaunchApp     "UserDefaultKey_FirstTimeLaunchApp"

/* indicated if is user has saw the preview guide */
#define UserDefaultKey_HasSawPreviewGuide     "UserDefaultKey_HasSawPreviewGuide"

/* indicated if is user has saw the choose guide */
#define UserDefaultKey_HasSawChooseGuide      "UserDefaultKey_HasSawChooseGuide"

/* indicated if item has been purchased, like UserDefaultKey_hasPurchasedItem_ + resName */
#define UserDefaultKey_HasPurchasedItem       "UserDefaultKey_HasPurchasedItem"

/* indicated if is open sound */
#define UserDefaultKey_IsOpenSound            "UserDefaultKey_IsOpenSound"

/* user remaining coins */
#define UserDefaultKey_Coins                  "UserDefaultKey_Coins"

/* user has removed ads */
#define UserDefaultKey_HasRemoveAds           "UserDefaultKey_HasRemoveAds"

/* indicated if is new purchased res, show the new logo */
#define UserDefaultKey_IsNewPurchaseRes       "UserDefaultKey_IsNewPurchaseRes_"

/* indicated if is new paint res, show the new logo */
#define UserDefaultKey_IsNewPaintRes          "UserDefaultKey_IsNewPaintRes_"

/* ----------------Game config---------------- */

/* the max shown coin, if coins more than this value, coin label will show as "9999+" */
#define MAX_SHOWN_COIN  9999

/* the default coin */
#define DEFAULT_COIN    500

/* the max count of the picture can be saved in one category  */
#define MAX_PICTURE_SAVE_COUTN  10

/* the saved photo display scale  */
#define PHOTO_DISPLAY_SCALE     0.66

/* ----------------Data paths---------------- */
#define DATA_PATH_PENS           "config/pens.csv"
#define DATA_PATH_PATTERN        "config/patterns.csv"
#define DATA_PATH_RES            "config/data.csv"
#define DATA_PATH_SAVED_PHOTOS   "savedPhotos.plist"
#define DATA_PATH_PAINT_RES      "paintResData.plist"

/* ----------------Paint res keys---------------- */
#define KEY_PAINT_TOP       "PaintTop"
#define KEY_PAINT_BOTTOM    "PaintBottom"
#define KEY_PAINT_DRESS     "PaintDress"
#define KEY_PAINT_SHOES     "PaintShoes"
#define KEY_SAVED_PHOTOS    "savedPhotos"

/* ----------------Res path---------------- */
#define PATH_CLOTHES        "res/clothes/"
#define PATH_ICON           "res/icon/"

/* ----------------Sound File---------------- */
#define SOUND_BGM_COLOR    "bgm_color.mp3"
#define SOUND_BGM_SELECT   "bgm_select.mp3"
#define SOUND_BGM_START    "bgm_start.mp3"
#define SOUND_BGM_DRESS    "bgm_dress.mp3"

#define SOUND_DRESS        "sfx_dress1.mp3"
#define SOUND_POPUP        "sfx_popup.mp3"
#define SOUND_COLORING     "sfx_color_brushing.mp3"
#define SOUND_BUTTON       "sfx_btn.mp3"
#define SOUND_PLAY         "sfx_btn_play.mp3"
#define SOUND_CAMERA       "sfx_camera.mp3"


/* ----------------Platform config---------------- */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    /* Apple app id */
    #define APP_ID                              "1006345425"

    /* mupub ads id */
    #define IPHONE_BANNER_ID                    @"a515d549a3104eb2b3f22ed01750dacd"
    #define IPHONE_FULLSCREEN_ID                @"c82384c6e4bf4463a2f1b582782569cf"

    #define IPAD_BANNER_ID                      @"321b0e40cd1b4b8ab8f16fb61dbf287e"
    #define IPAD_FULLSCREEN_ID                  @"56e1a5a56af14afa954e31228388faa8"

    #define IPHONE_MEDIUM_RECT_ID               @""
    #define IPAD_MEDIUM_RECT_ID                 @""

    #define IPHONE_FULLSCREEN_ID_CROSSPROMO     @"95b00977959949d4aedaf8ed12881f7e"
    #define IPAD_FULLSCREEN_ID_CROSSPROMO       @"f15b5ddb5dba48db901528be7fb7f92b"

    #define IPHONE_FULLSCREEN_ID_REWARD         @""
    #define IPAD_FULLSCREEN_ID_REWARD           @""

    /* iap id */
    #define IAP_FEW_COINS                       "com.toyboxmediainc.littletailorstory.afewcoins"
    #define IAP_SOME_COINS                      "com.toyboxmediainc.littletailorstory.somecoins"
    #define IAP_LOTS_COINS                      "com.toyboxmediainc.littletailorstory.lotsofcoins"

    #define IAP_REMOVE_ADS                      "com.toyboxmediainc.littletailorstory.removead"

    /* flurry id */
    #define FLURRY_KEY                          @"QVG4CHCCT8Z6HBK6XS5G"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    /* iap id */
    #define IAP_FEW_COINS                       "com.toyboxmediainc.android_littletailordressup.billing_afewcoins"
    #define IAP_SOME_COINS                      "com.toyboxmediainc.android_littletailordressup.billing_somecoins"
    #define IAP_LOTS_COINS                      "com.toyboxmediainc.android_littletailordressup.billing_lotsofcoins"

    #define IAP_REMOVE_ADS                      "com.toyboxmediainc.android_littletailordressup.billing_removead"

    /* flurry id */
    #define FLURRY_ID                           ""

#endif

#endif
