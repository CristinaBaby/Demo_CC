//
//  CBPackPage.cpp
//  ColorBook
//
//  Created by maxiang on 4/23/15.
//
//

#include "CBPackPage.h"
#include "extensions/cocos-ext.h"
#include "CBServerPackSprite.h"
#include "../Scene/CBPictureScene.h"
#include "../Scene/CBPackScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
CB_BEGIN_NAMESPACE

bool PackPage::init()
{
    if (! TableViewCell::init())
    {
        return false;
    }
    
    bool rcode = false;
    do {

        //init app purchase
        _appBilling.setIABDelegate(this);

        rcode = true;
    } while (0);
    
    return rcode;
}

void PackPage::reset()
{
    removeAllChildren();
}

void PackPage::touchEnd(cocos2d::Touch *touch)
{
    Point point = convertToNodeSpace(touch->getLocation());
    
    Sprite *pack1 = dynamic_cast<Sprite*>(getChildByTag(0));
    if (pack1 && pack1->getBoundingBox().containsPoint(point))
    {
        //touch pack 1
        log("Touch pack 1");
        choosePack(_pageIndex * 3, pack1);
        return;
    }
    
    Sprite *pack2 = dynamic_cast<Sprite*>(getChildByTag(1));
    if (pack2 && pack2->getBoundingBox().containsPoint(point))
    {
        //touch pack 2
        log("Touch pack 2");
        choosePack(_pageIndex * 3 + 1, pack2);
        return;
    }
    
    Sprite *pack3 = dynamic_cast<Sprite*>(getChildByTag(2));
    if (pack3 && pack3->getBoundingBox().containsPoint(point))
    {
        //touch pack 3
        log("Touch pack 3");
        choosePack(_pageIndex * 3 + 2, pack3);
        return;
    }
}

void PackPage::reload(const int pageIndex)
{
    _pageIndex = pageIndex;

    //all packs count except special pack
    int allPacksCount = PackManager::getInstance()->getPackCount();
    
    for (int i = 0; i < 3; ++i)
    {
        //packIndex, not pageIndex!!!
        int packIndex = i + pageIndex * 3;
        
        //just return, there is no pack avaliable
        if (packIndex > allPacksCount) {
            break;
        }
        
        //reach the end of packs, check if the special pack avaliable
        if (packIndex == allPacksCount){
            
            bool hasSpecialPack = PackManager::getInstance()->checkHasSpecialPack();
            if (hasSpecialPack) {
                createSpecialPackUI(PackManager::getInstance()->getSpecialPackInfo(), i);
            }
            
            break;
        }
        
        PackInfo *pack = PackManager::getInstance()->getPackInfo(packIndex);
        if (!pack){
            break;
        }

        if (pack->isServerPack)
        {
            createServerPackUI(pack, i);
        }
        else
        {
            createLocalPackUI(pack, i);
        }
    }
}

#pragma mark- Pack ui

void PackPage::createServerPackUI(PackInfo* pack, const int index)
{
    float interval = getContentSize().width / 3;
    float offsetX = 0;
    
    std::string logoPath = PackManager::getInstance()->getPackLogoPath(pack->packName);
    bool isExsit = FileUtils::getInstance()->isFileExist(logoPath);
    if (!isExsit)
    {
        //download pack logo from server
        PackManager::getInstance()->downloadServerPackLogo(pack->packName);
        logoPath = "ui03_pack.png";
    }
    
    //remove the last one
    removeChildByTag(index);
    
    ServerPackSprite *logo = nullptr;
    
    //get download command if has it
    DownloadPackCommand *command = PackManager::getInstance()->getDownloadCommand(pack->packName);
    if (command)
    {
        //download command has been added to download queue, get logo from it
        logo = command->getPackLogo();
    }
    else
    {
        //create logo sprite
        logo = ServerPackSprite::create(logoPath);
        CCASSERT(logo, "Create pack logo failed!");
        logo->setupUI();
        logo->setTag(index);
        offsetX = ((interval - logo->getContentSize().width)/2) * (2 * index + 1) + logo->getContentSize().width * index;
        logo->setPosition(Vec2(offsetX + logo->getContentSize().width/2, getContentSize().height/2));
    }
    addChild(logo);

    //check server pack has been downloaded?
    std::string userDefaultKey = UserDefaultKey_HasDownloadServerPack + pack->packName;
    bool hasDownloaded = UserDefault::getInstance()->getBoolForKey(userDefaultKey.c_str(), false);
    if (!hasDownloaded)
    {
        if (command)
        {
            logo->changeToDownloadingStyle();

            DownloadPackCommand *currentDownload = PackManager::getInstance()->getCurrentDownloadingCommand();
            
            if (currentDownload->getPackName() == pack->packName)
            {
                //this is current downloading pack, fake a progress
                ProgressTimer *timer = logo->getProgressTimer();
                if (timer)
                {
                    float percent = timer->getPercentage();
                    float duration = 1.0/3.0 * (90 - percent);
                    timer->runAction(ProgressFromTo::create(duration, percent, 90));
                }
            }
        }
        else
        {
            logo->changeToNotDownloadStyle();
        }
    }
    else
    {
        logo->changeToHasDownloadStyle();
    }
}

