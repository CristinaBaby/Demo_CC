//
//  MyDialog.h
//  MyCppGame
//
//  Created by luotianqiang1 on 9/25/14.
//
//

#ifndef __MyCppGame__MyDialog__
#define __MyCppGame__MyDialog__

#include <iostream>
#include "cocos2d.h"
#include "EventLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

#define oneBtnPath "ui/popup_save.json"
#define twoBtnPath "ui/popup.json"
#define yesName "popup_btn_yes"
#define noName "popup_btn_no"
#define okName "popup_btn_ok"
#define lableName "text"
#define bgName "popup_box"
#define closeName "popup_btn_close"

class MyDialog:public EventLayer{
public:
    MyDialog();
    CREATE_FUNC(MyDialog);
    static MyDialog* create(std::string content,std::string root_path);
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onBack();
    void actionEnd(float);
    std::function<void(MyDialog * dialog,int dialogTag,int index)> dialogBtnClick;
protected:
    bool isShowing;
    void show();
    void dismiss(int tag);
    void touchBtn(Ref* ref,ui::Widget::TouchEventType type);
    int index;
    std::string content,root_path;
    ui::Widget* bg,*root;
    void addLis(ui::Widget* root);
    void removeLis(ui::Widget* root);
};
#endif /* defined(__MyCppGame__MyDialog__) */
