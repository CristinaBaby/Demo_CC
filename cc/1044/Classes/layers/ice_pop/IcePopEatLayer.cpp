//
//  IcePopEatLayer.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/13.
//
//

#include "IcePopEatLayer.h"
#include "AdapterScreen.h"
#include "ToolSprite.h"
#include "Dialog.h"
#include "CFSystemFunction.h"
#include "FavoriteManager.h"
#include "AudioHelp.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"

#include "FillMaterialModel.h"
#include "SceneManager.h"

enum buttonType
{
    KBACKBUTTON,
    KFAVORITEBUTTON,
    KHOMEBUTTON,
    kNEXTBUTTON,
    
};
IcePopEatLayer::IcePopEatLayer()
{
    
}
IcePopEatLayer::~IcePopEatLayer()
{
    
}
bool IcePopEatLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initUI();
        initMenu();
        
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void IcePopEatLayer::initUI()
{
    
    //    if(-1 == backgroundIndex)
    //    {
    //        m_pBackGround = Utils::getUISprite("images/background/pizza_bg.png");
    //    }
    //    else
    //    {
    //        m_pBackGround = Utils::getUISprite(__String::createWithFormat("images/decorate/category/bg/bg%d.png",
    //                                                                      backgroundIndex)->getCString());
    //    }
    //    kAdapterScreen->setExactPosition(m_pBackGround, 480, 320);
    //    this->addToBackGroundLayer(m_pBackGround, 0);
    
    
    auto pBgSprite = Utils::getUISprite("images/ice_pop/dec/bg.jpg");
    kAdapterScreen->setExactPosition(pBgSprite, 320, 480);
    this->addToBackGroundLayer(pBgSprite, 1);
    
    auto pBg = Utils::getUISprite("images/ice_pop/dec/bg1.png");
    kAdapterScreen->setExactPosition(pBg, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addToBackGroundLayer(pBg, 2);
    
//    kAdapterScreen->setExactPosition(G_IcePop_MouleSprite, 320, 350 + 50 + 100);
//    this->addChild(G_IcePop_MouleSprite,3);
    
    m_pSticks = Sprite::create("images/ice_pop/3/stick.png");
    kAdapterScreen->setExactPosition(m_pSticks, 320, 300 + 50);
    this->addChild(m_pSticks,2);
    
    if(G_IcePop_SticksNum != -1){
    
        __String* str = __String::createWithFormat("images/dec/category/sticks/sticks%d.png",G_IcePop_SticksNum);
        m_pSticks->setTexture(str->getCString());
        
    }
    
    auto ice = Sprite::create(__String::createWithFormat("images/ice_pop/2/moule/mould_%d.png",G_IcePop_MouleNum)->getCString());
    kAdapterScreen->setExactPosition(ice, 320, 350 + 50 + 100);
    this->addChild(ice,4);
    ice->setRotation(180);
    
//    log("--------%d",G_IcePop_MouleSprite->getReferenceCount());
    
    m_pSpriteTip = nullptr;
    
    RenderTexture *rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->begin();
    
    G_IcePop_MouleSprite->visit();
    ice->visit();
    decorationLayer->visit();
    fillDecorationLayer->visit();
    
    rt->end();
    Director::getInstance()->getRenderer()->render();
    rt->saveToFile("eat.png");
    this->scheduleOnce(schedule_selector(IcePopEatLayer::getScribbleCallback), 0.2f);
    
    ice->setVisible(false);
    

    
    
    
}
void IcePopEatLayer::initMenu()
{
    auto favBtn = ui::Button::create("images/button/btn_share.png");
    favBtn -> addTouchEventListener(CC_CALLBACK_2(IcePopEatLayer::touchEvent, this));
    favBtn->setTag(KFAVORITEBUTTON);
    kAdapterScreen->setExactPosition(favBtn, 10, 110 + 100 + 10,Vec2(1, 0),kBorderRight,kBorderBottom);
    this->addToUILayer(favBtn, 20);
    
    auto homeBtn = ui::Button::create("images/button/btn_home.png");
    homeBtn -> addTouchEventListener(CC_CALLBACK_2(IcePopEatLayer::touchEvent, this));
    homeBtn->setTag(KHOMEBUTTON);
    kAdapterScreen->setExactPosition(homeBtn, 10, 10 + 100 + 100 + 10,Vec2(0, 0),kBorderLeft,kBorderBottom);
    this->addToUILayer(homeBtn, 20);
    
    auto backBtn = ui::Button::create("images/button/btn_back.png");
    backBtn -> addTouchEventListener(CC_CALLBACK_2(IcePopEatLayer::touchEvent, this));
    backBtn->setTag(KBACKBUTTON);
    kAdapterScreen->setExactPosition(backBtn, 10, 110,Vec2(0, 0),kBorderLeft,kBorderBottom);
    this->addToUILayer(backBtn, 20);
    
    auto nextBtn = ui::Button::create("images/button/btn_next.png");
    nextBtn -> addTouchEventListener(CC_CALLBACK_2(IcePopEatLayer::touchEvent, this));
    nextBtn->setTag(kNEXTBUTTON);
    kAdapterScreen->setExactPosition(nextBtn, 10, 110 ,Vec2(1, 0),kBorderRight,kBorderBottom);
    this->addToUILayer(nextBtn, 20);
    
    m_pEatAgain = ToolSprite::create("images/dec/eat again.png");
    kAdapterScreen->setExactPosition(m_pEatAgain, 320, 100 - 500,Vec2(0.5, 1),kBorderNone,kBorderTop);
    m_pEatAgain->setDelegate(this);
    m_pEatAgain->setIsMove(false);
    m_pEatAgain->cancelEvent(true);
    this->addChild(m_pEatAgain,20);
    
}


void IcePopEatLayer::onEnter()
{
    BaseLayer::onEnter();
    //    _showTip();
//    ShowIcon::show();
//    G_IcePop_MouleSprite->setVisible(false);
    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(IcePopEatLayer::eatOver), "eatOver", NULL);
}
void IcePopEatLayer::eatOver(Ref*)
{
    node->disable();
    node->removeFromParent();
    node=nullptr;
    this->unschedule(schedule_selector(IcePopEatLayer::getScribbleCallback));
    auto func = CallFunc::create([&](){
        
        eatAgain();
    });
    ActionUtils::moveBy(m_pEatAgain, Vec2(0,-500),1.0,func);
    
    
    
}
void IcePopEatLayer::eatAgain()
{
    m_pEatAgain->cancelEvent(false);
    ScaleBy* move = ScaleBy::create(0.5, 1.1);
    m_pEatAgain->runAction(RepeatForever::create(Sequence::create(move,move->reverse(),NULL)));
}
void IcePopEatLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    G_IcePop_MouleSprite->setVisible(false);
}

