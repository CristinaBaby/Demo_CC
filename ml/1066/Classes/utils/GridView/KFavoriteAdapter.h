//
//  KFavoriteAdapter.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-12.
//
//

#ifndef __OreoMaker__KFavoriteAdapter__
#define __OreoMaker__KFavoriteAdapter__

#include <iostream>
#include "cocos2d.h"
#include "KSSimpleAdapter.h"
#include "AdapterScreen.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

class KFavoriteAdapter: public KSSimpleAdapter
{
public:
    
    KFavoriteAdapter(const Vector<Sprite*>& data);
    
    ~KFavoriteAdapter();
    
public:
    
    static KFavoriteAdapter* create(const Vector<Sprite*>& data);
    
public:
    
    bool init();
    
    virtual MenuItem* getView(int position,MenuItem* cacheView,Node* parent);
    
    virtual float getItemWidth();
    
    virtual float getItemHeight();
private:
    Size m_oItemSize;
};
#endif
/* defined(__OreoMaker__KFavoriteAdapter__) */
