//
//  ChangeBtnAdapter.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 11/19/14.
//
//

#include "ChangeBtnAdapter.h"
#include "MyScollView.h"
USING_NS_KD;
ChangeBtnAdapter::ChangeBtnAdapter() {
    currenIndex = -1;
    itemClick = CC_CALLBACK_3(ChangeBtnAdapter::chaneBtnClick,this);
	changeBtnitemClick = nullptr;
    upClick = nullptr;
}

EventSprite * ChangeBtnAdapter::getItem(size_t position){
    auto btn = EventSprite::create(file_path.at(position));
    return btn;
}

void ChangeBtnAdapter::chaneBtnClick (EventSprite* btn,size_t index,MyScollView* sol){
    if(currenIndex != -1){
        auto btn = sol->nodeCache.at(currenIndex);
        if(btn)
            btn->setSpriteFrame(Sprite::create(file_path.at(currenIndex))->getSpriteFrame());
    }
    if(currenIndex == index) {
        currenIndex = -1;
        if(upClick != nullptr) {
            upClick(btn,index,sol);
            return;
        }
    }else {
        currenIndex = index;
        auto btn = sol->nodeCache.at(currenIndex);
        if(btn)
            btn->setSpriteFrame(Sprite::create(clickPath.at(currenIndex))->getSpriteFrame());
    }
	if(nullptr != changeBtnitemClick)
		changeBtnitemClick(btn,index,sol);
}
 void ChangeBtnAdapter::setUpItemClick(std::function<void(EventSprite*,size_t,MyScollView*)> upClick) {
     this->upClick = upClick;
}

void ChangeBtnAdapter::click(){
    if (currenIndex != -1&& scol){
        auto btn = scol->nodeCache.at(currenIndex);
        if(btn)
            btn->setSpriteFrame(Sprite::create(file_path.at(currenIndex))->getSpriteFrame());
    
    }
    currenIndex = -1;
}

void ChangeBtnAdapter::setItemClick(std::function<void(EventSprite*,size_t,MyScollView*)> click){
    changeBtnitemClick = click;
}
