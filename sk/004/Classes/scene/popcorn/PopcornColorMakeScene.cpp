//
//  PopcornColorMakeScene.cpp
//  shake004
//
//  Created by liji on 16/10/25.
//
//

#include "PopcornColorMakeScene.h"
#include "TapDropNode.h"
#include "BowlNode.h"
#include "LiquidNode.h"
#include "ButterNode.h"
#include "ContentSpoonNode.h"
#include "ComponentCycleScroll.h"
#include "RewardManager.h"
#include "LockUIManager.h"
#include "ESDataManager.h"
#include "MixSpoonNode.h"
#include "ScoopSpoon.h"
#include "SceneHeader.h"
#include "ESScrollView.h"
#include "ESUnitAdapter.h"
PopcornColorMakeScene::PopcornColorMakeScene():
_bowl(nullptr),
_previousColor(nullptr),
_butterTipIndex(0),
_scrollBG(nullptr),
_box(nullptr)
{

}

PopcornColorMakeScene::~PopcornColorMakeScene()
{

}

bool PopcornColorMakeScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    this->setBackground("res/popcorn/make_bg.png");

    this->showBowl();
    
//    this->chooseBox();
    return true;
}

void PopcornColorMakeScene::showBowl()
{
    kAudioUtil->playEffect(kVoicePopcornMakecaremel);
    
    _bowl = BowlNode::create("res/popcorn/5/bowl2_down.png", "res/popcorn/5/bowl2_up.png");
    this->addToContentLayer(_bowl, 1);
    ActionHelper::moveInToSceneExponentialOut(_bowl, CMVisibleRect::getPosition(202, 445), ActionDirection::kActionDirectionLeft,CC_CALLBACK_0(PopcornColorMakeScene::addWater, this));
}

void PopcornColorMakeScene::addWater()
{
    _ingredientsCount = 1;

    auto water = LiquidNode::create("res/popcorn/5/cup2_0.png","res/popcorn/5/cup2_1.png","res/popcorn/5/cup2_shadow.png","res/popcorn/5/water.png");
    addToContentLayer(water, 11);
    
    auto waterInCall = [=]
    {
        water->onMoveToTargetCallback = [=]()
        {
            auto node = TipsUIController::getInstance()->showAccelerateTip(this, water->convertToWorldSpaceAR(Vec2::ZERO) + Vec2(-150.0, 0.0),true);
            node->setLocalZOrder(100);
        };
        
        water->setPourWorldPoint(Vec2(_bowl->getPosition().x + 60, _bowl->getPosition().y + 200.0), [=](){
            
            kAudioUtil->playEffect(kSoundAddWater);
            
            auto animate = ActionHelper::createAnimate("res/popcorn/5/water%d.png", 1, 2);
            auto waterSprite = Sprite::create("res/popcorn/5/water1.png");
            waterSprite->setPosition(Vec2(water->getPosition().x - 115, water->getPosition().y - 132));
            this->addToContentLayer(waterSprite, 10);
            
            waterSprite->runAction(Sequence::create(animate, animate, animate, CallFunc::create([=](){
                
                TipsUIController::getInstance()->removeAllTips();
                
                _bowl->addContent("res/popcorn/5/bowl2_water.png", "water", 1);
                _bowl->setContentVisible("water");
                waterSprite->removeFromParent();
                water->backToOriginal([=](){
                    ActionHelper::moveOutofSceneAndRemoveSelf(water, kActionDirectionRight);
                    
                    if (this->isStepOver())
                    {
                        this->showIngredients();
                    }
                    
                });
            }), NULL));
        });
    };
    ActionHelper::moveInToSceneExponentialOut(water, CMVisibleRect::getPosition(501,387), ActionDirection::kActionDirectionRight,waterInCall);
}

void PopcornColorMakeScene::showIngredients()
{
    _ingredientsCount = 3;
    addButter();
    addSugar();
    addSyrup();
}

