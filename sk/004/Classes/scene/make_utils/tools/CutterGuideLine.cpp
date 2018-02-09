//
//  CutterGuideLine.cpp
//  BHUG1062
//
//  Created by maxiang on 5/16/16.
//
//

#include "CutterGuideLine.h"

CutterGuideLine::~CutterGuideLine()
{
    
}

bool CutterGuideLine::init()
{
    if (!Node::init())
        return false;
    
    auto line = Sprite::create("res/Sandwich/6/tips_cut0.png");
    _progressTimer = ProgressTimer::create(line);
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setMidpoint(Vec2(0.0, 0.0));
    _progressTimer->setBarChangeRate(Vec2(0.0, 1.0));
    _progressTimer->setPercentage(100);
    addChild(_progressTimer);
    
    _indicator = Sprite::create("res/Sandwich/6/tips_cut1.png");
    _indicator->setPosition(Vec2(0.0, line->getContentSize().height/2 + _indicator->getContentSize().height/2));
    addChild(_indicator);
    
    return true;
}

void CutterGuideLine::start()
{
    _progressTimer->runAction(RepeatForever::create(Sequence::create(ProgressTo::create(1.0, 0), ProgressTo::create(0.0, 100), NULL)));
    _indicator->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.0, Vec2(0.0, -_progressTimer->getSprite()->getContentSize().height)), CallFunc::create([=](){
        
        _indicator->setPosition(Vec2(0.0, _progressTimer->getSprite()->getContentSize().height/2 + _indicator->getContentSize().height/2));
    }), NULL)));
}

void CutterGuideLine::stop()
{
    getActionManager()->removeAllActionsFromTarget(_progressTimer);
    getActionManager()->removeAllActionsFromTarget(_indicator);
}



