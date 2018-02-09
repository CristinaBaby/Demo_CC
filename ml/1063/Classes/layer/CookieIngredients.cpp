//
//  CookieIngredients.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/25.
//
//

#include "CookieIngredients.h"
#include "DragFall.h"
#include "WeddingFoodData.h"
#include "DrawLine.h"
#include "BezierCreator.h"
#include "CookieRoll.h"
#include "ChangeLoading.h"
REGIST_SECENE(CookieIngredients);
CookieIngredients::CookieIngredients(){
    
}

bool CookieIngredients::init(){
    if(BaseMakeLayer::init()) {
        inintNecessary("CookieIngredients.csb", WeddingFoodData::getMixingCookie());
        setMixAction(RepeatForever::create(RotateBy::create(1, 90)));
        vanillaBottle = StudioLayer::createNoLayout(WeddingFoodData::getCookieVanillaBottle());
        _operate->getNodeByName("optionLayer")->addChild(vanillaBottle);
        auto node = _operate->getNodeByName("vanillaMix");
        node->removeAllChildren();
        node->addChild(StudioLayer::createNoLayout(WeddingFoodData::getCookieVanilla()));
        _prompt = _operate->getNodeByName<Sprite*>("finger");
        return true;
    }
    return false;
}

void CookieIngredients::fallEnd(Node*node,Component* fall, OperateListner* _lis){
    if(count == 2) {
        string addNames[] = {"flour","egg","butter"};
        for(int i=0;i<sizeof(addNames)/sizeof(*addNames);i++){
            showThing(addNames[i],0.35*(i+1));
        }
    }
    BaseMakeLayer::fallEnd(node, fall, _lis);
    float time = 0.4;
    if(node->getName() == "milk")
        time = 1.4;
    else if(node->getName() == "butter")
        ActionHelper::hide(_operate->getNodeByName("butter_plate"), ActionHelper::ShowDirection::show_from_right);
    
    ActionHelper::delayFunc(time, this, [node]{
        node->runAction(RotateTo::create(0.6, 0));
        ActionHelper::hide(node, ActionHelper::ShowDirection::show_from_right);
    });
}

void CookieIngredients::onEnterTransitionDidFinish(){
    BaseMakeLayer::onEnterTransitionDidFinish();
     SoundPlayer::getInstance()->playBackGroundMusic("sound/make stage.mp3");
    string addNames[] = {"milk","vanilla","whitesugar"};
    for(int i=0;i<sizeof(addNames)/sizeof(*addNames);i++){
        showThing(addNames[i],0.35*(i+1));
    }
}

void CookieIngredients::showThing(const string& name,float delayTime){
    auto _node = _operate->getNodeByName(name);
    auto drag = createDraOperate(_node);
    drag->setEnabled(false);
    ActionInterval* inBowAction;
    
    if(name.compare("vanilla") == 0){
        drag->getFrameStatus()->setSearchAction(vanillaBottle->getAction());
        if(_node->getChildByName("particle"))
            inBowAction = FadeTo::create(1.1, 255);
        else
            inBowAction = Sequence::create(FadeTo::create(0, 255), ScaleTo::create(0, 0.3),MoveBy::create(0, Vec2(0,80)),MoveBy::create(0.4, Vec2(0,-80)),ScaleTo::create(0.15, 1), nullptr);
    }else if(name.compare("egg") == 0){
       
        inBowAction = Sequence::create(CallFunc::create([_node](){
            auto animaion = Animation::create();
            animaion->setDelayPerUnit(0.3);
            for(int i=0;i<4;++i)
                animaion->addSpriteFrameWithFile(StringUtils::format("png/cookies/2/bowl_egg%d.png",i));
            _node->getChildByName("noshade")->runAction(Animate::create(animaion));
            //_node->setVisible(false);
        }),DelayTime::create(1.2), FadeTo::create(0, 255),CallFunc::create([_node](){_node->setVisible(false);}), nullptr);
    }
    else if(name.compare("butter") == 0) {
        inBowAction = Sequence::create(CallFunc::create([_node](){
            _node->runAction(Sequence::create(MoveBy::create(0.3, Vec2(0,-50)),Hide::create(), nullptr));
        }),DelayTime::create(0.3), FadeTo::create(0, 255),CallFunc::create([drag](){
            drag->playSound();
        }), nullptr);
    }
    else{
        float time = 1.6;
        if(name.compare("whitesugar") == 0)
            time = 1.1;
        inBowAction = FadeTo::create(time, 255);
    }
    drag->setInbow(_operate->getNodeByName(name+"Inbowl"), Sequence::create(CallFunc::create([_node]{
        vector<std::string> checkNames = {"milk","whitesugar","flour"};
        for(auto checkName:checkNames){
            if(_node->getName().compare(checkName) == 0){
                _node->runAction(MoveBy::create(0.8, Vec2(0, 90)));
                break;
            }
        }
    }),inBowAction, nullptr));
    ActionHelper::delayFunc(delayTime, this, [drag,_node,this](){
        if(_node->getName() == "butter"){
            auto _plate = _operate->getNodeByName("butter_plate");
            ActionHelper::showBezier(_plate, _plate->getPosition(), ActionHelper::ShowDirection::show_from_right);
            _plate->setVisible(true);
        }
        SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
        ActionHelper::showBezier(_node, _node->getPosition(), ActionHelper::ShowDirection::show_from_right,[drag,_node,this](){
            drag->setEnabled(true);
            if(_node->getName() == "whitesugar") {
                if(_prompt != nullptr){
                    vector<Vec2> points = {
                        Vec2(525.909546,837.534180),
                        Vec2(350.885559,729.899170),
                        Vec2(352.757507,728.963135),
                        Vec2(334.974274,546.451538)
                    };
                    
                    auto se = Sequence::create(CallFunc::create([this](){_operate->getNodeByName("down")->setVisible(true);}), FadeTo::create(0, 255), BezierCreator::createAction(points, 1),CallFunc::create([this](){_operate->getNodeByName("down")->setVisible(false);}),FadeTo::create(0, 0), DelayTime::create(2.7f),nullptr);
                    _prompt->runAction(RepeatForever::create(se));
                    _operate->getNodeByName("down")->setVisible(true);
                }
            }
        });
        _node->setVisible(true);
        SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    });
    
}

void CookieIngredients::stopPrompt(){
    BaseMakeLayer::stopPrompt();
    _operate->getNodeByName("down")->setVisible(false);
}

void CookieIngredients::mixEnd(Node*node,Component* c, OperateListner* _lis){
    BaseMakeLayer::mixEnd(node, c, _lis);
    auto bowl = _operate->getNodeByName("bowl");
    bowl->runAction(ScaleTo::create(0.6, 1.2));
    ActionHelper::delayFunc(1.4, this, [](){
    
             Director::getInstance()->replaceScene(TransitionFade::create(0.8,CookieRoll::createScene(),Color3B::WHITE));
    });
}