//
//  ShapeButton.cpp
//  DUM2204_CuteSnowman
//
//  Created by huanghanzhi on 14-11-13.
//
//

#include "ShapeButton.h"

ShapeButton::ShapeButton()
:m_isTouched(false),m_image(nullptr),
m_fnClickCall(nullptr)
{
    
}

ShapeButton::~ShapeButton()
{
    CC_SAFE_DELETE(m_image);
}

ShapeButton* ShapeButton::create(const string &fileName)
{
    ShapeButton* pItem = new ShapeButton;
    pItem->initWithFile(fileName);
    pItem->initButton(fileName);
    pItem->autorelease();
    return pItem;
}

void ShapeButton::initButton(const string& fileName)
{
    // 添加点击事件
    EventListenerTouchOneByOne* m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(true);
    m_listener->onTouchBegan = CC_CALLBACK_2(ShapeButton::onTouchBegan, this);
    m_listener->onTouchMoved = CC_CALLBACK_2(ShapeButton::onTouchMoved, this);
    m_listener->onTouchEnded = CC_CALLBACK_2(ShapeButton::onTouchEnded, this);
    m_listener->onTouchCancelled = CC_CALLBACK_2(ShapeButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
    
    
    m_image = new Image();
    m_image->initWithImageFile(fileName);
    
//    RenderTexture*  tempRender = RenderTexture::create(this->getContentSize().width,
//                                                       this->getContentSize().height,
//                                                       Texture2D::PixelFormat::RGBA8888);
//    // 开始绘制
//    tempRender->begin();
//    Vec2 ptOrigin = getPosition();
//    setPosition(getContentSize().width * 0.5, getContentSize().height * 0.5);
//    this->visit();
//    setPosition(ptOrigin);
//    // 结束绘制
//    tempRender->end();
//    // 需要调用这一句，否则Image取出的Data为空，或者在下一帧处理
//    Director::getInstance()->getRenderer()->render();
//    // 通过画布获取图片的像素信息，存放在Image中
//    if (m_image)
//    {
//        CC_SAFE_RELEASE_NULL(m_image);
//    }
//    m_image = tempRender->newImage();
}

bool ShapeButton::onTouchBegan(Touch *touch, Event *unused_event)
{
    m_isTouched = false;
    Vec2 touchPoint = touch->getLocation();
    Vec2 realPos = this->getParent()->convertToNodeSpace(touchPoint);
    if (this->boundingBox().containsPoint(realPos))
    {
        m_color4B.r = 0;
        m_color4B.g = 0;
        m_color4B.b = 0;
        m_color4B.a = 0;
        
        Vec2 nodePos = this->convertTouchToNodeSpace(touch);
        unsigned int x = nodePos.x;
        unsigned int y = this->getContentSize().height - nodePos.y;

        // 获取像素数据
        unsigned char* data = m_image->getData();
        unsigned int* pixel = (unsigned int*)data;
        pixel = pixel + (y*(int)getContentSize().width)*1 + x * 1;
        
        m_color4B.r = (*pixel) & 0xff;
        m_color4B.g = (*pixel >> 8) & 0xff;
        m_color4B.b = (*pixel >> 16) & 0xff;
        m_color4B.a = (*pixel >> 24) & 0xff;
        
        if (m_color4B.a > 50)
        {
            m_isTouched = true;
//            this->setColor(Color3B::GRAY);
        }
        else
        {
            m_isTouched = false;
        }
    }
    
    return m_isTouched;
}

void ShapeButton::onTouchMoved(Touch *touch, Event *unused_event)
{
//    Vec2 touchPoint = touch->getLocation();
//    if (this->getBoundingBox().containsPoint(touchPoint))
//    {
//        Vec2 nodePos = this->convertTouchToNodeSpace(touch);
//        unsigned int x = nodePos.x;
//        unsigned int y = this->getContentSize().height - nodePos.y;
//        unsigned char* data = m_image->getData();
//        unsigned int* pixel = (unsigned int*)data;
//        pixel = pixel + (y*(int)getContentSize().width)*1 + x * 1;
//        m_color4B.r = (*pixel) & 0xff;
//        m_color4B.g = (*pixel >> 8) & 0xff;
//        m_color4B.b = (*pixel >> 16) & 0xff;
//        m_color4B.a = (*pixel >> 24) & 0xff;
//        
//        if (m_color4B.a < 50)
//        {
//            this->setColor(Color3B::WHITE);
//            m_isTouched = false;
//        }
//        else if (m_color4B.a > 50)
//        {
//            this->setColor(Color3B::GRAY);
//            m_isTouched = true;
//        }
//    }
//    else
//    {
//        this->setColor(Color3B::WHITE);
//    }
}

void ShapeButton::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 touchPoint = touch->getLocation();
    Vec2 realPos = this->getParent()->convertToNodeSpace(touchPoint);
    if (this->getBoundingBox().containsPoint(realPos))
    {
//        Vec2 nodePos = this->convertTouchToNodeSpace(touch);
//        unsigned int x = nodePos.x;
//        unsigned int y = this->getContentSize().height - nodePos.y;
//        unsigned char* data = m_image->getData();
//        unsigned int* pixel = (unsigned int*)data;
//        pixel = pixel + (y*(int)getContentSize().width)*1 + x * 1;
//        m_color4B.r = (*pixel) & 0xff;
//        m_color4B.g = (*pixel >> 8) & 0xff;
//        m_color4B.b = (*pixel >> 16) & 0xff;
//        m_color4B.a = (*pixel >> 24) & 0xff;
//        
//        if (m_color4B.a < 50)
//        {
//            this->setColor(Color3B::WHITE);
//        }
//        else if (m_color4B.a > 50)
//        {
//            this->setColor(Color3B::GRAY);
//        }
        if (m_fnClickCall)
        {
            m_fnClickCall(this);
        }
    }
    this->setColor(Color3B::WHITE);
}