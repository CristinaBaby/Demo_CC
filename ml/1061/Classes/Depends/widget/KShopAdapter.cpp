//
//  KShopAdapter.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-12.
//
//

#include "KShopAdapter.h"
#include "../utils/Utils.h"
KShopAdapter::KShopAdapter(const Vector<Sprite*>& data):KSSimpleAdapter(data),m_btClickCall(nullptr)
{
    
}

KShopAdapter::~KShopAdapter()
{
}

bool KShopAdapter::init()
{
    return true;
}


KShopAdapter* KShopAdapter::create(const Vector<Sprite*>& data)
{
    KShopAdapter* adapter = new KShopAdapter(data);
    if (adapter&&adapter->init())
    {
        adapter->autorelease();
        return adapter;
    }
    CC_SAFE_DELETE(adapter);
    return nullptr;
    
}

MenuItem* KShopAdapter::getView(int position,MenuItem* cacheView,Node* parent)
{
    Sprite* item =(Sprite*)(m_Data.at(position));
    MenuItemSprite* itemBtn = NULL;
    if (item)
    {
        itemBtn = MenuItemSprite::create(item, NULL);
        itemBtn->setUserObject(__String::createWithFormat("%d",position));
        
//        auto m_pBuyButton = cocos2d::ui::Button::create("images/shop_fav/btn_buy@2x.png");
//        m_pBuyButton->setTag(101);
//        m_pBuyButton -> addTouchEventListener(CC_CALLBACK_2(KShopAdapter::touchEvent, this));
//        m_pBuyButton->setVisible(false);
//        m_pBuyButton->ignoreAnchorPointForPosition(false);
//        m_pBuyButton->setAnchorPoint(Vec2(0.5,0.5));
//        m_pBuyButton->setPosition(Vec2(itemBtn->getContentSize().width/2,0));
//        itemBtn->addChild(m_pBuyButton,2);
//        auto m_pBuyButton = Sprite::create("images/shop_fav/shop_btn_buy.png");
//        m_pBuyButton->ignoreAnchorPointForPosition(false);
//        m_pBuyButton->setAnchorPoint(Vec2(0.5,0.5));
//        m_pBuyButton->setPosition(Vec2(itemBtn->getContentSize().width/2,197));
//        itemBtn->addChild(m_pBuyButton,2);
    }
    return itemBtn;
}

void KShopAdapter::touchEvent(Ref *obj ,Widget::TouchEventType type)
{
    if(Widget::TouchEventType::ENDED == type){
        auto* item =dynamic_cast<cocos2d::ui::Button*>(obj);
        if(m_btClickCall)
            m_btClickCall(((__String*)item->getParent()->getUserObject())->intValue(),item);
    }
}
void KShopAdapter::setBuyCallback(function<void(int,cocos2d::ui::Button*)> callback)
{
    m_btClickCall=callback;
}