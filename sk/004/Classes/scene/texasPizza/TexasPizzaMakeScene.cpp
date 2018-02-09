//
//  TexasPizzaMakeScene.cpp
//  shake004
//
//  Created by liji on 16/10/17.
//
//

#include "TexasPizzaMakeScene.h"
#include "BowlNode.h"
#include "ComponentTouchMove.h"
#include "MakeNodeManager.h"
#include "ScribbleNode.h"
#include "ComponentScribblePercent.h"
#include "ComponetFade.h"
#include "ESDataManager.h"
#include "SceneHeader.h"
#include "SSCInternalLibManager.h"
#include "DialogLayer.h"
#include "RewardManager.h"
#include "LockUIManager.h"
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
        
        
//        SoundConfig::getInstance()->playLoopEffect(kSoundCakeTurnButton);
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
            
//            kAudioUtil->playEffect(kSoundCakeBakeDone);
        }
        SoundConfig::getInstance()->stopLoopEffect();
    };
    
    _eventListenerTouchOneByOne->onTouchCancelled = _eventListenerTouchOneByOne->onTouchEnded;
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_eventListenerTouchOneByOne, -1);
}



TexasPizzaMakeScene::TexasPizzaMakeScene():
_plate(nullptr),
_tempBread(nullptr),
_breadCount(3),
_brush(nullptr),
_oven(nullptr),
_door(nullptr),
_rotater(nullptr),
_progressTimer(nullptr),
_progressBg(nullptr)
{

}

TexasPizzaMakeScene::~TexasPizzaMakeScene()
{

}

bool TexasPizzaMakeScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    
    _screenEventName = "food0.0_movienight_texastoastpizza";
    this->setBackground("res/make_bg.png");
    
    _plate = BowlNode::create("res/pizza/pizza/overlook/bakeware_down.png", "");
    this->addToContentLayer(_plate,10);
    
    auto cover = Sprite::create("res/pizza/pizza/overlook/bakeware_up.png");
    _plate->addChild(cover,100);
    cover->setPosition(0, -163);
    
    ActionHelper::moveInToSceneBounce(_plate, CMVisibleRect::getPosition(320, 300), kActionDirectionRight,CC_CALLBACK_0(TexasPizzaMakeScene::addBread, this));

    
    return true;
}

void TexasPizzaMakeScene::addBread()
{
    kAudioUtil->playEffect(kVoicePizzaDragBread);
    
    auto bag = Sprite::create("res/pizza/bag.png");
    this->addToContentLayer(bag,2);
    bag->setScale(0.5f);
    MakeNodeManager::getInstance()->registerRemoveNode(bag, kActionDirectionRight);
    
    ActionHelper::moveInToSceneBounce(bag, CMVisibleRect::getPosition(326, 591), kActionDirectionLeft,[=]
    {
        TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(326, 591), CMVisibleRect::getPosition(326, 300));
    });
    
    auto cTouch = ComponentTouchMove::create();
    cTouch->setIsCanMove(false);
    cTouch->touchBeganCall = [=](Touch* t)
    {
        if (_tempBread) {
            _tempBread->removeFromParentAndCleanup(true);_tempBread=nullptr;
        }
        TipsUIController::getInstance()->removeAllTips();
        _tempBread = Sprite::create("res/pizza/garlicbread.png");
        this->addToContentLayer(_tempBread,11);
        _tempBread->setScale(0.f);
        
        _tempBread->runAction(EaseBackOut::create(ScaleTo::create(0.2f, 0.5f)));
        _tempBread->setPosition(t->getLocation());
        
    };
    cTouch->touchMovedCall = [=](Touch* t)
    {
        _tempBread->setPosition(t->getLocation());
    };
    cTouch->touchEndedCall = [=](Touch* t)
    {

        if (_plate->getBowlRect().containsPoint(_plate->convertToNodeSpace(t->getLocation())))
        {
            this->addBreadInPlate();
            if (_tempBread) {
                _tempBread->removeFromParentAndCleanup(true);_tempBread=nullptr;
            }
        }
        else
        {
            _tempBread->runAction(EaseBackIn::create(ScaleTo::create(0.2f, 0.f)));
        }
    };
    bag->addComponent(cTouch);
    
}

