//
//  BaseMakeLayer.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/21.
//
//

#include "BaseMakeLayer.h"
#include "DistanceToTarget.h"

 BaseMakeLayer::BaseMakeLayer(){
     count =0;
     addEndNum = 0;
}

void BaseMakeLayer::inintNecessary(const string& layerPath,const vector<string>& mixPath){
    _operate = StudioLayer::create(layerPath);
    addChild(_operate);
    auto _node2 = _operate->getNodeByName("bowl_up");
    _node2->getParent()->reorderChild(_node2, 3);
    
    mix = TimerChangeComponent::create();
    mix->setFrameFile(mixPath);
    mix->setDelayPerUnit(1.5);
    _operate->getNodeByName("mixLayer")->addComponent(mix);
    mix->addListners(TimerChangeEnd,CC_CALLBACK_3(BaseMakeLayer::mixEnd,this));
}

void BaseMakeLayer::touchEnd(ui::Widget* widget){
    if(widget->getName().find("rote") != string::npos){
        widget->stopAllActions();
        widget->setRotation(0);
        widget->runAction(ActionHelper::createRoteAction(10,0.22));
    }
}

void BaseMakeLayer::mixEnd(Node*node,Component*, OperateListner* _lis){
    moveSpoon->setEnabled(false);
    string nodeNames[] = {"bowl","bowl_up"};
    for(int i=0;i<2;i++){
        auto node = _operate->getNodeByName(nodeNames[i]);
        node->runAction(ScaleTo::create(0.6, 1));
        node->runAction(MoveBy::create(0.4, Vec2(80,0)));
        
    }
    auto _particle = ParticleSystemQuad::create("particles/start_1.plist");
    auto bow = _operate->getNodeByName("bowl");
    _particle->setPosition(bow->convertToWorldSpace(bow->getContentSize()*.5));
    SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
    addChild(_particle,30);
    ActionHelper::hide(_operate->getNodeByName("spoon"), ActionHelper::ShowDirection::show_from_right);
}

void BaseMakeLayer::fallEnd(Node*node,Component*, OperateListner* _lis){
    stopPrompt();
    count++;
    string nodeNames[] = {"bowl","bowl_up"};
    for(int i=0;i<2;i++)
        _operate->getNodeByName(nodeNames[i])->runAction(Sequence::create(ScaleTo::create(0.17, 1.05),ScaleTo::create(0.17, 1), nullptr));
    if(count == addEndNum){
        ActionHelper::delayFunc(0.4, this, [=](){
            for(int i=0;i<2;i++)
                _operate->getNodeByName(nodeNames[i])->runAction(MoveBy::create(0.4, Vec2(-80,0)));
            showSpoon();
        });
    }
}

void BaseMakeLayer::showSpoon(){
    auto spoon = _operate->getNodeByName("spoon");
    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    ActionHelper::showBackInOut(spoon, spoon->getPosition(), ActionHelper::ShowDirection::show_from_right,[=](){
        
        DistanceToTarget *rote =DistanceToTarget::create();
        rote->setCheckPoint(Vec2(0.2,0.8));
        rote->setTargetRect(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("spoonLImit")));
        spoon->addComponent(rote);
        rote->addListners(MoveDistancePercent,[](Node* _node,Component*,OperateListner* _oper){
            float _percent = dynAny_cast<float>(_oper->getData());
            if(_percent<0)
                _percent =0;
            else if(_percent>1)
                _percent = 1;
            _node->setRotation(-90*_percent);
        });

        
        moveSpoon = MoveHideShade::create();
        moveSpoon->setTarget(LQRect::create(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("spoonLImit"))));
        moveSpoon->setCheckPoint(Vec2(0.2,0.8));
        moveSpoon->setMoveType(MoveComponentType::kTypeBack);
        moveSpoon->addListners(ComponentTouchMove, [=](Node*,Component*, OperateListner* _lis){
            if(moveSpoon->checkInlimit()){
                _operate->getNodeByName("mixing")->setVisible(true);
                auto mix0  = _operate->getNodeByName("mixing0");
                mix0->setOpacity(0);
                mix0->setVisible(true);
                mix0->runAction(Sequence::create(DelayTime::create(0.2),FadeTo::create(0.2, 255),  nullptr));
                rote->pauseUpdate();
                spoon->runAction(RotateTo::create(0.2, -90));
                _lis->setEnable(false);
                moveSpoon->setBrintTop(false);
                spoon->setZOrder(0);
                moveSpoon->setMoveType(MoveComponentType::kTypeInRect);
                moveSpoon->addListners(ComponentTouchMove, [this](Node*,Component*, OperateListner* _lis){
                    moveSpoon->playSound();
                    mix->start();
                    scaleBowl();
                });
                auto end = [=](Node*,Component*, OperateListner* _lis){
                    rote->pauseUpdate();
                    moveSpoon->stopSound();
                    mix->stop();
                    scaleBackBowl();
                };
                moveSpoon->addListners(ComponentTouchEnd,end);
                moveSpoon->addListners(ComponentTouchCancle, end);
            } else {
                rote->startUpdate();
            }
        });
        spoon->addComponent(moveSpoon);
    });
    spoon->setVisible(true);
    
}

void BaseMakeLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    showNode("bowl", ActionHelper::ShowDirection::show_from_left);
    showNode("bowl_up", ActionHelper::ShowDirection::show_from_left);
}

void BaseMakeLayer::showNode(const string& nodeName,ActionHelper::ShowDirection dire,float delay) {
    if(delay<0.0001){
            SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
        auto _node = _operate->getNodeByName(nodeName);
        ActionHelper::showBezier(_node, _node->getPosition(), dire);
        _node->setVisible(true);
    }else {
        ActionHelper::delayFunc(delay, this, [=](){
                SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
            auto _node = _operate->getNodeByName(nodeName);
            ActionHelper::showBezier(_node, _node->getPosition(), dire);
            _node->setVisible(true);
        });
    }
}

void BaseMakeLayer::scaleBowl() {
//    string nodeNames[] = {"bowl","bowl_up"};
//    for(int i=0;i<2;i++){
//        auto node = _operate->getNodeByName(nodeNames[i]);
//        int actionTag =10;
//        auto ac = node->getActionByTag(actionTag);
//        if(ac == nullptr){
//            ac = ScaleTo::create(0.6, 1.06);
//            ac->setTag(actionTag);
//            node->runAction(ac);
//        }
//    }
    
}
void BaseMakeLayer::scaleBackBowl(){
//    string nodeNames[] = {"bowl","bowl_up"};
//    for(int i=0;i<2;i++){
//        auto node = _operate->getNodeByName(nodeNames[i]);
//        node->stopAllActions();
//        node->runAction(ScaleTo::create(0.6, 1));
//    }
}

DragEndPosFall* BaseMakeLayer::createDraOperate(){
    auto _con = DragEndPosFall::create();
    _con->addListners(FallThingfallEnd, CC_CALLBACK_3(BaseMakeLayer::fallEnd, this));
    _con->setTargetRect(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit")));
    addEndNum++;
    return _con;
}