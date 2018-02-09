//
//  RollStep.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/11.
//
//

#include "RollStep.h"
#include "MoveHideShade.h"
#include "TimerChangeComponent.h"
#include "TouchGame.h"
#include "ChangeLayer.h"
#include "ParticleLayer.h"
#include "FlurryEventManager.h"
const string RollStep::_eventName = "Donut cut";
IMPLEMENT_COCOS2DX_CLASS(RollStep);
const int RollStepShakeTag = 210;
void RollStep::touchEnd(ui::Widget* widget){
    if(widget->getName() == "btn_next"){
        ChangeLayer::_changeLayer = [](){
            return TouchGame::create();
        };
        ChangeLayer::loading();
    } else if(widget->getName() == "btn_reset"){
        _operate->getNodeByName("btn_next")->setVisible(false);
        pCutOperate->setEnabled(false);
        pCutOperate->getOwner()->stopAllActions();
        pCutOperate->getOwner()->setScale(1);
        ActionHelper::show(pCutOperate->getOwner(), _starPos, ActionHelper::ShowDirection::show_from_top,[this](){
             pCutOperate->setEnabled(true);
            pCutOperate->reset();
        });
           pCutOperate->getOwner()->setScale(1);
         SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
        
    }
}

bool RollStep::init() {
    if(BaseStepLayer::init()) {
        _prompt = Sprite::create("ui/pubilc/finger1.png");
        addChild(_prompt,300);
        _prompt->setVisible(false);
        _changeOperate = nullptr;
        _operate = StudioLayer::create("cocostudio/rollStep.csb");
        addChild(_operate);
        return true;
    }
    return false;
}
 void RollStep::onExitTransitionDidStart(){
      FlurryEventManager::getInstance()->logCurrentModuleExitEvent(_eventName);
     BaseStepLayer::onExitTransitionDidStart();
}

void RollStep::onEnterTransitionDidFinish() {
    BaseStepLayer::onEnterTransitionDidFinish();
     FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(_eventName);
    _starPos  = _operate->getNodeByName("cutter")->getPosition();
    auto _roll = _operate->getNodeByName("tool");
    ActionHelper::show(_roll, _roll->getPosition(), ActionHelper::ShowDirection::show_from_right,[this,_roll](){
        auto _actionShake = RepeatForever::create(Sequence::create(ActionHelper::createShakeAction(),DelayTime::create(2),nullptr));
        _actionShake->setTag(RollStepShakeTag);
        _roll->runAction(_actionShake);
        
        auto _rect = CocosHelper::getNodeRectInWorld(_operate->getNodeByName("dough"));
        auto _moveOp = MoveHideShade::create();
        _moveOp->setMoveType(MoveComponentType::kTypeBack);
        _moveOp->setTarget(LQRect::create(_rect));
        _moveOp->addListners(ComponentTouchEnd, CC_CALLBACK_3(RollStep::toucComponetCallback, this));
        _moveOp->addListners(ComponentTouchMove, CC_CALLBACK_3(RollStep::toucComponetCallback, this));
        _moveOp->addListners(ComponentTouchCancle, CC_CALLBACK_3(RollStep::toucComponetCallback, this));
        _roll->addComponent(_moveOp);
        
        auto _donut = _operate->getNodeByName("dough");
        _changeOperate = TimerChangeComponent::create();
        vector<string> _path;
        for(int i=0;i<=7;++i)
            _path.push_back(StringUtils::format("png/3/dough-rolling_%d.png",i));
        _changeOperate->setDelayPerUnit(1.4);
        _changeOperate->setFrameFile(_path);
        _donut->addComponent(_changeOperate);
        _changeOperate->addListners(TimerChangeEnd, CC_CALLBACK_3(RollStep::TimerChangeCallBack, this));
        _changeOperate->addListners(TimerChangeFrame, CC_CALLBACK_3(RollStep::TimerChangeCallBack, this));
        _donut->runAction(ActionHelper::createRollJelly());
        _actionManager->pauseTarget(_donut);
    });
    _roll->setVisible(true);
    
    
}
void RollStep::toucComponetCallback(Node* _node,Component* _moveOper,OperateListner* _oper) {
    auto move = dynamic_cast<TouchMoveComponent*>(_moveOper);
    if(_oper->getKey() == ComponentTouchMove) {
        
        if(move->checkInlimit()) {
            if(move->getMoveType() == MoveComponentType::kTypeBack) {
                _node->stopActionByTag(RollStepShakeTag);
                ActionHelper::showScaleArrow(_operate->getNodeByName("title_v"));
                move->setMoveType(MoveComponentType::kTypeInRect);
                _node->runAction(RotateBy::create(0.3, 90));
            }
            _changeOperate->start();
            _actionManager->resumeTarget(_operate->getNodeByName("dough"));
        }
        _node->stopActionByTag(RollStepShakeTag);
    } else {
        _changeOperate->stop();
        _actionManager->pauseTarget(_operate->getNodeByName("dough"));
        if(move->getMoveType() == MoveComponentType::kTypeBack){
            _node->stopActionByTag(RollStepShakeTag);
            auto _actionShake = RepeatForever::create(Sequence::create(ActionHelper::createShakeAction(),DelayTime::create(2),nullptr));
            _actionShake->setTag(RollStepShakeTag);
            _node->runAction(_actionShake);
        }
    }
}
void RollStep::TimerChangeCallBack(Node* _node,Component* c,OperateListner* _lis){
    if(_lis->getKey() == TimerChangeFrame) {
        auto _frameIndex = dynAny_cast<int>(_lis->getData());
        if(_frameIndex == 3){
            _operate->getNodeByName("title_v")->setVisible(false);
            ActionHelper::showScaleArrow(_operate->getNodeByName("title_h"));
            _operate->getNodeByName("tool")->runAction(RotateBy::create(0.3, -90));
        }
    } else if(_lis->getKey() ==  TimerChangeEnd) {
        _operate->getNodeByName("title_h")->setVisible(false);
        _node->setScale(1,1);
        auto _tool = _operate->getNodeByName("tool");
        _changeOperate->setEnabled(false);
        _tool->removeAllComponents();
        ActionHelper::hide(_tool, ActionHelper::ShowDirection::show_from_right);
        showCutter();
        auto _pari = ParticleSystemQuad::create("particle/start_1.plist");
        _pari->setPosition(_node->convertToWorldSpace(_node->getContentSize()*.5));
        this->addChild(_pari,50);
        SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
    }
}