void TexasPizzaMakeScene::addBreadInPlate()
{
    vector<Vec2> posVec =
    {
        Vec2(-127, 46),
        Vec2(129, 46),
        Vec2(0, -80),
    };
    
    auto breadInPlate = Sprite::create("res/pizza/pizza/overlook/beforebake/garlicbread_1.png");
    _plate->addContent(breadInPlate, "bread", 1);
    breadInPlate->setTag(3-_breadCount);
    breadInPlate->setScale(0.85, 0.9);
    breadInPlate->setPosition(posVec.at(3-_breadCount));
    breadInPlate->setOpacity(255);
    static int s_index = 0;
    if (++s_index<=3)
    {
        breadInPlate->setTag(s_index);
    }
    else
    {
        s_index = 0;
    }
    _breadInPlateVec.push_back(breadInPlate);
    
    _breadCount--;
    if (_breadCount == 0)
    {
        _breadCount = 3;
        MakeNodeManager::getInstance()->removeAllRegisterNodes(CC_CALLBACK_0(TexasPizzaMakeScene::addScribble, this));
    }
}


void TexasPizzaMakeScene::addScribble()
{
    kAudioUtil->playEffect(kVoicePizzaSauce);

    auto scribbleBowl = BowlNode::create("res/pizza/Seasoning/bowl_down.png", "res/pizza/Seasoning/bowl_up.png");
    this->addToContentLayer(scribbleBowl,20);
    auto word = Sprite::create("res/pizza/Seasoning/sauce.png");
    scribbleBowl->addChild(word,100);
    word->setPosition(0, -30);
    
    auto sauceDown = Sprite::create("res/pizza/Seasoning/bowl_sauce2.png");
    scribbleBowl->addChild(sauceDown,2);
    sauceDown->setAnchorPoint(Vec2(0.5, 0));
    sauceDown->setScale(1,0.7);
    
    auto sauceUp = Sprite::create("res/pizza/Seasoning/bowl_sauce1.png");
    scribbleBowl->addChild(sauceUp,4);
    sauceUp->setAnchorPoint(Vec2(0.5, 0));
    sauceUp->setScale(1,0.7);

    _brush = Sprite::create("res/pizza/Seasoning/brush_2.png");
    scribbleBowl->addChild(_brush,3);
    _brush->setAnchorPoint(Vec2(0.5, 0.15));
    _brush->setPosition(19, 62);
    _brush->setScale(0.9f);

    ActionHelper::moveInToSceneBounce(scribbleBowl, CMVisibleRect::getPosition(320, 521), kActionDirectionLeft,CC_CALLBACK_0(TexasPizzaMakeScene::scribbleSauce, this));

    MakeNodeManager::getInstance()->registerRemoveNode(scribbleBowl, kActionDirectionLeft);
    
}

void TexasPizzaMakeScene::scribbleSauce()
{
    for (auto breadInPlate : _breadInPlateVec)
    {
        auto sauceScribble = ScribbleNode::create(214,105);
        breadInPlate->addChild(sauceScribble,2);
        sauceScribble->setPosition(Vec2(123,112));
        sauceScribble->useTarget("res/pizza/pizza/overlook/beforebake/sauce_1.png");
        sauceScribble->useBrush("res/solid_brush.png", Scribble::eBrush);
        _scribbleVec.push_back(sauceScribble);
        
        auto compScribbleComplete = ComponentScribblePercent::create();
        sauceScribble->addComponent(compScribbleComplete);
        compScribbleComplete->setScribbleTargetPath("res/pizza/pizza/overlook/beforebake/sauce_1.png");
        compScribbleComplete->setIsScribbleBrushTypeEraser(false);
        compScribbleComplete->start();
        _percentVec.pushBack(compScribbleComplete);

    }
    
    const float completePercent = 0.85;

    auto compTouch = ComponentTouchMove::create();
    
    compTouch->touchBeganCall = [=](Touch* t)
    {
        _brush->setZOrder(102);
        _brush->runAction(EaseBackOut::create(ScaleTo::create(0.1f, 1.f)));
    };
    compTouch->touchMovedCall = [=](Touch* t)
    {
        auto startPt = t->getLocation();
        for (auto sauceScribble : _scribbleVec)
        {
            sauceScribble->paint(this->convertToWorldSpaceAR(_brush->getPosition()));
        }
        
        
    };
    compTouch->touchEndedCall = [=](Touch* t)
    {
        
        SoundConfig::getInstance()->stopLoopEffect();
        for (auto compScribbleComplete : _percentVec)
        {
            if(compScribbleComplete->getPercentComplete() > completePercent)
            {
                
                //scribble done
                auto sauceInBread = Sprite::create("res/pizza/pizza/overlook/beforebake/sauce_1.png");
                compScribbleComplete->getOwner()->addChild(sauceInBread,1);

                dynamic_cast<ScribbleNode*>(compScribbleComplete->getOwner())->getCanvas()->clear(0, 0, 0, 0);
//                dynamic_cast<ScribbleNode*>(compScribbleComplete->getOwner())->paint("res/pizza/pizza/overlook/beforebake/sauce_1.png");
                compScribbleComplete->stop();
                _percentVec.eraseObject(compScribbleComplete);
                
            }
        }
        
        if (_percentVec.size() == 0)
        {
            compTouch->stop();
            MakeNodeManager::getInstance()->removeAllRegisterNodes(CC_CALLBACK_0(TexasPizzaMakeScene::addCheese, this));
        }

    };
    
    
    _brush->addComponent(compTouch);
}


