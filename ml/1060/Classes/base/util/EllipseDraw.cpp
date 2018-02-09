//
//  EllipseDraw.cpp
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/17.
//
//

#include "EllipseDraw.h"

EllipseDraw* EllipseDraw::create(EllipseConfig _config){
    auto _node = new EllipseDraw();
    if(_node && _node->init(_config)){
        _node->autorelease();
        return _node;
    }
    CC_SAFE_RELEASE_NULL(_node);
    return nullptr;
}

bool EllipseDraw::init(EllipseConfig _config){
    if(Node::init()){
        this->_config = _config;
        _drawNode = DrawNode::create();
        addChild(_drawNode);
        _draw(_config);
        
        _event_type = Ellipse_event_type::type_null;
        
        initTool();
        return true;
    }
    return false;
}

void EllipseDraw::initTool(){
    _tool = Node::create();
    _tool->setContentSize(Size(500,100));
    _tool->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _tool->setPosition(Vec2::ZERO);
    addChild(_tool);
    
    _labeCenter = Label::createWithSystemFont(StringUtils::format("center:(%5.1f,%5.1f)",_config._center.x,_config._center.y), "AppleGothic", 25);
    _tool->addChild(_labeCenter);
    _labeCenter->setAnchorPoint(Vec2::ZERO);
    _labeCenter->setPosition(Vec2(0,40));
    _labeCenter->setTextColor(Color4B::BLACK);
    
    _labeA = Label::createWithSystemFont(StringUtils::format("a:%5.1f",_config._a), "AppleGothic", 25);
    _tool->addChild(_labeA);
    _labeA->setAnchorPoint(Vec2::ZERO);
     _labeA->setTextColor(Color4B::BLACK);
    
    _labeB = Label::createWithSystemFont(StringUtils::format("b:%5.1f",_config._b), "AppleGothic", 25);
    _tool->addChild(_labeB);
    _labeB->setAnchorPoint(Vec2::ZERO);
    _labeB->setPosition(Vec2(0,-40));
     _labeB->setTextColor(Color4B::BLACK);
    
    labeTouchPonit = Label::createWithSystemFont(StringUtils::format("touchPoint:(%5.1f,%5.1f",0.0,0.0), "AppleGothic", 25);
    labeTouchPonit->setPosition(Vec2(250,0));
    _tool->addChild(labeTouchPonit);
    labeTouchPonit->setTextColor(Color4B::BLACK);
  }
void EllipseDraw::upDateTool(){
    _labeCenter->setString(StringUtils::format("center:(%5.1f,%5.1f)",_config._center.x,_config._center.y));
     _labeA->setString(StringUtils::format("a:%5.1f",_config._a));
    _labeB->setString(StringUtils::format("b:%5.1f",_config._b));
 
}

void EllipseDraw::_drawPoint(Vec2 _vec,Color4F _color ){
     _drawNode->drawDot(_vec, 5, _color);
}

void EllipseDraw::_draw(EllipseConfig _config){
    _drawNode->clear();
    _drawNode->drawDot(_config._center, 10, Color4F::BLACK);
    
    Vec2 _prePos;
    for(float i=0;i<3.14*2+0.1;i=i+0.1){
        float x = _config._a*cos(i)+_config._center.x;
        float y = _config._b*sin(i)+_config._center.y;
        if(i!=0)
            _drawNode->drawSegment(_prePos, Vec2(x,y), 3, Color4F::RED);
        _prePos = Vec2(x,y);
    }
}

void EllipseDraw::onEnter() {
    Node::onEnter();
    auto  touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(EllipseDraw::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(EllipseDraw::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(EllipseDraw::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(EllipseDraw::onTouchCancle, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    touchListener->setSwallowTouches(true);
}

bool EllipseDraw::isContain(Vec2 _center,Vec2 _pos){
    return (_center.x-5)<_pos.x&&(_center.x+5)>_pos.x&&(_center.y-5)<_pos.y&&(_center.y+5)>_pos.y;
}

bool EllipseDraw::onTouchBegan(Touch *pTouch, Event *pEvent) {
    auto _touchPos = convertToNodeSpace(pTouch->getLocation());
    if (isContain(_config._center, _touchPos)) {
        _event_type = Ellipse_event_type::type_moveCenter;
        return true;
    } else {
       auto _touchPos = _tool->convertToNodeSpace(pTouch->getLocation());
        if(_labeA->getBoundingBox().containsPoint(_touchPos)){
            _event_type = Ellipse_event_type::type_moveA;
            return true;
        } else if(_labeB->getBoundingBox().containsPoint(_touchPos)) {
            _event_type = Ellipse_event_type::type_moveB;
            return true;
        } else if(Rect(0,0,_tool->getContentSize().width,_tool->getContentSize().height).containsPoint(_touchPos)) {
            _event_type = Ellipse_event_type::type_moveTool;
            return true;
        }
    }
    
    labeTouchPonit->setString(StringUtils::format("touchPoint:(%5.1f,%5.1f",_touchPos.x,_touchPos.y));
    return false;
}
void EllipseDraw::onTouchMoved(Touch *pTouch, Event *pEvent) {
    switch (_event_type) {
        case Ellipse_event_type::type_moveCenter:
            _config._center = _config._center +pTouch->getDelta();
            break;
        case Ellipse_event_type::type_moveA:
            _config._a = _config._a + pTouch->getDelta().x;
            break;
        case Ellipse_event_type::type_moveB:
             _config._b = _config._b + pTouch->getDelta().y;
            break;
        case Ellipse_event_type::type_moveTool:
            _tool->setPosition(_tool->getPosition()+pTouch->getDelta());
            break;
        default:
            break;
    }
    
    _draw(_config);
    upDateTool();
}
void EllipseDraw::onTouchEnded(Touch *pTouch, Event *pEvent) {
    switch (_event_type) {
        case Ellipse_event_type::type_moveCenter:
            _config._center = _config._center +pTouch->getDelta();
            break;
        case Ellipse_event_type::type_moveA:
            _config._a = _config._a + pTouch->getDelta().x;
            break;
        case Ellipse_event_type::type_moveB:
            _config._b = _config._b + pTouch->getDelta().y;
            break;
        case Ellipse_event_type::type_moveTool:
            _tool->setPosition(_tool->getPosition()+pTouch->getDelta());
            break;
        default:
            break;
    }

    _draw(_config);
    upDateTool();
}
void EllipseDraw::onTouchCancle(Touch *pTouch, Event *pEvent) {
    
}
