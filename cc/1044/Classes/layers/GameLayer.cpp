//
//  GameLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#include "GameLayer.h"
#include "AdapterScreen.h"
#include "Utils.h"
#include "IAPManager.h"
#include "STAds.h"
#include "SceneManager.h"
#include "AudioHelp.h"
#include "FillMaterialModel.h"
#include "Global.h"
#include "IceCreamStepLayer1.h"
#include "IceCreamStepLayer2.h"
#include "IceCreamStepLayer3.h"
#include "IcePopStepLayer1.h"
#include "IcePopStepLayer2.h"
#include "IcePopStepLayer3.h"
#include "SnowStepLayer1.h"
#include "SnowStepLayer2.h"
#include "SnowStepLayer3.h"
#include "SnowStepLayer4.h"
#include "SnowStepLayer5.h"
#include "IceCreamDecorationLayer.h"
#include "ActionUtils.h"
#include "IcePopDecorationLayer.h"

GameLayer::GameLayer()
{
    m_bNextButtonIsShow = false;
}

GameLayer::~GameLayer()
{

//     log("-------%d",G_IceCream_CupSprite->getReferenceCount());
//    if(G_IceCream_CupSprite){
//    
//        log("-------%d",G_IceCream_CupSprite->getReferenceCount());
//    
//    }
    
//    if (m_pDecorationLayer->getReferenceCount()>1 &&m_pDecorationLayer->getReferenceCount()) {
//        for (int i = 0; i<m_pDecorationLayer->getReferenceCount() - 1; i++) {
//            m_pDecorationLayer->release();
//        }
//    }
    
    
}

void GameLayer::onEnter()
{
    BaseLayer::onEnter();
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.requestAds();
    }
    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("bgmusic.mp3");

}
void GameLayer::onExit()
{
    BaseLayer::onExit();

}
void GameLayer::showBanner(Ref* r)
{

    bannerHeight = 100;
    log("bannerDidShow");
}
void GameLayer::hideBanner(Ref* r)
{
    //隐藏banner
    bannerHeight = 0;
    log("bannerDidHide");
}
bool GameLayer::init()
{
    bool isInit=false;
    
    do {
        CC_BREAK_IF(!BaseLayer::init());
        initUI();
        isOperateEnd=false;
        isDecorateEnd=false;
        
        m_fAddPosY = 0.0;
        m_bNextAction = true;
//      //选择进入
      if(CHOOSE_TYPE == 0)
      {
            
           doFirstStep(SnowStepLayer1::create(this));
            
      }else if (CHOOSE_TYPE == 1)
      {
           
           doFirstStep(IceCreamStepLayer1::create(this));
            
      }else if (CHOOSE_TYPE == 2)
      {
           
          doFirstStep(IcePopStepLayer1::create(this));
            
      }
////
//        CHOOSE_TYPE =2;
//        doFirstStep(IcePopStepLayer1::create(this));
//        CHOOSE_TYPE = 0;
        m_pNextStepLayer=nullptr;
        m_pDecorationLayer=nullptr;
        
//       OperateEndToDecoration(IcePopDecorationLayer::create(this));
       isInit = true;
        
    } while (0);
    
    return isInit;
}

void GameLayer::initUI()
{
    m_pBgSprite = Sprite::create("images/ice_cream/1/bg.jpg");
    kAdapterScreen->setExactPosition(m_pBgSprite, DESIGN_WIDTH/2, DESIGN_HEIGHT/2);
    this->addToBackGroundLayer(m_pBgSprite, 1);
    
//    m_pMenu = Menu::create();
//    m_pMenu->setPosition(Vec2::ZERO);
//    m_pMenu->setZOrder(100);
//    m_pNextItemSprite = Utils::getMenuItemSprite("images/button/btn_next.png", "images/button/btn_next.png", m_pMenu, this, menu_selector(GameLayer::onBtnCallBack));
//    kAdapterScreen->setExactPosition(m_pNextItemSprite, 10, 10 + 100 + m_fAddPosY,Vec2(1, 0),kBorderRight,kBorderBottom);
//    setNextButtonEnable(false);
    m_pNextItemSprite = ui::Button::create("images/button/btn_next.png");
    m_pNextItemSprite->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    m_pNextItemSprite->setTag(100);
    m_pNextItemSprite->setVisible(false);
    kAdapterScreen->setExactPosition(m_pNextItemSprite, 10, 10 + 100,Vec2(1, 0),kBorderRight,kBorderBottom);
//    this->addToUILayer(nextBtn, 20);
    
//    this->addChild(m_pNextItemSprite, 100);
    this->addToUILayer(m_pNextItemSprite, 10);
    log("???????---%f,%f",m_pNextItemSprite->getPositionX(),m_pNextItemSprite->getPositionY());
}

