//
//  MakeStep1.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/13.
//
//

#include "MakeStep1.h"
using namespace ui;
#include "DragFallComponent.h"
#include "MoveHideShade.h"
#include "RollStep.h"
#include "ChangeLayer.h"
#include "ParticleLayer.h"
#include "TouchNodeComponent.h"
#include "EventPromotLayer.h"

float MakeStep1_endRote[] = {0,0,-90,-45,-90,0,-75,-75,0};
Vec2 MakeStep1_checkPoint[] = {
    Vec2(0.5,0.5),Vec2(0.5,0.5),Vec2(0.1,0.9)
    ,Vec2(0.1,0.9),Vec2(0.1,0.9),Vec2(0.5,0.5)
    ,Vec2(0.1,0.9),Vec2(0.1,0.9),Vec2(0.5,1)};

IMPLEMENT_COCOS2DX_CLASS(MakeStep1);

MakeStep1::~MakeStep1(){
}

bool MakeStep1::init(){
    if(BaseStepLayer::init()){
        cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("stiracg/stiracg.ExportJson");
        
        _operate = StudioLayer::create("cocostudio/MakeStep.csb");
        addChild(_operate);
        
        auto bowl_up = _operate->getNodeByName("bowl_up");
        bowl_up->getParent()->reorderChild(bowl_up, 14);
        mixArm = Armature::create("stiracg");
        mixArm->setPosition(bowl_up->getContentSize()*.5+Size(0,30));
        bowl_up->addChild(mixArm);
        mixArm->getAnimation()->play("1",  -1, 1);
        mixArm->setVisible(false);
        
        _prompt = Sprite::create("ui/pubilc/finger1.png");
        addChild(_prompt,300);
        _prompt->setVisible(false);
        _mixSprite = nullptr;
        addCount = 0;
       
        CocosHelper::visitAllNode(_operate->getNodeByName("optionLayer"), []( Node* _node){
            auto _particle = dynamic_cast<ParticleSystemQuad*>(_node);
            if(_particle != nullptr)
                _particle->stopSystem();
        });
        
        CocosHelper::visitAllNode(_operate->getNodeByName("mixLayer"), []( Node* _node){
            _node->getParent()->reorderChild(_node,12);
            _node->setCascadeOpacityEnabled(false);
        });
        
        
        soundPath.push_back("sound/pour sugar.mp3");
        soundPath.push_back("sound/add_water.mp3");
        soundPath.push_back("sound/add_water.mp3");
        soundPath.push_back("sound/pour sugar.mp3");
        _mixComponent = nullptr;
        
        return true;
    }
    return false;
}

