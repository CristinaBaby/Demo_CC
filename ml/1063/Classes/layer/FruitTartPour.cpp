//
//  FruitTartPour.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#include "FruitTartPour.h"
#include "FrameStatus.h"
#include "DragFall.h"
#include "FruitTartBake.h"
REGIST_SECENE(FruitTartPour);
FruitTartPour::FruitTartPour(){
    
}
bool FruitTartPour::init(){
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("FruitTartPour.csb");
        addChild(_operate);
        _prompt = _operate->getNodeByName<Sprite*>("finger");
        CocosHelper::visitAllNode(_operate, [](Node* node){
            auto _paritcle = dynamic_cast<ParticleSystemQuad*>(node);
            if(_paritcle != nullptr)
                _paritcle->stopSystem();
        });
        return true;
    }
    return false;
}
void FruitTartPour::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
        auto bowl = _operate->getNodeByName("bow");
    auto node = _operate->getNodeByName("mixThing");
    auto frame = FrameStatus::create(node->getName());
    frame->setSearchAction(_operate->getAction());
    node->runAction(Sequence::create(frame->createAction(0.7, "moveEnd"),CallFunc::create([bowl,this,node](){
        ActionHelper::runBezierMoveHideAction(_prompt, _prompt->getPosition(), _operate->getNodeByName("mixThingInbowl")->getPosition());
        _prompt->setVisible(true);
        auto drag = DragFall::create();
        drag->setInbow(_operate->getNodeByName("mixThingInbowl"), Sequence::create(CallFunc::create([this](){
            auto pour0 = _operate->getNodeByName<Sprite*>("pour0");
            pour0->setVisible(true);
            auto _animation= Animation::create();
            _animation->setDelayPerUnit(0.15);
            _animation->addSpriteFrame(pour0->getSpriteFrame());
            _animation->addSpriteFrameWithFile("png/cookies/2/pour1.png");
            _animation->setLoops(-1);
            pour0->runAction(Animate::create(_animation));
        }), DelayTime::create(1), FadeTo::create(1.5, 255),CallFunc::create([node,this](){
            _operate->getNodeByName("pour0")->setVisible(false);
            node->getChildByName("inner")->setVisible(false);
        }), nullptr));
        drag->setTargetRect(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit")));
        node->addComponent(drag);
        drag->getFrameStatus()->setSearchAction(_operate->getAction());
        drag->addListners(FallThingstartFall, [this,bowl](Node*node,Component*, OperateListner* _lis){
            this->stopPrompt();
            auto inner = node->getChildByName("inner");
            inner->stopAllActions();
            inner->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(2, 0.7,0.5),Hide::create(),CallFunc::create([node](){
//                auto particle = dynamic_cast<ParticleSystemQuad*>(node->getChildByName ("particle"));
//                particle->stopSystem();
            }), nullptr));
        });
        drag->addListners(FallThingfallEnd, [this,bowl](Node*node,Component*, OperateListner* _lis){
            ActionHelper::delayFunc(0.3,this, [node,bowl,this]{
                node->runAction(RotateTo::create(0.6, 0));
                ActionHelper::hide(node, ActionHelper::ShowDirection::show_from_right);
                bowl->runAction(Sequence::create(Spawn::create(MoveTo::create(0.6, bowl->getParent()->convertToNodeSpace(Director::getInstance()->getVisibleSize()*.5)),ScaleTo::create(0.6, 1.15), nullptr),CallFunc::create([this](){
                    auto mixEndParticle = _operate->getNodeByName<ParticleSystemQuad*>("mixEndPraticle");
                    _operate->playNodeSound("mixEndPraticle");
                    if(mixEndParticle != nullptr)
                        mixEndParticle->resetSystem();
                    ActionHelper::delayFunc(1, this, [](){
                        Director::getInstance()->replaceScene(FruitTartBake::createScene());
                    });
                }),  nullptr));
            });
        });
    }), nullptr));
    
SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
    ActionHelper::showBezier(bowl, bowl->getPosition(), ActionHelper::ShowDirection::show_from_left);
    bowl->setVisible(true);
}

void FruitTartPour::touchEnd(ui::Widget* widget){
    
}
