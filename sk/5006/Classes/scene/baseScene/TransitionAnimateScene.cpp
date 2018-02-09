//
//  TransitionAnimateScene.cpp
//  kfood007
//
//  Created by liji on 16/9/8.
//
//

#include "TransitionAnimateScene.h"
#include "SceneHeader.h"
#include "BowlNode.h"
#include "ComponentTouchMove.h"
#include "JuiceBottleNode.h"
REGIST_SECENE(TransitionAnimateScene);

TransitionAnimateScene::TransitionAnimateScene():
_animationIndex(0)
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
    if (!kESDataManager->getIsFirstAnimation())
    {
        this->showNextButton();
        kAudioUtil->stopAllEffect();
        this->onNextButtonTouchCallback=[=]
        {
            SceneManager::getInstance()->replaceWithDefaultTransition(ChooseFoodScene::scene());
        };
    }

    this->showStore();
    return true;
}

void TransitionAnimateScene::showStore()
{
    this->setBackground("res/story1/story1.png");
    _bgSprite->setScale(1.5f);
    _bgSprite->runAction(ScaleTo::create(11, 1.f));
    
    this->runAction(Sequence::create(CallFunc::create([]
                                                      {
                                                          kAudioUtil->playEffect("sound/voice over1/story1/Welcome to the Rainbow Desserts Bakery.mp3");
                                                      }),
                                     DelayTime::create(5),
                                     CallFunc::create([]
                                                      {
                                                          kAudioUtil->playEffect("sound/voice over1/story1/Such as rainbow cake pops.mp3");
                                                      }),
                                     DelayTime::create(6),
//                                     CallFunc::create([]
//                                                      {
//                                                          kAudioUtil->playEffect("sound/voice over1/story1/Try to mix different colors.mp3");
//                                                      }),
//                                     DelayTime::create(8),
                                     CallFunc::create([=]
                                                      {
                                                          this->showStory();
                                                      }),
                                     NULL));

    
}


void TransitionAnimateScene::showStory()
{
    this->setBackground("res/start2/bg.png");
    
    auto cakepop = Sprite::create("res/start2/cakepop/cakepop_down.png");
    cakepop->setPosition(CMVisibleRect::getPosition(200-600, 762));
    this->addToContentLayer(cakepop,1);
    
    auto shadow1 = Sprite::create("res/start2/cakepop/cakepop_shadow.png");
    shadow1->setPosition(Vec2(190,200));
    cakepop->addChild(shadow1,1);
    _shadowVec.push_back(shadow1);

    auto dec1 = Sprite::create("res/start2/cakepop/cakepop_up.png");
    dec1->setPosition(Vec2(186,218));
    cakepop->addChild(dec1,1);

    auto cupcake = Sprite::create("res/start2/cupcake/cupcake_down.png");
    cupcake->setPosition(CMVisibleRect::getPosition(423+600, 549));
    this->addToContentLayer(cupcake,2);
    
    auto shadow2 = Sprite::create("res/start2/cupcake/cupcake_shadow.png");
    shadow2->setPosition(Vec2(208,214));
    cupcake->addChild(shadow2,2);
    _shadowVec.push_back(shadow2);
    
    auto dec2 = Sprite::create("res/start2/cupcake/cupcake_up.png");
    dec2->setPosition(Vec2(256,229));
    cupcake->addChild(dec2,2);

    auto slushy = Sprite::create("res/start2/slushy/slushy_down.png");
    slushy->setPosition(CMVisibleRect::getPosition(295-600, 273));
    this->addToContentLayer(slushy,3);
    
    auto shadow3 = Sprite::create("res/start2/slushy/slushy_shadow.png");
    shadow3->setPosition(Vec2(172,182));
    slushy->addChild(shadow3,2);
    _shadowVec.push_back(shadow3);
    
    auto dec3 = Sprite::create("res/start2/slushy/slushy_up.png");
    dec3->setPosition(Vec2(139,186));
    slushy->addChild(dec3,2);
    
    
    cakepop->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1.8f, Vec2(600, 0))),
                                        TargetedAction::create(cupcake, EaseBackOut::create(MoveBy::create(1.8f, Vec2(-600, 0)))),
                                        TargetedAction::create(slushy, EaseBackOut::create(MoveBy::create(1.8f, Vec2(600, 0)))),
                                        NULL));
    kAudioUtil->playEffect("sound/voice over1/story1/Try to mix different colors.mp3");
    this->runAction(Sequence::create(DelayTime::create(8.f),
                                     CallFunc::create([=]
    {
        showAnimation();
    }),
                                     NULL));

}


