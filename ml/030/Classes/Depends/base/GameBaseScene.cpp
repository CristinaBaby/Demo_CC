//
//  GameBaseScene.cpp
//  MLABS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/29.
//
//

#include "GameBaseScene.h"

GameBaseScene::GameBaseScene():m_bIsShowReturn(false),m_bNextAction(true)
{
    
}

GameBaseScene::~GameBaseScene()
{
    _eventDispatcher->removeEventListener(m_pKeyboardListener);

}
bool GameBaseScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        m_pKeyboardListener = EventListenerKeyboard::create();
        m_pKeyboardListener->onKeyReleased = CC_CALLBACK_2(GameBaseScene::onKeyReleasedCallBack, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pKeyboardListener, this);
        
        initUI();
        
        
        
        
        bRet = true;
    } while (0);
    return bRet;
}
void GameBaseScene::initUI()
{
    m_pBackGroundLayer = Layer::create();
    m_pContentLayer = Layer::create();
    m_pUILayer = Layer::create();
    this->addChild(m_pBackGroundLayer, 0);
    this->addChild(m_pContentLayer, 5);
    this->addChild(m_pUILayer, 10);

    m_pNextButton = ui::Button::create("images/button/btn_next@2x.png");
    m_pNextButton->addTouchEventListener(CC_CALLBACK_2(GameBaseScene::touchEvent, this));
    m_pNextButton->setTag(100);
    m_pNextButton->setVisible(false);
    kAdapterScreen->setExactPosition(m_pNextButton, 10 + m_pNextButton->getContentSize().width/2, 320,Vec2(0.5, 0.5),kBorderRight,kBorderNone);
    m_pUILayer->addChild(m_pNextButton,100);
    
    
}
void GameBaseScene::onEnter()
{
    Layer::onEnter();
//    showListener = _eventDispatcher ->addCustomEventListener(kAdNotifyInterstitialShown, CC_CALLBACK_1(GameBaseScene::showAdsLaod, this));
//    closeListener = _eventDispatcher->addCustomEventListener(kAdNotifyInterstitialDismissed, CC_CALLBACK_1(GameBaseScene::closeAdsLaod, this));
}

void GameBaseScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void GameBaseScene::onExit()
{
    Layer::onExit();
//    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(kAdNotifyInterstitialShown);
//    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(kAdNotifyInterstitialDismissed);
//    _eventDispatcher->removeEventListener(showListener);
//    _eventDispatcher->removeEventListener(closeListener);
}
void GameBaseScene::hideNextButton()
{
    m_bNextAction=false;
    m_pNextButton->setTouchEnabled(false);
    m_pNextButton->setVisible(false);
}
//参数决定是否跳动
void GameBaseScene::showNextButton(bool isAction)
{
    m_bNextAction = isAction;
    m_pNextButton->setTouchEnabled(true);
    m_pNextButton->setVisible(true);
    //按钮动画
    if (m_bNextAction) {
        
        m_pNextButton->stopAllActions();
        ScaleTo* scale = ScaleTo::create(0.5, 1.1);
        ScaleTo* scale1 = ScaleTo::create(0.5, 1.0);
        m_pNextButton->runAction(RepeatForever::create(Sequence::create(scale,scale1, NULL)));
        
    }
}
void GameBaseScene::addToBGLayer(Node* node,int zorder)
{
    m_pBackGroundLayer->addChild(node, zorder);
}
void GameBaseScene::addToContentLayer(Node* node,int zorder)
{
    m_pContentLayer->addChild(node,zorder);
}
void GameBaseScene::addToUILayer(Node* node,int zorder)
{
    m_pUILayer->addChild(node, zorder);
}
void GameBaseScene::touchEvent(Ref *obj , cocos2d::ui::Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
                    //下一个步骤，在子类实现
                    nextStep();
                }
                    break;
            }
        }
    }
}
//展示全屏广告的回调
void GameBaseScene::showAdsLaod(EventCustom*)
{
    Director::getInstance()->stopAnimation();

}
//关闭全屏广告的回调
void GameBaseScene::closeAdsLaod(EventCustom*)
{
    Director::getInstance()->startAnimation();

}
void GameBaseScene::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if (!m_bIsShowReturn) {
            m_bIsShowReturn = true;
            onKeyBackDown();
            
        }
        
    }
    
}
void GameBaseScene::onKeyBackDown()
{
    
    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
    //                    dialog -> setAnchorPoint(Vec2(0,0));
    //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
    dialog -> setAnchorPoint(Vec2(0,0));
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    dialog->setContentText(ReturnHome);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    m_pUILayer->addChild(dialog, 9999);
    
}
void GameBaseScene::onNegativeClick(void* type)
{
     m_bIsShowReturn = false;
}
void GameBaseScene::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    //回到HomeScene
    //广告
    if(kIAPManager->isShowAds())
    {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
    }
    Director::getInstance()->popToRootScene();
    SceneChangeManager->enterHomeScene();
}


