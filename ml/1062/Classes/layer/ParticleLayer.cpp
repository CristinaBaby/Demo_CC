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
    auto s = Sprite::create("png/animation/donhua_1.png");
    s->setPosition(getContentSize()*.5);
    addChild(s);
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.25);
    for(int i=1;i<=8;i++)
        anim->addSpriteFrameWithFile(StringUtils::format("png/animation/donhua_%d.png",i));
    s->runAction(Sequence::create(Animate::create(anim),DelayTime::create(0.25), RemoveSelf::create(), nullptr));
//    for(int i=0;i<9;i++){
//        this->runAction(Sequence::create(DelayTime::create(0.4*i),CallFunc::create([this](){
//           
//            auto* effect = ParticleSystemQuad::create("particles/powerStar.plist");
//        
//            effect->setPosition(Vec2(60+rand()%820,200+rand()%485));
//            this->addChild(effect);
//        }), nullptr));
//    }
  
}