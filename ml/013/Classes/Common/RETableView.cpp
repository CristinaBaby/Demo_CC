//
//  RETableView.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-18.
//
//

#include "RETableView.h"


RETableView* RETableView::create(TableViewDataSource* dataSource, Size size)
{
    RETableView *table = new RETableView();
    table->initWithViewSize(size, nullptr);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

void RETableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    
    ScrollView::onTouchEnded(pTouch, pEvent);

    
    if (!this->isVisible()) {
        return;
    }
    
    if (_touchedCell){
		Rect bb = this->getBoundingBox();
		bb.origin = _parent->convertToWorldSpace(bb.origin);
        
		if (bb.containsPoint(pTouch->getLocation()) && m_RETableViewDelagate != nullptr)
        {
            m_RETableViewDelagate->tableCellTouchedWithTouch(this, _touchedCell,pTouch,pEvent);
        }
        
        _touchedCell = nullptr;
    }
    

}

void RETableView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    TableView::onTouchCancelled(pTouch, pEvent);
    m_RETableViewDelagate -> tableCellTouchCancelled();
}