void RollStep::showCutter(){
    auto _cutter = _operate->getNodeByName("cutter");
    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    ActionHelper::showBouce(_cutter, _starPos, ActionHelper::ShowDirection::show_from_top,[_cutter,this](){
        ActionHelper::delayFunc(0.7, _prompt, [this,_cutter](){
            
        ActionHelper::runBezierMoveHideAction(_prompt, _cutter->getPosition(),_operate->getPosinWord("dough")+Vec2(0,80));
        });
        ActionHelper::showTitle(_operate->getNodeByName("title"));
        auto _reset = _operate->getNodeByName("btn_reset");
        ActionHelper::showBackInOut(_reset, _reset->getPosition(), ActionHelper::ShowDirection::show_from_right);
        _reset->setVisible(true);
        
        pCutOperate = CutDountComplent::create();
        pCutOperate->setDountPath("png/3/dough0.png");
        pCutOperate->setMaskPath("png/3/mask.png");
        pCutOperate->setDough(_operate->getNodeByName("dough"));
        pCutOperate->setTargetNum(6);
        _cutter->addComponent(pCutOperate);
        pCutOperate->addListners(cutDountFailed, [this](Node* _node,Component* c,OperateListner* _lis){
             SoundPlayer::getInstance()->playEffect("sound/other/mini game_miss.mp3");
          
        });
        pCutOperate->addListners(cutDountSuccess, [this](Node* _node,Component* c,OperateListner* _lis){
            this->stopPrompt();
            auto _particle = ParticleSystemQuad::create("particle/fallStarParticle.plist");
            _particle->setPosition(_node->convertToWorldSpace(_node->getContentSize()*.5));
            addChild(_particle,30);
              SoundPlayer::getInstance()->playEffect("sound/general/cut donut particle.mp3");
        
            _operate->getNodeByName("title")->stopAllActions();
            //_operate->getNodeByName("title")->setVisible(false);
        });
        pCutOperate->addListners(cutDountEnd, [this](Node* _node,Component* c,OperateListner* _lis){
            pCutOperate->setEnabled(false);
            ActionHelper::hide(pCutOperate->getOwner(), ActionHelper::ShowDirection::show_from_left,nullptr,false);
        
            auto dounts = dynamic_cast<CutDountComplent*>(c)->getCutDounts();
            for(int i=0;i<dounts.size();i++){
                auto dount = dounts.at(i);
                auto _s = Sprite::create("png/3/dough.png");
                _s->setPosition(dount->getContentSize()*.5);
                dount->addChild(_s);
                _s->setOpacity(0);
                _s->runAction(FadeTo::create(0.5, 255));
                dount->runAction(Sequence::create(FadeTo::create(0.5, 0),DelayTime::create(0.1*i),MoveTo::create(0.5, dount->getParent()->convertToNodeSpace(Vec2(getContentSize().width*.5-300+120*i,getContentSize().height*.5))),CallFunc::create([i,this,c](){
                    if(i==5){
                        auto newdounts = dynamic_cast<CutDountComplent*>(c)->getCutDounts();
                        for(int j=0;j<newdounts.size();j++){
                            auto dount = newdounts.at(j);
                            dount->runAction(Sequence::create(DelayTime::create(0.35*j), CallFunc::create([](){
                                SoundPlayer::getInstance()->playEffect("sound/general/cut tool fall.mp3");
                            }), ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1),CallFunc::create([j,this](){
                                if(j==5){
                                    _operate->getNodeByName("title")->stopAllActions();
                                    _operate->getNodeByName("title")->setVisible(false);
                                    this->addChild(ParticleLayer::create());
                                    auto _next = _operate->getNodeByName("btn_next");
                                    ActionHelper::showBackInOut(_next, _next->getPosition(), ActionHelper::ShowDirection::show_from_top);
                                    _next->setVisible(true);
                                }
                            }), nullptr));
                        }
                        
                    }
                }), nullptr));
                dount->runAction(Sequence::create(DelayTime::create(0.35*i+1),ActionHelper::getJellyAction(), nullptr));
            }
            
            _operate->getNodeByName("dough")->runAction(FadeTo::create(0.7, 0));
        });
    });
    _cutter->setVisible(true);
    
}
