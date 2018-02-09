//
//  MyBaseAdapter.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 9/9/14.
//
//

#include "MyBaseAdapter.h"
USING_NS_KD;
MyBaseAdapter::MyBaseAdapter() {
    itemClick = nullptr;
    scol = nullptr;
}

void MyBaseAdapter::setMyScollView(MyScollView* scol){
    this->scol = scol;
}

std::function<void(EventSprite*,int,MyScollView*)>  MyBaseAdapter::getItemClick(){
    return itemClick;
}

void MyBaseAdapter::setItemClick( std::function<void(EventSprite*,int,MyScollView*)> click){
    itemClick = click;
}

int MyBaseAdapter::getCount(){
    return file_path.size();
}

EventSprite * MyBaseAdapter::getItem(int position){
	auto item = EventSprite::create( file_path.at(position));
    return  item;
}
 std::vector<std::string> MyBaseAdapter::getFilePath(){
    return this->file_path;
}

