//
//  CBServerPackSprite.cpp
//  ColorBook
//
//  Created by maxiang on 5/6/15.
//
//

#include "CBServerPackSprite.h"

USING_NS_CC;

ServerPackSprite* ServerPackSprite::create(const std::string& filename)
{
    ServerPackSprite *sprite = new (std::nothrow) ServerPackSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void ServerPackSprite::setupUI()
{
    //download button
    _downloadButton = Sprite::create("ui03_btn_download.png");
    _downloadButton->setPosition(Vec2(getContentSize().width/2,
                               _downloadButton->getContentSize().height + 10));
    _downloadButton->setName("DownLoadButton");
    addChild(_downloadButton);
    
    //pack avaliable logo
    _availableLogo = Sprite::create("ui03_label_new.png");
    _availableLogo->setPosition(Vec2(getContentSize().width/2,
                                getContentSize().height - _availableLogo->getContentSize().height*3/2+15));//hard code, not good, but....
    _availableLogo->setName("PackAvaliableLogo");
    addChild(_availableLogo);
    
    //6+ icon, stupid!!!!!
    _topRightLogo = Sprite::create("ui03_icon.png");
    _topRightLogo->setPosition(Vec2(getContentSize().width - 50, getContentSize().height - 40));
    addChild(_topRightLogo);
    
    //init progress background
    _downloadProgressBg = Sprite::create("ui03_download_bar.png");
    _downloadProgressBg->setPosition(_downloadButton->getPosition());
    addChild(_downloadProgressBg);
    
    //init progress
    Sprite *progress = Sprite::create("ui03_download_progress.png");
    _progressTimer = ProgressTimer::create(progress);
    _progressTimer->setPosition(_downloadProgressBg->getPosition());
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setMidpoint(Vec2(0, 1));
    _progressTimer->setBarChangeRate(Vec2(1, 0));
    _progressTimer->setPercentage(0);
    _progressTimer->setName("ProgressTimer");
    _progressTimer->setPercentage(2);
    addChild(_progressTimer);
}

void ServerPackSprite::changeToNotDownloadStyle()
{
    _availableLogo->setVisible(true);
    _downloadButton->setVisible(true);
    _progressTimer->setVisible(false);
    _downloadProgressBg->setVisible(false);
    _topRightLogo->setVisible(false);
}

void ServerPackSprite::changeToDownloadingStyle()
{
    _availableLogo->setVisible(true);
    _downloadButton->setVisible(false);
    _progressTimer->setVisible(true);
    _downloadProgressBg->setVisible(true);
    _topRightLogo->setVisible(false);
}

void ServerPackSprite::changeToHasDownloadStyle()
{
    _availableLogo->setVisible(false);
    _downloadButton->setVisible(false);
    _progressTimer->setVisible(false);
    _downloadProgressBg->setVisible(false);
    _topRightLogo->setVisible(true);
}