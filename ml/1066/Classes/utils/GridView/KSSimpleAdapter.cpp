
//
//  KASimpleAdapter.cpp
//  KidsFrameWorkTest
//
//  Created by zhangguangzong1 on 2/24/14.
//
//

#include "KSSimpleAdapter.h"
#include "IAPManager.h"


KSSimpleAdapter::KSSimpleAdapter(const Vector<Sprite*>& data, const Vector<Sprite*>& data2):m_Data(data),m_Data2(data2),m_fScale(1.0f)
{
    
}

KSSimpleAdapter::~KSSimpleAdapter()
{
}


KSSimpleAdapter* KSSimpleAdapter::create(const Vector<Sprite*>& data, const Vector<Sprite*>& data2)
{
    KSSimpleAdapter* adapter = new KSSimpleAdapter(data, data2);
    
    if (adapter)
    {
        adapter->autorelease();
        return adapter;
    }
    
    CC_SAFE_DELETE(adapter);
    return nullptr;
}

int KSSimpleAdapter::getCount()
{
    return (int)m_Data.size();
}

bool KSSimpleAdapter::isEmpty()
{
    return m_Data.size() == 0;

}



float KSSimpleAdapter::getItemWidth()
{
    if (m_Data.size() > 0)
    {
        Node* node = dynamic_cast<Node*>(m_Data.at(0));
        if (node)
        {
            return node->getContentSize().width;
        }
        return 0;
    }else
    {
        return 0;
    }
}

float KSSimpleAdapter::getItemHeight()
{
    if (m_Data.size() > 0)
    {
        Node* node = dynamic_cast<Node*>(m_Data.at(0));
        if (node)
        {
            return node->getContentSize().height;
        }
        return 0;
    }else
    {
        return 0;
    }
}

float KSSimpleAdapter::getTotalHeight(int count)
{
    float height = 0;
    for (int i = 0; i < m_Data.size(); i++) {
        if (i >= count) {
            break;
        }
        Node* node = m_Data.at(i);
        height += node->getContentSize().height;
    }
    
    return height;
}

MenuItem* KSSimpleAdapter::getView(int position,MenuItem* cacheView,Node* parent)
{
    Sprite* item =dynamic_cast<Sprite*>(m_Data.at(position));

    MenuItemSprite* itemBtn = nullptr;
    if (item)
    {
        if (m_Data2.size() != 0)
        {
            Sprite* item_h =dynamic_cast<Sprite*>(m_Data2.at(position));

            itemBtn = MenuItemSprite::create(item, item_h);
        }
        else
        {
            itemBtn = MenuItemSprite::create(item, nullptr);
        }
    }
//    itemBtn->setScale(m_fScale);
//    itemBtn->getChildren().at(0)->setScale(m_fScale);
    for(int i = 0 ; i < itemBtn->getNormalImage()->getChildren().size(); i++){
        itemBtn->getNormalImage()->getChildren().at(i)->setScale(m_fScale);
        if (m_Data2.size() != 0) {
            itemBtn->getSelectedImage()->getChildren().at(i)->setScale(m_fScale);
        }
    }
    return itemBtn;
    
}

void KSSimpleAdapter::setData(const Vector<Sprite*>& data)
{
    m_Data.clear();
    m_Data = data;
}

void KSSimpleAdapter::setScaleValue(float value)
{
    m_fScale = value;
}
