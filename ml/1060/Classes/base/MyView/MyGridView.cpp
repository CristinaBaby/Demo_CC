//
//  MyGridView.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 9/15/14.
//
//

#include "MyGridView.h"
USING_NS_KD;
void MyGridView::setCol(int col){
    this->col = col;
}
void MyGridView::setRow(int row){
    this->row = row;
}

void MyGridView::setSize(float w,float h){
    setViewSize(Size(w, h));
}

MyGridView * MyGridView::create(){
    auto scol = new MyGridView();
    if(scol&& scol->init()) {
        return scol;
    }
    delete scol;
    scol = nullptr;
    return scol;
    
}

MyGridView * MyGridView::create(float w, float h){
    auto scol = new MyGridView();
    if(scol&& scol->init()) {
        scol->setViewSize(Size(w,h));
        scol->autorelease();
        return scol;
    }
    delete scol;
    scol = nullptr;
    return scol;

}

MyGridView::MyGridView(){
    this->col = 1;
    this->row = 1;
    itemSize = Size(115,115);
    
}

void MyGridView::setAdapter(MyBaseAdapter* base){
    CC_SAFE_RETAIN(base);
    CC_SAFE_RELEASE_NULL(currAdpter);
    currAdpter = base;
    if(base == nullptr)
        return;
    base->setMyScollView(this);
    nodeCache.clear();
    float curentPositionX(0) ,curentPositionY(0),height(0);
    switch (getDirection()) {
        case Direction::VERTICAL:
            height  = marginBottom + (itemSize.height)*((base->getCount()+col-1)/col)+space*((base->getCount()+col-1)/col-1);
            curentPositionY = height;
            for(int i = 0; i < base->getCount();i++){
                curentPositionX = marginLeft+(space+itemSize.width)*(i%col);
                curentPositionY = height - (space+itemSize.height)*(i/col);
                auto btn  = base->getItem(i);
				btn->touchSpriteBegan  = CC_CALLBACK_3(MyGridView::touchDown,this);
				btn->touchSpriteEnded  = CC_CALLBACK_3(MyGridView::touchEnd,this);
                btn->setTag(i);
                btn->setAnchorPoint(Vec2(0,1));
                btn->setPosition(curentPositionX,curentPositionY);
                ScrollView::addChild(btn);
                nodeCache.pushBack(btn);
            }
            setContentSize(Size(marginLeft+itemSize.width*col + space*(col-1)+marginRight,height+marginTop));
              setContentOffset(Vec2(0,getViewSize().height-getContentSize().height));
            break;
        default:
            for(int i = 0;i<base->getCount();i++){
                curentPositionX = marginLeft + (space + itemSize.width)*(i/row);
                curentPositionY = marginBottom + (space + itemSize.height)*(row-i%row)-space;
                auto btn = base->getItem(i);
               	btn->touchSpriteBegan  = CC_CALLBACK_3(MyGridView::touchDown,this);
				btn->touchSpriteEnded  = CC_CALLBACK_3(MyGridView::touchEnd,this);
                btn->setTag(i);
                btn->setAnchorPoint(Vec2(0, 1));
                btn->setPosition(Vec2(curentPositionX, curentPositionY));
                 ScrollView::addChild(btn);
                nodeCache.pushBack(btn);
            }
            setContentSize(Size(marginLeft + (space + itemSize.width)*((base->getCount()+row-1)/row)-space+marginRight,marginTop+marginBottom+(space+itemSize.height)*row-space));
            setContentOffset(Vec2(0,0));
            break;
    }
}

void MyGridView::onTouchMoved(Touch* touch, Event* event) {
    ScrollView::onTouchMoved(touch, event);
    auto dis = touch->getLocation().distance(touch->getStartLocation());
    if(dis > discardMove)
        isMove = true;

    
}

void MyGridView::setItemSize(Size itemSize){
    this->itemSize = itemSize;
}