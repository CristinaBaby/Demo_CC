//
//  Dialog.h
//  ColorBook
//
//
//

#ifndef __ColorBook__Dialog__
#define __ColorBook__Dialog__

#include "UIHeader.h"

#define xDialog         (Dialog::getInstance())

typedef std::function<void(const string& )> ccDialogCallback;

class Dialog
{
public:
    static Dialog* getInstance();
    
    void show(const string& text, const string& left, const string& right, const ccDialogCallback &callback);
    
public:
    ~Dialog();
    void destroyInstance();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void setUserData(int userData);
    int getUserData();
    
protected:
    Dialog();
    
private:
    Layout*             _root;
    Button*             _left;
    Button*             _right;
    ImageView*          _text;
    ccDialogCallback    _callback;
    int                 _userData;
};

#endif /* defined(__ColorBook__Dialog__) */