void TexasPizzaMakeScene::addCheese()
{
    kAudioUtil->playEffect(kVoicePizzaAddCheese);
    

    auto cheeseBowl = BowlNode::create("res/pizza/Seasoning/bowl_down.png", "res/pizza/Seasoning/bowl_up.png");
    this->addToContentLayer(cheeseBowl,20);
    auto word = Sprite::create("res/pizza/Seasoning/cheese.png");
    cheeseBowl->addChild(word,100);
    word->setPosition(0, -30);
    
    auto sauceDown = Sprite::create("res/pizza/Seasoning/bowl_cheese1.png");
    cheeseBowl->addChild(sauceDown,2);
    sauceDown->setPosition(0, 44);
    auto call = [=]
    {
        auto cTouch = ComponentTouchMove::create();
        cTouch->setIsCanMove(false);
        cTouch->touchBeganCall = [=](Touch* t)
        {
            if (_tempBread) {
                _tempBread->removeFromParentAndCleanup(true);_tempBread=nullptr;
            }
            
            _tempBread = Sprite::create("res/pizza/Seasoning/bowl_cheese2.png");
            this->addToContentLayer(_tempBread,21);
            _tempBread->setScale(0.f);
            
            _tempBread->runAction(EaseBackOut::create(ScaleTo::create(0.2f, 0.5f)));
            _tempBread->setPosition(t->getLocation());
            
        };
        cTouch->touchMovedCall = [=](Touch* t)
        {
            _tempBread->setPosition(t->getLocation());
        };
        cTouch->touchEndedCall = [=](Touch* t)
        {
            
//            if (_plate->getBowlRect().containsPoint(_plate->convertToNodeSpace(t->getLocation())))
//            {
//                this->addBreadInPlate();
//                if (_tempBread) {
//                    _tempBread->removeFromParentAndCleanup(true);_tempBread=nullptr;
//                }
//            }
//            else
                _tempBread->runAction(EaseBackIn::create(ScaleTo::create(0.2f, 0.f)));
            for (auto inplate : _breadInPlateVec)
            {
                if (inplate->getBoundingBox().containsPoint(_plate->convertToNodeSpace(t->getLocation())) && !inplate->getChildByName("cheese"))
                {
                    
                    
                    auto cheeseInBread = Sprite::create("res/pizza/pizza/overlook/beforebake/cheese_1.png");
                    inplate->addChild(cheeseInBread,2);
                    cheeseInBread->setPosition(Vec2(123,112));
                    cheeseInBread->setOpacity(0);
                    cheeseInBread->runAction(FadeIn::create(1.2f));
                    cheeseInBread->setName("cheese");
                    _cheeseOnBreadVec.push_back(cheeseInBread);
                    auto particle = ParticleSystemQuad::create("particle/cheese.plist");
                    inplate->addChild(particle,99);
                    particle->setPosition(Vec2(124, 202));
                    
                    static int s_cheeseCount = 0;
                    s_cheeseCount++;
                    if (s_cheeseCount == 3)
                    {
                        s_cheeseCount = 0;
                        MakeNodeManager::getInstance()->removeAllRegisterNodes(CC_CALLBACK_0(TexasPizzaMakeScene::addOther, this));
                    }
                }
            }
        };
        cheeseBowl->addComponent(cTouch);
    };
    this->runAction(Sequence::create(DelayTime::create(3.5f),
                                     CallFunc::create([=]
                                                      {
                                                          kAudioUtil->playEffect(kVoicePizzaDragOther);
                                                          call();
                                                      }),
                                     NULL));

    
    ActionHelper::moveInToSceneBounce(cheeseBowl, CMVisibleRect::getPosition(320, 521), kActionDirectionLeft);
    
    MakeNodeManager::getInstance()->registerRemoveNode(cheeseBowl, kActionDirectionLeft);
}

