//
//  KFavoriteAdapter.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-12.
//
//

#include "KFavoriteAdapter.h"
#include "Utils.h"
#include "Global.h"

KFavoriteAdapter::KFavoriteAdapter(const Vector<Sprite*>& data):KSSimpleAdapter(data, data)
{
    m_oItemSize = Size::ZERO;
}

KFavoriteAdapter::~KFavoriteAdapter()
{
}

bool KFavoriteAdapter::init()
{
    return true;
}


KFavoriteAdapter* KFavoriteAdapter::create(const Vector<Sprite*>& data)
{
    KFavoriteAdapter* adapter = new KFavoriteAdapter(data);
    
    if (adapter&&adapter->init())
    {
        adapter->autorelease();
        return adapter;
    }
    
    CC_SAFE_DELETE(adapter);
    return nullptr;

}

MenuItem* KFavoriteAdapter::getView(int position, MenuItem* cacheView, Node* parent)
{
    Sprite* item = m_Data.at(position);
    if (g_isDebugMode) {
        log("item->getContentSize().width=======%f, height========%f", item->getContentSize().width, item->getContentSize().height);
    }

    // 添加相册背景
    auto pFrameBg = Sprite::create("images/favorite/fav_frame.png");
    
    float scaleX = (pFrameBg->getContentSize().width - 30) / item->getContentSize().width;
    float scaleY = (pFrameBg->getContentSize().height - 30) / item->getContentSize().height;
    float minScale = MIN(scaleX, scaleY);

    item->setScale(minScale);
    item->setPosition(Vec2(pFrameBg->getContentSize().width / 2, pFrameBg->getContentSize().height / 2));
    pFrameBg->addChild(item);
    m_oItemSize = pFrameBg->getContentSize();

    Sprite* pFrameBg_mask = Sprite::create("images/favorite/fav_frame_mask.png");
    pFrameBg_mask->setPosition(pFrameBg->getContentSize() / 2);
    pFrameBg->addChild(pFrameBg_mask, 10);
    
    MenuItemSprite* itemBtn = nullptr;
    if (item)
    {
        itemBtn = MenuItemSprite::create(pFrameBg, nullptr);
        itemBtn->setUserObject(__String::createWithFormat("%d", position));
    }
    return itemBtn;
}


float KFavoriteAdapter::getItemWidth()
{
    if (g_isDebugMode) {
        log("getItemWidth()======%f", m_oItemSize.width == 0 ? 660 : m_oItemSize.width);
    }
    return m_oItemSize.width == 0 ? 660 : m_oItemSize.width;
    
}
float KFavoriteAdapter::getItemHeight()
{
    if (g_isDebugMode) {
        log("getItemHeight()======%f", m_oItemSize.height == 0 ? 660 : m_oItemSize.height);
    }
    return m_oItemSize.height == 0 ? 660 : m_oItemSize.height;
    
}
