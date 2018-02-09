//
//  CBPackManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#include "PackManager.h"
#include "AppConfigs.h"
#include "../Model/CBUtils.h"
#include "CBPackScene.h"
#include "TimerManager.hpp"

#include <stdio.h>
#include <vector>
#include <thread>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <dirent.h>
#endif

#include "../../cocos2dx/cocos2d/external/unzip/unzip.h"

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

USING_NS_CC;
using namespace network;

static PackManager *s_SingletonPackManager = nullptr;

PackManager* PackManager::getInstance()
{
    if (!s_SingletonPackManager)
    {
        s_SingletonPackManager = new (std::nothrow)(PackManager);
        
        //setup packs save dir
        s_SingletonPackManager->setPacksRootPath(PACKS_ROOT_DIR_NAME);
        
        CCASSERT(s_SingletonPackManager, "ABORT: create PackManager failed");
    }
    
    return s_SingletonPackManager;
}

PackManager::PackManager()
{
    setupLocalPacksInfo(LOCAL_PACKS_FILE_NAME);
    setupServerPacksInfo(SERVER_PACKS_FILE_NAME);
}

PackManager::~PackManager()
{
    s_SingletonPackManager = nullptr;
}

#pragma mark- Packs Save Path

void PackManager::setPacksRootPath(const std::string& path)
{
    //writeable path
    _wirteablePath = FileUtils::getInstance()->getWritablePath();
    log("MX: Writeable path: %s", _wirteablePath.c_str());
    
    std::string packsFullPath = _wirteablePath + path;
    bool isExist = FileUtils::getInstance()->isFileExist(packsFullPath);
    if (!isExist) {
        createDirectory(packsFullPath.c_str());
    }
    
    _packsRootPath = std::string(PACKS_ROOT_DIR_NAME) + "/";
    _packsRootPathWithWriteablePath = std::string(packsFullPath) + "/";
    
    //create pack logo document
    _packsLogoPathWithWriteablePath = _packsRootPathWithWriteablePath + PACK_LOGO_DIR_NAME + "/";
    isExist = FileUtils::getInstance()->isFileExist(_packsLogoPathWithWriteablePath);
    if (!isExist) {
        createDirectory(_packsLogoPathWithWriteablePath.c_str());
    }
}

std::string PackManager::getPacksRootPath(bool withWriteablePath)
{
    if (withWriteablePath)
    {
        return _packsRootPathWithWriteablePath;
    }
    else
    {
        return _packsRootPath;
    }
}

std::string PackManager::getPackPath(const int packIndex, bool withWriteablePath)
{
    int length = strlen(PACK_DIR_NAME) + PACK_NAME_MAX_LENGTH + 2;
    char packPath[length];
    sprintf(packPath, "%s%d/",PACK_DIR_NAME, packIndex);
    
    std::string packPathString = std::string(packPath);
    std::string packFullPath = _packsRootPathWithWriteablePath + packPathString;
    bool isExist = FileUtils::getInstance()->isFileExist(packFullPath.c_str());
    if (!isExist)
    {
        createDirectory(packFullPath.c_str());
    }
    
    if (withWriteablePath)
    {
        return packFullPath;
    }
    else
    {
        return _packsRootPath + packPath;
    }
}

std::string PackManager::getPackPath(const std::string& packName, bool withWriteablePath)
{
    if (withWriteablePath)
    {
        return _packsRootPathWithWriteablePath + packName;
    }
    else
    {
        return _packsRootPath + packName;
    }
}

std::string PackManager::getPackLogoPath(const std::string& packName)
{
    return _packsLogoPathWithWriteablePath + packName + "_Logo.png";
}

std::string PackManager::getPackPicturePath(const int packIndex,
                                            const int pictureIndex,
                                            bool withWriteablePath)
{
    return getPath(packIndex, pictureIndex, withWriteablePath, PACK_PICTURE_NAME);
}

std::string PackManager::getPackSVGPath(const int packIndex,
                                            const int pictureIndex,
                                            bool withWriteablePath)
{
    return getPath(packIndex, pictureIndex, withWriteablePath, PACK_SVG_NAME);
}

std::string PackManager::getPackHistoryPicturePath(const int packIndex,
                                                   const int pictureIndex,
                                                   bool withWriteablePath)
{
    return getPath(packIndex, pictureIndex, withWriteablePath, PACK_HISTORY_PICTURE_NAME);
}