void TexasPizzaMakeScene::addOther()
{
    kAudioUtil->playEffect(kVoicePizzaAddOther);
    
    
    auto chooseNode = Sprite::create("ui/dec/bg.png");
    chooseNode->setAnchorPoint(Vec2(0.5,1));
    this->addToUILayer(chooseNode,2);
    chooseNode->setPosition(CMVisibleRect::getPosition(320,130-300,kBorderNone,kBorderTop));
    chooseNode->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, -300)),
                                           CallFunc::create([=]
    {
        TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(320,130,kBorderNone,kBorderTop), _plate->getPosition());
    }),
                                           NULL));
    this->onNextButtonTouchCallback=[=]
    {
        this->disableOther();
        this->showOven();
        this->hideNextButton();
        chooseNode->removeFromParent();
    };

    
    for (int i = 0; i<4; i++)
    {
        auto icon = Sprite::create(StringUtils::format("res/pizza/pizza/overlook/icon/%d.png",i+1));
        icon->setPosition(Vec2(56+81/2+(81+56)*i, chooseNode->getContentSize().height/2));
        chooseNode->addChild(icon,200);
        
        auto cTouch = ComponentTouchMove::create();
        cTouch->setIsCanMove(false);
        cTouch->setName(StringUtils::format("%d",i+1));
        cTouch->touchBeganCall = [=](Touch* t)
        {
            TipsUIController::getInstance()->removeAllTips();
            if (_tempBread) {
                _tempBread->removeFromParentAndCleanup(true);_tempBread=nullptr;
            }
            
            _tempBread = Sprite::create(StringUtils::format("res/pizza/pizza/overlook/beforebake/other/%d.png",atoi(cTouch->getName().c_str())));
            this->addToContentLayer(_tempBread,11);
            _tempBread->setScale(0.f);
            
            _tempBread->runAction(EaseBackOut::create(ScaleTo::create(0.2f, 0.5f)));
            _tempBread->setPosition(t->getLocation());
            
        };
        cTouch->touchMovedCall = [=](Touch* t)
        {
            this->showNextButton();

            
            _tempBread->setPosition(t->getLocation());
        };
        cTouch->touchEndedCall = [=](Touch* t)
        {
            Rect r = Rect(43, 84, 141, 54);
            
            for (auto bread : _breadInPlateVec)
            {
                auto pos = bread->convertToNodeSpace(t->getLocation());

                if (r.containsPoint(pos))
                {
                    auto temp = Sprite::create(StringUtils::format("res/pizza/pizza/overlook/beforebake/other/%d.png",atoi(cTouch->getName().c_str())));
                    bread->addChild(temp,10);
                    temp->setName(StringUtils::format("%d",atoi(cTouch->getName().c_str())));
                    temp->setPosition(pos);
                    _otherOnBreadVec.push_back(temp);
                    
                    auto cTouch = ComponentTouchMove::create();
                    temp->addComponent(cTouch);
                    cTouch->setName("otherMove");
                    Rect nR = r;
                    nR.origin = bread->convertToWorldSpace(r.origin);
                    cTouch->setMoveArea(nR);
                }
            }
            _tempBread->setVisible(false);
        };
        icon->addComponent(cTouch);

    }
}

