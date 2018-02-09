//
//  ActionManager.cpp
//  CCATS007
//
//  Created by liqiang on 15/9/22.
//
//

#include "AppearManager.h"

void AppearManager::LeftAppear(Node* n,float time,CallFunc* func )
{
   
    n->setVisible(false);
    
    Vec2 pos = n->convertToWorldSpace(Vec2::ZERO);
    
//    Vec2 pos = n->getPosition();
    
    float m_foffSetX = pos.x+n->getContentSize().width;
    

    
    MoveBy* move = MoveBy::create(time, Vec2(m_foffSetX, 0));
    
    n->runAction(Sequence::create(MoveBy::create(0, Vec2(-m_foffSetX, 0)),Show::create(),EaseBackInOut::create(move),func, NULL));
}

void AppearManager::RightAppear(Node* n,float time,CallFunc* func )
{
    n->setVisible(false);
    
    Vec2 pos = n->convertToWorldSpace(Vec2::ZERO);
    
    float m_foffSetX = Director::getInstance()->getVisibleSize().width-pos.x+n->getContentSize().width/2;
    
    
    MoveBy* move = MoveBy::create(time, Vec2(-m_foffSetX, 0));
    
    n->runAction(Sequence::create(MoveBy::create(0, Vec2(m_foffSetX,0)),Show::create(),EaseBackInOut::create(move),func, NULL));
}

void AppearManager::TopAppear(Node* n,float time,CallFunc* func)
{

    n->setVisible(false);
    
    Vec2 pos = n->convertToWorldSpace(Vec2::ZERO);
    

    
    float m_foffSetY = Director::getInstance()->getVisibleSize().height-pos.y+n->getContentSize().height/2;
    

    
    MoveBy* move = MoveBy::create(time, Vec2(0, -m_foffSetY));
    
    n->runAction(Sequence::create(MoveBy::create(0, Vec2(0, m_foffSetY)),Show::create(),EaseBackInOut::create(move),func, NULL));
}

void AppearManager::BottomAppear(Node* n,float time,CallFunc* func )
{

    
    n->setVisible(false);
    
    Vec2 pos = n->convertToWorldSpace(Vec2::ZERO);
    

    
    float m_foffSetY = pos.y+n->getContentSize().height/2;
    

    
    MoveBy* move = MoveBy::create(time, Vec2(0, m_foffSetY));
    
    n->runAction(Sequence::create(MoveBy::create(0, Vec2(0, -m_foffSetY)),Show::create(),EaseBackInOut::create(move),func, NULL));


}






void AppearManager::LeftDisAppear(Node* n,float time,CallFunc* func )
{
    
   
    
    Vec2 pos = n->convertToWorldSpace(Vec2::ZERO);
    
    float m_foffSetX = pos.x+n->getContentSize().width;
    
    MoveBy* move = MoveBy::create(time, Vec2(-m_foffSetX, 0));
    
    n->runAction(Sequence::create(EaseBackInOut::create(move),func, NULL));
}

void AppearManager::RightDisAppear(Node* n,float time,CallFunc* func )
{
    
    
    Vec2 pos = n->convertToWorldSpace(Vec2::ZERO);
    
    float m_foffSetX = Director::getInstance()->getVisibleSize().width-pos.x+n->getContentSize().width/2;
    log("111111===========%f",m_foffSetX);
    MoveBy* move = MoveBy::create(time, Vec2(m_foffSetX, 0));
    
    n->runAction(Sequence::create(EaseBackInOut::create(move),func, NULL));
}

void AppearManager::TopDisAppear(Node* n,float time,CallFunc* func)
{
    
   
    Vec2 pos = n->convertToWorldSpace(Vec2::ZERO);
    
    float m_foffSetY = Director::getInstance()->getVisibleSize().height-pos.y+n->getContentSize().height/2;
    
    MoveBy* move = MoveBy::create(time, Vec2(0, m_foffSetY));
    
    n->runAction(Sequence::create(EaseBackInOut::create(move),func, NULL));
}

void AppearManager::BottomDisAppear(Node* n,float time,CallFunc* func )
{
    
    
   
    
    Vec2 pos = n->convertToWorldSpace(Vec2::ZERO);
    
    float m_foffSetY = pos.y+n->getContentSize().height;
    
    
    MoveBy* move = MoveBy::create(time, Vec2(0, -m_foffSetY));
    
    n->runAction(Sequence::create(EaseBackInOut::create(move),func, NULL));
    
    
}