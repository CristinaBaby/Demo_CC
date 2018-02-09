
//
//  KASimpleAdapter.cpp
//  KidsFrameWorkTest
//
//  Created by zhangguangzong1 on 2/24/14.
//
//

#include "KSSimpleAdapter.h"
#include "IAPManager.h"


string iapType[8]={"cake_select","mac","syrupcup","icecup","icecream","icespoon","mould","taste"};

string taskType[8] = {"cake_select","mac","syrupcup","icecup","icecream","icespoon","mould","taste"};

KSSimpleAdapter::KSSimpleAdapter(const Vector<Sprite*>& data):m_Data(data),m_iIapType(0)
{
    
}

KSSimpleAdapter::~KSSimpleAdapter()
{
    
}


KSSimpleAdapter* KSSimpleAdapter::create(const Vector<Sprite*>& data)
{
    KSSimpleAdapter* adapter = new KSSimpleAdapter(data);
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


void KSSimpleAdapter::setIapType(int i)
{

    m_iIapType = i;
    
}
MenuItem* KSSimpleAdapter::getView(int position,MenuItem* cacheView,Node* parent)
{
    Sprite* item =dynamic_cast<Sprite*>(m_Data.at(position));
    item->retain();
    item->removeFromParent();
    MenuItemSprite* itemBtn = NULL;
    if (item)
    {
        
        //先移除锁，防止解锁不及时
        if (item->getChildByTag(100)) {
            item->getChildByTag(100)->removeFromParent();
        }
        //判断IAP是否加锁
        if (!kIAPManager->isFree(iapType[m_iIapType].c_str(), position)) {
            
            Sprite* lock = nullptr;
            lock = Sprite::create("images/select/lock_cream@2x.png");
            
            lock->setAnchorPoint(Vec2::ZERO);
            lock->setTag(100);
            lock->setPosition(Vec2(90,70)+Vec2(-39,116));
            item->addChild(lock);
            
        }

        itemBtn = MenuItemSprite::create(item, NULL);

        ScaleTo* s1 = ScaleTo::create(0.1f,1.08f,0.90f);
        ScaleTo* s2 = ScaleTo::create(0.1f,0.90,1.08f);
        ScaleTo* s3 = ScaleTo::create(0.1f,1.0f,1.0f);
        itemBtn->runAction(Sequence::create(s1,s2,s3,NULL));
        
       
        
    }
    return itemBtn;
}

void KSSimpleAdapter::setData(const Vector<Sprite*>& data)
{
    m_Data.clear();
    m_Data = data;
}
