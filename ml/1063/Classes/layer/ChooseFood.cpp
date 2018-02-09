//
//  ChooseFood.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#include "ChooseFood.h"
#include "FrameStatus.h"
#include "CookieChooseFlavor.h"
#include "FruitTartIngredients.h"
#include "WeddingFoodData.h"
#include "SceneManager.h"
REGIST_SECENE(ChooseFood);
ChooseFood::ChooseFood():moveDistance(500)
,chooseParticle(nullptr)
,chooseIndex(0){
    
}

ChooseFood::~ChooseFood(){
    CC_SAFE_RELEASE_NULL(chooseParticle);
}

bool ChooseFood::init(){
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("ChooseFood.csb");
        addChild(_operate);
        _operate->getNodeByName<ui::ScrollView*>("ScrollView_1")->setScrollBarOpacity(0);
        chooseParticle = _operate->getNodeByName<ParticleSystemQuad*>("particle");
        chooseParticle->retain();
        chooseParticle->stopSystem();
        return true;
    }
    return false;
}

void ChooseFood::onEnter(){
    BaseStepLayer::onEnter();
    for(auto i =0;i<8;i++){
        auto node = _operate->getNodeByName(StringUtils::format("food_%d",i));
        if(node != nullptr){
            posY.push_back(node->getPositionY());
            node->runAction(MoveBy::create(0, Vec2(-moveDistance,0)));
        }
        auto node2 = _operate->getNodeByName(StringUtils::format("food_%d1",i));
        if(node2 != nullptr)
            node2->runAction(MoveBy::create(0, Vec2(-moveDistance,0)));
    }
}

void ChooseFood::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
      SoundPlayer::getInstance()->playBackGroundMusic("sound/start page.mp3");
    _eventDispatcher->setEnabled(false);
    for(auto i =0;i<8;i++){
        auto node = _operate->getNodeByName(StringUtils::format("food_%d",i));
        if(node != nullptr)
            node->runAction(MoveBy::create(0.8, Vec2(moveDistance,0)));
        auto node2 = _operate->getNodeByName(StringUtils::format("food_%d1",i));
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

void ChooseFood::touchEnd(ui::Widget* widget){
    if(widget->getName() == "btn_next") {
        switch (chooseIndex) {
            case 0:
                SceneManager::getInstance()->enterSelectScene();
                break;
            case 1:
                  Director::getInstance()->replaceScene(CookieChooseFlavor::createScene());
                break;
            case 2:
                 Director::getInstance()->replaceScene(FruitTartIngredients::createScene());
                break;
        }
          SoundPlayer::getInstance()->playBackGroundMusic("sound/make stage.mp3");
        WeddingFoodData::setFoodIndex(chooseIndex);
    }else
    for(auto i =0;i<3;i++){
        auto str = StringUtils::format("food_%d",i);
        if(str.compare(widget->getName()) == 0){
             _operate->playNodeSound("food_0");
             _operate->getNodeByName("btn_next_light")->setVisible(true);
            _operate->getNodeByName("btn_next")->setVisible(true);
            for(auto i =0;i<3;i++){
                auto node = _operate->getNodeByName(StringUtils::format("food_%d",i));
                node->stopAllActions();
                node->setPositionY(posY[i]);
            }
            widget->runAction(RepeatForever::create(Sequence::create(JumpBy::create(0.8, Vec2(0,0), 30, 2) , DelayTime::create(2),nullptr)));
            chooseIndex = i;
            chooseParticle->removeFromParent();
            widget->addChild(chooseParticle);
            chooseParticle->resetSystem();
            break;
        }
    }
}