void PackPage::createLocalPackUI(PackInfo* pack, const int index)
{
    float interval = getContentSize().width / 3;
    float offsetX = 0;
    
    std::string logoPath = pack->logoName;
    
    //remove the last one
    removeChildByTag(index);
    
    //create logo sprite
    Sprite *logo = Sprite::create(logoPath);
    CCASSERT(logo, "Create pack logo failed!");
    logo->setTag(index);
    offsetX = ((interval - logo->getContentSize().width)/2) * (2 * index + 1) + logo->getContentSize().width * index;
    logo->setPosition(Vec2(offsetX + logo->getContentSize().width/2, getContentSize().height/2));
    addChild(logo);
    
    //6+ icon, stupid!!!!!
    Sprite *icon = Sprite::create("ui03_icon.png");
    CCASSERT(icon, "Create pack 6+ icon failed!");
    icon->setPosition(Vec2(logo->getContentSize().width - 50, logo->getContentSize().height - 40));
    logo->addChild(icon);
    
    //check if paid pack?
    std::string purchaseIdentifier = PackManager::getInstance()->getPackPurchasedIdentifier(pack);

    //this is purchase pack
    if (purchaseIdentifier.size() > 0) {
        
        //check if has been purchased
        std::string userDefaultKey = UserDefaultKey_HasPurchasedPack + purchaseIdentifier;
        bool hasPurchased = UserDefault::getInstance()->getBoolForKey(userDefaultKey.c_str(), false);
        
        if (!hasPurchased) {
            
            auto purchase = Sprite::create("ui03_lable_lockpattern.png");
            purchase->setPosition(Vec2(logo->getContentSize().width/2,
                                       purchase->getContentSize().height));
            purchase->setName("PurchasePack");
            logo->addChild(purchase);
        }
    }
}

void PackPage::createSpecialPackUI(PackInfo* pack, const int index)
{
    float interval = getContentSize().width / 3;
    float offsetX = 0;
    
    std::string logoPath = pack->logoName;
    
    //remove the last one
    removeChildByTag(index);
    
    //create logo sprite
    Sprite *logo = Sprite::create(logoPath);
    CCASSERT(logo, "Create pack logo failed!");
    logo->setTag(index);
    offsetX = ((interval - logo->getContentSize().width)/2) * (2 * index + 1) + logo->getContentSize().width * index;
    logo->setPosition(Vec2(offsetX + logo->getContentSize().width/2, getContentSize().height/2));
    addChild(logo);
    
    //29+ icon
    Sprite *icon = Sprite::create("ui03_icon_crayons.png");
    CCASSERT(icon, "Create pack 6+ icon failed!");
    icon->setPosition(Vec2(logo->getContentSize().width - 50, logo->getContentSize().height - 40));
    logo->addChild(icon);
    
    //unlock logo
    auto unlock = Sprite::create("ui03_lable_lockpack.png");
    CCASSERT(unlock, "Create pack 6+ icon failed!");
    unlock->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    unlock->setPosition(Vec2(logo->getContentSize().width/2, 40.0));
    unlock->setName("PurchasePack");
    logo->addChild(unlock);
}

#pragma mark- Choose Pack

void PackPage::choosePack(const int index, Sprite *logoSprite)
{
    //all packs count except special pack
    int allPacksCount = PackManager::getInstance()->getPackCount();

    //just return, there is no pack avaliable
    if (index > allPacksCount) {
        return;
    }
    
    if (index == allPacksCount)
    {
        chooseSpecialPack(index, logoSprite);
        return;
    }
    
    PackInfo *pack = PackManager::getInstance()->getPackInfo(index);
    if (pack)
    {
        if (pack->isServerPack)
        {
            chooseServerPack(index, logoSprite);
        }
        else
        {
            chooseLocalPack(index, logoSprite);
        }
    }
}

