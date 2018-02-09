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
#include "STAds.h"
#include "ActionUtils.h"
#include "AudioHelp.h"

SelectLayer::SelectLayer()
{
    
}
SelectLayer::~SelectLayer()
{
    
}
bool SelectLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initData();
        initUI();
        
        m_pCurrentSprite = nullptr;
        
        m_pIndex = -1;
        
        m_iTouchIndex = -1;
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void SelectLayer::initData()
{
    
    m_vTypes.clear();
    for (int i = 0; i<3; i++) {
        
        __String* str = __String::createWithFormat("images/select/%s.png",CHOOSE_ITEM_PATH[i].c_str());
        Sprite* data = Sprite::create(str->getCString());
        if(i == 1 && UserDefault::getInstance()->getIntegerForKey(IceCream, 0) != 2){
            Color3B color = data->getColor();
            data->setColor(Color3B(color.r/2,color.g/2,color.b/2));
        }
        
        if( i == 2 && UserDefault::getInstance()->getIntegerForKey(IcePop, 0) != 2){
            
            Color3B color = data->getColor();
            data->setColor(Color3B(color.r/2,color.g/2,color.b/2));
        
        }
        
        m_vTypes.pushBack(data);
        
        
    }
    

}
void SelectLayer::initUI()
{
    Sprite* bg = Sprite::create("images/select/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);

//    Sprite* table = Sprite::create("images/background/choose_table.png");
//    kAdapterScreen->setExactPosition(table, 480, 320);
//    this->addChild(table);
    
    
//    m_pTitle = Sprite::create("images/button/choosefood.png");
//    kAdapterScreen->setExactPosition(m_pTitle, 480, 200 - 400,Vec2(0.5, 1),kBorderNone,kBorderTop);
//    this->addChild(m_pTitle,5);
    
    
    
    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width, 700));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fHSpace = 20;
//    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 5;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;
    
