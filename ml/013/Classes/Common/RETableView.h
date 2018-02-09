//
//  RETableView.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-18.
//
//

//celltouch代理中获取touch事件

#ifndef __MakeSnowMan__RETableView__
#define __MakeSnowMan__RETableView__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class RETableViewDelegate : public TableViewDelegate
{
public:
    virtual void tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent)= 0;
    virtual void tableCellTouchCancelled(){};
};


class RETableView : public TableView
{
public:
    static RETableView* create(TableViewDataSource* dataSource, Size size);
    void setDelegate(RETableViewDelegate* pDelegate) { m_RETableViewDelagate = pDelegate; }


protected:
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;
    RETableViewDelegate* m_RETableViewDelagate;

    
};

#endif /* defined(__MakeSnowMan__RETableView__) */
