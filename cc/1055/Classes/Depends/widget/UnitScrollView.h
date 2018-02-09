//
//  UnitScrollView.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-5.
//
//

#ifndef __OreoMaker__UnitScrollView__
#define __OreoMaker__UnitScrollView__

#include <iostream>
#include "../widget/KDGridView.h"
#include "../widget/KSSimpleAdapter.h"
#include "cocos2d.h"
#include "KSDecorationAdapter.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;
USING_NS_CC;
using namespace std;

//用于装饰界面的装饰物
class UnitScrollView:public KDGridView{
private:
    UnitScrollView();
    ~UnitScrollView();
    
public:
    virtual bool init(Size size);
    static UnitScrollView* create(Size size);
    
private:
    bool initUI();
    void _onTypePageCallback(Ref* sender, Control::EventType controlEvent);
    
public:
    void loadingUnitScrollViewWithType(int index);
    std::string getDecType();
private:
    Vector<Sprite*> m_vDecorates;
    KSDecorationAdapter* m_pAdapter;
    string m_pType;
};

#endif /* defined(__OreoMaker__UnitScrollView__) */
