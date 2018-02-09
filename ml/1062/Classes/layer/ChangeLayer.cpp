//
//  ChangeLayer.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/23.
//
//

#include "ChangeLayer.h"
#include "DistanceToTarget.h"
#include "MoveForeverComponent.h"
#include "EventPromotLayer.h"
std::function<Scene*()> ChangeLayer::_changeLayer = nullptr;
bool ChangeLayer::isShow = false;
void ChangeLayer::onExitTransitionDidStart(){
    AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds,true);
    _changeLayer = nullptr;
    isShow = false;
    BaseStepLayer::onExitTransitionDidStart();
}

void ChangeLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    auto fryTool = _operate->getNodeByName("Fried0_3");
        SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    ActionHelper::showBackInOut(fryTool, fryTool->getPosition(), ActionHelper::ShowDirection::show_from_left,[this](){
        for(int i=0;i<4;i++){
            auto food = _operate->getNodeByName(StringUtils::format("food%d",i));
            food->setCascadeOpacityEnabled(false);
            food->runAction(Sequence::create(DelayTime::create(0.6*i),MoveBy::create(0, Vec2(0,600)), CallFunc::create([=](){
                food->setVisible(true);
                SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
            }),MoveTo::create(1,food->getPosition()),CallFunc::create([=](){
                auto oil = Sprite::create("png/4/oil0.png");
                oil->setPosition(food->getContentSize()*.5);
                food->addChild(oil);
                auto anima = Animation::create();
                anima->setDelayPerUnit(0.2);
                anima->addSpriteFrame(oil->getSpriteFrame());
                oil->runAction(Sequence::create(Animate::create(anima),DelayTime::create(0.2), FadeTo::create(0, 0), nullptr));
                if(i == 0){
                     _loopSound = SoundPlayer::getInstance()->playEffectLoop("sound/general/fry donut.mp3");
                }
                food->getChildByName("fry")->runAction(FadeTo::create(3, 255));
            }),FadeTo::create(3, 0),CallFunc::create([=](){
                if(i==3){
                    SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
                    _loopSound = -1;
                    for(int i=0;i<4;i++){
                        auto food = _operate->getNodeByName(StringUtils::format("food%d",i));
                        food->runAction(RepeatForever::create(ActionHelper::createShakeAction()));
                        food->runAction(RepeatForever::create(ActionHelper::createShakeAction()));
                    
                        
                    }
                }
            }),  DelayTime::create(0.4), CallFunc::create([=](){
               
             
                if(i == 3){
                      SoundPlayer::getInstance()->playEffect("sound/general/fry_ok.mp3");
                    auto frame = _operate->getNodeByName("frame");
                    auto frmeUp = _operate->getNodeByName("frameup");
                    for(int i=0;i<4;i++){
                             auto food = _operate->getNodeByName(StringUtils::format("food%d",i));
                        food->setZOrder(-1);
                        CocosHelper::reAddNode(food, frmeUp);
                    }
                    frame->runAction(MoveBy::create(0.7, Vec2(0,75)));
                    frmeUp->runAction(Sequence::create(MoveBy::create(0.7, Vec2(0,75)),CallFunc::create([=](){
                        for(int i=0;i<4;i++){
                            auto food = _operate->getNodeByName(StringUtils::format("food%d",i));
                            food->stopAllActions();
                        }
                        }),DelayTime::create(0.7),CallFunc::create([=](){
                      
                        if(_changeLayer != nullptr){
                                 Director::getInstance()->replaceScene(TransitionFade::create(0.8,_changeLayer(),Color3B::WHITE));
                        }
                    }),  nullptr));
                }
            }), nullptr));
        }
    });
    fryTool->setVisible(true);
    
}

void ChangeLayer::loading(){
    if(_changeLayer != nullptr){
        Director::getInstance()->replaceScene(_changeLayer());
    }
    
//    if(isShow)
//        return;
//    AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds,false);
//    isShow = true;
//    auto _scene = Scene::create();
//    _scene->addChild(ChangeLayer::create());
//    Director::getInstance()->replaceScene(_scene);
}

bool ChangeLayer::init(){
    BaseStepLayer::init();
    soundIndex = 0;
    auto _EventPromotLayer = EventPromotLayer::create();
    _EventPromotLayer->setCallbakc([](){});
    addChild(_EventPromotLayer,50);
    _operate = StudioLayer::create("loading.csb");
    addChild(_operate);
    _operate->getNodeByName("Fried0_3")->setVisible(false);
    for(int i=0;i<4;i++)
        _operate->getNodeByName(StringUtils::format("food%d",i))->setVisible(false);
    return true;
}