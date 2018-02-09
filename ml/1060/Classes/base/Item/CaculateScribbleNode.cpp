//
//  CaculateScribbleNode.cpp
//  HappyParty
//
//  Created by luotianqiang1 on 15/9/28.
//
//

#include "CaculateScribbleNode.h"

bool  CaculateScribbleNode::onTouchBegan(Touch *pTouch, Event *pEvent){
    //    auto _pos= this->convertToNodeSpace(pTouch->getLocation());
    //    Rect _rect;
    //    _rect.size = getContentSize();
    //    if(_rect.containsPoint(_pos))
    //        return true;
    //    else
    //        return false;
    return true;
}
void  CaculateScribbleNode::onTouchMoved(Touch *pTouch, Event *pEvent) {
    if(_moveCallback != nullptr)
        _moveCallback(this,pTouch->getLocation());
    auto _prePos= this->convertToNodeSpace(pTouch->getPreviousLocation());
    Rect _rect;
    _rect.size = getContentSize();
    if(_rect.containsPoint(_prePos)){
        auto _current = this->convertToNodeSpace(pTouch->getLocation());
        _drawNode->drawSegment(_prePos, _current, _drawRadius, _drawColor);
    }
}
void  CaculateScribbleNode::paint(Vec2 wordPos,Vec2 wordPos2){
    auto _prePos= this->convertToNodeSpace(wordPos);
    auto _current = this->convertToNodeSpace(wordPos2);
    _drawNode->drawSegment(_prePos, _current, _drawRadius, _drawColor);
    
}

void  CaculateScribbleNode::onTouchEnded(Touch *pTouch, Event *pEvent) {
    
}
void  CaculateScribbleNode::onTouchCancle(Touch *pTouch, Event *pEvent){
    
}

void CaculateScribbleNode::clear(){
    if(_drawNode != nullptr)
        _drawNode->clear();
}

bool CaculateScribbleNode::init(){
    if(Node::init()){
        setContentSize(Director::getInstance()->getWinSize());
        return true;
    }
    return false;
}

void   CaculateScribbleNode::onExitTransitionDidStart(){
    if(nullptr != _lis)
        _eventDispatcher->removeEventListener(_lis);
    Node::onExitTransitionDidStart();
}

void CaculateScribbleNode::onEnterTransitionDidFinish(){
    Node::onEnterTransitionDidFinish();
    if (_drawNode == nullptr) {
        _drawNode = DrawNode::create();
        addChild(_drawNode);
        _drawNode->clear();
        
        //        GLProgram *p = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR);
        //        _drawNode->setGLProgram(p);
    }
    if(_lis == nullptr){
        _lis = EventListenerTouchOneByOne::create();
        _lis->setSwallowTouches(false);
        _lis->setEnabled(isEnbale);
        _lis->onTouchBegan = CC_CALLBACK_2(CaculateScribbleNode::onTouchBegan, this);
        _lis->onTouchMoved = CC_CALLBACK_2(CaculateScribbleNode::onTouchMoved, this);
        _lis->onTouchEnded = CC_CALLBACK_2(CaculateScribbleNode::onTouchEnded, this);
        _lis->onTouchCancelled = CC_CALLBACK_2(CaculateScribbleNode::onTouchCancle, this);
        _lis->retain();
    }
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_lis, this);
}

void CaculateScribbleNode::setEnable(bool flag){
    isEnbale = flag;
    if(nullptr != _lis)
        _lis->setEnabled(flag);
}

CaculateScribbleNode::~CaculateScribbleNode(){
    CC_SAFE_RELEASE_NULL(_lis);
}

CaculateScribbleNode::CaculateScribbleNode(){
    _drawColor = Color4F::WHITE;
    _drawRadius= 10;
    isEnbale = true;
    _drawNode = nullptr;
    _lis = nullptr;
    _moveCallback = nullptr;
}