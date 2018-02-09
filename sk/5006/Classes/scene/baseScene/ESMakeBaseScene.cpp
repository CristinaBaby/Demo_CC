//
//  ESMakeBaseScene.cpp
//  BHUG1064
//
//  Created by liji on 16/5/26.
//
//

#include "ESMakeBaseScene.h"
#include "SceneManager.h"
#include "SceneHeader.h"
#include "SSCMoreGameButton.h"
#include "AdsLoadingScene.h"
#include "DialogLayer.h"
#include "ShopScene.h"
#include "Analytics.h"
#include "RewardManager.h"
#include "LockUIManager.h"
#include "ComponentCycleScroll.h"
#include "MiniShopScene.h"
#include "ShopScene.h"
ESMakeBaseScene::ESMakeBaseScene():
_nextButton(nullptr),
isClicked(false),
_selectWidget(nullptr),
_selectBg(nullptr),
_scrollNode(nullptr),
_selectBgName(""),
_scrollView(nullptr),
_progressTimer(nullptr),
_progressBg(nullptr)
{


}


bool ESMakeBaseScene::init()
{
    if (!StudioLayer::init())
    {
        return false;
    }
    
    _iapIdentify = ShopScene::getAllFoodString();
    
    
    _nextButton = Button::create("ui/tap/next_btn.png");
    _nextButton->loadTexturePressed("ui/tap/next_btn.png");
    _nextButton->setScale(0.0);
    _nextButton->setAnchorPoint(Vec2(1, 1));
    _nextButton->setPosition(CMVisibleRect::getPosition(10, 10,kBorderRight,kBorderTop));
    _nextButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            if (!isClicked)
            {
                if (onNextButtonTouchCallback)
                    onNextButtonTouchCallback();
                
                kAudioUtil->playEffect(kSoundNext);

                isClicked=true;
                
                this->runAction(Sequence::create(DelayTime::create(0.2),
                                                 CallFunc::create([=]
                {
                
                    isClicked=false;
                }),
                                                 NULL));
                
            }
            
        }
    });
    addToUILayer(_nextButton, 20);

    SSCMoreGameButton* moreGameButton = SSCMoreGameButton::create(true);
    auto sizeMore = moreGameButton->getButtonSize();
    moreGameButton->setPosition(CMVisibleRect::getPosition(80+sizeMore.width/2,sizeMore.height/2-10,kBorderLeft,kBorderTop));
    moreGameButton->setClickCall([=](SSCMoreGameButton*){
        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    });
    addToUILayer(moreGameButton, 20);

    
    this->allStepOver=[=]
    {
        this->showNextButton();
    };
    
    
    _homeButton = Button::create("ui/tap/home_btn.png");
    _homeButton->setAnchorPoint(Vec2(0, 1));
    _homeButton->setPosition(CMVisibleRect::getPosition(10, 10,kBorderLeft,kBorderTop));
    _homeButton->setVisible(true);
    _homeButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            if (!isClicked)
            {
                kAudioUtil->stopAllEffect();
                SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());

//                AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
//                AdsLoadingScene::loadingDoneCallback = [=]
//                {
//                    SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
//                };
                
                kAudioUtil->playEffect(kSoundNext);
                
                isClicked=true;
                
                this->runAction(Sequence::create(DelayTime::create(0.2),
                                                 CallFunc::create([=]
                                                                  {
                                                                      
                                                                      isClicked=false;
                                                                  }),
                                                 NULL));
                
            }
            
        }
    });
    addToUILayer(_homeButton, 20);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyPressed = CC_CALLBACK_2(ESMakeBaseScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
#endif

    
    return true;
}


void ESMakeBaseScene::showNextButton()
{
    if (_nextButton->getScale() > 0.1)
        return;
    _nextButton->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 1.05), ScaleTo::create(0.3, 1.0), NULL)));
    _nextButton->runAction(Sequence::create(EaseExponentialInOut::create(ScaleTo::create(0.5, 1.0)), NULL));

}

