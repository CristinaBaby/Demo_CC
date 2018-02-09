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

ESMakeBaseScene::ESMakeBaseScene():
_nextButton(nullptr),
isClicked(false)
{


}


bool ESMakeBaseScene::init()
{
    if (!StudioLayer::init())
    {
        return false;
    }
    
    _nextButton = Button::create("ui/tap/next_btn.png");
    _nextButton->setScale(0.0);
    _nextButton->setPosition(Vec2(getContentSize().width - _nextButton->getContentSize().width/2 - 20.0, getContentSize().height/2));
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
    auto size = moreGameButton->getButtonSize();
    moreGameButton->setPosition(CMVisibleRect::getPosition(110+10+size.width/2,size.height/2,kBorderLeft,kBorderTop));
    moreGameButton->setClickCall([=](SSCMoreGameButton*){
        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    });
    addToUILayer(moreGameButton, 20);

    
    this->allStepOver=[=]
    {
        this->showNextButton();
    };
    
    _homeButton = Button::create("ui/tap/home_btn.png");
    _homeButton->setPosition(CMVisibleRect::getPosition(20, 20,kBorderLeft,kBorderTop));
    _homeButton->setAnchorPoint(Vec2(0, 1));
    _homeButton->setVisible(true);
    _homeButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            if (!isClicked)
            {
                kAudioUtil->stopAllEffect();

                AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
                AdsLoadingScene::loadingDoneCallback = [=]
                {
                    SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
                };
                
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
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(ESMakeBaseScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
#endif

    
    return true;
}


void ESMakeBaseScene::showNextButton()
{
    if (_nextButton->getScale() > 0.1)
        return;
    _nextButton->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 1.2), ScaleTo::create(0.3, 1.0), NULL)));
    _nextButton->runAction(Sequence::create(EaseExponentialInOut::create(ScaleTo::create(0.5, 1.0)), NULL));

}

void ESMakeBaseScene::hideNextButton()
{
    _nextButton->getActionManager()->removeAllActionsFromTarget(_nextButton);
    _nextButton->setScale(0.0);

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
        DialogLayer::showWithMessage("Are you sure you want to \nreturn to \nthe main menu? \nYour current progress \nwill be lost.", [=](){
            
            kAudioUtil->stopAllEffect();
            
            AdsLoadingScene::showLoading<AdsLoadingScene>(true);
            AdsLoadingScene::loadingDoneCallback=[]
            {
                SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
            };
            
            
        }, nullptr, Director::getInstance()->getRunningScene(), 35);
    }
    
}

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
