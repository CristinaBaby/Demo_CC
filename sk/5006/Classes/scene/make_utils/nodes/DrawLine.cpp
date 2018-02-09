//
//  DrawLine.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/12/30.
//
//

#include "DrawLine.h"

DrawLine::DrawLine():touchIndex(-1)
,_acctionNode(nullptr)
,_spwanAction(nullptr){
    
}

DrawLine::~DrawLine(){
    CC_SAFE_RELEASE_NULL(_acctionNode);
    CC_SAFE_RELEASE_NULL(_spwanAction);
}

bool DrawLine::init(){
    if(Layer::init()){
        _drawNode = DrawNode::create();
        addChild(_drawNode);
        
   
        
        _add = DrawNode::create();
        _add->drawDot(Vec2::ZERO, 20, Color4F::BLUE);
        addChild(_add);
        _add->setPosition(Vec2(getContentSize())- Vec2(80,80));
        return true;
    }
    return false;
}

bool DrawLine::touchbegain(Touch* pTouch, Event*){
    auto _current = this->convertToNodeSpace(pTouch->getLocation());
    if(_add->getPosition().distanceSquared(_current)<1600){
        if(points.size()==0)
            points.push_back(Vec2(100,100));
        points.push_back(Vec2(150,200));
        points.push_back(Vec2(150,100));
        points.push_back(Vec2(200,100));
        updateDrawNode();
    }
    
    touchIndex = -1;

    for(int i=0;i<points.size();i++){
        if(points.at(i).distanceSquared(_current)<1600){
            touchIndex =i;
            break;
        }
    }
    if(touchIndex != -1)
        return true;
    else
        return false;
}
void DrawLine::touchMove(Touch* pTouch, Event*) {
    if(touchIndex != -1){
        auto _current = this->convertToNodeSpace(pTouch->getLocation());
        swap(points[touchIndex], _current);
        updateDrawNode();
    }
}
 void DrawLine::touchEnd(Touch*, Event*){
     if(touchIndex != -1){
            Vector<FiniteTimeAction*> actionList;
         _acctionNode->stopAllActions();
         _acctionNode->setPosition(points.at(0));
         for(int i=1;i<points.size();i=i+3){
             ccBezierConfig _config;
             _config.endPosition = points.at(i+2);
             _config.controlPoint_1 = points.at(i);
             _config.controlPoint_2 = points.at(i+1);
             actionList.pushBack(BezierTo::create(2, _config));
         }
         _acctionNode->runAction(Sequence::create(actionList));
         if(_spwanAction != nullptr)
             _acctionNode->runAction(_spwanAction);
         
         updateDrawNode();
     }
     touchIndex = -1;
}

void DrawLine::setPos(const vector<Vec2>& points){
    this->points = points;
    updateDrawNode();
}

void DrawLine::onEnter(){
    Layer::onEnter();
    if(_acctionNode == nullptr){
       auto _acctionNode = DrawNode::create();
        _acctionNode->drawDot(Vec2::ZERO, 20, Color4F::YELLOW);
        addChild(_acctionNode);
        setAcctionNode(_acctionNode);
    }
    auto _eventLis = EventListenerTouchOneByOne::create();
    _eventLis->setSwallowTouches(true);
    _eventLis->setEnabled(true);
    _eventLis->onTouchBegan = CC_CALLBACK_2(DrawLine::touchbegain, this);
    _eventLis->onTouchMoved = CC_CALLBACK_2(DrawLine::touchMove, this);
    _eventLis->onTouchEnded = CC_CALLBACK_2(DrawLine::touchEnd, this);
    _eventLis->onTouchCancelled = CC_CALLBACK_2(DrawLine::touchEnd, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_eventLis, -1);

}

void DrawLine::updateDrawNode(){
    _drawNode->clear();
    for(auto pos :points)
        CCLOG("Vec2(%f,%f),",pos.x,pos.y);
    for(auto pos:points)
        _drawNode->drawPoint(pos, 40, Color4F::RED);
     for(int i=1;i<points.size();i=i+3)
         _drawNode->drawCubicBezier(points.at(i-1), points.at(i), points.at(i+1), points.at(i+2), 200, Color4F::WHITE);
}