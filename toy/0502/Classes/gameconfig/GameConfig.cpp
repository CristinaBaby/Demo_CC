//
//  GameConfig.cpp
//  puzzle
//
//  Created by liyang1 on 4/30/14.
//
//

#include "GameConfig.h"
#include "AppGlobal.h"
#include "../utilities/LYString.h"
#include <string>
#include "../utilities/Utils.h"
#include "../modules/STSystemFunction.h"
#include "../utilities/MyZipUtils.h"
#include "../AppConfig.h"

using namespace std;
//
ParkDataConfig::~ParkDataConfig(){}

ParkDataConfig* ParkDataConfig::create(CCDictionary* _dic) {
    ParkDataConfig *pRet = new ParkDataConfig();
    if (pRet && pRet->init(_dic))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        
        pRet = NULL;
        return NULL;
    }
}

bool ParkDataConfig::init(cocos2d::CCDictionary *_dic)
{
    if(!_dic)
        return false;
    
    CCString* park_id_str = (CCString*)_dic->objectForKey(PARK_ID);
    CCString* park_name_str = (CCString*)_dic->objectForKey(PARK_NAME);
    CCString* problem_ids_str = (CCString*)_dic->objectForKey(PARK_PROBLEM_IDS);
    CCString* park_unlock_str = (CCString*)_dic->objectForKey(PARK_UNLOCK_ID);
    CCString* park_picture_pos_str = (CCString*)_dic->objectForKey(PARK_PICTURE_POS);
    CCString* park_lock_name_str = (CCString*)_dic->objectForKey(PARK_LOCK_NAME);
    
    if (!park_id_str || !park_name_str || !problem_ids_str || !park_unlock_str) {
        return false;
    }
    
    m_parkId = park_id_str->getCString();
    //    CCLOG("m_parkId:%d", m_parkId);
    m_parkName = park_name_str->getCString();
    //    CCLOG("m_parkName:%s", m_parkName.c_str());
    m_parkUnlockId = park_unlock_str->intValue();
    
    m_park_picture_pos = park_picture_pos_str->intValue();
    
    m_parkLockName = park_lock_name_str->getCString();
    
    cocos2d::liyang::LYString *problem_ids_str_ = cocos2d::liyang::LYString::create(problem_ids_str->getCString());
    CCArray *array  = problem_ids_str_->componentsSeparatedByString(",");
    if(!array || array->count() <= 0)
        return false;
    
    m_problemCount = array->count();
    m_problemIds = (int*)malloc(sizeof(int)*m_problemCount);
    
    for (int i=0; i<m_problemCount; i++) {
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
        m_problemIds[i] = problem_id_string->intValue();
        if(m_problemIds[i] == -1){
            m_parkIsCustom = true;
            break;
        }
    }
    
    if(m_parkIsCustom){
        free(m_problemIds);
        m_problemIds = NULL;
        m_problemCount = 0;
    }
    
    return true;
    
};

ParkDataConfig* ParkDataConfig::create(cocos2d::ValueMap &_dic) {
    ParkDataConfig *pRet = new ParkDataConfig();
    if (pRet && pRet->init(_dic))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        
        pRet = NULL;
        return NULL;
    }
}

bool ParkDataConfig::init(cocos2d::ValueMap &_dic)
{
    string park_id_str = _dic[PARK_ID].asString();
    string park_name_str = _dic[PARK_NAME].asString();
    string problem_ids_str = _dic[PARK_PROBLEM_IDS].asString();
    int park_unlock_str = _dic[PARK_UNLOCK_ID].asInt();
    int park_picture_pos_str = _dic[PARK_PICTURE_POS].asInt();
    string park_lock_name_str = _dic[PARK_LOCK_NAME].asString();
    
    m_parkId = park_id_str;
    //    CCLOG("m_parkId:%d", m_parkId);
    m_parkName = park_name_str;
    
    //    CCLOG("m_parkName:%s", m_parkName.c_str());
    m_parkUnlockId = park_unlock_str;
    
    m_park_picture_pos = park_picture_pos_str;
    
    m_parkLockName = park_lock_name_str;
    
    cocos2d::liyang::LYString *problem_ids_str_ = cocos2d::liyang::LYString::create(problem_ids_str);
    CCArray *array  = problem_ids_str_->componentsSeparatedByString(",");
    if(!array || array->count() <= 0)
        return false;
    
    m_problemCount = array->count();
    m_problemIds = (int*)malloc(sizeof(int)*m_problemCount);
    
    for (int i=0; i<m_problemCount; i++) {
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
        m_problemIds[i] = problem_id_string->intValue();
        if(m_problemIds[i] == -1){
            m_parkIsCustom = true;
            break;
        }
    }
    
    if(m_parkIsCustom){
        free(m_problemIds);
        m_problemIds = NULL;
        m_problemCount = 0;
    }
    
    return true;
    
};


