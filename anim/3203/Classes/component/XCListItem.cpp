//
// XCListItem.cpp
// SHAKE4040
//
// Created by dengcheng on 15/8/24.
//
//

#include "XCListItem.h"

XCListItem::XCListItem()
:m_nThresholdsAlpha(40)
,m_pItem(nullptr)
,m_pImage(nullptr)
,m_pTouchShow(nullptr)
,m_vTouchPos(Vec2::ZERO)
{
    static int sId = 0;
    ++sId;
    m_nID = sId;
}

void XCListItem::onEnter()
{
    Node::onEnter();
    
}

XCListItem* XCListItem::create(const std::string &aFileName)
{
    auto pRet = new XCListItem();
    if(pRet && pRet->init(aFileName))
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool XCListItem::init(const std::string &aFileName)
{
    if ( !Node::init() )
    {
        return false;
    }
    
    m_sResourcePath = aFileName;
    
    m_pItem = Sprite::create(aFileName);
    addChild(m_pItem);
    
    m_pImage = new Image();
    m_pImage->initWithImageFile(aFileName);
    
    return true;
}

void XCListItem::setShowImage(const std::string &aFilePath)
{
    m_pTouchShow = Sprite::create(aFilePath);
    addChild(m_pTouchShow);
    m_pTouchShow->setVisible(false);
}

void XCListItem::showDefualtStatus()
{
    m_pItem->setVisible(true);
    if (m_pTouchShow) {
        m_pTouchShow->setVisible(false);
    }
}

void XCListItem::showTouchStatus()
{
    m_pItem->setVisible(false);
    if (m_pTouchShow) {
        m_pTouchShow->setVisible(true);
    }
}

void XCListItem::setThesholdsAlphaValue(const int &aAlpha)
{
    do {
        
        if (aAlpha < 0) {
            m_nThresholdsAlpha = 0;
            break;
        }
        
        if (aAlpha > 255) {
            m_nThresholdsAlpha = 255;
            break;
        }
        
        m_nThresholdsAlpha = aAlpha;
        
    } while (false);
}

bool XCListItem::getTouchItem(const Vec2 &aPos)
{
    
    Vec2 localPos = convertToNodeSpace(aPos);
    
    if (m_pItem->isVisible() && m_pItem->getBoundingBox().containsPoint(localPos) && touchAtImageIsAlpha(m_pItem->convertToNodeSpace(aPos))) {
        m_vTouchPos = localPos;
        return true;
    }
    
    return false;
}

bool XCListItem::touchAtImageIsAlpha(const Vec2 &aPos)
{
    
    Image *pImage = m_pImage;
    
    unsigned char *data = pImage->getData();
    
    //only support rgba888
    const int bytesPerPixel = 4;
    
    int row = pImage->getHeight() - aPos.y;
    int col = aPos.x;
    
    if (row < 0) {
        row = 0;
    } else if(row >= pImage->getHeight()) {
        row = pImage->getHeight() - 1;
    }
    
    if (col < 0) {
        col = 0;
    } else if(col >= pImage->getWidth()) {
        col = pImage->getWidth() - 1;
    }
    
    int index = row * pImage->getWidth() + col;
    index *= bytesPerPixel;
    
    if ((3 == bytesPerPixel) ? 255 : data[index + 3] > m_nThresholdsAlpha) {
        return true;
    }
    
    return false;
}

void XCListItem::onExit()
{

    Node::onExit();
}
XCListItem::~XCListItem()
{
    delete m_pImage;
}