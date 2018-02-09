//
//  IceCreamStepLayer3.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/3.
//
//

#include "IceCreamStepLayer3.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"

#include "FillMaterialModel.h"
#include "IceCreamDecorationLayer.h"


IceCreamStepLayer3::IceCreamStepLayer3(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
IceCreamStepLayer3::~IceCreamStepLayer3()
{
    
}
IceCreamStepLayer3* IceCreamStepLayer3::create(GameLayer* gameLayer)
{
    IceCreamStepLayer3* step1 = new IceCreamStepLayer3(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool IceCreamStepLayer3::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        
        initData();
        
        m_rIceMoveRect = Rect(0, 0, 2000, 2000);
        m_iSpoonType = -1;
        m_iIceType = -1;
        m_bNext = true;
        m_bCupShow = true;
        m_pCup1 = nullptr;
        
        
        
        CC_BREAK_IF(!initUI());
        
        
        isInit = true;
        
    } while (0);
    
    return isInit;
    
    
    
}
//初始化冰淇淋数据
void IceCreamStepLayer3::initData()
{
    m_vTypes.clear();
    for (int i = 1; i<6; i++) {

        __String* str = __String::createWithFormat("images/ice_cream/3/icon/spoon_%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
//        __String* strData = __String::create("icespoon");
//        
//        sprite->setUserData((char*)strData->getCString());
        
        m_vTypes.pushBack(sprite);
        
    }
    
    m_vIceTypes.clear();
    for (int i = 1; i<15; i++) {
        __String* str = __String::createWithFormat("images/ice_cream/2/Ice cream_%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        
        __String* strData = __String::create("icecream");
        sprite->setUserData((char*)strData->getCString());
        
        m_vIceTypes.pushBack(sprite);
        
    }
    
    
}
bool IceCreamStepLayer3::initUI()
{
    Sprite* bg = Sprite::create("images/ice_cream/2/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);

    Sprite* bg1 = Sprite::create("images/ice_cream/2/bg_1.png");
    kAdapterScreen->setExactPosition(bg1, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(bg1);
    
    m_pGridViewBg = Sprite::create("images/ice_cream/3/popup_bg.png");
    kAdapterScreen->setExactPosition(m_pGridViewBg, 600, 900);
    m_pGridViewBg->setScale(0);
    this->addChild(m_pGridViewBg,20);
    
    m_pLayerColor = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(m_pLayerColor,19);
    m_pLayerColor->setVisible(false);
    
    m_pGirdView = KDGridView::create(Size(505, 250));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    
    m_pGirdView->m_fHSpace = 30;
    //    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 5;
    m_pGirdView->m_fMarginRight = 30;
    m_pGirdView->m_fMarginLeft = 30;
    m_pGirdView->setPosition(Vec2(298,170));
    m_pGridViewBg->addChild(m_pGirdView,5);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(5);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(IceCreamStepLayer3::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;
    
    
    
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-213*5, 0));
    
    m_pIceGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width + 20, 700),1);
    m_pIceGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pIceGirdView->m_iRow = 1;
    m_pIceGirdView->m_fHSpace = 10;
    //    m_pGirdView->m_fVSpace = 20;
    m_pIceGirdView->m_fMarginTop = 5;
    m_pIceGirdView->m_fMarginRight = 100;
    m_pIceGirdView->m_fMarginLeft = 100;
    
    
    m_pIceGirdView->setEnable(false);
    
    kAdapterScreen->setExactPosition(m_pIceGirdView, 0, 0-215,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pIceGirdView,4);
    
    
    
    m_pIceAdapter = KSSimpleAdapter::create(m_vIceTypes);
    m_pIceAdapter->setIapType(4);
    m_pIceGirdView->setAdapter(m_pIceAdapter);
    m_pIceGirdView->setItemClickCall(CC_CALLBACK_3(IceCreamStepLayer3::typeIceItemClick, this));
    m_pIceGirdView->m_pScrollView->setBounceable(false);
    m_pIceGirdView->setScrollCallback(this);
    m_pIceGirdView->m_bNeedAdjust = true;
    
    m_pIceGirdView->m_pScrollView->setContentOffset(Vec2(-5200, 0));
//    m_pGirdView->m_pScrollView->setContentOffset(Vec2(20, 0));
    
    m_pFreezerDoor = Sprite::create("images/ice_cream/2/Freezer door_7.png");
    kAdapterScreen->setExactPosition(m_pFreezerDoor, 0, 160,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pFreezerDoor,5);

 
   
    m_pGarbage = Sprite::create("images/ice_cream/3/trash0.png");
    kAdapterScreen->setExactPosition(m_pGarbage, 10 - 500, 480 + 100,Vec2(0,0),kBorderLeft,kBorderNone);
    this->addChild(m_pGarbage,5);

    
    m_pSpoon = ToolSprite::create("images/ice_cream/3/icon/spoon_1.png");
    m_pSpoon->setDelegate(this);
    m_pSpoon->cancelEvent(true);
    
    m_pSpoon->setTag(KSpoonNull);//100为空状态，200为有冰淇淋状态
    kAdapterScreen->setExactPosition(m_pSpoon, 320, 480 ,Vec2(0,0),kBorderNone,kBorderNone);
    this->addChild(m_pSpoon,15);
    _positionSpoon = m_pSpoon->getPosition();
    m_pSpoon->setResponsePoint(Vec2(0.294,0.381));
    m_pSpoon->setVisible(false);

    m_pBack = ToolSprite::create("images/ice_cream/3/popup_btn_back.png");
    m_pBack->setDelegate(this);
    m_pBack->setTag(KBack);
    m_pBack->setAnchorPoint(Vec2(0.5,0.5));
    m_pBack->setIsMove(false);
    m_pBack->setPosition(Vec2(25,175));
    m_pGridViewBg->addChild(m_pBack);
    
    m_pNext = ToolSprite::create("images/ice_cream/3/popup_btn_next.png");
    m_pNext->setDelegate(this);
    m_pNext->setTag(KNext);
    m_pNext->setAnchorPoint(Vec2(0.5,0.5));
    m_pNext->setIsMove(false);
    m_pNext->setPosition(Vec2(571,175));
    m_pGridViewBg->addChild(m_pNext);
    
    m_pIcon = ToolSprite::create("images/ice_cream/shape_icon.png");
    m_pIcon->setDelegate(this);
    m_pIcon->setTag(KICON);
    m_pIcon->setIsMove(false);
    m_pIcon->cancelEvent(true);
    kAdapterScreen->setExactPosition(m_pIcon, 10, 20,Vec2(1,1),kBorderRight,kBorderTop);
    this->addChild(m_pIcon,10);

    for(int i = 0;i < m_vIceTypes.size();i++){
    
        m_pSpoon->addRectByNode(m_vIceTypes.at(i));
        
    }
    
    
    if (G_IceCream_CupNum != -1) {
        G_IceCream_CupSprite = Sprite::create(__String::createWithFormat("images/ice_cream/1/cup_%d_1.png",G_IceCream_CupNum)->getCString());
        kAdapterScreen->setExactPosition(G_IceCream_CupSprite, 320, 480 + 200 + 100 - 100);
        this->addChild(G_IceCream_CupSprite,8);
        G_IceCream_CupSprite->setScale(0.7);
        
        m_pCup1 = Sprite::create(__String::createWithFormat("images/ice_cream/1/cup_%d.png",G_IceCream_CupNum)->getCString());
        kAdapterScreen->setExactPosition(m_pCup1, 320, 480 + 200 + 100 - 100);
        m_pCup1->setTag(102);
        this->addChild(m_pCup1,12);
        m_pCup1->setScale(0.7);
        
        m_rIceCupRect = Rect(G_IceCream_CupSprite->getPositionX()-141, G_IceCream_CupSprite->getPositionY() + 50, 321, 54);
        m_rIceMoveRect = Rect(G_IceCream_CupSprite->getPositionX()-146, G_IceCream_CupSprite->getPositionY()  - 116 , 321, 54 + 400);
        
        G_IceCream_CupSprite->setPositionY(G_IceCream_CupSprite->getPositionY() + 1000);
        m_pCup1->setPositionY(m_pCup1->getPositionY() + 1000);
        
        
        G_IceCream_CupSprite->retain();
        
        m_pSpoon->clearRectVector();
        for(int i = 0;i < m_vIceTypes.size();i++){
            
            m_pSpoon->addRectByNode(m_vIceTypes.at(i));
            
        }
        
        m_pSpoon->addRect(m_rIceCupRect);
        m_pSpoon->addRectByNode(m_pGarbage);
    }

    
    startAction();
    

    

    return true;
}
void IceCreamStepLayer3::onEnter()
{
    StepLayer::onEnter();
    
    m_pIceGirdView->refresh();
    m_pGirdView->refresh();
    
    m_pGirdView->setEnable(true);
    m_pIceGirdView->setEnable(true);
   
    m_pSpoon->clearRectVector();
    for(int i = 0;i < m_vIceTypes.size();i++){
        
        m_pSpoon->addRectByNode(m_vIceTypes.at(i));
        
    }
    
    m_pSpoon->addRect(m_rIceCupRect);
    m_pSpoon->addRectByNode(m_pGarbage);
    
}
void IceCreamStepLayer3::onExit()
{
    StepLayer::onExit();
   
}
void IceCreamStepLayer3::onScrollEnd(float offsetX,float offsetY)
{
    log("onScrollEnd");
    m_pSpoon->clearRectVector();
    for(int i = 0;i < m_vIceTypes.size();i++){
        
        m_pSpoon->addRectByNode(m_vIceTypes.at(i));
        
    }

    m_pSpoon->addRect(m_rIceCupRect);
    m_pSpoon->addRectByNode(m_pGarbage);
}
void IceCreamStepLayer3::onScrolling(float offsetX,float offsetY)
{
    log("onScrolling");
//    m_pSpoon->clearRectVector();

}
void IceCreamStepLayer3::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
     m_pIceGirdView->setEnable(true);
   
}
void IceCreamStepLayer3::startAction()
{
    
    
    m_pIceGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0, 0), 2);
    auto func = CallFunc::create([&](){
        
        m_pIceGirdView->setScrollEnable(true);
        m_pIceGirdView->setEnable(true);
        m_pLayerColor->setVisible(true);
        girdViewShowAction();
        m_pIcon->cancelEvent(true);
        
        m_pSpoon->clearRectVector();
        for(int i = 0;i < m_vIceTypes.size();i++){
            
            m_pSpoon->addRectByNode(m_vIceTypes.at(i));
            
        }
        
        m_pSpoon->addRect(m_rIceCupRect);
         m_pSpoon->addRectByNode(m_pGarbage);

        
    });
    ActionUtils::delayTimeAction(this,2,func);


}
void IceCreamStepLayer3::girdViewShowAction()
{
     m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(20, 0), 1);
    
    ScaleTo* s1 = ScaleTo::create(0.05f,1.08f,0.70f);
    ScaleTo* s2 = ScaleTo::create(0.05f,0.70,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.05f,1.0f,1.0f);
    
    MoveTo* move = MoveTo::create(0.5, kAdapterScreen->getExactPostion(Vec2(320,480)));
    
    ScaleTo* scale = ScaleTo::create(0.5, 1);
    
    Spawn* spawn = Spawn::create(move,scale, NULL);
    
    EaseSineIn* ease = EaseSineIn::create(spawn);
    
    m_pGridViewBg->runAction(Sequence::create(ease,s1,s2,s3,NULL));
    
