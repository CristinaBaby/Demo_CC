//
//  FryFood.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang on 16/1/23.
//
//

#include "FryFood.h"
#include "GameFood.h"
#include "TouchLineComonent.h"
#include "TouchMoveComponent.h"
#include "DecorationLayer.h"
#include "ParticleLayer.h"
bool FryFood::init(){
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("FryFood.csb");
        addChild(_operate);
        bubble = _operate->getNodeByname<ParticleSystemQuad*>("bubble");
        bubble->stopSystem();
        _operate->getNodeByname<ParticleSystemQuad*>("smoke")->stopSystem();
        _operate->getNodeByName("plate")->setVisible(false);
        _operate->getNodeByName("putPlate")->setVisible(false);
        _prompt = _operate->getNodeByname<Sprite*>("finger2");
        
        return true;
    }
    return false;
}

void FryFood::touchEnd(ui::Widget* widget){
    if(widget->getName() == "switch"){
        _operate->getNodeByName("lamp1")->setVisible(true);
        widget->stopAllActions();
        dynamic_cast<ui::Button*>(widget)->setEnabled(false);
        stopPrompt();
        bubble->resetSystem();
        _operate->getNodeByname<ParticleSystemQuad*>("smoke")->resetSystem();
        ActionHelper::delayFunc(2, this,CC_CALLBACK_0(FryFood::showPlate, this));
    }
}

void FryFood::moveEnd(Node* node,Component* com, OperateListner* _lis){
    node->setVisible(false);
    com->setEnabled(false);
    stopPrompt();
    auto newFood = GameFood::getInstance()->createFryEndInplate();
    auto putPlate = _operate->getNodeByName("putPlate");
    newFood->setPosition(putPlate->getContentSize()*.5);
    putPlate->addChild(newFood);
    putPlate->getParent()->reorderChild(putPlate, 20);
    _operate->getNodeByName("Fried0_3")->runAction(MoveBy::create(0.7, Vec2(0,-500)));
    
    putPlate->runAction(Sequence::create(Spawn::create(MoveTo::create(0.7, Vec2(480,300)),ScaleTo::create(0.7, 1.3), nullptr),CallFunc::create([this](){
        this->addChild(ParticleLayer::create(),30);
        ActionHelper::delayFunc(2.25, this, [](){
            Director::getInstance()->replaceScene(TransitionFade::create(0.8, DecorationLayer::createScene(),Color3B::WHITE));
        });
    }), nullptr));
}

void FryFood::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    foodCotainerPos = _operate->getNodeByName("foodContainer")->getPosition();
    palatePos = _operate->getNodeByName("plate")->getPosition();
    _prompt->setPosition(_operate->getPosinWord("switch")+Vec2(0,35));
    _prompt->runAction(RepeatForever::create(ActionHelper::createUpDownPrompt()));
}

