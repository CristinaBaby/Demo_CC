//
//  NumEditBox.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#ifndef NumEditBox_hpp
#define NumEditBox_hpp

#include"cocos2d.h"
#include "cocostudio/cocostudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace std;
class NumEditBox: public ui::EditBox,ui::EditBoxDelegate {
public:
    NumEditBox();
    static NumEditBox* create(Size, cocos2d::ui::Scale9Sprite *normalSprite);
public:
    void setPattern(const string& str);
    void openKeyboard();
    void closeKeyboard();
    void setText(const char* _tex);
protected:
    string currentTex;
    string pattern;
public:
    virtual void editBoxTextChanged( ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox){};
    static const string pricePattern;
protected:
    virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info)   {}
 
    virtual void keyboardDidShow(IMEKeyboardNotificationInfo& info)    {}
  
};

#endif /* NumEditBox_hpp */