std::string PackManager::getPackHistoryLogoPath(const int packIndex,
                                                const int pictureIndex,
                                                bool withWriteablePath)
{
    return getPath(packIndex, pictureIndex, withWriteablePath, PACK_HISTORY_PICTURE_LOGO);
}

std::string PackManager::getPackMaskPicturePath(const int packIndex,
                                                const int pictureIndex,
                                                bool withWriteablePath)
{
    return getPath(packIndex, pictureIndex, withWriteablePath, PACK_PICTURE_MASK_NAME);
}

std::string PackManager::getPath(const int packIndex,
                    const int pictureIndex,
                    bool withWriteablePath,
                    const std::string& name)
{
    std::string indeString;
    std::string format;
    if (pictureIndex < 10)//stupid!!!!
    {
        format = "L0%d_%s";
    }
    else
    {
        format = "L%d_%s";
    }
    
    char indexStringChar[PICTURE_NAME_MAX_LENGTH];
    sprintf(indexStringChar, format.c_str(), pictureIndex, name.c_str());
    indeString = std::string(indexStringChar);
    
    if (withWriteablePath)
    {
        return getPackPath(packIndex, true) + indeString;
    }
    else
    {
        return getPackPath(packIndex, false) + indeString;
    }
}

#pragma mark- Packs info

bool PackManager::checkHasSpecialPack()
{
    /*
     If one of the conditions find, remove special pack
     1. User unlock pattern pen
     2. There is no unlocked pack
     */
    bool hasUnlockPatternPen = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_PatternPenHasUnlocked, false);
    if (hasUnlockPatternPen) {
        return false;
    }
    
    for (auto iterator = _allPacks.begin(); iterator != _allPacks.end(); ++iterator)
    {
        std::string purchaseIdentifier;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        purchaseIdentifier = iterator->iOSPurchaseIdentifier;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        purchaseIdentifier = iterator->androidPurchaseIdentifier;
#else
#error "Write platform code here!"
#endif
        if (purchaseIdentifier.size() != 0)
        {
            std::string userDefault = UserDefaultKey_HasPurchasedPack + purchaseIdentifier;
            bool hasPurchase = UserDefault::getInstance()->getBoolForKey(userDefault.c_str(), false);
            if (!hasPurchase) {
                return true;
            }
        }
    }
    
    CCASSERT(false, "这个后面来写");
    
    return false;
}

int PackManager::getPackCount() const
{
    return (int)_allPacks.size();
}

