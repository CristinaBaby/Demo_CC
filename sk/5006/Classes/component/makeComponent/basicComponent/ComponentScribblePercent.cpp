//
//  ComponentScribblePercent.cpp
//  BHUG1062
//
//  Created by tanshoumei on 4/19/16.
//
//

#include "ComponentScribblePercent.h"
#include "3rdparty_depends/scribble/ScribbleUtil.h"

static long getNonAlphaPixelCount(Image* pImage)
{
    long count = 0;
    for (long i = 0; i < pImage->getHeight(); ++i) {
        for (long j = 0; j < pImage->getWidth(); ++j) {
            unsigned char *lData = pImage->getData();
            long lPixcelIndex = i * pImage->getWidth() + j;
            unsigned char lRed = lData[lPixcelIndex * 4];
            unsigned char lGreen = lData[lPixcelIndex * 4 + 1];
            unsigned char lBlue = lData[lPixcelIndex * 4 + 2];
            unsigned char lAlpha = lData[lPixcelIndex * 4 + 3];
            if (lAlpha > 10) {
                if (lRed > 0 || lGreen > 0 || lBlue > 0) {
                    ++count;
                }
            }
        }
    }
    return count;
}

static long getNonAlphaPixelCount(RenderTexture* rndTex)
{
    auto pImage = rndTex->newImage();
    long count = getNonAlphaPixelCount(pImage);
    delete pImage;
    return count;
}

ComponentScribblePercent::ComponentScribblePercent():
_checkDuration(0.2),
_isScribbleBrushTypeEraser(true)
{
    
}

ComponentScribblePercent::~ComponentScribblePercent()
{
    stop();
}

void ComponentScribblePercent::start()
{
    auto owner = getOwner();
    if(nullptr == owner)
        return;
    
    if(_scribbleTargetPath.length() == 0)
    {
        __CCLOGWITHFUNCTION("waring: must set scirrble target path");
        return;
    }
    
    _scribbleNode = dynamic_cast<ScribbleNode*>(owner);
    if(nullptr == _scribbleNode)
        return;
    auto targetSprite = Sprite::create(_scribbleTargetPath);
    RenderTexture *lRT = RenderTexture::create(targetSprite->getContentSize().width, targetSprite->getContentSize().height);
    targetSprite->setPosition(targetSprite->getContentSize() / 2);
    lRT->begin();
    targetSprite->visit();
    lRT->end();
    Director::getInstance()->getRenderer()->render();
    auto targetNonAlphaCount = getNonAlphaPixelCount(lRT);
    
    _originNonAlphaCount = _isScribbleBrushTypeEraser ? targetNonAlphaCount : 0;
    _targetNonAlphaCount = _isScribbleBrushTypeEraser ? 0 : targetNonAlphaCount;
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ComponentScribblePercent::checkSchedule), this, _checkDuration, false);
}

void ComponentScribblePercent::pause()
{
    Director::getInstance()->getScheduler()->pauseTarget(this);
}

void ComponentScribblePercent::resume()
{
    Director::getInstance()->getScheduler()->resumeTarget(this);
}

void ComponentScribblePercent::stop()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentScribblePercent::checkSchedule), this);
}

void ComponentScribblePercent::checkSchedule(float dt)
{
    if(!_scribbleNode)
        return;
    auto nonAlphaCountNow = getNonAlphaPixelCount(_scribbleNode->getCanvas());
    if(_isScribbleBrushTypeEraser)
        _percentComplete = (_originNonAlphaCount - nonAlphaCountNow) * 1.0 / _originNonAlphaCount;
    else
        _percentComplete = nonAlphaCountNow * 1.0 / _targetNonAlphaCount;
    if(scribbleCompletePercentCall)
    {
        scribbleCompletePercentCall(_percentComplete);
    }
}