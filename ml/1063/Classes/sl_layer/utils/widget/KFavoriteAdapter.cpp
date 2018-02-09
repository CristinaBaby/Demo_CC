//
//  KFavoriteAdapter.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-12.
//
//

#include "KFavoriteAdapter.h"
#include "Utils.h"
KFavoriteAdapter::KFavoriteAdapter(const Vector<Sprite*>& data):KSSimpleAdapter(data),m_btClickCall(nullptr)
{
    
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

MenuItem* KFavoriteAdapter::getView(int position,MenuItem* cacheView,Node* parent)
{
    Sprite* item =m_Data.at(position);
    Sprite* bg = Utils::getUISprite("images/shop_fav/frame.png");
    float scaleX = (bg->getContentSize().width-7)/item->getContentSize().width;
    float scaleY = (bg->getContentSize().height-7)/item->getContentSize().height;
    item->setScale(MIN(scaleX, scaleY));
    item->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height/2));
    bg->addChild(item);

    MenuItemSprite* itemBtn = NULL;
    
    if (item)
    {
        itemBtn = MenuItemSprite::create(bg, NULL);
        itemBtn->setUserObject(__String::createWithFormat("%d",position));
        auto m_pDeleteButton = cocos2d::ui::Button::create("images/shop_fav/btn_delete@2x.png");
        m_pDeleteButton->setTag(101);
        m_pDeleteButton -> addTouchEventListener(CC_CALLBACK_2(KFavoriteAdapter::touchEvent, this));
        //m_pDeleteButton->setVisible(false);
        m_pDeleteButton->setAnchorPoint(Vec2(0.5,0.5));
        m_pDeleteButton->setPosition(Vec2(itemBtn->getContentSize().width,itemBtn->getContentSize().height - 10));
        itemBtn->addChild(m_pDeleteButton,2);
    }
    return itemBtn;
    
    
}

void KFavoriteAdapter::touchEvent(Ref *obj ,Widget::TouchEventType type)
{
    if(Widget::TouchEventType::ENDED == type)
    {
        auto* item =dynamic_cast<cocos2d::ui::Button*>(obj);
        if(m_btClickCall)
            m_btClickCall(((__String*)item->getParent()->getUserObject())->intValue(),item);
    }
}
void KFavoriteAdapter::setDeleteCallback(function<void(int,cocos2d::ui::Button*)> callback)
{
    m_btClickCall=callback;
}

float KFavoriteAdapter::getItemWidth()
{
    return 350;
}
float KFavoriteAdapter::getItemHeight()
{
    return 863;
    
}
