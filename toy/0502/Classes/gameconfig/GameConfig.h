//
//  GameConfig.h
//  puzzle
//
//  Created by liyang1 on 4/30/14.
//
//

#ifndef __puzzle__GameConfig__
#define __puzzle__GameConfig__

#include "cocos2d.h"
#include "../utilities/ImageDownload.h"
#include "../AppGlobal.h"

USING_NS_CC;

/*
 park_id :: 包名ID
 park_name :: 包名
 problem_ids :: 具体图片的关键字
 park_unlock_id :: 包的状态  （ 0 free 1 sale  2 paid  ）
 package_lock_name:: 解锁包名
 park_type :
 
 <key>park_id</key>
 <integer>1</integer>
 <key>park_name</key>
 <string>中国</string>
 <key>problem_ids</key>
 <string>1,2,3,4,5</string>
 <key>park_unlock_id</key>
 <string>0</string>
 <key>park_type</key>
 <string>0</string>
 <key>package_lock_name</key>
 <string>com.jigsaw.zhuoyue</string>
 
 */


typedef enum {
    DEVICE_IPHONE = 0,      //iphone
    DEVICE_IPHONE5,         //iphone5
    DEVICE_IPAD,            //ipad
    DEVICE_OTHER,            //other
}DEVICE_TYPE;

#define CCDisignSizeMake2 (IS_2048 ? Size((float)(1536), (float)(2048)) : Size((float)(768), (float)(1024)))

#define GROUP_FRAME_WIDTH   (416)
#define GROUP_FRAME_HEIGHT  (416)

//------------------------配置文档数据------------------------

#define more_CONFIG_NAME   "Jigsaw/Deprecated/more_config.plist"
#define GAME_BG_FILE_PRE    "ui09_background/";

#define PARK_ID ("park_id")
#define PARK_NAME ("park_name")
#define PARK_PROBLEM_IDS ("problem_ids")
#define PARK_UNLOCK_ID ("park_unlock_id")
#define PARK_PICTURE_POS ("park_type")
#define PARK_LOCK_NAME ("package_lock_name")



#define PROBLEM_ID ("problem_id")
#define PROBLEM_RESOURCE_ID ("problem_resource_id")
#define PROBLEM_TOTAL_STEP ("problem_total_step")
#define PROBLEM_NAME ("problem_name")
#define PROBLEM_STATUS ("problem_status")
#define PROBLEM_PASS_OPEN_PROBLEMS ("problem_pass_open_problems")
#define PROBLEM_UNLOCK_ID ("problem_unlock_id")

//------------------------配置文档数据------------------------


//----------声音文件
#define sound_bg_music  ("sound/sfx-bgm.mp3")
#define sound_vitory  ("sound/sfx-solved.mp3")
#define sound_combine  ("sound/sfx-combine.mp3")
#define sound_correct_pos  ("sound/sfx-jigsaw.mp3")

//----------声音文件



//------------------------UI路径------------------------

#define PICTURE_BG    "ui02_pack_bg.png"
#define PICTURE_FRAME  "ui04_frame.png"
#define MENU_PATH       "ui01_menu"
#define  THUMB_FILE_PRE             "UI/thumb/"
#define  NORMAL_FILE_PRE      "UI/normal/"
#define  JPG_SUF   ".jpg"
//------------------------UI路径------------------------

//-------------------------字体文件--------------------------

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

//#define PARK_FONT_THREME  "Helvetica Neue"
//#define BUTTON_FONT_THREME  "HelveticaRounded LT Bold"
#define GUIDE_FONT_THREME  "GROBOLD"

#else

#define PARK_FONT_THREME  "fonts/Helvetica Neue.ttf"
#define BUTTON_FONT_THREME  "fonts/HelveticaRounded LT Bold.ttf"
#define GUIDE_FONT_THREME  "fonts/GROBOLD.ttf"
#endif


//-------------------------字体文件--------------------------

//-------------------------显示内容--------------------------
#define PARK_SHOW_SALE_NAME       "sale"
#define PARK_SHOW_FREE_NAME       "free"
#define PARK_SHOW_UPGRADE_NAME       "upgrade"

#define PARK_SHOW_PHOTO_NAME       "your photo"


//-------------------------显示内容--------------------------


//-------------------------保存数据--------------------------

//（针对现有的包）用户暂停的关卡信息  包名 和  该包的具体关卡下标  用户用时  组队个数
#define GAME_RESUME_USER_DATA   "resume_user_data"
#define GAME_RESUME_PACKAGE_STR    "package_str"
#define GAME_RESUME_PROBLEM_INDEX   "resume_problem_index"
#define GAME_RESUME_USER_TIME   "resume_user_time"
#define GAME_RESUME_GROUP_COUNT   "resume_group_count"
#define GAME_RESUME_ROTATE   "resume_rotate_status"
#define GAME_RESUME_PIECES_NUMBER   "resume_pieces_number"


