//
//  RateUsNode.cpp
//  CCATS022
//
//  Created by liji on 16/11/21.
//
//

#include "RateUsNode.h"
#include "SSCInternalLibManager.h"
RateUsNode::RateUsNode()
{

}

RateUsNode::~RateUsNode()
{

}

bool RateUsNode::init()
{
    if (!ESMakeBaseScene::initWithCSBFile("ratus.csb"))
    {
        return false;
    }
    
    _uiAction->gotoFrameAndPause(0);
    
    auto layer = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(layer,-1);
    layer->setContentSize(Size(9999,9999));
    layer->setPosition(-200, -200);
    

    auto okBtn = (Button*)ui::Helper::seekNodeByName(_rootNode, "Button_Rate");
    auto cancelBtn = (Button*)ui::Helper::seekNodeByName(_rootNode, "Button_Cancel");
    
    auto call = [=](Ref* ref, ui::Widget::TouchEventType t)
    {
        auto btn = dynamic_cast<Button*>(ref);
        if (t == Widget::TouchEventType::ENDED)
        {
            if (btn->getName() == "Button_Rate")
            {
                SSCInternalLibManager::getInstance()->rateUs();
                this->runAction(RemoveSelf::create());
            }
            if (btn->getName() == "Button_Cancel")
            {
                _director->getEventDispatcher()->pauseEventListenersForTarget(this);
                
                getActionTimeLine()->play("disappear", false);
                getActionTimeLine()->setLastFrameCallFunc([=]
                {
                    this->runAction(RemoveSelf::create());
                });
            }
        }
    };
    okBtn->addTouchEventListener(call);
    cancelBtn->addTouchEventListener(call);

    
    return true;
}

void RateUsNode::onEnterTransitionDidFinish()
{
    ESMakeBaseScene::onEnterTransitionDidFinish();
    
    getActionTimeLine()->play("show", false);
}