void GameLayer::changeBackGround(string bgPath)
{
    m_pBgSprite->setTexture(bgPath);
}

void GameLayer::setNextButtonEnable(bool isVisible)
{
    m_pNextItemSprite->setVisible(isVisible);
    m_pNextItemSprite->setEnabled(isVisible);
    
//    ScaleBy* scale=ScaleBy::create(0.6f, 1.2f);
    if(isVisible){
        
        log("showNextButton");
        m_bNextButtonIsShow = true;
        if (m_bNextAction) {
            ActionUtils::jumpAction(m_pNextItemSprite,2,nullptr,0.5);
        }
   }
    
}

void GameLayer::showNextButton(bool isVisible)
{
    m_pNextItemSprite->setVisible(isVisible);
    m_pNextItemSprite->setEnabled(isVisible);
    
}
void GameLayer::addNextSpritePosY(float y)
{
    m_pNextItemSprite->setPositionY(m_pNextItemSprite->getPositionY() + y);
}
void GameLayer::doFirstStep(StepLayer* firstStep)
{
    m_pCurrentStepLayer = firstStep;
    if(m_pCurrentStepLayer){
        this->addToContentLayer(m_pCurrentStepLayer, 0);
    }
}

void GameLayer::doNextStep(StepLayer* nextStep)
{
    setNextButtonEnable(true);
    if(nextStep){
       nextStep->retain();
    }
   
    if(m_pNextStepLayer){
        
        m_pNextStepLayer->release();
        m_pNextStepLayer = nullptr;
        
    }
    
    m_pNextStepLayer=nextStep;


    
    
}

void GameLayer::OperateEndToDecoration(BaseLayer* dec)
{
    isOperateEnd=true;
    if(dec)
        dec->retain();
    if(m_pDecorationLayer){
        m_pDecorationLayer->release();
    }
    if(m_pNextStepLayer){
        
         m_pNextStepLayer->release();
        
    }

    m_pDecorationLayer = dec;

    setNextButtonEnable(true);
}

