//
//  MakeStep1.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/21.
//
//

#include "MakeStep1.h"
#include "GameFood.h"
#include "MakePowder.h"
#include "ParticleLayer.h"
#include "ChangeLayer.h"

bool MakeStep1::init(){
    if(BaseMakeLayer::init()) {
        dipCount = 0;
        moveCount =0;
        inBowlCount =0;
        std::vector<string> file;
        for(int i=0;i<4;i++)
            file.push_back(StringUtils::format("png/2/bowl_stiregg%d.png",i));
        inintNecessary("MakeStep1.csb",file);
        _prompt = dynamic_cast<Sprite*>(_operate->getNodeByName("finger"));
        auto _node = _operate->getNodeByName("smallBowlup");
        _node->getParent()->reorderChild(_node, 3);
        _operate->getNodeByname<ParticleSystemQuad*>("dippingParticle")->stopSystem();
        return true;
    }
    return false;
}


void MakeStep1::mixEnd(Node*node,Component* c, OperateListner* _lis){
    BaseMakeLayer::mixEnd(node,c,_lis);
    stopPrompt();
    showFood();
}




void MakeStep1::showFood(){
    auto plate = _operate->getNodeByName("getPlate");
    ActionHelper::showBackInOut(plate, plate->getPosition(), ActionHelper::ShowDirection::show_from_left,[this](){
        ActionHelper::runBezierMoveHideAction(_prompt, _operate->getPosinWord("getPlate"), _operate->getPosinWord("bowl"));
        auto foods = GameFood::getInstance()->getFoods();
        for(auto node:foods){
            auto moveOperate = TouchMoveComponent::create();
            moveOperate->setMoveType(MoveComponentType::kTypeBack);
            moveOperate->setTarget(LQRect::create(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("diplimit"))));
            moveOperate->addListners(ComponentTouchMove,[moveOperate,this](Node* node,Component*, OperateListner* _lis){
                if(moveOperate->checkInlimit()){
                  
                    inBowlCount++;
                    this->stopPrompt();
                    _lis->setEnable(false);
                    moveOperate->setBrintTop(false);
                    auto optionLayer = _operate->getNodeByName("optionLayer");
                    if(node->getParent() != optionLayer){
                        node->setZOrder(0);
                        CocosHelper::reAddNode(node, optionLayer);
                    }
                    auto paths = GameFood::getInstance()->getDipPath();
                    dynamic_cast<Sprite*>(node)->setSpriteFrame(Sprite::create(paths.at(0))->getSpriteFrame());
                    auto dipping = Sprite::create(paths[1]);
                    dipping->setName("dipping");
                    dipping->setOpacity(0);
                    dipping->setPosition(node->getContentSize()*.5);
                    node->addChild(dipping);
                    moveOperate->setMoveType(MoveComponentType::kTypeInRect);
                    moveOperate->addListners(ComponentTouchMove,CC_CALLBACK_3(MakeStep1::dipping,this));
                    auto end = [this](Node* node,Component*, OperateListner* _lis){
                        auto particle = _operate->getNodeByname<ParticleSystemQuad*>("dippingParticle");
                        if(particle->isActive())
                            particle->stopSystem();
                        _actionManager->pauseTarget(node);
                        _actionManager->pauseTarget(node->getChildByName("dipping"));
                        scaleBackBowl();
                    };
                    moveOperate->addListners(ComponentTouchEnd,end);
                    moveOperate->addListners(ComponentTouchCancle, end);
                    if(inBowlCount==GameFood::getInstance()->getFoods().size()){
                        ActionHelper::hide(_operate->getNodeByName("getPlate"), ActionHelper::ShowDirection::show_from_left);
                    }
                }
            });
            node->addComponent(moveOperate);
        }
    },1);
    auto food = GameFood::getInstance()->createBeforeDip();
    food->setPosition(plate->getContentSize()*.5);
    plate->addChild(food);
    plate->setVisible(true);
    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
}

void MakeStep1::dipping(Node*node,Component*c, OperateListner* _lis){
    const int actiontag = 10;
    scaleBowl();
    auto action = node->getActionByTag(actiontag);
    auto particle = _operate->getNodeByname<ParticleSystemQuad*>("dippingParticle");
    if(!particle->isActive())
        particle->resetSystem();
    if(action == nullptr){
        if(moveCount == 0){
            _prompt->setPosition(node->convertToWorldSpace(node->getContentSize()*.5)+Vec2(40,-60));
            auto action2 = RepeatForever::create(Repeat::create(Sequence::create(MoveBy::create(0.6, Vec2(80,0)),MoveBy::create(0.6, Vec2(-80,0)), nullptr),2));
            _prompt->runAction(action2);
            _prompt->setVisible(true);
        }
        node->runAction(RepeatForever::create(ActionHelper::createRoteAction(18,0.1)));
        
        action = Sequence::create(FadeTo::create(2, 155),CallFunc::create([=](){
            node->stopAllActions();
            node->setOpacity(0);
            _lis->setEnable(false);
            dipEnd(node,c);
        }),nullptr);
        action->setTag(actiontag);
        node->runAction(action);
        node->getChildByName("dipping")->runAction(FadeTo::create(2, 255));
    }else {
        moveCount++;
        if(moveCount==25)
            stopPrompt();
        _actionManager->resumeTarget(node);
        _actionManager->resumeTarget(node->getChildByName("dipping"));
    }
}

