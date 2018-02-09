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

ItemClick  MyBaseAdapter::getItemClick(){
    return itemClick;
}

void MyBaseAdapter::setItemClick(ItemClick click){
    itemClick = click;
}

size_t MyBaseAdapter::getCount(){
    return file_path.size();
}

EventSprite * MyBaseAdapter::getItem(size_t position){
	auto item = EventSprite::create( file_path.at(position));
    return  item;
}
 std::vector<std::string> MyBaseAdapter::getFilePath(){
    return this->file_path;
}

