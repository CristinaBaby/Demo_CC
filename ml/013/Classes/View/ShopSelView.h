//
//  ShopSelView.h
//  Make
//
//  Created by QAMAC01 on 15-4-13.
//
//

#ifndef __Make__ShopSelView__
#define __Make__ShopSelView__

#include <stdio.h>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "RETableView.h"
USING_NS_CC;
USING_NS_CC_EXT;

class ShopSelView : public Node, public RETableViewDelegate ,TableViewDataSource
{
public:
    CREATE_FUNC(ShopSelView);
    bool init();
    std::function<void()>callback = nullptr;

private:
    CC_SYNTHESIZE(RETableView*, unitSelTableView_, UnitSelTableView);
    void initWithTableView();
    
    __Array* unitArray_;
    
    void touchEvent(Ref *obj , ui::Widget::TouchEventType type);
    void checkUnlock();
    
    //datasource
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    
    //delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellTouchedWithTouch(TableView* table, TableViewCell* cell,Touch *pTouch,Event *pEvent);
    
    __String* currentTypeName_;
    
    bool unlockType_;
    
    bool isClicked_;
    
    void purchaseCall(bool flag);
    void restoreCall(bool ,bool);
    
    virtual void onEnter();
    virtual void onExit();
    
};

#endif /* defined(__Make__ShopSelView__) */
