//
//  JuiceBottleNode.cpp
//  ccats1061
//
//  Created by liji on 16/8/22.
//
//

#include "JuiceBottleNode.h"
#include "ComponentTouchMove.h"
#include "SoundConfig.h"
JuiceBottleNode::JuiceBottleNode():
_bottle(nullptr),
_lid(nullptr),
_lidBtm(nullptr),
_juiceInBottle(nullptr),
isFirst(true),
_straw(nullptr),
_strawShadow(nullptr),
_cream(nullptr)
{


}


bool JuiceBottleNode::init()
{
    if (!MakeBaseNode::init())
    {
        return false;
    }

    int cupIndex = kESDataManager->getSodaFood().cupIndex;
    int flavorIndex = kESDataManager->getSodaFood().flavorIndex;
    
    string btmPath = StringUtils::format("res/rainbow_slushy/cup/cup%d_2.png",cupIndex);
    string coverPath = StringUtils::format("res/rainbow_slushy/cup/cup%d_1.png",cupIndex);
    string maskPath = "res/rainbow_slushy/cup/cup_mixture/shadow.png";

    string juicePath = StringUtils::format("res/rainbow_slushy/cup/cup_mixture/cup_%d.png",flavorIndex);
    _bottle = BowlNode::create(btmPath, coverPath);
    _bottle->initMaskNode(maskPath);
    this->addChild(_bottle,1);
    _juiceInBottle = Sprite::create(juicePath);
    _bottle->addContentToMask(_juiceInBottle, "juice", 1);
//    if (kESDataManager->getSodaFood().cupIndex==13)
//    {
//        _juiceInBottle->setPosition(Vec2(31,-53));
//    }
    
    this->updateStraw(kESDataManager->getSodaFood().strawIndex);
    this->updateBottleLid(kESDataManager->getSodaFood().cupIndex);
//    this->updateCream(kESDataManager->getSodaFood().creamIndex);
//    this->addIce();
    return true;
}


Size JuiceBottleNode::getContentSize()
{
    return _bottle->getContentSize();
}

void JuiceBottleNode::updateBottle(int index)
{
    if (index == -1)
    {
        return;
    }
    string btmPath = StringUtils::format("res/rainbow_slushy/cup/cup%d_0.png",index);
    string coverPath = StringUtils::format("res/rainbow_slushy/cup/cup%d_1.png",index);
    string maskPath = "res/rainbow_slushy/cup/cup_mixture/shadow.png";
    this->updateStraw(kESDataManager->getSodaFood().strawIndex);
    
    _bottle->updateBowlConfig(btmPath, coverPath,maskPath);
}

void JuiceBottleNode::updateCream(int index)
{
    if (index == -1)
    {
        return;
    }
    if (!_cream)
    {
        _cream = Sprite::create();
        _juiceInBottle->addChild(_cream,1);
        _cream->setPosition(Vec2(145-20,403+10));
        _cream->setScale(1.18,1.0);
        _cream->setOpacity(0);
        _cream->runAction(FadeIn::create(1.2f));
        
        if (kESDataManager->getSodaFood().cupIndex==13)
        {
//            _cream->setPosition(Vec2(145,403+10));
        }
    }
    _cream->setTexture(StringUtils::format("res/4decoration/1cream/cream/cream%d.png",index));
}


void JuiceBottleNode::updateBottleLid(int index)
{
    if (index == -1)
    {
        if (_lid && _lidBtm)
        {
            _lid->removeFromParentAndCleanup(true);_lid = nullptr;
            _lidBtm->removeFromParentAndCleanup(true);_lidBtm = nullptr;
        }
        
        return;
    }
    
    string lidPath = StringUtils::format("res/rainbow_slushy/cup/cup%d_top1.png",index);

    if (!_lid && !_lidBtm)
    {
        _lid = Sprite::create(lidPath);
        this->addChild(_lid,10);
        
        _lidBtm = Sprite::create("res/rainbow_slushy/cup/cup1_top2.png");
        this->addChild(_lidBtm,-1);
    }
    else
    {
        _lid->setTexture(lidPath);
    }
    _lid->setVisible(true);
    _lidBtm->setVisible(true);
}


void JuiceBottleNode::showLid(bool visible)
{
    if (_lid && _lidBtm)
    {
        _lid->setVisible(visible);
        _lidBtm->setVisible(visible);
    }
    
}

