//
//  KASimpleAdapter.h
//  KidsFrameWorkTest
//
//  Created by zhangguangzong1 on 2/24/14.
//
//

#ifndef __KidsFrameWorkTest__KASimpleAdapter__
#define __KidsFrameWorkTest__KASimpleAdapter__

#include <iostream>
#include "cocos2d.h"
#include "KDAdapter.h"

USING_NS_CC;

class KSSimpleAdapter : public KDAdapter{
    
protected:
    KSSimpleAdapter(const Vector<Sprite*>& data);
    virtual ~KSSimpleAdapter();
public:
    static KSSimpleAdapter* create(const Vector<Sprite*>& data);
public:
    virtual int getCount();
    
    virtual bool isEmpty();
    
    virtual float getItemWidth();
    
    virtual float getItemHeight();
    
    void setType(int i);
    void setIapType(int i);
    
    virtual MenuItem* getView(int position,MenuItem* cacheView,Node* parent);
public:
    void setData(const Vector<Sprite*>& data);
public:
    Vector<Sprite*> m_Data;
    
public:
    int m_iType;
    int m_iIapType;
};

#endif /* defined(__KidsFrameWorkTest__KASimpleAdapter__) */