void PopcornColorMakeScene::addButter()
{
    auto butterPlate = Sprite::create("res/popcorn/5/butter_plate.png");
    auto butter = ButterNode::createPicChange("res/popcorn/5/butter%d.png",Vec2(0, 4));
    butter->setPosition(butterPlate->getContentSize() * 0.5);
    butterPlate->addChild(butter);
    
    addToContentLayer(butterPlate, 11);
    
    ActionHelper::moveInToSceneExponentialOut(butterPlate, CMVisibleRect::getPosition(228, 223), ActionDirection::kActionDirectionRight);
    
    butter->onMoveToTargetCallback = [=]()
    {
        
        auto node = TipsUIController::getInstance()->showClickTip(this, butter->convertToWorldSpaceAR(Vec2(0.0, -20.00)));
        node->setLocalZOrder(100);
    };
    butter->setPourWorldPoint(Vec2(_bowl->getPosition().x, _bowl->getPosition().y + 100.0), [=](){
        TipsUIController::getInstance()->removeAllTips();

        kAudioUtil->playEffect(kSoundAddButter);
        
        auto butterTip = Sprite::create("res/popcorn/5/bowl_butter0.png");
        butterTip->setPosition(_bowl->convertToNodeSpaceAR(butter->convertToWorldSpaceAR(Vec2::ZERO)));
        _bowl->addChild(butterTip, 1);
        
        float x = random(-60.0, 60.0);
        float y = random(-30.0, 50.0);
        float roatate = random(0.0, 360.0);
        
        butterTip->runAction(Spawn::create(MoveTo::create(0.3, Vec2(x, y)), RotateTo::create(0.3, roatate), ScaleTo::create(0.3, 1), NULL));
        
        if (++_butterTipIndex == 5)
        {
            ActionHelper::moveOutofSceneAndRemoveSelf(butterPlate, kActionDirectionRight);
            
            //finish
            butter->removeFromParent();
            if (this->isStepOver())
            {
                this->addColors();
            }
        }
    });
}

void PopcornColorMakeScene::addSugar()
{
    auto cronstarch = Sprite::create("res/popcorn/5/sugar1.png");
    this->addToContentLayer(cronstarch,13);
    
    auto inCronstarch = Sprite::create("res/popcorn/5/sugar2.png");
    cronstarch->addChild(inCronstarch);
    inCronstarch->setPosition(Vec2(70, 100));
    ActionHelper::moveInToSceneBounce(cronstarch, CMVisibleRect::getPosition(466,301), ActionDirection::kActionDirectionRight);
    
    auto flourSpoon = ContentSpoonNode::create("res/popcorn/5/sugar_spoon1.png", "res/popcorn/5/sugar_spoon2.png");
    addToContentLayer(flourSpoon, 15);
    auto spoonCall = [=]
    {
        flourSpoon->startDragToWorldPositonPour(Vec2(_bowl->getPosition().x, _bowl->getPosition().y + 180.0),
                                                "particle/flour.plist",
                                                [=](){
                                                    
                                                    kAudioUtil->playEffect(kSoundAddSugar);
                                                    
                                                    _bowl->addContent("res/popcorn/5/bowl_sugar.png", "bowl_sugar.png", 1);
                                                    _bowl->setContentVisible("bowl_sugar.png");
                                                    
                                                    if (this->isStepOver())
                                                    {
                                                        this->addColors();
                                                    }
                                                    
                                                });
        flourSpoon->backToOriginalPositionCallback = [=]
        {
            
            ActionHelper::moveOutofSceneAndRemoveSelf(cronstarch, kActionDirectionRight);
            ActionHelper::moveOutofSceneAndRemoveSelf(flourSpoon, kActionDirectionRight);
        };
        
    };
    ActionHelper::moveInToSceneBounce(flourSpoon, CMVisibleRect::getPosition(511,416), ActionDirection::kActionDirectionRight,spoonCall);
}

void PopcornColorMakeScene::addSyrup()
{
    auto syrup = TapDropNode::create("res/popcorn/5/corn_syrup.png");
    addToContentLayer(syrup, 9);
    _bowl->addContent("res/popcorn/5/bowl_corn_syrup.png", "syrup", 8, Vec2(0.0, -30));
    ActionHelper::moveInToSceneExponentialOut(syrup, CMVisibleRect::getPosition(511, 576), ActionDirection::kActionDirectionRight, [=](){
        
        syrup->setPourWorldPoint(CMVisibleRect::getPosition(333,691));
        syrup->dragBeginCall = [=]
        {
            
        };
        syrup->rotateDoneCall = [=]
        {
            auto node = TipsUIController::getInstance()->showClickTip(this, syrup->convertToWorldSpaceAR(Vec2(0.0, -50.0)));
            node->setLocalZOrder(100);
        };
        
        syrup->dropShowCall = [=]
        {
            TipsUIController::getInstance()->removeAllTips();
            auto drop = Sprite::create("res/popcorn/5/corn_syrup1.png");
            drop->setPosition(CMVisibleRect::getPosition(273, 334));
            drop->runAction(Repeat::create(Sequence::create(
                                                            CallFunc::create([]
                                                                             {
                                                                                 kAudioUtil->playEffect("Water drop.mp3");
                                                                             }),
                                                            Place::create(CMVisibleRect::getPosition(202,485)),
                                                            FadeIn::create(0),
                                                            MoveBy::create(0.3, Vec2(0, -60)),
                                                            FadeOut::create(0.3),
                                                            NULL), 1));
            this->addToContentLayer(drop, 26);
            _bowl->setContentVisible("syrup");
        };
        syrup->dropDoneCall = [=]()
        {
            syrup->backToOriginal([=](){
                
                ActionHelper::moveOutofSceneAndRemoveSelf(syrup, ActionDirection::kActionDirectionRight, [=](){
                    
                    if (this->isStepOver())
                    {
                        this->addColors();
                    }
                });
            });
        };
    });

}