void TexasPizzaMakeScene::disableOther()
{
//    for (auto other : _otherOnBreadVec)
//    {
//        if (other->getComponent("otherMove"))
//        {
//            dynamic_cast<ComponentTouchMove*>(other->getComponent("otherMove"))->setEnabled(false);
//        }
//    }
    
    for (auto other : _otherOnBreadVec)
    {
        other->removeAllComponents();
    }

}

void TexasPizzaMakeScene::showOven()
{

    _oven = Sprite::create("res/pizza/oven/oven.png");
    this->addToContentLayer(_oven,2);
    
    auto doorClose = Sprite::create("res/pizza/oven/oven_close.png");
    _oven->addChild(doorClose,10);
    doorClose->setAnchorPoint(Vec2(0.5, 0));
    doorClose->setPosition(234,40);
    
    _rotater = Sprite::create("res/pizza/oven/button.png");
    _rotater->setPosition(485, 223);
    _oven->addChild(_rotater,8);
   
    auto rotaterDown = Sprite::create("res/pizza/oven/button.png");
    rotaterDown->setPosition(485, 96);
    _oven->addChild(rotaterDown,8);

    
    _door = Sprite::create("res/pizza/oven/oven_open.png");
    this->addToContentLayer(_door,9);
    _door->setAnchorPoint(Vec2(0.5, 1));
    _door->setPosition(CMVisibleRect::getPosition(279,486));
    _door->setScale(1,-2.1);
    _door->setVisible(false);
    
    auto call = [=]
    {
        TipsUIController::getInstance()->showClickTip(doorClose, doorClose->getContentSize()/2);
        
        auto cTouch = ComponentTouchMove::create();
        cTouch->setIsCanMove(false);
        cTouch->touchEndedCall=[=](Touch* t)
        {
            TipsUIController::getInstance()->removeAllTips();
            cTouch->pause();
            doorClose->setVisible(false);
            _door->setVisible(true);
            _door->runAction(Sequence::create(ScaleTo::create(0.8f, 1),
                                                 CallFunc::create(CC_CALLBACK_0(TexasPizzaMakeScene::putInOven, this)),
                                                 NULL));
            _plate->runAction(MoveTo::create(0.8f, CMVisibleRect::getPosition(320, 200)));

        };
        _oven->addComponent(cTouch);
    };
    
    ActionHelper::moveInToSceneBounce(_oven, CMVisibleRect::getPosition(320, 630), kActionDirectionLeft, call);
}

void TexasPizzaMakeScene::putInOven()
{
    TipsUIController::getInstance()->showMoveTip(this, _plate->getPosition(), CMVisibleRect::getPosition(278, 610));
    kAudioUtil->playEffect(kVoicePizzaDragToOven);

    
    auto cTouch = ComponentTouchMove::create();
    cTouch->touchBeganCall=[=](Touch* t)
    {
        TipsUIController::getInstance()->removeAllTips();
        _plate->setScale(1,0.6);
    };
    cTouch->touchEndedCall=[=](Touch* t)
    {
        if (_oven->getBoundingBox().containsPoint(t->getLocation()))
        {
            cTouch->pause();
            auto sp = Spawn::create(ScaleTo::create(0.8f,0.5,0.3),
                                    MoveTo::create(0.8f,CMVisibleRect::getPosition(278, 610)),
                                    NULL);
            _door->setZOrder(11);
            _plate->runAction(Sequence::create(sp,
                                               CallFunc::create([=]
                                                                {
                                                                    _door->runAction(Sequence::create(ScaleTo::create(0.8f, 1, -2.1),
                                                                                                      CallFunc::create([=]
                                                                    {
                                                                        _door->setTexture("res/pizza/oven/oven_close.png");
                                                                        _door->setAnchorPoint(Vec2(0.5, 0));
                                                                        _door->setPosition(CMVisibleRect::getPosition(281, 488));
                                                                        _door->setScale(1);
                                                                        
                                                                        _door->runAction(MoveBy::create(0.4f, Vec2(0, -200)));
                                                                        _plate->runAction(MoveBy::create(0.4f, Vec2(0, -200)));
                                                                        _oven->runAction(MoveBy::create(0.4f, Vec2(0, -200)));
                                                                        
                                                                    }),
                                                                                                      DelayTime::create(0.4f),
                                                                                                      CallFunc::create([=]
                                                                    {
                                                                        this->setTemperature();
                                                                    }),
                                                                                                      
                                                                                                      NULL));
                                                                }),
                                               NULL));
        }
        else
        {
            _plate->setScale(1);
            _plate->setPosition(CMVisibleRect::getPosition(320, 200));
        }
    };
    _plate->addComponent(cTouch);
}

