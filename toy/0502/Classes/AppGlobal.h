//
//  AppGlobal.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#ifndef __LVUP002__AppGlobal__
#define __LVUP002__AppGlobal__

#include "cocos2d.h"

using std::string;

// ===========================================================

/*
 * market macros. when we will publish amazon version, we need add "ST_MARKET_AMAZON"
 * value in "Application.mk" file, which under the "jni" directory, with param "-DST_MARKET_AMAZON"
 */

// distinguish google play from amazon
#define ST_TARGET_MARKET_APPLE                                 0
#define ST_TARGET_MARKET_GOOGLE_PLAY                           1
#define ST_TARGET_MARKET_AMAZON                                2


#if defined(ST_MARKET_AMAZON)
#undef  ST_TARGET_MARKET
#define ST_TARGET_MARKET        ST_TARGET_MARKET_AMAZON
#elif defined(ST_MARKET_GOOGLE_PLAY)
#undef  ST_TARGET_MARKET
#define ST_TARGET_MARKET        ST_TARGET_MARKET_GOOGLE_PLAY
#endif

//add for jigsaw
#define NS_CC_MAX_BEGIN                     namespace cocos2d { namespace liyang {
#define NS_CC_MAX_END                       }}
#define USING_NS_CC_MAX                     using namespace cocos2d::liyang

#define GET_CCDIRCTOR                           Director::getInstance()
#define CCRectPointSizeMake(point, size)        Rect( ((Vec2)(point)).x, ((Vec2)(point)).y, ((Size)(size)).width, ((Size)(size)).height )
#define VISIBLE_RECT                            CCRectPointSizeMake((GET_CCDIRCTOR->getVisibleOrigin()), (GET_CCDIRCTOR->getVisibleSize()))
#define ccp_center_screen                       Vec2(VISIBLE_RECT.getMidX(), VISIBLE_RECT.getMidY())
#define ccp_horziable_screen(y)                 Vec2(VISIBLE_RECT.getMidX(), y)
#define GET_RESUME_DATA                         GameConfig::sharedGameConfig()->getUserGameData()
#define GET_GAME_MAP_LEFTTOP                    GameConfig::sharedGameConfig()->getGameMapLeftTopPoint()
#define GET_CURRENT_PARK                        GameConfig::sharedGameConfig()->getCurrentPark()
#define GET_GAME_CONFIG                         GameConfig::sharedGameConfig()
#define GET_PLATFORM_SAVE_PATH                  GET_GAME_CONFIG->getPlatformWritePath()


#define bg_number                               4
#define PIECE_NUMBER                            6   //定义难度大小阀值

#define DEBUG_GAME                              0

// ===========================================================



/* ------------------------ root directory ------------------------ */


// resources' root directory
#define ST_UI_2048                                      "ui-2048X1536"
#define ST_UI_1024                                      "ui-1024X768"
// all strings's root directory
#define ST_STRINGS_ROOT                                 "strings"
// all maps' root directory
#define ST_MAPS_ROOT                                    "maps"


// ----------------------------------------------------------------


// file seperator
#define st_file_seperator                               "/"
#define st_file_hyphen                                  "-"
// protocol of parse map file.
#define st_suffix_jpg                                   ".jpg"
#define st_suffix_png                                   ".png"
#define st_suffix_csv                                   ".csv"
// level's identifer + this suffix, such as "a1.tmx"
#define st_map_suffix                                   ".tmx"
// the describe file of texture sheet of hidden objects
#define st_texture_packer_file_suffix                   ".plist"
#define st_texture_packer_image_suffix                  ".png"


// ----------------------------------------------------------------


#define st_strings_root                                 "strings"
#define st_strings_file                                 "strings/strings.csv"
#define st_strings_id                                   "id"
#define st_strings_Name                                 "Name"


// ----------------------------------------------------------------


// MapsConfig.csv file
#define st_maps_config_file                             "maps/MapsConfig.csv"
// the first level label
#define st_maps_config_version                          "Version"
#define st_maps_config_capacity                         "Capacity"
#define st_maps_config_free                             "Free"

