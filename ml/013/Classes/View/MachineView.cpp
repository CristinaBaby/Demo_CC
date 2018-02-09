//
//  MachineView.cpp
//  Make
//
//  Created by liji on 15/5/28.
//
//

#include "MachineView.h"
#include "Config.h"

bool MachineView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    {
        listener_ = EventListenerTouchOneByOne::create();
        listener_ -> retain();
        
        listener_ -> onTouchBegan = [&](Touch* touch, Event* event)
        {
            auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
            
            Point locationInNode = target->convertToNodeSpace(touch->getLocation());
            Size s = target->getContentSize();
            Rect rect = Rect(0, 0, s.width, s.height);
            
            if (rect.containsPoint(locationInNode))
            {
                if (target ->getTag() != kTagNone)
                {
                    return true;
                }
            }
            return false;
        };
        
        listener_ -> onTouchMoved = [&](Touch* touch, Event* event)
        {
            //向左滑动屏幕
            if (this -> getTag() == kTagLeft && touch -> getDelta().x <=0 )
            {
                lid_ -> runAction(Sequence::create(
                                                            RotateBy::create(1, -180),
                                                            CallFunc::create(CC_CALLBACK_0(MachineView::showIceBag, this)),
                                                            NULL));
                this -> setTag(kTagNone);
                hint_ -> stopAllActions();
                hint_ -> setVisible(false);
                CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/open the lid.mp3");
                
            }
            //向右滑动屏幕
            if (this -> getTag() == kTagRight && touch -> getDelta().x >=0 )
            {
                lid_ -> runAction(Sequence::create(
                                                            RotateBy::create(1, 180),
                                                            CallFunc::create(CC_CALLBACK_0(MachineView::machineCoverClosed, this)),
                                                            NULL));
                this -> setTag(kTagNone);
                hint_ -> stopAllActions();
                hint_ -> setVisible(false);
                CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/close the lid.mp3");
                
            }
            //向下滑动屏幕
            if (this -> getTag() == kTagDown && touch -> getDelta().y <=0 )
            {
                this -> setTag(kTagNone);
                hint_ -> stopAllActions();
                hint_ -> setVisible(false);

                boxBottom_ -> runAction(MoveBy::create(1, Vec2(0, -108)));
                boxCover_ -> runAction(MoveBy::create(1, Vec2(0, -108)));
                pile_ -> runAction(MoveBy::create(1, Vec2(0, -108)));
                
                CocosDenshion::SimpleAudioEngine::getInstance() -> stopAllEffects();
                auto p = ParticleSystemQuad::create("stars.plist");
                p -> setPosition(Vec2(320, 480));
                this -> addChild(p,10);
                CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/success1.mp3");

                if(delegate_)
                {
                    this -> runAction(Sequence::create(
                                                       DelayTime::create(1),
                                                       CallFunc::create([&]
                    {
                        delegate_ -> allStepFinished();
                    })
                    
                                                       , NULL));
                }
                
            }
            
        };
        
        listener_ -> onTouchEnded = [&](Touch* touch, Event* event)
        {
            auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
            if (target -> getTag() == kTagBox)
            {
                target -> stopAllActions();
                target -> runAction(RotateTo::create(0.1, 0));
                target -> setTag(kTagNone);
                this -> iceBagClicked();
            }
            if (target -> getTag() == kBtnInject)
            {
                target -> setTag(kTagNone);
                target -> setTexture("makeIce/btn_button_h.png");
                this -> runMachine();
            }
            
        };
        
        _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener_, this);
        this -> setTag(kTagLeft);

    
    
    }
    
    
    
    
    this -> initMachine();
    
    return true;

}


void MachineView::initMachine()
{
    auto machineBg = Sprite::create("makeIce/crusher_bg.png");
    machineBg -> setPosition(Vec2(322, 428));
    this -> addChild(machineBg,0);
    
    auto machineBack = Sprite::create("makeIce/crusher2.png");
    machineBack -> setPosition(Vec2(322, 428));
    this -> addChild(machineBack,2);

    
    auto machineTop = Sprite::create("makeIce/crusher.png");
    machineTop -> setPosition(Vec2(322, 428));
    this -> addChild(machineTop,10);

    boxBottom_ = Sprite::create("makeIce/crusher_box.png");
    boxBottom_ -> setPosition(Vec2(318, 344));
    this -> addChild(boxBottom_,1);

    boxCover_ = Sprite::create("makeIce/crusher_boxCover.png");
    boxCover_ -> setPosition(Vec2(318, 344));
    this -> addChild(boxCover_,4);

    lid_ = Sprite::create("makeIce/lid.png");
    lid_ -> setPosition(Vec2(255, 671));
    lid_ -> setAnchorPoint(Vec2(0.05, 0.19));
    this -> addChild(lid_,11);
    
    hint_ = Sprite::create("makeIce/btn_gyrate.png");
    hint_ -> setPosition(404, 772);
    this -> addChild(hint_,12);
    Config::flashAction(hint_, true, 0);

    btn_ = Sprite::create("makeIce/btn_button.png");
    btn_ -> setPosition(320, 496);
    this -> addChild(btn_,10,kBtnInject);
    
    
    if(!pile_)
    {
        pile_ = Sprite::create("makeIce/snow3.png");
        pile_ -> setPosition(Vec2(313, 346));
        this -> addChild(pile_,2);
        auto size = pile_ -> getContentSize();
        pile_ -> setTextureRect(Rect(0, -size.height, size.width, size.height));
        _rectPosY = -size.height;
        _speed = size.height/(_totalTime*10);
        
        log("%f speed is" , _speed);

    }

    
}


