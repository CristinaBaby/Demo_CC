//
//  ChooseFlavorView.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-5.
//
//

#ifndef __MakeSnowMan__ChooseFlavorView__
#define __MakeSnowMan__ChooseFlavorView__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "RETableView.h"
USING_NS_CC;
USING_NS_CC_EXT;
class ChooseFlavorViewDelegate
{
public:
    virtual void shopClicked()= 0 ;
    virtual void unitSelected(const char* pUnitImageName) = 0;
};


class ChooseFlavorView : public Node, public RETableViewDelegate ,TableViewDataSource
{
public:
    static ChooseFlavorView* create(int orTypeIdx);
    bool init(int orTypeIdx);
    void setDelegate(ChooseFlavorViewDelegate* delegate){delegate_ = delegate ;};
    void reloadData();
    
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
    ChooseFlavorViewDelegate* delegate_;
    
    bool isClicked_;
    
    
};
#endif /* defined(__MakeSnowMan__ChooseFlavorView__) */
