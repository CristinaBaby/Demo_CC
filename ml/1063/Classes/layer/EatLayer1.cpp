//
//  EatLayer1.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "EatLayer1.h"
#include "FilePathUtil.h"
#include "TouchNodeComponent.h"
#include "MyDialog.h"
#include "LQ_adLoadingLayer.h"
#include "AblumLayer.h"
#include "HomeLayer.h"
#include "ChooseFood.h"
#include "WeddingFoodData.h"
#include "ChangeLoading.h"
const string EatLayer1::_eventName = "Eat page";
REGIST_SECENE(EatLayer1);
EatLayer1::~EatLayer1(){
    
}

bool EatLayer1::init(){
    if(BaseStepLayer::init()){
        playIndex =0;
        _operate = StudioLayer::create("EatLayer1.csb");
        addChild(_operate);
        
        vector<string> btnName = {"btn_home","btn_camer","btn_next", "btn_pre","tap_eat"};
        for(auto name:btnName)
            _operate->getNodeByName(name)->setVisible(false);
        
        check = CheckEatPercent::create();
        addComponent(check);
        check->setEnabled(false);
        check->eatPercentageCallback = [this](float f){
            if(f>0.45)
                checkEnd();
        };
        
        
        
        
        
        auto platePatsh = WeddingFoodData::getAllPlatePath();
        auto eatPath = WeddingFoodData::getAllEatPath();
        for(int i=0;i<3;i++){
            if(!platePatsh[i].empty()){
                dynamic_cast<Sprite*>(ui::Helper::seekNodeByName(_operate->getNodeByName(StringUtils::format("food%d",i)),"plates"))->setTexture(platePatsh[i]);
            }
            
            auto foodC = ui::Helper::seekNodeByName(_operate->getNodeByName(StringUtils::format("food%d",i)),"food");
              dynamic_cast<ui::Widget*>(ui::Helper::seekNodeByName(_operate->getNodeByName(StringUtils::format("food%d",i)),"food_0"))->setEnabled(false);
            auto food = EatSprite::create();
            food->setCheckEnable(false);
            food->setEnable(false);
            food->setPosition(foodC->getContentSize()*.5);
            auto s = CocosHelper::spriteCreateWithNoCache(eatPath.at(i).c_str());
            food->addSprite(s);
            food->_soundPath = "sound/eat.mp3";
            foodC->addChild(food);
            foods.pushBack(food);
            
        }
        return true;
    }
    return false;
}

void EatLayer1::checkEnd(){
    auto _eatAgain = _operate->getNodeByName<ui::Button*>("eat_again");
    if(_eatAgain!= nullptr&&!_eatAgain->isVisible()){
        // addChild(ParticleLayer::create(),30);
          _operate->playNodeSound(_eatAgain->getName());
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

void EatLayer1::touchEnd(ui::Widget* widge){
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
        ChangeLoading::loading([](){
            Director::getInstance()->replaceScene(TransitionFade::create(0.8,ChooseFood::createScene(),Color3B::WHITE));
        });
    }
}

void  EatLayer1::showEnd(){
    auto tap_eat = _operate->getNodeByName("tap_eat");
    ActionHelper::showBackInOut(tap_eat, tap_eat->getPosition(), ActionHelper::ShowDirection::show_from_top);
    tap_eat->setVisible(true);
    
}

void EatLayer1::onExitTransitionDidStart(){
    BaseStepLayer::onExitTransitionDidStart();
}

void EatLayer1::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
     _operate->playNodeSound("eat_again");
     SoundPlayer::getInstance()->playBackGroundMusic("sound/rock-that-party.mp3");
    _operate->getNodeByName("btn_next")->stopAllActions();
    _eatAgainPos = _operate->getNodeByName("eat_again")->getPosition();
    SoundPlayer::getInstance()->playEffect("sound/note_swish.mp3");
    //  _operate->getNodeByName("btn_camer") ->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.9, 1.1),ScaleTo::create(0.9, 1), nullptr)));
    
    for(int i=0;i<foods.size();i++){
        
        ActionHelper::delayFunc(0.5*i, this, [this,i](){
            
            SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
            auto eatCotainer = _operate->getNodeByName(StringUtils::format("food%d",i));
            ActionHelper::showBezier(eatCotainer, eatCotainer->getPosition(), ActionHelper::ShowDirection::show_from_left,[this,i](){
                if(i == foods.size()-1){
                    vector<string> btnName = {"btn_home","btn_camer","tap_eat"};
                    vector<ActionHelper::ShowDirection> der = {ActionHelper::ShowDirection::show_from_left,ActionHelper::ShowDirection::show_from_right,ActionHelper::ShowDirection::show_from_top};
                    for(int i=0;i<btnName.size();i++){
                        auto btn = _operate->getNodeByName(btnName[i]);
                        if(i == btnName.size()-1){
                            ActionHelper::delayFunc(0.3, this, [=](){
                                 SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
                                ActionHelper::showBackInOut(btn, btn->getPosition(), der[i],[=](){
                                    foods.at(0)->_touchCallBack = [=](){
                                        auto tap_eat = _operate->getNodeByName("tap_eat");
                                        foods.at(0)->_touchCallBack = nullptr;
                                        tap_eat->stopAllActions();
                                        ActionHelper::hide(tap_eat,  ActionHelper::ShowDirection::show_from_top,[tap_eat](){tap_eat->setVisible(false);},false);
                                    };
                                    
                                    for(int i=0;i<foods.size();i++) {
                                        auto f = foods.at(i);
                                        auto s = WeddingFoodData::createEatDonwFood(i);
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
                }
            });
            eatCotainer->setVisible(true);
        });
    }
    vector<string> plateNames = {"Animation2","Animation3","Animation4","Animation5"};
    this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.5),CallFunc::create([this,plateNames](){
        playIndex++;
        playIndex = playIndex%4;
        _operate->getNodeByName<cocostudio::Armature*>("people")->getAnimation()->play(plateNames[playIndex],-1,0);
    }), nullptr)));
}

void EatLayer1::saveAbumle(){
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


void EatLayer1::homeClick(){
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


