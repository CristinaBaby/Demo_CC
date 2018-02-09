//
//  RainbowCakePopDipScene.cpp
//  shake5008
//
//  Created by liji on 17/1/10.
//
//

#include "RainbowCakePopDipScene.h"
#include "SceneHeader.h"
#include "ButterNode.h"
#include "MixSpoonNode.h"
#include "ComponentTouchMove.h"
REGIST_SECENE(RainbowCakePopDipScene)

RainbowCakePopDipScene::RainbowCakePopDipScene():
_tipIndex(0),
_bowl(nullptr),
_light(nullptr),
_pot(nullptr)
{

}

RainbowCakePopDipScene::~RainbowCakePopDipScene()
{

}

bool RainbowCakePopDipScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    
    this->setBackground("res/cake_pop/cake_pop_bg1.png");
    
    this->showPot();
//    showDip();
    
    return true;
}

void RainbowCakePopDipScene::showPot()
{
    _pot = Sprite::create("res/cake_pop/step7_chocolate/pot2.png");
    this->addToContentLayer(_pot);
    _pot->setPosition(CMVisibleRect::getPosition(320, 286));
    _pot->setScale(0.9f);

    _bowl = BowlNode::create("res/cake_pop/step7_chocolate/pot.png", "");
    this->addToContentLayer(_bowl,2);
    _bowl->initMaskNode("res/cake_pop/step7_chocolate/bowl_shadow.png",Vec2(0,35));
    
    auto inCall = [=]()
    {
        showChocolate();
        
    };
    ActionHelper::moveInToSceneBounce(_bowl, CMVisibleRect::getPosition(320, 375), kActionDirectionRight,inCall);

    
}

void RainbowCakePopDipScene::showChocolate()
{
    DecorationConfig config = DecorationConfig(
                                               "rainbowcakepop",
                                               "chocolate",
                                               "",
                                               "",
                                               "res/cake_pop/step7_chocolate/chunk/chocolate%d_chunk1.png",
                                               "res/cake_pop/step7_chocolate/chunk/chocolate%d_chunk1.png",
                                               1,
                                               6
                                               );
    
    ScrollViewConfig sConfig = ScrollViewConfig
    (
     Size(getContentSize().width,280),
     "",
     "",
     CMVisibleRect::getPosition(320, 734),
     CMVisibleRect::getPosition(320, 734),
     202+20
     );
    auto callback = [=](ui::Widget* item)
    {
        _scrollView->setVisible(false);
        _scrollView->setEnabled(false);
        
        int itemTag = item->getTag();
        auto food = kESDataManager->getRainbowcakepopFood();
        food.chocolateIndex = itemTag;
        kESDataManager->setRainbowcakepopFood(food);
        auto inBowl = Sprite::create(StringUtils::format("res/cake_pop/step7_chocolate/mix/chocolate%d_1.png",itemTag));
        _bowl->addContentToMask(inBowl, "inBowl", 1);
        inBowl->setOpacity(0);
        
        auto chocolate = ButterNode::createPicChange(StringUtils::format("res/cake_pop/step7_chocolate/chunk/chocolate%d_chunk",itemTag)+"%d.png", Vec2(1, 4));
        this->addToContentLayer(chocolate,10);
        chocolate->setPosition(CMVisibleRect::getPosition(320, 794));
        TipsUIController::getInstance()->showClickTip(this, chocolate->getPosition());
        chocolate->setClickCall([=]
        {
            TipsUIController::getInstance()->removeAllTips();
            inBowl->runAction(FadeIn::create(0.6f));
            kAudioUtil->playEffect(kSoundAddButter);
            auto p = ParticleSystemQuad::create("particle/ice.plist");
            this->addToContentLayer(p,9);
            p->setPosition(CMVisibleRect::getPosition(320, 714));
            p->setDuration(0.4f);
            p->setTexture(_director->getTextureCache()->addImage(StringUtils::format("res/cake_pop/step7_chocolate/chunk/chocolate%d_chunk.png",itemTag)));
            inBowl->setTexture(StringUtils::format("res/cake_pop/step7_chocolate/mix/chocolate%d_%d.png",itemTag,_tipIndex));
            ++_tipIndex;
            if (_tipIndex == 4)
            {
                ActionHelper::moveOutofSceneAndRemoveSelf(chocolate, kActionDirectionRight,[=]
                {
                    showMix();
                });
            }
        });
    };
    this->createScrollView(config,callback,sConfig,false);
}

