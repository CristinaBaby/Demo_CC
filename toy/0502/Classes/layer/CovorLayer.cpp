

//
//  CovorLayer.cpp
//  MyGame_Project
//
//  Created by liyang1 on 12/1/14.
//
//

#include "CovorLayer.h"
#include "../AppGlobal.h"

USING_NS_CC;



bool CovorLayer::init(Color4B color)
{
    //实现模态对话框
    initWithColor(color) ;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    return true;
}
