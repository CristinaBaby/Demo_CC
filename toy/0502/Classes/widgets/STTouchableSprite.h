//
//  STTouchableSprite.h
//  SpotDifference
//
//  Created by Steven.Tian on 14-10-21.
//
//

#ifndef __SpotDifference__STTouchableSprite__
#define __SpotDifference__STTouchableSprite__

#include "cocos2d.h"


class STTouchableSprite : public cocos2d::Sprite
{
public:
    typedef std::function<void(STTouchableSprite * sprite)> ccTouchBeganCallback;
    
    /**
     * @see   Sprite::create(const std::string& filename)
     */
    static STTouchableSprite* create(const std::string& filename);
    
    /**
     * @see   Sprite::createWithTexture(Texture2D *texture)
     */
    static STTouchableSprite* createWithTexture(cocos2d::Texture2D *texture);
    
    /**
     * @see   Sprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
     */
    static STTouchableSprite* createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
    
    /**
     * @see   Sprite::createWithSpriteFrameName(const std::string& spriteFrameName)
     */
    static STTouchableSprite* createWithSpriteFrameName(const std::string& spriteFrameName);
    
    
    void setTouchBegan(ccTouchBeganCallback touchBeganCallback);
    
    
protected:
    STTouchableSprite();
    
    /**
     * @see   Sprite::initWithTexture(Texture2D *texture)
     */
    virtual bool initWithTexture(cocos2d::Texture2D *texture);
    
    /**
     * @see   Sprite::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
     */
    virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated);
    
private:
    ccTouchBeganCallback _touchBeganCallback;
};

#endif /* defined(__SpotDifference__STTouchableSprite__) */
