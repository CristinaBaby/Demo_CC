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
class NumEditBox: public ui::EditBox,public ui::EditBoxDelegate {
public:
    NumEditBox();
    static NumEditBox* create(Size, cocos2d::ui::Scale9Sprite *normalSprite);
    virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info) override;
    /**
     * @js NA
     * @lua NA
     */
    virtual void keyboardDidShow(IMEKeyboardNotificationInfo& info) override;
    /**
     * @js NA
     * @lua NA
     */
    virtual void keyboardWillHide(IMEKeyboardNotificationInfo& info) override;
    /**
     * @js NA
     * @lua NA
     */
    virtual void keyboardDidHide(IMEKeyboardNotificationInfo& info) override;
protected:
    bool initWithSizeAndBackgroundSprite(Size, cocos2d::ui::Scale9Sprite *normalSprite);
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
    
    /**
     * This method is called when the return button was pressed or the outside area of keyboard was touched.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn( ui::EditBox* editBox);
    static const string pricePattern;
    std::function<void(float)> _changed;
};

#endif /* NumEditBox_hpp */
