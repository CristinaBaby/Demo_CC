//
//  MakeLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-2-12.
//
//

#include "MakeLayer.h"
#include "KSVisibleRect.h"
#include "Config.h"
#include "MyMakeConfig.h"
#include "IAPManager.h"
#include "GameMaster.h"
#include "CandyScribbleView.h"
#include "Dialog.h"
#include "MyMakeConfig.h"
#include "FillMaterialModel.h"
#include "MathUtil.h"

#define bgPath "makeIce/bg2.png"

bool MakeLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    isInShare_ = false;
    scribbleLayer_ = nullptr;
    cupCover_ = nullptr;
    
//    auto color = LayerColor::create(Color4B(255, 255, 255, 255),640,500);
//    color -> setPosition(Vec2(0 , 100));
//    this -> addChild(color,9999);

    
    _yDistance = IAPManager::getInstance()->isShowAds()?100:0;
    
    
    listener  = EventListenerTouchOneByOne::create();
    listener -> setSwallowTouches(true);
    listener -> retain();
    listener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());//获取的当前触摸的目标
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode))
        {
                return true;
        }
        return false;
    };
    listener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        if(!isInShare_ && target -> getTag()!= kTagReady)
        {
            Rect rect = Rect(0, 100, 640, 700);
//            rect.size = size;
            
            
            if (rect.containsPoint(target->getPosition() + touch->getDelta()))
            {
                target -> setPosition(target -> getPosition() + touch -> getDelta());
            }
            else
            {
//                target -> setPosition(touch -> getLocation());
            }
        }

    };
    listener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        
        auto target = static_cast<Node*>(event->getCurrentTarget());//获取的当前触摸的目标
        if (isInShare_ &&  target -> getTag() == kTagUmbrella)
        {
            
            target -> setTag(kTagNone);
            target -> runAction(Sequence::create(
                                                 CallFunc::create([&]
            {
                CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/tool_fly.mp3");

            }),
                                                 MoveBy::create(0.5, Vec2(100, -100)),
                                                 MoveBy::create(0.5, Vec2(900, 300)),
                                                 NULL));
        }
        if (isInShare_ &&  target -> getTag() == kTagSpoon)
        {
            target -> setTag(kTagNone);
            target -> runAction(Spawn::create(
                                                 RotateBy::create(1.5, 720*2),
                                                 ScaleTo::create(1.5, 0),
                                                 NULL));
        }
        
        if (!isInShare_ &&  target -> getTag() == kTagReady)
        {
            
            if(!FileUtils::getInstance() -> isFileExist(tapFoodString))
            {
                return;
            }
            
            //点击屏幕生成道具的功能
            FillMaterialModel* food = FillMaterialModel::create(tapFoodString);
            
            food -> setPosition(touch ->getLocation());
            
            eatLayer_ -> getShowLayer() -> addChild(food,10);
            vec_movealbleItem.pushBack(food);

            auto p = ParticleSystemQuad::create("rain.plist");
            p -> setPosition(touch ->getLocation());
            this -> addChild(p,10);
            this -> allowMove();
        }


    };
    
    this -> showDefaultBackground();
    this -> showDefaultUnit();

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener -> clone(), this);
    this -> setTag(kTagReady);
    tapFoodString = "nullitem";
    
    return true;
}



void MakeLayer::showDefaultBackground()
{
    if (!background_) {
        background_ = Sprite::create(bgPath);
        background_-> setPosition(320,480-_yDistance);
        addChild(background_,1);
    }
//    else
//        background_ -> setTexture("bg_decoration.jpg");
    
}
void MakeLayer::showDefaultUnit()
{
    log(" makerlayer %s ",GameMaster::getInstance() -> getCupName() -> getCString());

    cup_ = Sprite::create(GameMaster::getInstance() -> getCupName() -> getCString());
    cup_ -> setPosition(Vec2(320, 310-80-80));
    this -> addChild(cup_,5);
    
}

void MakeLayer::showDefaultDecorationUnit()
{
    
//    defaultFoodLayer_ = EatScribbleView::create();
//    this -> addChild(defaultFoodLayer_,5);
//    defaultFoodLayer_ -> setCheckPercent(true);
//    defaultFoodLayer_ -> setDelegate(this);
    
    eatLayer_ = EatScribbleView::create();
    this -> addChild(eatLayer_,6);
    
    //按照需求，选取之前用户第一次选中的形状
    
    snowcone_ = Sprite::create(Config::getShapeName());
    snowcone_ -> setPosition(Config::getSnowconePos());
    eatLayer_ -> getShowLayer()  -> addChild(snowcone_,1);

    
}

