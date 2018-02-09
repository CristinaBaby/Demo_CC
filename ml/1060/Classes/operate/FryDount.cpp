//
//  FryDount.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/14.
//
//

#include "FryDount.h"


#include "CicleLoadingBar.h"
#include "ChangeLayer.h"
#include "DecorationLayer.hpp"
#include "ParticleLayer.h"
#include "EventLayer.h"
IMPLEMENT_COCOS2DX_CLASS(FryDount);
FryDount::FryDount() {
    fryCount = 0;
}
bool FryDount::init() {
    if(BaseStepLayer::init()) {
        isFirst = true;
        _prompt = Sprite::create("ui/pubilc/finger1.png");
        _prompt->setAnchorPoint(Vec2(0.15, 0.85));
        _prompt->setFlippedX(true);
        addChild(_prompt,30);
        _operate = StudioLayer::create("cocostudio/FryDount.csb");
        addChild(_operate);
        _operate->getNodeByName("title")->setVisible(false);
        for(int i=0;i<=5;++i) {
            auto _node = _operate->getNodeByname<Sprite*>(StringUtils::format("donut%d",i));
            dounts.pushBack(_node);
            _node->setVisible(false);
            _startPos.push_back(_node->getPosition());
        }
        return true;
    }
    return false;
}
void FryDount::addFryOperate(Node* _dount){
    _dount->removeAllComponents();
    auto _fryOperate = DragFryCommonent::create();
    _fryOperate->setFryLimit(LQRect::create(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit"))));
    _fryOperate->setEndlimit(LQRect::create(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit2"))));
    vector<string> paths;
    for(int i=1;i<=5;++i){
        if(i!=4)
        paths.push_back(StringUtils::format("png/5/donuts%d.png",i));
    }
    _fryOperate->setFryPath(paths);
//    vector<string> bubblePath;
//    bubblePath.push_back("png/5/bubble.png");
//    bubblePath.push_back("png/5/bubble1.png");
//    _fryOperate->setBubblePath(bubblePath);
    _fryOperate->addListners(fryOperateFailed, CC_CALLBACK_3(FryDount::fryFailed, this));
    _fryOperate->addListners(FryOperateEnd, CC_CALLBACK_3(FryDount::fryEnd, this));
    _fryOperate->addListners(fryOperateOk, [this](Node* _node,Component*,OperateListner*){
            SoundPlayer::getInstance()->playEffect("sound/general/fry_ok.mp3");
        auto _action = RepeatForever::create(ActionHelper::createShakeAction(2.5));
        _action->setTag(100);
        _node->runAction(_action);
        if(this->isFirst){
        
        this->stopPrompt();
            
            ActionHelper::runBezierMoveHideAction(_prompt, _node->convertToWorldSpace(_node->getContentSize()*.5), _operate->getPosinWord("plate1"));
        }
    });
    _fryOperate->addListners(FryOperateStart, [this](Node* _node,Component*, OperateListner* _lis){
//        if(_loopSound == -1)
//            _loopSound = SoundPlayer::getInstance()->playEffectLoop("sound/general/fry donut.mp3");
        
        auto _bar = _node->getChildByName("loadingBar");
        _operate->getNodeByName("title")->stopActionByTag(ActionHelper::titleActionTag);
        if(_bar != nullptr)
        _bar->setVisible(true);
        this->stopPrompt();
        auto _particle = dynamic_cast<ParticleSystemQuad*>(_node->getChildByName("paritlce"));
        if(_particle != nullptr &&! _particle->isActive())
            _particle->resetSystem();
        
    });
    _fryOperate->addListners(FryOperatePause, [this](Node* _node,Component*, OperateListner* _lis){
        
        auto _bar = _node->getChildByName("loadingBar");
         if(_bar != nullptr)
        _bar->setVisible(false);
        auto _particle = dynamic_cast<ParticleSystemQuad*>(_node->getChildByName("paritlce"));
        if(_particle != nullptr && _particle->isActive())
            _particle->stopSystem();
             _node->stopActionByTag(100);
    });
    _fryOperate->addListners(fryOperateing, [](Node* _node,Component*, OperateListner* _lis){
        auto _bar = dynamic_cast<CicleLoadingBar*>(_node->getChildByName("loadingBar"));
        if(_bar != nullptr){
            _bar->setPercent(dynAny_cast<float>(_lis->getData()));
        }
    });
    
    _dount->removeChildByName("loadingBar");
    auto _loadingBar = CicleLoadingBar::create("ui/fry/progress-bar.png");
    _loadingBar->setName("loadingBar");
    _loadingBar->setVisible(false);
    _loadingBar->setStartRote(-107.00);
    _loadingBar->getMaskSprite()->setAnchorPoint(Vec2(0.5,-0.7));
    _loadingBar->getMaskSprite()->setPosition(Vec2(57,-28));
    _loadingBar->setPosition(_dount->getContentSize().width*.5,_dount->getContentSize().height);
    auto _bg = Sprite::create("ui/fry/progress-bar2.png");
    _bg->setPosition(_loadingBar->getContentSize()*.5);
    _loadingBar->addChild(_bg,-1);
    _dount->addChild(_loadingBar,30);
    auto _arraw = Sprite::create("ui/fry/pointer.png");
    _arraw->setPosition(86.16,24);
    _loadingBar->addChild(_arraw);
    _dount->addComponent(_fryOperate);
    auto _paritlce = ParticleSystemQuad::create("particle/BubbleParticle.plist");
    _paritlce->setPosition(_dount->getContentSize()*.5);
    _dount->addChild(_paritlce,-100);
    _paritlce->setName("paritlce");
    _paritlce->stopSystem();
}

void FryDount::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    auto plate0 = _operate->getNodeByName("plate0");
    ActionHelper::showBackInOut(plate0, plate0->getPosition(), ActionHelper::ShowDirection::show_from_left,[this](){
        for(int i=0;i<dounts.size();i++){
            auto _dount = dounts.at(i);
            ActionHelper::delayFunc(0.2*i, _dount, [this,_dount,i](){
                SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                ActionHelper::showBackInOut(_dount, _dount->getPosition(), ActionHelper::ShowDirection::show_from_top,[this,i](){
                    if(i==5){
                          ActionHelper::showTitle(_operate->getNodeByName("title"));
                        ActionHelper::delayFunc(0.7, _prompt, [this](){
                            
                        ActionHelper::runBezierMoveHideAction(_prompt, _operate->getPosinWord("plate0"), _operate->getPosinWord("Fried"));
                        });
                        for(auto _dount:dounts){
                            addFryOperate(_dount);
                        }
                    }
                });
                _dount->setVisible(true);
            });
        }
        
    });
    plate0->setVisible(true);
    auto plate1 = _operate->getNodeByName("plate1");
    ActionHelper::showBackInOut(plate1, plate1->getPosition(), ActionHelper::ShowDirection::show_from_right);
    plate1->setVisible(true);
}

void FryDount::fryEnd(Node* _node,Component*,OperateListner*){
    stopPrompt();
        isFirst = false;
    fryCount++;
    CocosHelper::reAddNode(_node, _operate->getNodeByName("plate1"));
    _node->runAction(ActionHelper::getJellyAction());
    SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
    if(fryCount == dounts.size()){
        if(_loopSound !=-1){
            SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
            _loopSound = -1;
        }
        auto _plate = _operate->getNodeByName("plate1");
        auto _eventLyaer = EventLayer::create();
        _eventLyaer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _eventLyaer->ignoreAnchorPointForPosition(false);
        _eventLyaer->setPosition(_plate->getParent()->getContentSize()*.5);
        _plate->getParent()->addChild(_eventLyaer,11);
        _plate->getParent()->reorderChild(_plate,12);
        _plate->runAction(Sequence::create(Spawn::create(MoveTo::create(0.8, _plate->getParent()->convertToNodeSpace(Director::getInstance()->getVisibleSize()*.5)),ScaleTo::create(0.8, 1.25),RotateTo::create(0.8, 720-90), nullptr),CallFunc::create([this](){
            auto _particle = ParticleSystemQuad::create("particle/start_1.plist");
            _particle->setPosition(Director::getInstance()->getVisibleSize()*.5);
            this->addChild(_particle,30);
            SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
            ActionHelper::delayFunc(0.6, this, [this](){
                _operate->getNodeByName("title")->stopAllActions();
                _operate->getNodeByName("title")->setVisible(false);
                addChild(ParticleLayer::create(),30);
                ActionHelper::delayFunc(3, this, [](){
                    auto _scene = Scene::create();
                    _scene->addChild(DecorationLayer::create());
                    Director::getInstance()->replaceScene(TransitionFade::create(0.8,_scene,Color3B::WHITE));
                  
                });
            });
           
        }), nullptr));
       
       
    }
}

void FryDount::fryFailed(Node* _node,Component* c,OperateListner*){
     stopPrompt();
        SoundPlayer::getInstance()->playEffect("sound/general/fry failed.mp3");
    for(auto _child:_node->getChildren())
        _child->runAction(Sequence::create(DelayTime::create(1), FadeTo::create(1.8, 0),nullptr));
     _node->stopActionByTag(100);
    c->setEnabled(false);
    auto _smoke = Sprite::create("png/5/smoke.png");
    _smoke->setPosition(_node->convertToWorldSpace(_node->getContentSize()*.5));
    addChild(_smoke,20);
    _smoke->setScale(1, 0.5);
    _smoke->setOpacity(0);
    _smoke->runAction(Sequence::create(DelayTime::create(2),FadeTo::create(0,255),ScaleTo::create(2, 1,1),FadeTo::create(0.3, 0),CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent,_smoke)) ,nullptr));
    _smoke->runAction(Sequence::create(DelayTime::create(2),MoveBy::create(1.1, Vec2(0,400)),nullptr));
    auto _black = Sprite::create("png/5/Black-particles.png");
   
    _smoke->setAnchorPoint(Vec2(0.5,0));
    _black->setOpacity(0);
    _black->runAction(FadeTo::create(0.5, 255));
    auto  _fried = _operate->getNodeByName("Fried");
    _black->setPosition(_fried->convertToNodeSpace(_node->convertToWorldSpace(_node->getContentSize()*.5)));
    _fried->addChild(_black);
    _node->runAction(Sequence::create(DelayTime::create(3),DelayTime::create(0.5), CallFunc::create([_node,this](){
        auto _childs = _node->getChildren();
        for(auto _child:_childs)
            if(_child->getName().compare("Audio") !=0)
                _child->removeFromParent();
      
        auto _itor = std::find(dounts.begin(), dounts.end(), _node);
            SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
        ActionHelper::show(_node, _startPos.at(_itor-dounts.begin()), ActionHelper::ShowDirection::show_from_top,[_node,this](){
            addFryOperate(_node);
        });
        _node->setOpacity(255);
    }), nullptr));
   
}