void IcePopEatLayer::onExit()
{
    BaseLayer::onExit();
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "eatOver");
}
void IcePopEatLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    m_pEatAgain->cancelEvent(true);
    ActionUtils::moveBy(m_pEatAgain, Vec2(0,500));
    
   
//    node = nullptr;
    this->scheduleOnce(schedule_selector(IcePopEatLayer::getScribbleCallback), 0.2f);
    
}
void IcePopEatLayer::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    
    
}
void IcePopEatLayer::onBtnCallBack(Ref* sender)
{
    
}

void IcePopEatLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case KFAVORITEBUTTON:
                {
                    m_pFavoriteTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
                    m_pFavoriteTexture->retain();
                    m_pFavoriteTexture->begin();
                    m_pBackGroundLayer->visit();
                    m_pSticks->visit();
                    m_pContentLayer->visit();
                    m_pFavoriteTexture->end();
                    Director::getInstance()->getRenderer()->render();
                    m_pFavoriteTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
                    
                    //saveToPhoto();
                    
                    AudioHelp::getInstance()->playEffect("button_camera.mp3");
                    scheduleOnce(schedule_selector(IcePopEatLayer::saveToPhoto), 0.01f);
                    btn->setTouchEnabled(false);
                }
                    break;
                case KHOMEBUTTON:
                {
                    AudioHelp::getInstance()->playSelectEffect();
                    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
                    //                    dialog -> setAnchorPoint(Vec2(0,0));
                    //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
                    dialog -> setAnchorPoint(Vec2(0,0));
                    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
                    dialog->setContentText(__String::createWithFormat("Are you sure you want to start over?")->getCString());
                    dialog->setPositiveBtnText("");
                    dialog->setNegativeBtnText("");
                    dialog->setCallback(this);
                    this->addToUILayer(dialog, 9999);
                    
                }
                    break;
                case KBACKBUTTON:
                {
                    AudioHelp::getInstance()->playEffect("previous page.mp3");
                    SceneManager::getInstance()->popScene();
                    remove(__String::createWithFormat("%seat.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    if (node) {
                          node->removeFromParent();
                    }
                  
                    
                    
                    G_IcePop_MouleSprite->setVisible(true);
                }
                    break;
                    
                case kNEXTBUTTON:
                {
                    //回到选择游戏界面
                    Director::getInstance()->popToRootScene();
                    SceneChangeManager->enterSelectScene();
                }
                    break;
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
}


