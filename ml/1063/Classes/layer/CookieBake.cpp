//
//  CookieBake.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#include "CookieBake.h"
#include "FrameStatus.h"
#include "WeddingFoodData.h"
#include "DecorationLayer.h"

REGIST_SECENE(CookieBake);
CookieBake::CookieBake(){
    
}
bool CookieBake::init(){
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("CookieBake.csb");
        addChild(_operate);
        _prompt = _operate->getNodeByName<Sprite*>("finger");
        _operate->getNodeByName<ui::CheckBox*>("button")->setEnabled(false);
        CocosHelper::visitAllNode(_operate, [](Node* node){
            auto _paritcle = dynamic_cast<ParticleSystemQuad*>(node);
            if(_paritcle != nullptr)
                _paritcle->stopSystem();
        });
        return true;
    }
    return false;
}
void CookieBake::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    cookie = _operate->getNodeByName("cookie");
    auto frame = FrameStatus::create(cookie->getName());
    frame->setSearchAction(_operate->getAction());
    cookie->runAction(Sequence::create(frame->createAction(0.7, "end"),CallFunc::create([this](){
        _prompt->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.35, Vec2(10,0)), MoveBy::create(0.35, Vec2(-10,0)),  nullptr)));
        _prompt->setVisible(true);
        _operate->getNodeByName("open")->setVisible(false);
        _operate->getNodeByName("close")->setVisible(true);
        _operate->playNodeSound("close");
        _operate->getNodeByName<ui::CheckBox*>("button")->setEnabled(true);
    }), nullptr));
}

void CookieBake::touchEnd(ui::Widget* widget){
    if(widget->getName() == "button"){
        _eventDispatcher->setEnabled(false);
        widget->stopAllActions();
        stopPrompt();
         _loopSound = _operate->playNodeSound("button");
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
        
        auto cookieAnimation = Animation::create();
        for(int i=0;i<3;i++)
            cookieAnimation->addSpriteFrameWithFile(StringUtils::format("png/cookies/5/cookies%d.png",i));
        cookieAnimation->setDelayPerUnit(1.5);
        cookie->runAction(Sequence::create(Animate::create(cookieAnimation),CallFunc::create([widget,this,close](){
            auto checkBox =  dynamic_cast<ui::CheckBox*>(widget);
            checkBox->setSelected(!checkBox->isSelected());
            close->setVisible(false);
            close->stopAllActions();
            SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
            _loopSound = -1;
            _operate->playNodeSound("open");
            _operate->getNodeByName("open")->setVisible(true);
            auto pan = _operate->getNodeByName("pan");
            pan->setVisible(true);
            auto panFrame = FrameStatus::create(pan->getName());
            panFrame->setSearchAction(_operate->getAction());
            float time = 1.5;
            pan->runAction(panFrame->createAction(time, "end"));
            ActionHelper::delayFunc(time, this, [this](){
                    auto mixEndParticle = _operate->getNodeByName<ParticleSystemQuad*>("mixEndPraticle");
                    if(mixEndParticle != nullptr)
                        mixEndParticle->resetSystem();
                _operate->playNodeSound("mixEndPraticle");
                auto text = _operate->getNodeByName("text");
                ActionHelper::showBackInOut(text, text->getPosition(), ActionHelper::ShowDirection::show_from_top);
                text->setVisible(true);
                _eventDispatcher->setEnabled(true);
            });
            auto paths = WeddingFoodData::getBakeEndCookie();
            for(auto i = 0;i<paths.size();++i) {
                auto s = Sprite::create(paths[i]);
                s->setTag(i);
                auto touch = TouchNodeComponent::create();
                touch->addListners(ComponentTouchEnd, CC_CALLBACK_3(CookieBake::touchCookie,this));
                s->addComponent(touch);
                _operate->getNodeByName(StringUtils::format("cookie_%d",i))->addChild(s);
                s->runAction(Sequence::create(DelayTime::create(0.35*i+time), ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1), nullptr));
            }
          
        }),Hide::create(),  nullptr));
    }
}

void CookieBake::touchCookie(Node* ower,Component*, OperateListner* _lis){
    auto text = _operate->getNodeByName("text");
    if(text == nullptr)
        return;
    text->setName("tttttt");
    ActionHelper::hide(text, ActionHelper::ShowDirection::show_from_top);
    ower->getParent()->getParent()->reorderChild(ower->getParent(), 500);

    ower->setZOrder(1000);
    auto scale = ower->getParent()->getScale();
    CocosHelper::reAddNode(ower, this);
    ower->setScale(scale);
    ActionHelper::hide(_operate->getNodeByName("pan"), ActionHelper::ShowDirection::show_from_left);
    auto shapes = WeddingFoodData::getCookieShapes();
    WeddingFoodData::setShape(shapes[ower->getTag()]);
    ower->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.7, 1),MoveTo::create(0.7, ower->getParent()->convertToNodeSpace(Director::getInstance()->getVisibleSize()*.5)), nullptr),CallFunc::create([this,ower](){
      
        ActionHelper::delayFunc(0.6, this, [](){
            Director::getInstance()->replaceScene(DecorationLayer::createScene());
        });
    }), nullptr));
}