void PopcornColorMakeScene::addColors()
{
    _scrollBG = Sprite::create("ui/dec/bg.png");
    _scrollBG->setAnchorPoint(Vec2(0.5,1));
    this->addToUILayer(_scrollBG,2);
    _scrollBG->setPosition(CMVisibleRect::getPosition(320,130-300,kBorderNone,kBorderTop));
    _scrollBG->runAction(Sequence::create(MoveBy::create(0.6f, Vec2(0,-300)),
                                          CallFunc::create([=]
                                                           {
//                                                               scrollComp->scrollBy(0.3f, Vec2(200, 0), nullptr);
                                                           }),
                                          NULL));

    auto scrollNode = Node::create();
    scrollNode->setPosition(_scrollBG->getContentSize()/2);
    _scrollBG->addChild(scrollNode, 5);
    
    auto adapter = ESUnitAdapter::create("res/popcorn/5/icon_coloring_bottle/icon_coloring%d_bottle.png", 1, 10);
    auto scroll = ESScrollView::create();
    scroll->setItemAdapter(adapter);
    scroll->setDirection(ScrollView::Direction::HORIZONTAL);
    scroll->setContentSize(Size(_scrollBG->getContentSize().width-60,90));
    scroll->setMarginX(110);
    scroll->setMarginY(90);
    scroll->loadScrollView();
    //    scroll->setPosition(CMVisibleRect::getPosition(320, 725));
    scroll->setAnchorPoint(Vec2(0.5, 0.5));
    scrollNode->addChild(scroll);
    scroll->loadScrollView();
    
    for (auto item : scroll->vec_Item)
    {
        item->setScale(0.8f);
        int index = item->getTag();
        auto key = kRewardManager->getItemKey("popcorn", "flavor", index);
        item->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "flavor", index, item))
        {
            LockUIManager::getInstance()->registerLock("popcorn", "flavor", index, item,Vec2(100,20));
        }
    }
    
    adapter->unitTouchEnded = [=](ui::ImageView* item)
    {
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllContentString());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree("box", item->getTag()))
            {
                auto keyObject = item->getUserObject();
                if(!keyObject)
                    return;
                string key = ((__String*)(keyObject))->getCString();
                if(kRewardManager->isLocked(key))
                {
                    //如果能找到reward名字的lock，显示reward，否则显示iap
                    if (item->getChildByName(LOCK_NAME_REWARD))
                        kRewardManager->showRewardAds(key);
                    return;
                }
            }
        }
        auto popcorn = kESDataManager->getPopcornFood();
        popcorn.flavorIndex = item->getTag();
        kESDataManager->setPopcornFood(popcorn);
        colorChoosed(item->getTag());
    };
    scroll->show(nullptr);

    return;
    auto scrollComp = ComponentCycleScroll::create();
    scrollComp->setEnableMoveItem(true);
    scrollComp->setCycleScrollSize(Size(_scrollBG->getContentSize().width-100, 60+30));
    scrollNode->addComponent(scrollComp);
    scrollComp->setMargin(90);
    
    string bunFormat;
    for (int index=1; index<11; index++)
    {
        bunFormat = StringUtils::format("res/popcorn/5/icon_coloring_bottle/icon_coloring%d_bottle.png",index);
        auto icon = ImageView::create(bunFormat);
        auto widget = Widget::create();
        widget->setContentSize(icon->getContentSize());
        icon->setPosition(widget->getContentSize()/2);
        widget->addChild(icon, 2);
        widget->setTag(index);
        widget->setScale(0.85f);
        scrollComp->pushCustomItem(widget);
        
        auto key = kRewardManager->getItemKey("popcorn", "flavor", index);
        widget->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "flavor", index, widget))
        {
            LockUIManager::getInstance()->registerLock("popcorn", "flavor", index, widget);
        }

    }
    scrollComp->itemClickCallback=[=](Widget *item, Touch *touch)
    {
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllContentString());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree("flavor", item->getTag()))
            {
                auto keyObject = item->getUserObject();
                if(!keyObject)
                    return;
                string key = ((__String*)(keyObject))->getCString();
                if(kRewardManager->isLocked(key))
                {
                    //如果能找到reward名字的lock，显示reward，否则显示iap
                    if (item->getChildByName(LOCK_NAME_REWARD))
                        kRewardManager->showRewardAds(key);
                    return;
                }
            }
        }
        auto popcorn = kESDataManager->getPopcornFood();
        popcorn.flavorIndex = item->getTag();
        kESDataManager->setPopcornFood(popcorn);
        colorChoosed(item->getTag());
    };
    
    scrollComp->start();

    _scrollBG->runAction(Sequence::create(MoveBy::create(0.6f, Vec2(0,-300)),
                                                         CallFunc::create([=]
    {
        scrollComp->scrollBy(0.3f, Vec2(200, 0), nullptr);
    }),
                                                                          NULL));
}

