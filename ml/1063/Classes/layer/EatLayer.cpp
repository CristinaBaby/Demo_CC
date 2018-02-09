//
//  EatLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "EatLayer.h"
#include "FilePathUtil.h"
#include "TouchNodeComponent.h"
#include "MyDialog.h"
#include "LQ_adLoadingLayer.h"
#include "AblumLayer.h"
#include "HomeLayer.h"
#include "ChooseFood.h"
#include "WeddingFoodData.h"
#include "EatLayer1.h"
const string EatLayer::_eventName = "Eat page";
REGIST_SECENE(EatLayer);
EatLayer::~EatLayer(){
    
}

bool EatLayer::init(){
    if(BaseStepLayer::init()){
        
        _operate = StudioLayer::create("EatLayer0.csb");
        addChild(_operate);
        
        vector<string> btnName = {"btn_home","btn_camer","btn_next", "btn_pre","tap_eat"};
        for(auto name:btnName)
            _operate->getNodeByName(name)->setVisible(false);
        
        check = CheckEatPercent::create();
        addComponent(check);
        check->setEnabled(false);
        check->eatPercentageCallback = [this](float f){
            if(f>0.6)
                checkEnd();
        };
        
        auto foodCotainer = _operate->getNodeByName("foodCotainer");
        auto layer = StudioLayer::createNoLayout(WeddingFoodData::getEatLayerPath());
        foodCotainer->addChild(layer);
        foodCotainer->setVisible(false);
        auto foodC = layer->getNodeByName("food");
        
        auto food = EatSprite::create();
        food->_soundPath = "sound/eat.mp3";
        food->setPosition(foodC->getContentSize()*.5);
        auto s = CocosHelper::spriteCreateWithNoCache(WeddingFoodData::getEatPath().c_str());
        food->addSprite(s);
        food->setEnable(false);
        foodC->addChild(food);
        foods.pushBack(food);
        food->setCheckEnable(false);
        if(!WeddingFoodData::getPlatePath().empty())
            _operate->changeNode("plates", WeddingFoodData::getPlatePath());
        if(!WeddingFoodData::getForksPath().empty()){
            _operate->changeNode("forks", WeddingFoodData::getForksPath());
            _operate->getNodeByName("forks")->setVisible(true);
        }
        return true;
    }
    return false;
}

void EatLayer::checkEnd(){
    auto _eatAgain = _operate->getNodeByName<ui::Button*>("eat_again");
    if(_eatAgain!= nullptr&&!_eatAgain->isVisible()){
        _operate->playNodeSound(_eatAgain->getName());
        // addChild(ParticleLayer::create(),30);
        ActionHelper::showBackInOut(_eatAgain, _eatAgainPos, ActionHelper::ShowDirection::show_from_top);
        _eatAgain->setVisible(true);
        _eatAgain->setEnabled(true);
        auto next = _operate->getNodeByName("btn_next");
        ActionHelper::showBackInOut(next, next->getPosition(), ActionHelper::ShowDirection::show_from_right);
        next->setVisible(true);
        _eatAgain->setVisible(true);
        _eatAgain->setEnabled(true);
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
        for(auto _eatSprite:foods)
            _eatSprite->reset();
        
        check->reset();
        ActionHelper::hide(widge, ActionHelper::ShowDirection::show_from_bottom,[=](){widge->setVisible(false);},false);
        widge->setEnabled(false);
    }else if(widge->getName().compare("btn_next") == 0){
        Director::getInstance()->popToRootScene();
        if(WeddingFoodData::checkPlayAllFoods())
            Director::getInstance()->replaceScene(TransitionFade::create(0.8,EatLayer1::createScene(),Color3B::WHITE));
        else
            Director::getInstance()->replaceScene(TransitionFade::create(0.8,ChooseFood::createScene(),Color3B::WHITE));
    }
}

void  EatLayer::showEnd(){
    auto tap_eat = _operate->getNodeByName("tap_eat");
    ActionHelper::showBackInOut(tap_eat, tap_eat->getPosition(), ActionHelper::ShowDirection::show_from_top);
    tap_eat->setVisible(true);
    
}

void EatLayer::onExitTransitionDidStart(){
    BaseStepLayer::onExitTransitionDidStart();
}

void EatLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    _operate->getNodeByName("btn_next")->stopAllActions();
    _eatAgainPos = _operate->getNodeByName("eat_again")->getPosition();
    SoundPlayer::getInstance()->playEffect("sound/note_swish.mp3");
  //  _operate->getNodeByName("btn_camer") ->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.9, 1.1),ScaleTo::create(0.9, 1), nullptr)));
    auto eatCotainer = _operate->getNodeByName("foodCotainer");
    SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
    ActionHelper::showBezier(eatCotainer, eatCotainer->getPosition(), ActionHelper::ShowDirection::show_from_left,[this](){
        vector<string> btnName = {"btn_home","btn_camer","btn_pre","tap_eat"};
         SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
        vector<ActionHelper::ShowDirection> der = {ActionHelper::ShowDirection::show_from_left,ActionHelper::ShowDirection::show_from_right,ActionHelper::ShowDirection::show_from_left,ActionHelper::ShowDirection::show_from_top};
        for(int i=0;i<btnName.size();i++){
            auto btn = _operate->getNodeByName(btnName[i]);
            if(i == btnName.size()-1){
                ActionHelper::delayFunc(0.3, this, [=](){
                    ActionHelper::showBackInOut(btn, btn->getPosition(), der[i],[=](){
                        foods.at(0)->_touchCallBack = [=](){
                            auto tap_eat = _operate->getNodeByName("tap_eat");
                            foods.at(0)->_touchCallBack = nullptr;
                            tap_eat->stopAllActions();
                            ActionHelper::hide(tap_eat,  ActionHelper::ShowDirection::show_from_top,[tap_eat](){tap_eat->setVisible(false);},false);
                        };
                        for(auto f:foods){
                            auto s = WeddingFoodData::createEatDonwFood(WeddingFoodData::getFoodIndex());
                            f->addDownSprite(s);
                            f->setEnable(true);
                            check->addCheckNode(f);
                        }
                        check->setEnabled(true);
                    });
                    btn->setVisible(true);
                });
                
            }else {
                ActionHelper::showBackInOut(btn, btn->getPosition(), der[i]);
                btn->setVisible(true);
            }
            
        }
    });
    eatCotainer->setVisible(true);
}

void EatLayer::saveAbumle(){
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    _operate->getNodeByName("uiLayer")->setVisible(false);
    _operate->getNodeByName("centerCotatner")->setVisible(false);
    auto _size = Director::getInstance()->getVisibleSize();
    auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _render->begin();
    visit();
    _render->end();
    Director::getInstance()->getRenderer()->render();
    _operate->getNodeByName("uiLayer")->setVisible(true);
    _operate->getNodeByName("centerCotatner")->setVisible(true);
    auto _layer = AblumLayer::create();
    addChild(_layer,30);
    _layer->setRender(_render);
    SoundPlayer::getInstance()->playEffect("sound/general/pop_up_open.mp3");
}


void EatLayer::homeClick(){
    auto _dialog = MyDialog::create("Do you want to start over with a new game?", MyDialog::twoBtnPath);
    _dialog->dialogBtnClick =[this](MyDialog * dialog,int index){
        if(index == 0) {
            _operate->setIsChange(true);
            auto _endcallBack = [](){
                LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                Director::getInstance()->popToRootScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.8,HomeLayer::createScene(),Color3B::WHITE));
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