bool PackManager::setupLocalPacksInfo(const std::string& path)
{
    setupPacksInfo(path, _localPacks);
    
    if (_localPacks.size() != 0)
    {
        for (auto itor = _localPacks.begin(); itor != _localPacks.end(); ++itor)
        {
            itor->isServerPack = false;
            _allPacks.push_back(*itor);
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

bool PackManager::setupServerPacksInfo(const std::string& path)
{
    //in writeable path
    std::string packPath = _packsRootPathWithWriteablePath + path;
    
    bool isExsit = FileUtils::getInstance()->isFileExist(packPath);
    
    if (isExsit)
    {
        setupPacksInfo(packPath, _serverPacks);
        
        if (_serverPacks.size() != 0)
        {
            for (auto itor = _serverPacks.begin(); itor != _serverPacks.end(); ++itor)
            {
                itor->isServerPack = true;
                _allPacks.push_back(*itor);
            }
            
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool PackManager::setupPacksInfo(const std::string& path, std::vector<PackInfo>& packs)
{
    packs.clear();
    
    ValueMap map = FileUtils::getInstance()->getValueMapFromFile(path);
    if (map.empty()) {
        return false;
    }
    
    ValueVector items = map[PackInfoKey_PacksInfo].asValueVector();
    if (items.empty()) {
        return false;
    }
    
    //clear old data
    packs.clear();

    //init new data
    for (auto itor = items.begin(); itor != items.end(); )
    {
        auto dic = itor->asValueMap();
        PackInfo info;
        info.packId = dic[PackInfoKey_PackId].asInt();
        info.packName = dic[PackInfoKey_PackName].asString();
        info.logoName = dic[PackInfoKey_Logo].asString();
        info.itemsCount = dic[PackInfoKey_ItemsCount].asInt();
        info.iOSPurchaseIdentifier = dic[PackInfoKey_iOSPurchaseIdentifier].asString();
        info.androidPurchaseIdentifier = dic[PackInfoKey_AndroidPurchaseIdentifier].asString();
        
        if (info.packName == PackInfoKey_SpecialPack)
        {
            _specialPack = info;
        }
        else
        {
            packs.push_back(info);
        }
        ++itor;
    }
    
    if (packs.empty()) {
        return false;
    }
  
    return true;
}

PackInfo* PackManager::getPackInfo(const int index)
{
    if (_allPacks.empty() || _allPacks.size() < index+1)
        return nullptr;
    
    return &_allPacks[index];
}

bool PackManager::isPackPurchasedIdentifier(const string& identifier)
{
    for (int i = 0; i < getPackCount(); i ++) {
        if (_allPacks.at(i).isPurchased && identifier == getPackPurchasedIdentifier(i)) {
            return true;
        }
    }
    
    return false;
}

std::string PackManager::getPackPurchasedIdentifier(int index)
{
    return std::move(getPackPurchasedIdentifier(getPackInfo(index)));
}

std::string PackManager::getPackPurchasedIdentifier(PackInfo* pack)
{
    //if is server pack, check if is paid pack
    std::string purchaseIdentifier;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    purchaseIdentifier = pack->iOSPurchaseIdentifier;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    purchaseIdentifier = pack->androidPurchaseIdentifier;
#else
    #error "Write platform code here!"
#endif
    
    return purchaseIdentifier;
}

void PackManager::refreshPacks()
{
    if (_tmpAllPacks.size() == 0) {
        return;
    }
    
    _allPacks.clear();
    for (auto iterator = _tmpAllPacks.begin(); iterator != _tmpAllPacks.end(); ++iterator)
    {
        _allPacks.push_back(*iterator);
    }
    
    _tmpAllPacks.clear();
}

void PackManager::unlockSpecialPack()
{
    for (auto iterator = _allPacks.begin(); iterator != _allPacks.end(); ++iterator)
    {
        std::string iapIdentifier = getPackPurchasedIdentifier(&(*iterator));
        if (iapIdentifier.size() > 0)
        {
            std::string userDefault = UserDefaultKey_HasPurchasedPack + iapIdentifier;
            bool hasPurchased = UserDefault::getInstance()->getBoolForKey(userDefault.c_str(), false);
            if (!hasPurchased)
            {
                UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
                UserDefault::getInstance()->flush();
                break;
            }
        }
    }
    
    CCASSERT(false, "这个后面来写");
    //ColouringManager::getInstance()->unlockPatternPen();
}

#pragma mark- Check Server Pack Info

void PackManager::checkServerPackUpdateInfo()
{
    httpRequest(HttpRequestTag_CheckPacksUpdateInfo, URL_SERVER_PACKS_INFO);
}

void PackManager::finishedCheckServerPackUpdateInfo(HttpResponse* response)
{
    std::vector<char> *data = response->getResponseData();
    std::string stream = std::string(data->begin(), data->end());
    
    std::string rootPath = _packsRootPathWithWriteablePath + "temp" + SERVER_PACKS_FILE_NAME;
    FILE *fp = fopen(rootPath.c_str(), "wb+");
    fwrite(stream.c_str(), 1, stream.size(), fp);
    fclose(fp);
    
    /* ------------------IMPORTANT------------------
       Check the data integrity here, if one of the conditions find, this pack 
       may be has some problem:
       1. New pack size less than old pack size(maybe not good)
       2. New pack data not same as old one(maybe not good)
     */
    std::vector<PackInfo> packs;
    if (!setupPacksInfo(rootPath, packs)){
        remove(rootPath.c_str());
        return;
    }
    
    //compare with the old server pack info
    /* ------------------IMPORTANT------------------
       if we don't check the size, server can controls client server pack info, for
       example, if old server pack info has 5 packs, we upload new server pack info,
       include only 1 pack, then, this file will replace old one, user can only see
       one server pack, even if they have purchase the other ones 
     */
//    if (packs.size() < _serverPacks.size() && _serverPacks.size() > 0) {
//        remove(rootPath.c_str());
//        return;
//    }
    
    for (int i = 0; i < _serverPacks.size(); ++i)
    {
        PackInfo *oldPack = &_serverPacks.at(i);
        PackInfo *newPack = &packs.at(i);
        
        if (oldPack->packName != newPack->packName) {
            //maybe upload wrong pack info, abort
            remove(rootPath.c_str());
            return;
        }
    }
    
    //if pass the check, replace old server pack info with new one
    _tmpAllPacks.clear();
    for (auto itor = _localPacks.begin(); itor != _localPacks.end(); ++itor)
    {
        itor->isServerPack = false;
        _tmpAllPacks.push_back(*itor);
    }
    
    _serverPacks.clear();
    for (auto itor = packs.begin(); itor != packs.end(); ++itor)
    {
        itor->isServerPack = true;
        _serverPacks.push_back(*itor);
        _tmpAllPacks.push_back(*itor);
        
        /* used to debug */
//        std::string userDefaultKey = UserDefaultKey_HasDownloadServerPack + itor->packName;
//        UserDefault::getInstance()->setBoolForKey(userDefaultKey.c_str(), false);
//        UserDefault::getInstance()->flush();
        /* used to debug */
        
    }

    //replace plist file
    remove(rootPath.c_str());
    
    rootPath = _packsRootPathWithWriteablePath + SERVER_PACKS_FILE_NAME;
    fp = fopen(rootPath.c_str(), "wb+");
    fwrite(stream.c_str(), 1, stream.size(), fp);
    fclose(fp);
    
    for (auto iterator = packs.begin(); iterator != packs.end(); ++iterator)
    {
        std::string packPath = _packsRootPathWithWriteablePath + iterator->packName;
        
        //check the pack logo is exsit?
        std::string packLogoPath = _packsLogoPathWithWriteablePath + iterator->logoName;
        bool isExsit = FileUtils::getInstance()->isFileExist(packLogoPath);
        if (!isExsit)
        {
            //if pack logo not exsit, download it
            downloadServerPackLogo(iterator->logoName);
        }
    }
}

#pragma mark- Download Server Pack Logo

void PackManager::downloadServerPackLogo(const std::string& logoName)
{
    std::string packLogoURL = URL_SERVER_PACK_LOGO + logoName;
    std::string tag = HttpRequestTag_DownloadPackLogo + logoName;
    
    httpRequest(tag, packLogoURL);
}

void PackManager::finishedDownloadServerPackLogo(HttpResponse* response)
{
    std::string tag = response->getHttpRequest()->getTag();
    
    std::string::size_type n = tag.find(HttpRequestTag_DownloadPackLogo);
    if (n != std::string::npos)
    {
        std::string logoName = tag.substr(n+strlen(HttpRequestTag_DownloadPackLogo), tag.size());
        
        std::vector<char> *data = response->getResponseData();
        std::string stream = std::string(data->begin(), data->end());
        
        std::string logoPath = _packsLogoPathWithWriteablePath + logoName;
        FILE *fp = fopen(logoPath.c_str(), "wb+");
        fwrite(stream.c_str(), 1, stream.size(), fp);
        fclose(fp);
    }
}

#pragma mark- Download Server Pack

void PackManager::addDownloadPackCommandToQueue(DownloadPackCommand* command)
{
    _downloadQueue.push_back(command);
    
    refreshDownloadQueue();
}

DownloadPackCommand* PackManager::getCurrentDownloadingCommand()
{
    DownloadPackCommand *command = _downloadQueue.front();
    return command;
}

DownloadPackCommand* PackManager::getDownloadCommand(const std::string& packName)
{
    for (auto iterator = _downloadQueue.begin(); iterator != _downloadQueue.end(); ++iterator)
    {
        if ((*iterator)->getPackName() == packName)
        {
            return *iterator;
        }
    }
    
    return nullptr;
}

void PackManager::refreshDownloadQueue()
{
    if (!_isDownloading && _downloadQueue.size() > 0)
    {
        DownloadPackCommand *command = _downloadQueue.front();
        
        command->execute();
        _isDownloading = true;
    }
}

void PackManager::downloadServerPack(const std::string& packName)
{
    std::string packURL = URL_SERVER_PACK + packName + ".zip";
    std::string tag = HttpRequestTag_DownloadPack + packName;
    
    httpRequest(tag, packURL);
}

void PackManager::finishedDownloadServerPack(HttpResponse* response)
{
    std::string tag = response->getHttpRequest()->getTag();
    
    std::string::size_type n = tag.find(HttpRequestTag_DownloadPack);
    if (n != std::string::npos)
    {
        std::string packName = tag.substr(n+strlen(HttpRequestTag_DownloadPack), tag.size());
        
        std::vector<char> *data = response->getResponseData();
        std::string stream = std::string(data->begin(), data->end());
        
        std::string packPath = _packsRootPathWithWriteablePath + packName + ".zip";
        FILE *fp = fopen(packPath.c_str(), "wb+");
        fwrite(stream.c_str(), 1, stream.size(), fp);
        fclose(fp);
        
        //unzip pack
        bool success = unzipFile(packPath);
        
        //remove zip file
        remove(packPath.c_str());

        //call back
        DownloadPackCommand *command = _downloadQueue.front();
        command->finishDownload(success);
        
        /* remove command from download queue */
        _downloadQueue.erase(_downloadQueue.begin());

        /* ------------------IMPORTANT------------------
           Don't delete command here, command will do something else in it
         */
        
        _isDownloading = false;
        refreshDownloadQueue();
    }
}

#pragma mark- Http Request

void PackManager::httpRequest(const std::string& tag, const std::string& url)
{
    HttpRequest* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(PackManager::httpRequestFinished,this));
    request->setTag(tag.c_str());
    HttpClient::getInstance()->send(request);
    request->release();
}

void PackManager::httpRequestFinished(HttpClient* client, HttpResponse* response)
{
    if (!response || !response->isSucceed())
    {
        return;
    }
    
    std::string tag = response->getHttpRequest()->getTag();
    
    if (tag == HttpRequestTag_CheckPacksUpdateInfo)
    {
        finishedCheckServerPackUpdateInfo(response);
    }
    
    std::string::size_type n = tag.find(HttpRequestTag_DownloadPackLogo);
    if (n != std::string::npos)
    {
        finishedDownloadServerPackLogo(response);
        return;
    }
    
    n = tag.find(HttpRequestTag_DownloadPack);
    if (n != std::string::npos)
    {
        finishedDownloadServerPack(response);
        return;
    }
}

#pragma mark- Utils

/*
 * Create a direcotry is platform depended.
 */
bool PackManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#endif
}

bool PackManager::unzipFile(const std::string& filePath)
{
    // Open the zip file
    unzFile zipfile = unzOpen(filePath.c_str());
    if (! zipfile)
    {
        log("can not open downloaded zip file %s", filePath.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        log("can not read file global info of %s", filePath.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    log("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  nullptr,
                                  0,
                                  nullptr,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        const std::string fullPath = _packsRootPathWithWriteablePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            //There are not directory entry in some case.
            //So we need to test whether the file directory exists when uncompressing file entry
            //, if does not exist then create directory
            const std::string fileNameStr(fileName);
            
            size_t startIndex=0;
            
            size_t index=fileNameStr.find("/",startIndex);
            
            while(index != std::string::npos)
            {
                const std::string dir = _packsRootPathWithWriteablePath + fileNameStr.substr(0,index);;
                
                FILE *out = fopen(dir.c_str(), "r");
                
                if(!out)
                {
                    if (!createDirectory(dir.c_str()))
                    {
                        CCLOG("can not create directory %s", dir.c_str());
                        unzClose(zipfile);
                        return false;
                    }
                    else
                    {
                        CCLOG("create directory %s",dir.c_str());
                    }
                }
                else
                {
                    fclose(out);
                }
                
                startIndex=index+1;
                
                index=fileNameStr.find("/",startIndex);
                
            }

            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", filePath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    unzClose(zipfile);
    
    return true;
}

void PackManager::getLoadPath(int packIndex, int pictureIndex, std::vector<std::string> &paths)
{
    //pageIndex   12 34    56 78
    for (int i = 0; i < 4; ++i)
    {
        PackInfo *pack = getPackInfo(packIndex);
        if (!pack)
            return;
        
        std::string historyLogo = getPackHistoryLogoPath(packIndex, pictureIndex, true);
        bool isExsit = FileUtils::getInstance()->isFileExist(historyLogo);
        if (isExsit)
        {
            Director::getInstance()->getTextureCache()->removeTextureForKey(historyLogo);
            paths.push_back(historyLogo);
        }
        else
        {
            std::string pictureName = getPackPicturePath(packIndex, pictureIndex, pack->isServerPack);
            if (FileUtils::getInstance()->isFileExist(pictureName))
            {
                paths.push_back(pictureName);
            }
        }
        pictureIndex++;
    }
}

void PackManager::selectPicture(int pack, int index)
{
    _lastPack = pack;
    _lastIndex = index;
}

bool PackManager::isSamePicture(int pack, int index)
{
    if(pack == _lastPack && index == _lastIndex)
    {
        return true;
    }
    else
    {
        return false;
    }
}
