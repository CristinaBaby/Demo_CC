//
//  ViewAdapter.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 9/9/14.
//
//

#include "ViewAdapter.h"
ViewAdapter::ViewAdapter() {
    itemClick = nullptr;
    _recycleTexture = false;
    _itemSize = Size::ZERO;
}

ClcikFunc  ViewAdapter::getItemClick(){
    return itemClick;
}

void ViewAdapter::setItemClick(ClcikFunc click){
    itemClick = click;
}

size_t ViewAdapter::getCount(){
    return file_path.size();
}

Node * ViewAdapter::getItem(int position){
    Sprite* _item = nullptr;
    //log("%d == ",position);
    //log("path == %s",file_path.at(position).c_str());
    if(file_path.size()>0&&position<file_path.size()){
        _item = Sprite::create( file_path.at(position));
        _item->setPosition(_item->getContentSize()*.5);
    }
    
    return  _item;
}

const  std::vector<std::string>& ViewAdapter::getFilePath(){
    return this->file_path;
}

void ViewAdapter::recycle(Node* cell,int index){
    if(_recycleTexture&& cell!=nullptr&& file_path.size()>0&&index <file_path.size())
        Director::getInstance()->getTextureCache()->removeTextureForKey(file_path.at(index));
}

void ViewAdapter::setItemSize(const Size& _size){
    _itemSize = _size;
}
const Size& ViewAdapter::getItemSize(int index){
    if(_itemSize.width != Size::ZERO.width||_itemSize.height != Size::ZERO.height)
        return _itemSize;
    else {
        auto _item = getItem(index);
        if(_item != nullptr)
            return _item->getContentSize();
        else
            return Size::ZERO;
    }
}