void ParkDataConfig::updateProblems(cocos2d::CCArray *_problems)
{
    
}



static GameConfig* m_gameConfig = NULL;

GameConfig* GameConfig::sharedGameConfig() {
    if (!m_gameConfig) {
        m_gameConfig = new GameConfig();
        m_gameConfig->init();
        
    }
    return m_gameConfig;
}

bool GameConfig::init()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // iOS
//    STSystemFunction *function = new STSystemFunction;
//    androidWritePath.append(function->getSdCardPath()).append("/Android/data/com.ticktockapps.android_jigsawpuzzle/");
//
//    if(!CCFileUtils::sharedFileUtils()->isFileExist(androidWritePath)){
//    	MyZipUtils *zipUtil = new MyZipUtils;
//    	MyZipUtils->createDirectory(androidWritePath.c_str());
//    }
#elif  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    STSystemFunction *function = new STSystemFunction;
//    if(function->isIosFiveUp()){
//        log("平台大于5.0");
//        platformSavePath = CCFileUtils::sharedFileUtils()->getWritablePath();
//    }else{
//        log("平台小于5.0");
//        platformSavePath = function->getPlatformSavePath();
//    }
//    log("platformSavePath=%s" , platformSavePath.c_str());
#endif
    
    updateUserGameData();
    initMoreConfig();
    initParkConfig();
    initResumeConfig();
    initUserSelectResumeConfig();
    current_request_tag = 1;
    user_last_pieces_number=4;
    game_rotated_status = 0;//0表示未旋转   1表示旋转
    session_play_times = 0;
    isShowRateUs = false;
    play_game_success_times = 0;
//    log("%s" , androidWritePath.c_str());
    return true;
}

void GameConfig::updateRequestTag(){
    current_request_tag++;
}

void GameConfig::updateRotateStatus(int int_status)
{
    game_rotated_status = int_status;
    CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_ROTATE_STATUS, game_rotated_status);
}

void GameConfig::initResumeConfig()
{
    resume_data = new ResumeData();
    
    resume_data->user_time = (CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_RESUME_USER_TIME));
    resume_data->resume_data_str = (CCUserDefault::sharedUserDefault()->getStringForKey(GAME_RESUME_USER_DATA));
    resume_data->resume_park_id = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_RESUME_PACKAGE_STR);
    resume_data->resume_problem_index = CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_RESUME_PROBLEM_INDEX);
    resume_data->resume_group_count = CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_RESUME_GROUP_COUNT);
    resume_data->isResumeMode = (false);
    resume_data->pieceNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_RESUME_PIECES_NUMBER);
}



void GameConfig::initUserSelectResumeConfig()
{
    user_picture_resume_data = NULL;
    user_picture_resume_data = new ResumeData();
    
    user_picture_resume_data->user_time = (CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_RESUME_USER_SELECT_TIME));
    user_picture_resume_data->resume_data_str = (CCUserDefault::sharedUserDefault()->getStringForKey(GAME_RESUME_USER_SELECT_DATA));
    user_picture_resume_data->resume_park_id = STR_USER_SELECT_MODE;
    user_picture_resume_data->resume_group_count = CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_RESUME_USER_SELECT_GROUP_COUNT);
    user_picture_resume_data->isResumeMode = (false);
    user_picture_resume_data->pieceNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_RESUME_USER_SELECT_PIECES_NUMBER);
    user_picture_resume_data->isComplete = CCUserDefault::sharedUserDefault()->getBoolForKey(GAME_RESUME_USER_SELECT_STATUS);
}