void GameLayer::popDecorationLayer()
{
    isOperateEnd=false;
    isDecorateEnd = false;
    
    if(m_pDecorationLayer)
    {
      m_pDecorationLayer->release();
      m_pDecorationLayer =nullptr;
    }
}
void GameLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
                    m_pNextItemSprite->stopAllActions();
                    AudioHelp::getInstance()->playEffect("next page.mp3");
                    setNextButtonEnable(false);
                    m_bNextButtonIsShow = false;
                    if(m_pCurrentStepLayer!=nullptr)
                    {
                        m_pContentLayer->removeChild(m_pCurrentStepLayer);
                    }
                    if(!isDecorateEnd)
                    {
                        
                        if(!isOperateEnd)
                        {
                            log("m_pCurrentStepLayer---%d",m_pCurrentStepLayer->getReferenceCount());
                            m_pCurrentStepLayer=m_pNextStepLayer;
                            log("m_pCurrentStepLayer---%d",m_pCurrentStepLayer->getReferenceCount());
                            this->addToContentLayer(m_pCurrentStepLayer, 0);
                            log("m_pCurrentStepLayer---%d",m_pCurrentStepLayer->getReferenceCount());
                            
                        }else
                        {
                            
                            this->addToContentLayer(m_pDecorationLayer, 0);
                            m_pDecorationLayer->release();
                        }
                        
                    }
                    else
                    {
                        //停止移动精灵的动画
                        if (fillDecorationLayer) {
                            for (int i = 0; i<fillDecorationLayer->getChildren().size(); i++) {
                                
                                dynamic_cast<FillMaterialModel*>(fillDecorationLayer->getChildren().at(i))->stopTipsFrameStatus();
                                
                            }
                        }
                        if (iceDecorationLayer) {
                            for (int i = 0; i<iceDecorationLayer->getChildren().size(); i++) {
                                
                                dynamic_cast<FillMaterialModel*>(iceDecorationLayer->getChildren().at(i))->stopTipsFrameStatus();
                                
                            }
                        }
                        
                        
                        //cross
                        Scene* scene = nullptr;
                        if (CHOOSE_TYPE == 0) {
                            
                            
                            scene = SceneManager::getInstance()->enterSnowConeEatScene();
                            
                        }else if(CHOOSE_TYPE == 1){
                            
                            
                            scene = SceneManager::getInstance()->enterIceCreamEatScene();
                            
                        }else if(CHOOSE_TYPE == 2){
                            
                            
                            scene = SceneManager::getInstance()->enterIcePopEatScene();
                            
                        }
                        if (kIAPManager->isShowAds()) {
                            
                            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true,scene);
                            
                        }
                        
                        
                    }

                }
                    break;
            }
        }
    }

}
void GameLayer::onBtnCallBack(Ref* sender)
{
    m_pNextItemSprite->stopAllActions();
    AudioHelp::getInstance()->playEffect("next page.mp3");
    setNextButtonEnable(false);
    m_bNextButtonIsShow = false;
    if(m_pCurrentStepLayer!=nullptr)
    {
        m_pContentLayer->removeChild(m_pCurrentStepLayer);
    }
    if(!isDecorateEnd)
    {
        
        if(!isOperateEnd)
        {
           
            m_pCurrentStepLayer=m_pNextStepLayer;
            this->addToContentLayer(m_pCurrentStepLayer, 0);
            
           
        }else
        {
            
            this->addToContentLayer(m_pDecorationLayer, 0);
            
        }

    }
    else
    {
        //停止移动精灵的动画
        if (fillDecorationLayer) {
            for (int i = 0; i<fillDecorationLayer->getChildren().size(); i++) {
                
                dynamic_cast<FillMaterialModel*>(fillDecorationLayer->getChildren().at(i))->stopTipsFrameStatus();
                
            }
        }
        if (iceDecorationLayer) {
            for (int i = 0; i<iceDecorationLayer->getChildren().size(); i++) {
                
                dynamic_cast<FillMaterialModel*>(iceDecorationLayer->getChildren().at(i))->stopTipsFrameStatus();
                
            }
        }
        
        
        
        log("m_pDecorationLayer ------- %d",m_pDecorationLayer->getReferenceCount());
        
        //cross
        Scene* scene = nullptr;
        if (CHOOSE_TYPE == 0) {
            
            
            scene = SceneManager::getInstance()->enterSnowConeEatScene();
            
        }else if(CHOOSE_TYPE == 1){
        
            
            scene = SceneManager::getInstance()->enterIceCreamEatScene();
            
        }else if(CHOOSE_TYPE == 2){
            
            
            scene = SceneManager::getInstance()->enterIcePopEatScene();
            
        }
        if (kIAPManager->isShowAds()) {
            
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true,scene);
            
        }
        
        
    }
}
//模拟Button
void GameLayer::nextButtonClicked(StepLayer* nextStep)
{
    log("nextStep---%d",nextStep->getReferenceCount());
    if(nextStep)
        nextStep->retain();
    
    log("nextStep---%d",nextStep->getReferenceCount());
    
    log("m_pNextStepLayer---%d",m_pNextStepLayer->getReferenceCount());
    log("m_pNextStepLayer---%d",m_pNextStepLayer->getReferenceCount());
    if(m_pNextStepLayer){
        m_pNextStepLayer->release();
    }
    log("m_pNextStepLayer---%d",m_pNextStepLayer->getReferenceCount());
    m_pNextStepLayer=nextStep;
    log("m_pNextStepLayer---%d",m_pNextStepLayer->getReferenceCount());
    
    m_pNextItemSprite->stopAllActions();
    AudioHelp::getInstance()->playEffect("next page.mp3");
    setNextButtonEnable(false);
    m_bNextButtonIsShow = true;
    if(m_pCurrentStepLayer!=nullptr)
    {
        m_pContentLayer->removeChild(m_pCurrentStepLayer);
    }
    if(!isDecorateEnd)
    {
        
        if(!isOperateEnd)
        {
            m_pCurrentStepLayer=m_pNextStepLayer;
            this->addToContentLayer(m_pCurrentStepLayer, 0);
            log("m_pCurrentStepLayer---%d",m_pCurrentStepLayer->getReferenceCount());
            
        }else
        {
            this->addToContentLayer(m_pDecorationLayer, 0);
        }
        
    }
    else
    {
        if (kIAPManager->isShowAds()) {
            STAds ads;
            ads.preloadCrosspromo();
        }
        if (CHOOSE_TYPE == 0) {
            
            SceneManager::getInstance()->enterIceCreamEatScene();
            
        }else if(CHOOSE_TYPE == 1){
            
            SceneManager::getInstance()->enterIcePopEatScene();
            
        }else if(CHOOSE_TYPE == 2){
            
            SceneManager::getInstance()->enterSnowConeEatScene();
            
        }
        
        
        
    }
}
void GameLayer::setEatCreamSceneNum(int num)
{
    m_iEatSceneNum = num;

}
// 监听Android返回键事件
void GameLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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

void GameLayer::onNegativeClick(void* type)
{
    isReturn = false;
}
void GameLayer::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }

}

void GameLayer::addToBGSprite(Node* node,int zOrder)
{
    m_pBgSprite->addChild(node,zOrder);

}