void MachineView::showIceBag()
{
    //moveMachine
    this -> runAction(MoveBy::create(0.5, Vec2(0, -200)));
    
    
    //showbag
    iceBag_ = Sprite::create("makeIce/icepack.png");
    iceBag_ -> setPosition(542+700, 909);
    this -> addChild(iceBag_,3,kTagBox);
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener_ -> clone(), iceBag_);
    iceBag_ -> runAction(MoveBy::create(1, Vec2(-700, 0)));
    Config::zoomAction(iceBag_, true, 0);
    
}

void MachineView::iceBagClicked()
{
//    Sprite* ice = Sprite::create("makeIce/ice.png");
//    ice -> setPosition(409, 793);
//    this -> addChild(ice);
    
//    auto move = MoveBy::create(0.5, Vec2(0, -200));
//    ice -> runAction(Sequence::create(move,
//                                      Hide::create()
//                                      , NULL));
    auto ice = ParticleSystemQuad::create("particle/iceParticle.plist");
    ice -> setPosition(409,893);
    ice -> setAutoRemoveOnFinish(true);
    this -> addChild(ice,3);

    
    
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/pour the ice.mp3");
    this -> runAction(Sequence::create(
                                       DelayTime::create(0.7),
                                       CallFunc::create([&]
                                                        {
                                                            this -> runAction(MoveBy::create(0.5, Vec2(0, 200)));
                                                            iceBag_ -> runAction(MoveBy::create(0.5, Vec2(700, 0)));
                                                        }),
                                       DelayTime::create(1),
                                       CallFunc::create([&]
                                                        {
                                                            this -> setTag(kTagRight);
                                                            hint_ -> setVisible(true);
                                                            Config::flashAction(hint_, true, 0);
                                                            hint_ -> setFlippedX(true);
                                                        }),
                                       NULL));
    
}

void MachineView::machineCoverClosed()
{
    hint_ -> setVisible(true);
    hint_ -> setTexture("makeIce/hand.png");
    hint_ -> setPosition(334, 442);
    Config::zoomAction(hint_, false, 1);
    
    this -> runAction(Sequence::create(DelayTime::create(1),
                                       CallFunc::create(CC_CALLBACK_0(MachineView::showMachineBtn, this)),
                                       NULL));
    
}

void MachineView::showMachineBtn()
{
    hint_ -> setVisible(false);
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener_ -> clone(), btn_);
    
}

void MachineView::runMachine()
{
//    for (int i=1; i<4; i++)
//    {
//        __String* str = __String::createWithFormat("makeIce/crushice%d.png",i);
//        Sprite* ice = Sprite::create(str -> getCString());
//        ice -> setPosition(Vec2(308, 341-30*(i-1)));
//        ice -> setVisible(false);
//        this -> addChild(ice);
//        vec_ice.pushBack(ice);
//    }
    
    this -> schedule(schedule_selector(MachineView::iceEffect), 0.5);
    this -> schedule(schedule_selector(MachineView::snowconeFall), 0.1);
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/machine snow cone.mp3");
    
}

void MachineView::iceEffect(float dt)
{
//    //icepile堆积
//    if (iceCount_>2)
//    {
//        iceCount_ = 0;
////        this ->showIcePile();
//    }
//    iceCount_++;
    
    //粒子效果
    auto crush = ParticleSystemQuad::create("particle/crushParticle.plist");
    crush -> setPosition(308,400);
    crush -> setAutoRemoveOnFinish(true);
    this -> addChild(crush,3);
    
}

void MachineView::snowconeFall(float dt)
{
    auto size = pile_ -> getContentSize();
    
    _rectPosY += _speed;
    log("%f _rectPosY is" , _rectPosY);

    pile_ -> setTextureRect(Rect(0, _rectPosY, size.width, size.height));
    if (_rectPosY >=0)
    {
        this -> unschedule(schedule_selector(MachineView::snowconeFall));
        this -> unschedule(schedule_selector(MachineView::iceEffect));
        this -> stepFinish();

    }
}

void MachineView::showIcePile()
{
    static int count = 1;
    __String* str = __String::createWithFormat("makeIce/snow%d.png",count);
    if(!pile_)
    {
        pile_ = Sprite::create(str -> getCString());
        pile_ -> setPosition(Vec2(313, 346));
        this -> addChild(pile_,2);
    }
    pile_ -> setTexture(str -> getCString());
    
    count++;
    if (count > 3)
    {
        count = 1;
        this -> unschedule(schedule_selector(MachineView::iceEffect));
        this -> runAction(Sequence::create(DelayTime::create(0.3),
                                           CallFunc::create([&]
                                                            {
                                                                for (Sprite* ice : vec_ice)
                                                                {
                                                                    ice -> setVisible(false);
                                                                }
                                                                this -> stepFinish();
                                                            }
                                                            )
                                           , NULL));
    }
    
}

void MachineView::stepFinish()
{
    CocosDenshion::SimpleAudioEngine::getInstance() -> stopAllEffects();
    auto p = ParticleSystemQuad::create("stars.plist");
    p -> setPosition(Vec2(320, 480));
    this -> addChild(p,10);
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/success1.mp3");
    
    if(delegate_)
    {
        this -> runAction(Sequence::create(
                                           DelayTime::create(1),
                                           CallFunc::create([&]
                                                            {
                                                                delegate_ -> allStepFinished();
                                                            })
                                           
                                           , NULL));
    }

}

