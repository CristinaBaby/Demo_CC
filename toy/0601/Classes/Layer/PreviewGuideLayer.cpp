//
//  PreviewGuideLayer.cpp
//  PaintDress
//
//  Created by maxiang on 7/7/15.
//
//

#include "PreviewGuideLayer.h"

USING_NS_CC;
#define RES_SLIDE    "ui05_slide.png"

PreviewGuideLayer::~PreviewGuideLayer()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool PreviewGuideLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0.0, 0.0, 0.0, 200))) {
        return false;
    }
    
    bool rcode = false;
    do {
        //guide
        auto slide = Sprite::create(RES_SLIDE);
        slide->setPosition(Vec2(getContentSize().width/2 + 100, getContentSize().height/2));
        addChild(slide);
        
        slide->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.7, Vec2(getContentSize().width/2 - 120, getContentSize().height/2)), MoveTo::create(0.0, Vec2(getContentSize().width/2 + 100, getContentSize().height/2)), NULL)));
        
        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(false);
        touchEventListener->onTouchBegan = [=](Touch* touch, Event* event) {
            
            this->removeFromParent();
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