//（针对用户自选的图片）
#define STR_USER_SELECT_MODE  "user_select_picture_mode"
#define GAME_RESUME_USER_SELECT_DATA   "resume_user_select__data"
#define GAME_RESUME_USER_SELECT_PACKAGE_STR    "package_user_select_str"
#define GAME_RESUME_USER_SELECT_TIME   "resume_user_select__time"
#define GAME_RESUME_USER_SELECT_GROUP_COUNT   "resume_user_select_group_count"
#define GAME_RESUME_USER_SELECT_ROTATE   "resume_user_select_rotate_status"
#define GAME_RESUME_USER_SELECT_PIECES_NUMBER   "resume_user_select_pieces_number"
#define GAME_RESUME_USER_SELECT_STATUS   "resume_complete_status"//完成状态   



#define GAME_BG_KEY   "game_bg_index"
#define GAME_ROTATE_STATUS "game_rotate_status"
#define GAME_VICTORY_DATA  "vitory_data"
#define GAME_PURCHASE_ARRAY  "purchase_package_array"
//#define GAME_RESTORE_ARRAY  "restore_package_array"
#define GAME_FUNCTION_PURCHASE_    "game_function_is_purchase"
#define GAME_TRY_MY_PHOTO    "game_is_try_my_photo"


//rate us  用户玩的次数   进一次游戏 算一次
#define RATE_STATUS  "game_rate_status"
#define GAME_RATE_PLAY_TIMES  "game_rate_play_times"
#define GAME_RATE_INFO   "If you enjoy using this app,would you mind taking a moment to rate it? Thanks for your support!"
#define GAME_SOUND_STATUS  "game_sound_status"
#define IS_PLAY_GAME "is_play_game" //是否进入游戏界面
#define OVER_PLAY_TIMES "is_play_game" //rateus  记录用户第二次完成后
//-------------------------保存数据--------------------------




#define UPGRADE_TAG "upgrade_tag"

//#define    PARK_LOCK  0  //sale
//#define    PARK_FREE  1
//#define    PARK_SALE_1 1    //1.99
//#define    PARK_SALE_2  2    //2.99
//#define    PARK_SALE_3  3   //3.99

typedef enum {
    PARK_FREE = 0,
    PARK_PAID_1 = 1, //0.99
    PARK_PAID_2 = 2, //1.99
    PARK_PAID_3 = 3, //2.99
     PARK_PAID_4 = 4, //3.99
     PARK_PAID_5 = 5, //4.99
    PARK_SALE_6 = 6 ,//2.99   1.99
    PARK_SALE_7 = 7 ,//3.99   1.99
    PARK_SALE_8 = 8, //3.99   2.99
    PARK_SALE_9 = 9 ,//4.99   2.99
    PARK_SALE_10 = 10 //4.99  3.99
} PRICE_KINDS;

#define    PARK_MORE   -1

#define    MORE_URL_SUF   "http://liveupdate_ticktockapps.s3.amazonaws.com/jigsaw/thumb/"
#define    ZIP_URL_SUF   "http://liveupdate_ticktockapps.s3.amazonaws.com/jigsaw/zip/"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS


#define PARK_CONFIG_NAME ("jigsaw/Deprecated/park_config_ios.plist")      //包名配置文件名字

#elif  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // android

#define PARK_CONFIG_NAME ("jigsaw/Deprecated/park_config_android.plist")      //包名配置文件名字

#endif

#define   USER_SELECT_IMAGE_THUMB_NAME "user_select_image_prepare_thumb.jpg"
#define   USER_SELECT_IMAGE_NAME "user_select_image_prepare.jpg"
#define   USER_SELECT_SAVE_IMAGE_NAME "user_select_image.jpg"
#define   USER_SELECT_SAVE_IMAGE_THUMB_NAME "user_select_image_thumb.jpg"

#define   SELE_PICTURE       0  //项目自带图片
#define   MACHINE_PICTURE    1 //存储在机器上的图片

#define   RESUME_GAME_TABLE  "resume_game_data"




class ParkDataConfig : public Ref {
    
public:
    ParkDataConfig()
    :m_parkId("")
    ,m_problemCount(-1)
    ,m_parkIconId(1)
    ,m_problemIds(NULL)
    ,m_parkUnlockId(-1)
    ,m_parkName("")
    ,m_parkIsCustom(false)   //不知何意
    ,m_parkUnlockCount(0)
    ,m_park_picture_pos(0)
    ,m_parkLockName("")
    ,m_isReady(true)
    {};
    ~ParkDataConfig();
    
    static ParkDataConfig* create(CCDictionary* _dic);
    static ParkDataConfig* create(cocos2d::ValueMap &_dic);
    bool init(CCDictionary* _dic);
    bool init(cocos2d::ValueMap &_dic);
    