void IcePopEatLayer::_showTip()
{
    
    if (!m_pSpriteTip) {
        m_pSpriteTip = Utils::getUISprite("images/button/tap@2x.png");
        this->addToUILayer(m_pSpriteTip, 255);
        m_pSpriteTip->setZOrder(101);
    }
    showTip();
}
void IcePopEatLayer::showTip()
{
    if (m_pSpriteTip) {
        m_pSpriteTip->setPosition(Vec2(visibleSize.width+534.30f,577.51f));
        m_pSpriteTip->runAction(Sequence::create(MoveTo::create(1.5f, kAdapterScreen->getExactPostion(Vec2(534.30f,577.51f))),
                                                 DelayTime::create(1.0f),
                                                 MoveTo::create(1.5f, kAdapterScreen->getExactPostion(Vec2(-534.30f,577.51f))),
                                                 nullptr));
        
    }
}

void IcePopEatLayer::onNegativeClick(void*)
{
    
}

void IcePopEatLayer::getScribbleCallback(float ft)
{
    
    node  = ScribbleTouchNode::create(Size(visibleSize.width, visibleSize.height),0);
    string str = FileUtils::getInstance()->getWritablePath()+"eat.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    node->useTarget(str);
    node->useBrush(str,Scribble::eBrush);
    node->paint(Vec2::ZERO);
    kAdapterScreen->setExactPosition(node, 320, 480);
    this->addToContentLayer(node, 10);
    node->enable();
    node->enableTouchMoved(false);
//    node->enableTouchEnded(false);
    node->useBrush("images/button/eat_mask.png",Scribble::eEaser);
    
}


void IcePopEatLayer::onPositiveClick(void* type)
{
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        
        Director::getInstance()->popToRootScene();
        auto scene = SceneChangeManager->enterHomeScene();
        //cross
        if (kIAPManager->isShowAds()) {
            
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true,scene);
            
        }
    }
    if (lType == kDialogTypeNone)
    {
        
    }
    
}

void IcePopEatLayer::saveToPhoto(float ft)
{
    Image* image1 = m_pFavoriteTexture->newImage();
    CFSystemFunction sys;
    sys.saveToAlbum(image1, CC_CALLBACK_1(IcePopEatLayer::addFavoriteCallback, this));
}
void IcePopEatLayer::addFavoriteCallback(bool isSuc)
{
    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE);
    
    dialog -> setAnchorPoint(Vec2(0,0));
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    
    if(isSuc)
    {
        
        dialog->setContentText(__String::create("Your photo has been saved to your album!")->getCString());
    }
    else
    {
        dialog->setContentText("Fail to save photo!");
    }
    Button *btn = dynamic_cast<Button *>(this->m_pUILayer->getChildByTag(KFAVORITEBUTTON));
    btn->setTouchEnabled(true);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addToUILayer(dialog, 9999);
}



// 监听Android返回键事件
void IcePopEatLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!isReturn)
        {
            isReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText(__String::createWithFormat("Are you sure you want to start over with a new %s?",MAKERNAME)->getCString());
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}
