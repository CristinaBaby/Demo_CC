//
//  HintView.cpp
//  Make
//
//  Created by liji on 15/6/5.
//
//

#include "HintView.h"
#include "ActionController.h"
#include "Config.h"
bool HintView::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto layerColor = LayerColor::create(Color4B(0, 0, 0, 155));
    layerColor -> setContentSize(Size(9999, 9999));
    layerColor -> setPosition(-480, -320);
    this -> addChild(layerColor);
    
    lis_ = EventListenerTouchOneByOne::create();
    lis_ -> onTouchBegan = [&](Touch* touch, Event* event)
    {
        
        return true;
    };
    
    lis_ -> onTouchEnded = [&](Touch* touch, Event* event)
    {
        if (this -> getTag() == kTagReady)
        {
            this -> setTag(kTagNone);
            if (delegate_)
            {
                delegate_ -> hintViewShowOver();
            }
        }
    };

    lis_ -> setSwallowTouches(true);
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(lis_ -> clone(), this);
    
    
    auto hint = Sprite::create("minigame/point out.png");
    hint -> setPosition(320, 480);
    hint -> setScale(0);
    this -> addChild(hint);
    
    ActionController::popUp(0.2, hint);
    
    
    return true;
}


void HintView::allowTouchHint(float pDelayTime)
{
    this -> runAction(Sequence::create(
                                       DelayTime::create(pDelayTime),
                                       CallFunc::create([&]
    {
    
        this -> setTag(kTagReady);
    })
                                       , NULL));

}