void ESMakeBaseScene::hideNextButton()
{
    _nextButton->getActionManager()->removeAllActionsFromTarget(_nextButton);
    _nextButton->setScale(0.0);

}

void ESMakeBaseScene::hideHomeButton()
{
    _homeButton->setVisible(false);
    _homeButton->getActionManager()->removeAllActionsFromTarget(_nextButton);
}


void ESMakeBaseScene::showNextStep()
{
    if (_stepCount>=_stepName.size())
    {
        log("====step all over go to next scene===");
        
        if (allStepOver)
            allStepOver();
        _stepCount = 0;
        return;
    }
    
    std::string stepName = _stepName.at(_stepCount);
    _stepCount++;
    auto it = _steps.find(stepName);
    if(it != _steps.end())
    {
        auto callun = it->second;
        callun();
        
        log("======step name %s======", stepName.c_str());
    }
    
}

void ESMakeBaseScene::checkNextStep()
{
    if(isStepOver())
        this->showNextStep();
}

bool ESMakeBaseScene::isStepOver()
{
    _ingredientsCount--;
    if (_ingredientsCount<=0)
    {
        _ingredientsCount = 0;
        return true;
    }
    else
        return false;
    
}


void ESMakeBaseScene::registTouchEvent(ui::Widget* obj)
{
    obj->addTouchEventListener(CC_CALLBACK_2(ESMakeBaseScene::touchEvent, this));

}

void ESMakeBaseScene::touchEvent(Ref *obj ,ui::Widget::TouchEventType type)
{
    ui::Button *btn = dynamic_cast<ui::Button *>(obj);
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        switch (btn->getTag())
        {
            case 99:
            
                break;
                
            default:
            {
                if (_btnClicked)
                {
                    _btnClicked(btn);
                }
            }
                break;
        }

    }

}

void ESMakeBaseScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    if (Director::getInstance()->getRunningScene()->getChildByTag(129))
    {
        
        Director::getInstance()->getRunningScene()->removeChildByTag(129);
        return;
    }
    
    if (!AdsLoadingScene::s_enableBackClick)
    {
        return;
    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
    {
        DialogLayer::showWithMessage("Are you sure \nyou want to return to \nthe main menu? \nYour current progress \nwill be lost.", [=](){
            
            kAudioUtil->stopAllEffect();
            
            SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
            
            
        }, nullptr, Director::getInstance()->getRunningScene(), 35.f);
    }
    
}

