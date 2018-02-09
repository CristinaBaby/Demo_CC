//
//  UnitSelView.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-5.
//
//

#ifndef __MakeSnowMan__UnitSelView__
#define __MakeSnowMan__UnitSelView__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "RETableView.h"
USING_NS_CC;
USING_NS_CC_EXT;
class UnitSelViewDelegate
{
public:
    virtual void shopClicked()= 0 ;
    virtual void unitSelected(const char* pUnitImageName) = 0;
};


class UnitSelView : public Node, public RETableViewDelegate ,TableViewDataSource
{
public:
    static UnitSelView* create(int orTypeIdx);
    bool init(int orTypeIdx);
    void setDelegate(UnitSelViewDelegate* delegate){delegate_ = delegate ;};
    
private:
    CC_SYNTHESIZE(RETableView*, unitSelTableView_, UnitSelTableView);
    void initWithTableView(int orTypeIdx);
    
    __Array* unitArray_;
    
    void touchEvent(Ref *obj , ui::Widget::TouchEventType type);
    
    //datasource
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    
    //delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent);
    
    __String* currentTypeName_;
    
    bool unlockType_;
    UnitSelViewDelegate* delegate_;
    
    bool isClicked_;
    

};
#endif /* defined(__MakeSnowMan__UnitSelView__) */
