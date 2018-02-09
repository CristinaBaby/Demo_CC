//
//  KSDecorationAdapter.h
//  MLABS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/29.
//
//

#ifndef __MLABS1044_BeachFoodParty__KSDecorationAdapter__
#define __MLABS1044_BeachFoodParty__KSDecorationAdapter__

#include <stdio.h>
#include "cocos2d.h"
#include "KDAdapter.h"

USING_NS_CC;

//装饰界面的itemAdapter
class KSDecorationAdapter : public KDAdapter{
    
protected:
    KSDecorationAdapter(const Vector<Sprite*>& data);
    virtual ~KSDecorationAdapter();
public:
    static KSDecorationAdapter* create(const Vector<Sprite*>& data);
public:
    virtual int getCount();
    
    virtual bool isEmpty();
    
    virtual float getItemWidth();
    
    virtual float getItemHeight();
  
    
    virtual MenuItem* getView(int position,MenuItem* cacheView,Node* parent);
public:
    void setData(const Vector<Sprite*>& data);
public:
    Vector<Sprite*> m_Data;
};
#endif /* defined(__MLABS1044_BeachFoodParty__KSDecorationAdapter__) */