    void updateProblems(CCArray* _problems);
    
public:
    std::string m_parkId;
    int m_parkIconId;
    int m_problemCount;
    int* m_problemIds;
    int m_parkUnlockId;
    std::string m_parkName;
    std::string m_parkLockName;
    bool m_parkIsCustom;
    int m_parkUnlockCount;
    bool is_resume; //表示这一个包有一关是暂停状态
    int m_park_picture_pos; // 图片是自身项目还是在机器上  0自身  1网络
    bool m_isReady ;
};

class ResumeData {

    public :
    ResumeData()
    :isResumeMode(false)
    ,resume_data_str("")
    ,user_time(0)
    ,resume_park_id("")
    ,resume_problem_index(0)
    ,resume_rotate(0)
    ,pieceNumber(2){};

public:
    std::string resume_data_str;  // like "-1,1,300.1,500,-1;"
    bool isResumeMode;   //resume mode   是否开启了resume模式
    bool isComplete; //是否完成
    int user_time;
    std::string resume_park_id;
    int resume_problem_index;   // 具体关卡的下标
    int resume_group_count;   //group count
    int resume_rotate;   //whether support rotate
    int pieceNumber;   // number x  number
};


class GameConfig{
    public :
    GameConfig(){};
    ~GameConfig();
    
    static GameConfig* sharedGameConfig();
    std::string androidWritePath;
    
    bool init();
    
//    bool checkUserIsSaveData();
    /*功能函数封装*/
    void initParkConfig();
    void initMoreConfig();
    void initResumeConfig();
    void initUserSelectResumeConfig();
    CCArray* getParkConfigs();
    CCArray* getMoreConfigs();
     int user_last_pieces_number;
    
    bool isShowRateUs;//是否显示过rateus
    int count;
    int play_game_success_times;
    
    private :
    CCArray* m_parkConfigs;
    CCArray* m_moreConfigs;
    ParkDataConfig *curPark;
    ResumeData *resume_data;
    
    ResumeData *user_picture_resume_data ;
    
    public :
    float getBannerHeight();
    
    Vec2 getGameMapLeftTopPoint();//获取瓦片地图坐上角的位置
    
    void setCurrentPack(ParkDataConfig *infoConfig);
    
    ParkDataConfig* getCurrentPark();
    
    Sprite* getGameBg();
    
    Sprite* getNormalGameBg();
    
    Sprite*  changgeGameBg();
    
    public : //数据库操作
//    void createTable();
    
    void saveUserGameData(bool isUserSelectImage, std::string dataStr , int index , int time , int groupSize,int rotate , int pieceNumber);
    
    
    
    void saveUserVictoryData(int picture_index , const char* time);
    
    bool isPictureVictory(int picture_index);
    
    ResumeData* getUserGameData();   //get resume data
    
    ResumeData* getUserSelectPictureResumeData();   //get resume data
    
    void updateUserGameData();
    
    void updateRequestTag();
    
    void updateRotateStatus(int int_status);
    //是否恢复关卡resume状态 （包含用户自选图片判断）
    bool isNeeDResumeGuanqia(int problem_id);
    
    bool isShowResumeStatus(int problem_id);
    
    void removeResumeModeData(); //if user get the victory in resume mode , remove it
    
    void savePurchsedPackage(std::string m_parkId);   //purchase success, save data
    
    void saveRestorePackage(std::string m_parkId);   //purchase success, save data
    
    bool isHaveBuyPackage(std::string m_packageName);
    
    void savePurchsedFunction(bool isPurchase);   //purchase success, 解锁功能
    
    void updateParkAndMore(std::string m_park_purchase_name);
    
    bool isHaveTreyMyPhoto();
    
    void setHaveTreyMyPhoto(bool isTry);
    
    std::string getPlatformWritePath();//不同平台  不同路径
    
public:
    //获取封面照片
    std::string getPackageThumbShowPath(std::string fileName , int type);
    //获取缩略图
    std::string getPackageThumbPicturePath(std::string fileName ,  int index, int type);
    
    std::string getPackageThumbPicturePath_str(std::string fileName ,  int index, int type);
    //获取实际大图
    std::string getPackageBigPicturePath(std::string fileName ,  int index, int type);
    //获取zip地址
    std::string getDownLoadZipUrl(ParkDataConfig *parkInfo);
    //
    std::string getZipSavePath();
    
    //获取自选图片路径
     std::string getUserSelectPicturePath(int type);
    
    int bg_index;
    
    int current_request_tag;
    
    int game_rotated_status ;
    
    int session_play_times;  //用户进过几次游戏
    
//    std::string current_download_park_name;  //正在更新的包名
    std::map<std::string, int> map_package_download;
    
    std::map<std::string, std::string> map_more_pictrue_download;
    
    std::string getGuideMask(int index);
    
    void rateUs();
    
    Sprite* getCurrentBg();
    
    Sprite* getCurrentScrollBg();
    
    bool isFirstPlayGame();//是否第一次玩游戏  用户指导界面
    
    
//    std::string getAndroidTempFilePath();
//    bool isCanDownloadZip;
};

#endif /* defined(__puzzle__GameConfig__) */