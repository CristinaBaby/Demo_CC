//
//  VoteLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "VoteLayer.hpp"
#include "VoteOperate.h"

IMPLEMENT_COCOS2DX_CLASS(VoteLayer);
VoteLayer::~VoteLayer(){
}

bool VoteLayer::init(){
    if(BaseStepLayer::init()){
        
        
        _operate = StudioLayer::create("VoteLayer.csb");
        addChild(_operate);
        
        
        return true;
    }
    return false;
}



void VoteLayer::touchEnd(ui::Widget* widge){
     if(widge->getName() == "btn_ok"){
        auto _tip = _operate->getNodeByName("tips");
        _tip->stopAllActions();
        _tip->runAction(Sequence::create(EaseExponentialIn::create(ScaleTo::create(0.5, 0)),
                                         CallFunc::create([this](){
            auto _layer = StudioLayer::create("ChooseLove.csb");
            addChild(_layer,40);
             _layer->getNodeByName("vote_tex0")->setCascadeOpacityEnabled(false);
               _layer->getNodeByName("vote_tex1")->setCascadeOpacityEnabled(false);
            auto _operate = VoteOperate::create();
            vector<string> paths;
            for(int i=0;i<=19;i++)
                paths.push_back(StringUtils::format("png/donuts/donuts%d.png",i));
            _operate->setVotePath(paths);
            _layer->addComponent(_operate);
        }), nullptr));
    }
}



void VoteLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
        SoundPlayer::getInstance()->playBackGroundMusic("sound/Sound/house-disco-party.mp3");
    auto _vote = _operate->getNodeByName("vote");
    _vote->setVisible(true);
    auto _tip = _operate->getNodeByName("tips");
    _tip->setScale(0);
    _tip->runAction(EaseBackOut::create(ScaleTo::create(0.7, 1)));
}
