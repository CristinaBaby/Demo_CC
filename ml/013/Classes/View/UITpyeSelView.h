//
//  UITpyeSelView.h
//  Make
//
//  Created by liji on 15/8/6.
//
//

#ifndef __Make__UITpyeSelView__
#define __Make__UITpyeSelView__

#include <stdio.h>
#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "RETableView.h"
USING_NS_CC;
USING_NS_CC_EXT;



class UITpyeSelView : public Layer
{
public:
    CREATE_FUNC(UITpyeSelView);
    
    bool init();
    
    
    void loadTypeListFromTo(int idxFrom,int idxTo);
    
    void enableTouch(bool enable);
    
    void setSelectNone();
    
    std::function<void(std::string typeName)> typeSelected = nullptr;
    std::function<void()> typeUnSelected = nullptr;
    
    
private:
    void touchEvent(Ref*,ui::CheckBox::EventType);
    
    //获取选中名字
    std::string getSelectedName(std::string pName);
    
private:
    ui::ScrollView* _typeScrollView = nullptr;
    ui::CheckBox* _selectedBtn = nullptr;
    bool _isClicked = false; //防止连续点击

    
};
#endif /* defined(__Make__UITpyeSelView__) */
