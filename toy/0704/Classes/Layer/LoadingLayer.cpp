//
//  LoadingLayer.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/14.
//
// 

#include "LoadingLayer.h"
#include "UICpp.h"

bool LoadingLayer::init(){
    if (Layer::init()) {
        auto root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("loading.json"));
        addChild(root);
        
        root->addTouchEventListener(CC_CALLBACK_2(LoadingLayer::onButton, this));
        
        _tapImage = (ImageView*)Helper::seekWidgetByName(root, "tap to start");
        _tapImage->setVisible(false);
        
        return true;
    }
    return false;
}

void LoadingLayer::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;

    if(_tapToStart)
    {
        _listView->scrollToLeft(2, false);
        removeFromParent();
    }
}

void LoadingLayer::showTapToStart()
{
    _tapToStart = true;
    _tapImage->setVisible(true);
}

void LoadingLayer::setListView(ListView * listview)
{
    _listView = listview;
}