//没有Park的概念, 后面可以去掉,或者留一部分为以后做准备.都默认Park1.
void GameConfig::initParkConfig()
{
    auto vv = FileUtils::getInstance()->getValueVectorFromFile(PARK_CONFIG_NAME);
    
//    CCLOG("park_configs num = %d" ,park_configs->count());
    CCAssert(vv.size() != 0, "读取park文档文档错误!");
    m_parkConfigs = CCArray::create();
    m_parkConfigs->retain();
    
    for (int i = 0; i < vv.size(); i++) {
        ValueMap value = vv.at(i).asValueMap();
        ParkDataConfig* config = ParkDataConfig::create(value);
        
        if (config && i<=2) {
            m_parkConfigs->addObject(config);
        }
    }
    
    
    //add purchase package or free package
    cocos2d::liyang::LYString *purchase_str = cocos2d::liyang::LYString::create(CCUserDefault::sharedUserDefault()->getStringForKey(GAME_PURCHASE_ARRAY));
//    log("购买信息 purchase_str=%s" , purchase_str->getCString());
    if(purchase_str->length()==0){
        
        for (int i = 0; i < vv.size(); i++) {
            ValueMap value = vv.at(i).asValueMap();
            ParkDataConfig* config = ParkDataConfig::create(value);
            
            if (config && i>2) {
                m_parkConfigs->addObject(config);
            }
        }
        return;
    }
    
    CCArray *array  = purchase_str->componentsSeparatedByString(",");
    
    int m_problemCount = array->count();
    for (int i=0; i<m_problemCount; i++) {
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
        if(problem_id_string->findFirstOf(";") == problem_id_string->length()-1)
            updateParkAndMore(problem_id_string->substringToIndex(problem_id_string->length()-1)->m_sString);
        else
            updateParkAndMore(problem_id_string->m_sString);
    }
    
    for (int i = 0; i < vv.size(); i++) {
        ValueMap value = vv.at(i).asValueMap();
        ParkDataConfig* config = ParkDataConfig::create(value);
        
        if (config && i>2) {
            m_parkConfigs->addObject(config);
        }
    }
    
    for (int i = 0; i < vv.size(); i++) {
        ValueMap value = vv.at(i).asValueMap();
        ParkDataConfig* config = ParkDataConfig::create(value);
        
        if (config && i>2) {
            m_parkConfigs->addObject(config);
        }
    }
}


void GameConfig::initMoreConfig()
{
    
    string morePath = GameConfig::sharedGameConfig()->getPlatformWritePath();
    morePath.append("more_config.plist");
    CCArray *more_configs;
    if(CCFileUtils::sharedFileUtils()->isFileExist(morePath)){
        
        more_configs = CCArray::createWithContentsOfFileThreadSafe(morePath.c_str());
        
    }else
        more_configs = CCArray::createWithContentsOfFileThreadSafe(more_CONFIG_NAME);
    
    
    log("more num = %d" ,more_configs->count());
    
    CCAssert(more_configs != NULL, "无法读取park文档");
    m_moreConfigs = CCArray::create();
    m_moreConfigs->retain();
    
    for (int i=0; i<more_configs->count(); i++) {
        CCDictionary *dic = (CCDictionary*)more_configs->objectAtIndex(i);
        ParkDataConfig *dataConfig = ParkDataConfig::create(dic);
//        CCLOG("dataConfig name=%s" , dataConfig->m_parkId.c_str());
        if(dataConfig)
            m_moreConfigs->addObject(dataConfig);
    }
}

CCArray* GameConfig::getMoreConfigs()
{
    return m_moreConfigs;
}


CCArray* GameConfig::getParkConfigs()
{
    return m_parkConfigs;
};

float GameConfig::getBannerHeight()
{
    
    return 100;
}

void GameConfig::setCurrentPack(ParkDataConfig *infoConfig)
{
    curPark = infoConfig;
}

ParkDataConfig* GameConfig::getCurrentPark()
{
    return curPark;
}

Vec2 GameConfig::getGameMapLeftTopPoint()
{
    return Vec2((VISIBLE_RECT.getMaxX() - 1024*UI_SCALE)/2, VISIBLE_RECT.getMaxY() - (VISIBLE_RECT.getMaxY() - 1024*UI_SCALE)/2-200*UI_SCALE);
}

Sprite* GameConfig::getGameBg()
{
    string bgPath = GAME_BG_FILE_PRE;
    bgPath.append("background0");
    stringstream str_index ;
    str_index << bg_index;
    bgPath.append(str_index.str());
    bgPath.append(".jpg");
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Sprite *sprite = Util::getUISprite(bgPath.c_str());
    sprite->setPosition(Vec2(0, winSize.height));
    sprite->setAnchorPoint(Vec2(0,1));
//    log("width = %f %f" , sprite->getContentSize().width , sprite->getContentSize().height);
    
    float scaleX = winSize.width/sprite->getContentSize().width;
    float scaleY = winSize.height/sprite->getContentSize().height;
    sprite->setScale(MAX(scaleX, scaleY) );
    return sprite;
}

