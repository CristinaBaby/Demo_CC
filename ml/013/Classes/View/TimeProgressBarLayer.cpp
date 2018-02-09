//
//  TimeProgressBarLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-2-5.
//
//

#include "TimeProgressBarLayer.h"
#include "Config.h"
static const int totalTime = 30;

bool TimeProgressBarLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    
    
    auto bg = Sprite::create("minigame/yellow.png");
    bg -> setAnchorPoint(Vec2(0, 0.5));
    addChild(bg);

    auto timerImage = Sprite::create("minigame/red.png");

    timer_ = ProgressTimer::create(timerImage);
    timer_ -> setPosition(Vec2(24,191));
    timer_ -> setType(ProgressTimer::Type::BAR);
    timer_ -> setMidpoint(Point(0, 0));
    timer_ -> setBarChangeRate(Point(0, 1));
    timer_ -> setPercentage(100.f);
    bg -> addChild(timer_);

    
    
    return true;
}

void TimeProgressBarLayer::pause()
{

    this -> unscheduleUpdate();

}



void TimeProgressBarLayer::countingDown()
{
//    this -> schedule(schedule_selector(TimeProgressBarLayer::count), 0.25);
    this -> scheduleUpdate();
}

void TimeProgressBarLayer::count(float dt)
{
    
    float x = timer_-> getPercentage();
    x-=5/4;
    timer_ -> setPercentage(x);
    if (x <= 0)
    {
        this -> unschedule(schedule_selector(TimeProgressBarLayer::count));
        if(delegate_)
            delegate_ -> timeUp();

    }
}

void TimeProgressBarLayer::update(float dt)
{
    speed_ = 100.f/(totalTime*60);
    float x = timer_-> getPercentage();
    x-=speed_;
    timer_ -> setPercentage(x);
    if (x <= 0)
    {
        this -> unscheduleUpdate();
        if(delegate_)
            delegate_ -> timeUp();
        
    }
}

void TimeProgressBarLayer::loseTime(float pTime)
{
    float x = timer_-> getPercentage();
    x-=speed_*60*pTime;
    timer_ -> setPercentage(x);
    
    auto par = ParticleSystemQuad::create("bluestars.plist");
    par -> setPosition(timer_ -> getPosition());
    
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/losing_star.mp3");

    
    if(x <= 50)
        x = (x-100);
    if(x >= 50)
        x  = x-20;
    
    par -> setPositionY(timer_ -> getPositionY()*x/100);
    log("%f par ",par -> getPositionY());
    log("%f timer_ ",timer_ -> getPositionY());

    this -> addChild(par);
    
}

