//
//  TapDropNode.cpp
//  BHUG1066
//
//  Created by liji on 16/6/28.
//
//

#include "TapDropNode.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"

TapDropNode::TapDropNode():
_container(nullptr),
_dropCountNeeded(1)
{
    
}

TapDropNode::~TapDropNode()
{
    
}

TapDropNode* TapDropNode::create(const std::string& file)
{
    TapDropNode *pRet = new TapDropNode();
    if (pRet && pRet->init(file))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool TapDropNode::init(const std::string& file)
{
    if (!MakeBaseNode::init())
        return false;
    
    _container = Sprite::create(file);
    
    addChild(_container, 1);
    
    _mainContent = _container;
    return true;
}

void TapDropNode::setPourWorldPoint(const Vec2& worldPoint)
{
    _originalPosition = getPosition();
    
    auto compMove = ComponentTouchMoveToTarget::create(worldPoint);
    compMove->setMaxDistanceForMoveToTarget(_moveToTargetTriggerDistance);
    addComponent(compMove);
    compMove->start();
    
    compMove->touchBeganCall = [=](Touch* touch)
    {
        if (dragBeginCall)
            dragBeginCall();
            
        this->runAction(ScaleTo::create(0.3, 1.2));
    };
    
    compMove->touchEndedCall = [=](Touch* touch)
    {
        this->runAction(ScaleTo::create(0.3, 1.0));
    };
    
    compMove->componentTouchMoveToTargetDoneCall = [=]()
    {
        auto rotate = RotateTo::create(0.35, -120);
        auto rotateDone = CallFunc::create([=]()
                                           {
                                               auto touchComp = ComponentTouchMove::create();
                                               touchComp->setIsCanMove(false);
                                               touchComp->touchClickedCall = [=](Touch* t)
                                               {
                                                   --_dropCountNeeded;
                                                   if(dropShowCall)
                                                   {
                                                       dropShowCall();
                                                   }
                                                   if(_dropCountNeeded <= 0)
                                                   {
                                                       this->removeComponent(touchComp);
                                                       this->runAction(Sequence::create(DelayTime::create(1.2),
                                                                                        RotateTo::create(0.2,0),
                                                                                        CallFunc::create([=]
                                                       {
                                                           if(dropDoneCall)
                                                           {
                                                               dropDoneCall();
                                                           }
                                                       }),
                                                                                        NULL));
                                                   }
                                               };
                                               this->addComponent(touchComp);
                                               touchComp->start();
                                               if(rotateDoneCall)
                                               {
                                                   rotateDoneCall();
                                               }
                                           });
        this->runAction(Sequence::create(rotate, rotateDone, nullptr));
    };
}
void TapDropNode::backToOriginal(const std::function<void()>& callback)
{
    this->runAction(Sequence::create(RotateTo::create(0.5, 0.0), MoveTo::create(0.3, _originalPosition), CallFunc::create([=](){
        
        if (callback)
            callback();
    }), NULL));


}