void PopcornColorMakeScene::colorChoosed(int index)
{
    if (_previousColor)
    {
        ActionHelper::moveOutofSceneAndRemoveSelf(_previousColor, kActionDirectionRight);
        _previousColor = nullptr;
    }
    
    string bottlePath = StringUtils::format("res/popcorn/5/coloring_bottle/coloring%d_bottle.png",index);
    string dropPath = StringUtils::format("res/popcorn/5/coloring_bottle/coloring%d.png",index);
    string inBowlPath = StringUtils::format("res/popcorn/5/bowl_coloring/bowl_coloring%d.png",index);
    
    auto syrup = TapDropNode::create(bottlePath);
    addToContentLayer(syrup, 9);
    _previousColor = syrup;
    _bowl->addContent(inBowlPath, "inBowl", 9, Vec2(10.0, 10));
    ActionHelper::moveInToSceneExponentialOut(syrup, CMVisibleRect::getPosition(511, 576), ActionDirection::kActionDirectionRight, [=](){
        
        syrup->setPourWorldPoint(CMVisibleRect::getPosition(333,691));
        syrup->dragBeginCall = [=]
        {
            if (_scrollBG)
            {
                _scrollBG->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, 300)),
                                                      RemoveSelf::create(),
                                                      NULL));
                _scrollBG = nullptr;
            }
        };
        syrup->rotateDoneCall = [=]
        {
            auto node = TipsUIController::getInstance()->showClickTip(this, syrup->convertToWorldSpaceAR(Vec2(0.0, -50.0)));
            node->setLocalZOrder(100);
        };
        
        syrup->dropShowCall = [=]
        {
            TipsUIController::getInstance()->removeAllTips();
            auto drop = Sprite::create(dropPath);
            drop->setPosition(CMVisibleRect::getPosition(273, 334));
            drop->runAction(Repeat::create(Sequence::create(
                                                            CallFunc::create([]
                                                                             {
                                                                                 kAudioUtil->playEffect("Water drop.mp3");
                                                                             }),
                                                            Place::create(CMVisibleRect::getPosition(202,485+80)),
                                                            FadeIn::create(0),
                                                            MoveBy::create(0.3, Vec2(0, -60)),
                                                            FadeOut::create(0.3),
                                                            NULL), 1));
            this->addToContentLayer(drop, 26);
            _bowl->setContentVisible("inBowl");
        };
        syrup->dropDoneCall = [=]()
        {
            syrup->backToOriginal([=](){
                
                ActionHelper::moveOutofSceneAndRemoveSelf(syrup, ActionDirection::kActionDirectionRight, [=](){
                    
                    _bowl->runAction(Sequence::create(MoveTo::create(0.5f, CMVisibleRect::getPosition(320, 420)),
                                                      CallFunc::create([=]
                    {
                        this->mixColors();
                    }),
                                                      NULL));
                });
            });
        };
    });
}