void MakeStep1::dipEnd(Node* node,Component*com){
    stopPrompt();
    auto DippingParticle = _operate->getNodeByname<ParticleSystemQuad*>("dippingParticle");
    if(DippingParticle->isActive())
        DippingParticle->stopSystem();
    ActionHelper::runBezierMoveHideAction(_prompt, node->convertToWorldSpace(node->getContentSize()*.5), _operate->getPosinWord("putPlate"));
    auto moveOperte = dynamic_cast<TouchMoveComponent*>(com);
    moveOperte->setBrintTop(true);
    moveOperte->setMoveType(MoveComponentType::kTypeBack);
    auto putPlate = _operate->getNodeByName("putPlate");
    moveOperte->setOrnagnalPos(node->getPosition());
    auto rect = CocosHelper::getNodeRectInWorld(putPlate);
    rect.origin = rect.origin - Vec2(0,30);
    rect.size = rect.size+Size(0,60);
    moveOperte->setTarget(LQRect::create(rect));
    moveOperte->setEnabled(false);
    ActionHelper::delayFunc(0.5, this, [=](){moveOperte->setEnabled(true);});
    scaleBackBowl();
    auto particle = ParticleSystemQuad::create("particles/washEnd.plist");
    particle->setPosition(node->convertToWorldSpace(node->getContentSize()*.5));
    addChild(particle,10);
    SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
    moveOperte->addListners(ComponentTouchEnd, [this,putPlate](Node* node,Component*com,OperateListner*){
        node->setZOrder(0);
        CocosHelper::reAddNode(node, putPlate);
        auto putNode = GameFood::getInstance()->getDippedFood(dipCount);
        node->runAction(MoveTo::create(0.2,putNode->getPosition()));
        node->runAction(ScaleTo::create(0.2,putNode->getScale()));
        node->runAction(RotateTo::create(0.2,putNode->getRotationX(),putNode->getRotationY()));
        com->setEnabled(false);
        this->stopPrompt();
        dipCount++;
        if(dipCount==GameFood::getInstance()->getFoods().size())
            endAllStep();
    });
    if(!putPlate->isVisible()){
        ActionHelper::showBackInOut(putPlate, putPlate->getPosition(), ActionHelper::ShowDirection::show_from_right);
        putPlate->setVisible(true);
        SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    }
}

void MakeStep1::onEnterTransitionDidFinish(){
    BaseMakeLayer::onEnterTransitionDidFinish();
    showNode("bowl", ActionHelper::ShowDirection::show_from_left);
    showNode("bowl_up", ActionHelper::ShowDirection::show_from_left);
    string eggs[] = {"egg0","egg1","smallBowl","smallBowlup"};
    for(int i=0;i<sizeof(eggs)/sizeof(eggs[0]);i++){
        ActionHelper::delayFunc(0.35, this, [=](){
            
            auto _node = _operate->getNodeByName(eggs[i]);
            auto endFunc = [=](){
                if(i==0)
                    ActionHelper::runBezierMoveHideAction(_prompt, _operate->getPosinWord("smallBowl"), _operate->getPosinWord("bowl"));
                if(eggs[i].find("egg")!= string::npos) {
                    auto _con = createDraOperate();
                    auto inbowlNode = _operate->getNodeByName(eggs[i]+"Inbowl");
                    _con->setEndPos(Vec2(419.25,385.77));
                    if(eggs[i] == "egg1")
                        _con->setEndPos(Vec2(502.22,391.59));
                    auto anima = Animation::create();
                    anima->setDelayPerUnit(0.3);
                    for(int i=0;i<4;i++)
                        anima->addSpriteFrameWithFile(StringUtils::format("png/2/bowl_egg%d.png",i));
                    auto _action = Sequence::create(Animate::create(anima), nullptr);
                    _con->setInbow(inbowlNode, _action);
                    _con->addListners(FallThingstartFall, [this](Node*node,Component*, OperateListner* _lis){
                        node->setVisible(false);
                        
                    });
                    _node->addComponent(_con);
                }
            };
            ActionHelper::showBezier(_node, _node->getPosition(), ActionHelper::ShowDirection::show_from_right,endFunc);
            _node->setVisible(true);
            SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
        });
        
    }
}

void MakeStep1::fallEnd(Node*node,Component* fall, OperateListner* _lis){
    BaseMakeLayer::fallEnd(node, fall, _lis);
    if(count == addEndNum){
        ActionHelper::hide(_operate->getNodeByName("smallBowl"), ActionHelper::ShowDirection::show_from_right);
        ActionHelper::hide(_operate->getNodeByName("smallBowlup"), ActionHelper::ShowDirection::show_from_right);
    };
}

void MakeStep1::endAllStep(){
    string names[] = {"bowl","bowl_up","getPlate"};
    ActionHelper::ShowDirection dir[] = {ActionHelper::ShowDirection::show_from_right,ActionHelper::ShowDirection::show_from_right,ActionHelper::ShowDirection::show_from_left};
    for(int i=0;i<2;i++)
        ActionHelper::hide(_operate->getNodeByName(names[i]), dir[i]);
    auto _putPlate = _operate->getNodeByName("putPlate");
    float time = 1;
    _putPlate->runAction(ScaleTo::create(time, 1.3));
    _putPlate->runAction(Sequence::create(MoveTo::create(time, Vec2(480,280)),
                                          CallFunc::create([this](){
        addChild(ParticleLayer::create(),30);
        ActionHelper::delayFunc(2.25, this, [](){
            ChangeLayer::_changeLayer = [](){
                return MakePowder::createScene();
            };
            ChangeLayer::loading();;
        });
        
    }), nullptr));
    
    
}