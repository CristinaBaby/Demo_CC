//
//  MyAdapter.cpp
//  MyCppGame
//
//  Created by youjinchao1 on 14-9-25.
//
//

#include "MyAdapter.h"
#include "IAPManager.h"

MyAdapter::MyAdapter(Vector<__String*> itemName, Vector<__String*> itemHName, std::string name){
    MyAdapter::itemName = itemName;
    iconName = name;
    if(!itemHName.empty()){
        MyAdapter::itemHName = itemHName;
    }
}

MyAdapter::MyAdapter(Vector<__String*> itemName, Vector<__String*> itemHName){
    MyAdapter::itemName = itemName;
    if(!itemHName.empty()){
        MyAdapter::itemHName = itemHName;
    }
}

int MyAdapter::getCount(){
    return (int)itemName.size();
}

bool MyAdapter::isEmpty(){
    return itemName.empty();
}

float MyAdapter::getItemWidth(){
    auto item = Sprite::create(itemName.at(0)->getCString());
    return item->getContentSize().width;
}

float MyAdapter::getItemHeight(){
    auto item = Sprite::create(itemName.at(0)->getCString());
    return item->getContentSize().height;
}

void MyAdapter::setIconName(const std::string name){
    iconName = name;
}

std::string MyAdapter::getIconName(){
    return iconName;
}

MenuItem* MyAdapter::getView(int position,MenuItem* cacheView,Node* parent){
    if(cacheView == NULL){
        std::string name = "";
        if(!itemHName.empty())
        {
            name = itemHName.at(position)->getCString();
        }
        cacheView = MenuItemImage::create(itemName.at(position)->getCString(), name);
        
        if(iconName != "" && !IAPManager::getInstance()->isFree(iconName,position)){
            Sprite* lock = Sprite::create("ui/lock.png");
            lock->setPosition(Vec2(cacheView->getContentSize().width/2,
                                    cacheView->getContentSize().height/2));
            cacheView->addChild(lock);

        }
    }
    return cacheView;
}