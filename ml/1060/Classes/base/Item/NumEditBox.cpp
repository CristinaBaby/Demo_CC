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
,pattern(""){
}

NumEditBox*  NumEditBox::create(Size s, cocos2d::ui::Scale9Sprite *normalSprite){
    NumEditBox* _ps = new NumEditBox();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _ps->setDelegate(_ps);
#endif
    _ps->initWithSizeAndBackgroundSprite(s, normalSprite);
    _ps->autorelease();
    return _ps;
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


