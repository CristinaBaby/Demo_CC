//
//  KShopAdapter.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-12.
//
//

#ifndef __OreoMaker__KShopAdapter__
#define __OreoMaker__KShopAdapter__

#include <iostream>
#include "cocos2d.h"
#include "KSSimpleAdapter.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "../utils/AdapterScreen.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

class KShopAdapter: public KSSimpleAdapter
{
public:
    KShopAdapter(const Vector<Sprite*>& data);
    ~KShopAdapter();
public:
    static KShopAdapter* create(const Vector<Sprite*>& data);
public:
    bool init();
    virtual MenuItem* getView(int position,MenuItem* cacheView,Node* parent);
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    void setBuyCallback(function<void(int,cocos2d::ui::Button*)> callback);
protected:
    function<void(int,cocos2d::ui::Button*)>  m_btClickCall;
};

#endif /* defined(__OreoMaker__KShopAdapter__) */