void MakeLayer::updateUnit(std::string unitImageName)
{
    auto typeName = MyMakeConfig::getInstance() -> typeName_.c_str();
    
    //tapFoodString
    tapFoodString = "nullitem";
    
    //选择了其他项就把涂抹关了
    if (scribbleLayer_)
    {
        scribbleLayer_ -> forbidScribble();
        
    }


    if (!strcmp(typeName, "background"))
    {
        background_ -> setTexture(unitImageName);
        this -> allowMove();
        return;
    }
//    else if (!strcmp(typeName, "syrup"))
//    {
//
//        int h = MyMakeConfig::getInstance() -> getH(idx);
//        int s = MyMakeConfig::getInstance() -> getS(idx);
//        int v = MyMakeConfig::getInstance() -> getV(idx);
//        
//        if (!scribbleLayer_)
//        {
//            scribbleLayer_ = CandyScribbleView::create();
//            defaultFoodLayer_ -> getShowLayer() -> addChild(scribbleLayer_,2);
//        }
//        
//        Texture2D * tex =    Config::getShapeScribbleTexture(h, s, v ,idx);
//        scribbleLayer_ -> updateScribbleWithTexture(tex , "xxx");
//    }
//
//    else if (!strcmp(typeName, "fruit") ||!strcmp(typeName, "topper")||  !strcmp(typeName, "chocolate") )
//    {
//
//        this -> updateFood(unitImageName);
//        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/select.mp3");
//        return;
//    }
//    
//    else if ( !strcmp(typeName, "candy") ||  !strcmp(typeName, "sprinkle"))
//    {
//        
//        this -> generateFood(unitImageName);
//        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/select.mp3");
//        return;
//    }
    else if (!strcmp(typeName, "spoons"))
    {

        this -> updateSpoon(unitImageName);
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/select.mp3");
        return;
    }
    else if (!strcmp(typeName, "extras"))
    {
        
        this -> updateExtra(unitImageName);
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/select.mp3");
        return;
    }
    else
    {
        this -> updateFood(unitImageName);
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/select.mp3");
        return;
    }
}


void MakeLayer::updateUnit(int idx)
{
    tapFoodString = "nullitem";

    if (scribbleLayer_)
    {
        scribbleLayer_ -> openScribble();
        
    }

    
    
    {
            
        int h = MyMakeConfig::getInstance() -> getH(idx);
        int s = MyMakeConfig::getInstance() -> getS(idx);
        int v = MyMakeConfig::getInstance() -> getV(idx);
        
        if (!scribbleLayer_)
        {
            scribbleLayer_ = CandyScribbleView::create();
            eatLayer_ -> getShowLayer() -> addChild(scribbleLayer_,2);
        }
        
        Texture2D * tex =    Config::getShapeScribbleTexture(h, s, v ,idx);
        MyMakeConfig::getInstance()->setTypeName("syrup_bottle");
        auto bottleName = MyMakeConfig::getInstance() -> vec_UnitName.at(idx);

        scribbleLayer_ -> updateScribbleWithTexture(tex , bottleName->getCString());

    }
}

void MakeLayer::updateCup(std::string unitImageName)
{
    if (cupCover_ && cupCover_ -> getParent())
    {
        
        this -> stopAllActions();
        cupCover_ -> removeFromParent(); cupCover_ = nullptr;
    }
    cupCover_ = Sprite::create(unitImageName);
    cupCover_ -> setPosition(Vec2(320, 310-80-80+300));
    this -> addChild(cupCover_,99);
    
    auto spawn = Spawn::create(ScaleTo::create(0.2, 1.2),
                               MoveBy::create(0.5, Vec2(0, -300)),
                               NULL);
    
    cupCover_->runAction(Sequence::create(
                                          Show::create(),
                                          spawn,
                                    ScaleTo::create(0.2, 1),
                                    Hide::create(),
                                    NULL));
    
    this -> runAction(Sequence::create(
                                       DelayTime::create(0.7),
                                       CallFunc::create([=]
    {
        
        if(cupCover_)
            cup_ -> setTexture(unitImageName);

    }),
                                       NULL));
}


