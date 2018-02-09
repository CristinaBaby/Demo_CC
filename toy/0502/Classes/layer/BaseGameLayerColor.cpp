//
//  BaseGameLayerColor.cpp
//  MyGame_Project
//
//  Created by liyang1 on 12/1/14.
//
//

#include "BaseGameLayerColor.h"

USING_NS_CC;

RenderTexture* BaseGameLayerColor::getGameTargetSprite(int w_size, int h_size)
{
    
    CCASSERT(baseTargetPath.compare("")!=0, "涂抹花纹的路径为空");
    Vector<Sprite*> spriteArray ;
    for (int i=0; i<h_size; i++) {
        for (int j =0; j<w_size; j++) {
            Sprite *target_sprite = Sprite::create(baseTargetPath.c_str());
            target_sprite->setAnchorPoint(Vec2(0, 0));
            target_sprite->setPosition(Vec2(j*baseTargetSize, i*baseTargetSize));
            target_sprite->retain();
            spriteArray.pushBack(target_sprite);
        }
    }
    
    RenderTexture *targetCanvas = RenderTexture::create(baseTargetSize*w_size   , baseTargetSize*h_size);
    targetCanvas->clear(1, 1, 1, 1);
    targetCanvas->retain();
    targetCanvas->begin();
    
    Vector<Sprite*>::iterator it;
    for(it=spriteArray.begin();it!=spriteArray.end();it++){
        (*it)->visit();
    }
    
    targetCanvas->end();
    //用来测显示否正确
//    Sprite *targetSprite = Sprite::createWithTexture(targetCanvas->getSprite()->getTexture());
//    targetSprite->setAnchorPoint(Vec2(0, 0));
//    targetSprite->setPosition(Vec2(0, 0));
//    addChild(targetSprite , 99);
    

    return targetCanvas;
}


cocos2d::RenderTexture* BaseGameLayerColor::convertFromSpriteToImage(Sprite *checkSprite){
    
    float x = checkSprite->getPositionX();
    float y = checkSprite->getPositionY();
    checkSprite->setPosition(Vec2(0, 0));
    checkSprite->setVisible(true);
    
    RenderTexture  *m_pRenderTexture =  RenderTexture::create(checkSprite->getContentSize().width, checkSprite->getContentSize().height);
    m_pRenderTexture->setPosition(Vec2(checkSprite->getContentSize().width/2 , checkSprite->getContentSize().height/2));
    m_pRenderTexture->setAnchorPoint(Vec2(0, 0));
    addChild(m_pRenderTexture);
    m_pRenderTexture->begin();
    //绘制
    checkSprite->visit();
    //结束绘制
    m_pRenderTexture->end();
    m_pRenderTexture->setVisible(false);
    
    
    
    checkSprite->setPosition(x, y);
    checkSprite->setVisible(false);
    return m_pRenderTexture;
}

int mystrlen(unsigned char* str)
{
    int i = -1;
    while(i++,'\0' != str[i]);
    return i;
}

 bool BaseGameLayerColor::checkIsInBounding(Image *pImage ,  int imageWidth , int imageHeight ,Vec2 nodePos)
{
    unsigned int x =nodePos.x;
    unsigned  int y= imageHeight - nodePos.y ;
    
    if ( y > imageHeight ) {
        CCLOG("超过图片区域");
        return false;
    }
    
//    CCLOG("x=%d  y=%d" , x, y);
    Color4B color4B = {0,0,0,0};
    unsigned char* data_ = pImage->getData();
    
    unsigned int *pixel = (unsigned int *)data_;
    
    pixel = pixel + (y * (int)imageWidth) * 1 + x * 1;
    
    //R通道
    color4B.r = *pixel & 0xff;
    //G通道
    color4B.g = (*pixel >> 8) & 0xff;
    //B通过
    color4B.b = (*pixel >> 16) & 0xff;
    //Alpha通道，我们有用的就是Alpha
    color4B.a = (*pixel >> 24) & 0xff;
    
//    CCLOG("r%d  g%d  b %d a%d" , color4B.r,color4B.g,color4B.b,color4B.a);
    //recycle image
    pImage->release();
    if(color4B.a!=0){
        return true;
    }
    return false;
}

Sprite* BaseGameLayerColor::addUserDrawBg(const std::string& filePath){
    bgGameSprite = Sprite::create(filePath);
    bgGameSprite->setAnchorPoint(Vec2(0, 0));
    bgGameSprite->setPosition(Vec2(0, 0));
    addChild(bgGameSprite);
//    bgGameSprite->getTexture()->setAliasTexParameters();
    
//    Texture2D *lTexture = bgGameSprite->getTexture();
    
//    lTexture->generateMipmap();
//    Texture2D::TexParams texParams = {GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
//    lTexture->setTexParameters(&texParams);

    bgGameSprite->getTexture()->setAntiAliasTexParameters();
    return bgGameSprite;
}


