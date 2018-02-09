//
//  FruitTartIngredients.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/25.
//
//

#include "FruitTartIngredients.h"
#include "DragFall.h"
#include "FruitTartPour.h"
#include "BezierCreator.h"
#include "ChangeLoading.h"
REGIST_SECENE(FruitTartIngredients);
FruitTartIngredients::FruitTartIngredients(){
    spoonRote = -20;
}

bool FruitTartIngredients::init(){
    if(BaseMakeLayer::init()) {
        vector<string> mixFile;
        for(int i=0;i<=3;i++)
            mixFile.push_back(StringUtils::format("png/Fruit Tart/1/mix_%d.png",i));
        inintNecessary("FruitTartIngredients.csb", mixFile);
        _prompt = _operate->getNodeByName<Sprite*>("finger");
        
        return true;
    }
    return false;
}

void FruitTartIngredients::fallEnd(Node*node,Component* fall, OperateListner* _lis){
    if(count == 3) {
        string addNames[] = {"flour","vanilla","martha"};
        for(int i=0;i<sizeof(addNames)/sizeof(*addNames);i++){
            showThing(addNames[i],0.35*(i+1));
        }
    }
    BaseMakeLayer::fallEnd(node, fall, _lis);
    float time = 0.4;
    if(node->getName() == "milk")
        time = 1.4;
    ActionHelper::delayFunc(time, this, [node]{
        node->runAction(RotateTo::create(0.6, 0));
        ActionHelper::hide(node, ActionHelper::ShowDirection::show_from_right);
    });
//    if(count == addEndNum){
//        ActionHelper::delayFunc(time, this, [this]{
//            _operate->getNodeByName("bowlUp")->runAction(ScaleBy::create(0.4, 1.2));
//            _operate->getNodeByName("bowl")->runAction(ScaleBy::create(0.4, 1.2));
//        });
//        
//    }
}


void FruitTartIngredients::onEnterTransitionDidFinish(){
    BaseMakeLayer::onEnterTransitionDidFinish();
     SoundPlayer::getInstance()->playBackGroundMusic("sound/make stage.mp3");
    string addNames[] = {"milk","egg0","egg1","sugar"};
    for(int i=0;i<sizeof(addNames)/sizeof(*addNames);i++){
        showThing(addNames[i],0.35*(i+1));
    }
}

void FruitTartIngredients::showThing(const string& name,float delayTime){
    auto _node = _operate->getNodeByName(name);
    auto drag = createDraOperate(_node);
    drag->setEnabled(false);
    ActionInterval* inBowAction;
    if(name.compare("vanilla") == 0){
        inBowAction = Sequence::create(FadeTo::create(0, 255), ScaleTo::create(0, 0.3),MoveBy::create(0, Vec2(0,80)),MoveBy::create(0.4, Vec2(0,-80)),ScaleTo::create(0.15, 1), nullptr);
    }else if(name.find("egg") != string::npos) {
        auto animaion = Animation::create();
        animaion->setDelayPerUnit(0.3);
        for(int i=0;i<3;++i)
            animaion->addSpriteFrameWithFile(StringUtils::format("png/cookies/2/bowl_egg%d.png",i));
        animaion->addSpriteFrame(_operate->getNodeByName<Sprite*>((name+"Inbowl"))->getSpriteFrame());
        inBowAction = Sequence::create(CallFunc::create([_node](){
            _node->setVisible(false);
        }),FadeTo::create(0, 255),Animate::create(animaion), nullptr);
    }
    else{
        float time = 1.6;
        if(name.compare("sugar") == 0)
            time = 1.1;
        inBowAction = FadeTo::create(time, 255);
    }
    drag->setInbow(_operate->getNodeByName(name+"Inbowl"), Sequence::create(CallFunc::create([_node]{
        vector<std::string> checkNames = {"milk","sugar","flour","martha"};
        for(auto checkName:checkNames){
            if(_node->getName().compare(checkName) == 0){
                _node->runAction(MoveBy::create(0.8, Vec2(0, 90)));
                break;
            }
        }
    }),inBowAction, nullptr));
    ActionHelper::delayFunc(delayTime, this, [drag,_node,this](){
        SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
        ActionHelper::showBezier(_node, _node->getPosition(), ActionHelper::ShowDirection::show_from_right,[drag,_node,this](){
            drag->setEnabled(true);
            if(_node->getName() == "sugar") {
                if(_prompt != nullptr){
                    vector<Vec2> points = {
                        Vec2(429.505951,544.579590),
                        Vec2(203.940338,556.747070),
                        Vec2(245.122467,578.274048),
                        Vec2(172.117798,360.196167),
                    };
                    
                    auto se = Sequence::create(CallFunc::create([this](){_operate->getNodeByName("down")->setVisible(true);}), FadeTo::create(0, 255), BezierCreator::createAction(points, 1),CallFunc::create([this](){_operate->getNodeByName("down")->setVisible(false);}),FadeTo::create(0, 0), DelayTime::create(2.7f),nullptr);
                    _prompt->runAction(RepeatForever::create(se));
                }
            }
        });
        _node->setVisible(true);
        SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    });
    
}

void FruitTartIngredients::mixEnd(Node*node,Component* c, OperateListner* _lis){
    BaseMakeLayer::mixEnd(node, c, _lis);
    ActionHelper::delayFunc(0.8, this, [](){
     
              Director::getInstance()->replaceScene(FruitTartPour::createScene());
       
    });
}
void FruitTartIngredients::stopPrompt(){
    BaseMakeLayer::stopPrompt();
    _operate->getNodeByName("down")->setVisible(false);
}