void TexasPizzaMakeScene::setTemperature()
{
    if (!_rotater)
        return;
    _progressTimer = ProgressTimer::create(Sprite::create("ui/choose/progress_up.png"));
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setMidpoint(Vec2(0.0, 0.0));
    _progressTimer->setBarChangeRate(Vec2(1, 0));
    _progressTimer->setPercentage(0.f);
    
    _progressBg = Sprite::create("ui/choose/progress_down.png");
    _progressTimer->setPosition(_progressBg->getContentSize()/2);
    _progressBg->addChild(_progressTimer,1);
    this->addToUILayer(_progressBg, 2);
    _progressBg->setPosition(CMVisibleRect::getPosition(320, 780));
    
    _rotater->runAction(ActionHelper::createScaleHint());
    TipsUIController::getInstance()->showClickTip(this, _oven->convertToWorldSpace(_rotater->getPosition())+Vec2(30,-50));

    auto cTouch = ComponentTouchMove::create();
    cTouch->setIsCanMove(false);
    cTouch->touchEndedCall=[=](Touch* t)
    {
        TipsUIController::getInstance()->removeAllTips();
        _rotater->stopAllActions();
        cTouch->stop();
        this->showRotate();
    };
    _rotater->addComponent(cTouch);
    
}

void TexasPizzaMakeScene::showRotate()
{
    auto bg = Sprite::create("res/pizza/oven/button_3.png");
    bg->setPosition(CMVisibleRect::getCenterOfScene());
    this->addToContentLayer(bg,22);

    auto rotate = Sprite::create("res/pizza/oven/button_2.png");
    rotate->setPosition(CMVisibleRect::getPosition(320, 468));
    this->addToContentLayer(rotate,22);
    rotate->setRotation(-180);
    rotate->setAnchorPoint(Vec2(0.5,0.47));

    auto hand = Sprite::create("ui/tips/finger.png");
    hand->setPosition(533,285);
    bg->addChild(hand,2);
    hand->setRotation(-86);
    
    auto m1 = MoveTo::create(0.4f, Vec2(470, 266));
    auto s1 = ScaleTo::create(0.4f, 0.8f);
    auto s = Spawn::create(m1,s1,NULL);
    hand->runAction(RepeatForever::create(Sequence::create(s,
                                                           DelayTime::create(0.3f),
                                                           Place::create(Vec2(533,285)),
                                                           ScaleTo::create(0, 1),
                                                           NULL)));
    
    
    
    auto compRotateTemperature = TouchRotateComponent::create();
    compRotateTemperature->setRotateTargetDegree(180);
    rotate->addComponent(compRotateTemperature);
    compRotateTemperature->rotateDoneCall = [=]()
    {
        log("====r1");

        hand->removeFromParent();
        log("====r2");

        bg->removeFromParent();
        log("====r3");

        rotate->setVisible(false);
        log("====r4");

        compRotateTemperature->setEnabled(false);
        log("====r5");

        _rotater->setRotation(180);
        log("====r6");

        this->bake();
    };
    compRotateTemperature->start();

}


void TexasPizzaMakeScene::bake()
{
    kAudioUtil->playEffect(kVoicePizzaBake);
    log("====bake");
    this->bakeOther();
    float duration = 5.f;
    
    _progressTimer->runAction(Sequence::create(ProgressTo::create(duration, 50.000f),
                                               CallFunc::create([=]
    {
        auto ok = Sprite::create("ui/choose/progress_ok.png");
        ok->setPosition(200, 26);
        _progressBg->addChild(ok,19);
        ok->setScale(0);
        ok->runAction(EaseBackInOut::create(ScaleTo::create(0.3f, 1)));
    }),
                                               NULL));
    
    auto light = Sprite::create("res/pizza/oven/fire_1.png");
    _door->addChild(light,-1);
    light->setPosition(_door->getContentSize()/2);
    light->runAction(RepeatForever::create(ActionHelper::createAnimate("res/pizza/oven/fire_%d.png", 1, 2)));
    
    this->runAction(Sequence::create(DelayTime::create(duration),
                                     CallFunc::create([=]
    {
        light->removeFromParent();
        bakeOver();
        kAudioUtil->playEffect(kVoicePizzaBakingOver);
    }),
                                     NULL));
    
    for (auto other : _cheeseOnBreadVec)
    {
        log("%s,",(other->getName().c_str()));
        
        auto bstr = "res/pizza/pizza/overlook/beforebake/cheese_1.png";
        auto astr = "res/pizza/pizza/overlook/afterbake/cheese_1.png";
        ComponetFade* f = ComponetFade::create(bstr, astr);
        other->addComponent(f);
        f->setFadeDuration(duration);
        f->start();
    }
}


