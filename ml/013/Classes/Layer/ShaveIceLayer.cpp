//
//  ShaveIceLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#include "ShaveIceLayer.h"
#include "GameMaster.h"
#include "IAPManager.h"
#include "Config.h"
#include "KSVisibleRect.h"
#include "AudioController.h"

Scene* ShaveIceLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(ShaveIceLayer::create());
    return pScene;
    
}


bool ShaveIceLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    shaveCount_ = 0;
    shaveSnow_ = nullptr;
    this ->  setBackground("makeIce/bg2.png");

    
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
        auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        if (target -> getTag() == kTagKnife )
        {
            
            Rect rect = Rect(0, 100, 640, 600);
            
            Vec2 afterP = target->getPosition()+touch->getDelta();
            if (rect.containsPoint( afterP)) {
                target -> setPosition(target -> getPosition() + touch -> getDelta());
            }
        }
        if (basicPile_->getBoundingBox().containsPoint(target -> getPosition()))
        {
            static int dt = 0;
            dt++;
            
            showShaveSnowEffect();

            AudioController::getInstance() -> playLoopEffect("sound/scrape ice.mp3");
            
            if (dt % 6 == 0)
            {
                shaveCount_++;
            }
            
            basicPile_ -> setOpacity(255-shaveCount_*3);
            pile_ -> setOpacity(shaveCount_*3>=255? 255:shaveCount_*3);
            if (shaveCount_*3 >= 255)
            {
                
                this -> stopShaveSnowEffect();
                basicPile_ -> setOpacity(0);
                knife_ -> setTag(kTagNone);
                knife_ -> runAction(Sequence::create(
                                                     MoveBy::create(0.5, Vec2(500, 0)),
                                                     
                                                     TargetedAction::create(this,                                                      CallFunc::create([&]
                {
                    nextBtn_ -> setVisible(true);
                    auto p = ParticleSystemQuad::create("stars.plist");
                    p -> setPosition(KSVisibleRect::getPosition(320, 480));
                    this -> addChild(p,10);
                    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/success1.mp3");

                }
)
                                                                            ),
                                                     NULL));
            }
        }
        
        else
        {
        
            this -> stopShaveSnowEffect();

        }
    };
    listener_ -> onTouchEnded = [&](Touch* touch, Event* event)
    {
    
        this -> stopShaveSnowEffect();
        AudioController::getInstance() -> stopLoopEffect();

    };

    this -> initSnowCone();
    
    return true;
}


void ShaveIceLayer::initSnowCone()
{
    Sprite* cup = Sprite::create(GameMaster::getInstance() -> getCupName() -> getCString());
    cup -> setPosition(KSVisibleRect::getPosition(319, 146.5));
    this -> addChild(cup,5);

    
    
    pile_ = Sprite::create(Config::getShapeName());
    pile_ -> setPosition(KSVisibleRect::getPosition(Config::getSnowconePos().x, Config::getSnowconePos().y));
    pile_ -> setOpacity(0);
    this -> addChild(pile_,6);
    
    basicPile_ = Sprite::create("choose shape/basis_shape.png");
    basicPile_ -> setPosition(KSVisibleRect::getPosition(310, 513));
    this -> addChild(basicPile_,7);
    
    knife_ = Sprite::create("choose shape/knife.png");
    knife_ -> setPosition(KSVisibleRect::getPosition(310, 513));
    this -> addChild(knife_,8,kTagKnife);
    
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener_ -> clone(), knife_);

}

void ShaveIceLayer::showShaveSnowEffect()
{
    if (!shaveSnow_ )
    {
        shaveSnow_ = ParticleSystemQuad::create("shaveSnow.plist");
        shaveSnow_ -> setPosition(Vec2(29, 148));
        knife_ -> addChild(shaveSnow_);
    }
    shaveSnow_ -> setVisible(true);
}

void ShaveIceLayer::stopShaveSnowEffect()
{
    if (shaveSnow_ )
        shaveSnow_ -> setVisible(false);
 
}



void ShaveIceLayer::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();
    
    GameMaster::getInstance() -> goToDecorationLayer();
}


void ShaveIceLayer::onEnter()
{
    BaseLayer::onEnter();
}

void ShaveIceLayer::onExit()
{
    BaseLayer::onExit();
    
}
