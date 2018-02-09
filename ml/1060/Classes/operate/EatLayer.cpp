//
//  EatLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "EatLayer.hpp"
#include "DecorationLayer.hpp"
#include "FilePathUtil.h"
#include "TouchNodeComponent.h"
#include "MyDialog.h"
#include "LQ_adLoadingLayer.h"
#include "HomeLayer.hpp"
#include "Pricelayer.h"
#include "AblumLayer.h"
#include "VoteLayer.hpp"
#include "FlurryEventManager.h"

USING_NS_KD;

const string EatLayer::_eventName = "Eat page";

IMPLEMENT_COCOS2DX_CLASS(EatLayer);
EatLayer::~EatLayer(){
    Director::getInstance()->getTextureCache()->removeTextureForKey(_imagePath);
}

bool EatLayer::init(){
    if(BaseStepLayer::init()){
        
        
        _operate = StudioLayer::create("EatLayer.csb");
        addChild(_operate);
       
        _eatSprite = EatSprite::create();
        _eatSprite->_soundPath = "sound/other/eat.mp3";
        _eatSprite->_touchCallBack = [this](){
            auto tap_eat = _operate->getNodeByName("tap_eat");
            _eatSprite->_touchCallBack = nullptr;
            tap_eat->stopAllActions();
            ActionHelper::hide(tap_eat,  ActionHelper::ShowDirection::show_from_left,[tap_eat](){tap_eat->setVisible(false);},false);
        };
        _imagePath = FileUtils::getInstance()->getWritablePath() + "eatFood.png";
        _eatSprite->setContentSize(_operate->getNodeByName("eatContiner")->getContentSize());
        _operate->getNodeByName("eatContiner")-> addChild(_eatSprite);
        auto  _s = Sprite::create(_imagePath);
        _s->setPosition(_eatSprite->getContentSize()*.5);
        _eatSprite->addSprite(_s);
        auto _doun = Sprite::create("png/6/donuts.png");
        _doun->setPosition(_operate->getNodeByName("eatContiner")->getContentSize()*.5);
        //_eatSprite->CloneSprite();
        _eatSprite->addDounSprite(_doun);
        _eatSprite->eatPercentageCallback = [this](float _percent){
            if(_percent>0.7){
                checkEnd();
            }
        };
        return true;
    }
    return false;
}

void EatLayer::checkEnd(){
    auto _eatAgain = _operate->getNodeByname<ui::Button*>("eat_again");
    if(_eatAgain!= nullptr&&!_eatAgain->isVisible()){
        ActionHelper::show(_eatAgain, _eatAgainPos, ActionHelper::ShowDirection::show_from_top);
        _eatAgain->setVisible(true);
        _eatAgain->setEnabled(true);
        // addChild(ParticleSystemQuad::create("particle/happyParticle.plist"),40);
        SoundPlayer::getInstance()->playEffect("sound/win.mp3");
    }
    
}

void EatLayer::touchEnd(ui::Widget* widge){
    if(widge->getName().compare("btn_pre") == 0){
        Director::getInstance()->popScene();
    } else if(widge->getName().compare("btn_camer") == 0){
        saveAbumle();
        SoundPlayer::getInstance()->playEffect("sound/camera.mp3");
    }
    else if(widge->getName().compare("btn_home") == 0){
        homeClick();
    } else if(widge->getName().compare("eat_again")==0){
        _eatSprite->reset();
        ActionHelper::hide(widge, ActionHelper::ShowDirection::show_from_bottom,[=](){widge->setVisible(false);},false);
        widge->setEnabled(false);
    }else if(widge->getName() == "btn_next"){
        Director::getInstance()->popToRootScene();
        auto scene =Scene::create();
        scene->addChild(VoteLayer::create());
        Director::getInstance()->replaceScene(scene);

    }
}

void  EatLayer::showEnd(){
    auto tap_eat = _operate->getNodeByName("tap_eat");
    ActionHelper::showBackInOut(tap_eat, tap_eat->getPosition(), ActionHelper::ShowDirection::show_from_top);
    tap_eat->setVisible(true);
 
}

 void EatLayer::onExitTransitionDidStart(){
     FlurryEventManager::getInstance()->logCurrentModuleExitEvent(_eventName);
       BaseStepLayer::onExitTransitionDidStart();
}

void EatLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(_eventName);
    if(_operate != nullptr){
        auto _next = _operate->getNodeByName("btn_next");
        if(_next != nullptr)
            _next->stopAllActions();
        
    }
    _operate->playAction("show",CC_CALLBACK_0(EatLayer::showEnd, this));
    _eatAgainPos = _operate->getNodeByName("eat_again")->getPosition();
    SoundPlayer::getInstance()->playEffect("sound/note_swish.mp3");
   
     _operate->getNodeByName("btn_camer") ->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.9, 1.1),ScaleTo::create(0.9, 1), nullptr)));
}

void EatLayer::saveAbumle(){
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    _operate->getNodeByName("rightTopCotainer")->setVisible(false);
    _operate->getNodeByName("leftTopCotainer")->setVisible(false);
    _operate->getNodeByName("centerCotatner")->setVisible(false);
    auto _size = Director::getInstance()->getVisibleSize();
    auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _render->begin();
    visit();
    _render->end();
    Director::getInstance()->getRenderer()->render();
    _operate->getNodeByName("rightTopCotainer")->setVisible(true);
    _operate->getNodeByName("leftTopCotainer")->setVisible(true);
    _operate->getNodeByName("centerCotatner")->setVisible(true);
    auto _layer = AblumLayer::create();
    
    addChild(_layer,30);
    _layer->setRender(_render);
}

void EatLayer::saveMyMenu() {
    auto _eatContiner =_operate->getNodeByName("eatContiner");
    auto _pos = _eatContiner->getPosition();
    auto _an = _eatContiner->getAnchorPoint();
    
    auto _bg = Sprite::create("ui/menu/menu-donutbg.png");
    _bg->setAnchorPoint(Vec2::ZERO);
    auto _size = _bg->getContentSize();
    _eatContiner->setPosition(_size*.5);
    _eatContiner->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _eatContiner->setScale(0.5);
    auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _render->begin();
    _bg->visit();
    _eatContiner->visit();
    _render->end();
    Director::getInstance()->getRenderer()->render();
    _eatContiner->setPosition(_pos);
    _eatContiner->setScale(1);
    _eatContiner->setAnchorPoint(_an);
    FilePathUtil::saveToFavorite(_render, nullptr, "DonutShopMenu");
}

void EatLayer::homeClick(){
    auto _dialog = MyDialog::create("Do you want to start over with a new game?", MyDialog::twoBtnPath);
    _dialog->dialogBtnClick =[this](MyDialog * dialog,int index){
        if(index == 0) {
            _operate->setIsChange(true);
            auto _endcallBack = [](){
                Director::getInstance()->popScene();
                LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                auto _scne = Scene::create();
                _scne->addChild(HomeLayer::create());
                Director::getInstance()->replaceScene(TransitionFade::create(0.8,_scne,Color3B::WHITE));
                //Director::getInstance()->replaceScene(TransitionFadeTR::create(1.5,_scne));
            };
            if(IAPManager::getInstance()->isShowAds()){
                LQ_adLoadingLayer::loadingDoneCallback = _endcallBack;
                LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
            } else
                _endcallBack();
            
        }
    };
    
    getScene()->addChild(_dialog);
}