void ESMakeBaseScene::createScrollView(DecorationConfig config,
                                       const std::function<void(ui::Widget * item)>& itemClickCallback,
                                       ScrollViewConfig sConfig,
                                       bool animated)
{
    removeScrollView();
    _selectBgName = sConfig.selectedName;
    _scrollPosOut = sConfig.posOut;
    _scrollNode = Node::create();
    this->addToUILayer(_scrollNode);
    _scrollNode->setPosition(sConfig.pos);

    auto bg = Sprite::create();
    bg->setTexture(sConfig.bgName);
    _scrollNode->addChild(bg);
    
    if (_scrollView)
    {
        _scrollView->removeFromParent();_scrollView=nullptr;
    }
    
    _scrollView = ESScrollView::create();
    _scrollNode->addChild(_scrollView,2);
    
    auto adapter = ESUnitAdapter::create(config.iconFormat, config.begin, config.end);
    _scrollView->setItemAdapter(adapter);
    _scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    _scrollView->setContentSize(sConfig.scrollContentSize);
    _scrollView->setMarginX(sConfig.margin);
    _scrollView->setMarginY(sConfig.scrollContentSize.height);
    _scrollView->loadScrollView();
    //    scroll->setPosition(CMVisibleRect::getPosition(320, 725));
    _scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    _scrollView->loadScrollView();
    
    _iapIdentify = ShopScene::getAllFoodString();
    
    for (auto item : _scrollView->vec_Item)
    {
        item->setScale(0.8f);
        int index = item->getTag();
        auto key = kRewardManager->getItemKey(config.moduleName, config.name, index);
        item->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(_iapIdentify.c_str(), config.name, index, item))
        {
            LockUIManager::getInstance()->registerLock(config.moduleName, config.name, index, item,Vec2(90,20));
        }
    }
    
    adapter->unitTouchEnded = [=](ui::Widget* item)
    {
        bool hasPurchased = kIAPManager->isPackagePurchased(_iapIdentify.c_str());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree(config.name, item->getTag()))
            {
                auto keyObject = item->getUserObject();
                if(!keyObject)
                    return;
                string key = ((__String*)(keyObject))->getCString();
                if(kRewardManager->isLocked(key))
                {
                    //如果能找到reward名字的lock，显示reward，否则显示iap
                    if (item->getChildByName(LOCK_NAME_REWARD))
                    {
                        static bool s_isClicked = false;
                        if (s_isClicked)
                        {
                            return;
                        }
                        s_isClicked = true;
                        this->runAction(Sequence::create(DelayTime::create(0.5f),
                                                         CallFunc::create([=]
                                                                          {
                                                                              s_isClicked = false;
                                                                          }),
                                                         NULL));

                        auto node = MiniShopScene::create();
                        this->addChild(node,9999);
                        node->setPosition(CMVisibleRect::getPosition(0, 0));
                        node->showAllFoodButton();
                        node->_onRewardButtonClicked = [=]
                        {
                            kRewardManager->showRewardAds(key);
                        };
                        node->_onShopButtonClicked = [=]
                        {
                            
                            this->showShop();
                        };

                    }
                    return;
                }
            }
        }
        
        markItemSelect(item);
        _selectWidget = item;
        
        if (itemClickCallback)
            itemClickCallback(item);
        kAudioUtil->playEffect(kSoundChoose);
        
    };
    
    if (animated)
    {
        _scrollNode->setPosition(sConfig.posOut);
        _scrollNode->runAction(MoveTo::create(0.3f, sConfig.pos));
    }
    
    _scrollView->show(nullptr);

}

void ESMakeBaseScene::removeScrollView(bool animated)
{
    if (!_scrollNode)
        return;
    if (animated)
    {
        _scrollNode->runAction(MoveTo::create(0.3f, _scrollPosOut));
    }
    else
    {
        _scrollView = nullptr;
        _scrollNode->removeFromParentAndCleanup(true); _scrollNode = nullptr;
    }
}


void ESMakeBaseScene::markItemSelect(Widget *item)
{
    if(_selectBgName.empty())
        return;
    
    if (!_selectBg){
        _selectBg = Sprite::create(_selectBgName);
        _selectBg->retain();
    }
    
    if (_selectBg->getParent())
        _selectBg->removeFromParent();
    
    _selectBg->setPosition(item->getContentSize()/2);
//    _selectBg->setAnchorPoint(Vec2::ZERO);
    item->addChild(_selectBg, -1);
}


