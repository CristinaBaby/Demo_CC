//
//  KSDecorationAdapter.cpp
//  MLABS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/29.
//
//

#include "KSDecorationAdapter.h"
#include "../managers/IAPManager.h"


KSDecorationAdapter::KSDecorationAdapter(const Vector<Sprite*>& data):m_Data(data)
{
    
}

KSDecorationAdapter::~KSDecorationAdapter()
{
    
}


KSDecorationAdapter* KSDecorationAdapter::create(const Vector<Sprite*>& data)
{
    KSDecorationAdapter* adapter = new KSDecorationAdapter(data);
    if (adapter)
    {
        adapter->autorelease();
        return adapter;
    }
    CC_SAFE_DELETE(adapter);
    return nullptr;
}

int KSDecorationAdapter::getCount()
{
    return (int)m_Data.size();
}

bool KSDecorationAdapter::isEmpty()
{
    return m_Data.size() == 0;
    
}



float KSDecorationAdapter::getItemWidth()
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

float KSDecorationAdapter::getItemHeight()
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

MenuItem* KSDecorationAdapter::getView(int position,MenuItem* cacheView,Node* parent)
{
    Sprite* item =dynamic_cast<Sprite*>(m_Data.at(position));
    item->retain();
    item->removeFromParent();
    MenuItemSprite* itemBtn = NULL;
    if (item)
    {
        
        itemBtn = MenuItemSprite::create(item, NULL);
        
        ScaleTo* s1 = ScaleTo::create(0.1f,1.08f,0.90f);
        ScaleTo* s2 = ScaleTo::create(0.1f,0.90,1.08f);
        ScaleTo* s3 = ScaleTo::create(0.1f,1.0f,1.0f);
        itemBtn->runAction(Sequence::create(s1,s2,s3,NULL));
    }
    return itemBtn;
}

void KSDecorationAdapter::setData(const Vector<Sprite*>& data)
{
    m_Data.clear();
    m_Data = data;
}