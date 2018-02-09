//
//  CookieChooseFlavor.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#include "CookieChooseFlavor.h"
#include "FrameStatus.h"
#include "WeddingFoodData.h"
#include "CookieIngredients.h"
#include "ChangeLoading.h"
REGIST_SECENE(CookieChooseFlavor);
CookieChooseFlavor::CookieChooseFlavor():moveDistance(800)
,chooseParticle(nullptr){
    
}

CookieChooseFlavor::~CookieChooseFlavor(){
    CC_SAFE_RELEASE_NULL(chooseParticle);
}

bool CookieChooseFlavor::init(){
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("CookieChooseFlavor.csb");
        addChild(_operate);
        _operate->getNodeByName<ui::ScrollView*>("ScrollView_1")->setScrollBarOpacity(0);
        chooseParticle = _operate->getNodeByName<ParticleSystemQuad*>("particle");
        chooseParticle->retain();
        chooseParticle->stopSystem();
        return true;
    }
    return false;
}

void CookieChooseFlavor::onEnter(){
    BaseStepLayer::onEnter();
    for(auto i =0;i<8;i++){
        auto node = _operate->getNodeByName(StringUtils::format("cookie_%d",i));
        if(node != nullptr){
            posY = node->getPositionY();
            node->runAction(MoveBy::create(0, Vec2(-moveDistance,0)));
        }
        auto node2 = _operate->getNodeByName(StringUtils::format("cookie_%d1",i));
        if(node2 != nullptr)
            node2->runAction(MoveBy::create(0, Vec2(-moveDistance,0)));
    }
}

void CookieChooseFlavor::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    _eventDispatcher->setEnabled(false);
    for(auto i =0;i<8;i++){
        auto node = _operate->getNodeByName(StringUtils::format("cookie_%d",i));
        if(node != nullptr)
            node->runAction(MoveBy::create(0.8, Vec2(moveDistance,0)));
        auto node2 = _operate->getNodeByName(StringUtils::format("cookie_%d1",i));
        if(node2 != nullptr)
            node2->runAction(MoveBy::create(0.8, Vec2(moveDistance,0)));
    }
    ActionHelper::delayFunc(0.8, this, [this]{
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
        auto text = _operate->getNodeByName("text");
        SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
        ActionHelper::showBackInOut(text, text->getPosition(), ActionHelper::ShowDirection::show_from_top);
        text->setVisible(true);
    });
}

void CookieChooseFlavor::touchEnd(ui::Widget* widget){
    if(widget->getName() == "btn_next") {
        ChangeLoading::loading([](){
            Director::getInstance()->replaceScene(TransitionFade::create(0.8,CookieIngredients::createScene(),Color3B::WHITE));
        });
    }else
    for(auto i =0;i<8;i++){
        auto str = StringUtils::format("cookie_%d",i);
        if(str.compare(widget->getName()) == 0){
                _operate->playNodeSound("cookie_0");
            _operate->getNodeByName("btn_next")->setVisible(true);
             _operate->getNodeByName("btn_next_light")->setVisible(true);
            for(auto i =0;i<8;i++){
                auto node = _operate->getNodeByName(StringUtils::format("cookie_%d",i));
                node->stopAllActions();
                node->setPositionY(posY);
            }
            widget->runAction(RepeatForever::create(Sequence::create(JumpBy::create(0.8, Vec2(0,0), 30, 2) , DelayTime::create(2),nullptr)));
            WeddingFoodData::setCookieFlavor(i);
            chooseParticle->removeFromParent();
            widget->addChild(chooseParticle);
            chooseParticle->resetSystem();
            break;
        }
    }
}
