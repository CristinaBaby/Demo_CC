//
//  CartView.cpp
//  Make
//
//  Created by liji on 15/6/2.
//
//

#include "CartView.h"

Point p[5] = {Vec2(-26, 85),Vec2(83, 26),Vec2(-132, 50),Vec2(-175, 45),Vec2(-14, 20),};


bool CartView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto bottom = Sprite::create("minigame/cart.png");
    bottom -> setScale(0.8);
    this -> addChild(bottom,1);
    
    auto cover = Sprite::create("minigame/cart1.png");
    cover -> setScale(0.8);
    this -> addChild(cover,3);
    
    cartCollisionRect_ = Rect(-150, 0, 220, 10);

//直观的显示碰撞区域。。。
//    auto testRect = LayerColor::create(Color4B(255, 255, 255, 255));
//    testRect -> setPosition(cartCollisionRect_.origin);
//    testRect -> setContentSize(cartCollisionRect_.size);
//    this -> addChild(testRect,2);
    
    itemNode_ = Node::create();
    this -> addChild(itemNode_, 2);
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener -> onTouchBegan = [&](Touch* touch, Event* event)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode))
        {
            
            return true;
        }
        return false;

    };
    
    listener -> onTouchMoved = [&](Touch* touch, Event* event)
    {
        float x = this -> getPositionX();
        Size s = Director::getInstance() -> getVisibleSize();
        if (x > s.width)
        {
            x = s.width ;
        }
        
        if (x < 0)
        {
            x = 0 ;
        }

        
        this -> setPositionX(x + touch -> getDelta().x);
    };
    
    listener -> onTouchEnded = [&](Touch* touch, Event* event)
    {
        
    };

    _eventDispatcher-> addEventListenerWithSceneGraphPriority(listener -> clone(), bottom);
    
    return true;
}


void CartView::addItemToCart(int itemIdx)
{
    for(auto node : itemNode_ -> getChildren())
    {
        if (node -> getTag() == itemIdx)
        {
            return;
        }
    }
    
    auto itemStr = __String::createWithFormat("minigame/%d_fall.png",itemIdx);
    Sprite* item = Sprite::create(itemStr -> getCString());
    item -> setPosition(p[itemIdx -1]);
    itemNode_ -> addChild(item,itemIdx,itemIdx);
    
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/place_on_cart.mp3");
    


}