void PackPage::chooseServerPack(const int index, cocos2d::Sprite *logoSprite)
{
    PackInfo *pack = PackManager::getInstance()->getPackInfo(index);

    //is purchase pack
    std::string purchaseIdentifier = PackManager::getInstance()->getPackPurchasedIdentifier(pack);
    
    if (purchaseIdentifier.size() > 0)
    {
        std::string userDefaultKey = UserDefaultKey_HasPurchasedPack + purchaseIdentifier;
        bool hasPurchased = UserDefault::getInstance()->getBoolForKey(userDefaultKey.c_str(), false);
        if (!hasPurchased)
        {
            if (!_ispurchasing)
            {
                //if has not been purchased, purchase it
                _ispurchasing = true;

                _appBilling.purchase(purchaseIdentifier.c_str());
            }
            return;
        }
    }
    
    //check if has been downloaded
    std::string userDefaultKey = UserDefaultKey_HasDownloadServerPack + pack->packName;
    bool hasDownloaded = UserDefault::getInstance()->getBoolForKey(userDefaultKey.c_str(), false);
    if (!hasDownloaded)
    {
        DownloadPackCommand *command = PackManager::getInstance()->getDownloadCommand(pack->packName);
        if (command)
        {
            //do nothing, just return
        }
        else
        {
            ServerPackSprite *logo = dynamic_cast<ServerPackSprite*>(logoSprite);
            DownloadPackCommand *command = new (std::nothrow)(DownloadPackCommand);
            command->init(pack->packName, logo);
            logo->changeToDownloadingStyle();
            
            PackManager::getInstance()->addDownloadPackCommandToQueue(command);
        }
        
        return;
    }
    
    if (!_hasChoosed)
    {
        auto scene = PictureScene::createSceneWithPackIndex(index);
        Director::getInstance()->replaceScene(scene);
        _hasChoosed = true;
    }
}

void PackPage::chooseLocalPack(const int index, cocos2d::Sprite *logoSprite)
{
    PackInfo *pack = PackManager::getInstance()->getPackInfo(index);
    
    //if is server pack, check if is paid pack
    std::string purchaseIdentifier = PackManager::getInstance()->getPackPurchasedIdentifier(pack);
    
    if (purchaseIdentifier.size() > 0)
    {
        std::string userDefaultKey = UserDefaultKey_HasPurchasedPack + purchaseIdentifier;
        bool hasPurchased = UserDefault::getInstance()->getBoolForKey(userDefaultKey.c_str(), false);
        if (!hasPurchased)
        {
            if (!_ispurchasing)
            {
                _ispurchasing = true;
                _appBilling.purchase(purchaseIdentifier.c_str());
            }
            return;
        }
    }
    
    if (!_hasChoosed)
    {
        auto scene = PictureScene::createSceneWithPackIndex(index);
        Director::getInstance()->replaceScene(scene);
        _hasChoosed = true;
    }
}

void PackPage::chooseSpecialPack(const int index, cocos2d::Sprite *logoSprite)
{
    /* no need to check other information, beacause if special pack is available,
       it's definitely has not been purchased, just buy it!
     */

    if (!_ispurchasing)
    {
        PackInfo *sepecialPack = PackManager::getInstance()->getSpecialPackInfo();
        
        std::string purchaseIdentifier = PackManager::getInstance()->getPackPurchasedIdentifier(sepecialPack);
        
        _ispurchasing = true;
        _appBilling.purchase(purchaseIdentifier.c_str());
    }
}

cocos2d::ProgressTimer* PackPage::createProgressTimer(Sprite *containerSprite, const std::string& packName)
{
    //init progress background
    Sprite *progressbg = Sprite::create("ui03_download_bar.png");
    progressbg->setPosition(containerSprite->getChildByName("DownLoadButton")->getPosition());
    containerSprite->removeChildByName("DownLoadButton");
    containerSprite->addChild(progressbg);
    
    //init progress
    Sprite *progress = Sprite::create("ui03_download_progress.png");
    auto progressTimer = ProgressTimer::create(progress);
    progressTimer->setPosition(progressbg->getPosition());
    progressTimer->setType(ProgressTimer::Type::BAR);
    progressTimer->setMidpoint(Vec2(0, 1));
    progressTimer->setBarChangeRate(Vec2(1, 0));
    progressTimer->setPercentage(0);
    progressTimer->setName("ProgressTimer");
    progressTimer->setPercentage(2);
    containerSprite->addChild(progressTimer);
    
    return progressTimer;
}

#pragma mark- STIABDelegate

void PackPage::purchaseSuccessful(const char* pid)
{
    _ispurchasing = false;
    if (std::string(pid) == IAP_UNLOCK_SPECIAL_PACK)
    {
        /* if user unlock special pack, unlock the patter pen and unlock a random 
           unlock pack */
        PackManager::getInstance()->unlockSpecialPack();
    }
    else
    {
        std::string userDefault = UserDefaultKey_HasPurchasedPack + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
    }
    
    Scene *scene = Director::getInstance()->getRunningScene();
    PackScene *layer = dynamic_cast<PackScene*>(scene->getChildren().at(0));
    layer->reloadPacks();
}

void PackPage::purchaseFailed(const char *pid, int errorCode)
{
    _ispurchasing = false;

}

void PackPage::restoreSuccessful(const char* pid)
{
    _ispurchasing = false;

}

void PackPage::restoreFailed(const char* pid, int errorCode)
{
    _ispurchasing = false;

}








CB_END_NAMESPACE