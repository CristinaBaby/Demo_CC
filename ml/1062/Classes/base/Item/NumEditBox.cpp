//
//  NumEditBox.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#include "NumEditBox.h"
#include <regex>
#include <iostream>
#include "ui/UIEditBox/UIEditBoxImpl.h"
using namespace ui;

//const string NumEditBox::pricePattern = "(^[1-9]\\d*(\\.\\d{1,})?)|(^[0]{1}(\\.\\d{1,})?)";
const string NumEditBox::pricePattern = "\\d*\\.?\\d*";

NumEditBox::NumEditBox():
currentTex("")
,pattern("")
,_changed(nullptr){
    
}

NumEditBox*  NumEditBox::create(Size s, cocos2d::ui::Scale9Sprite *normalSprite){
    NumEditBox* _ps = new NumEditBox();
    _ps->initWithSizeAndBackgroundSprite(s, normalSprite);
    _ps->autorelease();
    return _ps;
}

bool NumEditBox::initWithSizeAndBackgroundSprite(Size s, cocos2d::ui::Scale9Sprite *normalSprite){
    if(EditBox::initWithSizeAndBackgroundSprite(s,normalSprite)){
        setInputMode(ui::EditBox::InputMode::NUMERIC);
        setMaxLength(5);
        setDelegate(this);
        setFontColor(Color3B::BLACK);
        return true;
    }
    return false;
}

void  NumEditBox::setPattern(const string& str){
    pattern = str;
}

void NumEditBox::openKeyboard() {
    if(_editBoxImpl != nullptr)
        _editBoxImpl->openKeyboard();
}
void NumEditBox::closeKeyboard(){
    if(_editBoxImpl != nullptr)
        _editBoxImpl->closeKeyboard();
}

void NumEditBox::setText(const char* str){
    currentTex = str;
    EditBox::setText(str);
}

 void  NumEditBox::editBoxTextChanged( ui::EditBox* editBox, const std::string& text) {
     bool isMatch = true;
     if(!pattern.empty()){
        std::regex _pattern(pattern);
        isMatch = regex_match(text,_pattern);
     }
     if(isMatch)
           currentTex  = text;
     editBox->setText(currentTex.c_str());
 };


void  NumEditBox::editBoxReturn( ui::EditBox* editBox){
    
}

void NumEditBox::keyboardWillShow(IMEKeyboardNotificationInfo& info){
    // CCLOG("CCEditBox::keyboardWillShow");
    Size contentSize = this->getContentSize();
    Rect rect = Rect(0, 0, contentSize.width, contentSize.height);
    Rect rectTracked =RectApplyTransform(rect, this->getNodeToWorldTransform());
    // some adjustment for margin between the keyboard and the edit box.
    rectTracked.origin.y -= 4;
    
    // if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
    if (!rectTracked.intersectsRect(info.end))
    {
        CCLOG("needn't to adjust view layout.");
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    _adjustHeight = info.end.getMaxY() - rectTracked.getMinY();
    // CCLOG("CCEditBox:needAdjustVerticalPosition(%f)", _adjustHeight);
//    
//    if (_editBoxImpl != nullptr)
//    {
//        _editBoxImpl->doAnimationWhenKeyboardMove(info.duration, _adjustHeight);
//    }
    if(_changed != nullptr)
        _changed(_adjustHeight);
}

void NumEditBox::keyboardWillHide(IMEKeyboardNotificationInfo& info)
{
//    // CCLOG("CCEditBox::keyboardWillHide");
//    if (_editBoxImpl != nullptr)
//    {
//        _editBoxImpl->doAnimationWhenKeyboardMove(info.duration, -_adjustHeight);
//    }
    if(_changed != nullptr)
        _changed(-_adjustHeight);
}

void NumEditBox::keyboardDidShow(IMEKeyboardNotificationInfo& info){
    
}


void NumEditBox::keyboardDidHide(IMEKeyboardNotificationInfo& info){
   
}

