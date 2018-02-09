//
//  CookieRoll.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/11.
//
//

#include "CookieRoll.h"
#include "CookieCut.h"
#include "WeddingFoodData.h"
#include "FrameStatus.h"
REGIST_SECENE(CookieRoll);
const string CookieRoll::_eventName = "Donut cut";
IMPLEMENT_COCOS2DX_CLASS(CookieRoll);
const int CookieRollShakeTag = 210;

CookieRoll::CookieRoll(){
    _moveOp = nullptr;
}

void CookieRoll::touchEnd(ui::Widget* widget){
    if(widget->getName() == "btn_next"){
        
    } else if(widget->getName() == "btn_reset"){
        
    }
}

bool CookieRoll::init() {
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("CookieRoll.csb");
        addChild(_operate);
        
        auto paths = WeddingFoodData::getRollCookie();
        float time = 1.3;
        
        for(int i=0;i<4;i++) {
            auto _node = _operate->getNodeByName(StringUtils::format("dough%d",i));
            _operate->changeNode(_node->getName(), paths[i]);
            auto _frameStatus = FrameStatus::create(_node->getName());
            _frameStatus->setSearchAction(_operate->getAction());
            Vector<FiniteTimeAction*> scaleArrays,hideArrays;
            if(i !=0){
                scaleArrays.pushBack(DelayTime::create(i/2*time*2));
                scaleArrays.pushBack(_frameStatus->createAction(time*2,StringUtils::format("%d",i)));
                hideArrays.pushBack(DelayTime::create((i*2-1)*time));
                hideArrays.pushBack(FadeTo::create(0, 175));
                hideArrays.pushBack(FadeTo::create(time, 255));
                hideArrays.pushBack(CallFunc::create(CC_CALLBACK_0(CookieRoll::changeFunc, this,i)));
            }
            if(i != 3){
                scaleArrays.pushBack(_frameStatus->createAction(time*2,StringUtils::format("%d",i+1)));
                hideArrays.pushBack(DelayTime::create(time));
                hideArrays.pushBack(FadeTo::create(time, 0));
                hideArrays.pushBack(Hide::create());
                scaleArrays.pushBack(CallFunc::create(CC_CALLBACK_0(CookieRoll::changeFunc, this,i)));
            }
           
            _node->runAction(Sequence::create(scaleArrays));
            _node->runAction(Sequence::create(hideArrays));
            
        }
        
        return true;
    }
    return false;
}
void CookieRoll::onExitTransitionDidStart(){
    BaseStepLayer::onExitTransitionDidStart();
}

void CookieRoll::onEnterTransitionDidFinish() {
    BaseStepLayer::onEnterTransitionDidFinish();
    auto title = _operate->getNodeByName("text");
    SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
    ActionHelper::showBackInOut(title, title->getPosition(), ActionHelper::ShowDirection::show_from_top,[this](){
        auto _roll = _operate->getNodeByName("tool");
        ActionHelper::showBackInOut(_roll, _roll->getPosition(), ActionHelper::ShowDirection::show_from_right,[this,_roll](){
            auto _actionShake = RepeatForever::create(Sequence::create(ActionHelper::createShakeAction(),DelayTime::create(2),nullptr));
            _actionShake->setTag(CookieRollShakeTag);
            _roll->runAction(_actionShake);
            
            auto _rect = CocosHelper::getNodeRectInWorld(_operate->getNodeByName("dough0"));
             _moveOp = MoveHideShade::create();
            _moveOp->setMoveType(MoveComponentType::kTypeBack);
            _moveOp->setTarget(LQRect::create(_rect));
            _moveOp->addListners(ComponentTouchEnd, CC_CALLBACK_3(CookieRoll::toucComponetCallback, this));
            _moveOp->addListners(ComponentTouchMove, CC_CALLBACK_3(CookieRoll::toucComponetCallback, this));
            _moveOp->addListners(ComponentTouchCancle, CC_CALLBACK_3(CookieRoll::toucComponetCallback, this));
            _roll->addComponent(_moveOp);
            
        });
        _roll->setVisible(true);
    });
    title->setVisible(true);

    
}

 void CookieRoll::onEnter(){
     BaseStepLayer::onEnter();
    // auto _node = _operate->getNodeByName("dough");
    // _node->runAction(RepeatForever::create(ActionHelper::getJellyAction()));
    // _actionManager->pauseTarget(_node);
     for(int i=0;i<4;i++) {
         auto _node = _operate->getNodeByName(StringUtils::format("dough%d",i));
         _actionManager->pauseTarget(_node);
     }
}

void CookieRoll::toucComponetCallback(Node* _node,Component* _moveOper,OperateListner* _oper) {
    auto move = dynamic_cast<TouchMoveComponent*>(_moveOper);
    if(_oper->getKey() == ComponentTouchMove) {
        
        if(move->checkInlimit()) {
            if(move->getMoveType() == MoveComponentType::kTypeBack) {
                _node->stopActionByTag(CookieRollShakeTag);
                ActionHelper::showScaleArrow(_operate->getNodeByName("title_v"));
                move->setMoveType(MoveComponentType::kTypeInRect);
                //_node->runAction(RotateBy::create(0.3, 90));
            }
              move->playSound();
            auto node = _operate->getNodeByName("text");
            if(node != nullptr){
                node->setName("text1");
                ActionHelper::hide(node, ActionHelper::ShowDirection::show_from_top);
            }
            for(int i=0;i<4;i++) {
                auto _node = _operate->getNodeByName(StringUtils::format("dough%d",i));
                _actionManager->resumeTarget(_node);
            }
            _actionManager->resumeTarget(_operate->getNodeByName("dough"));
        }
        _node->stopActionByTag(CookieRollShakeTag);
    } else {
        move->pauseSound();
        for(int i=0;i<4;i++) {
            auto _node = _operate->getNodeByName(StringUtils::format("dough%d",i));
            _actionManager->pauseTarget(_node);
        }
         _actionManager->pauseTarget(_operate->getNodeByName("dough"));
        if(move->getMoveType() == MoveComponentType::kTypeBack){
            _node->stopActionByTag(CookieRollShakeTag);
            auto _actionShake = RepeatForever::create(Sequence::create(ActionHelper::createShakeAction(),DelayTime::create(2),nullptr));
            _actionShake->setTag(CookieRollShakeTag);
            _node->runAction(_actionShake);
        }
    }
}
void CookieRoll::changeFunc(int index){
    log("==%d",index);
    if(index == 0) {
            _operate->getNodeByName("title_v")->setVisible(false);
            ActionHelper::showScaleArrow(_operate->getNodeByName("title_h"));
            _operate->getNodeByName("tool")->runAction(RotateBy::create(0.3, -90));
        
    } else if(index ==  3) {
        _moveOp->stopSound();
        auto _node = _operate->getNodeByName("dough");
        _node->stopAllActions();
        _operate->getNodeByName("title_h")->setVisible(false);
        _node->setScale(1,1);
        auto _tool = _operate->getNodeByName("tool");
        _tool->removeAllComponents();
        ActionHelper::hide(_tool, ActionHelper::ShowDirection::show_from_right,[](){
            Director::getInstance()->replaceScene(CookieCut::createScene());
        });
        
        auto _pari = ParticleSystemQuad::create("particle/start_1.plist");
        if(_pari != nullptr) {
            _pari->setPosition(_node->convertToWorldSpace(_node->getContentSize()*.5));
            this->addChild(_pari,50);
        }
        SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
    }
}