Sprite* GameConfig::getNormalGameBg()
{
//    string bgPath = GAME_BG_FILE_PRE;
//    bgPath.append("ui01_bg.jpg");
//    stringstream str_index ;
//    str_index << 1;
//    bgPath.append(str_index.str());
//    bgPath.append(".jpg");
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Sprite *sprite = Util::getUISprite("ui01_bg.jpg");
    sprite->setPosition(ccp_center_screen);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    
    float scaleX = winSize.width/sprite->getContentSize().width;
    float scaleY = winSize.height/sprite->getContentSize().height;
    sprite->setScale(MAX(scaleX, scaleY));
    return sprite;
}



Sprite*  GameConfig::changgeGameBg()
{
    bg_index++;
    if(bg_index == bg_number){
        bg_index = 1;
    }
    string bgPath = GAME_BG_FILE_PRE;
    bgPath.append("background0");
    stringstream str_index ;
    str_index << bg_index;
    bgPath.append(str_index.str());
    bgPath.append(".jpg");
    CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_BG_KEY, bg_index);
    
    
    return getGameBg();
}

//void GameConfig::createTable()
//{
//    std::string dbFilePath = CCFileUtils::sharedFileUtils()->getWritablePath()+"my_puzzle.db";
//    int result;
//    sqlite3 *pDB = NULL;//数据库指针
//    char * errMsg = NULL;//错误信息
//    //    CCFileUtils::sharedFileUtils()->getWritablePath()
//    
//    result = sqlite3_open(dbFilePath.c_str(), &pDB);
//    if( result != SQLITE_OK )
//        log( "打开数据库失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
//    else
//        log("成功创建数据库");
//    //    std::string filename = CCFileUtils::sharedFileUtils()->fullPathForFilename("db/my_puzzle.db");
//    
//    
//    string  createTableSQL = "create table if not exists " + std::string(RESUME_GAME_TABLE) + "(id int unsigned auto_increment primary key, tile_index int unsigned,  pos_x float,  pos_y float , group_index int , lock_status int)";
//    result = sqlite3_exec(pDB,createTableSQL.c_str(),NULL,NULL,&errMsg);
//    if( result != SQLITE_OK )
//        log( "创建表失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
//    else{
//        log("创建表 或者 已经读取到表");
//    }
//    
//}

void  GameConfig::saveUserGameData(bool isUserSelectImage , std::string dataStr , int index , int time , int groupSize , int rotate , int number)
{
    if (isUserSelectImage) {
        
        if(dataStr.compare("") != 0){
        rename(GET_GAME_CONFIG->getPlatformWritePath().append(USER_SELECT_IMAGE_NAME).c_str(), GET_GAME_CONFIG->getPlatformWritePath().append(USER_SELECT_SAVE_IMAGE_NAME).c_str());
        rename(GET_GAME_CONFIG->getPlatformWritePath().append(USER_SELECT_IMAGE_THUMB_NAME).c_str(), GET_GAME_CONFIG->getPlatformWritePath().append(USER_SELECT_SAVE_IMAGE_THUMB_NAME).c_str());
        }
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_RESUME_USER_SELECT_DATA, dataStr.c_str());
        CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_USER_SELECT_TIME, time);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_USER_SELECT_GROUP_COUNT, groupSize);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_USER_SELECT_ROTATE, rotate);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_USER_SELECT_PIECES_NUMBER, number);
        CCUserDefault::sharedUserDefault()->setBoolForKey(GAME_RESUME_USER_SELECT_STATUS, false);
        getUserSelectPictureResumeData()->resume_problem_index = index;
        getUserSelectPictureResumeData()->user_time = time;
        getUserSelectPictureResumeData()->resume_data_str = dataStr;
        getUserSelectPictureResumeData()->resume_group_count = groupSize;
        getUserSelectPictureResumeData()->resume_rotate = rotate;
        getUserSelectPictureResumeData()->pieceNumber = number;
        getUserSelectPictureResumeData()->isComplete = false;
    }else{
    log("rotate%d",rotate);
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_RESUME_USER_DATA, dataStr.c_str());
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_RESUME_PACKAGE_STR, getCurrentPark()->m_parkId);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_PROBLEM_INDEX, index);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_USER_TIME, time);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_GROUP_COUNT, groupSize);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_ROTATE, rotate);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_PIECES_NUMBER, number);
    getUserGameData()->resume_problem_index = index;
    getUserGameData()->user_time = time;
    getUserGameData()->resume_park_id = getCurrentPark()->m_parkId;
    getUserGameData()->resume_data_str = dataStr;
    getUserGameData()->resume_group_count = groupSize;
    getUserGameData()->resume_rotate = rotate;
    getUserGameData()->pieceNumber = number;
    }
}