void PopcornColorMakeScene::mixColors()
{
    kAudioUtil->playEffect("popcorn6.mp3");
    
    auto spoon = MixSpoonNode::create("res/popcorn/5/spoon2.png");
    addToContentLayer(spoon, 20);
    ActionHelper::moveInToSceneExponentialOut(spoon, CMVisibleRect::getPosition(320, 485+50), ActionDirection::kActionDirectionLeft,[=]
    {
        TipsUIController::getInstance()->showMixTip45Degree(this, CMVisibleRect::getPosition(320, 485+50));
    });
    auto popcorn = kESDataManager->getPopcornFood();
    _bowl->addContent(StringUtils::format("res/popcorn/5/mix/mix_coloring%d_1.png", popcorn.flavorIndex), "MixContent", 20);
    
    static bool isFirst = true;
    isFirst = true;
    spoon->mixPercentCallback = [=](float percent)
    {
        if (isFirst)
        {
            isFirst = false;
            _bowl->setContentVisible("MixContent");
            _bowl->setContentVisible("water", nullptr, false, false);
            TipsUIController::getInstance()->removeAllTips();
        }
        
        if(percent > 99.9){
            
            SoundConfig::getInstance()->stopLoopEffect();
            TipsUIController::getInstance()->removeAllTips();

            ActionHelper::moveOutofSceneAndRemoveSelf(spoon, ActionDirection::kActionDirectionLeft, [=](){
                
//                _bowl->runAction(Sequence::create(MoveTo::create(0.8f, CMVisibleRect::getCenterOfScene()),
//                                                 EaseBackOut::create(ScaleTo::create(0.4f,1.1)),
//                                                 EaseBackIn::create(ScaleTo::create(0.4f,1)),
//                                                 NULL));
                //                        this->showNextButton();
                kSoundInstance->playRandomStepFinishVoice();
                this->runAction(Sequence::create(DelayTime::create(1.f),
                                                 CallFunc::create([=]
                {
                    this->mixPopcorn();
                }),
                                                 NULL));
            });
        }
    };
    spoon->mixPaused=[=]
    {
        SoundConfig::getInstance()->stopLoopEffect();
    };
    auto boudingbox = _bowl->getBowlRect();
    boudingbox.origin = _bowl->convertToWorldSpace(boudingbox.origin);
    
    std::string format = "res/popcorn/5/mix/mix_coloring" + std::to_string(popcorn.flavorIndex) + "_%d.png";
    Vec2 range = Vec2(1, 5);
    spoon->startMix(boudingbox,
                    dynamic_cast<Sprite*>(_bowl->getChildByName("MixContent")),
                    format,
                    range.x,
                    range.y);
}

void PopcornColorMakeScene::mixPopcorn()
{
    _bowl->runAction(MoveTo::create(0.3, CMVisibleRect::getPosition(472, 281)));
    _bowl->runAction(ScaleTo::create(0.3, 0.8f));
    auto popcorn = kESDataManager->getPopcornFood();

    auto cornBowl = BowlNode::create("res/popcorn/6/bowl2.png", "res/popcorn/6/bowl1.png");
    this->addToContentLayer(cornBowl, 11);
    auto corn = cornBowl->addContent("res/popcorn/6/1.png", "corn", 2,Vec2(0, 40));
    corn->setOpacity(255);
    
    ActionHelper::moveInToSceneExponentialOut(cornBowl, CMVisibleRect::getPosition(201, 479), ActionDirection::kActionDirectionLeft, [=](){
        
        auto spoon = ScoopSpoon::create("res/popcorn/6/spoon.png");
        spoon->setContent(StringUtils::format("res/popcorn/6/spoon_coloring%d.png", popcorn.flavorIndex));
        spoon->setScale(0.8f);
        this->addToContentLayer(spoon, 22);
//        spoon->setEnableContentMoveAnimation(false);
//        spoon->setEnableScoopAnimation(false);
//        spoon->setEnablePourAnimation(false);
        ActionHelper::moveInToSceneExponentialOut(spoon, CMVisibleRect::getPosition(567, 391), ActionDirection::kActionDirectionRight, [=](){
            
            spoon->startDragToWorldPositon(CMVisibleRect::getPosition(567, 391));
            spoon->onScroopedCallback = [=]()
            {
                spoon->setBackPosition(CMVisibleRect::getPosition(300, 668));
            };
            spoon->onFinishPourCallback = [=]()
            {
                kAudioUtil->playEffect("Pouring caramel.mp3");
                cornBowl->runAction(MoveTo::create(0.5, CMVisibleRect::getPosition(320, 400)));
                ActionHelper::moveOutofSceneAndRemoveSelf(spoon, ActionDirection::kActionDirectionLeft);
                ActionHelper::moveOutofSceneAndRemoveSelf(_bowl, ActionDirection::kActionDirectionRight);
                
                auto content = cornBowl->addContent(StringUtils::format("res/popcorn/6/bowl2_coloring%d.png", popcorn.flavorIndex), "color", 5);
                content->setPosition(Vec2(0.0, 115.0));
                content->runAction(Sequence::create(FadeIn::create(0.5), CallFunc::create([=](){
                    
                    auto mixSpoon = MixSpoonNode::create("res/popcorn/5/spoon2.png");
                    this->addToContentLayer(mixSpoon, 20);
                    ActionHelper::moveInToSceneExponentialOut(mixSpoon, CMVisibleRect::getPosition(317, 584), ActionDirection::kActionDirectionRight);
//                    auto content1 = cornBowl->addContent(StringUtils::format("res/popcorn/6/bowl_color_popcorn%d.png", popcorn.flavorIndex), "MixContent", 25, Vec2(0.0, 40.f));
                    auto content1 =Sprite::create(StringUtils::format("res/popcorn/6/bowl_color_popcorn%d.png", popcorn.flavorIndex));
                    corn->addChild(content1);
                    content1->setOpacity(0);
                    content1->setPosition(corn->getContentSize()/2);
                    
                    static bool isFirst = true;
                    isFirst = true;
                    mixSpoon->mixPercentCallback = [=](float percent)
                    {
                        if (isFirst)
                        {
                            isFirst = false;
//                            cornBowl->setContentVisible("corn", nullptr, false, false);
                            cornBowl->setContentVisible("color", nullptr, false, false);
                            TipsUIController::getInstance()->removeAllTips();
                        }
                        float p = percent/100*255*2;
                        log ("%f", p);
                        content1->setOpacity(p);

                        if(percent > 51){
                            dynamic_cast<Sprite*>(corn)->setTexture(StringUtils::format("res/popcorn/6/bowl_color_popcorn%d.png", popcorn.flavorIndex));

                            SoundConfig::getInstance()->stopLoopEffect();
                            
                            
                            ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoon, ActionDirection::kActionDirectionLeft);
                            ActionHelper::moveOutofSceneAndRemoveSelf(cornBowl, kActionDirectionRight,CC_CALLBACK_0(PopcornColorMakeScene::chooseBox, this));
                        }
                    };
                    mixSpoon->mixPaused=[=]
                    {
                        SoundConfig::getInstance()->stopLoopEffect();
                    };
                    auto boudingbox = cornBowl->getBowlRect();
                    boudingbox.origin = cornBowl->convertToWorldSpace(boudingbox.origin);

                    std::string format = "";
                    Vec2 range = Vec2(0, 0);
//                    mixSpoon->setEnableAnimation(false);
                    mixSpoon->startMix(boudingbox,
                                       dynamic_cast<Sprite*>(corn),
                                       format,
                                       range.x,
                                       range.y);
                }), NULL));
            };
        });
    });
}

