//
//  CFLayer.cpp
//  CodeBase
//
//  Created by zhangguangzong1 on 9/1/14.
//
//

#include "depends/component/layerext/CFLayer.h"

bool CFLayer::init()
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        _fnBackFunc = nullptr;
        
        _bgLayer = Layer::create();
        _bgLayer->setPosition(Vec2::ZERO);
        this->addChild(_bgLayer,kBackground);
        
        _contentLayer = Layer::create();
        _contentLayer->setPosition(Vec2::ZERO);
        this->addChild(_contentLayer,kContent);
        
        _uiLayer = Layer::create();
        _uiLayer->setPosition(Vec2::ZERO);
        this->addChild(_uiLayer,kUI);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

bool CFLayer::onBackKeyDown()
{
    if(_fnBackFunc)
        return _fnBackFunc();
    return false;
}

void CFLayer::onEnter()
{
    KeypadDispatchCenter::getInstance()->addDelegate(this);
    Layer::onEnter();
}

void CFLayer::onExit()
{
    Layer::onExit();
    KeypadDispatchCenter::getInstance()->removeDelegate(this);
}

void CFLayer::addToBgLayer(Node* node,int zorder,int tag)
{
    _bgLayer->addChild(node,zorder,tag);
}

void CFLayer::addToContentLayer(Node* node,int zorder,int tag)
{
    _contentLayer->addChild(node,zorder,tag);
}

void CFLayer::addToUILayer(Node* node,int zorder,int tag)
{
    _uiLayer->addChild(node,zorder,tag);
}

bool CFLayer::isNodeVisiable(Node* node)
{
    bool visible = node->isVisible();
    
    for (Node *c = node->getParent(); c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    return visible;
}