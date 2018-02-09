//
//  MakeStepDecDec.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/13.
//
//

#include "MakeStepDec.h"
#include "MakeStepEat.h"
#include "SSCFileUtility.h"
#include "ShopScene.hpp"
REGIST_SECENE(MakeStepDec)


#define TAGDecIcing    1001
#define TAGDecSprinkle 1002
#define TAGDecPlate    1003
#define TAGDecFruit    1004

MakeStepDec::MakeStepDec()
{
    _studioLayer = nullptr;
    _decType = 0;
    m_pDecSelect = nullptr;
    m_bIsTouch = false;
    _icing = nullptr;
    m_iIndex = -1;
    _cliping = nullptr;
    _clipingSprinkle = nullptr;
    _icingSprinkle = nullptr;
    m_pHand = nullptr;
    m_pTitle = nullptr;
    m_pParticleSystemQuad = nullptr;
    m_bIsShowHand = false;
    m_bIsShowHand1 = false;
    m_bIsShowHand2 = false;
}
MakeStepDec::~MakeStepDec()
{
    
}
bool MakeStepDec::init()
{
    if(!BaseLayer::init())
        return false;
    
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeStepDec.csb");
    addChild(_studioLayer,10);
    
    //加载碗，盘子一类的工具
    m_pDecBtn = _studioLayer->getNodeByName<Sprite*>("dec_icon");
    m_pDecBg  = _studioLayer->getNodeByName<Sprite*>("dec_bg");;
    m_pDecBtn->setLocalZOrder(20);
    m_pDecBg->setLocalZOrder(10);
    m_pBowl   = _studioLayer->getNodeByName<Sprite*>("bowl");
    m_pBowl->setLocalZOrder(10);
    m_pBowl->setTag(-1);
    __String* foodPath ;
    
    if (GameDataInstance->getGameType() == GameTypeOne) {
        
        foodPath = __String::createWithFormat("cocostudio/dec/cookie_%s.png",GameDataInstance->getFlavor().c_str());
        
        GameDataInstance->setMold(MoldRound);
        
    }else if(GameDataInstance->getGameType() == GameTypeOther){
    
        foodPath = __String::createWithFormat("cocostudio/dec/cookie_%s_%s.png",GameDataInstance->getFlavor().c_str(),GameDataInstance->getMold().c_str());
    
    }
    m_pFood = Sprite::create(foodPath->getCString());
    m_pFood->setPosition(m_pBowl->getContentSize()/2);
    m_pBowl->addChild(m_pFood);
    
    _decorate = _studioLayer->getNodeByName<Layout*>("decorate");
    
    _listView = ListView::create();
    _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _listView->setBounceEnabled(true);
    _listView->setLocalZOrder(15);
    _listView->setContentSize(Size(120, 570 - 100-75));
    _studioLayer->getNodeByName<Layout*>("layer")->addChild(_listView);
    _listView->setAnchorPoint(Vec2(0.5,1));
    _listView->setPosition(Vec2(_studioLayer->getNodeByName<Layout*>("layer")->getContentSize().width/2,_studioLayer->getNodeByName<Layout*>("layer")->getContentSize().height - 60));
    _listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(MakeStepDec::selectedItemEvent, this));
    _listView->setScrollBarOpacity(0);
    _listView->setItemsMargin(_listView->getItemsMargin()+10);
   
    if (!m_pTitle) {
        
        m_pTitle = Sprite::create("cocostudio/UI/decorate/decorate.png");
        m_pTitle->setAnchorPoint(Vec2(0.5,1));
        this->addChild(m_pTitle,100);
        kAdapterScreen->setExactPosition(m_pTitle,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    }
    showDecIcing();
    
    auto lisenter = EventListenerTouchOneByOne::create();
    lisenter->onTouchBegan=CC_CALLBACK_2(MakeStepDec::onTouchAddDec, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(lisenter, this);
    
    auto lisenterBtn = MoveLisenter::create();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenterBtn, m_pDecBtn);
    lisenterBtn->setOrginOffset();
    lisenterBtn->m_bIsMove = false;
    lisenterBtn->onTouchDown = CC_CALLBACK_2(MakeStepDec::onTouchDown, this);

    return true;
}

void MakeStepDec::onEnter()
{
    BaseLayer::onEnter();
//    showDecFruit();
    if (m_pTitle) {
        AudioHelp::getInstance()->playEffect("VoiceOver/decorate the cookies.mp3");
        m_pTitle->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(0.5, Vec2(0,-500)), 1.0), NULL));
        
    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(kIAPManager->isPackagePurchased(DEC_IAP_ID)){
        for (auto lock:m_vLock) {
            lock->removeFromParent();
        }
    }
