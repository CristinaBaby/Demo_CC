//
//  ComponentTouchMove.cpp
//
//  Created by tanshoumei on 1/21/16.
//
//

#include "component/makeComponent/basicComponent/ComponentTouchMove.h"


ComponentTouchMove::ComponentTouchMove():
_bRestrictMoveArea(true),
_bCanMove(true),
_fMinDistanceForMoveStart(5),
_moveArea(Rect::ZERO),
_needTouchTarget(false)
{
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
//    _eventListenerTouchOneByOne->setSwallowTouches(true);
    _eventListenerTouchOneByOne->retain();
    setName("ComponentTouchMove");
}

ComponentTouchMove::~ComponentTouchMove()
{
    CC_SAFE_RELEASE_NULL(_eventListenerTouchOneByOne);
}

void ComponentTouchMove::onAdd()
{
    ComponentBase::onAdd();
    
    if (_moveArea.equals(Rect::ZERO))
    {
        _moveArea.origin = Director::getInstance()->getOpenGLView()->getVisibleOrigin();
        _moveArea.size = Director::getInstance()->getWinSize();
    }

    if(getOwner())
    {
        _eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(ComponentTouchMove::onTouchBegan, this);
        
        _eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(ComponentTouchMove::onTouchMoved, this);
        
        _eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(ComponentTouchMove::onTouchEnded, this);
        
        _eventListenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(ComponentTouchMove::onTouchCancelled, this);
        _eventListenerTouchOneByOne->setSwallowTouches(true);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_eventListenerTouchOneByOne, getOwner());
    }
}

void ComponentTouchMove::onRemove()
{
    if(_eventListenerTouchOneByOne)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
    }
}

void ComponentTouchMove::pause()
{
    _paused = true;
    _eventListenerTouchOneByOne->setSwallowTouches(false);
//    _eventListenerTouchOneByOne->setEnabled(false);
}

void ComponentTouchMove::resume()
{
    _paused = false;
    _eventListenerTouchOneByOne->setSwallowTouches(true);
//    _eventListenerTouchOneByOne->setEnabled(true);
}

void ComponentTouchMove::stop()
{
    if(_eventListenerTouchOneByOne)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
    }
}

bool ComponentTouchMove::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (_paused || !_enabled)
        return false;
    
    Node* parent = getOwner()->getParent();
    if(parent)
    {
        _fMoveDistance = 0;
        
        Point ptInParent = parent->convertTouchToNodeSpace(touch);
        bool isTouchIn = isTouchInRecursionChildren(getOwner(), touch);//getOwner()->getBoundingBox().containsPoint(ptInParent);
        if(isTouchIn)
        {
            if(touchBeganCall)
                touchBeganCall(touch);
        }
        return isTouchIn;
    }
    return false;
}

void ComponentTouchMove::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(_paused || !_enabled)
        return;

    _fMoveDistance += touch->getDelta().length();
    if(_fMoveDistance > _fMinDistanceForMoveStart && _bCanMove)
    {
        if(getOwner())
        {
            Node* ownerParent = getOwner()->getParent();
            //限制在屏幕范围内.先将坐标转换为世界坐标，看其是否还在限制范围内，如是，则设置新的坐标，否则，不做处理
            if(ownerParent)
            {
                Vec2 newPtWorld = getOwner()->convertToWorldSpaceAR(Vec2::ZERO) + touch->getDelta();
                
                auto target = static_cast<Node*>(event->getCurrentTarget());
                Point locationInNode = target->convertToNodeSpace(touch->getLocation());
                Size s = target->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);
                
                if(_bRestrictMoveArea)
                {
                    if(_moveArea.containsPoint(newPtWorld))
                    {
                        if (!_needTouchTarget)
                        {
                            getOwner()->setPosition(getOwner()->getPosition() + touch->getDelta());
                        }
                        else if (rect.containsPoint(locationInNode))
                        {
                            getOwner()->setPosition(getOwner()->getPosition() + touch->getDelta());
                        }
                    }
                }
                else
                {
                    if (!_needTouchTarget)
                    {
                        getOwner()->setPosition(getOwner()->getPosition() + touch->getDelta());
                    }
                    else if (rect.containsPoint(locationInNode))
                    {
                        getOwner()->setPosition(getOwner()->getPosition() + touch->getDelta());
                    }
                }
            }
        }
    }
    if(touchMovedCall)
    {
        touchMovedCall(touch);
    }
}

void ComponentTouchMove::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(_paused || !_enabled)
        return;
    
    
    if(touchEndedCall)
    {
        touchEndedCall(touch);
    }
    if(_fMoveDistance <= _fMinDistanceForMoveStart)
    {
        if(touchClickedCall)
            touchClickedCall(touch);
    }
}

void ComponentTouchMove::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

float ComponentTouchMove::getMovedDistance()
{
    return _fMoveDistance;
}

bool ComponentTouchMove::isTouchInRecursionChildren(Node* node, cocos2d::Touch * t)
{
    Node* parent = node->getParent();
    if(!parent)
        return false;
    Point ptInParent = parent->convertTouchToNodeSpace(t);
    bool isTouchIn = node->getBoundingBox().containsPoint(ptInParent);
    if(isTouchIn)
    {
        return true;
    }
    for(auto child: node->getChildren())
    {
        if(isTouchInRecursionChildren(child, t))
            return true;
    }
    return false;
}

void ComponentTouchMove::setBackToOriginal(bool isBack, Vec2 oriPos)
{
    _isBack = isBack;
    _originalPos = oriPos;
}


void ComponentTouchMove::back(std::function<void()> backFunc)
{
    if (_isBack)
    {
        _owner->runAction(Sequence::create(MoveTo::create(0.3, _originalPos),
                                           CallFunc::create([=]
                                                            {
                                                                if (backFunc)
                                                                {
                                                                    backFunc();
                                                                }
                                                            }),
                                           NULL));
    }
}