void TexasPizzaMakeScene::bakeOther()
{
    
    
    for (auto other : _otherOnBreadVec)
    {
        log("%s,",(other->getName().c_str()));
        
        auto bstr = StringUtils::format("res/pizza/pizza/overlook/beforebake/other/%d.png",atoi(other->getName().c_str()));
        auto astr = StringUtils::format("res/pizza/pizza/overlook/afterbake/other/%d.png",atoi(other->getName().c_str()));
        
        ComponetFade* f = ComponetFade::create(bstr, astr);
        other->addComponent(f);
        f->setFadeDuration(5.f);
        f->start();
    }
}

void TexasPizzaMakeScene::bakeOver()
{

    _door->setAnchorPoint(Vec2(0.5, 1));
    _door->setPosition(CMVisibleRect::getPosition(279,486-200));
    _door->setScale(1, -1.1);
    
    _door->runAction(Sequence::create(ScaleTo::create(0.8f, 1),
                                      CallFunc::create([=]
    {
        auto sp = Spawn::create(ScaleTo::create(0.8f,1,0.6),
                                MoveTo::create(0.8f,CMVisibleRect::getPosition(320, 300)),
                                NULL);
        
        auto doorClose = Sprite::create("res/pizza/oven/oven_open.png");
        _oven->addChild(doorClose,10);
        doorClose->setPosition(234,-24);
        
        _door->setVisible(false);
        _plate->runAction(Sequence::create(sp,
                                           CallFunc::create([=]
        {
            kSoundInstance->playRandomStepFinishVoice();
            ActionHelper::moveOutofSceneAndRemoveSelf(_oven, kActionDirectionLeft);
            _plate->setScale(1);
            chooseBread();
        }),
                                           nullptr));

    }),
                                      NULL));
}


void TexasPizzaMakeScene::chooseBread()
{
    _progressBg->setVisible(false);

    auto hint = Sprite::create("ui/tips/tips.png");
    this->addToUILayer(hint,10);
    hint->setPosition(CMVisibleRect::getPosition(320, 200,kBorderNone,kBorderTop));

    for (auto bread :_breadInPlateVec )
    {
        auto cTouch = ComponentTouchMove::create();
        cTouch->setIsCanMove(false);
        cTouch->touchClickedCall=[=](Touch* t)
        {
            auto food = kESDataManager->getTexasPizzaFood();
            food.foodIndex = bread->getTag();
            kESDataManager->setTexasPizzaFood(food);
            SceneManager::getInstance()->replaceWithDefaultTransition(TexasPizzaDecScene::scene());
        };
        bread->addComponent(cTouch);
        
    
        Rect r = bread->getBoundingBox();
        
        auto renderTexture1 = RenderTexture::create(r.size.width, r.size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        renderTexture1->setKeepMatrix(true);
        
        Rect fullRect1 = Rect(0.0, 0.0,  Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
        Rect fullViewport1 = Rect(0.0, 0.0, Director::getInstance()->getWinSizeInPixels().width, Director::getInstance()->getWinSizeInPixels().height);
        
        renderTexture1->setVirtualViewport(Vec2(0,0), fullRect1, fullViewport1);
        auto pos =bread->getPosition();
        bread->setPosition(r.size/2);
        renderTexture1->begin();
        bread->visit();
        renderTexture1->end();
        bread->setPosition(pos);
        _director->getRenderer()->render();
        
        renderTexture1->saveToFile(StringUtils::format("pizza_%d.png",bread->getTag()), true, nullptr);
        
        
    }
}

