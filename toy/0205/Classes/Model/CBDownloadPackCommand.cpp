//
//  CBDownloadPackCommand.cpp
//  ColorBook
//
//  Created by maxiang on 5/6/15.
//
//

#include "CBDownloadPackCommand.h"
#include "CBPackManager.h"
#include "../Layer/CBDownloadFaildLayer.h"
#include "PackLayer.h"

USING_NS_CC;

DownloadPackCommand::~DownloadPackCommand()
{
    CC_SAFE_RELEASE_NULL(_packLogo);
}

bool DownloadPackCommand::init(const std::string& packName, ServerPackSprite *packLogo)
{
    _packName = packName;
    
    //CC_SAFE_RELEASE_NULL(_packLogo);
    _packLogo = packLogo;
    _packLogo->retain();
    
    return true;
}

void DownloadPackCommand::execute()
{
    _packLogo->changeToDownloadingStyle();

    ProgressTimer *timer = _packLogo->getProgressTimer();
    timer->setPercentage(1);
    timer->runAction(ProgressTo::create(60, 90));
    
    PackManager::getInstance()->downloadServerPack(_packName);
}

void DownloadPackCommand::progressTimerCallback()
{
    _packLogo->changeToHasDownloadStyle();
    delete this;
}

void DownloadPackCommand::finishDownload(bool success)
{
    if (success)
    {
        ProgressTimer *timer = _packLogo->getProgressTimer();
        timer->runAction(Sequence::create(ProgressTo::create(1, 100), CallFunc::create(CC_CALLBACK_0(DownloadPackCommand::progressTimerCallback, this)), NULL));
        
        std::string userDefaultKey = UserDefaultKey_HasDownloadServerPack + _packName;
        UserDefault::getInstance()->setBoolForKey(userDefaultKey.c_str(), true);
        UserDefault::getInstance()->flush();
    }
    else
    {
        //if PackScene is running scene, show the download failed layer
        auto scene = Director::getInstance()->getRunningScene();
        PackLayer *packLayer = dynamic_cast<PackLayer*>(scene->getChildren().at(0));
        if (packLayer)
        {
            auto downloadFailedLayer = DownloadFaildLayer::create();
            downloadFailedLayer->setYesActionCallback(CC_CALLBACK_0(DownloadPackCommand::continueDownload, this));
            downloadFailedLayer->setNoActionCallback(CC_CALLBACK_0(DownloadPackCommand::cancelDownload, this));
            packLayer->addChild(downloadFailedLayer, 1000);
        }
        
        //restore to not download style
        _packLogo->changeToNotDownloadStyle();
        ProgressTimer *timer = _packLogo->getProgressTimer();
        timer->setPercentage(2);
    }
}

void DownloadPackCommand::continueDownload()
{
    PackManager::getInstance()->addDownloadPackCommandToQueue(this);
}

void DownloadPackCommand::cancelDownload()
{
    delete this;
}