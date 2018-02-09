//
//  MyDialog.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 9/25/14.
//
//

#include "MyDialog.h"
#include "KeyBackEvent.h"

#include "cocostudio/cocostudio.h"


MyDialog* MyDialog::create(std::string content,std::string root_path){
    auto pRt = new MyDialog();
    pRt->content = content;
    pRt->root_path = root_path;
    if(pRt&&pRt->init()){
        pRt->autorelease();
        return pRt;
    }
    delete pRt;
    pRt = nullptr;
    return nullptr;
}

MyDialog::MyDialog(){
    isShowing = true;
    setLocalZOrder(99999);
    root_path="";
    content="";
    dialogBtnClick = nullptr;
    bg = nullptr;
    root = nullptr;
}

bool MyDialog::init(){
    if(EventLayer::init()){
        _localZOrder=  9999;
        setOpacity(0);
        root = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(root_path.c_str());
        addChild(root,2);
      
        
        auto label = (cocos2d::ui::Widget*)ui::Helper::seekNodeByName(root, lableName);
        if(dynamic_cast<ui::TextBMFont*>(label)){
            auto _node = dynamic_cast<ui::TextBMFont*>(label);
              _node->setString(content);
            auto _texL = dynamic_cast<Label*>(label->getVirtualRenderer());
            _texL->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            _texL->setDimensions(380, 130);
        } else if(dynamic_cast<ui::Text*>(label)== nullptr){
            dynamic_cast<ui::Text*>(label)->setString(content);
        }
      
        bg = (cocos2d::ui::Widget*)ui::Helper::seekNodeByName(root, bgName);
        bg->setAnchorPoint(Vec2(0.5,0.5));
        bg->setVisible(false);
     
        return true;
    }
    return false;
}

void MyDialog::show(){
   // auto bgScale = ScaleTo::create(0.25, 1);
    auto action = EaseBackOut::create(ScaleTo::create(.3f, 1));
    bg->runAction(Sequence::create(ScaleTo::create(0, 0),CallFunc::create([&](){bg->setVisible(true);}), action,CallFunc::create([&](){
        isShowing = false;
    }),nullptr));
}
void MyDialog::dismiss(int _index){
    isShowing = true;
    index = _index;
    auto bgScale = ScaleTo::create(0.15f, 0);
    auto action = EaseExponentialIn::create(bgScale);
    bg->runAction(Sequence::create(action, CallFunc::create([&](){
        this->scheduleOnce(schedule_selector(MyDialog::actionEnd), 0);
    }),nullptr));
    
}

void MyDialog::actionEnd(float){
    removeFromParentAndCleanup(false);
    if(dialogBtnClick!= nullptr)
        dialogBtnClick(this,_tag,index);
}

void MyDialog::addLis(ui::Widget* root){
    if(root != nullptr){
        auto d = dynamic_cast<ui::Button*>(root);
        if(d != nullptr) {
            d->addTouchEventListener(CC_CALLBACK_2(MyDialog::touchBtn, this));
            d->setPressedActionEnabled(true);
        }
        for(auto _chil:root->getChildren())
            addLis(dynamic_cast<ui::Widget*>(_chil));
    }
}

void MyDialog::removeLis(ui::Widget* root){
    if(root != nullptr){
        root->addTouchEventListener(nullptr);
        for(auto _chil:root->getChildren())
            removeLis(dynamic_cast<ui::Widget*>(_chil));
    }
}

void MyDialog::touchBtn(Ref* ref,ui::Widget::TouchEventType type) {
    if(isShowing)
        return;
    if(type == ui::Widget::TouchEventType::ENDED) {
        auto widget = dynamic_cast<ui::Widget*>(ref);
        if(nullptr != widget){
            if(std::string(okName).compare(widget->getName())==0)
                dismiss(0);
            else if(std::string(noName).compare(widget->getName())==0)
                dismiss(1);
            else if(std::string(yesName).compare(widget->getName())==0)
                dismiss(0);
            else if(std::string(closeName).compare(widget->getName())== 0)
                dismiss(1);
        }
    }
}

 void MyDialog::onEnter(){
    KeyBackEvent::getInstance()->addBackEvent(CC_CALLBACK_0(MyDialog::onBack, this),this);
    EventLayer::onEnter();
    addLis(root);
    show();
}

void MyDialog::onExit() {
    removeLis(root);
    EventLayer::onExit();
}

bool MyDialog::onBack(){
    dismiss(1);
    return true;
}
