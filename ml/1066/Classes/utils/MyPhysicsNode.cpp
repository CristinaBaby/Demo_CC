//
//  MyPhysicsNode.cpp
//  MyTest
//
//  Created by huxixiang on 16/5/4.
//
//

#include "MyPhysicsNode.hpp"
#define DRAG_BODYS_TAG 10

MyPhysicsNode::MyPhysicsNode():m_pNode(nullptr)
{

}

MyPhysicsNode::~MyPhysicsNode()
{

}

MyPhysicsNode* MyPhysicsNode::create(const std::string& filename)
{
    auto pRet = new MyPhysicsNode();
    if (pRet && pRet->init(filename)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool MyPhysicsNode::init(const std::string& filename)
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!Node::init());
        CC_BREAK_IF(!initData());
        CC_BREAK_IF(!initLayer(filename));
        isInit = true;
    }while(0);
    
    return isInit;
}

bool MyPhysicsNode::initData()
{
    _mouses.clear();
    m_oTouchPoint = Vec2::ZERO;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(MyPhysicsNode::TouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyPhysicsNode::TouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyPhysicsNode::TouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MyPhysicsNode::TouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

bool MyPhysicsNode::initLayer(const std::string& filename)
{
    m_pNode = Sprite::create(filename);
    m_pNode->setPhysicsBody(PhysicsBody::createBox(m_pNode->getContentSize() - Size(100, 100)));
    m_pNode->getPhysicsBody()->setTag(DRAG_BODYS_TAG);
    m_pNode->getPhysicsBody()->setAngularDamping(1.0f);
//    m_pNode->setPosition(this->getContentSize() / 2);
    addChild(m_pNode);
    
    return true;
}

bool MyPhysicsNode::TouchBegan(Touch *touch, Event *pEvent)
{
    auto location = touch->getLocation();
    
    auto arr = this->getScene()->getPhysicsWorld()->getShapes(location);
    
    PhysicsBody* body = nullptr;
    for (auto& obj : arr)
    {
        if ((obj->getBody()->getTag() & DRAG_BODYS_TAG) != 0)
        {
            body = obj->getBody();
            break;
        }
    }
    
    if (body != nullptr)
    {
        Node* mouse = Node::create();
        mouse->setPhysicsBody(PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY));
        mouse->getPhysicsBody()->setDynamic(false);
        mouse->setPosition(location);
        this->addChild(mouse);
        PhysicsJointPin* joint = PhysicsJointPin::construct(mouse->getPhysicsBody(), body, location);
        //            PhysicsJointDistance* joint = PhysicsJointDistance::construct(mouse->getPhysicsBody(), body, Point::ZERO, Point::ZERO);
        joint->setMaxForce(20000.0f * body->getMass());
        this->getScene()->getPhysicsWorld()->addJoint(joint);
        _mouses.insert(std::make_pair(touch->getID(), mouse));
        
        return true;
    }
    
    return false;
}

void MyPhysicsNode::TouchMoved(Touch *touch, Event *pEvent)
{
    auto it = _mouses.find(touch->getID());
    
    if (it != _mouses.end())
    {
        it->second->setPosition(touch->getLocation());
    }
}

void MyPhysicsNode::TouchEnded(Touch *touch, Event *pEvent)
{
    auto it = _mouses.find(touch->getID());
    
    if (it != _mouses.end())
    {
        this->removeChild(it->second);
        _mouses.erase(it);
    }
}

void MyPhysicsNode::TouchCancelled(Touch *touch, Event *pEvent)
{
    TouchEnded(touch, pEvent);
}

Sprite* MyPhysicsNode::getNode(){
    return m_pNode;
}

void MyPhysicsNode::onEnter()
{
    Node::onEnter();
}

void MyPhysicsNode::onExit()
{
    Node::onExit();
}