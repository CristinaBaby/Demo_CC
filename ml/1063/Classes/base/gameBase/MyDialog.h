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
#include "StudioLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace std;

class MyDialog:public StudioLayer{
public:
    MyDialog();
    CREATE_FUNC(MyDialog);
    static MyDialog* create(std::string content,std::string root_path);
    bool init();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    virtual bool onBack();
    void actionEnd(float);
    std::function<void(MyDialog * dialog,int _touchInd)> dialogBtnClick;
protected:
    void show();
    void dismiss();
    int _touchId;
    std::string content;
    ui::Widget* bg;
    virtual void clickBtn(ui::Widget* widget);
public:
    static const  string oneBtnPath;
    static const  string twoBtnPath;
    static const  string yesName;
    static const  string noName;
    static const  string okName;
    static const  string lableName;
    static const  string bgName;
    static const  string closeName;
};
#endif /* defined(__MyCppGame__MyDialog__) */
