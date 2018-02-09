//
//  RainbowCupcakeDecScene.cpp
//  shake5008
//
//  Created by liji on 17/1/12.
//
//

#include "RainbowCupcakeDecScene.h"

#include "RainbowCupcakeDecScene.h"
#include "ScribbleSauce.h"
#include "ComponentTouchMove.h"
#include "SceneHeader.h"
#include "Analytics.h"
#include "LockUIManager.h"
#include "RewardManager.h"
#include "SceneHeader.h"
#include "ScribbleNode.h"
#include "ComponentScribblePercent.h"
#include "MakeNodeManager.h"

REGIST_SECENE(RainbowCupcakeDecScene)

bool RainbowCupcakeDecScene::init()
{
    if (!ESMakeBaseScene::init())
        return false;
    
    this->setBackground("res/rainbow_slushy/bg-2.png");
    
//    _module = "rainbowcupcake";
//    _iapIdentifier = ShopScene::getAllContentString();
    
    _food = RainbowCupcakeNode::create();
    this->addChild(_food,10);
    
    ActionHelper::moveInToSceneBounce(_food, CMVisibleRect::getPosition(320, 380), kActionDirectionLeft);
    
    
    //    this->showCorndogDec();
    showToppingDec();
//    showOtherDec();
    return true;
}

void RainbowCupcakeDecScene::showToppingDec()
{
    if (!_bag)
    {
        _bag = Sprite::create();
        //        _bag = Sprite::create(StringUtils::format("res/rainbow_cupcake/8/cream_bag/%d.png",itemTag));
        this->addChild(_bag,999);
        _bag->setAnchorPoint(Vec2(0.03,0.03));
        _bag->setPosition(CMVisibleRect::getPosition(-9999, -9999));
    }
    
    if (!_drawTopping)
    {
        _drawTopping = ComponentTouchMove::create();
        _drawTopping->setIsCanMove(false);
        this->addComponent(_drawTopping);
        _drawTopping->setEnabled(false);
    }

    kAudioUtil->playEffect("sound/voice over1/Wow So many flavors.mp3");

    DecorationConfig config = DecorationConfig("rainbowcupcake",
                                               "topping",
                                               "",
                                               "",
                                               "res/rainbow_cupcake/8/cream_bag/%d.png",
                                               "res/rainbow_cupcake/8/cream_topping/%d.png",
                                               1,
                                               12
                                               );
    
    ScrollViewConfig sConfig = ScrollViewConfig
    (
     Size(getContentSize().width,228),
     "",
     "",
     CMVisibleRect::getPosition(320, 731),
     CMVisibleRect::getPosition(320-600, 731),
     169+10
     );
    
    
    auto callback = [=](ui::Widget* item)
    {
        this->hideNextButton();
        _scrollView->setEnabled(false);
        _drawTopping->setEnabled(false);
        int itemTag = item->getTag();
        log("===item clicked %d====",itemTag);
        auto food = kESDataManager->getRainbowcupcakeFood();
        food.toppingIndex = itemTag;
        kESDataManager->setRainbowcupcakeFood(food);
        
        Vec2 pos = CMVisibleRect::getPosition(435,471);
        if (_bag->getPosition().x>0)
            pos = _bag->getPosition();
            
        auto changeCall = [=]
        {
            _drawTopping->setEnabled(true);
            _scrollView->setEnabled(true);
            
//            updateCup();
            //            _corndog->setVisible(true);
            //            _corndog->updateStick();
            this->showParticle(this, _bag->getPosition());
            _bag->setTexture(StringUtils::format(config.iconFormat.c_str(),itemTag));
            if (_bag->getPosition().x<0)
                _bag->setPosition(CMVisibleRect::getPosition(435,471));
            
            _food->scribbleTopping();
            _drawTopping->touchBeganCall = [=](Touch* t)
            {
                _bag->setPosition(t->getLocation());
            };
            _drawTopping->touchMovedCall = [=](Touch* t)
            {
                kSoundInstance->playLoopEffect(kSoundSmear);
                _bag->setPosition(t->getLocation());
                _food->scribble(this->convertToWorldSpaceAR(t->getLocation()));
            };
            _drawTopping->touchEndedCall = [=](Touch* t)
            {
                kSoundInstance->stopLoopEffect();
                _food->checkScribble([=]
                {
                    _drawTopping->setEnabled(false);
 
                    _bag->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(600, 0)),
                                                     Place::create(CMVisibleRect::getPosition(-9999, -9999)),
                                                     CallFunc::create([=]
                    {
                        _scrollView->setEnabled(true);
                        this->showNextButton();
                        this->showParticle(this, CMVisibleRect::getCenterOfScene());
                    }),
                                                     NULL));
                });
            };
            
        };
        auto tempBox = Sprite::create(StringUtils::format(config.iconFormat.c_str(),itemTag));
        tempBox->setAnchorPoint(Vec2(0.03,0.03));
        tempBox->setScale(0.3f);
        auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
        tempBox->setPosition(startPos);
        auto btm = Sprite::create();
        tempBox->addChild(btm,-1);
        btm->setPosition(tempBox->getContentSize()/2);
        
        tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,pos, 0.8),
                                                          EaseBackOut::create(ScaleTo::create(0.8, 1.15)),
                                                          NULL),
                                            CallFunc::create(changeCall),
                                            RemoveSelf::create(),
                                            NULL));
        this->addChild(tempBox,INT_MAX);
    };
    
    this->createScrollView(config,callback,sConfig,false);

    this->onNextButtonTouchCallback = [=]
    {
        this->hideNextButton();
        this->onNextButtonTouchCallback = nullptr;
        this->showOtherDec();
    };
    
}

