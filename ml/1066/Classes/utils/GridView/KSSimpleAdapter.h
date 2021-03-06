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

class KSSimpleAdapter : public KDAdapter
{
    
protected:
    
    KSSimpleAdapter(const Vector<Sprite*>& data, const Vector<Sprite*>& data2);
    
    virtual ~KSSimpleAdapter();
    
public:
    
    static KSSimpleAdapter* create(const Vector<Sprite*>& data, const Vector<Sprite*>& data2 = Vector<Sprite*>());
    
public:
    virtual int getCount();
    
    virtual bool isEmpty();
    
    virtual float getItemWidth();
    
    virtual float getItemHeight();
    virtual float getTotalHeight(int count = 0);
    
    virtual MenuItem* getView(int position,MenuItem* cacheView,Node* parent);
    
public:
    
    void setData(const Vector<Sprite*>& data);
    
    void setScaleValue(float value = 1.0f);
    
public:
    Vector<Sprite*> m_Data;
    Vector<Sprite*> m_Data2;
    
    float m_fScale;
};

#endif /* defined(__KidsFrameWorkTest__KASimpleAdapter__) */
