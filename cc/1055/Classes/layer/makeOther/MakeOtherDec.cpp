//
//  MakeOtherDec.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#include "MakeOtherDec.h"
MakeOtherDec::MakeOtherDec()
{
    
}
MakeOtherDec::~MakeOtherDec()
{
    
}
bool MakeOtherDec::init()
{
    if(!BaseLayer::init())
        return false;
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeOtherDec.csd");
    addChild(_studioLayer);
    
    //加载碗，盘子一类的工具
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    ActionHelper::showBackInOut(m_pBowl, 0.5, ActionHelper::ShowDirection::show_from_left);
    
    return true;
}
void MakeOtherDec::onEnter()
{
    BaseLayer::onEnter();
}
void MakeOtherDec::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeOtherDec::onExit()
{
    BaseLayer::onExit();
    
}
void MakeOtherDec::onTouchDown(Node* node,MoveLisenter* lisenter)
{
}
void MakeOtherDec::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    
}
void MakeOtherDec::onTouchUp(Node* node,MoveLisenter* lisenter)
{
}
void MakeOtherDec::touchEnd(ui::Widget* widget)
{
    
    
}