void RainbowCupcakeDecScene::showOtherDec()
{
    this->removeScrollView();
    _ingredientsCount = 3;
    
    auto plateLeft = Sprite::create("res/rainbow_cupcake/9/plate_m.png");
    this->addChild(plateLeft,99);
    MakeNodeManager::getInstance()->registerRemoveNode(plateLeft, kActionDirectionLeft);
    
    auto stepOverCall = [=]
    {
        MakeNodeManager::getInstance()->removeAllRegisterNodes();
        
        this->stepFinishEffect(_food, []
                               {
                                   SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCupcakeDec2Scene::scene());
                               }, CMVisibleRect::getCenterOfScene());
    };
    
    for (int i = 0; i<2; i++)
    {
        auto item = BowlNode::create(StringUtils::format("res/rainbow_cupcake/9/marshmallow%d.png",i+1),"");
        plateLeft->addChild(item);
        item->setTag(i+1);
        item->setPosition(Vec2(123+60*i, 97+15*i));
        Vec2 pos;
        if (i == 0 )
        {
            pos = CMVisibleRect::getPosition(274,394);
        }
        if (i == 1)
        {
            pos = CMVisibleRect::getPosition(399,431);
        }
        item->_touchBeginCall = [=]
        {
            if (_shadow)
            {
                _shadow->setVisible(true);
            }
        };
        item->_touchEndedCall = [=]
        {
            if (_shadow)
            {
                _shadow->setVisible(false);
            }
        };

        item->setPourWorldPoint(pos, [=]
        {
            _shadow->setVisible(false);
            TipsUIController::getInstance()->removeAllTips();
            item->setVisible(false);
            _food->addMarshmallow(item->getTag());
            if (isOtherDecOver())
            {
                stepOverCall();
            }
        });
        
    }
    ActionHelper::moveInToSceneBounce(plateLeft, CMVisibleRect::getPosition(153, 677), kActionDirectionLeft,[=]
    {
        TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(490, 677), CMVisibleRect::getPosition(320, 380));
    });
    
    _shadow = Sprite::create("res/rainbow_cupcake/9/hint.png");
    this->addToContentLayer(_shadow,200);
    _shadow->setVisible(false);
    _shadow->setPosition(CMVisibleRect::getPosition(320, 437));
    
    auto plateRight = Sprite::create("res/rainbow_cupcake/9/plate_r.png");
    this->addChild(plateRight,99);
    MakeNodeManager::getInstance()->registerRemoveNode(plateRight, kActionDirectionRight);
    auto item = BowlNode::create("res/rainbow_cupcake/9/rainbow.png","");
    plateRight->addChild(item);
    item->setPosition(Vec2(150,112));
    Vec2 pos = CMVisibleRect::getPosition(320, 432);
    
    item->setPourWorldPoint(pos, [=]
                            {
                                TipsUIController::getInstance()->removeAllTips();
                                item->setVisible(false);
                                _food->addRainBow();
                                if (isOtherDecOver())
                                {
                                    stepOverCall();
                                }
                            });
    ActionHelper::moveInToSceneBounce(plateRight, CMVisibleRect::getPosition(490, 677), kActionDirectionLeft);

    this->onNextButtonTouchCallback = [=]
    {
        this->onNextButtonTouchCallback = nullptr;
    };

}

bool RainbowCupcakeDecScene::isOtherDecOver()
{
    _ingredientsCount--;
    if (_ingredientsCount == 0)
        return true;
    
    return false;
}



