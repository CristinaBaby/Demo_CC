//
//  BaseLayer.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/29.
//
//

#include "BaseLayer.h"
#include "HomeScene.h"
#include "SSCInternalLibManager.h"


BaseLayer::BaseLayer()
{
    m_bIsShowReturn = false;
    _type = kDialogReturnHome;
    _studioLayer = nullptr;
}
BaseLayer::~BaseLayer()
{
    
}
bool BaseLayer::init()
{

    if(!Layer::init())
        return false;
    
    //注册android返回按钮
    auto e = EventListenerKeyboard::create();
    e->onKeyReleased = CC_CALLBACK_2(BaseLayer::onKeyReleasedCallBack, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
    
    this->setKeypadEnabled(true);
    this->setKeyboardEnabled(true);
    
    return true;
}
void BaseLayer::onEnter()
{
    Layer::onEnter();
}
void BaseLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    //设置接收事件
    if(_studioLayer != nullptr && _studioLayer->_touchBtnCallback == nullptr)
        _studioLayer->_touchBtnCallback = CC_CALLBACK_1(BaseLayer::touchEnd, this);
    
    if(_studioLayer != nullptr){
        
        auto btn_next_light = _studioLayer->getNodeByName("btn_next_light");
        if(btn_next_light != nullptr){
            btn_next_light->stopAllActions();
            btn_next_light->runAction(RepeatForever::create(RotateBy::create(1, 90)));
        }
        
        auto _next = _studioLayer->getNodeByName("btn_next");
        if(_next != nullptr){
            _next->stopAllActions();
            _next->setRotation(0);
            _next->setScale(1);
            _next->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2),Sequence::create(EaseExponentialOut::create(Sequence::create(ScaleTo::create(0.65, 1.1),ScaleTo::create(0.5, 1),nullptr)), nullptr), nullptr)));
        }
    }


}
void BaseLayer::onExit()
{
    Layer::onExit();
}
void BaseLayer::setAndroidKeyType(DialogFuncType type)
{
    _type = type;
}
// 监听Android返回键事件
void BaseLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if(GameDataInstance->getIsMoreGame())
            return;
        
        if (!m_bIsShowReturn) {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create((void*)_type, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            if(_type == kDialogExit)
                dialog->setContentText(ExitGame);
            else
                dialog->setContentText(ReturnHome);
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addChild(dialog, 9999);
        }
    }
}
void BaseLayer::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void BaseLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    size_t lType = (size_t)type;
    if(lType == kDialogReturnHome){
    
        //回到Home界面
        if(IAPManager::getInstance()->isShowAds()){
            
            Director::getInstance()->popToRootScene();
            
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
            AdLoadingLayerBase::loadingDoneCallback=[=](){
                
                Director::getInstance()->runWithScene(HomeScene::createScene());
            };
            
        }else{
            
            Director::getInstance()->popToRootScene();
            Director::getInstance()->runWithScene(HomeScene::createScene());
            
        }
    }
    if(lType == kDialogExit){
        
        //离开游戏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        CFSystemFunction sys;
        SSCInternalLibManager::getInstance()->endSession();
        Director::getInstance()->end();
#endif
    
    }
}