//
//  MyTableView.cpp
//  lqtest
//
//  Created by luotianqiang1 on 16/1/14.
//
//

#include "MyTableView.h"
USING_NS_CC_EXT;

const int MyTableView::MyTableViewItemTag = 988989;
MyTableView* MyTableView::create(Size size)
{
    MyTableView *table = new (std::nothrow) MyTableView();
    table->initWithViewSize(size, nullptr);
    table->autorelease();
    table->setDataSource(table);
    table->setDelegate(table);
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    table->setDirection(ScrollView::Direction::HORIZONTAL);
    return table;
}

MyTableView::MyTableView():
_adapter(nullptr){
    
}
MyTableView::~MyTableView(){
    CC_SAFE_RELEASE_NULL(_adapter);
}


 void MyTableView::setAdapter(ViewAdapter* a){
     CC_SAFE_RETAIN(a);
     CC_SAFE_RELEASE_NULL(_adapter);
     _adapter = a;
    _updateCellPositions();
     _updateContentSize();
     reloadData();
}

const  Vector<TableViewCell*>& MyTableView::getCellsUsed(){
    return _cellsUsed;
}

void MyTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
    if(_adapter != nullptr){
        auto _click = _adapter->getItemClick();
        if(_click != nullptr){
            auto _item = cell->getChildByTag(MyTableViewItemTag);
            _click(_item, cell->getIdx());
        }
    }
}

Size MyTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
     if(_adapter == nullptr)
        return Size::ZERO;
    else
        return _adapter->getItemSize(idx);
}

TableViewCell* MyTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{

    TableViewCell *cell =nullptr;
    if(table != nullptr)
        cell = table->dequeueCell();
    if (!cell) {
        cell = new (std::nothrow) TableViewCell();
        cell->autorelease();
    }
    Node* _node = nullptr;
    if(_adapter != nullptr && idx<_adapter->getCount()){
        _node = _adapter->getItem(idx);
        _node->setTag(MyTableViewItemTag);
    }
    if(_node != nullptr){
        cell->removeAllChildren();
        cell->addChild(_node);
    }
    return cell;
}

ssize_t MyTableView::numberOfCellsInTableView(TableView *table)
{
    if(_adapter != nullptr)
        return _adapter->getCount();
    else
        return 0;
}

  void MyTableView::tableCellWillRecycle(TableView* table, TableViewCell* cell){
      if(_adapter != nullptr)
          _adapter->recycle(cell,cell->getIdx());
  };
