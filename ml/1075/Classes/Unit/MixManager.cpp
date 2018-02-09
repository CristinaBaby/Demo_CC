
#include "MixManager.h"
#include "cocos2d.h"
#include "AudioHelp.h"

USING_NS_CC;
using namespace cocos2d::ui;


void MixManager::mix()
{
    if (!_mixtureNode) {
        return;
    }
    _count++;
    if (_mixRotateSpeed == 0) {
        if(_count%10==0){
            _mixtureNode->setScaleX(-_mixtureNode->getScaleX());
        }
    }else{
        _mixtureNode->setRotation(_mixtureNode->getRotation()+_mixRotateSpeed);
    }
    if (_count%(_mixSpeed*_mixCount/_scheduleSegment)==0) {
        
        if (onScheduleCallback) {
            onScheduleCallback(_count/(_mixSpeed*_scheduleSegment/_mixCount));
        }
    }
    if (_count%_mixSpeed==0) {
        int mixtureStep = _count/_mixSpeed;
        if(onMixCallback){
            onMixCallback(mixtureStep);
        }
        if (mixtureStep<=_mixCount){
            Sprite* pMixture = Sprite::create(_mixturePathEX+std::to_string(_mixtureStartIndex+mixtureStep)+".png");
            _mixtureNode->removeAllChildren();
            _mixtureNode->addChild(pMixture);
            pMixture->setPosition(_mixturePos);
        }
        if (mixtureStep>=_mixCount){
            //mix finish
            _mixtureNode->stopAllActions();
            _mixtureNode->setScaleX(1);
            if(onMixFinish){
                onMixFinish();
            }
        }
    }
}

void MixManager::startStir()
{
    
}

void MixManager::endStir()
{
    
}

void MixManager::saveMixture()
{
    RenderTexture* render = RenderTexture::create(_mixtureSize.width, _mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    _mixtureNode->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"mixture.png", false);
    pImage->autorelease();
    
}