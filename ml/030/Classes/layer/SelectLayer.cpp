        //
//  SelectLayer.cpp
//  MLABS1043_BrownieMaker
//
//  Created by wusonglin1 on 15-1-5.
//
//

#include "SelectLayer.h"
#include "Global.h"
#include "IAPManager.h"
#include "ActionUtils.h"
#include "AudioHelp.h"

Vec2 itemPos[5] ={Vec2(169,295),Vec2(344,350),Vec2(488,250),Vec2(388,125),Vec2(201,158)};
string itemPath[5] = {"chocolate","vanilla","strawberry","greentea","cookie"};
SelectLayer::SelectLayer()
{
    m_bIsFirst = true;
}
SelectLayer::~SelectLayer()
{
    m_vTool.clear();
}
bool SelectLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
      
        initUI();
        
        
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void SelectLayer::initUI()
{
    Sprite* bg = Sprite::create("images/select/choose_bg.jpg");
    kAdapterScreen->setExactPosition(bg, 480, 320);
    this->addChild(bg);

    m_pCar = Sprite::create("images/home/start_car.png");
    kAdapterScreen->setExactPosition(m_pCar, 0 - 500, 20,Vec2(0.5,0),kBorderLeft,kBorderBottom);
    this->addToContentLayer(m_pCar, 5);
    
    auto wheel = Sprite::create("images/home/start_tyre0.png");
    wheel->setPosition(Vec2(230,86));
    wheel->setScale(0.9);
    wheel->setTag(10);
    m_pCar->addChild(wheel);
    
    auto wheel1 = Sprite::create("images/home/start_tyre0.png");
    wheel1->setPosition(Vec2(579,80));
    wheel1->setScale(0.9);
    wheel1->setTag(20);
    m_pCar->addChild(wheel1);
    
    wheel->runAction(RepeatForever::create(RotateBy::create(0.5, 120)));
    wheel1->runAction(RepeatForever::create(RotateBy::create(0.5, 120)));
    
    
    m_pDialog = Sprite::create("images/select/dialog.png");
    kAdapterScreen->setExactPosition(m_pDialog, 480, 320);
    m_pDialog->setAnchorPoint(Vec2(0,0.26));
    m_pDialog->setPosition(m_pDialog->getPosition()-Point(200,50));
    this->addToContentLayer(m_pDialog, 5);
    m_pDialog->setScale(0);
   
    for (int i = 0; i<5;i++) {

        auto item = ToolSprite::create(__String::createWithFormat("images/select/choose_%s.png",itemPath[i].c_str())->getCString());
        item->setDelegate(this);
        item->setIsMove(false);
        item->setPosition(itemPos[i]);
        item->setTag(i);
//        item->setScale(0.7);
        item->setScale(0);
        item->cancelEvent(true);
        m_pDialog->addChild(item);
        m_vTool.pushBack(item);
        

        if(i>2 &&!kIAPManager->isFree(UNLOCK_ALL)){
            auto lock = Sprite::create("images/select/lock_cream@2x.png");
            lock->setPosition(item->getContentSize()/2);
            item->addChild(lock);
        }
        
    }
    btn_yes = ui::Button::create("images/select/btn_yes@2x.png");
    btn_yes->setPosition(Vec2(409,121));
    m_pDialog->addChild(btn_yes,10);
    btn_yes->setTag(100);

    btn_no = ui::Button::create("images/select/btn_no@2x.png");
    btn_no->setPosition(Vec2(213,121));
    m_pDialog->addChild(btn_no,10);
    btn_no->setTag(200);
    
    btn_yes->addTouchEventListener(CC_CALLBACK_2(SelectLayer::touchEvent, this));
    btn_no->addTouchEventListener(CC_CALLBACK_2(SelectLayer::touchEvent, this));
    
    btn_no->setVisible(false);
    btn_yes->setVisible(false);
}
void SelectLayer::onEnter()
{
    BaseLayer::onEnter();
//    AudioHelp::getInstance()->StopBackgroundMusic();
//    AudioHelp::getInstance()->playBackgroundMusic("gameBg.mp3");
    
    if (m_bIsFirst) {
        m_bIsFirst = false;
        carAction();
    }
    if (kIAPManager->isUnlockAll()) {
        
        for (auto item:m_vTool) {
            
            int tag = item->getTag();
            if (tag>2) {
                item->removeAllChildren();
            }
            
            
        }
        
        
    }
   
    
    
    
}

void SelectLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    if (kIAPManager->isShowAds()) {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
    
}
void SelectLayer::onExit()
{
    BaseLayer::onExit();
}
void SelectLayer::carAction()
{
    auto func = CallFunc::create([=](){
    
        dialogAction();
        
    });
    
    ActionUtils::moveBy(m_pCar, Vec2(500,0),0.5);
    
    ActionUtils::delayTimeAction(this,1.5,func);
    
}
void SelectLayer::dialogAction()
{
    m_pCar->getChildByTag(10)->stopAllActions();
    m_pCar->getChildByTag(20)->stopAllActions();
    auto func = CallFunc::create([=](){
        
        itemAction();
        
    });
    ScaleTo* scale = ScaleTo::create(0.5, 1.0);
    EaseSineIn* es = EaseSineIn::create(scale);
    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    m_pDialog->runAction(Sequence::create(es,s1,s2,s3,NULL));
    
    ActionUtils::delayTimeAction(this,1.5,func);
    
}
void SelectLayer::itemAction()
{
    
    for (auto tool:m_vTool) {
        
        ScaleTo* scale = ScaleTo::create(0.5, 0.7);
        RotateBy* rota = RotateBy::create(0.5, 360);
        Spawn* sp = Spawn::create(scale,rota, NULL);
        
        tool->runAction(sp);
    }
    this->schedule(schedule_selector(SelectLayer::checkTouch), 5.0);
    
    auto func = CallFunc::create([=](){
    
        for (auto tool:m_vTool) {
            
            tool->cancelEvent(false);
        }
    
    });
    
    ActionUtils::delayTimeAction(this,1.0,func);
    
    
    
    
}
void SelectLayer::checkTouch(float f)
{
    int n = rand()%5;
    
    ActionUtils::shakeCup(m_vTool.at(n), 0.1, 3);

}
void SelectLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    
    int tag = toolSprite->getTag();
    if (!kIAPManager->isUnlockAll()) {
        
        if (tag>2) {
            SceneChangeManager->enterShopScene();
            return;
        }
        
    }
    
    
    this->unschedule(schedule_selector(SelectLayer::checkTouch));
    toolSprite->setUserData((void*)1);
    toolSprite->cancelEvent(true);
    item_pos = toolSprite->getPosition();
    g_choose_waste = itemPath[toolSprite->getTag()];
    
    AudioHelp::getInstance()->playEffect("choose.mp3");
    
    for (auto tools:m_vTool) {
        
        if ((uintptr_t)tools->getUserData() == 1) {
        }else{
            tools->cancelEvent(true);
        }
        
    }
    
    auto func = CallFunc::create([=](){
    
        itemHide();
    
    });
    ScaleTo* s1 = ScaleTo::create(0.2f,0.78f,0.60f);
    ScaleTo* s2 = ScaleTo::create(0.2f,0.60,0.78f);
    ScaleTo* s3 = ScaleTo::create(0.2f,0.7f,0.7f);

    toolSprite->runAction(Sequence::create(s1,s2,s3,func,NULL));
    
    
}
void SelectLayer::itemHide()
{
    ToolSprite* tool = nullptr;
    
    for (auto tools:m_vTool) {
        
        if ((uintptr_t)tools->getUserData() == 1) {
            
            tool = tools;
            
        }else{
        
            ScaleTo* scale = ScaleTo::create(0.5, 0);
            RotateBy* rota = RotateBy::create(0.5, -360);
            Spawn* sp = Spawn::create(scale,rota, NULL);
            tools->runAction(sp);
        }
        
    }
    
    auto flash = Sprite::create("images/select/flash0.png");
    flash->setPosition(tool->getContentSize()/2);
    flash->setTag(100);
    tool->addChild(flash,-1);
    

    auto func = CallFunc::create([=](){
        
        auto pOvenAnimation = Animation::create();
        pOvenAnimation->addSpriteFrameWithFile("images/select/flash0.png");
        pOvenAnimation->addSpriteFrameWithFile("images/select/flash1.png");
        
        pOvenAnimation->setDelayPerUnit(0.3f);
        pOvenAnimation->setLoops(-1);
        
        auto pOvenAnimat = Animate::create(pOvenAnimation);
        
        flash->runAction(pOvenAnimat);
        
        btn_no->setVisible(true);
        btn_yes->setVisible(true);
        
    });
    
    MoveTo* move = MoveTo::create(0.5, Vec2(312,238));
    ScaleTo* scale = ScaleTo::create(0.5, 0.8);
    Spawn* sp = Spawn::create(move,scale, NULL);
    
    
    tool->runAction(Sequence::create(sp,func,NULL));
}
void SelectLayer::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{

}
void SelectLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("Are you sure you want to start over?");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}

void SelectLayer::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void SelectLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    
    if(kIAPManager->isShowAds())
    {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
    }
    Director::getInstance()->popToRootScene();
    SceneChangeManager->enterHomeScene();
    
}
void  SelectLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
                   SceneChangeManager->enterMarketAction();
                }
                    break;
                case 200:
                {
                    for (auto tools:m_vTool) {
                        
                        if ((uintptr_t)tools->getUserData() == 1) {
                            
                            if (tools->getChildByTag(100)) {
                                tools->getChildByTag(100)->removeFromParent();
                            }
                            tools->setUserData(nullptr);
                            tools->cancelEvent(false);
                            tools->setPosition(item_pos);
                            
                        }
                        tools->cancelEvent(true);
                    }
                    
                    btn_no->setVisible(false);
                    btn_yes->setVisible(false);
                    
                    itemAction();
                }
                    break;
            }
        }
    }

}
