//
//  CBAppGlobal.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef ColorBook_CBAppGlobal_h
#define ColorBook_CBAppGlobal_h
#include "cocos2d.h"
#include "AppConfigs.h"

/* ----------------User Default Keys---------------- */

/* indicated if is first time launch app */
#define UserDefaultKey_FirstTimeLaunchApp     "UserDefaultKey_FirstTimeLaunchApp"

/* indicated if pattern pen has unlocked */
#define UserDefaultKey_PatternPenHasUnlocked  "UserDefaultKey_PatternPenHasUnlocked"

/* indicated if this server pack has been downloaded, this key will set to
   UserDefaultKey_HasDownloadServerPack_ + "PackName", to distinguish different server pack
   */
#define UserDefaultKey_HasDownloadServerPack  "UserDefaultKey_HasDownloadServerPack_"

/* indicated if this pack has been purchased, this key will set to 
   UserDefaultKey_HasPaidPack_ + purchasedIdentifier */
#define UserDefaultKey_HasPurchasedPack       "UserDefaultKey_HasPurchasedPack_"

/* indicated if is open bgm */
#define UserDefaultKey_isOpenBgm              "UserDefaultKey_isOpenBgm"

/* indicated if is open sound effect */
#define UserDefaultKey_isOpenSfx              "UserDefaultKey_isOpenSfx"

/* ----------------Pack Config---------------- */

/* /device writeable dir/ColorBookPacks/ */
#define PACKS_ROOT_DIR_NAME         "ColorBookPacks"

/* /device writeable dir/ColorBookPacks/Pack1....1000/ */
#define PACK_DIR_NAME               "Pack" //

/* /device writeable dir/ColorBookPacks/PacksLogo/serverPackLogo1...100.png */
#define PACK_LOGO_DIR_NAME          "PacksLogo" //

/* indicated the max lenght of the pack name */
#define PACK_NAME_MAX_LENGTH        100

/* /device writeable dir/ColorBookPacks/Pack1/Pack_1_1_picture.png */
#define PACK_PICTURE_NAME           "picture.png"

/* /device writeable dir/ColorBookPacks/Pack1/Pack_1_1_history_picture.png */
#define PACK_HISTORY_PICTURE_NAME   "history_picture.png"

/* /device writeable dir/ColorBookPacks/Pack1/Pack_1_1_history_logo.png */
/* -------IMPORTANT------
 history picture just a color mask, not include wireframe, so we must store the full
 image to history logo, used to show in CBPictureScene */
#define PACK_HISTORY_PICTURE_LOGO   "history_logo.png"

/* /device writeable dir/ColorBookPacks/Pack1/Pack_1_1_mask.png */
#define PACK_PICTURE_MASK_NAME      "mask.png"

/* indicated the max lenght of the picture/mask/history name */
#define PICTURE_NAME_MAX_LENGTH     100

#define PackInfoKey_PacksInfo                           "PacksInfo"

#define PackInfoKey_Logo                                "Logo"
#define PackInfoKey_iOSPurchaseIdentifier               "iOS_PurchaseIdentifier"
#define PackInfoKey_AndroidPurchaseIdentifier           "Android_PurchaseIdentifier"
#define PackInfoKey_ItemsCount                          "ItemsCount"
#define PackInfoKey_PackName                            "PackName"
#define PackInfoKey_PackId                              "PackId"
#define PackInfoKey_SpecialPack                         "SpecialPack"

/* ----------------Local Pack Config---------------- */
#define LOCAL_PACKS_FILE_NAME       "LocalPacksInfo.plist"

/* ----------------Server Pack Config---------------- */
#define SERVER_PACKS_FILE_NAME      "ServerPacksInfo.plist"

/* server packs info url */
#if (DEBUG_STAGING_SERVER)
    #define URL_SERVER_PACKS_INFO        "http://liveupdate.toyboxmediainc.com/colorbook_toysoldier/staging/ServerPacksInfo.plist"
    #define URL_SERVER_PACK_LOGO         "http://liveupdate.toyboxmediainc.com/colorbook_toysoldier/staging/packs_logo/"
    #define URL_SERVER_PACK              "http://liveupdate.toyboxmediainc.com/colorbook_toysoldier/staging/packs_detail/"
#else
    #define URL_SERVER_PACKS_INFO        "http://liveupdate.toyboxmediainc.com/colorbook_toysoldier/live/ServerPacksInfo.plist"
    #define URL_SERVER_PACK_LOGO         "http://liveupdate.toyboxmediainc.com/colorbook_toysoldier/live/packs_logo/"
    #define URL_SERVER_PACK              "http://liveupdate.toyboxmediainc.com/colorbook_toysoldier/live/packs_detail/"
#endif

/* used to identifier http request */
#define HttpRequestTag_CheckPacksUpdateInfo  "HttpRequestTag_CheckPacksUpdateInfo"

/* HttpRequestTag_DownloadPackLogo_Pack1...100 */
#define HttpRequestTag_DownloadPackLogo      "HttpRequestTag_DownloadPackLogo_"

/* HttpRequestTag_DownloadPack_Pack1...100 */
#define HttpRequestTag_DownloadPack          "HttpRequestTag_DownloadPack_"

/* ----------------Sound File---------------- */
#define SOUND_BUTTON        "sfx-button.mp3" 
#define SOUND_BGM           "sfx-bgm.mp3"
#define SOUND_CHANGE_PENS   "sfx-alternatvie.mp3"
#define SOUND_COLORED       "sfx-colored.mp3"










#endif