void PopcornColorMakeScene::chooseBox()
{
    kAudioUtil->playEffect(kVoiceDecChooseBox);

    _box = BowlNode::create("res/popcorn/7/box/box_down.png", "res/popcorn/7/box/box_1.png");
    this->addToContentLayer(_box,10);
    ActionHelper::moveInToSceneBounce(_box, CMVisibleRect::getPosition(320, 325), kActionDirectionLeft);
    
    _scrollBG = Sprite::create("ui/dec/bg.png");
    _scrollBG->setAnchorPoint(Vec2(0.5,1));
    this->addToUILayer(_scrollBG,2);
    _scrollBG->setPosition(CMVisibleRect::getPosition(320,130-300,kBorderNone,kBorderTop));
    _scrollBG->runAction(MoveBy::create(0.6f, Vec2(0,-300)));

    auto scrollNode = Node::create();
    scrollNode->setPosition(_scrollBG->getContentSize()/2);
    _scrollBG->addChild(scrollNode, 5);
    
    auto adapter = ESUnitAdapter::create("res/popcorn/7/icon_box/icon_box%d.png", 1, 9);
    auto scroll = ESScrollView::create();
    scroll->setItemAdapter(adapter);
    scroll->setDirection(ScrollView::Direction::HORIZONTAL);
    scroll->setContentSize(Size(_scrollBG->getContentSize().width-60,90));
    scroll->setMarginX(110);
    scroll->setMarginY(90);
    scroll->loadScrollView();
    //    scroll->setPosition(CMVisibleRect::getPosition(320, 725));
    scroll->setAnchorPoint(Vec2(0.5, 0.5));
    scrollNode->addChild(scroll);
    scroll->loadScrollView();
    
    for (auto item : scroll->vec_Item)
    {
        item->setScale(0.8f);
        int index = item->getTag();
        auto key = kRewardManager->getItemKey("popcorn", "box", index);
        item->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "box", index, item))
        {
            LockUIManager::getInstance()->registerLock("popcorn", "box", index, item,Vec2(100,20));
        }
    }
    
    adapter->unitTouchEnded = [=](ui::ImageView* item)
    {
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllContentString());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree("box", item->getTag()))
            {
                auto keyObject = item->getUserObject();
                if(!keyObject)
                    return;
                string key = ((__String*)(keyObject))->getCString();
                if(kRewardManager->isLocked(key))
                {
                    //如果能找到reward名字的lock，显示reward，否则显示iap
                    if (item->getChildByName(LOCK_NAME_REWARD))
                        kRewardManager->showRewardAds(key);
                    return;
                }
            }
        }
        scroll->setEnabled(false);
        auto popcorn = kESDataManager->getPopcornFood();
        popcorn.boxIndex = item->getTag();
        kESDataManager->setPopcornFood(popcorn);
        
        this->showNextButton();
        this->onNextButtonTouchCallback = [=]
        {
            scroll->setEnabled(false);
            this->fillPopcorn();
            if (_scrollBG)
            {
                _scrollBG->runAction(Sequence::create(MoveBy::create(0.6f, Vec2(0, 300)),
                                                      RemoveSelf::create(),
                                                      NULL));
                _scrollBG = nullptr;
            }
        };
        
        auto boxPath = StringUtils::format("res/popcorn/7/box/box_%d.png",item->getTag());
        auto changeCall = [=]
        {
            scroll->setEnabled(true);
            _box->updateBowlConfig("res/popcorn/7/box/box_down.png", boxPath);
            
            kAudioUtil->playEffect(kSoundShow);
            
            auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
            particle->setPosition(_box->getPosition());
            this->addToContentLayer(particle,999);
        };
        
        auto tempBox = Sprite::create(boxPath);
        tempBox->setScale(0.3f);
        auto startPos = scroll->getInnerContainer()->convertToWorldSpace(item->getPosition());
        tempBox->setPosition(startPos);
        
        tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,_box->getPosition(), 0.8),
                                                          EaseBackOut::create(ScaleTo::create(0.8, 1.15)),
                                                          NULL),
                                            CallFunc::create(changeCall),
                                            RemoveSelf::create(),
                                            NULL));
        this->addChild(tempBox,INT_MAX);
        
    };
    scroll->show(nullptr);

    
    return;
    auto scrollComp = ComponentCycleScroll::create();
    scrollComp->setEnableMoveItem(true);
    scrollComp->setCycleScrollSize(Size(_scrollBG->getContentSize().width-100, 60+30));
    scrollNode->addComponent(scrollComp);
    scrollComp->setMargin(110);
    
    string bunFormat;
    for (int index=1; index<10; index++)
    {
        bunFormat = StringUtils::format("res/popcorn/7/icon_box/icon_box%d.png",index);
        auto icon = ImageView::create(bunFormat);
        auto widget = Widget::create();
        widget->setContentSize(icon->getContentSize());
        icon->setPosition(widget->getContentSize()/2);
        icon->setScale(0.8f);
        widget->addChild(icon, 2);
        widget->setTag(index);
        scrollComp->pushCustomItem(widget);
        
        auto key = kRewardManager->getItemKey("popcorn", "box", index);
        widget->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "box", index, widget))
        {
            LockUIManager::getInstance()->registerLock("popcorn", "box", index, widget);
        }
    }
    LockUIManager::getInstance()->updateLockStatus();
    scrollComp->itemClickCallback=[=](Widget *item, Touch *touch)
    {
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllContentString());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree("box", item->getTag()))
            {
                auto keyObject = item->getUserObject();
                if(!keyObject)
                    return;
                string key = ((__String*)(keyObject))->getCString();
                if(kRewardManager->isLocked(key))
                {
                    //如果能找到reward名字的lock，显示reward，否则显示iap
                    if (item->getChildByName(LOCK_NAME_REWARD))
                        kRewardManager->showRewardAds(key);
                    return;
                }
            }
        }
        scrollComp->setEnabled(false);

        auto popcorn = kESDataManager->getPopcornFood();
        popcorn.boxIndex = item->getTag();
        kESDataManager->setPopcornFood(popcorn);

        this->showNextButton();
        this->onNextButtonTouchCallback = [=]
        {
            scrollComp->setEnabled(false);
            this->fillPopcorn();
            if (_scrollBG)
            {
                _scrollBG->runAction(Sequence::create(MoveBy::create(0.6f, Vec2(0, 300)),
                                                      RemoveSelf::create(),
                                                      NULL));
                _scrollBG = nullptr;
            }
        };
        
        auto boxPath = StringUtils::format("res/popcorn/7/box/box_%d.png",item->getTag());
        auto changeCall = [=]
        {
            scrollComp->setEnabled(true);
            _box->updateBowlConfig("res/popcorn/7/box/box_down.png", boxPath);
            
            kAudioUtil->playEffect(kSoundShow);
            
            auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
            particle->setPosition(_box->getPosition());
            this->addToContentLayer(particle,999);
        };
        
        auto tempBox = Sprite::create(boxPath);
        tempBox->setScale(0.3f);
        tempBox->setPosition(touch->getLocation());
        auto startPos = touch->getLocation();
        
        tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(touch->getLocation(),_box->getPosition(), 0.8),
                                                          EaseBackOut::create(ScaleTo::create(0.8, 1.15)),
                                                          NULL),
                                            CallFunc::create(changeCall),
                                            RemoveSelf::create(),
                                            NULL));
        this->addChild(tempBox,INT_MAX);
    };
    
    scrollComp->start();
    scrollComp->scrollBy(0.3f, Vec2(200, 0), nullptr);

    _scrollBG->runAction(MoveBy::create(0.6f, Vec2(0,-300)));
}