void MakeStep1::showThing(const vector<string>& fallthingNames){
    int _index = 0;
    for(const auto& fallthingName:fallthingNames){
        ActionHelper::delayFunc(0.3*_index, this, [fallthingNames,this,fallthingName,_index](){
             SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                auto _node = _operate->getNodeByName(fallthingName);
           
        
            if(fallthingName=="butter"){
                auto _plate = _operate->getNodeByName("butter_plate");
                ActionHelper::showBackInOut(_plate, _plate->getPosition(), ActionHelper::ShowDirection::show_from_right);
                _plate->setVisible(true);
            }
            ActionHelper::showBackInOut(_node, _node->getPosition(), ActionHelper::ShowDirection::show_from_right,[this,_node,fallthingName,_index,fallthingNames](){
                if(_index == fallthingNames.size()-1){
                    ActionHelper::showTitle(_operate->getNodeByName("title"));
                    _operate->changeNode("title", "ui/cook/cook.png");
                }
                if(fallthingName == "sugar"){
                    ActionHelper::delayFunc(0.7, _prompt, [this,_node](){
                        
                        ActionHelper::runBezierMoveHideAction(_prompt, _node->getPosition(),_operate->getPosinWord("bowl")+Vec2(0,25));
                    
                    });
                }
                Rect _rect;
                ActionInterval*_inBowAction;
                auto _hideFinger = CallFunc::create([this](){
                    _operate->getNodeByName("title")->stopActionByTag(ActionHelper::titleActionTag);
                    //_operate->getNodeByName("title")->setVisible(false);
                    this->stopPrompt();
                });
                auto pFall = DragFallComponent::create();
                _node->addComponent(pFall);
                auto _inBowNode = _operate->getNodeByName(StringUtils::format("%sInbow",_node->getName().c_str()));
                if(fallthingName=="egg0"||fallthingName=="egg1"){
                    _rect = CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit_0"));
                    auto _animAl = Animation::create();
                    _animAl->setDelayPerUnit(0.3);
                    for(int i=0;i<=3;i++)
                        _animAl->addSpriteFrameWithFile(StringUtils::format("png/2/bowl_egg%d.png",i));
                    _inBowAction = Sequence::create(_hideFinger,CallFunc::create([_node,this]{
                        _node->getComponent("MoveHideShade")->setEnabled(false);
                 
                        Director::getInstance()->getEventDispatcher()->setEnabled(false);
                        _node->setVisible(false);
                    }),FadeTo::create(0, 255), Animate::create(_animAl),CallFunc::create([this](){
                       
                        Director::getInstance()->getEventDispatcher()->setEnabled(true);}), nullptr);
                } else if(fallthingName == "butter") {
                    _inBowAction = Sequence::create(_hideFinger,CallFunc::create([this,_node,_inBowNode](){
                     
                        Director::getInstance()->getEventDispatcher()->setEnabled(false);
                        _node->setVisible(false);
                        _node->getComponent("MoveHideShade")->setEnabled(false);
                        _inBowNode->setCascadeOpacityEnabled(false);
                        auto _s = Sprite::create("png/2/butter.png");
                        _s->setPosition(194.85, 316.63);
                        _inBowNode->addChild(_s);
                        _s->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0,199.62-316.63)),FadeTo::create(0, 0),nullptr));
                        
                    }),DelayTime::create(0.51), FadeTo::create(0, 255),CallFunc::create([this](){
                        auto _fallegg = _operate->getNodeByName("fallegg");
                        _fallegg->stopAllActions();
                        auto _anima = Animation::create();
                        for(int i=0;i<=1;i++)
                            _anima->addSpriteFrameWithFile(StringUtils::format("png/fallImage/egg%d.png",i));
                        _anima->setDelayPerUnit(0.15);
                        _fallegg->getParent()->reorderChild(_fallegg, 501);
                        _fallegg->runAction(Sequence::create(FadeTo::create(0, 255), Animate::create(_anima),FadeTo::create(0, 0),nullptr));
                        Director::getInstance()->getEventDispatcher()->setEnabled(true);
                    }), nullptr);
                    _rect = CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit_0"));
                } else {
                    float _time = 3;
//                    if(_node->getName() == "salt")
//                        _time =2;
                    _rect = CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit"));
                    _inBowAction = Sequence::create(_hideFinger,CallFunc::create([_node,this](){
                          _eventDispatcher->setEnabled(false);
                        if(_node->getName() == "water"){
                            ActionHelper::delayFunc(1, this, [this](){
                                auto _fallWater = _operate->getNodeByName("fallWater");
                                _fallWater->stopAllActions();
                                auto _anima = Animation::create();
                                for(int i=0;i<=1;i++)
                                    _anima->addSpriteFrameWithFile(StringUtils::format("png/fallImage/water%d.png",i));
                                _anima->setLoops(-1);
                                _anima->setDelayPerUnit(0.25);
                                _fallWater->setOpacity(255);
                                _fallWater->runAction(Animate::create(_anima));
                            });
                        } else if(_node->getName() == "flour"||_node->getName() == "powder"||_node->getName() == "sugar"){
                            auto _fallWater = _operate->getNodeByName("fallFlour");
                            _fallWater->stopAllActions();
                            auto _anima = Animation::create();
                            for(int i=0;i<=2;i++)
                                _anima->addSpriteFrameWithFile(StringUtils::format("png/fallImage/smoke%d.png",i));
                            _anima->setDelayPerUnit(0.5);
                            _fallWater->runAction(Sequence::create(DelayTime::create(1),FadeTo::create(0, 255), Animate::create(_anima),DelayTime::create(0.5),FadeTo::create(0, 0),nullptr));
                        }
                        auto _paticle = dynamic_cast<ParticleSystemQuad*>(_node->getChildByName("particle"));
                      
                        _paticle->stopSystem();
                        _node->stopAllActions();
                        if(_node->getName() == "salt")
                            _node->runAction(Sequence::create(MoveTo::create(0.3, _operate->getNodeByName("bowl")->getPosition()+Vec2(130,150+70)),CallFunc::create([=](){
                                _node->getChildByName("noshade")->setVisible(false);
                                _node->setRotation(0);
                                auto _innerNode = _node->getChildByName("salt_inner");
                                _innerNode->setVisible(true);
                                _innerNode->runAction(Sequence::create(RotateBy::create(0.5, -100),CallFunc::create([=](){
                                    _paticle->resetSystem();
                                    _paticle->setVisible(true);
                                    SoundPlayer::getInstance()->playEffect("sound/general/add salt.mp3",0.7);
                                }),DelayTime::create(0.4),RotateBy::create(0.2, 100), RotateBy::create(0.5, -100),CallFunc::create([=](){
                                    SoundPlayer::getInstance()->playEffect("sound/general/add salt.mp3",0.7);
                                    auto _paritlce2 = dynamic_cast<ParticleSystemQuad*>(_node->getChildByName("particle_0"));
                                    _paritlce2->resetSystem();
                                    _paritlce2->setVisible(true);
                                }), nullptr));
                            
                            }), nullptr));
                        else
                        _node->runAction(Sequence::create(MoveTo::create(0.3, _operate->getNodeByName("bowl")->getPosition()+Vec2(100,150)),CallFunc::create([=](){
                            _node->getParent()->reorderChild(_node, 5);
                            _paticle->resetSystem();
                            _paticle->setVisible(true);
                        }),MoveBy::create(1.4, Vec2(0,100)), nullptr));
                    }), DelayTime::create(0.3), FadeTo::create(_time, 255),CallFunc::create([this,_node](){
                            _node->getParent()->reorderChild(_node, 500);
                        _operate->getNodeByName("fallFlour")->stopAllActions();
                        _operate->getNodeByName("fallFlour")->setOpacity(0);
                         _operate->getNodeByName("fallWater")->stopAllActions();
                        _operate->getNodeByName("fallWater")->setOpacity(0);
                       
                        _eventDispatcher->setEnabled(true);
                    }), nullptr);
                }
                pFall->setTargetRect(_rect);
                pFall->setInbow(_inBowNode,_inBowAction);
                pFall->setCheckPoint(MakeStep1_checkPoint[addCount+_index]);
                pFall->setEndRote(MakeStep1_endRote[addCount+_index]);
                pFall->addListners(FallThingfallEnd, CC_CALLBACK_3(MakeStep1::fallThingCallback, this));
            });
            _node->setVisible(true);
        });
        _index++;
    }
}

