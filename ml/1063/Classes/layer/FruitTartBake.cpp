//
//  FruitTartBake.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#include "FruitTartBake.h"
#include "FrameStatus.h"
#include "WeddingFoodData.h"
#include "FruitTartAddFruit.h"
#include "ChangeLoading.h"
REGIST_SECENE(FruitTartBake);

FruitTartBake::FruitTartBake(){
    
}
bool FruitTartBake::init(){
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("FruitTartBake.csb");
        addChild(_operate);
        _operate->getNodeByName<ui::CheckBox*>("button")->setEnabled(false);
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
void FruitTartBake::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    fruitTart = _operate->getNodeByName("fruitTart");
    auto frame = FrameStatus::create(fruitTart->getName());
    frame->setSearchAction(_operate->getAction());
    fruitTart->runAction(Sequence::create(frame->createAction(0.7, "end"),CallFunc::create([this](){
        _prompt->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.35, Vec2(10,0)), MoveBy::create(0.35, Vec2(-10,0)),  nullptr)));
        _prompt->setVisible(true);
        _operate->getNodeByName("open")->setVisible(false);
        _operate->getNodeByName("close")->setVisible(true);
         _operate->playNodeSound("close");
        _operate->getNodeByName<ui::CheckBox*>("button")->setEnabled(true);
    }), nullptr));
}

void FruitTartBake::touchEnd(ui::Widget* widget){
    if(widget->getName() == "button"){
        _loopSound = _operate->playNodeSound("button");
        stopPrompt();
        widget->stopAllActions();
        widget->runAction(CallFunc::create([widget](){widget->setEnabled(false);}));
        auto close =  _operate->getNodeByName("close");
        auto light = _operate->getNodeByName("light");
        light->setVisible(true);
        auto animattion = Animation::create();
        animattion->setDelayPerUnit(0.15);
        animattion->setLoops(-1);
        animattion->addSpriteFrame(Sprite::create("png/Fruit Tart/3/fire0.png")->getSpriteFrame());
        animattion->addSpriteFrame(Sprite::create("png/Fruit Tart/3/fire1.png")->getSpriteFrame());
        light->runAction(Animate::create(animattion));
        auto baking = _operate->getNodeByName("bakeing");
        
        baking->runAction(Sequence::create(FadeTo::create(3.5, 255),CallFunc::create([widget,this,close](){
            auto checkBox =  dynamic_cast<ui::CheckBox*>(widget);
            checkBox->setSelected(!checkBox->isSelected());
            close->setVisible(false);
            close->stopAllActions();
            SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
            _loopSound = -1;
            _operate->playNodeSound("open");
            _operate->getNodeByName("open")->setVisible(true);
            fruitTart->runAction(Sequence::create(Spawn::create(MoveTo::create(1, fruitTart->getParent()->convertToNodeSpace(Director::getInstance()->getVisibleSize()*.5)),ScaleTo::create(1, 0.8), nullptr),CallFunc::create([this](){
                auto mixEndParticle = _operate->getNodeByName<ParticleSystemQuad*>("mixEndPraticle");
                _operate->playNodeSound("mixEndPraticle");
                if(mixEndParticle != nullptr)
                    mixEndParticle->resetSystem();
                ActionHelper::delayFunc(0.6, this, [](){
            
                          Director::getInstance()->replaceScene(TransitionFade::create(0.8,FruitTartAddFruit::createScene(),Color3B::WHITE));
                   
                });
            }), nullptr));
        }),  nullptr));
    }
}
