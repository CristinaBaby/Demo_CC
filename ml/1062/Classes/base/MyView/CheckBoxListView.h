//
//  CheckBoxListView.h
//  CakePop
//
//  Created by luotianqiang1 on 15/7/13.
//
//

#ifndef __CakePop__CheckBoxListView__
#define __CakePop__CheckBoxListView__

#include"cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class CheckBoxListView {
public:
    CheckBoxListView();
private:
    Node* _container;
    ui::CheckBox* _curent;
    bool _enable;
    void addLis(Node* root);
    int _itemDis;
public:
    void setContainer(Node* _container);
    std::function<void(ui::CheckBox*,int index)> selectClick,unSelectClick;//如果不需要取消选中状态，就不要给unSelectClick赋值
    void preScolll();
    void nextScoll();
    void resetState();
    void setEnable(bool);
    ui::ScrollView* getListView();
    Node* getContainer();
    bool isEnable(){return _enable;}
    void chooseIndex(int _index);
protected:
    void touchBtn(Ref*,ui::CheckBox::EventType);
};

#endif /* defined(__CakePop__CheckBoxListView__) */
