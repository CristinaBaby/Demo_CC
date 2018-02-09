//
//  VoteOperate.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/14.
//
//

#include "VoteOperate.h"
#include "ActionHelper.h"
#include "VoteEndStep.h"
#include "ParticleLayer.h"
IMPLEMENT_COCOS2DX_CLASS(VoteOperate);
VoteOperate::VoteOperate():
curentNum(0)
,endNum(5)
,winIndex(-1){
    
}
bool VoteOperate::init(){
    if(LQComponent::init()) {
        return true;
    }
    return false;
}
void VoteOperate::onAdd(){
    LQComponent::onAdd();
    dynamic_cast<StudioLayer*>(_owner)->getNodeByname<ParticleSystemQuad*>("voteEndPartcile0")->stopSystem();
     dynamic_cast<StudioLayer*>(_owner)->getNodeByname<ParticleSystemQuad*>("voteEndPartcile1")->stopSystem();
    auto _layer = dynamic_cast<StudioLayer*>(_owner);
    if(_layer != nullptr){
        _layer->getNodeByName("Particle_1")->setVisible(false);
        _layer->_touchBtnCallback = CC_CALLBACK_1(VoteOperate::touchBtn, this);
        for(int i=0;i<2;i++){
            auto _tex = _layer->getNodeByName((StringUtils::format("vote_tex%d",i)));
            _tex->setVisible(false);
            vote_texs.pushBack(_tex);
            auto _lable = ImageLabel::createWithPlist("nums.plist");
            _lable->setPosition(Vec2(25,98));
            _lable->setName("text");
            _lable->setRotation(-42.25);
            _lable->setScale(0.9);
            _lable->setSpace(-2);
            _tex->addChild(_lable);
        }
          auto _jubp = Sequence::create(ScaleTo::create(0.6, 1.1),ScaleTo::create(0.6, 1), nullptr);
        auto _a1 = RepeatForever::create(_jubp->clone());
        _a1->setTag(521);
        auto _a2 = _a1->clone();
        _a2->setTag(521);
        _layer->getNodeByName("vote_btn0")->runAction(_a1);
        _layer->getNodeByName("vote_btn1")->runAction(_a2);
    }
    auto _bg = _layer->getNodeByName("bg");
    if(_bg != nullptr){
        _bg->setScale(0);
        _bg->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.4, 1)),CallFunc::create(CC_CALLBACK_0(VoteOperate::showLovers, this)),nullptr));
    }else
        showLovers();
  
    
}

void VoteOperate::touchBtn(ui::Widget* widget){
    if((widget->getName() == "vote_btn0" || widget->getName() == "vote_btn1")&&curentNum<endNum){
        if(widget->getName() == "vote_btn0"){
            dynamic_cast<StudioLayer*>(_owner)->getNodeByName("vote_btn1")->stopActionByTag(521);
            dynamic_cast<StudioLayer*>(_owner)->getNodeByName("vote_btn1")->setScale(1);
        } else if(widget->getName() == "vote_btn1"){
            dynamic_cast<StudioLayer*>(_owner)->getNodeByName("vote_btn0")->stopActionByTag(521);
            dynamic_cast<StudioLayer*>(_owner)->getNodeByName("vote_btn0")->setScale(1);
        }
        dynamic_cast<StudioLayer*>(_owner)->getNodeByName("vote_btn0")->getChildByName("touchEnd")->setVisible(true);
        dynamic_cast<StudioLayer*>(_owner)->getNodeByName("vote_btn1")->getChildByName("touchEnd")->setVisible(true);
        auto _particle = dynamic_cast<StudioLayer*>(_owner)->getNodeByName("Particle_1");
         _particle->setVisible(true);
        dynamic_cast<ParticleSystemQuad*>(_particle)->resetSystem();
        _particle->setPositionY(_particle->getPositionY()+60);
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
        int _first = 58+rand()%15;
        if(winIndex == 1)
            _first = 28+rand()%15;
        if(winIndex !=-1)
        dynamic_cast<StudioLayer*>(_owner)->getNodeByname<ParticleSystemQuad*>(StringUtils::format("voteEndPartcile%d",winIndex))->resetSystem();
        SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3",0.2);
        dynamic_cast<ImageLabel*>(vote_texs.at(0)->getChildByName("text"))->setText(StringUtils::format("%d%%",_first));
        vote_texs.at(0)->setVisible(true);
        vote_texs.at(0)->runAction(Sequence::create(ScaleTo::create(0.4, 1.2), ScaleTo::create(0.4, 1),nullptr));
         dynamic_cast<ImageLabel*>(vote_texs.at(1)->getChildByName("text"))->setText(StringUtils::format("%d%%",100-_first));
        vote_texs.at(1)->setVisible(true);
        vote_texs.at(1)->runAction(Sequence::create(ScaleTo::create(0.4, 1.2), ScaleTo::create(0.4, 1),nullptr));
        curentNum++;
        auto _parent = dynamic_cast<StudioLayer*>(_owner);
        auto _loadingBar =  _parent->getNodeByname<ui::LoadingBar*>("loadingBar");
        _loadingBar->setPercent(curentNum/(5*1.0)*100);
        if(curentNum == endNum){
            _owner->addChild(ParticleLayer::create(),30);
            ActionHelper::delayFunc(3, _parent, [](){
                auto _sence = Scene::create();
                _sence->addChild(VoteEndStep::create());
                Director::getInstance()->replaceScene(TransitionFade::create(0.8,_sence,Color3B::WHITE));
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
            });
          
            
        }else
            ActionHelper::delayFunc(2, widget, CC_CALLBACK_0(VoteOperate::showLovers, this));
        
    }else if(widget->getName() == "btn_close"){
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
         auto _operate = dynamic_cast<StudioLayer*>(_owner);
        auto container = _operate->getNodeByName("bg");
        container->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(ScaleTo::create(0.4, 0.3), MoveTo::create(0.4, Vec2(480,320)), nullptr)),CallFunc::create([this](){
            auto _sence = Scene::create();
            auto _end = VoteEndStep::create();
            _end->setPlayIndex(2);
            _sence->addChild(_end);
            Director::getInstance()->replaceScene(TransitionFade::create(0.8,_sence,Color3B::WHITE));
            Director::getInstance()->getEventDispatcher()->setEnabled(true);

            Director::getInstance()->getEventDispatcher()->setEnabled(true);
        }), nullptr));
    }
}

