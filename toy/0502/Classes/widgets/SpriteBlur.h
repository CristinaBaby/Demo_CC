//
//  SpriteBlur.h
//  cocosTest
//
//  Created by liyang1 on 4/24/14.
//
//

#ifndef __cocosTest__SpriteBlur__
#define __cocosTest__SpriteBlur__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class SpriteBlur : public Sprite
{
public:
    ~SpriteBlur();
    void setBlurSize(float f);
    bool initWithTexture(CCTexture2D* texture, const Rect&  rect);
    void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    void initProgram();
    void listenBackToForeground(Ref *obj);
    
    static SpriteBlur* create(const char *pszFileName);
    
    Vec2 blur_;
    GLfloat    sub_[4];
    
    GLuint    blurLocation;
    GLuint    subLocation;
};


#endif /* defined(__cocosTest__SpriteBlur__) */