#endif
}
void MakeStepDec::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    
}
void MakeStepDec::onExit()
{
    BaseLayer::onExit();
    
}
void MakeStepDec::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    lisenter->setEnabled(false);
    
    float timeScale = 1.2;
    ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
    ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
    ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
    ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
   
    node->runAction(Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6,CallFunc::create([=](){
    
        lisenter->setEnabled(true);
    
    }), NULL));
    
}
void MakeStepDec::showDecFruit()
{
    _decType = TAGDecFruit;
    showListview("fruit");
    _listView->setTag(TAGDecFruit);
}
void MakeStepDec::showDecIcing()
{
    _decType = TAGDecIcing;
    showListview("icing",true);
    _listView->setTag(TAGDecIcing);
}
void MakeStepDec::showDecPlate()
{
    _decType = TAGDecPlate;
    showListview("plate");
     _listView->setTag(TAGDecPlate);
}
void MakeStepDec::showDecSprinkle()
{
    _decType = TAGDecSprinkle;
    showListview("sprinkle");
     _listView->setTag(TAGDecSprinkle);
}
void MakeStepDec::showHand()
{
    if (!m_pHand) {
        
        m_pHand = Sprite::create("cocostudio/UI/tips/finger.png");
        m_pHand->setPosition(Vec2(183,225));
        m_pBowl->addChild(m_pHand,20);
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        
        
            m_pHand->setVisible(true);
        
        }),MoveBy::create(0.5,Vec2(120,110)),MoveBy::create(0.5,Vec2(-120,-180)),MoveBy::create(0.5,Vec2(80,80)),CallFunc::create([=]{
        
            m_pHand->setVisible(false);
        
        }),MoveTo::create(0, Vec2(183,225)),DelayTime::create(1.0), NULL)));
        
    }
}
void MakeStepDec::showListview(string pathDec,bool isMold)
{
    _listView->removeAllChildren();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_vLock.clear();
#endif
    
    _listView->setPositionY(_listView->getPositionY()+1000);
    m_pDecBg->setPositionY(m_pDecBg->getPositionY()+1000);
    _listView->scrollToPercentVertical(0.5,0,true);
    m_pDecSelect = nullptr;
    m_pParticleSystemQuad = nullptr;
    
    m_pDecBtn->setTexture(__String::createWithFormat("cocostudio/UI/decorate/%s-btn.png",pathDec.c_str())->getCString());
    float timeScale = 1.2;
    ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
    ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
    ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
    ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
    m_pDecBtn->runAction(Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6,CallFunc::create([=](){
    
        int num = 20;
        if (pathDec == "icing") {
            num = 10;
        }
        
        for(int i = 0;i<num;i++){
            __String* path;
            if(!isMold)
                path= __String::createWithFormat("cocostudio/dec/%s_icon/%s%d.png",pathDec.c_str(),pathDec.c_str(),i);
            else
                path= __String::createWithFormat("cocostudio/dec/%s_icon/%s_%s%d.png",pathDec.c_str(),pathDec.c_str(),GameDataInstance->getMold().c_str(),i);
            
            Button* custom_button = Button::create(path->getCString(), path->getCString());
            //custom_button->setScale9Enabled(true);
            custom_button->setTag(555);
            //add by navy android 端无iap
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            if (!kIAPManager->isPackagePurchased(DEC_IAP_ID) && i>num/3) {
            
                auto lock = Sprite::create("cocostudio/UI/decorate/lock.png");
                lock->setPosition(custom_button->getContentSize()/2 + Size(20,-30));
                lock->setScale(0.7);
                custom_button->addChild(lock);
            
                m_vLock.pushBack(lock);
                            
            }
#endif

            
            Layout* custom_item = Layout::create();
            custom_item->setContentSize(custom_button->getContentSize());
            custom_button->setPosition(Vec2(custom_item->getContentSize().width / 2.0f, custom_item->getContentSize().height / 2.0f));
            custom_item->setTag(666);
            custom_item->addChild(custom_button);
            
            _listView->pushBackCustomItem(custom_item);

            
        }
        
        AudioHelp::getInstance()->playEffect("banner drop.mp3");

        
        m_pDecBg->runAction(Sequence::create(EaseBounceOut::create(MoveBy::create(0.5,Vec2(0,-1000))),CallFunc::create([=](){
            
            
            
        }), NULL));
        
        _listView->runAction(Sequence::create(EaseBounceOut::create(MoveBy::create(0.5,Vec2(0,-1000))),CallFunc::create([=](){
            
            _listView->scrollToTop(1.0,true);
            
        }), NULL));
        
    }), NULL));
    
    
    
}
bool MakeStepDec::onTouchAddDec(Touch* touch, Event* event)
{
    if(!m_bIsTouch)
        return false;
    
    if(m_pBowl->getChildByTag(107))
        m_pBowl->getChildByTag(107)->removeFromParent();
    
    if(m_pBowl->getChildByTag(108))
        m_pBowl->getChildByTag(108)->removeFromParent();

    if(_listView->getTag() == TAGDecSprinkle){
        
        if(_clipingSprinkle == nullptr){
        
            _clipingSprinkle = ClippingNode::create();
            Sprite* sp = Sprite::createWithTexture(m_pBowl->getTexture());
            sp->setScale(1.2);
            sp->setPosition(m_pBowl->getContentSize()/2);
            _clipingSprinkle->setStencil(sp);
            _clipingSprinkle->setAlphaThreshold(0.01);
            m_pBowl->addChild(_clipingSprinkle);
        
        }
        
        AudioHelp::getInstance()->playEffect("present tap.mp3");
        
        auto fruit = Sprite::create(__String::createWithFormat("cocostudio/dec/sprinkle/sprinkle%d.png",m_iIndex)->getCString());
        fruit->setPosition(m_pBowl->convertToNodeSpace(touch->getLocation()));
        _clipingSprinkle->addChild(fruit,TAGDecSprinkle);
        fruit->setScale(0);
        fruit->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.25,1.0),RotateBy::create(0.25, rand()%360), NULL), NULL));
    
    }
    
    if(_listView->getTag() == TAGDecFruit){
    
        if(_cliping == nullptr){
            
            __String* path ;
            
            if (m_pBowl->getTag() == -1) {
                
                path = __String::create("cocostudio/dec/plate.png");
                
            }else{
                
                path = __String::createWithFormat("cocostudio/dec/plate/plate%d.png",m_pBowl->getTag());
                
            }
            
            _cliping = ClippingNode::create();
            Sprite* sp = Sprite::create(path->getCString());
            sp->setScale(1.2);
            sp->setPosition(m_pBowl->getContentSize()/2);
            _cliping->setStencil(sp);
            _cliping->setAlphaThreshold(0.01);
            m_pBowl->addChild(_cliping);
            
        }
        
        
        AudioHelp::getInstance()->playEffect("present tap.mp3");
        
        auto fruit = Sprite::create(__String::createWithFormat("cocostudio/dec/fruit/fruit%d.png",m_iIndex)->getCString());
        fruit->setPosition(m_pBowl->convertToNodeSpace(touch->getLocation()));
        _cliping->addChild(fruit,TAGDecFruit);
        fruit->setScale(0);
        fruit->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.25,1.0),RotateBy::create(0.25, rand()%360), NULL), NULL));
    
    }


    return true;
}

