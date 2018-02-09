//
//  OvenNode.cpp
//  ccats024
//
//  Created by liji on 16/11/30.
//
//

#include "OvenNode.h"
#include "ComponentBase.h"
#include "TipsUIController.h"
#include "SoundConfig.h"
#include "CMVisibleRect.h"
class TouchRotateComponent: public ComponentBase
{
public:
    CREATE_FUNC(TouchRotateComponent);
    
    TouchRotateComponent();
    
    ~TouchRotateComponent();
    
    void start();
    
    /**
     *  true=顺时针，false＝逆时针; default = true;
     */
    CC_SYNTHESIZE(bool, _isClockWiseRotate, IsClockWiseRotate);
    
    /**
     *  旋转目标度数0-360(顺时针为正，逆时针为负)
     */
    CC_SYNTHESIZE(int, _rotateTargetDegree, RotateTargetDegree);
    
    std::function<void()> rotateDoneCall = nullptr;
    
private:
    EventListenerTouchOneByOne* _eventListenerTouchOneByOne = nullptr;
    
    float _degreeRotated = 0;
};

TouchRotateComponent::TouchRotateComponent():
_isClockWiseRotate(true)
{
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
    _eventListenerTouchOneByOne->retain();
    setName(typeid(this).name());
}

TouchRotateComponent::~TouchRotateComponent()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
    CC_SAFE_RELEASE_NULL(_eventListenerTouchOneByOne);
}

void TouchRotateComponent::start()
{
    if(!getOwner())
        return;
    _eventListenerTouchOneByOne->onTouchBegan = [=](Touch* touch, Event* ){
        Point ptInParent = getOwner()->getParent()->convertTouchToNodeSpace(touch);
        bool isTouchIn = getOwner()->getBoundingBox().containsPoint(ptInParent);
        return isTouchIn;
    };
    
    _eventListenerTouchOneByOne->onTouchMoved = [=](Touch* touch, Event* ){
        auto previousLocal = getOwner()->convertToNodeSpaceAR(touch->getPreviousLocation());
        auto positionLocal = getOwner()->convertToNodeSpaceAR(touch->getLocation());
        float degreeDelta = CC_RADIANS_TO_DEGREES(positionLocal.getAngle(previousLocal));
        
        if(degreeDelta > 20)
            return;
        //        if((_isClockWiseRotate && (degreeDelta > 0))
        //           || (!_isClockWiseRotate && degreeDelta < 0))
        //        {
        _degreeRotated += degreeDelta;
        getOwner()->setRotation(getOwner()->getRotation() + degreeDelta);
        //        }
        
        
                SoundConfig::getInstance()->playLoopEffect(kSoundRotateButton);
    };
    
    _eventListenerTouchOneByOne->onTouchEnded = [=](Touch* touch, Event* ){
        __CCLOGWITHFUNCTION("_degreeRotated end:%d", ((int)_degreeRotated % 360 + 360) % 360);
        int rotated = ((int)_degreeRotated % 360 + 360) % 360;
        if(abs(rotated - _rotateTargetDegree) < 15)
        {
            getOwner()->setRotation(getOwner()->getRotation() + (_rotateTargetDegree - rotated));
            _eventListenerTouchOneByOne->setEnabled(false);
            if(rotateDoneCall)
            {
                rotateDoneCall();
            }
            
                        kAudioUtil->playEffect(kSoundOvenDone);
        }
        SoundConfig::getInstance()->stopLoopEffect();
    };
    
    _eventListenerTouchOneByOne->onTouchCancelled = _eventListenerTouchOneByOne->onTouchEnded;
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_eventListenerTouchOneByOne, -1);
}


bool OvenNode::init()
{
    if (!MakeBaseNode::init())
    {
        return false;
    }


    auto oven = Sprite::create("res/cake_pop/step3_oven/oven.png");
    this->addChild(oven,0);
    _ovenDoor = Sprite::create("res/cake_pop/step3_oven/oven_door.png");
    this->addChild(_ovenDoor,50);
    _ovenDoor->setVisible(false);
    
    auto btn1 = Sprite::create("res/cake_pop/step3_oven/oven_btn_l.png");
    auto btn2 = Sprite::create("res/cake_pop/step3_oven/oven_btn_l.png");
    this->addChild(btn1,99);
    btn1->setPosition(Vec2(228+36, 72-20));
    this->addChild(btn2,99);
    btn2->setPosition(Vec2(228+36, -57-25));
    _btnVec.push_back(btn1);
    _btnVec.push_back(btn2);
    return true;
}

void OvenNode::showOvenDoor(bool visible)
{
    _ovenDoor->setVisible(visible);

}


void OvenNode::showButtonTurnNode(std::function<void()> callback)
{
    auto banner = Sprite::create("res/cake_pop/step3_oven/oven_banner.png");
    _director->getRunningScene()->addChild(banner,INT_MAX);
    banner->setPosition(CMVisibleRect::getCenterOfScene());
    banner->setScale(0);
    banner->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1)));

    auto bg = Sprite::create("res/cake_pop/step3_oven/button_banner.png");
    bg->setPosition(banner->getContentSize()/2);
    banner->addChild(bg,-1);

    auto call = [=]
    {
        for (auto b : _btnVec)
        {
            b->setRotation(180);
        }
        
        if (_rotateCount == 0)
        {
            banner->runAction(Sequence::create(ScaleTo::create(0.3f, 0),
                                             CallFunc::create([=]
            {
                if(callback)
                    callback();
            }),
                                               RemoveSelf::create(),
                                             NULL));
        }
    };
    
    auto btn_b1 = Sprite::create("res/cake_pop/step3_oven/oven_btn.png");
    banner->addChild(btn_b1);
    btn_b1->setPosition(247,640-85);
    auto compRotateTemperature = TouchRotateComponent::create();
    compRotateTemperature->setRotateTargetDegree(180);
    btn_b1->addComponent(compRotateTemperature);
    compRotateTemperature->rotateDoneCall = [=]()
    {
//        TipsUIController::getInstance()->removeAllTips();
        compRotateTemperature->setEnabled(false);
        _rotateCount--;
        call();
    };
    compRotateTemperature->start();
//    TipsUIController::getInstance()->showPressTip(banner, Vec2(344, 602-85), Vec2(324, 582-85));
    

    auto btn_b2 = Sprite::create("res/cake_pop/step3_oven/oven_btn.png");
    banner->addChild(btn_b2);
    btn_b2->setPosition(517,640-85);
    auto compRotateTemperature1 = TouchRotateComponent::create();
    compRotateTemperature1->setRotateTargetDegree(180);
    btn_b2->addComponent(compRotateTemperature1);
    compRotateTemperature1->rotateDoneCall = [=]()
    {
        TipsUIController::getInstance()->removeAllTips();
        compRotateTemperature1->setEnabled(false);
        _rotateCount--;
        call();
    };
    compRotateTemperature1->start();
}

void OvenNode::onEnter()
{
    Node::onEnter();

}

void OvenNode::rotateButton(float duration)
{
    _btnVec.at(0)->runAction(RotateTo::create(duration, 0));

}



