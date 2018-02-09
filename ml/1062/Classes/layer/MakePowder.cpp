//
//  MakePowder.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang on 16/1/22.
//
//

#include "MakePowder.h"
#include "CoatePowder.h"
#include "ParticleLayer.h"
bool MakePowder::init(){
    if(BaseMakeLayer::init()) {
        string names[] = {"milk","salt","sugar","flour","crumbs","chili"};
        Vec2 pos[] = {
            Vec2(505.79,435.88)
            ,Vec2(628.76,431.85)
            ,Vec2(716.16,483.98)
            ,Vec2(599.49,505.61)
            ,Vec2(675.96,427.07)
            ,Vec2(547.69,466.73)
        };
        float rote[] = {-90.24,-107.99,-33.25,-82.80,-75.27,-30.28};
        float time[] = {1.6,1,2,2,2,2};
        for(int i=0;i<sizeof(names)/sizeof(names[0]);i++){
            endPos.insert(unordered_map<string,Vec2>::value_type(names[i],pos[i]));
            endRote.insert(unordered_map<string,float>::value_type(names[i],rote[i]));
            fallTime.insert(unordered_map<string,float>::value_type(names[i],time[i]));
        }
        
        std::vector<string> file;
        for(int i=0;i<4;i++)
            file.push_back(StringUtils::format("png/2/bowl_stir%d.png",i));
        inintNecessary("MakePowder.csb",file);
        CocosHelper::visitAllNode(_operate, [](Node* node){
            auto _paritcle = dynamic_cast<ParticleSystemQuad*>(node);
            if(_paritcle != nullptr)
                _paritcle->stopSystem();
        });
        _prompt =  _operate->getNodeByname<Sprite*>("finger");
        return true;
    }
    return false;
}
void MakePowder::onEnterTransitionDidFinish(){
    BaseMakeLayer::onEnterTransitionDidFinish();
    string addNames[] = {"milk","salt","sugar"};
    for(int i=0;i<sizeof(addNames)/sizeof(addNames[0]);i++){
        ActionHelper::delayFunc(0.35*(i+1), this, [=](){
            auto _node = _operate->getNodeByName(addNames[i]);
            auto endFunc = [=](){
                    if(_node->getName() == "sugar")
                        ActionHelper::runBezierMoveHideAction(_prompt, _operate->getPosinWord("milk"), _operate->getPosinWord("bowl"));
                _node->addComponent(createDraOperate(addNames[i]));
            };
            ActionHelper::showBezier(_node, _node->getPosition(), ActionHelper::ShowDirection::show_from_right,endFunc);
            _node->setVisible(true);
                SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
        });
    }
}

void MakePowder::fallEnd(Node*node,Component* fall, OperateListner* _lis){
    if(count == 2) {
        string addNames[] = {"flour","crumbs","chili"};
        Vector<DragEndPosFall*> coms;
        for(int i=0;i<sizeof(addNames)/sizeof(addNames[0]);i++){
            coms.pushBack(createDraOperate(addNames[i]));
            ActionHelper::delayFunc(0.35*(i+1), this, [=](){
                auto _node = _operate->getNodeByName(addNames[i]);
                auto endFunc = [=](){_node->addComponent(coms.at(i));};
                ActionHelper::showBezier(_node, _node->getPosition(), ActionHelper::ShowDirection::show_from_right,endFunc);
                _node->setVisible(true);
                SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
            });
        }
    }
    BaseMakeLayer::fallEnd(node, fall, _lis);
    float time = 0.4;
    if(node->getName() == "milk")
        time = 1.5;
    ActionHelper::delayFunc(time, this, [=]{
        node->runAction(RotateTo::create(0.6, 0));
        ActionHelper::hide(node, ActionHelper::ShowDirection::show_from_right);
    });
}

DragEndPosFall* MakePowder::createDraOperate(const string& nodeName){
    auto drag =  BaseMakeLayer::createDraOperate();
    auto inbowlNode = _operate->getNodeByName(nodeName+"Inbowl");
    drag->setEndPos(endPos.at(nodeName));
    drag->setEndRote(endRote.at(nodeName));
    if(nodeName=="milk")
        drag->setInbow(inbowlNode, Sequence::create(CallFunc::create([=](){
            _operate->getNodeByName("milk")->getChildByName("noshade")-> setOpacity(0);
            auto milk0 = _operate->getNodeByName("milk0");
           milk0->setVisible(true);
            auto fall = _operate->getNodeByName("milkFall");
            fall->runAction(Sequence::create(ScaleTo::create(0.6, 1,1),CallFunc::create([=](){
                auto anima = Animation::create();
                anima->setDelayPerUnit(0.3);
                anima->setLoops(-1);
                auto anima2 = Animation::create();
                anima2->setDelayPerUnit(2.2/4.0f);
                for(int i=0;i<3;i++){
                    anima->addSpriteFrameWithFile(StringUtils::format("png/2/milkFall/milk%d_%d.png",i,i));
                    anima2->addSpriteFrameWithFile(StringUtils::format("png/2/milkFall/milk%d.png",i));
                }
                 anima2->addSpriteFrameWithFile("png/2/milkFall/milk3.png");
                milk0->runAction(Animate::create(anima2));
                fall->runAction(Animate::create(anima));
            }), nullptr));
        }), DelayTime::create(0.6), FadeTo::create(fallTime.at(nodeName), 255),CallFunc::create([this](){ _operate->getNodeByName("milkFall")->setVisible(false);}),nullptr));
    else
    drag->setInbow(inbowlNode, Sequence::create(DelayTime::create(0.2), FadeTo::create(fallTime.at(nodeName), 255),nullptr));
    drag->addListners(FallThingstartFall, [this](Node*,Component*, OperateListner* _lis){this->stopPrompt();});
    return drag;
}

void MakePowder::mixEnd(Node*node,Component* c, OperateListner* _lis){
    BaseMakeLayer::mixEnd(node, c, _lis);
    
    _operate->getNodeByName("bowl")->stopAllActions();
    _operate->getNodeByName("bowl_up")->stopAllActions();
    _operate->getNodeByName("bowl")->runAction(Spawn::create(ScaleTo::create(0.5, 1.15),MoveBy::create(0.5, Vec2(60,60)), nullptr));
    _operate->getNodeByName("bowl_up")->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 1.15),MoveBy::create(0.5, Vec2(60,60)), nullptr),CallFunc::create([=](){
        addChild(ParticleLayer::create(),30);
        ActionHelper::delayFunc(2.25, this, [](){
            Director::getInstance()->replaceScene(TransitionFade::create(0.8, CoatePowder::createScene(),Color3B::WHITE));
        });
    }), nullptr));
    
   
}