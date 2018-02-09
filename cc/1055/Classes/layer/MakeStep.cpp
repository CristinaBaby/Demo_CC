//
//  MakeStep.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/30.
//
//

#include "MakeStep.h"

//REGIST_SECENE(MakeStep)

string MakeStepToolPath[5]={"water","butter","salt","sugar","flour"};
MakeStep::MakeStep()
{
}
MakeStep::~MakeStep()
{
}
bool MakeStep::init()
{
    
    if(!BaseLayer::init())
        return false;
    //加载_studioLayer
    _studioLayer = StudioLayer::create("xxx.csb");
    addChild(_studioLayer);
    
    //加载碗，盘子一类的工具
    auto bowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    ActionHelper::showBackInOut(bowl, 0.5, ActionHelper::ShowDirection::show_from_left);
    
    //显示工具
    for (int i = 0 ;i<5;i++) {
        auto sprite = _studioLayer->getNodeByName<Sprite*>(MakeStepToolPath[i].c_str());
        
        auto lisenter = MoveLisenter::create();
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, sprite);
        lisenter->setOrginOffset();
        lisenter->addRectByNode(bowl);
        
        lisenter->onTouchDown = CC_CALLBACK_2(MakeStep::onTouchDown, this);
        lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeStep::onTouchMoveInRect, this);
        lisenter->onTouchUp = CC_CALLBACK_2(MakeStep::onTouchUp, this);
        
        ActionHelper::showBackInOut(sprite, 0.3*i, ActionHelper::ShowDirection::show_from_right);
    }
    return true;
}
void MakeStep::onEnter()
{
    BaseLayer::onEnter();
}
void MakeStep::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeStep::onExit()
{
    BaseLayer::onExit();

}
void MakeStep::onTouchDown(Node* node,MoveLisenter* lisenter)
{
}
void MakeStep::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    
}
void MakeStep::onTouchUp(Node* node,MoveLisenter* lisenter)
{
}
void MakeStep::touchEnd(ui::Widget* widget)
{
    
    
}