void FryFood::showPlate(){
    auto continer = _operate->getNodeByName("foodContainer");
    CocosHelper::reAddNode(continer, _operate->getNodeByName("frame"));
    continer->setPosition(foodCotainerPos);
    
    auto plate = _operate->getNodeByName("plate");
    plate->removeAllChildren();
    plate->removeAllComponents();
    plate->setRotation(0);
    ActionHelper::showBezier(plate, palatePos, ActionHelper::ShowDirection::show_from_right,[=](){
        _prompt = _operate->getNodeByname<Sprite*>("finger");
        ActionHelper::runBezierMoveHideAction(_prompt, _operate->getPosinWord("plate"), _operate->getPosinWord("frame"));
        auto com  = TouchMoveComponent::create();
        com->setTarget(LQRect::create(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit"))));
        com->setMoveType(MoveComponentType::kTypeBack);
        plate->addComponent(com);
        com->addListners(ComponentTouchEnd, [=](Node* node,Component* c, OperateListner* _lis){
           
            c->setEnabled(false);
            this->stopPrompt();
            node->getParent()->reorderChild(node, 0);
            node->runAction(Sequence::create(Spawn::create(MoveTo::create( 0.3,Vec2(229.29,494.42)),RotateTo::create(0.3, 40.52), nullptr), CallFunc::create([=](){
              
                auto _up = _operate->getNodeByName("frameup");
                _up->getParent()->reorderChild(_up, 2);
                _up = _operate->getNodeByName("Fried1_4");
                _up->getParent()->reorderChild(_up, 2);
                auto foods = GameFood::getInstance()->getFoods();
                auto inOilFood = GameFood::getInstance()->getInOilFood();
                ActionHelper::delayFunc(0.51, this, [=](){
                    if(_loopSound == -1)
                        _loopSound = SoundPlayer::getInstance()->playEffectLoop("sound/general/fry donut.mp3");
                    _operate->getNodeByName("oil0")->setVisible(true);
                    ActionHelper::delayFunc(0.2, this, [this](){
                        _operate->getNodeByName("oil0")->setVisible(false);
                        _operate->getNodeByName("oil1")->setVisible(true);
                    });
                    ActionHelper::delayFunc(0.4, this, [this](){
                        _operate->getNodeByName("oil0")->setVisible(false);
                        _operate->getNodeByName("oil1")->setVisible(false);
                    });
                    for(int i=0;i<foods.size();i++){
                        auto _food = foods.at(i);
                        _food->runAction(Sequence::create(FadeTo::create(5, 125),FadeTo::create(0, 0),nullptr));
                        auto frame = _operate->getNodeByName("frame");
                        auto pos = frame->getPosition();
                        auto oil = _operate->getNodeByName("oil_2");
                        auto oilPos = oil->getPosition();
                        _food->getChildByName("fry")->runAction(Sequence::create(FadeTo::create(5, 255),CallFunc::create([=](){
                            if(i==0) {
                                SoundPlayer::getInstance()->playEffect("sound/general/fry_ok.mp3");
                                this->stopPrompt();
                                continer->runAction(RepeatForever::create(ActionHelper::createShakeAction()));
                                ActionHelper::runMoveHideAction(_prompt, Vec2(820,340), Vec2(820,340+100));
                                
                                auto touchLine = TouchLineComonent::create();
                                touchLine->setDirection(Vec2(0,1));
                                touchLine->addListners(ComponentTouchMove,[=]( Node*,Component* com, OperateListner* _lis){
                                    
                                    Vec2 deta = dynAny_cast<Vec2>(_lis->getData());
                                    auto newPos = frame->getPosition()+deta;
                                    if(newPos.y<pos.y)
                                        newPos = pos;
                                    frame->setPosition(newPos);
                                    auto frameUp = _operate->getNodeByName("frameup");
                                    frameUp->setPosition(newPos);
                                    auto newOilPos = oil->getPosition()-deta*0.1;
                                    if(newOilPos.y>oilPos.y)
                                        newOilPos=  oilPos;
                                    oil->setPosition(newOilPos);
                                    if((newPos.y-pos.y)>100){
                                        if(_loopSound != -1){
                                            SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
                                            _loopSound = -1;
                                        }
                                        continer->stopAllActions();
                                        com->setEnabled(false);
                                        for(auto _child:foods)
                                            _child->getChildByName("fry")->stopAllActions();
                                        this->stopPrompt();
                                        bubble->stopSystem();
                                        _operate->getNodeByname<ParticleSystemQuad*>("smoke")->stopSystem();
                                        frame->getParent()->reorderChild(frame, 3);
                                        frameUp->getParent()->reorderChild(frameUp, 3);
                                        auto putPlate = _operate->getNodeByName("putPlate");
                                        ActionHelper::showBackInOut(putPlate, putPlate->getPosition(),ActionHelper::ShowDirection::show_from_right);
                                        putPlate->setVisible(true);
                                        auto tool = _operate->getNodeByName("Fried0_3");
                                        tool->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -200)),CallFunc::create([=](){
                                            auto putPlate = _operate->getNodeByName("putPlate");
                                            auto moveOperate = TouchMoveComponent::create();
                                            moveOperate->setMoveType(MoveComponentType::kTypeBack);
                                            auto rect = CocosHelper::getNodeRectInWorld(putPlate);
                                            rect.origin = rect.origin - Vec2(0,30);
                                            rect.size = rect.size+Size(0,60);
                                            moveOperate->setTarget(LQRect::create(rect));
                                            moveOperate->addListners(ComponentTouchEnd,CC_CALLBACK_3(FryFood::moveEnd,this));
                                            _operate->getNodeByName("foodContainer") ->addComponent(moveOperate);
                                            ActionHelper::runBezierMoveHideAction(_prompt, _operate->getPosinWord("Fried0_3"), _operate->getPosinWord("putPlate"));
                                            CocosHelper::reAddNode(frameUp, frame);
                                        }), nullptr));
                                    }
                                });
                                frame->addComponent(touchLine);
                            }
                        }),DelayTime::create(4),CallFunc::create([=]{
                            this->stopPrompt();
                            frame->removeAllComponents();
                            frame->setPosition(pos);
                            _operate->getNodeByName("frameup")->setPosition(pos);
                            oil->setPosition(oilPos);
                            _food->getChildByName("fryFailed")->runAction(FadeTo::create(0.6, 255));
                        }),FadeTo::create(0.6, 155),CallFunc::create([=](){
                            _food->getChildByName("fry")->setOpacity(0);
                            if(i==0){
                                SoundPlayer::getInstance()->playEffect("sound/general/fry failed.mp3");
                                continer->stopAllActions();
                                CocosHelper::reAddNode(continer, _operate->getNodeByName("oil_2"));
                                auto _smoke = Sprite::create("png/fryFail/smoke.png");
                                _smoke->setPosition(continer->convertToWorldSpace(continer->getContentSize()*.5)+Vec2(0,50));
                                addChild(_smoke,20);
                                _smoke->setScale(1, 0.5);
                                _smoke->setOpacity(0);
                                _smoke->runAction(Sequence::create(DelayTime::create(0.7),FadeTo::create(0,255),ScaleTo::create(2, 1,1),FadeTo::create(0.3, 0),CallFunc::create([=](){
                                    continer->removeAllComponents();
                                    continer->removeAllChildren();
                                    _operate->getNodeByName("black")->setVisible(true);
                                    this->showPlate();
                                }), RemoveSelf::create() ,nullptr));
                                _smoke->runAction(Sequence::create(DelayTime::create(2),MoveBy::create(1.1, Vec2(0,400)),nullptr));
                                
                            }
                        }), nullptr));
                    }
                    
                    
                    ActionHelper::hide(_operate->getNodeByName("plate"), ActionHelper::ShowDirection::show_from_left,nullptr,false);
                });
                
                
                
                for(int i=0;i<foods.size();i++){
                    auto endPos = foods.at(i)->getParent()->convertToNodeSpace(_operate->getNodeByName("positionnode")->convertToWorldSpace(inOilFood.at(i)->getPosition()));
                    foods.at(i)->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, endPos),RotateTo::create(0.5, inOilFood.at(i)->getRotationX(),inOilFood.at(i)->getRotationY()), nullptr),CallFunc::create([=](){
                        CocosHelper::reAddNode(foods.at(i), _operate->getNodeByName("foodContainer"));
                        
                    }), nullptr));
                }
            }), nullptr));
        });
    });
    plate->setVisible(true);
    auto food = GameFood::getInstance()->createFryFood();
    food->setPosition(plate->getContentSize()*.5);
    plate->addChild(food);
}
