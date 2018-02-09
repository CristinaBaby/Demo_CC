//
//  AppConfigs.h
//  ParfaitMaker
//
//  Created by maxiang on 7/30/15.
//
//

#ifndef ParfaitMaker_AppConfigs_h
#define ParfaitMaker_AppConfigs_h

#define CREATE_SCENE_FUNC(__TYPE__) \
static cocos2d::Scene* createScene() \
{\
    auto scene = cocos2d::Scene::create();\
    auto layer = __TYPE__::create();\
    scene->addChild(layer);\
    return scene;\
}\

/* design width & height */
#define DESIGN_WIDTH                960.0
#define DESIGN_HEIGHT               640.0

/* ----------------User Default Keys---------------- */

/* indicated if is first time launch app */
#define UserDefaultKey_FirstTimeLaunchApp     "UserDefaultKey_FirstTimeLaunchApp"

/* indicated if item has been purchased, like UserDefaultKey_hasPurchasedItem_ + identifer */
#define UserDefaultKey_HasPurchasedItem_       "UserDefaultKey_HasPurchasedItem_"

/* indicated if is open sound */
#define UserDefaultKey_IsOpenSound            "UserDefaultKey_IsOpenSound"

/* indicated if user has finished read finger guide */
#define UserDefaultKey_HasFinishedFingerGuide "UserDefaultKey_HasFinishedFingerGuide"

/* used to store recode info, max five records */
#define UserDefaultKey_Record_                "UserDefaultKey_Record_"

/* ----------------Purchase config---------------- */
#define FREE_ITEMS_COUNT        8
#define PURCHASE_ITEMS_COUNT    7

/* ----------------ColorBook Game---------------- */


/* ----------------Spot Game---------------- */
#define SPOT_GAME_RES_SCALE  0.90
#define SPOT_GAME_HINT_COOL_DOWN 20

/* ----------------Jigsaw Game---------------- */
#define JIGSAW_ORIGINAL_INTERVAL 15.0
#define JIGSAW_RELATIVE_INTERVAL 15.0
#define JIGSAW_HINT_TIME 5.0

/* ----------------Record Game---------------- */
#define RECORD_FILE             "recordInfo.plist"
#define RECORD_MAP_KEY          "recordInfo"
#define STORY_IMAGE_SCALE       0.66

#define TIMER_UPDATE_INTERVAL   0.1
#define RECORD_MAX_TIME         300

#define BOOK_ANIMATION_DURATION 0.3

/* ----------------Sound File---------------- */
#define SOUND_BGM_START                 "bg_start.mp3"
#define SOUND_BGM_STORY                 "bg_story.mp3"
#define SOUND_BTN_PLAY                  "sfx_btn_play.mp3"
#define SOUND_BTN_DEFAULT               "sfx_btn_default.mp3"
#define SOUND_COLOR_BRUSHING            "sfx_color_brushing.mp3"
#define SOUND_OVER                      "sfx_gameover_win.mp3"
#define SOUND_JIGSAW_CORRECT            "sfx_jigsaw_correct.mp3"
#define SOUND_POPUB                     "sfx_popup.mp3"
#define SOUND_SPOT_CORRECT              "sfx_spot_correct.mp3"
#define SOUND_SPOT_INCORRECT            "sfx_spot_incorrect.mp3"
#define SOUND_RECORD_START              "sfx_story_recordstart.mp3"


/* ----------------Platform config---------------- */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    /* Apple app id */
    #define APP_ID                              "1072023388"

    /* purchase identifer */
    #define IAP_UNLOCK_All           "com.toyboxmediainc.story_littlemermaid.unlockall"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    /* flurry id */
    #define FLURRY_ID                           ""

#endif


#endif