void JuiceBottleNode::enableDrink(const function<void()> &startDrinkingCallback,const function<void()> &finishedEatingCallback)
{
    auto sizeH = _bottle->getContentSize().height;

    auto temp = Button::create("res/rainbow_slushy/cup/cup13_top2.png");
    this->addChild(temp,1);
    temp->setOpacity(0);
    temp->setPosition(Vec2(0, 20));
    temp->addTouchEventListener([=](Ref*,Widget::TouchEventType type)
    {
        if (type == Widget::TouchEventType::BEGAN)
        {
            kSoundInstance->playLoopEffect(kSoundDrink);
            if (isFirst)
            {
                if (startDrinkingCallback)
                {
                    startDrinkingCallback();
                }
                _juiceInBottle->runAction(Sequence::create(MoveTo::create(10, Vec2(0, -sizeH*0.7)),
                                                           CallFunc::create([=]
                                                                            {
                                                                                if (finishedEatingCallback)
                                                                                {
                                                                                    finishedEatingCallback();
                                                                                }
                                                                                kSoundInstance->stopLoopEffect();
                                                                            }),
                                                           MoveTo::create(1, Vec2(0, -sizeH)),
                                                           NULL));
                isFirst = false;
            }
            else
            {
                _actionManager->resumeTarget(_juiceInBottle);
            }
        }
        if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
        {
            kSoundInstance->stopLoopEffect();
            _actionManager->pauseTarget(_juiceInBottle);
            log("%f==========",_juiceInBottle->getPositionY());
        }
    });
    
    return;
    log("======drink1");
    if (_bottle->getComponent("111"))
    {
        return;
    }
    log("======drink2");
    
    auto cTouch = ComponentTouchMove::create();
    cTouch->setIsCanMove(false);
    cTouch->touchBeganCall = [=](Touch* t)
    {
        kSoundInstance->playLoopEffect(kSoundDrink);
        if (isFirst)
        {
            if (startDrinkingCallback)
            {
                startDrinkingCallback();
            }
            _juiceInBottle->runAction(Sequence::create(MoveTo::create(5, Vec2(0, -sizeH*0.7)),
                                                       CallFunc::create([=]
            {
                if (finishedEatingCallback)
                {
                    finishedEatingCallback();
                }
            }),
                                                       MoveTo::create(1, Vec2(0, -sizeH)),
                                                       NULL));
            isFirst = false;
        }
        else
        {
            _actionManager->resumeTarget(_juiceInBottle);
        }
    };
    cTouch->touchEndedCall = [=](Touch* t)
    {
        kSoundInstance->stopLoopEffect();
        _actionManager->pauseTarget(_juiceInBottle);
        log("%f==========",_juiceInBottle->getPositionY());
    };
    cTouch->setName("111");
    log("======drink3");

    this->addComponent(cTouch);
    
    log("======drink4");

}

void JuiceBottleNode::resetDrink()
{
    isFirst = true;
    _juiceInBottle->stopAllActions();
    _juiceInBottle->setPosition(Vec2(0, 0));
    kSoundInstance->stopLoopEffect();
}

void JuiceBottleNode::updateStraw(int index,bool animated)
{
    if (index == -1)
    {
        if (_straw && _strawShadow)
        {
            _straw->removeFromParentAndCleanup(true);_straw = nullptr;
            _strawShadow->removeFromParentAndCleanup(true);_strawShadow = nullptr;
        }
        return;
    }
    
    auto path = StringUtils::format("res/rainbow_slushy/decoration/straw/straw%d.png",index);
    auto pathShadow = StringUtils::format("res/rainbow_slushy/decoration/straw_cut/straw%d.png",index);
    
    if (!_straw)
    {
        _straw = Sprite::create(path);
        _bottle->addContent(_straw, "straw", 1,Vec2(102,277));
        _bottle->setContentVisible("straw",nullptr,true,false);
        
        _strawShadow = Sprite::create(pathShadow);
        _bottle->addContent(_strawShadow, "strawShadow", 3,Vec2(102,71));
        _bottle->setContentVisible("strawShadow",nullptr,true,false);
        _straw->setScale(0.8f);
        _strawShadow->setScale(0.8f);

    }
    _straw->setTexture(path);
    _strawShadow->setTexture(pathShadow);
    Vec2 pos = Vec2(35, 120);
    if (animated)
    {
        kAudioUtil->stopAllEffect();
        float delta = 160;
        pos += Vec2(0, delta);
        _straw->setZOrder(3);
        _straw->stopAllActions();
        _strawShadow->stopAllActions();
        _straw->setPosition(pos);
        _straw->runAction(EaseBackIn::create(MoveBy::create(0.8f, Vec2(0, -delta))));
        _straw->runAction(Sequence::create(DelayTime::create(0.7f),
                                           CallFunc::create([=]
        {
            _straw->setZOrder(1);
            kAudioUtil->playEffect(kSoundAddStraw);
        }),
                                           NULL));
        _strawShadow->runAction(EaseBackIn::create(MoveBy::create(0.8f,Vec2(0, -delta))));
    }

    auto p = _straw->getContentSize().height - _strawShadow->getContentSize().height;
    p = p*0.85f;
    _straw->setPosition(pos);
    _strawShadow->setPosition(pos+Vec2(0,p/2));
    _straw->setScale(0.85f);
    _strawShadow->setScale(0.85f);
}

