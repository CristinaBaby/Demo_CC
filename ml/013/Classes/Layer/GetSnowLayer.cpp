//
//  GetSnowLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#include "GetSnowLayer.h"
#include "GameMaster.h"
#include "IAPManager.h"
#include "Config.h"
#include "KSVisibleRect.h"

Scene* GetSnowLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(GetSnowLayer::create());
    return pScene;
    
}


bool GetSnowLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    snowCount_ = 1;
    canFillSnow_ = false;

    this -> setBackground("start/default_bg.png");

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
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        if (target -> getTag() == kTagSpoon)
        {
            target -> setPosition(target -> getPosition() + touch -> getDelta());
            if (snowInMachine_ -> getBoundingBox().containsPoint(spoon_ -> getPosition()))
            {
                snowInSpoon_ -> setVisible(true);
                spoon_ -> setZOrder(10);
                snowInMachine_ -> setTexture("getSnow/box_ice.png");
                canFillSnow_ = true;
                boxCover_ -> setOpacity(50);
                
                auto size = snowInMachine_ -> getContentSize();
                
//                if (snowCount_ == 3) {
//                    
//                }
//                else
                {
                    snowInMachine_ -> setTextureRect(Rect(0, -size.height/3*snowCount_, size.width, size.height));
                }
            }
            if (cup_ -> getBoundingBox().containsPoint(spoon_ -> getPosition()) && canFillSnow_)
            {
                snowInSpoon_ -> setVisible(false);
                spoon_ -> setZOrder(10);
                auto size = snowInMachine_ -> getContentSize();
                snowInMachine_ -> setTextureRect(Rect(0, -size.height/3*snowCount_, size.width, size.height));
                boxCover_ -> setOpacity(255);
                this -> fillSnow();
            }
        }
        if (snowCount_ > 3)
        {

        }
    };
    
    listener_ -> onTouchEnded = [&](Touch* touch, Event* event)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
    };

    
    
    this -> initUI();
    
    return true;
}

void GetSnowLayer::initUI()
{
    boxBottom_ = Sprite::create("getSnow/box_down.png");
    boxBottom_ -> setPosition(KSVisibleRect::getPosition(318-500, 708.5+50));
    this -> addChild(boxBottom_,1);

    boxCover_ = Sprite::create("getSnow/box_up.png");
    boxCover_ -> setPosition(KSVisibleRect::getPosition(318-500, 708.5+50));
    this -> addChild(boxCover_,4);
    boxCover_ -> setVisible(false);
    
    cup_ = Sprite::create("getSnow/default_cup.png");
    cup_ -> setPosition(KSVisibleRect::getPosition(309, -75));
    this -> addChild(cup_);

    snowInMachine_ = Sprite::create("getSnow/box_ice.png");
    snowInMachine_ -> setPosition(KSVisibleRect::getPosition(313-500, 593));
    snowInMachine_ -> setAnchorPoint(Vec2(0.5, 0));
    this -> addChild(snowInMachine_,3);
    
    snowInCup_ = Sprite::create();
    snowInCup_ -> setPosition(KSVisibleRect::getPosition(309, 253));
    this -> addChild(snowInCup_,2);

    spoon_ = Sprite::create("getSnow/shovel.png");
    spoon_ -> setPosition(KSVisibleRect::getPosition(321 , 745+200));
    spoon_ -> setAnchorPoint(Vec2(0.21, 0.21));
    this -> addChild(spoon_,2,kTagSpoon);
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener_, spoon_);
    
    snowInSpoon_ = Sprite::create("getSnow/shovel2.png");
    snowInSpoon_ -> setAnchorPoint(Vec2::ZERO);
    spoon_ -> addChild(snowInSpoon_);
    snowInSpoon_ -> setVisible(false);
}

void GetSnowLayer::fillSnow()
{
    if (!canFillSnow_ || snowCount_ >3)
    {
        return;
    }
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/scoop shaved ice.mp3");


    auto snowInCupStr = __String::createWithFormat("getSnow/cup_snow%d.png",snowCount_);
    snowInCup_ -> setTexture(snowInCupStr -> getCString());


    
    snowCount_++;
    canFillSnow_ = false;
    
    if (snowCount_ > 3 )
    {
        spoon_ -> setTag(kTagNone);
        spoon_ -> runAction(MoveBy::create(0.5, Vec2(600, 0)));
        nextBtn_ -> setVisible(true);
        auto p = ParticleSystemQuad::create("stars.plist");
        p -> setPosition(KSVisibleRect::getPosition(320, 480));
        this -> addChild(p,10);
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/success1.mp3");

    }

}


void GetSnowLayer::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();
    
    GameMaster::getInstance() -> goToChooseCupLayer();
}


void GetSnowLayer::onEnter()
{
    BaseLayer::onEnter();
    
    boxBottom_ -> runAction(MoveBy::create(0.5, Vec2(500, 0)));
    boxCover_ -> runAction(MoveBy::create(0.5, Vec2(500, 0)));
    snowInMachine_ -> runAction(MoveBy::create(0.5, Vec2(500, 0)));
    
    spoon_ -> runAction(Sequence::create(
                                         DelayTime::create(0.5),
                                         MoveBy::create(0.5, Vec2(0, -200)),
                                         NULL));

}

void GetSnowLayer::onExit()
{
    BaseLayer::onExit();
    
}