void PopcornColorMakeScene::fillPopcorn()
{
    if (!_box)
    {
        _box = BowlNode::create("res/popcorn/7/box/box_down.png", "res/popcorn/7/box/box_1.png");
        this->addToContentLayer(_box,10);
        _box->setPosition(CMVisibleRect::getPosition(320, 325));
    }
    this->hideNextButton();
    _box->runAction(MoveTo::create(0.6f, CMVisibleRect::getPosition(235,482)));
    _box->runAction(ScaleTo::create(0.6f, 0.9f));
    
    auto popcorn = kESDataManager->getPopcornFood();

    auto popcornInBox = Sprite::create();
    _box->addChild(popcornInBox,2);
    popcornInBox->setPosition(0, 229);
    

    auto cornBowl = BowlNode::create("res/popcorn/6/bowl2.png", "res/popcorn/6/bowl1.png");
    this->addToContentLayer(cornBowl, 11);
    cornBowl->setScale(0.8f);
    auto corn = cornBowl->addContent(StringUtils::format("res/popcorn/6/bowl_color_popcorn%d.png",popcorn.flavorIndex), "corn", 2,Vec2(0, 40));
    corn->setOpacity(255);
    
    ActionHelper::moveInToSceneBounce(cornBowl, CMVisibleRect::getPosition(491, 255), kActionDirectionRight,[=]
    {
        cornBowl->setPourWorldPoint(CMVisibleRect::getPosition(475,788), [=]
                                {
                                    TipsUIController::getInstance()->showAccelerateTip(this, cornBowl->getPosition(),true);
                                    
                                    cornBowl->beginPour(-45.0, [=](){
                                        popcornInBox->setTexture(StringUtils::format("res/popcorn/7/box_popcorn%d.png",popcorn.flavorIndex));
                                        popcornInBox->setTextureRect(Rect(0, -popcornInBox->getContentSize().height/2+120, popcornInBox->getContentSize().width, popcornInBox->getContentSize().height));
                                        popcornInBox->runAction(SpriteTextureRectTo::create(1.2f, Rect(0, 0, popcornInBox->getContentSize().width, popcornInBox->getContentSize().height)));
                                        
                                        kAudioUtil->playEffect("Pour corn kernal.mp3");
                                        TipsUIController::getInstance()->removeAccelerateTip();
                                        
                                        auto particle = ParticleSystemQuad::create("particle/cornParticle.plist");
                                        particle->setPosition(cornBowl->getPosition() + Vec2(-cornBowl->getContentSize().width/2, -50.0));
                                        this->addToContentLayer(particle, 10);
                                        particle->setTexture(_director->getTextureCache()->addImage(StringUtils::format("res/popcorn/6/particle/%d.png",popcorn.flavorIndex)));
                                        
                                        corn->runAction(Sequence::create(ScaleTo::create(1.0, 0.0), CallFunc::create([=](){
                                            
                                            cornBowl->runAction(Sequence::create(RotateTo::create(0.3, 0.0), MoveBy::create(0.3, Vec2(400.0, 0.0)), CallFunc::create([=](){
                                                _box->runAction(Sequence::create(MoveTo::create(0.8f, CMVisibleRect::getCenterOfScene()),
                                                                                 EaseBackOut::create(ScaleTo::create(0.4f,1.1)),
                                                                                 EaseBackIn::create(ScaleTo::create(0.4f,1)),
                                                                                 CallFunc::create([=]
                                                                                                  {
                                                                                                      kSoundInstance->playRandomStepFinishVoice();
                                                                                                      this->showNextButton();
                                                                                                      this->onNextButtonTouchCallback=[=]
                                                                                                      {
                                                                                                          SceneManager::getInstance()->replaceWithDefaultTransition(PopcornDecScene::scene());
                                                                                                      };
                                                                                                  }),
                                                                                 NULL));
                                            }), RemoveSelf::create(), NULL));
                                        }), NULL));
                                    });
                                });
    });

    
    
}