// define the labels in "description.plist" file
#define st_maps_descfile_name                           "maps/Description.csv"
// the second level label under the Level
#define st_level_free                                   "free"
#define st_level_identifer                              "identifer"
#define st_level_location                               "location"
#define st_level_bg_thumb                               "thumbnail"


// ----------------------------------------------------------------


#define strings_button_yes                              "button_yes"
#define strings_button_no                               "button_no"
#define strings_button_ok                               "button_ok"
#define strings_quit_content                            "quit_content"
#define strings_delete_content                          "delete_content"
#define strings_restart_content                         "restart_content"
#define strings_exit_content                            "exit_content"
#define strings_restore_no_transaction                  "restore_no_transaction"
#define strings_restore_successfully                    "restore_successfully"
#define strings_restore_failed                          "restore_failed"
#define strings_billing_not_supported_message           "billing_not_supported_message"
#define strings_already_purchased                       "already_purchased"
#define strings_rate_us1                                "rate_us1"
#define strings_rate_us2                                "rate_us2"
#define strings_email_subject                           "email_subject"
#define strings_email_body                              "email_body"
#define strings_not_play_previous                       "tips_not_play_pre"



// ----------------------------------------------------------------


#define key_first_play_game                             "key_first_play_game"
#define key_iap_purchased_flag                          "iap_purchased_flag"
#define key_iap_ad_purchased_flag                       "key_iap_ad_purchased_flag"
#define key_music_volume                                "key_music_volume"
#define key_sound_volume                                "key_sound_volume"
#define key_rate_us_timestamp                           "key_rate_us_timestamp"
#define key_rate_us_never                               "key_rate_us_never"
#define key_high_score_time                             "TMHS"
#define key_high_score_attack                           "TAMHS"
#define key_high_score_zen                              "ZMHS"
#define key_complete_time                               "KCT"
#define key_complete_attack                             "KCA"
#define key_complete_zen                                "KCZ"
#define key_max_unlock_level                            "KXULL"
// highest score key for save to CCUserDefault
#define key_hint_quantity                               "key_hint_quantity"
#define key_freeze_quantity                             "key_freeze_quantity"
#define key_addtiome_quantity                           "key_addtiome_quantity"


// ----------------------------------------------------------------


#define sound_button                    "sounds/sfx_btn_default.mp3"
#define sound_game_combo                "sounds/sfx-combo.mp3"
#define sound_tap_right                 "sounds/sfx_spot_correct.mp3"
#define sound_tap_error                 "sounds/sfx_spot_incorrect.mp3"


// ----------------------------------------------------------------



/**
 * @brief maps' file storage location
 */
typedef enum {Inerval, External} FileLocation;


enum
{
    tag_scene_home = 10, tag_scene_game,
};


class AppGlobal
{
    AppGlobal();
    
public:
    
    enum class GameMode
    {
        TIME, TIME_ATTACK, ZEN,
    };
    
public:
    
    constexpr const static float ANIMATION_DURING = 0.4f;
    
    static cocos2d::ValueMap    s_m_MapsConfig;
    
    
    //MX：玩第二种模式的次数，不管成功失败，3次以后请求广告
    static int playGameAttackTimes;

    inline static std::string getLevelModeKey(const std::string &levelID, GameMode mode)
    {
        std::string key(levelID);
        switch(mode)
        {
            case GameMode::TIME:
                key.append(key_complete_time);
                break;
                
            case GameMode::TIME_ATTACK:
                key.append(key_complete_attack);
                break;
                
            case GameMode::ZEN:
                key.append(key_complete_zen);
                break;
        }
        
        return key;
    }
    
    inline static std::string getModeHighScoreKey(const std::string &levelID, GameMode mode)
    {
        std::string key(levelID);
        switch(mode)
        {
            case GameMode::TIME:
                key.append(key_high_score_time);
                break;
                
            case GameMode::TIME_ATTACK:
                key.append(key_high_score_attack);
                break;
                
            case GameMode::ZEN:
                key.append(key_high_score_zen);
                break;
        }
        
        return key;
    }
};



#endif /* defined(__LVUP002__AppGlobal__) */
