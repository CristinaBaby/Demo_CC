//
//  MyMenu.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/16.
//
//

#ifndef MyMenu_hpp
#define MyMenu_hpp

#include "BaseStepLayer.h"
#include "MyTableView.h"
#include "EventSprite.h"
class MyMenu: public BaseStepLayer{
    DECLARE_DYNCLASS(MyMenu);
public:
    ~MyMenu();
    CREATE_FUNC(MyMenu);
        void show(Vec2 pos);
protected:
    bool init();
    virtual void touchEnd(ui::Widget* widget);
    virtual void touchLabel(KDS::EventSprite*,int,Touch*);
   MyTableView *_listView;
    vector<string> _filsePath;

    void close();
    virtual bool onBack();
    Vec2 _starPos;
    Node* _bg;
};


#endif /* MyMenu_hpp */
