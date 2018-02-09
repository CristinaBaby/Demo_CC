
#include "AnimationHelp.h"

Vector<SpriteFrame*> AnimationHelp::getAnimFrames(const std::string& preString,int startNum,int endNum,bool isPng)
{
    Vector<SpriteFrame*> animFrames;
    char frameName[50] = {0};
    for (int i = startNum; i<=endNum; i++) {
        sprintf(frameName, "%s%d.%s",preString.c_str(),i,isPng?"png":"jpg");
        Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        Rect rect = Rect::ZERO;
        rect.size = texture->getContentSize();
        SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
        animFrames.pushBack(frame);
        
    }
    
    for (int i = startNum; i>=endNum; i--) {
        sprintf(frameName, "%s%d.%s",preString.c_str(),i,isPng?"png":"jpg");
        Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        Rect rect = Rect::ZERO;
        rect.size = texture->getContentSize();
        SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
        animFrames.pushBack(frame);
    }
    return animFrames;
}

void AnimationHelp::removeFrames(const std::string& preString, int startNum, int endNum,bool isPng)
{
    char str[50] = {0};
    for (int i = startNum; i<=endNum; i++) {
        sprintf(str, "%s%d.%s",preString.c_str(),i,isPng?"png":"jpg");
        TextureCache::getInstance()->removeTextureForKey(std::string(str));
    }
    
    for (int i = startNum; i>=endNum; i--) {
        sprintf(str, "%s%d.%s",preString.c_str(),i,isPng?"png":"jpg");
        TextureCache::getInstance()->removeTextureForKey(std::string(str));
    }
    
}

Vector<SpriteFrame*> AnimationHelp::getAnimFramesCache(const std::string& preString,int startNum, int endNum,bool isPng)
{
    Vector<SpriteFrame*> animFrames;
    char frameName[50] = {0};
    for (int i = startNum; i<=endNum; i++) {
        sprintf(frameName, "%s%d.%s",preString.c_str(),i,isPng?"png":"jpg");
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        animFrames.pushBack(frame);
    }
    
    for (int i = startNum; i>=endNum; i--) {
        sprintf(frameName, "%s%d.%s",preString.c_str(),i,isPng?"png":"jpg");
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        animFrames.pushBack(frame);
    }
    return animFrames;
    
}

Animate* AnimationHelp::createAnimateCache(const std::string& frameName, int startNum, int endNum,bool returnOriginFrame,bool isPng, float delay, unsigned int loops)
{
    
    Animation* frameAnimation = Animation::createWithSpriteFrames(getAnimFramesCache(frameName, startNum, endNum,isPng), delay, loops);
    frameAnimation->setRestoreOriginalFrame(returnOriginFrame);
    return Animate::create(frameAnimation);
}

Animate* AnimationHelp::createAnimate(const std::string& frameName, int startNum, int endNum,bool returnOriginFrame,bool isPng, float delay, unsigned int loops)
{
    Animation* frameAnimation = Animation::createWithSpriteFrames(getAnimFrames(frameName, startNum, endNum,isPng), delay, loops);
    frameAnimation->setRestoreOriginalFrame(returnOriginFrame);
    return Animate::create(frameAnimation);    
}