void MakeLayer::updateFood(std::string fullPath)
{
    auto typeName = MyMakeConfig::getInstance() -> typeName_.c_str();

    FillMaterialModel* food = FillMaterialModel::create(fullPath);
    if (!strcmp(typeName, "candy") ||  !strcmp(typeName, "sprinkle"))
    {
        int x = SM::MathUtil::randomBetween(100, 540);
        int Y = SM::MathUtil::randomBetween(200, 640);

        food -> setPosition(x,Y);

    }
    else
    {
        food -> setPosition(320,444);

    }

    eatLayer_ -> getShowLayer() -> addChild(food,10);
    vec_movealbleItem.pushBack(food);
    
    food -> runAction(Sequence::create(JumpBy::create(0.2, Vec2(0, 0), 40, 1),
                                       CallFunc::create([&]
                                                        {
                                                            auto p = ParticleSystemQuad::create("rain.plist");
                                                            p -> setPosition(Vec2(320, 444-80));
                                                            this -> addChild(p,10);
                                                        }),

                                       NULL));

    this -> allowMove();

}


void MakeLayer::generateFood(std::string unitImageName)
{
    this -> setTag(kTagReady);
    
    
    tapFoodString = unitImageName;
    
}

void MakeLayer::stopGenerate()
{
    tapFoodString = "nullitem";
}




void MakeLayer::updateSpoon(std::string fullPath)
{
    if (!spoon_)
    {
        spoon_ = Sprite::create(fullPath);
        this -> addChild(spoon_,10,kTagSpoon);
    }
    else
        spoon_ -> setTexture(fullPath);
    spoon_ -> setPosition(449, 537-80);

    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener -> clone(), spoon_);

}

void MakeLayer::updateExtra(std::string fullPath)
{

    if (!extras_)
    {
        extras_ = Sprite::create(fullPath);
        this -> addChild(extras_,10,kTagUmbrella);
    }
    else
        extras_ -> setTexture(fullPath);
    
    extras_ -> setPosition(244, 498-80);
    
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener -> clone(), extras_);
}

void MakeLayer::resetScribble()
{
    if (scribbleLayer_)
    {
        scribbleLayer_ -> removeFromParent(); scribbleLayer_ = nullptr;
    }
}

void MakeLayer::reset()
{
    background_ -> setTexture(bgPath);

    this -> resetEatNode();
    
    for (Sprite* item : vec_movealbleItem )
    {
        item -> removeFromParent(); item = nullptr;
    }
    vec_movealbleItem.clear();
    if (scribbleLayer_)
    {
        scribbleLayer_ -> removeFromParent(); scribbleLayer_ = nullptr;

    }

    if (extras_)
    {
        extras_ -> removeFromParent(); extras_ = nullptr;

    }
    
    if (spoon_)
    {
        spoon_ -> removeFromParent(); spoon_ = nullptr;

    }
    
    //杯子重置
    cup_ -> setTexture(GameMaster::getInstance() -> getCupName() -> getCString());
    if (cupCover_)
    {
        cupCover_ -> setTexture(GameMaster::getInstance() -> getCupName() -> getCString());
    }
    
    //tapfood生成屏蔽
    tapFoodString = "nullitem";

}

void MakeLayer::allowEat()
{
    
    eatLayer_ -> allowEat();
//    defaultFoodLayer_ -> allowEat();
}

void MakeLayer::resetEatNode()
{
//    eatLayer_ -> reset();
//    cupcakeLayer_ -> reset();
    
    eatLayer_ -> forbidEat();
//    defaultFoodLayer_ -> forbidEat();
}

void MakeLayer::forbidMove()
{
    log("size %zd",vec_movealbleItem.size());
    
    if(vec_movealbleItem.size()>0)
    {
        for (FillMaterialModel* item : vec_movealbleItem )
        {
            item -> removeTouch();
        }
    }
    
    if (!isInShare_)
    {
        if (spoon_)
        {
            _eventDispatcher -> removeEventListenersForTarget(spoon_);
        }
        if (extras_)
        {
            _eventDispatcher -> removeEventListenersForTarget(extras_);
        }
    }

}
void MakeLayer::allowMove()
{
    if(vec_movealbleItem.size()>0)
    {
        for (FillMaterialModel* item : vec_movealbleItem )
        {
            item -> enableTouch();
        }
    }
    
    if (spoon_)
    {
        _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener -> clone(), spoon_);
    }
    if (extras_)
    {
        _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener -> clone(), extras_);
    }
}

