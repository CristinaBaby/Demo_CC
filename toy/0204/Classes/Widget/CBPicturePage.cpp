//
//  CBPicturePage.cpp
//  ColorBook
//
//  Created by maxiang on 4/23/15.
//
//

#include "CBPicturePage.h"
#include "../Model/CBPackManager.h"
#include "../Scene/CBGameScene.h"
#include "../Layer/CBContinueLayer.h"
#include "../Model/CBUtils.h"
#include "../Model/STVisibleRect.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

bool PicturePage::init()
{
    if (! TableViewCell::init())
    {
        return false;
    }
    
    bool rcode = false;
    do {
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void PicturePage::reset()
{
    removeAllChildren();
}

void PicturePage::touchEnd(cocos2d::Touch *touch)
{
    Point point = convertToNodeSpace(touch->getLocation());
    
    for (int i = 1; i <= 6; ++i)
    {
        Sprite *picture = dynamic_cast<Sprite*>(getChildByTag(_picturePageIndex*6+i));
        if (picture && picture->getBoundingBox().containsPoint(point))
        {
            //touch pack 1
            log("INFO: Touch picture %d", i);
            int index = i + (_picturePageIndex * 6);
            std::string historyPath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex+1, index, true);
            bool isExsit = FileUtils::getInstance()->isFileExist(historyPath);
            if (isExsit)
            {
                ContinueLayer *layer = ContinueLayer::create(_packIndex, index);
                Director::getInstance()->getRunningScene()->addChild(layer);
            }
            else
            {
                if (!_hasChoosed)
                {
                    auto scene = GameScene::createScene(_packIndex, index);
                    Director::getInstance()->replaceScene(scene);
                    _hasChoosed = true;
                }
            }

            return;
        }
    }
}

void PicturePage::setPackIndex(const int packIndex)
{
    _packIndex = packIndex;
}

void PicturePage::reload(const int picturePageIndex)
{
    PackInfo *pack = PackManager::getInstance()->getPackInfo(_packIndex);
    if (!pack)
        return;
    
    _picturePageIndex = picturePageIndex;
    
    float intervalX = getContentSize().width / 3;
    float intervalY = getContentSize().height / 2;

    float offsetX = 0;
    float offsetY = 0;

    int index = 1 + (picturePageIndex * 6);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if( i == 1 && j == 2) continue;
            
            PackInfo *pack = PackManager::getInstance()->getPackInfo(_packIndex);
            std::string pictureName;
            std::string historyLogo;

            if (pack->isServerPack)
            {
                pictureName = PackManager::getInstance()->getPackPicturePath(_packIndex+1, index, true);
            }
            else
            {
                pictureName = PackManager::getInstance()->getPackPicturePath(_packIndex+1, index, false);
            }
            
            Sprite *logo= nullptr;
            
            /* --------IMPORTANT-------
               history logo stored in device writeable path, so local pack same as server pack
             */
            historyLogo = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex+1, index, true);
            
            bool isExsit = FileUtils::getInstance()->isFileExist(historyLogo);
            if (isExsit)
            {
                
                /* --------IMPORTANT-------
                   remove the last history logo in texturecache
                 */
                Director::getInstance()->getTextureCache()->removeTextureForKey(historyLogo);
                logo = Sprite::create(historyLogo);
            }
            else
            {
                logo = Sprite::create(pictureName);
                /* --------IMPORTANT-------
                   original picture is 1136 * 768, so we must set the texture rect to adapt
                   device screen
                 */
                logo->setTextureRect(Rect(namespaceST::STVisibleRect::getDesignOffset().x, namespaceST::STVisibleRect::getDesignOffset().y, Utils::SCREEN_WIDTH - (310 - (1136 - Utils::SCREEN_WIDTH)/2.0), Utils::SCREEN_HEIGHT));
            }
            
            if (!logo) {
                log("WARING: create pack picture logo sprite failed");
                continue;
            }
 
            offsetX = ((intervalX - logo->getContentSize().width)/2) * (2 * j + 1) + logo->getContentSize().width * j;
            offsetY = ((intervalY - logo->getContentSize().height)/2) * (2 * i + 1) + logo->getContentSize().height * i;

            //white background
            auto bg = Sprite::create("Pack1_1.png");
            
            float scaleX = bg->getContentSize().width*0.8 / logo->getContentSize().width;
            float scaleY = bg->getContentSize().height*0.8 / logo->getContentSize().height;
            float scale = scaleX > scaleY ? scaleY : scaleX;
            logo->setScale(scale - 0.02/* color mask */);
            logo->setTag(index);

            logo->setPosition(Vec2(getContentSize().width / 3 + (logo->getContentSize().width/2)*(i+j), getContentSize().height / 2));
            bg->setPosition(logo->getPosition());
            bg->setScale(0.8);
            
            addChild(bg);
            addChild(logo);

            index++;
        }
    }
}


CB_END_NAMESPACE