/*test
{
 ESDragUnitAdapter* adapter = ESDragUnitAdapter::create(config.iconFormat.c_str(),config.begin,config.end);
 adapter->_itemClicked=[=](int tag)
 {
 
 log("_itemClicked");
 };
 adapter->_itemDragged=[=](Node* n,Touch* p)
 {
 n->setPosition(p->getLocation());
 log("_itemDragged");
 };
 
 _scroll = ESScrollView::create();
 _scroll->setItemAdapter(adapter);
 _scroll->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
 _scroll->setMarginX(20);
 _scroll->setContentSize(Size(560, 110));
 node->addChild(_scroll,2);
 _scroll->setAnchorPoint(Vec2(0.5, 0.5));
 adapter->setScroll(_scroll);
 
 //    for (int index = config.begin; index<=config.end; index++)
 //    {
 //        auto widget = Widget::create();
 //        auto icon = Sprite::create(StringUtils::format(config.iconFormat.c_str(),index));
 //        widget->setContentSize(icon->getContentSize());
 //        icon->setPosition(widget->getContentSize()/2);
 //        widget->addChild(icon, 2);
 //        widget->setTag(index);
 //        widget->setPositionY(_scroll->getContentSize().height/2);
 ////        _scroll->setItemVec(widget);
 //
 //        auto key = kRewardManager->getItemKey("soda", config.name, index);
 //        widget->setUserObject(__String::create(key));
 //        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), config.name, index, widget))
 //        {
 //            LockUIManager::getInstance()->registerLock("soda", config.name, index, widget);
 //        }
 //
 //    }
 _scroll->loadScrollView();

 }*/

void ESMakeBaseScene::onExit()
{
    StudioLayer::onExit();
    SSCMoreGameManager::getInstance()->delegate = nullptr;
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    
    
}

void ESMakeBaseScene::onEnterTransitionDidFinish()
{
    StudioLayer::onEnterTransitionDidFinish();
    if (!_screenEventName.empty())
    {
        Analytics::getInstance()->sendScreenEvent(_screenEventName);
    }

}

void ESMakeBaseScene::stepFinishEffect(Node* node, std::function<void()> callfuc,Vec2 pos)
{
    node->runAction(Sequence::create(MoveTo::create(1.2f, pos),
                                     EaseBackOut::create(ScaleTo::create(0.4f,1.1)),
                                     EaseBackIn::create(ScaleTo::create(0.4f,1)),
                                     CallFunc::create([=]
                                                      {
                                                          showParticle(node);
                                                      }),
                                     DelayTime::create(1.2f),
                                     CallFunc::create([=]
                                                      {
                                                          if (callfuc)
                                                              callfuc();
                                                      }),
                                     NULL));
    kSoundInstance->playRandomStepFinishVoice();

}

void ESMakeBaseScene::showParticle(Node* node,Vec2 pos)
{

    auto particle = ParticleSystemQuad::create("particle/wirteParticle.plist");
    particle->setPosition(node->getPosition()+pos);
    node->getParent()->addChild(particle,INT_MAX);
    kAudioUtil->playEffect(kSoundShow);
}


void ESMakeBaseScene::initTypeButton()
{

}

void ESMakeBaseScene::setTypeEnable(bool enable)
{

}

void ESMakeBaseScene::initProgressTimer()
{
    if (_progressBg)
    {
        _progressTimer->removeFromParent(); _progressTimer = nullptr;
        _progressBg->removeFromParent(); _progressBg = nullptr;
    }
    
    
    _progressTimer = ProgressTimer::create(Sprite::create("ui/other/plan.png"));
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setMidpoint(Vec2(0.0, 0.0));
    _progressTimer->setBarChangeRate(Vec2(1.0, 0.0));
    _progressTimer->setPercentage(0);
    
    _progressBg = Sprite::create("ui/other/plan_bg.png");
    _progressTimer->setPosition(_progressBg->getContentSize()/2);
    _progressBg->addChild(_progressTimer);
    _progressBg->setPosition(CMVisibleRect::getPosition(320-900, 780));
    this->addToUILayer(_progressBg, 10);
    _progressBg->runAction(EaseBackOut::create(MoveBy::create(0.8f, Vec2(900, 0))));
}

void ESMakeBaseScene::removeProgressTimer()
{
    _progressBg->runAction(EaseBackIn::create(MoveBy::create(0.8f, Vec2(900, 0))));
}

void ESMakeBaseScene::showShop()
{
    ShopScene* layer = ShopScene::create();
    layer->setPosition(CMVisibleRect::getPosition(0, 0));
    this->addChild(layer,9999);
}


