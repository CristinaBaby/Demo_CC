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
std::function<Layer*()> ChangeLayer::_changeLayer = nullptr;
bool ChangeLayer::isShow = false;
void ChangeLayer::onExitTransitionDidStart(){
     AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds,true);
    _changeLayer = nullptr;
    isShow = false;
    BaseStepLayer::onExitTransitionDidStart();
}

void ChangeLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    _operate->playAction("show",CC_CALLBACK_0(ChangeLayer::endFunc, this));
    _operate->getNodeByName("donuts0")->runAction(FadeTo::create(0.7, 0));
    SoundPlayer::getInstance()->playEffect(StringUtils::format("sound/other/loading_1.mp3"));
    //soundIndex++;
    ActionHelper::delayFunc(0.7, this, [this](){
         SoundPlayer::getInstance()->playEffect(StringUtils::format("sound/other/loading_2.mp3"));
        _operate->getNodeByName("donuts1")->runAction(FadeTo::create(0.7, 0));
    });
    ActionHelper::delayFunc(0.7, this, [this](){
         SoundPlayer::getInstance()->playEffect(StringUtils::format("sound/other/loading_3.mp3"));
        _operate->getNodeByName("icing")->runAction(FadeTo::create(0.7, 255));
    });
}

void ChangeLayer::endFunc(){
    auto _dount = _operate->getNodeByname<Sprite*>("donut");
    auto _createParticle = CallFunc::create([_dount,this](){
        vector<int> index;
        for(int i=3;i<=10;i++){
            index.push_back(i);
            
        }
        
        for(int i=0;i<8;i++){
            auto _itor = index.begin()+rand()%index.size();
            int target = *_itor;
            ActionHelper::delayFunc(0.07*i, _dount, [_dount,target](){
                auto _particle = ParticleSystemQuad::create("particle/dount2.plist");
                _particle->setSpeed(1300);
                _particle->setTexture(Director::getInstance()->getTextureCache()->addImage(StringUtils::format("png/loading/donuts%d.png",target)));
                _particle->setPosition(_dount->getContentSize().width*.5,_dount->getContentSize().height*0.9);
                _dount->addChild(_particle,1);
            });
            
            index.erase(_itor);
        }
       
      
         SoundPlayer::getInstance()->playEffect("sound/Sound/loading_donuts.mp3");
    });
    
    _dount->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.1, 0.9, 1.2),ScaleTo::create(0.1, 1.1,0.9),_createParticle->clone(), DelayTime::create(0.6),ScaleTo::create(0.05, 1, 1), DelayTime::create(2), nullptr)));
    _dount->runAction(Sequence::create(DelayTime::create(5), CallFunc::create([this](){
        if(_changeLayer != nullptr){
            auto _layer = _changeLayer();
            auto _scne = Scene::create();
            _scne->addChild(_layer);
            Director::getInstance()->replaceScene(TransitionFade::create(0.8,_scne,Color3B::WHITE));
            _changeLayer = nullptr;
            isShow = false;
        }
    }),nullptr));
}

void ChangeLayer::loading(){
    if(isShow)
        return;
    AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds,false);
    isShow = true;
    auto _scene = Scene::create();
    _scene->addChild(ChangeLayer::create());
    Director::getInstance()->replaceScene(_scene);
}

bool ChangeLayer::init(){
    BaseStepLayer::init();
    soundIndex = 0;
    auto _EventPromotLayer = EventPromotLayer::create();
     _EventPromotLayer->setCallbakc([](){});
    addChild(_EventPromotLayer,50);
    _operate = StudioLayer::create("ChangeLayer.csb");
    addChild(_operate);
    auto _dount = _operate->getNodeByname<Sprite*>("donut");
    auto _shade = _operate->getNodeByName("shade");
    //    _shade->setVisible(false);
    //
    auto _target = DistanceToTarget::create();
    _target->setTargetRect(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("limit")));
    _dount->addComponent(_target);
    _target->addListners(MoveDistancePercent, [_shade](Node* _node,Component*, OperateListner* _lis){
        //float _percent = dynAny_cast<float>(_lis->getData());
        //_shade->setScale(1+_percent*0.2);
        _shade->setScaleX(_node->getScaleX());
    });
    _target->startUpdate();
    
    
    //
    //    auto _endFunc = CallFunc::create([this,_dount,_shade](){
    //        vector<int> index;
    //        for(int i=3;i<=10;i++){
    //            index.push_back(i);
    //
    //        }
    //        for(int i=0;i<5;i++){
    //            auto _itor = index.begin()+rand()%index.size();
    //            auto _particle = ParticleSystemQuad::create("particle/dountParticle.plist");
    //            _particle->setTexture(Director::getInstance()->getTextureCache()->addImage(StringUtils::format("png/loading/donuts%d.png",*_itor)));
    //            _particle->setPosition(_operate->getPosinWord("donut") -Vec2(40,40)+Vec2(rand()%80,rand()%80));
    //            this->addChild(_particle);
    //            index.erase(_itor);
    //        }
    //
    //    });
    //
    //    auto _pos = _dount->getPosition();
    //    ActionHelper::setStartPostion(_dount, _dount->getPosition(), ActionHelper::ShowDirection::show_from_left);
    //    auto _move =  PositionChange::create(MoveTo::create(1, _pos));
    //    _move->setCallBack([](Node* _node,Vec2 pre,Vec2 _crrrent){
    //        _node->setRotation(_node->getRotation()+ (_crrrent.x-pre.x)/_node->getContentSize().height*(180/3.1415926));
    //    });
    //    _dount->runAction(_move);
    //    auto _progres = ProgressTimer::create(Sprite::create("png/loading/donuts0.png"));
    //    _progres->setPosition(_dount->getContentSize()*.5);
    //    _dount->addChild(_progres,1);
    //    _progres->setReverseDirection(true);
    //
    //    auto _progres2 = ProgressTimer::create(Sprite::create("png/loading/donuts1.png"));
    //    _progres2->setPosition(_dount->getContentSize()*.5);
    //    _dount->addChild(_progres2,0);
    //    _progres2->setReverseDirection(true);
    //       _progres2->setPercentage(100);
    //      _progres->setPercentage(100);
    
    
    
    //    _dount->runAction(RepeatForever::create(Sequence::create(JumpBy::create(1, Vec2::ZERO, 60, 2), DelayTime::create(2),  nullptr)));
    //    _progres2->runAction(Sequence::create(DelayTime::create(4), ProgressFromTo::create(4, 100, 0),DelayTime::create(2), _endFunc,nullptr));
    //
    //    _progres->runAction(ProgressFromTo::create(4, 100, 0));
    
    
    //    _dount->runAction(Sequence::create(JumpBy::create(1, Vec2::ZERO, 60, 2),CallFunc::create([_progres](){
    //        _progres->runAction(ProgressFromTo::create(4, 100, 0));
    //    }), DelayTime::create(4),JumpBy::create(1, Vec2::ZERO, 60, 2), CallFunc::create([_progres2](){
    //        _progres2->runAction(ProgressFromTo::create(4, 100, 0));
    //    }),DelayTime::create(6),_endFunc, nullptr));
    return true;
}