void MakeStepDec::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    if (m_pTitle) {
        
        m_pTitle->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(0.5, Vec2(0,500)), 1.0),RemoveSelf::create(),CallFunc::create([=](){
        
            m_pTitle = nullptr;
        
        }), NULL));
        
    }
    
    if(type == cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END){
        
        AudioHelp::getInstance()->playEffect("category_selection_1.mp3");
        
        ListView* listView = static_cast<ListView*>(pSender);
        CC_UNUSED_PARAM(listView);
        CCLOG("select child end index = %ld", listView->getCurSelectedIndex());

        int tag = listView->getTag();
        int index = (int)listView->getCurSelectedIndex();
        //add by navy android端无iap
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        int num = 20;
        if(tag == TAGDecIcing){
        
            num = 10;
        
        }
        

        if (!kIAPManager->isPackagePurchased(DEC_IAP_ID)) {
            
            if (index > num/3) {
                
                Director::getInstance()->pushScene(ShopScne::createScene());
                return;
            }
            
        }
        if(!kIAPManager->isFree(DEC_IAP_ID,index)){
        
        }
#endif
//        
//        auto chooseParticle = ParticleSystemQuad::create("cocostudio/particle/knifeParticl.plist");
//        chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
//        chooseParticle->setVisible(true);
//        chooseParticle->setPosition(mold->getContentSize()*.5);
//        chooseParticle->setScale(1.0);
//        mold->addChild(chooseParticle,1);
        
        if(m_pParticleSystemQuad){

            m_pParticleSystemQuad->removeFromParent();
            m_pParticleSystemQuad = nullptr;
            
            m_pParticleSystemQuad = ParticleSystemQuad::create("cocostudio/particle/knifeParticl.plist");
            m_pParticleSystemQuad->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            m_pParticleSystemQuad->setVisible(true);
            m_pParticleSystemQuad->setPosition(listView->getItem(index)->getContentSize()*.5);
            m_pParticleSystemQuad->setContentSize(listView->getItem(index)->getContentSize());
            m_pParticleSystemQuad->setScale(0.3);
            listView->getItem(index)->addChild(m_pParticleSystemQuad,1);
//            m_pParticleSystemQuad = Sprite::create("cocostudio/UI/decorate/dec-bg-up.png");
//            m_pDecSelect->setPosition(listView->getItem(index)->getContentSize()/2);
//            listView->getItem(index)->addChild(m_pDecSelect);
        
        }else{
        
//            m_pDecSelect = Sprite::create("cocostudio/UI/decorate/dec-bg-up.png");
//            m_pDecSelect->setPosition(listView->getItem(index)->getContentSize()/2);
//            listView->getItem(index)->addChild(m_pDecSelect);
            m_pParticleSystemQuad = ParticleSystemQuad::create("cocostudio/particle/knifeParticl.plist");
            m_pParticleSystemQuad->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            m_pParticleSystemQuad->setVisible(true);
            m_pParticleSystemQuad->setContentSize(listView->getItem(index)->getContentSize());
            m_pParticleSystemQuad->setPosition(listView->getItem(index)->getContentSize()*.5);
            m_pParticleSystemQuad->setScale(0.3);
            listView->getItem(index)->addChild(m_pParticleSystemQuad,1);
        
        }
        switch (tag) {
            case TAGDecFruit:
            {
                m_iIndex = index;
                
                m_bIsTouch = true;
                
                AudioHelp::getInstance()->playEffect("present tap.mp3");
                
                if(m_bIsShowHand2)
                    return;
                
                
                if (!m_bIsShowHand2)
                    m_bIsShowHand2 = true;
                
                
                if(m_pBowl->getChildByTag(107))
                    m_pBowl->getChildByTag(107)->removeFromParent();
                
                if(m_pBowl->getChildByTag(108))
                    m_pBowl->getChildByTag(108)->removeFromParent();
                
                auto tips = Sprite::create("cocostudio/UI/tips/click.png");
                tips->setPosition(m_pBowl->getContentSize()/2);
                tips->setAnchorPoint(Vec2(0.5,0.5));
                m_pBowl->addChild(tips);
                tips->setTag(107);
                
                auto hand = Sprite::create("cocostudio/UI/tips/finger.png");
                hand->setAnchorPoint(Vec2(0.12,0.89));
                m_pBowl->addChild(hand);
                hand->setPosition(m_pBowl->getContentSize()/2 + Size(0,-100));
                hand->setVisible(false);
                hand->setTag(108);
                
                tips->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,1.2),ScaleTo::create(0.5,1.0), NULL)));
                
                hand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
                    
                    hand->setVisible(true);
                    tips->setVisible(true);
                    
                }),MoveBy::create(1.0,Vec2(0,100)),CallFunc::create([=](){
                    
                    hand->setPosition(m_pBowl->getContentSize()/2 + Size(0,-100));
                    
                }),MoveBy::create(1.0,Vec2(0,100)),CallFunc::create([=](){
                    
                    hand->setVisible(false);
                    hand->setPosition(m_pBowl->getContentSize()/2 + Size(0,-100));
                    tips->setVisible(false);
                }),DelayTime::create(2.0), NULL)));
                
            }
                break;
            case TAGDecIcing:
            {
                m_bIsTouch = false;
                
                auto sprite = Sprite::create(__String::createWithFormat("cocostudio/dec/icing/icing_%s%d.png",GameDataInstance->getMold().c_str(),index)->getCString());
                //                if(_icing != nullptr)
                //                    _icing->removeFromParent();
                //                _icing = nullptr;
                if(_icing == nullptr){
                    
                    _icing = ScribbleWithCallBack::create(sprite->getContentSize());
                    _icing->setPosition(m_pFood->getContentSize()*.5);
                    m_pFood->addChild(_icing);
                    _icing->setAntiAliasingBrush(true);
                    _icing->useBrush("cocostudio/dec/hole_stencilsamll.png",Scribble::eBrush);
                    _icing->callBack = [this](cocos2d::Touch *,ScribbleWithCallBack::TouchType _type){
                        if(_type == ScribbleWithCallBack::TouchType::eTouchMoved){
                            //      this->stopPrompt();
                        }
                        if(_type == ScribbleWithCallBack::TouchType::eTouchBegan){
                            if (m_pHand) {
                                m_pHand->removeFromParent();
                                m_pHand = nullptr;
                            }
                        }
                    };
                    
                }
                _icing->useTarget(__String::createWithFormat("cocostudio/dec/icing/icing_%s%d.png",GameDataInstance->getMold().c_str(),index)->getCString());
                _icing->enable();
                
                if(m_bIsShowHand1)
                    return;
                
                if (!m_bIsShowHand1)
                    m_bIsShowHand1 = true;
                
                showHand();
                
                
                
                
                
                
                
            }
                break;
            case TAGDecPlate:
            {
                m_bIsTouch = false;
                
                auto plate = Sprite::create(__String::createWithFormat("cocostudio/dec/plate/plate%d.png",index)->getCString());
                plate->setPosition(listView->getItem(index)->convertToWorldSpace(listView->getItem(index)->getContentSize()/2));
                m_pBowl->getParent()->addChild(plate);
                plate->setScale(0);
                _studioLayer->reorderChild(plate, 5);

                plate->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, m_pBowl->getPosition()),ScaleTo::create(0.5,1.0), NULL),RemoveSelf::create(),CallFunc::create([=](){
                
                    AudioHelp::getInstance()->playEffect("plate decoration.mp3");

                    m_pBowl->setTag(index);
                    GameDataInstance->setBowlIndex(index);
                    m_pBowl->setTexture(__String::createWithFormat("cocostudio/dec/plate/plate%d.png",index)->getCString());
                    m_pFood->setPosition(m_pBowl->getContentSize()/2);
                    
                }), NULL));
                
            }
                break;
            case TAGDecSprinkle:
            {
                m_iIndex = index;
                
                m_bIsTouch = true;

                if(m_bIsShowHand)
                    return;
                
                
                if (!m_bIsShowHand)
                    m_bIsShowHand = true;
               
                
                if(m_pBowl->getChildByTag(107))
                    m_pBowl->getChildByTag(107)->removeFromParent();
                
                if(m_pBowl->getChildByTag(108))
                    m_pBowl->getChildByTag(108)->removeFromParent();
                
                auto tips = Sprite::create("cocostudio/UI/tips/click.png");
                tips->setPosition(m_pBowl->getContentSize()/2);
                tips->setAnchorPoint(Vec2(0.5,0.5));
                m_pBowl->addChild(tips);
                tips->setTag(107);
                
                auto hand = Sprite::create("cocostudio/UI/tips/finger.png");
                hand->setAnchorPoint(Vec2(0.12,0.89));
                m_pBowl->addChild(hand);
                hand->setPosition(m_pBowl->getContentSize()/2 + Size(0,-100));
                hand->setVisible(false);
                hand->setTag(108);
                
                tips->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,1.2),ScaleTo::create(0.5,1.0), NULL)));
                
                hand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
                
                    hand->setVisible(true);
                    tips->setVisible(true);
                    
                }),MoveBy::create(1.0,Vec2(0,100)),CallFunc::create([=](){

                   hand->setPosition(m_pBowl->getContentSize()/2 + Size(0,-100));
                    
                }),MoveBy::create(1.0,Vec2(0,100)),CallFunc::create([=](){
                
                    hand->setVisible(false);
                   hand->setPosition(m_pBowl->getContentSize()/2 + Size(0,-100));
                    tips->setVisible(false);
                }),DelayTime::create(2.0), NULL)));
                
            }
                break;
            default:
                break;
        }
        
        
    }
    
    
}
void MakeStepDec::touchEnd(ui::Widget* widget)
{
//    Director::getInstance()->pushScene(MakeStepEat::createScene());
    if (m_pHand) {
        m_pHand->removeFromParent();
        m_pHand = nullptr;
    }
    
    if(m_pBowl->getChildByTag(107))
        m_pBowl->getChildByTag(107)->removeFromParent();
    
    if(m_pBowl->getChildByTag(108))
        m_pBowl->getChildByTag(108)->removeFromParent();
    
    int tag = widget->getTag();
    //下一步
    if (tag == 100) {
        widget->setEnabled(false);
        
        ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
        
            widget->setEnabled(true);
        
        }));
        
        if (_decType == TAGDecFruit) {
            
            Vec2 bowlPos = m_pBowl->getPosition();
            //next
            RenderTexture* texture = RenderTexture::create(m_pBowl->getContentSize().width, m_pBowl->getContentSize().height,Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
            texture->retain();
            texture->begin();
            
            m_pBowl->setCascadeOpacityEnabled(false);
            m_pBowl->setOpacity(0);
            m_pBowl->setAnchorPoint(Vec2::ZERO);
            m_pBowl->setPosition(Vec2::ZERO);
            m_pBowl->visit();
//            _decorate->cocos2d::Node::visit();
            
            texture->end();
            Director::getInstance()->getRenderer()->render();
            texture->getSprite()->getTexture()->setAntiAliasTexParameters();
            
            m_pBowl->setPosition(bowlPos);
            m_pBowl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_pBowl->runAction(FadeIn::create(0.0));
            
            SSCFileUtility::createDirectory((SSCFileUtility::getStoragePath()+ "/"+"cookie").c_str());
         
            string rootPath;
//            rootPath = FileUtility::getStoragePath() + "/";
            rootPath = FileUtils::getInstance()->getWritablePath();
            struct timeval tv;
            gettimeofday(&tv,NULL);
            long nowtime =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
            
            string path = StringUtils::format("%s%ld.png",rootPath.c_str(),nowtime);
            
            Image* image1 = texture->newImage();
            image1->saveToFile(path.c_str(),false);
            
            GameDataInstance->setEatPath(path);
            GameDataInstance->setBowlIndex(m_pBowl->getTag());
            log("%s",path.c_str());
            
            if(IAPManager::getInstance()->isShowAds()){
                
                AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
                AdLoadingLayerBase::loadingDoneCallback=[=](){
                    
                    
                    KitchenLayer::loading(KitchenType::kitchen_door,[=](){
                        
                        Director::getInstance()->replaceScene(MakeStepEat::createScene());
                        
                    });
                    
                };
                
            }else{
                
                KitchenLayer::loading(KitchenType::kitchen_door,[=](){
                    
                    Director::getInstance()->replaceScene(MakeStepEat::createScene());
                    
                });

                
            }
            
            
        }else if(_decType == TAGDecIcing){
        
            if(_icing)
                _icing->disable();
            
            showDecSprinkle();
        
        }else if(_decType == TAGDecPlate){
        
            showDecFruit();
        
        }else if(_decType == TAGDecSprinkle){
        
            showDecPlate();
        }
        
    }
    //reset
    else if(tag == 200){
    
        _studioLayer->getNodeByName<Button*>("btn_reset")->setEnabled(false);
        m_bIsShowReturn = true;
        Dialog* dialog= Dialog::create((void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL);
        dialog -> setAnchorPoint(Vec2(0,0));
        kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
        dialog->setContentText("Do you want to reset your food?");
        dialog->setPositiveBtnText("");
        dialog->setNegativeBtnText("");
        dialog->setCallback(this);
        this->addChild(dialog, 9999);
    
    }
}
void MakeStepDec::onNegativeClick(void* type)
{
     _studioLayer->getNodeByName<Button*>("btn_reset")->setEnabled(true);
    m_bIsShowReturn = false;
}
void MakeStepDec::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
     _studioLayer->getNodeByName<Button*>("btn_reset")->setEnabled(true);
    size_t lType = (size_t)type;
    if (lType == kDialogReset) {
        
        AudioHelp::getInstance()->playEffect("reset.mp3");

        if (_decType == TAGDecFruit) {
           
            if (_cliping) {
                _cliping->removeAllChildren();
            }
            
        }else if(_decType == TAGDecPlate){
        
            GameDataInstance->setBowlIndex(-1);
            m_pBowl->setTag(-1);
            m_pBowl->setTexture(__String::createWithFormat("cocostudio/dec/plate.png")->getCString());
            m_pFood->setPosition(m_pBowl->getContentSize()/2);
            
        }else if (_decType == TAGDecIcing){
        
            if (_icing) {
                _icing->removeFromParent();
                _icing = nullptr;
            }
            
        }else if(_decType == TAGDecSprinkle){
        
            if (_clipingSprinkle) {
                _clipingSprinkle->removeFromParent();
                _clipingSprinkle = nullptr;
            }
        }
    }
}