//    m_pGirdView->setScrollEnable(false);
    m_pGirdView->setEnable(false);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 100 - 40,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pGirdView,7);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setType(1);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SelectLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;
    
    nextBtn = ui::Button::create("images/button/btn_next.png");
    nextBtn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::touchEvent, this));
    nextBtn->setTag(100);
    nextBtn->setVisible(false);
    kAdapterScreen->setExactPosition(nextBtn, 10, 10 + 100 ,Vec2(1, 0),kBorderRight,kBorderBottom);
    this->addToUILayer(nextBtn, 20);

    
    chooseParticle = ParticleSystemQuad::create("particle/knifeParticl.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->retain();
    
    
    
}
void SelectLayer::onEnter()
{
    BaseLayer::onEnter();
    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("startbg.mp3");
    
    ActionUtils::jumpAction(nextBtn, 2);
   
    
    
}
void SelectLayer::iceCreamAction()
{
    auto sprite = dynamic_cast<Sprite*>(m_vTypes.at(1));
    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/flower.plist");
    emitter1->setDuration(1.5f);
    //    emitter1->setPosition(sprite->getParent()->convertToWorldSpace(sprite->getPosition()));
    emitter1->setAutoRemoveOnFinish(true);
    sprite->addChild(emitter1, 10);
    
    auto func = CallFunc::create([&](){
        
        changeColor(1);
    });
    
    sprite->runAction(Sequence::create(JumpBy::create(1.5, Vec2::ZERO, 30, 3),NULL));
    ActionUtils::delayTimeAction(this,1.5,func);
}
void SelectLayer::icePopAction()
{
    auto sprite = dynamic_cast<Sprite*>(m_vTypes.at(2));
   
    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/flower.plist");
    emitter1->setDuration(1.5f);
//    emitter1->setPosition(sprite->getParent()->convertToWorldSpace(sprite->getPosition()));
    emitter1->setAutoRemoveOnFinish(true);
    sprite->addChild(emitter1, 10);
    
    auto func = CallFunc::create([&](){
        
        changeColor(2);
    });
    
    sprite->runAction(Sequence::create(JumpBy::create(1.5, Vec2::ZERO, 30, 3),NULL));
    ActionUtils::delayTimeAction(this,1.5,func);
    
}
 void SelectLayer::changeColor(int i)
{
    auto sprite = dynamic_cast<Sprite*>(m_vTypes.at(i));
    Color3B color = sprite->getColor();
    sprite->setColor(Color3B(color.r*2,color.g*2,color.b*2));


}
void SelectLayer::showAdsLaod(Ref*)
{
    log("全屏加载");
//    ShowIcon::hide();
    Director::getInstance()->stopAnimation();

}
void SelectLayer::closeAdsLaod(Ref*)
{
    Director::getInstance()->startAnimation();

   
}
void SelectLayer::checkGame()
{
    //检测是否需要解锁其他游戏
    if (UserDefault::getInstance()->getIntegerForKey(IceCream, 0) == 1) {
        //解锁ice的动画
        
        UserDefault::getInstance()->setIntegerForKey(IceCream, 2);
        UserDefault::getInstance()->flush();
        auto func = CallFunc::create([&](){
            
            iceCreamAction();
        });
        
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-400,0),0.5);
        ActionUtils::delayTimeAction(this,0.5,func);
        
    }
    if (UserDefault::getInstance()->getIntegerForKey(IcePop, 0) == 1) {
        //解锁ice的动画
        
        UserDefault::getInstance()->setIntegerForKey(IcePop, 2);
        UserDefault::getInstance()->flush();
        
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-900,0),0.5);
        
        auto func = CallFunc::create([&](){
            
            icePopAction();
        });
        
        ActionUtils::delayTimeAction(this,0.5,func);
    }

}
void SelectLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
   
    chooseAction();
}
void SelectLayer::onExit()
{
    BaseLayer::onExit();
}
void SelectLayer::titleAction()
{
//     ActionUtils::dropOut(m_pTitle, Vec2(0,-400),2.0);
    
    
    auto func = CallFunc::create([&](){
        
        
//        m_pGirdView->setScrollEnable(true);
        m_pGirdView->setEnable(true);
        checkGame();
        
    });
    
    ActionUtils::delayTimeAction(this,2.0,func);

}
void SelectLayer::chooseAction()
{
   m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-800, 0), 1);
   
    
 
    auto func = CallFunc::create([&](){
   
        titleAction();
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(50, 0), 1);
    });
    this->runAction(Sequence::create(DelayTime::create(2.0),func, NULL));
}
void SelectLayer::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    
    if(index == 1 && UserDefault::getInstance()->getIntegerForKey(IceCream, 0) == 0){
    
        return;
    }
    if(index == 2 && UserDefault::getInstance()->getIntegerForKey(IcePop, 0) == 0){
        
        return;
    }
    
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    int y = item->getPositionY();
    
    
    if (m_iTouchIndex == index) {
        return;
    }else if(m_pCurrentSprite){
        
        m_pCurrentSprite->stopAllActions();
        m_pCurrentSprite->setPositionY(y);
    }
    
    m_pCurrentSprite = item;
    m_pIndex = index;
        
        //选择的游戏种类
    CHOOSE_TYPE = index;
    
    m_iTouchIndex = index;
    
    auto func = CallFunc::create([&](){
            
       
            
    });
    
    chooseParticle->removeFromParent();
    chooseParticle->setVisible(true);
    chooseParticle->setPosition(item->getContentSize()*.5);
    chooseParticle->setScale(1.5);
    item->addChild(chooseParticle,4);
    
    AudioHelp::getInstance()->playEffect("choose.mp3");
    
    nextBtn->setVisible(true);
    
    ActionUtils::jumpAction(item, 2,func,0.4);
    if(CHOOSE_TYPE == 0)
    {
            m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(50,0),0.3);
            
    }else if (CHOOSE_TYPE == 1)
    {
            m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-400,0),0.3);
            
    }else if (CHOOSE_TYPE == 2)
    {
            m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-900,0),0.3);
    }
   
   
    
}

void SelectLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!isReturn)
        {
            isReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("Are you sure you want to start over with a new Italian Food?");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}

void SelectLayer::onNegativeClick(void* type)
{
    isReturn = false;
}
void SelectLayer::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
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
                    SceneChangeManager->enterGameScene();
                }
                    break;
            }
        }
    }

}