void GameConfig::saveUserVictoryData(int picture_index, const char *time)
{
    
    if(picture_index == 201){
        getUserSelectPictureResumeData()->isComplete = true;
        CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_RESUME_USER_SELECT_STATUS, true);
        return;
    }
    
    stringstream str_picture;
    str_picture << picture_index;
    string packageVitoryStr = GAME_VICTORY_DATA;
    packageVitoryStr.append(getCurrentPark()->m_parkId);
    
    cocos2d::liyang::LYString *string1 = cocos2d::liyang::LYString::create(CCUserDefault::sharedUserDefault()->getStringForKey(packageVitoryStr.c_str()));
    CCArray *array  = string1->componentsSeparatedByString(",");
    if(CCUserDefault::sharedUserDefault()->getStringForKey(packageVitoryStr.c_str()).compare("")==0){
//        log("没有胜利数据");
        CCUserDefault::sharedUserDefault()->setStringForKey( packageVitoryStr.c_str()  , str_picture.str());
        return ;
    }
    
    int m_problemCount = array->count();
    int* m_problemIds = (int*)malloc(sizeof(int)*m_problemCount);
    
//    log("保存的数据有 %s" , string1->getCString());
    bool isNeedSave = true;
    for (int i=0; i<m_problemCount; i++) {
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
        m_problemIds[i] = problem_id_string->intValue();
        if(m_problemIds[i] == picture_index){
            isNeedSave = false;
        }
//        log(" m_problemIds[i]=%d", m_problemIds[i]);
    }
    
    if(isNeedSave){
        std::string save_victory_str;
        for (int i=0; i<m_problemCount; i++) {
            cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
            
            if(problem_id_string->compare(";")!=0){
//                log("problem_id_string=%s",problem_id_string->getCString());
                save_victory_str.append(problem_id_string->getCString());
                save_victory_str.append(",");
            }
        }
        save_victory_str.append(str_picture.str());
        log("save_victory_str = %s" , save_victory_str.c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey( packageVitoryStr.c_str()  , save_victory_str.c_str());
    }
    free(m_problemIds);
}

bool GameConfig::isPictureVictory(int picture_index)
{
    stringstream str_picture;
    str_picture << picture_index;
    
    string packageVitoryStr = GAME_VICTORY_DATA;
    packageVitoryStr.append(getCurrentPark()->m_parkId);
    cocos2d::liyang::LYString *string = cocos2d::liyang::LYString::create(CCUserDefault::sharedUserDefault()->getStringForKey(packageVitoryStr.c_str()));
    CCArray *array  = string->componentsSeparatedByString(",");
    if(!array || array->count() <= 0){
        return false;
    }
//    log("picture_index = %d   save_victory_str = %s" , picture_index ,string->getCString());

    int m_problemCount = array->count();
    int* m_problemIds = (int*)malloc(sizeof(int)*m_problemCount);
    
    for (int i=0; i<m_problemCount; i++) {
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
        m_problemIds[i] = problem_id_string->intValue();
        if(m_problemIds[i] == picture_index){
            return true;
        }
    }
    free(m_problemIds);
    return false;
}


bool GameConfig::isNeeDResumeGuanqia(int problem_id)
{
//    CCLOG("=%d =%d" , getUserGameData()->resume_problem_index , problem_id);
//    CCLOG("%s %s" , getUserGameData()->resume_park_id.c_str() , getCurrentPark()->m_parkId.c_str());

    if(problem_id != 201){
    if(getUserGameData()->resume_park_id.compare("")!=0&&  getUserGameData()->resume_park_id.compare(getCurrentPark()->m_parkId) == 0 && getUserGameData()->resume_problem_index == problem_id && getUserGameData()->isResumeMode){
        return true;
    }
    }else{
        if (getUserSelectPictureResumeData()->isResumeMode  && !getUserSelectPictureResumeData()->resume_data_str.empty()) {
            return true;
        }else{
            return false;
        }
    }
    return false;
}