//    m_pGridViewBg->runAction(move);
}
void IceCreamStepLayer3::girdViewHideAction()
{
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-213*5, 0));
    
    auto func = CallFunc::create([&](){
        
        m_pLayerColor->setVisible(false);
        m_pIcon->cancelEvent(false);
        
    });
    
    MoveTo* move = MoveTo::create(0.5, kAdapterScreen->getExactPostion(Vec2(580,950)));
//    EaseSineOut* ease = EaseSineOut::create(move);
    
    ScaleTo* scale = ScaleTo::create(0.5, 0);
    
    Spawn* spawn = Spawn::create(move,scale, NULL);
    
    EaseSineOut* ease = EaseSineOut::create(spawn);
    
    m_pGridViewBg->runAction(Sequence::create(ease,func,NULL));

}
//滑动杯子点击回调
void IceCreamStepLayer3::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    
    m_pGirdView->setEnable(false);
    m_pIceGirdView->setEnable(false);
    if (kIAPManager->isFree("icespoon", index)) {
        
        m_pCurrentSprite = nullptr;
        
//        m_pGirdView->setScrollEnable(false);
        
        int n = (uintptr_t)item->getUserData();
        CCLOG("%d",index);
        
        auto func = CallFunc::create([&](){
            
//            m_pGirdView->setScrollEnable(true);
            m_pGirdView->setEnable(true);
             m_pIceGirdView->setEnable(true);
            girdViewHideAction();
            
        });
        
        
        ActionUtils::shakeCup(item, 0.5, 3,func);
        
        m_iSpoonType = index + 1;
        
        m_pSpoon->setVisible(true);
        m_pSpoon->cancelEvent(false);
        
        
        
        m_pSpoon->setTexture(__String::createWithFormat("images/ice_cream/3/spoon/spoon_%d.png",m_iSpoonType)->getCString());
        
        m_pSpoon->setPositionX(m_pSpoon->getPositionX() + 1000);
        
        auto func1 = CallFunc::create([&](){
            
            m_pSpoon->clearRectVector();
            for(int i = 0;i < m_vIceTypes.size();i++){
                
                m_pSpoon->addRectByNode(m_vIceTypes.at(i));
             }
            
            m_pSpoon->addRect(m_rIceCupRect);
            m_pSpoon->addRectByNode(m_pGarbage);
            
        });
        
        ActionUtils::dropOut(m_pSpoon, Vec2(-1000,0),3.0,func1);
        
    }else{
        
        SceneManager::getInstance()->enterShopScene();
    }
    
    
}
void IceCreamStepLayer3::typeIceItemClick(int tag,MenuItem* menuItem,int index)
{
    m_pGirdView->setEnable(false);
    m_pIceGirdView->setEnable(false);
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if (kIAPManager->isFree("icecream", index)) {
        m_pIceGirdView->setEnable(true);
        m_pGirdView->setEnable(true);
    }else{
        
        SceneManager::getInstance()->enterShopScene();
        
    }
    

}
void IceCreamStepLayer3::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    if(tag == KBack){
        
        
        toolSprite->setTexture("images/ice_cream/3/popup_btn_back_h.png");
        m_pGirdView->movePreStep();

    }else if(tag == KNext){
        
        toolSprite->setTexture("images/ice_cream/3/popup_btn_next_h.png");
        m_pGirdView->moveNextStep();
        
    }else if(tag == KICON){
        m_pIcon->cancelEvent(true);
        m_pLayerColor->setVisible(true);
        m_pSpoon->setVisible(false);
        girdViewShowAction();
    }
}
void IceCreamStepLayer3::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    if(tag == KBack){
        
        toolSprite->setTexture("images/ice_cream/3/popup_btn_back.png");
        
    }else if(tag == KNext){
        
        toolSprite->setTexture("images/ice_cream/3/popup_btn_next.png");
        
    }else if(tag == KICON){
        
    }
}
void IceCreamStepLayer3::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    
}
void IceCreamStepLayer3::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    log("----%d",index);
    
    int tag = toolSprite->getTag();
    if (tag == KSpoonNull && index!=14&& index!=15 ) {
        
        if (kIAPManager->isFree("icecream", index)) {
            
            toolSprite->setTexture(__String::
                                   createWithFormat("images/ice_cream/3/spoon/spoon_%d_%d.png",m_iSpoonType,index + 1)->getCString());
            
            toolSprite->setTag(KSpoonEd);
            
            m_iIceType = index + 1;
            
            if (m_bCupShow) {
                m_bCupShow = false;
                
                AudioHelp::getInstance()->playEffect("general_swish_2.mp3");
                ActionUtils::dropOut(G_IceCream_CupSprite, Vec2(0,-1000),2.0);
                ActionUtils::moveBy(m_pGarbage, Vec2(500,0));
//                ActionUtils::dropOut(m_pCup1, Vec2(0,-1000),2.0);
                if (m_pCup1) {
                    ActionUtils::dropOut(m_pCup1, Vec2(0,-1000),2.0);
                }
            }
            
        }
        
        
        
        
    
    }else if(tag == KSpoonEd && index == 14 && !m_bCupShow){
        
        m_pSpoon->cancelEvent(true);
        m_pSpoon->setTexture(__String::createWithFormat("images/ice_cream/3/spoon/spoon_%d.png",m_iSpoonType)->getCString());
        toolSprite->setTag(KSpoonNull);
        
        auto ice = FillMaterialModel::create(__String::createWithFormat("images/ice_cream/3/Ice_cream/Ice cream_%d_%d.png",m_iSpoonType,m_iIceType)->getCString());
        log("%s",__String::createWithFormat("images/ice_cream/3/Ice_cream/Ice cream_%d_%d.png",m_iSpoonType,m_iIceType)->getCString());
        ice->setPosition(Vec2(200,200 + 100));
        ice->setAnchorPoint(Vec2(0.5,0.5));
        ice->setMoveRect(m_rIceMoveRect);
        G_IceCream_CupSprite->addChild(ice);
        
        auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
        particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        particle->setVisible(true);
        particle->setPosition(ice->getContentSize()*.5);
        ice->addChild(particle,4);
        
        AudioHelp::getInstance()->playEffect("all star .mp3");
        auto func = CallFunc::create([&](){
            
            m_pSpoon->cancelEvent(false);
            
        });
        
        toolSprite->runAction(Sequence::create(MoveTo::create(0.5, _positionSpoon),func,NULL));
        
        if (m_bNext) {
            m_bNext=false;
            m_pGameLayer->m_bNextAction = false;
            m_pGameLayer->OperateEndToDecoration(IceCreamDecorationLayer::create(m_pGameLayer));
        }
       
    }else if(tag == KSpoonEd && index == 15 && !m_bCupShow){
        
        m_pSpoon->cancelEvent(true);
        m_pSpoon->setTexture(__String::createWithFormat("images/ice_cream/3/spoon/spoon_%d.png",m_iSpoonType)->getCString());
        m_pGarbage->setTexture("images/ice_cream/3/trash1.png");
        toolSprite->setTag(KSpoonNull);
        
        auto sprite = Sprite::create(__String::createWithFormat("images/ice_cream/3/Ice_cream/Ice cream_%d_%d.png",m_iSpoonType,m_iIceType)->getCString());
        sprite->setPosition(m_pGarbage->getPosition());
        sprite->setAnchorPoint(Vec2(0,0));
        this->addChild(sprite);
        
        auto func = CallFunc::create([&](){
            
            m_pSpoon->cancelEvent(false);
            m_pGarbage->setTexture("images/ice_cream/3/trash0.png");
            
        });
        
        JumpBy* jump = JumpBy::create(0.5, Vec2(0,20), 200, 1);
        ScaleTo* scale = ScaleTo::create(0.5,0);
        MoveBy* move = MoveBy::create(0.5, Vec2(-5,0));
        Spawn* sp = Spawn::create(jump,scale,move, NULL);
        
        sprite->runAction(Sequence::create(sp,func, NULL));
        
        
    }
    
    
}


void IceCreamStepLayer3::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void IceCreamStepLayer3::onNegativeClick(void* type)
{
    isReturn = false;
}
void IceCreamStepLayer3::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}

void  IceCreamStepLayer3::touchEvent(Ref *obj , Widget::TouchEventType type)
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
