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
    
    string btmPath = "res/soda/cup/cup1_2.png";
    string coverPath = StringUtils::format("res/soda/cup/cup%d_1.png",cupIndex);
    string maskPath = "res/soda/cup_soda/shadow.png";

    string juicePath = StringUtils::format("res/soda/cup_soda/cup_soda%d.png",flavorIndex);
    _bottle = BowlNode::create(btmPath, coverPath);
    _bottle->initMaskNode(maskPath);
    this->addChild(_bottle,1);
    _juiceInBottle = Sprite::create(juicePath);
    _bottle->addContentToMask(_juiceInBottle, "juice", 1,Vec2(0, 0));
    
    
    this->updateStraw(kESDataManager->getSodaFood().strawIndex);
    this->updateBottleLid(kESDataManager->getSodaFood().lidIndex);
    this->updateCream(kESDataManager->getSodaFood().creamIndex);
    this->addIce();
    
    return true;
}

void JuiceBottleNode::updateBottle(int index)
{
    if (index == -1)
    {
        return;
    }
    string btmPath = "res/soda/cup/cup1_2.png";
    string coverPath = StringUtils::format("res/soda/cup/cup%d_2.png",index);
//    string maskPath = "res/soda/cup_soda/cup_soda_shadow.png";
    string maskPath = "res/soda/cup_soda/shadow.png";
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
        _cream->setPosition(Vec2(140, 303));
        _cream->setScale(1.05);
    }
    _cream->setTexture(StringUtils::format("res/decoration/cup_cream/cup_cream_%d.png",index));
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
    
    string lidPath = StringUtils::format("res/soda/cup/cup%d_top1.png",index);

    if (!_lid && !_lidBtm)
    {
        _lid = Sprite::create(lidPath);
        this->addChild(_lid,10);
        
        _lidBtm = Sprite::create("res/soda/cup/cup1_top2.png");
        this->addChild(_lidBtm,-1);
    }
    else
    {
        _lid->setTexture(lidPath);
    }
    _lid->setVisible(true);
    _lidBtm->setVisible(true);
}


void JuiceBottleNode::showLid(bool animated)
{
    int index = kESDataManager->getSodaFood().cupIndex;
    string lidPath = StringUtils::format("res/juice/decoration/3_cup/bottle%d/bottle_cap%d.png",index,index);

    if (!FileUtils::getInstance()->isFileExist(lidPath))
    {
        return;
    }
    
    _lid = Sprite::create(lidPath);
    if (animated)
    {
        _bottle->addContent(_lid, "lid", INT_MAX,Vec2(0, 300));
        _bottle->setContentVisible("lid",nullptr,true,false);
        _lid->runAction(MoveBy::create(0.3f, Vec2(0, -300)));
    }
    else
    {
        _bottle->addContent(_lid, "lid", INT_MAX);
        _bottle->setContentVisible("lid",nullptr,true,false);
    }
}

void JuiceBottleNode::enableDrink(const function<void()> &startDrinkingCallback,const function<void()> &finishedEatingCallback)
{
    auto sizeH = _bottle->getContentSize().height;

    auto temp = Button::create("res/decoration/straw/straw19.png");
    this->addChild(temp,1);
    temp->setOpacity(0);
    temp->setPosition(Vec2(0,100));
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
                _juiceInBottle->runAction(Sequence::create(MoveTo::create(5, Vec2(0, -sizeH*0.7)),
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
    _juiceInBottle->setPosition(0, 0);
    kSoundInstance->stopLoopEffect();
}

void JuiceBottleNode::updateStraw(int index)
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
    
    auto path = StringUtils::format("res/decoration/straw/straw%d.png",index);
    auto pathShadow = StringUtils::format("res/decoration/strawcut/straw%d.png",index);
    
    if (!_straw)
    {
        _straw = Sprite::create(path);
        _bottle->addContent(_straw, "straw", 1,Vec2(22,71));
        _bottle->setContentVisible("straw",nullptr,true,false);
        
        _strawShadow = Sprite::create(pathShadow);
        _bottle->addContent(_strawShadow, "strawShadow", 3,Vec2(22,71));
        _bottle->setContentVisible("strawShadow",nullptr,true,false);

    }
    _straw->setTexture(path);
    _strawShadow->setTexture(pathShadow);
    _straw->setPosition(22,71+30);
    _strawShadow->setPosition(22,71+30);
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
    Rect r = Rect(70, 51, 134, 292+60);
    r.origin = _juiceInBottle->convertToWorldSpace(r.origin);
    cTouch->setMoveArea(r);
    cTouch->setNeedTouchTarget(true);
    
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
        {
            return;
        }
        if (_director->getTextureCache()->getTextureForKey("paint_juice"))
        {
            _director->getTextureCache()->removeTextureForKey("paint_juice");
        }
        Image *img = new Image();
        if(img->initWithImageFile(path))
        {
            auto txt = _director->getTextureCache()->addImage(img, "paint_juice");
            _juiceInBottle->setTexture(txt);
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
    Rect r = _juiceInBottle->getBoundingBox();
    
    auto renderTexture1 = RenderTexture::create(r.size.width, r.size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    renderTexture1->setKeepMatrix(true);
    
    Rect fullRect1 = Rect(0.0, 0.0,  Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    Rect fullViewport1 = Rect(0.0, 0.0, Director::getInstance()->getWinSizeInPixels().width, Director::getInstance()->getWinSizeInPixels().height);
    
    renderTexture1->setVirtualViewport(Vec2(0,0), fullRect1, fullViewport1);
    _juiceInBottle->setPosition(r.size/2);
    renderTexture1->begin();
    _juiceInBottle->visit();
    renderTexture1->end();
    _director->getRenderer()->render();
    _juiceInBottle->setPosition(Vec2(0, 0));

    renderTexture1->saveToFile(shareNameJuice, true, callback);
}