void MakeStep1::touchEnd(ui::Widget* widget){
    if(widget->getName().find("roteSprite") != string::npos){
       // SoundPlayer::getInstance()->playEffect("sound/Sound/kitchen_tools.mp3");
        widget->stopAllActions();
        widget->setRotation(0);
        widget->runAction(ActionHelper::createRoteAction(10,0.22));
    }
}

void MakeStep1::TimerChangeCallBack(Node* _node,Component* _c,OperateListner*){
    if(_mixSprite != nullptr)
        _mixSprite->setVisible(false);
       mixArm->setVisible(false);
     _operate->getNodeByname<ParticleSystemQuad*>("mixparticle")->resetSystem();
    SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3",0.1);
    _c->setEnabled(false);
    auto _bowUp = _operate->getNodeByName("bowl");
     auto _bow = _operate->getNodeByName("bowl_up");
    
    const int _actionTag =1000;
    _bowUp->stopActionByTag(_actionTag);
    _bow->stopActionByTag(_actionTag);
    auto _scle = Spawn::create(MoveBy::create(0.6, Vec2(-40,0)), ScaleTo::create(0.6,1), nullptr);
    _scle->setTag(_actionTag);
    _bowUp->runAction(_scle);
    auto _scale2 = _scle->clone();
    _scale2->setTag(_actionTag);
    _bow->runAction(_scale2);
    
    auto _spoon = _operate->getNodeByName("spoon");
    _spoon->getParent()->reorderChild(_spoon, 500);
    moveOperate->setEnabled(false);
    //_bowUp->getParent()->reorderChild(_bowUp, 14);
    if(addCount == 9){
        _operate->getNodeByName("title")->stopAllActions();
        _operate->getNodeByName("title")->setVisible(false);
        _bowUp->stopActionByTag(_actionTag);
        _bow->stopActionByTag(_actionTag);
        _bow->runAction(Spawn::create(MoveTo::create(0.8, Vec2(480,_bow->getPositionY())),ScaleTo::create(0.8, 1.15), nullptr));
        _bowUp->runAction(Sequence::create(Spawn::create(MoveTo::create(0.8, Vec2(480,_bowUp->getPositionY())),ScaleTo::create(0.8, 1.15),nullptr) ,CallFunc::create([this](){ addChild(ParticleLayer::create());}), nullptr));
       
    }
      auto blender = _operate->getNodeByName("blender");
    ActionHelper::hide(_spoon, ActionHelper::ShowDirection::show_from_right,[_spoon,this](){
        _spoon->setVisible(false);
        _spoon->setRotation(0);
        if(addCount == 4){
            string fallthingNames[] = {"milk","butter"};
            showThing(vector<string>(fallthingNames,fallthingNames+2));
           
        }else if(addCount == 6){
            _operate->changeNode("spoon2_1_3", "png/2/spoon2_2.png");
            _operate->changeNode("blender3_1_1", "png/2/blender3_2.png");
            string fallthingNames[] = {"powder","flour","salt"};
            showThing(vector<string>(fallthingNames,fallthingNames+3));
        
        } else if(addCount == 9){
            ActionHelper::delayFunc(3, this, [](){
                auto _scene = Scene::create();
                _scene->addChild(RollStep::create());
                Director::getInstance()->replaceScene(TransitionFade::create(0.8,_scene,Color3B::WHITE));
            });
          
        }
    },false);
  
    blendOperate->setEnabled(false);
    blender->getParent()->reorderChild(blender, 500);
    ActionHelper::hide(blender, ActionHelper::ShowDirection::show_from_right,[_spoon,blender,this](){
        blender->setVisible(false);
        blender->setScale(0.8);
        blender->setRotation(0);
        blender->getChildByName("noshade")->setOpacity(255);
        blender->getChildByName("operating")->setOpacity(0);
        _spoon->getChildByName("noshade")->setOpacity(255);
        _spoon->getChildByName("operating")->setOpacity(0);
    },false);
}