bool GameConfig::isShowResumeStatus(int problem_id)
{
    if(getUserGameData()->resume_park_id.compare("")!=0&&  getUserGameData()->resume_park_id.compare(getCurrentPark()->m_parkId) == 0 && getUserGameData()->resume_problem_index == problem_id){
        return true;
    }
    return false;
}

void GameConfig::removeResumeModeData()
{
    saveUserGameData(false ,"-1", 0, 0 , -1 , 0 , 2);
}

ResumeData*  GameConfig::getUserGameData()
{
    return resume_data;
}

ResumeData*  GameConfig::getUserSelectPictureResumeData()
{
    return user_picture_resume_data;
}




void GameConfig::savePurchsedPackage(string m_park_purcha_name)
{
    
    string puchase_park_id = m_park_purcha_name;
    puchase_park_id.append(";");
    cocos2d::liyang::LYString *purchase_str = cocos2d::liyang::LYString::create(CCUserDefault::sharedUserDefault()->getStringForKey(GAME_PURCHASE_ARRAY));
    
    CCArray *array  = purchase_str->componentsSeparatedByString(";");
    if(purchase_str->length()==0){
        log("没有购买数据  添加一条 %s" ,m_park_purcha_name.c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_PURCHASE_ARRAY , puchase_park_id);
        return ;
    }
    
    int m_problemCount = array->count();
    int* m_problemIds = (int*)malloc(sizeof(int)*m_problemCount);
    
    log("该保存的数据是 %s    已保存的数据有  %s" , m_park_purcha_name.c_str() , purchase_str->getCString());
    
    for (int i=0; i<m_problemCount; i++) {
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
        
        if(problem_id_string->compare(";")!=0){
            log("problem_id_string=%s",problem_id_string->getCString());
            
            if(problem_id_string->compare(m_park_purcha_name.c_str()) == 0)
            {
                CCLOG("这个数据已经保存过 %s" , m_park_purcha_name.c_str());
                return;
            }
        }else{
            break;
        }
    }
    
    //若没有保存数据，则开始保存
    std::string save_puchase_str;
    save_puchase_str.append(purchase_str->getCString());
    save_puchase_str.append(puchase_park_id.c_str());

    
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_PURCHASE_ARRAY , save_puchase_str.c_str());
    log("save_puchase_str = %s" , CCUserDefault::sharedUserDefault()->getStringForKey(GAME_PURCHASE_ARRAY).c_str());
    
    free(m_problemIds);
}

//restore信息
void GameConfig::saveRestorePackage(string m_park_purcha_name)
{
    string puchase_park_id = m_park_purcha_name;
    puchase_park_id.append(";");
    cocos2d::liyang::LYString *purchase_str = cocos2d::liyang::LYString::create(CCUserDefault::sharedUserDefault()->getStringForKey(GAME_PURCHASE_ARRAY));
    CCArray *array  = purchase_str->componentsSeparatedByString(",");
    if(purchase_str->length()==0){
//        log("没有购买数据  添加一条 %s" ,m_park_purcha_name.c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_PURCHASE_ARRAY , puchase_park_id);
        return ;
    }
    
    int m_problemCount = array->count();
    int* m_problemIds = (int*)malloc(sizeof(int)*m_problemCount);
    
//    log("该保存的数据是 %s    已保存的数据有  %s" , m_park_purcha_name.c_str() , purchase_str->getCString());
    
    for (int i=0; i<m_problemCount; i++) {
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
        
            //            log("problem_id_string=%s",problem_id_string->getCString());
            
            if(problem_id_string->compare(m_park_purcha_name.c_str()) == 0)
            {
                CCLOG("这个数据已经保存过 %s" , m_park_purcha_name.c_str());
                return;
            }
       
    }

    //若没有保存数据，则开始保存
    std::string save_puchase_str;
    save_puchase_str.append(purchase_str->getCString());
    save_puchase_str.append(puchase_park_id.c_str());
    
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_PURCHASE_ARRAY , save_puchase_str.c_str());
    log("restore_puchase_str = %s" , CCUserDefault::sharedUserDefault()->getStringForKey(GAME_PURCHASE_ARRAY).c_str());
    
    free(m_problemIds);
}

