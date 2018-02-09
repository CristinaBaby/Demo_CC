//
//  ParticleLayer.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/31.
//
//

#include "ParticleLayer.h"
#include "SoundPlayer.h"

void ParticleLayer::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
    SoundPlayer::getInstance()->playEffect("sound/Sound/cheer_star.mp3");
    for(int i=0;i<9;i++){
        this->runAction(Sequence::create(DelayTime::create(0.4*i),CallFunc::create([this](){
           
            auto* effect = ParticleSystemQuad::create("particle/powerStar.plist");
        
            effect->setPosition(Vec2(60+rand()%820,200+rand()%485));
            this->addChild(effect);
        }), nullptr));
    }
  
}