void TransitionAnimateScene::onEnterTransitionDidFinish()
{
    StudioLayer::onEnterTransitionDidFinish();
}

void TransitionAnimateScene::showAnimation()
{
    if (_animationIndex == 0)
    {
        kAudioUtil->playEffect("sound/voice over1/story2/Rainbow cakepop.mp3");
        auto node = Node::create();
        node->setPosition(CMVisibleRect::getPosition(200, 762));
        auto bg1 = Sprite::create("res/start2/cakepop/cakepop0.png");
        node->addChild(bg1,1);
        this->addToContentLayer(node,99);
        node->setScale(0);
    
        
        
        auto shakeCall = [=]
        {
            auto bottle = Sprite::create("res/start2/cakepop/sprinkle_bottle1.png");
            node->addChild(bottle,101);
            bottle->setPosition(Vec2(117+600, 402));
            bottle->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(-600, 0)),
                                               CallFunc::create([=]
                                                                {
                                                                    kAudioUtil->playEffect(kSoundAddSprinkle);
                                                                    bottle->setTexture("res/start2/cakepop/sprinkle_bottle2.png");
                                                                    auto temp = Sprite::create("res/start2/cakepop/sprinkles1.png");
                                                                    bottle->addChild(temp);
                                                                    temp->setPosition(bottle->getContentSize()/2);
                                                                    temp->runAction(Sequence::create(MoveBy::create(1.2f, Vec2(0, -100)),
                                                                                                     Hide::create(),
                                                                                                     CallFunc::create([=]
                                                                                                                      {
                                                                                                                          auto tempcover = Sprite::create("res/start2/cakepop/sprinkles2.png");
                                                                                                                          node->addChild(tempcover,100);
                                                                                                                          tempcover->setOpacity(0);
                                                                                                                          tempcover->runAction(FadeIn::create(0.5f));
                                                                                                                          tempcover->setPosition(3, 161);
                                                                                                                      }),
                                                                                                     NULL));
                                                                }),
                                               ActionHelper::createFloatAction(),
                                               MoveBy::create(1.5f, Vec2(600, 0)),
                                               Hide::create(),
                                               CallFunc::create([=]
                                                                {
                                                                    node->runAction(Sequence::create(Spawn::create(ScaleTo::create(1.2f, 0),
                                                                                                                   MoveTo::create(1.2f, CMVisibleRect::getPosition(200, 762)),
                                                                                                                   RotateBy::create(1.2f, 360),
                                                                                                                   NULL),
                                                                                                     CallFunc::create([=]
                                                                                                                      {
                                                                                                                          _shadowVec.at(_animationIndex)->setVisible(false);
                                                                                                                          _animationIndex++;
                                                                                                                          showAnimation();
                                                                                                                      }),
                                                                                                     NULL));
                                                                }),
                                               NULL));
        };
        node->runAction(Sequence::create(
                                         Spawn::create(ScaleTo::create(1.2f, 1),
                                                       MoveTo::create(1.2f, CMVisibleRect::getCenterOfScene()),
                                                       RotateBy::create(1.2f, 360),
                                                       NULL),
                                         CallFunc::create(shakeCall),
                                         NULL));

    }
    if (_animationIndex == 1)
    {
        kAudioUtil->playEffect("sound/voice over1/story2/Rainbow cupcake.mp3");
        auto node = Node::create();
        node->setPosition(CMVisibleRect::getPosition(423, 549));
        auto bg1 = Sprite::create("res/start2/cupcake/cupcake0.png");
        node->addChild(bg1,1);
        this->addToContentLayer(node,99);
        node->setScale(0);
        
        auto animationCall = [=]
        {
            auto tool = Sprite::create("res/start2/cupcake/cream_1.png");
            node->addChild(tool,101);
            tool->setPosition(Vec2(162+600,260));
            
            int idd = kAudioUtil->playEffect(kSoundSmear,true);
            
            auto moveOut = Sequence::create(Spawn::create(ScaleTo::create(1.2f, 0),
                                                          MoveTo::create(1.2f, CMVisibleRect::getPosition(423, 549)),
                                                          RotateBy::create(1.2f, 360),
                                                          NULL),
                                            CallFunc::create([=]
                                                             {
                                                                 _shadowVec.at(_animationIndex)->setVisible(false);
                                                                 _animationIndex++;
                                                                 showAnimation();
                                                             }),
                                            NULL);
            
            
            tool->runAction(Sequence::create(MoveBy::create(1.5f, Vec2(-600, 0)),
                                             CallFunc::create([=]
                                                              {
                                                                  auto top = Sprite::create("res/start2/cupcake/cream.png");
                                                                  top->setPosition(Vec2(0, 94));
                                                                  node->addChild(top,100);
                                                                  top->setOpacity(0);
                                                                  top->runAction(FadeIn::create(3.5f));
                                                              }),
                                             ActionHelper::moveToPostionByBezier(CMVisibleRect::getPosition(162,260), CMVisibleRect::getPosition(162,260), 1.5f),
                                             ActionHelper::moveToPostionByBezier(CMVisibleRect::getPosition(162,260), CMVisibleRect::getPosition(162,260), 1.5f),
                                             CallFunc::create([=]
            {
                kAudioUtil->stopEffectBySoundId(idd);
            }),
                                             TargetedAction::create(node, moveOut),
                                             NULL));
            
        };
        node->runAction(Sequence::create(
                                         Spawn::create(ScaleTo::create(1.2f, 1),
                                                       MoveTo::create(1.2f, CMVisibleRect::getCenterOfScene()),
                                                       RotateBy::create(1.2f, 360),
                                                       NULL),
                                         CallFunc::create(animationCall),
                                         NULL));
        
    }
    if (_animationIndex == 2)
    {
        kAudioUtil->playEffect("sound/voice over1/story2/Rainbow slushy.mp3");
        auto node = Node::create();
        node->setPosition(CMVisibleRect::getPosition(295, 273));
        auto bg1 = Sprite::create("res/start2/slushy/slushy0.png");
        node->addChild(bg1,1);
        this->addToContentLayer(node,99);
        node->setScale(0);
        
        auto bottle = BowlNode::create("res/start2/slushy/cup1_2.png", "res/start2/slushy/cup1_1.png");
        node->addChild(bottle,10);
        bottle->setPosition(13,77);
        auto juice = Sprite::create("res/start2/slushy/mixture_cup.png");
        bottle->addChild(juice,3);
        auto coverTop = Sprite::create("res/start2/slushy/cup1_top1.png");
        bottle->addChild(coverTop,4);

        
        
        auto animationCall = [=]
        {
            auto tool = Sprite::create("res/start2/slushy/straw1.png");
            bottle->addChild(tool,2);
            tool->setPosition(Vec2(0,60+600));
            tool->setScale(0.8f);
            
            auto moveOut = Sequence::create(Spawn::create(ScaleTo::create(1.2f, 0),
                                                          MoveTo::create(1.2f, CMVisibleRect::getPosition(295, 273)),
                                                          RotateBy::create(1.2f, 360),
                                                          NULL),
                                            CallFunc::create([=]
                                                             {
                                                                 _shadowVec.at(_animationIndex)->setVisible(false);
                                                                 _animationIndex++;
                                                                 showAnimation();
                                                             }),
                                            NULL);
            auto water = Sprite::create("res/start2/slushy/water.png");
            bottle->addChild(water,2);
            water->setScale(0);
            
            tool->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1.5f, Vec2(0, -600))),
                                             CallFunc::create([]
            {
                kAudioUtil->playEffect(kSoundAddStraw);
            }),
                                             TargetedAction::create(water, EaseBackOut::create(ScaleTo::create(0.8f, 1))),
                                             TargetedAction::create(water, FadeOut::create(2.5f)),
                                             TargetedAction::create(node, moveOut),
                                             NULL));
            
        };
        node->runAction(Sequence::create(
                                         Spawn::create(ScaleTo::create(1.2f, 1),
                                                       MoveTo::create(1.2f, CMVisibleRect::getCenterOfScene()),
                                                       RotateBy::create(1.2f, 360),
                                                       NULL),
                                         CallFunc::create(animationCall),
                                         NULL));
        
    }
    if (_animationIndex == 3)
    {
        SceneManager::getInstance()->replaceWithDefaultTransition(ChooseFoodScene::scene());
    }

}






