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
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
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
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    void setDeleteCallback(function<void(int,cocos2d::ui::Button*)> callback);
protected:
    function<void(int,cocos2d::ui::Button*)>  m_btClickCall;
    //    virtual void onExit();

private:
    Sprite* m_pBg;
};
#endif
/* defined(__OreoMaker__KFavoriteAdapter__) */