void RainbowCakePopDipScene::showMix()
{
    if (!_light)
    {
        _light = Sprite::create("res/cake_pop/step7_chocolate/light1.png");
        _pot->addChild(_light,1);
        _light->setPosition(_pot->getContentSize()/2-Size(10,0));
        _light->runAction(RepeatForever::create(ActionHelper::createAnimate("res/cake_pop/step7_chocolate/light%d.png", 1, 2)));
    }
    
    int cIndex = kESDataManager->getRainbowcakepopFood().chocolateIndex;
    auto mix = Sprite::create(StringUtils::format("res/cake_pop/step7_chocolate/mix/chocolate%d_4.png",cIndex));
    _bowl->addContentToMask(mix, "mix", 1,Vec2(0,-40));
    mix->setOpacity(0);

    auto mixSpoon = MixSpoonNode::create("res/cake_pop/step7_chocolate/blender3.png");
    this->addToContentLayer(mixSpoon,3,10);
    mixSpoon->setScale(0.8f);
    mixSpoon->setRotation(20);
    
    auto ingredientsInCall1 = [=]
    {
        
        static bool isFirst = true;
        isFirst = true;
        
        static bool s_firstBlend = true;
        s_firstBlend = true;

        mixSpoon->mixerTouchBegan = [=](int tag)
        {
            _bowl->setContentInMaskVisible("inBowl",nullptr,false,true);

            mixSpoon->mixerTouchBegan = nullptr;
        };
        
        mixSpoon->mixPercentCallback = [=](float percent)
        {
            if (isFirst)
            {
                this->initProgressTimer();
                isFirst = false;
                _bowl->setContentVisible("mix", nullptr, false, false);
                TipsUIController::getInstance()->removeAllTips();
                if (_tempItemVec.size()>0)
                {
                    for (auto temp : _tempItemVec)
                    {
                        temp->runAction(Sequence::create(FadeOut::create(0.3f),
                                                         RemoveSelf::create(),
                                                         NULL));
                    }
                    _tempItemVec.clear();
                }
            }
            SoundConfig::getInstance()->playLoopEffect("Liquid mixing.mp3");
            
            if (percent>random(30, 50))
            {
                if (s_firstBlend)
                {
                    kAudioUtil->playEffect(kVoiceStir1);
                    s_firstBlend = false;
                }
            }
            _progressTimer->setPercentage(percent);
            if(percent > 99)
            {
                this->removeProgressTimer();
                SoundConfig::getInstance()->stopLoopEffect();
                mixSpoon->mixPercentCallback = nullptr;
                //                _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(CornDogMakeScene::showNextStep, this));
                ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoon, ActionDirection::kActionDirectionLeft);
                ActionHelper::moveOutofSceneAndRemoveSelf(_bowl, kActionDirectionLeft,[=]
                {
                    ActionHelper::moveOutofSceneAndRemoveSelf(_pot, kActionDirectionLeft,[=]
                    {
                        showDip();
                    });
                });

            }
        };
        mixSpoon->mixPaused = [=]
        {
            SoundConfig::getInstance()->stopLoopEffect();
        };
        auto boudingbox = _bowl->getBowlRect();
        boudingbox.origin = _bowl->convertToWorldSpace(boudingbox.origin);
        
        std::string format = StringUtils::format("res/cake_pop/step7_chocolate/mix/chocolate%d",cIndex)+("_%d.png");
        Vec2 range = Vec2(4, 7);
        mixSpoon->setEnableAnimation(true);
        mixSpoon->startMix(boudingbox,
                           mix,
                           format,
                           range.x,
                           range.y);
    };
    
    ActionHelper::moveInToSceneBounce(mixSpoon, CMVisibleRect::getPosition(521,694), kActionDirectionLeft,ingredientsInCall1);
    
    
    TipsUIController::getInstance()->showMixTip90Degree(_bowl, Vec2(0, 0));
}

