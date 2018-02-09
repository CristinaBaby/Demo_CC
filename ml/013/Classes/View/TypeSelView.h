//
//  TypeSelView.h
//  Chups006
//
//  Created by QAMAC01 on 14-9-17.
//
//

#ifndef __Chups006__TypeSelView__
#define __Chups006__TypeSelView__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "UnitSelView.h"
USING_NS_CC;
USING_NS_CC_EXT;

class TypeSelViewDelegate
{
public:
    virtual void showUnitView() = 0;
    virtual void removeUnitView() = 0;
    
    
};

class TypeSelView : public Node, public RETableViewDelegate ,TableViewDataSource
{
    
public:
    //    static TypeSelView* create();
    static TypeSelView* create(int orTypeIdx);
    
    bool init(int orTypeIdx);
    void resetTypeSelView();
    void setDelegate(TypeSelViewDelegate* delegate) {delegate_ = delegate;};
    
private:
    CC_SYNTHESIZE(RETableView*, typeSelTableView_, TypeSelTableView);
    
    void initWithTableView();
    
    //maketypeconfig
    __Array* typeArray_;
    
    //datasource
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    
    //delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent);
    virtual void onExit();
    
    
    
    //记录选中的按钮
    int chosenBtnTag_;
    std::string chosenBtnNameH_;
    std::string chosenBtnName_;
    
    
    UnitSelView* unitView_;
    int orIdx_;
    
    TypeSelViewDelegate* delegate_;
    
    bool isClicked_;
};

#endif /* defined(__Chups006__TypeSelView__) */
