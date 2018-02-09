//
//  MyTableView.hpp
//  lqtest
//
//  Created by luotianqiang1 on 16/1/14.
//
//

#ifndef MyTableView_hpp
#define MyTableView_hpp

#include <iostream>
#include "cocos2d.h"
#include "ViewAdapter.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
class MyTableView: public cocos2d::extension::TableView, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate{
public:
    static MyTableView* create( Size size);
    MyTableView();
    ~MyTableView();
  
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
     virtual void tableCellWillRecycle(TableView* table, cocos2d::extension::TableViewCell* cell)override;
public:
    virtual void setAdapter(ViewAdapter*);
     const  Vector< cocos2d::extension::TableViewCell*>& getCellsUsed();
protected:
    ViewAdapter* _adapter;
public:
    static const int MyTableViewItemTag;
};


#endif /* MyTableView_hpp */
