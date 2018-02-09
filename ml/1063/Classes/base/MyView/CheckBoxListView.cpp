//
//  CheckBoxListView.cpp
//  CakePop
//
//  Created by luotianqiang1 on 15/7/13.
//
//

#include "CheckBoxListView.h"
using namespace ui;

CheckBoxListView::CheckBoxListView():
selectClick(nullptr)
,unSelectClick(nullptr)
,_container(nullptr)
,_curent(nullptr)
,_itemDis(-1)
,_enable(true)
,userGary(false){
}


void CheckBoxListView::setContainer(Node* _container) {
    this->_container = _container;
    addLis(_container);
    if(_itemDis == -1){
       auto chils =  _container->getChildren();
        if(chils.size()>=2)
            _itemDis  = chils.at(1)->getPositionX() - chils.at(0)->getPositionX();
    }
}

void CheckBoxListView::preScolll() {
    auto _listView = dynamic_cast<ui::ScrollView*>(_container);
    if(_listView != nullptr && _enable) {
        int po = (int)-_listView->getInnerContainer()->getPositionX();
        float w = _listView->getInnerContainer()->getContentSize().width - _listView->getContentSize().width;
        auto pos =  (po+_itemDis/2)/_itemDis*_itemDis -_itemDis;
        _listView->scrollToPercentHorizontal(pos/w*100 , 0.2f, true);
    }
}
void CheckBoxListView::nextScoll(){
        auto _listView = dynamic_cast<ui::ScrollView*>(_container);
    if(_listView != nullptr&& _enable) {
        int po = (int)-_listView->getInnerContainer()->getPositionX();
        float w = _listView->getInnerContainer()->getContentSize().width - _listView->getContentSize().width;
        auto pos = (po+_itemDis/2)/_itemDis*_itemDis+_itemDis;
        _listView->scrollToPercentHorizontal(pos/w*100, 0.2f, true);
    }
}

void CheckBoxListView::resetState(){
    if(_curent != nullptr){
        _curent->setSelected(false);
        _curent->setColor(Color3B::WHITE);
    }
    _curent = nullptr;
}

void CheckBoxListView::setEnable(bool flag){
    _enable = flag;
}

Node* CheckBoxListView::getContainer(){
    return _container;
}

ui::ScrollView* CheckBoxListView::getListView(){
    return dynamic_cast<ui::ScrollView*>(_container);
}

void CheckBoxListView::chooseIndex(size_t _index){
    if(_container == nullptr || _index>=_container->getChildren().size()|| _curent == _container->getChildren().at(_index))
        return;
    if(_curent != nullptr)
        _curent->setSelected(false);
    _curent = dynamic_cast<ui::CheckBox*>(_container->getChildren().at(_index));
    if(_curent != nullptr)
    _curent->setSelected(true);
}

void CheckBoxListView::touchBtn(Ref* _ref,ui::CheckBox::EventType){
    auto _choose = dynamic_cast<CheckBox*>(_ref);
    if(_enable) {
        _choose->setColor(Color3B::WHITE);
        ActionInterval *ret = NULL;
        
        ScaleTo *lScale1 = ScaleTo::create(0.13, 1.0, 0.80);
        ScaleTo *lScale2 = ScaleTo::create(0.11, 0.82, 1.0);
        ScaleTo *lScale3 = ScaleTo::create(0.10, 1.0, 0.86);
        ScaleTo *lScale4 = ScaleTo::create(0.09, 0.88, 1.0);
        ScaleTo *lScale5 = ScaleTo::create(0.08, 1.0, 0.89);
        ScaleTo *lScale6 = ScaleTo::create(0.07, 1.0, 1.0);
        
//        ScaleTo *lScale1 = ScaleTo::create(0.13, 1.0, 0.90);
//        ScaleTo *lScale2 = ScaleTo::create(0.11, 0.92, 1.0);
//        ScaleTo *lScale3 = ScaleTo::create(0.10, 1.0, 0.96);
//        ScaleTo *lScale4 = ScaleTo::create(0.09, 0.98, 1.0);
//        ScaleTo *lScale5 = ScaleTo::create(0.08, 1.0, 0.99);
//        ScaleTo *lScale6 = ScaleTo::create(0.07, 0.99, 1.0);
        
        ret = Sequence::create(lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
          _choose->runAction(ret);

        if(_choose == _curent) {
            if(unSelectClick!= nullptr){
                _curent = nullptr;
                unSelectClick(_choose,_container->getChildren().getIndex(_choose));
                _choose->setColor(Color3B::WHITE);
            }
            else{
                _choose->setSelected(!_choose->isSelected());
                if(userGary){
                       _choose->setColor(Color3B::GRAY);
                }
            }
           
        } else {
            if(_curent != nullptr){
                _curent->setSelected(false);
                _curent->setColor(Color3B::WHITE);
            }
             _curent = _choose;
            if(userGary){
                  _curent->setColor(Color3B::GRAY);
            }
            
            if(selectClick != nullptr)
                selectClick(_choose,_container->getChildren().getIndex(_choose));
           
        }
    } else
        _choose->setSelected(!_choose->isSelected());
}

void CheckBoxListView::setUserGary(bool flag){
    userGary = flag;
}

void CheckBoxListView::addLis(Node* root){
    if(root != nullptr){
        auto d = dynamic_cast<ui::CheckBox*>(root);
        if(d != nullptr)
            d->addEventListener(CC_CALLBACK_2(CheckBoxListView::touchBtn,this));
        for(auto _chil:root->getChildren())
            addLis(_chil);
    }
}