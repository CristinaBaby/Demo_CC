//
//  CoatePowder.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang on 16/1/23.
//
//

#include "CoatePowder.h"
#include "GameFood.h"
#include "FryFood.h"
#include "ParticleLayer.h"

bool CoatePowder::init(){
    if(BaseStepLayer::init()) {
        count =0;
        moveCount = 0;
        _operate = StudioLayer::create("CoatedPowder.csb");
        addChild(_operate);
        _prompt = _operate->getNodeByname<Sprite*>("finger");
        
        return true;
    }
    return false;
}
void CoatePowder::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    auto plate0_14 = _operate->getNodeByName("plate0_14");
    ActionHelper::showBackInOut(plate0_14, plate0_14->getPosition(), ActionHelper::ShowDirection::show_from_right);
    plate0_14->setVisible(true);
    auto plate = _operate->getNodeByName("plate");
    ActionHelper::showBackInOut(plate, plate->getPosition(), ActionHelper::ShowDirection::show_from_left,[this](){
        ActionHelper::runBezierMoveHideAction(_prompt, _operate->getPosinWord("food0"), _operate->getPosinWord("plate0_14"));
        for(auto food:GameFood::getInstance()->getFoods()){
            auto move = TouchMoveComponent::create();
            move->setMoveType(MoveComponentType::kTypeBack);
            auto rect = CocosHelper::getNodeRectInWorld(_operate->getNodeByName("plate0_14"));
            rect.origin = rect.origin+Vec2(50, 30);
            rect.size = rect.size - Size(100,60);
            move->setTarget(LQRect::create(rect));
            move->addListners(ComponentTouchMove,[this,move](Node* node,Component*,OperateListner*lis){
                if(move->checkInlimit()){
                    CocosHelper::reAddNode(node, _operate->getNodeByName("optionLayer"));
                  
                    this->stopPrompt();
                    if(moveCount== 0){
                    _prompt->setPosition(node->convertToWorldSpace(node->getContentSize()*.5));
                    auto action = RepeatForever::create(Repeat::create(Sequence::create(MoveBy::create(0.6, Vec2(80,0)),MoveBy::create(0.6, Vec2(-80,0)), nullptr),2));
                    _prompt->runAction(action);
                    _prompt->setVisible(true);
                    }
                    move->setMoveType(MoveComponentType::kTypeInRect);
                    lis->setCallBack(CC_CALLBACK_3(CoatePowder::coate, this));
                    auto end = [this](Node* node,Component*, OperateListner* _lis){
                        _actionManager->pauseTarget(node);
                        _actionManager->pauseTarget(node->getChildByName("dipping"));
                    };
                    move->addListners(ComponentTouchEnd,end);
                    move->addListners(ComponentTouchCancle, end);
                }
            });
            food->addComponent(move);
        }
    });
    plate->setVisible(true);
    auto food = GameFood::getInstance()->createBeforePowder();
    food->setPosition(plate->getContentSize()*.5);
    plate->addChild(food);
}

 void CoatePowder::coate(Node* node,Component* c,OperateListner* _lis){
  
     if(moveCount>40)
         this->stopPrompt();
     else{
         moveCount++;
          _prompt->setPosition(node->convertToWorldSpace(node->getContentSize()*.5));
     }
     const int actiontag = 10;
     auto action = node->getActionByTag(actiontag);
     if(action == nullptr){
           node->runAction(RepeatForever::create(ActionHelper::createRoteAction()));
         action = Sequence::create(FadeTo::create(3, 155),CallFunc::create([=](){
             c->setEnabled(false);
             node->stopAllActions();
             node->setOpacity(0);
             auto particle = ParticleSystemQuad::create("particles/start_1.plist");
             particle->setPosition(node->convertToWorldSpace(node->getContentSize()*.5));
             addChild(particle,10);
             Vec2 endPos(900-node->getContentSize().width*.5,520-node->getContentSize().height*.5);
             endPos.y = endPos.y - 341.08/GameFood::getInstance()->getFoods().size()*count;
              SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
             node->runAction(Sequence::create(ScaleTo::create(0.3, 1.1),ScaleTo::create(0.3, 1),Spawn::create(RotateTo::create(0.3, 1),MoveTo::create(0.3, endPos), nullptr), CallFunc::create([=]{
                 CocosHelper::reAddNode(node, _operate->getNodeByName("plate0_14"));
                 count++;
                 if(count == GameFood::getInstance()->getFoods().size()){
                     ActionHelper::hide(_operate->getNodeByName("plate"), ActionHelper::ShowDirection::show_from_left);
                     _operate->getNodeByName("plate0_14")->runAction(MoveBy::create(0.3, Vec2(-140,0)));
                     auto foods = GameFood::getInstance()->getFoods();
                     auto pos = GameFood::getInstance()->getCoateEndPos();
                     for(int i=0;i<foods.size();i++){
                         auto dount = foods.at(i);
                         dount->runAction(Sequence::create(DelayTime::create(0.1*i),MoveTo::create(0.8, pos.at(i)),CallFunc::create([=](){
                             if(i==foods.size()-1){
                                 for(int j=0;j<foods.size();j++){
                                     auto dount = foods.at(j);
                                     dount->runAction(Sequence::create(DelayTime::create(0.35*j), CallFunc::create([](){
                                         SoundPlayer::getInstance()->playEffect("sound/general/cut tool fall.mp3");
                                     }), ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1),CallFunc::create([=](){
                                         if(j== foods.size() -1){
                                             
                                             addChild(ParticleLayer::create(),30);
                                             ActionHelper::delayFunc(2.25, this, [](){
                                                 Director::getInstance()->replaceScene(TransitionFade::create(0.8, FryFood::createScene(),Color3B::WHITE));
                                             });
                                         }
                                     }), nullptr));
                                 }
                                 
                             }
                         }), nullptr));
                         dount->runAction(Sequence::create(DelayTime::create(0.35*i+1),ActionHelper::getJellyAction(), nullptr));
                     }

                 
                 }
             }), nullptr));
         }),nullptr);
         action->setTag(actiontag);
         node->runAction(action);
         node->getChildByName("dipping")->runAction(Sequence::create(FadeTo::create(0.2, 100), FadeTo::create(2.8, 255),nullptr));
     }else {
         _actionManager->resumeTarget(node);
         _actionManager->resumeTarget(node->getChildByName("dipping"));
     }

}