void MakeStep1::fallThingCallback(Node* _fallThing,Component*_component, OperateListner*){
    _component->setEnabled(false);
     SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    auto _paticle = _fallThing->getChildByName("particle");
    if(_paticle != nullptr){
        _paticle->setVisible(false);
    }
    auto _bowUp = _operate->getNodeByName("bowl_up");
    //_bowUp->getParent()->reorderChild(_bowUp, 14);
    auto _bow = _operate->getNodeByName("bowl");
    auto _endFunc = [_bowUp,_bow,this](){
        ActionHelper::delayFunc(0.8, this, [this](){
            ActionHelper::showTitle(_operate->getNodeByName("title"));
            _operate->changeNode("title", "ui/cook/Mix-the-ingredients.png");
        });
     
            const int _actionTag =1000;
            _bowUp->stopActionByTag(_actionTag);
              _bow->stopActionByTag(_actionTag);
        auto _scle = Spawn::create(MoveBy::create(0.6, Vec2(40,0)), ScaleTo::create(0.6, _bow->getScale()+0.06), nullptr);
            _scle->setTag(_actionTag);
            _bowUp->runAction(_scle);
            auto _scale2 = _scle->clone();
            _scale2->setTag(_actionTag);
            _bow->runAction(_scale2);
    };

    
    _fallThing->runAction(EaseExponentialIn::create(RotateTo::create(0.6, 0)));
     SoundPlayer::getInstance()->playEffect("sound/general/ingredients_out.mp3");
    ActionHelper::hide(_fallThing, ActionHelper::ShowDirection::show_from_right);
    if(_fallThing->getName() =="butter"){
        auto _plate = _operate->getNodeByName("butter_plate");
        ActionHelper::hide(_plate, ActionHelper::ShowDirection::show_from_right);
    }
       auto spoon = _operate->getNodeByName("spoon");
     auto blender = _operate->getNodeByName("blender");
    spoon->getChildByName("spoon2_1_3")->setVisible(false);
     blender->getChildByName("blender3_1_1")->setVisible(false);
    addCount++;
    if(addCount == 4){
        _endFunc();
        vector<string> _paths;
        for(int i=0;i<=3;i++)
            _paths.push_back(StringUtils::format("png/2/bowl_stir%d.png",i));
        this->_mixComponent = TimerChangeComponent::create();
        _mixComponent->setDelayPerUnit(0.7);
        _mixComponent->setFrameFile(_paths);
        _mixComponent->addListners(TimerChangeEnd, CC_CALLBACK_3(MakeStep1::TimerChangeCallBack, this));
        _operate->getNodeByName("mixLayer")->addComponent(_mixComponent);
        
   
        ActionHelper::showBackInOut(blender, blendPos, ActionHelper::ShowDirection::show_from_right,[this,blender](){;
            blendOperate = MixToolOperate::create();
            blendOperate->addListners(MixToolOperate::operatemixing, [this](Node*,Component*,OperateListner*){
                   _operate->getNodeByName("title")->stopActionByTag(ActionHelper::titleActionTag);
                   mixArm->setVisible(true);
                auto mixing = _operate->getNodeByName("blender3_1_1");
                if(!mixing->isVisible()){
                    mixing->setOpacity(0);
                    mixing->setVisible(true);
                    mixing->runAction(FadeTo::create(0.5, 255));
                }else
                    _actionManager->resumeTarget(mixing);
              
            });
            blendOperate->addListners(MixToolOperate::operatemixpause, [this](Node*,Component*,OperateListner*){
                   mixArm->setVisible(false);
                 _actionManager->resumeTarget( _operate->getNodeByName("blender3_1_1"));
            });
            blendOperate->setTarget(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit_mixblend")));
            blendOperate->setEndRote(-34.07);
            blendOperate->setCheckPoint(Vec2(0.2,0.2));
            blender->addComponent(blendOperate);
            blendOperate->setMixLayer(_operate->getNodeByName("mixLayer"));
            auto _animation = Animation::create();
            for(int i=1;i<=3;i++)
                _animation->addSpriteFrameWithFile(StringUtils::format("png/2/blender%d.png",i));
            _animation->setLoops(-1);
            _animation->setDelayPerUnit(0.15);
            blendOperate->setOperating(Animate::create(_animation));
        });
        
        blender->setVisible(true);
        
        ActionHelper::showBackInOut(spoon, _spoonPos, ActionHelper::ShowDirection::show_from_right,[this,spoon](){
            moveOperate = MixToolOperate::create();
            moveOperate->setEndRote(-25);
            moveOperate->addListners(MixToolOperate::operatemixing, [this](Node*,Component*,OperateListner*){
                    _operate->getNodeByName("title")->stopActionByTag(ActionHelper::titleActionTag);
                   mixArm->setVisible(true);
                auto mixing = _operate->getNodeByName("spoon2_1_3");
                if(!mixing->isVisible()){
                    mixing->setOpacity(0);
                    mixing->setVisible(true);
                    mixing->runAction(FadeTo::create(0.5, 255));
                }else
                    _actionManager->resumeTarget(mixing);
                           });
            moveOperate->addListners(MixToolOperate::operatemixpause, [this](Node*,Component*,OperateListner*){
                   mixArm->setVisible(false);
                _actionManager->pauseTarget(_operate->getNodeByName("spoon2_1_3"));
            });
            moveOperate->setTarget(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit_mixspoon")));
            moveOperate->setCheckPoint(Vec2(0.1,0.1));
            spoon->addComponent(moveOperate);
            moveOperate->setMixLayer(_operate->getNodeByName("mixLayer"));
        });
        spoon->setVisible(true);
        
    } else if(addCount == 6){
           _endFunc();
        vector<string> _paths;
        for(int i=4;i<=7;i++)
            _paths.push_back(StringUtils::format("png/2/bowl_stir%d.png",i));
        _mixComponent->setFrameFile(_paths);
       
        ActionHelper::showBackInOut(spoon, _spoonPos, ActionHelper::ShowDirection::show_from_right,[this](){
                blendOperate->setTarget(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit_mixblend")));
                moveOperate->setTarget(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit_mixspoon")));
            moveOperate->setEnabled(true);
            _mixComponent->setEnabled(true);
        });
        spoon->setVisible(true);
        auto blender = _operate->getNodeByName("blender");
        ActionHelper::showBackInOut(blender, blendPos, ActionHelper::ShowDirection::show_from_right,[this](){
            blendOperate->setEnabled(true);
        });
        blender->setVisible(true);
        
    }else if(addCount == 9){
         _mixComponent->changeSound("sound/general/dough_mix.mp3");
           _endFunc();
        vector<string> _paths;
        for(int i=8;i<=12;i++)
            _paths.push_back(StringUtils::format("png/2/bowl_stir%d.png",i));
        _mixComponent->setFrameFile(_paths);
        ActionHelper::showBackInOut(spoon, _spoonPos, ActionHelper::ShowDirection::show_from_right,[this](){
            blendOperate->setTarget(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit_mixblend")));
            moveOperate->setTarget(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit_mixspoon")));
            moveOperate->setEnabled(true);
            _mixComponent->setEnabled(true);
        });
        spoon->setVisible(true);
       
        ActionHelper::showBackInOut(blender, blendPos, ActionHelper::ShowDirection::show_from_right,[this](){
            blendOperate->setEnabled(true);
        });
        blender->setVisible(true);
    }else {
        const int _actionTag =1000;
        _bowUp->stopActionByTag(_actionTag);
        _bow->stopActionByTag(_actionTag);
        auto _scle = Sequence::create(ScaleTo::create(0.4, 1.06), ScaleTo::create(0.4, _bow->getScale()),nullptr);
        _scle->setTag(_actionTag);
        _bowUp->runAction(_scle);
        auto _scale2 = _scle->clone();
        _scale2->setTag(_actionTag);
        _bow->runAction(_scale2);
    }
}

void MakeStep1::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    _spoonPos = _operate->getNodeByName("spoon")->getPosition();
    blendPos =_operate->getNodeByName("blender")->getPosition();
    string names[] = {"bowl","bowl_up"};
    for(int i=0;i<2;i++){
        auto _node = _operate->getNodeByName(names[i]);
        ActionHelper::showBackInOut(_node, _node->getPosition(), ActionHelper::ShowDirection::show_from_left);
        _node->setVisible(true);
    }
    string fallthingNames[] = {"egg1","egg0","water","sugar"};
    showThing(vector<string>(fallthingNames,fallthingNames+4));
   
    auto _EventPromotLayer = EventPromotLayer::create();
    _EventPromotLayer->setTime(2.5);
    addChild(_EventPromotLayer,200);
    _EventPromotLayer->setCallbakc([this](){
        string _names[] = {"roteSprite0","roteSprite1","roteSprite2"};
        auto _sinze = sizeof(_names)/sizeof(_names[0]);
        auto _node = _operate->getNodeByname<ui::Widget*>(_names[rand()%_sinze]);
        if(_node != nullptr)
            touchEnd(_node);
    });
}