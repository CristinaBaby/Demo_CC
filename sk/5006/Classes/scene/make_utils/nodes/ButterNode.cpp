//
//  ButterNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#include "ButterNode.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"
#include "component/actions/ActionHelper.h"

ButterNode::ButterNode()
{
    
}

ButterNode::~ButterNode()
{
    
}

ButterNode* ButterNode::create(const std::string& file)
{
    ButterNode *pRet = new ButterNode();
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

bool ButterNode::init(const std::string& file)
{
    if (!MakeBaseNode::init())
        return false;
    
    _butter = Sprite::create(file);
    addChild(_butter, 1);
    
    _mainContent = _butter;
    return true;
}


ButterNode* ButterNode::createPicChange(const std::string& fileFormat, Vec2 range)
{

    ButterNode *pRet = new ButterNode();
    if (pRet && pRet->initPicChange(fileFormat,range))
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

bool ButterNode::initPicChange(const std::string& fileFormat, Vec2 range)
{
    if (!MakeBaseNode::init())
        return false;
    
    _butter = Sprite::create(StringUtils::format(fileFormat.c_str(),(int)range.x));
    addChild(_butter, 1);
    
    _formatRange = range;
    _formatStr = fileFormat;
    
    _mainContent = _butter;
    return true;

}


void ButterNode::setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback)
{
    _originalPosition = getPosition();
    
    auto compMove = ComponentTouchMoveToTarget::create(worldPoint);
    compMove->setMaxDistanceForMoveToTarget(_moveToTargetTriggerDistance);
    addComponent(compMove);
    compMove->start();
    
    compMove->touchBeganCall = [=](Touch* touch)
    {
        this->runAction(ScaleTo::create(0.3, 1.2));
    };
    
    compMove->touchEndedCall = [=](Touch* touch)
    {
        this->runAction(ScaleTo::create(0.3, 1.0));
    };
    
    compMove->componentTouchMoveToTargetDoneCall = [=]()
    {
        if (onMoveToTargetCallback)
            onMoveToTargetCallback();
        
        setClickCall(showCallback);
    };
}

void ButterNode::setClickCall(const std::function<void()>& showCallback)
{
    auto touchComp = ComponentTouchMove::create();
    touchComp->setIsCanMove(false);
    _butter->addComponent(touchComp);
    touchComp->start();
    
    touchComp->touchClickedCall = [=](Touch* touch)
    {
        _butter->runAction(ActionHelper::createShakeAction());
        if (_formatRange != Vec2::ZERO)
        {
            _formatRange = Vec2(++_formatRange.x,_formatRange.y);
            if (_formatRange.x>_formatRange.y)
            {
                _butter->setVisible(false);
                touchComp->setEnabled(false);
            }
            else
            {
                log("======%s==========",_formatStr.c_str());
                log("======%s,%d==========",StringUtils::format(_formatStr.c_str(),_formatRange.x).c_str(),(int)_formatRange.x);
                
                _butter->setTexture(StringUtils::format(_formatStr.c_str(),(int)_formatRange.x));
            }
        }
        else
        {
            _butter->setScale(_butter->getScale() - 0.15);
        }
        if (showCallback)
            showCallback();
    };
}



