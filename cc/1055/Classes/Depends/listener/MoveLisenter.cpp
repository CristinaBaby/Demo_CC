//
//  MoveLisenter.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/29.
//
//

#include "MoveLisenter.h"
MoveLisenter::MoveLisenter()
{
    _oldPos = Vec2::ZERO;
    _zOrder = 0;
    m_bIsMoveBack = true;
    m_bIsMove = true;
}
MoveLisenter::~MoveLisenter()
{


}
MoveLisenter* MoveLisenter::create()
{
    auto ret = new (std::nothrow) MoveLisenter();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}
bool MoveLisenter::init()
{
    if(!EventListenerTouchOneByOne::init())
        return false;
    
    
    this->onTouchBegan = CC_CALLBACK_2(MoveLisenter::touchBean, this);
    this->onTouchMoved = CC_CALLBACK_2(MoveLisenter::touchMove, this);
    this->onTouchEnded = CC_CALLBACK_2(MoveLisenter::touchEnded, this);
    this->setSwallowTouches(true);
    
    m_vMoveRect = Rect(Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleOrigin().y, Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
    m_oOrginOffset = Vec2::ZERO;
   
    
    return true;
}
#pragma mark touch响应事件
bool MoveLisenter::touchBean(Touch* touch, Event* event)
{
    Vec2 touchPos = touch->getLocation();
    Vec2 realPos = _node->getParent()->convertToNodeSpace(touchPos);
    
    if (_node->boundingBox().containsPoint(realPos) && _node->isVisible())
    {
        _oldPos = _node->getPosition();
        _zOrder = _node->getLocalZOrder();
        
        if(onTouchDown){
        
            onTouchDown(_node,this);
        
        }
        _node->getParent()->reorderChild(_node,1000);
        return true;
    }
    return false;
}
void MoveLisenter::touchMove(Touch* touch, Event* event)
{
    
    if(onTouchMove)
        onTouchMove(_node,this,touch);
    
    if (!m_bIsMove) {
        return;
    }
    
    Vec2 deltaPos = touch->getDelta();
    Vec2 vec = _node->getPosition();
    Vec2 newPos = _node->getPosition()+m_oOrginOffset+deltaPos;
    
    if (m_vMoveRect.containsPoint(newPos)) {
         _node->setPosition(_node->getPosition() + deltaPos);
        
        int index = this->isInRect(newPos);
        if(index != -1)
        {
            if (onTouchMoveInRect){
                onTouchMoveInRect(_node,this,index);
            }
            
            if (onTouchMix){
                onTouchMix(_node,this,touch);
            }
            
        }else{
            if(onTouchMoveOutRect){
                onTouchMoveOutRect(_node,this);
            }
        }
    }else {
        if(newPos.x>m_vMoveRect.getMaxX()){
            _node->setPositionX(m_vMoveRect.getMaxX()-m_oOrginOffset.x);
        }
        else if(newPos.x<m_vMoveRect.getMinX())
            {
                _node->setPositionX(m_vMoveRect.getMinX()-m_oOrginOffset.x);
            }
        
        if(newPos.y>m_vMoveRect.getMaxY())
        {
            _node->setPositionY(m_vMoveRect.getMaxY()-m_oOrginOffset.y);
        }else if(newPos.y<m_vMoveRect.getMinY())
        {
                _node->setPositionY(m_vMoveRect.getMinY()-m_oOrginOffset.y);
        }
    }
}
void MoveLisenter::touchEnded(Touch* touch, Event* event)
{
    this->setEnabled(false);
    
    if(!m_bIsMoveBack){
        this->setEnabled(true);
        
        if(onTouchUp){
            
            onTouchUp(_node,this);
        }
        
    
    }else{
       
        _node->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(1.0,_oldPos), 1.0),CallFunc::create([=](){
            _node->getParent()->reorderChild(_node,_zOrder);
            this->setEnabled(true);
            
            if(onTouchUp){
                
                onTouchUp(_node,this);
            }
        }), NULL));
        
    
    }
}
void MoveLisenter::touchCancelled(Touch* touch, Event* event)
{
    
}
#pragma mark 矩形响应区域
void MoveLisenter::addRect(const Rect& rect,Node* root)
{
    if (root) {
        LayerColor* test = LayerColor::create(Color4B(255, 0, 0, 100));
        test->setAnchorPoint(Vec2(0, 0));
        test->setContentSize(Size(rect.getMaxX() - rect.getMinX(), rect.getMaxY() - rect.getMinY()));
        test->setPosition(Vec2(rect.getMinX(), rect.getMinY()));
        root->addChild(test,100);
    }

    m_vRects.push_back(rect);
}
void MoveLisenter::addRectByNode(Node* node, Node* root)
{
    if (!node->getParent())
    {
        log("this node has not been added to parent,rect maybe incorrect!");
    }
    Rect rect = RectApplyAffineTransform(Rect(0, 0, node->getContentSize().width, node->getContentSize().height),node->nodeToWorldTransform());
//    log("====================>minX:%f  minY:%f  maxX:%f    maxY:%f", rect.getMinX(), rect.getMinY(), rect.getMaxX(), rect.getMaxY());
    this->addRect(rect,root);
}
void MoveLisenter::clearRectVector()
{
    m_vRects.clear();
}
int MoveLisenter::isInRect(const Vec2& pos)
{
    for (int i = 0; i < m_vRects.size(); i++)
    {
        if (m_vRects[i].containsPoint(pos))
        {
            return i;
        }
    }
    return -1;
}
void MoveLisenter::setOrginOffset()
{
    Node* parent = _node->getParent();
    if (parent)
    {
        Rect rect = RectApplyAffineTransform(Rect(0, 0, parent->getContentSize().width, parent->getContentSize().height),parent->nodeToWorldTransform());
        m_oOrginOffset = Vec2(rect.getMinX(),rect.getMinY());
    }
}