bool GameConfig::isHaveBuyPackage(std::string m_packageName)
{
    log("%s"  ,m_packageName.c_str());
    cocos2d::liyang::LYString *purchase_str = cocos2d::liyang::LYString::create(CCUserDefault::sharedUserDefault()->getStringForKey(GAME_PURCHASE_ARRAY));
    CCArray *array  = purchase_str->componentsSeparatedByString(";");
    for (int i=0; i<array->count(); i++) {
        cocos2d::liyang::LYString *problem_id_string = (cocos2d::liyang::LYString*)array->objectAtIndex(i);
        
            if(problem_id_string->compare(m_packageName.c_str()) == 0)
            {
                log("这个包买过了 哈哈哈 %s" , m_packageName.c_str());
                return true;
            }
       
    }
    return false;
}

void GameConfig::updateParkAndMore(string m_park_purchase_name)
{
//    log("更新%s" , m_park_purchase_name.c_str());
//    string packagePurchaseStr = GAME_PURCHASE_ARRAY;
//    
//    cocos2d::liyang::LYString *purchase_str = cocos2d::liyang::LYString::create(CCUserDefault::sharedUserDefault()->getStringForKey(packagePurchaseStr.c_str()));
//    if(purchase_str->length()==0){
//        log("没有购买数据  添加一条 ");
//    }
//    log("保存的数据有 %s" , purchase_str->getCString());
//    Ref* obj = NULL;
//    CCARRAY_FOREACH(getMoreConfigs(),obj){
//        
//        ParkDataConfig  *child = (ParkDataConfig*)obj;
//        if(child->m_parkLockName.compare(m_park_purchase_name) == 0){
//            log("更新成功");
//            getParkConfigs()->insertObject(child, 3);
//            getMoreConfigs()->removeObject(child);
//            break;
//        }
//    }
}

bool GameConfig::isHaveTreyMyPhoto()
{
   return  CCUserDefault::sharedUserDefault()->getBoolForKey(GAME_TRY_MY_PHOTO , false);
};

void GameConfig::setHaveTreyMyPhoto(bool isTry)
{
    CCUserDefault::sharedUserDefault()->setBoolForKey(GAME_TRY_MY_PHOTO , isTry);
}


void GameConfig::updateUserGameData()
{
    bg_index = CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_BG_KEY, 1);
    IS_PURCHASED = CCUserDefault::sharedUserDefault()->getBoolForKey(GAME_FUNCTION_PURCHASE_, false);
//    if(DEBUG_MODE){
//        IS_PURCHASED = true;
//    }
}

void GameConfig::savePurchsedFunction(bool isPurchase)
{
    IS_PURCHASED = isPurchase;
    CCUserDefault::sharedUserDefault()->setBoolForKey(GAME_FUNCTION_PURCHASE_, isPurchase);
}

std::string GameConfig::getPackageThumbShowPath(std::string fileName, int type)
{
    if(type == SELE_PICTURE)
    {
        string my_path = THUMB_FILE_PRE;
        my_path.append(fileName);
        my_path.append("_1_thumb");
        my_path.append(JPG_SUF);
//        log("my_path=%s",my_path.c_str());
        return my_path;
    }else if(type == MACHINE_PICTURE)
    {
        return (GET_PLATFORM_SAVE_PATH + fileName + "_1_thumb" + JPG_SUF);
    }
    return "";
}


std::string GameConfig::getPackageThumbPicturePath(std::string fileName, int index ,int type)
{
    if(index == 201){
        return getUserSelectPicturePath(1);
        
    }
    
    string my_path;
    if(type == SELE_PICTURE)
    {
        string my_path = THUMB_FILE_PRE;
        my_path.append(fileName);
        my_path.append("_");
        my_path.append(Util::convertInt(index));
        my_path.append("_thumb.jpg");
        return my_path.c_str();
    }else if(type == MACHINE_PICTURE)
    {
        
        my_path = GameConfig::sharedGameConfig()->getPlatformWritePath();
        my_path.append(getCurrentPark()->m_parkName);
        my_path.append("_");
        my_path.append(Util::convertInt(index));
        my_path.append("_thumb.jpg" );
    }
    return my_path;
}

std::string GameConfig::getPackageBigPicturePath(std::string fileName ,  int index, int type)
{
    
    if(type == SELE_PICTURE)
    {
        string my_path = NORMAL_FILE_PRE;
        my_path.append(fileName);
        my_path.append("_");
        my_path.append(Util::convertInt(index));
        my_path.append(JPG_SUF);
        return my_path;
    }else if(type == MACHINE_PICTURE)
    {
        return (GET_PLATFORM_SAVE_PATH + fileName + "_" + Util::convertInt(index) + JPG_SUF);
    }
}

