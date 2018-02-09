//
//  TransitionAnimateScene.cpp
//  kfood007
//
//  Created by liji on 16/9/8.
//
//

#include "TransitionAnimateScene.h"
#include "SceneHeader.h"

TransitionAnimateScene::TransitionAnimateScene():
_girl(nullptr)
{

}

TransitionAnimateScene::~TransitionAnimateScene()
{

}

bool TransitionAnimateScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    this->setBackground("res/transition/2/bg.png");
    
    auto desk = Sprite::create("res/transition/2/table.png");
    this->addToContentLayer(desk,1);
    desk->setPosition(CMVisibleRect::getPosition(527, 380));

    
    _girl = Sprite::create("res/transition/2/head.png");
    this->addToContentLayer(_girl,2);
    _girl->setPosition(CMVisibleRect::getPosition(789, 357));
    
    auto body = Sprite::create("res/transition/2/boby.png");
    _girl->addChild(body,2);
    body->setPosition(_girl->getContentSize()/2);
    
    auto eye = Sprite::create("res/transition/2/eyes1.png");
    _girl->addChild(eye,2);
    eye->setPosition(_girl->getContentSize()/2);
    
    auto eyeBrow = Sprite::create("res/transition/2/eyebrow_2.png");
    _girl->addChild(eyeBrow,3);
    eyeBrow->setPosition(_girl->getContentSize()/2);
    
    auto hair = Sprite::create("res/transition/2/hair.png");
    _girl->addChild(hair,3);
    hair->setPosition(_girl->getContentSize()/2);
    
    auto mouth = Sprite::create("res/transition/2/mouth_2.png");
    _girl->addChild(mouth,3);
    mouth->setPosition(_girl->getContentSize()/2);

    auto animte = ActionHelper::createAnimate("res/transition/2/eyes%d.png", 1, 2,0.15,true);
    eye->runAction(RepeatForever::create(Sequence::create(animte,
                                                          DelayTime::create(1.2f),
                                                          NULL)));
    
    auto call = [=]
    {
        kAudioUtil->playEffect(kVoiceMakeSlushy);

        eyeBrow->setTexture("res/transition/2/eyebrow.png");
        mouth->setTexture("res/transition/2/mouth_1.png");
        
        eye->stopAllActions();
        auto animte = ActionHelper::createAnimate("res/transition/2/eyes_smile%d.png", 1, 2,0.15,true);
        eye->runAction(RepeatForever::create(Sequence::create(animte,
                                                              DelayTime::create(1.2f),
                                                              NULL)));

        
        vector<Vec2> pos =
        {
            CMVisibleRect::getPosition(586, 316),
            CMVisibleRect::getPosition(490, 359),
            CMVisibleRect::getPosition(343, 499),
        };
        for (int i=1; i<4; i++)
        {
            auto bubble = Sprite::create(StringUtils::format("res/transition/bubble/%d.png",i));
            bubble->setPosition(pos.at(i-1));
            this->addToContentLayer(bubble,2);
            bubble->setScale(0);
            
            if (i==3)
            {
                auto food = Sprite::create("res/transition/bubble/cottoncandy.png");
                food->setPosition(bubble->getContentSize()/2);
                bubble->addChild(food);
            }
            
            
            bubble->runAction(Sequence::create(DelayTime::create(0.4+0.4*i),
                                               CallFunc::create([=]
            {
                if (i==3)
                {
                    auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
                    particle->setPosition(pos.at(i-1));
                    this->addToContentLayer(particle,99);
                }
            }),
                                               EaseBackOut::create(ScaleTo::create(0.6f, 1)),
                                               DelayTime::create(1.8f),
                                               CallFunc::create([=]
            {
                if (i==3)
                {
                    kAudioUtil->playEffect(kVoiceGo);
                }
            }),
                                               DelayTime::create(2.f),
                                               CallFunc::create([=]
            {
                if (i==3)
                {
                }

            
            }),
                                               NULL));
        }
        
        
    };
    
    kAudioUtil->playEffect(kVoiceBoring);
    
    this->runAction(Sequence::create(DelayTime::create(5.f),
                                     CallFunc::create(call),
                                     NULL));
    
    
    
    return true;
}
