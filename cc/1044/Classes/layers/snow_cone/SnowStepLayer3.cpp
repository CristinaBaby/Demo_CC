//
//  SnowStepLayer3.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/8.
//
//

#include "SnowStepLayer3.h"
#include "SnowStepLayer4.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"



SnowStepLayer3::SnowStepLayer3(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
SnowStepLayer3::~SnowStepLayer3()
{
    
}
SnowStepLayer3* SnowStepLayer3::create(GameLayer* gameLayer)
{
    SnowStepLayer3* step1 = new SnowStepLayer3(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool SnowStepLayer3::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        
//        initData();
        
        CC_BREAK_IF(!initUI());
        
        
        
        isInit = true;
        
    } while (0);
    
    return isInit;
    
    
    
}
void SnowStepLayer3::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<20; i++) {
        
        __String* str = __String::createWithFormat("images/snow_cone/2/cup/icon/cup%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        sprite->setUserData((void*)i);
        
        m_vTypes.pushBack(sprite);
        
    }
    
}
bool SnowStepLayer3::initUI()
{
    Sprite* bg = Sprite::create("images/snow_cone/1/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    Sprite* bg1 = Sprite::create("images/snow_cone/1/bg1.png");
    kAdapterScreen->setExactPosition(bg1, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(bg1);
    
    
    m_pMachine = Sprite::create("images/snow_cone/1/icemachion1.png");
    kAdapterScreen->setExactPosition(m_pMachine,  320, 371 + 300);
    this -> addChild(m_pMachine,2);
    
    Sprite* machineCover  = Sprite::create("images/snow_cone/1/snowcone_cover.png");
    machineCover -> setAnchorPoint(Vec2(0.05, 0.285));
    machineCover->setPosition(Vec2(178,520));
    
    m_pMachine->addChild(machineCover,15);
    
    Sprite* box = Sprite::create("images/snow_cone/1/box1.png");
    box->setPosition(Vec2::ZERO);
    box->setAnchorPoint(Vec2::ZERO);
    m_pMachine->addChild(box,1);
    
    m_pBox = ToolSprite::create("images/snow_cone/1/box2.png");
    m_pBox->setPosition(Vec2::ZERO);
    m_pBox->setAnchorPoint(Vec2::ZERO);
    m_pBox->setTag(100);
    m_pBox->setDelegate(this);
//    m_pBox->cancelEvent(true);
    m_pBox->setIsMove(false);
    m_pMachine->addChild(m_pBox,8);
    
    Sprite* machine = Sprite::create("images/snow_cone/1/icemachion2.png");
    machine->setPosition(Vec2::ZERO);
    machine->setAnchorPoint(Vec2::ZERO);
    m_pMachine->addChild(machine,10);
    
    
    m_pIceMachine = Sprite::create("images/snow_cone/1/ice3.png");
    m_pIceMachine->setPosition(Vec2::ZERO);
    m_pIceMachine->setAnchorPoint(Vec2::ZERO);
    m_pMachine->addChild(m_pIceMachine,5);
    
    Sprite* switchBtn = Sprite::create("images/snow_cone/1/btn_button_h.png");
    switchBtn -> setAnchorPoint(Vec2(0.5, 0.5));
    switchBtn->setPosition(Vec2(222,373));
    m_pMachine->addChild(switchBtn,20);
    
    m_pCup = Sprite::create(__String::createWithFormat("images/snow_cone/2/cup/cup%d.png",G_Snow_CupNum)->getCString());
    kAdapterScreen->setExactPosition(m_pCup, 320, 0 - 1000);
    this->addChild(m_pCup,20);

    m_pArrow = Sprite::create("images/snow_cone/1/arrows_1.png");
    m_pArrow->setPosition(Vec2(221,204));
    m_pMachine->addChild(m_pArrow,20);
    
    Rect rectCup = Rect(m_pCup->getPositionX()-132,m_pCup->getPositionY()+243 - 100 + 1000, 274, 98);
    Rect rectMachine = Rect(m_pMachine->getPositionX()-185,m_pMachine->getPositionY() - 190, 350, 180);
    
    m_pSpoon = ToolSprite::create("images/snow_cone/3/spoon1.png");
    kAdapterScreen->setExactPosition(m_pSpoon, 320 + 200 + 1000, 400,Vec2(0.5,0.5),kBorderNone,kBorderBottom);
    m_pSpoon->setResponsePoint(Vec2(0.32,0.367));
    m_pSpoon->setTag(200);
    m_pSpoon->addRect(rectCup);
    m_pSpoon->addRect(rectMachine);
    m_pSpoon->setMoveRect(Rect::ZERO);
    m_pSpoon->setDelegate(this);
    m_pSpoon->cancelEvent(true);
    this->addChild(m_pSpoon,30);

    m_pIceCup = Sprite::create("images/snow_cone/3/coneice1.png");
    m_pIceCup->setAnchorPoint(Vec2(0.5,0));
    m_pIceCup->setPosition(Vec2(206,404));
    m_pCup->addChild(m_pIceCup);
    m_pIceCup->setVisible(false);
    
    
    return true;
}
void SnowStepLayer3::onEnter()
{
    StepLayer::onEnter();
    m_pCup->setTexture(__String::createWithFormat("images/snow_cone/2/cup/cup%d.png",G_Snow_CupNum)->getCString());
}
void SnowStepLayer3::onExit()
{
    StepLayer::onExit();
}
void SnowStepLayer3::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
    chooseAction();
    ActionUtils::flashAction(m_pArrow, -1);
    
    
    
}
void SnowStepLayer3::spoonAction()
{
    auto func = CallFunc::create([&](){
        
        AudioHelp::getInstance()->playEffect("general_swish_2.mp3");
        ActionUtils::dropOut(m_pSpoon, Vec2(-1000,0),1);
    });
    
    AudioHelp::getInstance()->playEffect("general_swish_2.mp3");
    ActionUtils::dropOut(m_pCup, Vec2(0,1000),1,func);
    
}
void SnowStepLayer3::chooseAction()
{
    auto func = CallFunc::create([&](){
        
//        spoonAction();
      
    });
    ActionUtils::delayTimeAction(this,0.5,func);

}
void SnowStepLayer3::spoonMiss()
{
    auto func = CallFunc::create([&](){
        
       //next
        auto particle = ParticleSystemQuad::create("particle/snowCone.plist");
        particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        particle->setVisible(true);
        particle->setPosition(Vec2(400,0));
        this->addChild(particle,100);
        
        AudioHelp::getInstance()->playEffect("hurrah.mp3");
        
        
//        m_pGameLayer->doNextStep(SnowStepLayer4::create(m_pGameLayer));
        nextStep();
    });
    
    ActionUtils::dropOut(m_pSpoon,Vec2(500,0),1.0,func);

}
void SnowStepLayer3::nextStep()
{
    auto func = CallFunc::create([&](){
        
        //next
        m_pGameLayer->doNextStep(SnowStepLayer4::create(m_pGameLayer));
    });
    ActionUtils::delayTimeAction(this,3.0,func);

}
//滑动杯子点击回调
void SnowStepLayer3::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
}
void SnowStepLayer3::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    if (tag == 100) {
        
    }
}
void SnowStepLayer3::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    
}
void SnowStepLayer3::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    if (tag == 100 && pTouch->getDelta().y <= 5) {
        m_pArrow->stopAllActions();
        m_pArrow->setVisible(false);
        
        toolSprite->cancelEvent(true);
        m_pBox->setTexture("images/snow_cone/1/box3.png");
        m_pSpoon->cancelEvent(false);
        
        
        auto func = CallFunc::create([&](){
            
            spoonAction();
            
        });
        
        
        ActionUtils::delayTimeAction(this,0.5,func);
        
    }

}
void SnowStepLayer3::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    static int step = 0;
    static bool machine = true;
    static bool cup = false;
    int tag = toolSprite->getTag();
    log("---rect %d",index);
    if (index == 1 && tag == 200&& machine) {

        machine = false;
        cup=true;
        toolSprite->setTexture("images/snow_cone/3/spoon2.png");
        toolSprite->setTag(300);
        step++;
        
        
        AudioHelp::getInstance()->playEffect("scoop up the ice.mp3");
        
        
        if (step == 2) {
            m_pIceMachine->setTexture("images/snow_cone/1/ice3.png");
        }
        
        if (step == 4) {
            
            m_pIceMachine->setVisible(false);
        }
        
        
        
    }else if (index == 0 && tag == 300 &&cup){
    
        machine = true;
        cup=false;
        
        AudioHelp::getInstance()->playEffect("cream pour.mp3");
        
        m_pIceCup->setVisible(true);
        
        toolSprite->setTexture("images/snow_cone/3/spoon1.png");
        toolSprite->setTag(200);
        
        if (step!=0) {
            __String* str = __String::createWithFormat("images/snow_cone/3/coneice%d.png",step);
            m_pIceCup->setTexture(str->getCString());
            if(step == 4){
                step = 0;
                toolSprite->cancelEvent(true);
                
                spoonMiss();
            }
            
        }
        
    }
    
    
    

}
void SnowStepLayer3::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void SnowStepLayer3::onNegativeClick(void* type)
{
    isReturn = false;
}
void SnowStepLayer3::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  SnowStepLayer3::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
                    SceneChangeManager->enterGameScene();                }
                    break;
            }
        }
    }
    
}