string GameConfig::getPackageThumbPicturePath_str(std::string fileName ,  int index, int type){
    
    string my_path;
    if(type == SELE_PICTURE)
    {
        string my_path = THUMB_FILE_PRE;
        my_path.append(fileName);
        my_path.append("_");
        my_path.append(Util::convertInt(index));
        my_path.append("_thumb");
        my_path.append(JPG_SUF);
        return my_path.c_str();
    }else if(type == MACHINE_PICTURE)
    {
        
        return (GET_PLATFORM_SAVE_PATH + getCurrentPark()->m_parkName+ "_" + Util::convertInt(index) +"_thumb.jpg" );
    }
}

std::string GameConfig::getUserSelectPicturePath(int type){
    
    if(type == 0){ //大图
        if(CCFileUtils::sharedFileUtils()->isFileExist(getPlatformWritePath().append(USER_SELECT_IMAGE_NAME))){
            return getPlatformWritePath().append(USER_SELECT_IMAGE_NAME);
        }
        return getPlatformWritePath().append(USER_SELECT_SAVE_IMAGE_NAME);
    }if(type == 1){//小图
        if(CCFileUtils::sharedFileUtils()->isFileExist(getPlatformWritePath().append(USER_SELECT_IMAGE_THUMB_NAME))){
            return getPlatformWritePath().append(USER_SELECT_IMAGE_THUMB_NAME);
        }
        return getPlatformWritePath().append(USER_SELECT_SAVE_IMAGE_THUMB_NAME);
    }
    
}



std::string GameConfig::getDownLoadZipUrl(ParkDataConfig *parkInfo)
{
    string my_path = ZIP_URL_SUF;
    my_path.append(parkInfo->m_parkName);
    my_path.append(".zip");
    return my_path;
}


std::string GameConfig::getGuideMask(int index)
{
    string path = "UI/mask/"+Util::convertFloat(index*index);
    if(IS_2048){
        path.append("-hd.png");
    }else{
        path.append(".png");
    }
    
    return path;
};

//std::string GameConfig::getAndroidTempFilePath()
//{
//    
//    return (CCFileUtils::sharedFileUtils()->getWritablePath() + "demo.png");
//}

std::string GameConfig::getPlatformWritePath()
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    
    return CCFileUtils::sharedFileUtils()->getWritablePath();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    return androidWritePath;
#endif
};

std::string GameConfig::getZipSavePath(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    
    STSystemFunction *function = new STSystemFunction;
    return function->getPlatformTempPath();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    return androidWritePath;
#endif
}

void GameConfig::rateUs(){
    
    STSystemFunction *rateFunction = new STSystemFunction;
    isShowRateUs = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    rateFunction->rating(apple_id, GAME_RATE_INFO);
#else
    rateFunction->rating();
#endif
}

bool GameConfig::isFirstPlayGame()
{
   bool isPlay =  CCUserDefault::sharedUserDefault()->getBoolForKey(IS_PLAY_GAME, true);
    if (isPlay) {
        CCUserDefault::sharedUserDefault()->setBoolForKey(IS_PLAY_GAME, false);
    }
    return  isPlay;
}

Sprite* GameConfig::getCurrentBg(){
    Size winSize = Director::getInstance()->getWinSize();
    std::stringstream path;
    path<<"UI/ui09_background/background0";
    path<<  bg_index;
    path<<".jpg";

    Sprite *sprite = Sprite::create(path.str().c_str());
    sprite->setPosition(ccp_center_screen);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    
    float scaleX = winSize.width/sprite->getContentSize().width;
    float scaleY = winSize.height/sprite->getContentSize().height;
    sprite->setScale(MAX(scaleX, scaleY));
    return sprite;
    
    
}


Sprite* GameConfig::getCurrentScrollBg(){
    Size winSize = Director::getInstance()->getWinSize();
    std::stringstream path;
    path<<"UI/ui09_background/ui09_bar0";
    path<<  bg_index;
    path<<".png";
    
    Sprite *sprite = Sprite::create(path.str().c_str());
    sprite->setAnchorPoint(Vec2(0, 0));
    float scaleX = winSize.width/sprite->getContentSize().width;
    sprite->setScale(scaleX);
    return sprite;
    
    
}