void VoteOperate::showLovers(){
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    auto _uiLayer = dynamic_cast<StudioLayer*>(_owner);
    
    auto _endFun = [this,_uiLayer](){
        dynamic_cast<StudioLayer*>(_owner)->getNodeByName("vote_btn0")->getChildByName("touchEnd")->setVisible(false);
        dynamic_cast<StudioLayer*>(_owner)->getNodeByName("vote_btn1")->getChildByName("touchEnd")->setVisible(false);
        for(auto _tex:vote_texs)
            _tex->setVisible(false);
        voteLayers.clear();
        if(curentNum>=endNum) {
             Director::getInstance()->getEventDispatcher()->setEnabled(true);
        } else {
            int leftIndex = rand()%votePaths.size();
            int rightIndex;
            if(leftIndex<votePaths.size()/2){
                winIndex = 0;
                rightIndex = votePaths.size()/2+ rand()%(votePaths.size()/2);
            }
            else{
                winIndex = 1;
                rightIndex = rand()%(votePaths.size()/2);
            }
            string _paths[2];
            _paths[0] = votePaths.at(leftIndex);
            _paths[1] = votePaths.at(rightIndex);
            votePaths.erase(votePaths.begin() +std::min(leftIndex, rightIndex));
            votePaths.erase(votePaths.begin() +std::max(leftIndex, rightIndex)-1);
            for(int i=0;i<2;i++){
                auto _layer = Sprite::create(_paths[i]);
                _layer->setAnchorPoint(Vec2::ZERO);
                auto _vote = _uiLayer->getNodeByName(StringUtils::format("vote%d",i));
                if(i==0)
                    ActionHelper::showBackInOut(_vote, voteLayerPos.at(i), ActionHelper::ShowDirection::show_from_left,nullptr);
                else
                    ActionHelper::showBackInOut(_vote, voteLayerPos.at(i), ActionHelper::ShowDirection::show_from_right,[](){
                        Director::getInstance()->getEventDispatcher()->setEnabled(true);
                    });
                _vote->addChild(_layer);
                voteLayers.pushBack(_layer);
            }
            
        }
    };
    if(voteLayers.size()>0){
        auto _vote = _uiLayer->getNodeByName(StringUtils::format("vote%d",0));
        _vote->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.3, Vec2(-750,0))), CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, voteLayers.at(0))), nullptr));
        _vote = _uiLayer->getNodeByName(StringUtils::format("vote%d",1));
        _vote->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.3, Vec2(600,0))),CallFunc::create(_endFun), CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, voteLayers.at(1))), nullptr));
    } else{
        for(int i=0;i<2;i++){
            auto _vote = _uiLayer->getNodeByName(StringUtils::format("vote%d",i));
            voteLayerPos.push_back(_vote->getPosition());
        }
        _endFun();
    }
    
}

void VoteOperate::setVotePath(const vector<string>& _vec){
      if(_vec.size()%2==0)
          votePaths = _vec;
}