//
//  CBDownloadPackCommand.h
//  ColorBook
//
//  Created by maxiang on 5/6/15.
//
//

#ifndef __ColorBook__CBDownloadPackCommand__
#define __ColorBook__CBDownloadPackCommand__

#include "cocos2d.h"
#include "CBAppGlobal.h"
#include "../Widget/CBServerPackSprite.h"

class DownloadPackCommand
{
public:
    
    virtual ~DownloadPackCommand();
    
    /* Init with pack name and pack logo */
    bool init(const std::string& packName, ServerPackSprite *packLogo);
    
    /* Begin download */
    void execute();
    
    /* When finished download server pack, call this methods to fake a progress timer */
    void finishDownload(bool success);
    
    std::string getPackName(){return _packName;};
    ServerPackSprite* getPackLogo(){return _packLogo;};
    
    /* download failed layer call back */
    void continueDownload();
    void cancelDownload();
    
protected:
    void progressTimerCallback();

protected:
    std::string _packName;
    ServerPackSprite *_packLogo;
};

#endif /* defined(__ColorBook__CBDownloadPackCommand__) */