void JuiceBottleNode::reset()
{
//    kESDataManager->resetJuiceDec();
    auto juiceFood = kESDataManager->getSodaFood();
    
    this->updateBottle(juiceFood.cupIndex);
    this->updateBottleLid(juiceFood.cupIndex);
    this->updateStraw(juiceFood.strawIndex);
}
void JuiceBottleNode::addSprinkle(string sprinklePath, Vec2 worldPos)
{
    auto sprinkle = Sprite::create(sprinklePath);
    _juiceInBottle->addChild(sprinkle,10);
    sprinkle->setScale(0);
    sprinkle->setPosition(_juiceInBottle->convertToNodeSpace(worldPos));
    
    float scale = rand()%4*0.1 + 0.6;
    sprinkle->runAction(EaseBackOut::create(ScaleTo::create(0.3f, scale)));
    sprinkle->setRotation(rand()%360);
}

void JuiceBottleNode::addSprinkleAnimated(Node *s, Vec2 worldPos)
{

    _juiceInBottle->addChild(s,10);

}


void JuiceBottleNode::addFood(string foodPath)
{
    auto food = Sprite::create(foodPath);
    _juiceInBottle->addChild(food,10);
    food->setPosition(140, 197+150);
    food->setScale(0);
    float scale = rand()%4*0.1 + 1;
    food->runAction(EaseBackOut::create(ScaleTo::create(0.3f, scale)));
    food->setRotation(rand()%360);
    
    auto cTouch = ComponentTouchMove::create();
    food->addComponent(cTouch);
    Rect r = Rect(70, 51, 134, 292);
    r.origin = _juiceInBottle->convertToWorldSpace(r.origin);
    cTouch->setMoveArea(r);
    
}
void JuiceBottleNode::addIce()
{
    if (kESDataManager->getSodaFood().iceIndex == -1)
        return;
    
    vector<Vec2> icePosVec =
    {
        Vec2(-38, -141),
        Vec2(-21, -164),
        Vec2(20, -167),
        Vec2(-49, -105),
        Vec2(0, -110),
        Vec2(44, -124),
    };
    
    for (int i = 0;  i<icePosVec.size(); i++)
    {
        auto iceInBowl = Sprite::create(StringUtils::format("res/soda/ice/ice%d.png",kESDataManager->getSodaFood().iceIndex));
        iceInBowl->setRotation(rand()%360);
        iceInBowl->setScale(rand()%3*0.1+0.8);
        iceInBowl->setOpacity(145);
        //    _cup->addContentToMask(iceInBowl, "iceInCup",10,icePosVec.at(ice->getTag())+Vec2(0,60));
        _juiceInBottle->addChild(iceInBowl);
        iceInBowl->setPosition(icePosVec.at(i)+Vec2(142, 222));
    }
}

void JuiceBottleNode::showDrink()
{
    if (_juiceInBottle)
    {
        auto path = FileUtils::getInstance()->getWritablePath()+ shareNameJuice;
        if (!FileUtils::getInstance()->isFileExist(path))
            return;
        _juiceInBottle->setTexture(path);
        if (_director->getTextureCache()->getTextureForKey("paint_juice"))
        {
            _director->getTextureCache()->removeTextureForKey("paint_juice");
        }
        Image *img = new Image();
        if (img->initWithImageFile(path))
        {
            _juiceInBottle->setTexture(_director->getTextureCache()->addImage(img, "paint_juice"));
            img->autorelease();
        }
    }
    if (_cream)
    {
        _cream->setVisible(false);
    }

}


void JuiceBottleNode::saveJuiceImage(const std::function<void(RenderTexture*, const std::string&)>& callback)
{
    Rect r3 = Rect(0,0,_juiceInBottle->getContentSize().width,_juiceInBottle->getContentSize().height);
    
    auto renderTexture1 = RenderTexture::create(getContentSize().width, getContentSize().height+100, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
//    renderTexture1->setKeepMatrix(true);
//    
//    Rect fullRect1 = Rect(0.0, 0.0,  Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
//    Rect fullViewport1 = Rect(0.0, 0.0, Director::getInstance()->getWinSizeInPixels().width, Director::getInstance()->getWinSizeInPixels().height);
//    
//    renderTexture1->setVirtualViewport(Vec2(0,0), fullRect1, fullViewport1);
    _juiceInBottle->setPosition(getContentSize()/2+Size(0,50));
    renderTexture1->begin();
    _juiceInBottle->visit();
    renderTexture1->end();
    _director->getRenderer()->render();
    _juiceInBottle->setPosition(Vec2(0,0));
    renderTexture1->saveToFile(shareNameJuice, true, callback);
}

void JuiceBottleNode::testScreen()
{
    Rect r = getBoundingRect();
    _juiceInBottle->setPosition(r.size/2);

}


void JuiceBottleNode::testCup()
{
    static int x = 0;
    x++;
    if (x>15)
        x = 1;
    log("%d=====" ,x );
    this->runAction(Sequence::create(DelayTime::create(1.2f),
                                     CallFunc::create([=]
    {
        this->updateBottle(x);
        testCup();
    }),
                                     NULL));
}

Rect JuiceBottleNode::getBoundingRect()
{
    auto original = this->convertToWorldSpace(Vec2::ZERO-getContentSize()/2);
    Rect r;
    r.origin = original;
    r.size = getContentSize();
    return r;
}

