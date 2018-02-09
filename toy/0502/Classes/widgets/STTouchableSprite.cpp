//
//  STTouchableSprite.cpp
//  SpotDifference
//
//  Created by Steven.Tian on 14-10-21.
//
//

#include "STTouchableSprite.h"

USING_NS_CC;

STTouchableSprite* STTouchableSprite::createWithTexture(Texture2D *texture)
{
    STTouchableSprite *sprite = new (std::nothrow) STTouchableSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

STTouchableSprite* STTouchableSprite::create(const std::string& filename)
{
    STTouchableSprite *sprite = new (std::nothrow) STTouchableSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


STTouchableSprite* STTouchableSprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
    STTouchableSprite *sprite = new (std::nothrow) STTouchableSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

STTouchableSprite* STTouchableSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    
    return createWithSpriteFrame(frame);
}

STTouchableSprite::STTouchableSprite()
{
}

bool STTouchableSprite::initWithTexture(Texture2D *texture)
{
    return Sprite::initWithTexture(texture);
}

bool STTouchableSprite::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
    if (! Sprite::initWithTexture(texture, rect, rotated)) {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event){
        // 获取事件所绑定的 target
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        // 获取当前点击点所在相对按钮的位置坐标
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        // 点击范围判断检测
        if (rect.containsPoint(locationInNode))
        {
            if (_touchBeganCallback) {
                _touchBeganCallback(this);
            }
            
            return true;
        }
        
        return false;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void STTouchableSprite::setTouchBegan(ccTouchBeganCallback touchBeganCallback)
{
    _touchBeganCallback = touchBeganCallback;
}