void MakeLayer::inDecLayer()
{
    
    if (scribbleLayer_)
    {
        scribbleLayer_ -> openScribble();
    }
    

}

void MakeLayer::resetSnowconeInshare()
{

//    defaultFoodLayer_ -> forbidEat();
//    defaultFoodLayer_ -> allowEat();
    eatLayer_ -> forbidEat();
    eatLayer_ -> allowEat();

}



void MakeLayer::inShareLayer()
{
    if (scribbleLayer_)
        scribbleLayer_ -> forbidScribble();

    isInShare_ = true;
    this -> forbidMove();
    this -> allowEat();
    
    if (cupCover_)
    {
        cup_ -> setTexture(cupCover_->getTexture());
        cupCover_ -> removeFromParent();cupCover_ = nullptr;
    }
    
    //重置点击生成的精灵
    tapFoodString = "nullitem";

}

void MakeLayer::backToDecLayer()
{
    this -> resetEatNode();
    this -> allowMove();
    this -> resetDecStatus();
    isInShare_ = false;
    //    defaultFoodLayer_ -> setCheckPercent(true);
    eatLayer_ -> setCheckPercent(true);
}


void MakeLayer::unitShow()
{
//    this -> forbidMove();
//    if (scribbleLayer_)
//        scribbleLayer_ -> forbidScribble();
}

void MakeLayer::scribbleCleared()
{
    Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
    dialog -> setAnchorPoint(Vec2(0,0));
    dialog -> setPosition(KSVisibleRect::getPosition(0,0, kBorderLeft ,kBorderBottom));
    dialog->setContentText("You have eaten your Snow Cone. Do you want a new one?");
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this -> getParent() -> addChild(dialog, 9999,129);

}

void MakeLayer::onPositiveClick(void* type)
{
//    defaultFoodLayer_ -> forbidEat();
//    defaultFoodLayer_ -> allowEat();
    
    eatLayer_ -> forbidEat();
    eatLayer_ -> allowEat();

}

void MakeLayer::onNegativeClick(void* type)
{
//    defaultFoodLayer_ -> setCheckPercent(false);
    eatLayer_ -> setCheckPercent(false);
}

void MakeLayer::resetDecStatus()
{
    if (spoon_)
    {
        spoon_ -> stopAllActions();
        spoon_ -> setPosition(449, 537-80);
        spoon_ -> setRotation(0);
        spoon_ -> setScale(1);
        spoon_ -> setZOrder(10);
        spoon_ -> setTag(kTagSpoon);
        _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener -> clone(), spoon_);

    }
    if (extras_)
    {
        extras_ -> stopAllActions();
        extras_ -> setPosition(244, 498-80);
        extras_ -> setTag(kTagUmbrella);
        _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener -> clone(), extras_);

    }

}

void MakeLayer::scribbleFinished()
{
    this -> scribbleScreenShot();
    
    this -> runAction(Sequence::create(DelayTime::create(0.1),
                                       MoveBy::create(0.2, Vec2(800, 0)),
                                       RemoveSelf::create(),
                                       NULL));

}


void MakeLayer::scribbleScreenShot()
{
    auto size = Director::getInstance() -> getVisibleSize();
    
    //clippingnode screenshot!!!
    RenderTexture* rtx = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    Scene* curScene = Director::getInstance()->getRunningScene();
    Point ancPos = curScene->getAnchorPoint();
    
    rtx -> begin();
    snowcone_ -> visit();
    scribbleLayer_ -> visit();
    rtx -> end();
    rtx->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    static int a = 0;
    if (a>5)
    {
        a = 0;
    }
    auto fileName = __String::createWithFormat("scribble%d",a);
    rtx -> saveToFile(fileName -> getCString());
    a++;

}


void MakeLayer::screenShotForShare()
{

    auto size = Director::getInstance() -> getVisibleSize();
    
    //clippingnode screenshot!!!
    RenderTexture* rtx = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    this -> setPosition(this->getPosition()+Vec2(0, 200));
    rtx -> begin();
    this -> visit();
    rtx -> end();
    Director::getInstance()->getRenderer()->render();
    rtx->getSprite()->getTexture()->setAntiAliasTexParameters();
    this -> setPosition(this->getPosition()+Vec2(0, -200));

    auto fileName = "eatforshare.png";
    rtx -> saveToFile(fileName,true);
}


