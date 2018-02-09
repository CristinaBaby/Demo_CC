//
//  MyDialog.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 9/25/14.
//
//

#include "MyDialog.h"
#include "cocostudio/cocostudio.h"
#include "KeyBackEvent.h"

const string MyDialog::oneBtnPath = "pop_ok.csb";
const string MyDialog::twoBtnPath = "pop_yesno.csb";
const string MyDialog::yesName = "btn_yes";
const string MyDialog::noName = "btn_no";
const string MyDialog::okName = "btn_ok";
const string MyDialog::lableName = "pop_Text";
const string MyDialog::bgName = "popup_bg";
const string MyDialog::closeName = "btn_close";

MyDialog* MyDialog::create(std::string content,std::string root_path){
    auto pRt = new MyDialog();
    pRt->content = content;
    pRt->_fileName = root_path;
    if(pRt&&pRt->init()){
        pRt->autorelease();
        return pRt;
    }
    delete pRt;
    pRt = nullptr;
    return nullptr;
}

MyDialog::MyDialog(){
    setLocalZOrder(600);
    content="";
    dialogBtnClick = nullptr;
    bg = nullptr;
    _touchId = -1;
}

bool MyDialog::init(){
    if(StudioLayer::init()){
        _localZOrder=  600;
        
        auto label = ui::Helper::seekNodeByName(_root, lableName);
        if(dynamic_cast<ui::TextBMFont*>(label)){
            auto _node = dynamic_cast<ui::TextBMFont*>(label);
            _node->setString(content);
            auto _texL = dynamic_cast<Label*>(_node->getVirtualRenderer());
            _texL->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            _texL->setDimensions(380, 130);
        } else if(dynamic_cast<ui::Text*>(label)!= nullptr){
            dynamic_cast<ui::Text*>(label)->setString(content);
        }
        
        bg = dynamic_cast<ui::Widget*>(ui::Helper::seekNodeByName(_root, bgName));
        uiAction->gotoFrameAndPause(0);
        return true;
    }
    return false;
}

void MyDialog::show(){
    playAction("in",[&](){isChange = false;});
}
void MyDialog::dismiss(){
        playAction("out",[&](){
            isChange = false;
              actionEnd(0);
        });
}
void MyDialog::actionEnd(float){
    retain();
    removeFromParent();
    if(dialogBtnClick!= nullptr)
        dialogBtnClick(this,_touchId);
    release();
}

void MyDialog::clickBtn(ui::Widget* widget) {

    if(nullptr != widget){
        if(std::string(okName).compare(widget->getName())==0)
           _touchId = 0;
        else if(std::string(noName).compare(widget->getName())==0)
            _touchId = 1;
      
        else if(std::string(yesName).compare(widget->getName())==0)
            _touchId = 0;
        else if(std::string(closeName).compare(widget->getName())== 0)
            _touchId = 1;
        if(_touchId != -1){
                isChange = true;
            dismiss();
        }
    }
}

void MyDialog::onEnterTransitionDidFinish(){
    StudioLayer::onEnterTransitionDidFinish();
    isChange = true;
    KeyBackEvent::getInstance()->addBackEvent(CC_CALLBACK_0(MyDialog::onBack, this), this);
    show();
}

void MyDialog::onExit() {
    StudioLayer::onExit();
}

bool MyDialog::onBack(){
    if(!isChange){
        _touchId = 1;
        dismiss();
    }
    return true;
}
