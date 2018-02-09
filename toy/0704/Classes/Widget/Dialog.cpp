//
//  Dialog.cpp
//  ColorBook
//
//
//

#include "Dialog.h"
#include "UITools.h"
#include "UICpp.h"


#define DIALOG_PATH     "cocos/dialog/"

static Dialog *_sharedDialog = nullptr;

Dialog* Dialog::getInstance()
{
    if (! _sharedDialog)
    {
        _sharedDialog = new Dialog();
    }
    
    return _sharedDialog;
}

void Dialog::destroyInstance()
{
    CC_SAFE_DELETE(_sharedDialog);
}

Dialog::Dialog()
{
    _root = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("dialog.json");
    quickLayout("root", _root, CC_CALLBACK_2(Dialog::onButton, this));
    CC_SAFE_RETAIN(_root);
    
    _left = quickButton("left", _root, CC_CALLBACK_2(Dialog::onButton, this));
    _right = quickButton("right", _root, CC_CALLBACK_2(Dialog::onButton, this));
    _text = (ImageView*)Helper::seekWidgetByName(_root, "text");
}

Dialog::~Dialog()
{
    CC_SAFE_RELEASE(_root);
}

void Dialog::show(const string& text, const string& left, const string& right, const ccDialogCallback &callback)
{
    if(_root->getParent() == nullptr)
    {
        _callback = callback;
        
        _text->loadTexture(DIALOG_PATH + text + ".png");
        _left->loadTextureNormal(DIALOG_PATH + left + ".png");
        _left->setName(left);
        _right->loadTextureNormal(DIALOG_PATH + right + ".png");
        _right->setName(right);
        
        Director::getInstance()->getRunningScene()->addChild(_root, 100);
    }
}

void Dialog::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    if(_callback)
    {
        _callback(name);
    }
    
    _root->removeFromParent();
}

void Dialog::setUserData(int userData)
{
    _userData = userData;
}

int Dialog::getUserData()
{
    return _userData;
}