void RainbowCakePopDipScene::showDip()
{
    _bowl = nullptr;
    _bowl = BowlNode::create("res/cake_pop/step8/make_bowl1.png", "res/cake_pop/step8/make_bowl2.png");
    this->addToContentLayer(_bowl);
    Sprite* inbowl = Sprite::create(StringUtils::format("res/cake_pop/step8/chocolate%d_mixture.png",kESDataManager->getRainbowcakepopFood().chocolateIndex));
    _bowl->addContent(inbowl, "inbowl", 1,Vec2(10, 30));
    inbowl->setOpacity(255);
    
    auto inCall = [=]
    {
        auto cakepop = BowlNode::create("res/cake_pop/step8/cake_pop/cake_pop1.png", "");
        this->addToContentLayer(cakepop);
        cakepop->initMaskNode("res/cake_pop/step8/cake_pop/cake_pop1.png");
        for (int i = 0 ; i<4;i++)
        {
            int index = kESDataManager->getRainbowcakepopFood().colorIndexVec.at(i);
            auto cake = Sprite::create(StringUtils::format("res/cake_pop/step4_push/push4_%d.png",index));
            cakepop->addContentToMask(cake, StringUtils::format("m_cake%d",i), 1);
            cake->setPosition(Vec2(8,-20+i*50));
            cake->setScale(1.4f);
        }
        
        auto stick = Sprite::create();
        cakepop->addChild(stick,-1);
        stick->setPosition(Vec2(0, 180));
        stick->setRotation(180);
        int index = kESDataManager->getRainbowcakepopFood().stickIndex;
        auto path = StringUtils::format("res/cake_pop/step6_sticks/stick%d.png",index);
        stick->setTexture(path);
        
        auto digCall =[=]
        {
            kAudioUtil->playEffect(kSoundAddStraw);

            cakepop->runAction(Sequence::create(EaseBackInOut::create(MoveBy::create(0.8f, Vec2(0, -80))),
                                                CallFunc::create([=]
            {
                auto cover = Sprite::create(StringUtils::format("res/cake_pop/step8/cake_pop/cake_pop%d.png",kESDataManager->getRainbowcakepopFood().chocolateIndex));
                cover->setScale(1.05);
                cover->setOpacity(0);
                cakepop->addChild(cover,99);
                cover->runAction(FadeIn::create(0.8f));
                auto temp = Sprite::create(StringUtils::format("res/cake_pop/step8/chocolate%d_mixture1.png",kESDataManager->getRainbowcakepopFood().chocolateIndex));
                temp->setPosition(0, 60);
                _bowl->addChild(temp,10);
                temp->runAction(Sequence::create(ActionHelper::createAnimate(StringUtils::format("res/cake_pop/step8/chocolate%d_mixture",kESDataManager->getRainbowcakepopFood().chocolateIndex)+"%d.png", 1, 3,0.15f),
                                                 RemoveSelf::create(),
                                                 NULL));
            }),
                                                DelayTime::create(0.8f),
                                                Spawn::create(MoveBy::create(1.2f, Vec2(0, 220)),
                                                              RotateBy::create(1.2f, 180),
                                                              NULL),
                                                CallFunc::create([=]
            {
                ActionHelper::moveOutofSceneAndRemoveSelf(_bowl, kActionDirectionLeft,[=]
                {
                    this->stepFinishEffect(cakepop, [=]
                    {
                        SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCakepopDecScene::scene());
                    
                    }, CMVisibleRect::getCenterOfScene());
                });
            }),
                                                NULL));
            
        };
        
        auto cakeInCall = [=]
        {
            TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(320, 400), CMVisibleRect::getPosition(320, 300));
            auto cTouch = ComponentTouchMove::create();
            cTouch->setIsCanMove(false);
            cakepop->addComponent(cTouch);
            static int s_moveDis;
            s_moveDis = 0;
            cTouch->touchMovedCall = [=](Touch* t)
            {
                auto deltaY = t->getDelta().y;
                if(deltaY<0)
                {
                    s_moveDis += deltaY;
                }
                log("======deltaY %f",deltaY);
            };
            cTouch->touchEndedCall = [=](Touch* t)
            {
                if (s_moveDis<-30)
                {
                    TipsUIController::getInstance()->removeAllTips();
                    digCall();
                    cTouch->setEnabled(false);
                }
            };
            
        };
        
        ActionHelper::moveInToSceneBounce(cakepop, CMVisibleRect::getPosition(320, 480), kActionDirectionLeft,cakeInCall);
    };
    
    ActionHelper::moveInToSceneBounce(_bowl, CMVisibleRect::getPosition(320, 306), kActionDirectionLeft,inCall);
    
}
