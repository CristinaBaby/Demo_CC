//
//  PackManager.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef __ColorBook__CBPackManager__
#define __ColorBook__CBPackManager__

#include "cocos2d.h"
#include "CBAppGlobal.h"
#include "../../cocos2dx/cocos2d/cocos/network/HttpClient.h"
#include "CBDownloadPackCommand.h"

#define xPack       PackManager::getInstance()

struct PackInfo
{
    std::string     packName;
    std::string     logoName;
    std::string     iOSPurchaseIdentifier;
    std::string     androidPurchaseIdentifier;

    int             packId;
    int             itemsCount;
    bool            isPurchased;
    bool            isServerPack;
};

class PackManager : public cocos2d::Ref
{
public:
    static PackManager* getInstance();
    virtual ~PackManager();
    
    /* get packs root path, /device write able path/ColorBookPacks/ */
    std::string getPacksRootPath(bool withWriteablePath);
    
    /* get pack path with pack index */
    std::string getPackPath(const int packIndex, bool withWriteablePath);
    
    /* get pack path with pack name */
    std::string getPackPath(const std::string& packName, bool withWriteablePath);
    
    /* get pack logo path pack name */
    std::string getPackLogoPath(const std::string& packName);
    
    /* get pack picture path */
    std::string getPackPicturePath(const int packIndex,
                                   const int pictureIndex,
                                   bool withWriteablePath);
    
    std::string getPackSVGPath(const int packIndex,
                                   const int pictureIndex,
                                   bool withWriteablePath);
    
    /* get pack history picture path, saved by user */
    std::string getPackHistoryPicturePath(const int packIndex,
                                          const int pictureIndex,
                                          bool withWriteablePath);
    
    /* get history logo, used in CBPictureSene */
    std::string getPackHistoryLogoPath(const int packIndex,
                                       const int pictureIndex,
                                       bool withWriteablePath);
    
    /* get pack mask path */
    std::string getPackMaskPicturePath(const int packIndex,
                                       const int pictureIndex,
                                       bool withWriteablePath);
    
    bool isPackPurchasedIdentifier(const std::string& identifier);
    /* get pack purchase identifier, indicated by platform */
    std::string getPackPurchasedIdentifier(int index);
    std::string getPackPurchasedIdentifier(PackInfo* pack);

    /* unzip zip file */
    bool unzipFile(const std::string& filePath);
    
    /* create directory in writeable path */
    bool createDirectory(const char *path);
    
    /* setup packs info, common method, used by local pack and server pack */
    bool setupPacksInfo(const std::string& path, std::vector<PackInfo>& packs);
    
    /* setup local packs */
    bool setupLocalPacksInfo(const std::string& path);
    
    /* setup server packs, will check if has it */
    bool setupServerPacksInfo(const std::string& path);

    /* get pack info with pack index */
    PackInfo* getPackInfo(const int index);
    
    /* get special pack, special pack will unlock one unlock pack and pattern pen */
    PackInfo* getSpecialPackInfo(){return &_specialPack;};

    /* get packs count, NOT include special pack */
    int getPackCount() const;
    
    /* check if has sepecial pack */
    bool checkHasSpecialPack();
    
    /* http request, common method */
    void httpRequest(const std::string& tag, const std::string& url);

    /* download server pack update info, check if has new pack available */
    void checkServerPackUpdateInfo();
    
    /* finish download server pack info plist, begin check */
    void finishedCheckServerPackUpdateInfo(cocos2d::network::HttpResponse* response);
    
    /* add download pack command to download queue, FIFO */
    void addDownloadPackCommandToQueue(DownloadPackCommand* command);
    
    /* download server pack, DO NOT call this method directly, you may create download 
       command, and add it to download queue */
    void downloadServerPack(const std::string& packName);
    
    /* get download pack command by pack name */
    DownloadPackCommand* getDownloadCommand(const std::string& packName);
    
    /* get current downloading pack, in the front of queue */
    DownloadPackCommand* getCurrentDownloadingCommand();
    
    /* download server pack logo */
    void downloadServerPackLogo(const std::string& logoName);
    void finishedDownloadServerPackLogo(cocos2d::network::HttpResponse* response);
    
    /* finnish download server pack, unzip zip file and write it to pack path */
    void finishedDownloadServerPack(cocos2d::network::HttpResponse* response);
    
    /* HttpClient call back */
    void httpRequestFinished(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
    
    /* if new server pack available, DO NOT dump them to _allPacks immediately,
       instead, dump them to _tmpPacks, when user into CBPackScene, then dump to 
       _allPacks */
    void refreshPacks();
    
    /* when user purchased the special pack, unlock pattern pen and a random unlock pack
     */
    void unlockSpecialPack();
    
    void getLoadPath(int packIndex, int pictureIndex, std::vector<std::string> &paths);
    
    void selectPicture(int pack, int index);
    
    bool isSamePicture(int pack, int index);
 
protected:
    PackManager();
    
    /* setup packs root path */
    void setPacksRootPath(const std::string& path);
    
    /* common method, used by many path methods */
    std::string getPath(const int packIndex,
                        const int pictureIndex,
                        bool withWriteablePath,
                        const std::string& name);
    
    /* when finish download one pack, refresh download queue, continue download next
       pack */
    void refreshDownloadQueue();

protected:
    std::vector<PackInfo> _allPacks;
    std::vector<PackInfo> _localPacks;
    std::vector<PackInfo> _serverPacks;
    PackInfo _specialPack;
    
    /* if find new server pack, refresh all packs to _tmpAllPacks, when user into CBPackScene, dump to _allPacks
     */
    std::vector<PackInfo> _tmpAllPacks;
    
    /* This is the server pack download queue, store pack name, FIFO */
    std::vector<DownloadPackCommand*> _downloadQueue;

    /* Current downloading pack */
    std::string _currentDownloadPack;
    
    /* Indicated if is downloading */
    bool _isDownloading;

    /* Pack save path */
    std::string _packsRootPath;
    std::string _wirteablePath;
    std::string _packsRootPathWithWriteablePath;
    std::string _packsLogoPathWithWriteablePath;
    
    int _lastPack = 0;
    int _lastIndex = 0;
};

#endif /* defined(__ColorBook__CBPackManager__) */
