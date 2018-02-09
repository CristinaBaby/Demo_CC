//
//  EatLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "EatLayer.h"
#include "DecorationLayer.h"
#include "FilePathUtil.h"
#include "TouchNodeComponent.h"
#include "MyDialog.h"
#include "LQ_adLoadingLayer.h"
#include "AblumLayer.h"
#include "HomeLayer.h"
#include "ParticleLayer.h"
#include "ChooseLayer.h"
USING_NS_KD;
const string EatLayer::_eventName = "Eat page";

IMPLEMENT_COCOS2DX_CLASS(EatLayer);
EatLayer::~EatLayer(){
    
}

bool EatLayer::init(){
    if(BaseStepLayer::init()){
        
        _operate = StudioLayer::create("EatLayer.csb");
        addChild(_operate);
        drink = nullptr;
        touchLis = nullptr;
        string names[] = {"plate","cup","sauce_bowl","side-dishplate"};
        auto bowl_up = _operate->getNodeByName("sauce_up");
        auto cup_up = _operate->getNodeByName("cup_up");
        bowl_up->getParent()->reorderChild(bowl_up, 1);
        cup_up->getParent()->reorderChild(cup_up, 1);
        for(int i=0;i<sizeof(names)/sizeof(names[0]);i++)
            _operate->getNodeByName(names[i])->setVisible(false);
        
        check = CheckEatPercent::create();
        addComponent(check);
        check->setEnabled(false);
        check->eatPercentageCallback = [this](float f){
            if(f>0.8)
                checkEnd();
        };
        return true;
    }
    return false;
}

void EatLayer::checkEnd(){
    auto _eatAgain = _operate->getNodeByname<ui::Button*>("eat_again");
    if(_eatAgain!= nullptr&&!_eatAgain->isVisible()){
        addChild(ParticleLayer::create(),30);
        ActionHelper::show(_eatAgain, _eatAgainPos, ActionHelper::ShowDirection::show_from_top);
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
        if(drink != nullptr)
            drink->resetScrSprite();
        check->reset();
        if(touchLis != nullptr)
            touchLis->setEnabled(true);
        ActionHelper::hide(widge, ActionHelper::ShowDirection::show_from_bottom,[=](){widge->setVisible(false);},false);
        widge->setEnabled(false);
    }else if(widge->getName().compare("btn_next") == 0){
        Director::getInstance()->popToRootScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.8,ChooseLayer::createScene(),Color3B::WHITE));
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
    _operate->getNodeByName("btn_camer") ->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.9, 1.1),ScaleTo::create(0.9, 1), nullptr)));
    for(int i=0;i<showNodes.size();i++){
        ActionHelper::delayFunc(0.3*i, this, [=](){
              SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
            ActionHelper::showBezier(showNodes.at(i), showNodes.at(i)->getPosition(), ActionHelper::ShowDirection::show_from_left,[=](){
                if(i == showNodes.size()-1){
                    
                    string btnName[] = {"btn_home","btn_camer","btn_pre","btn_next","tap_eat"};
                    ActionHelper::ShowDirection der[] = {ActionHelper::ShowDirection::show_from_left,ActionHelper::ShowDirection::show_from_right,ActionHelper::ShowDirection::show_from_top,ActionHelper::ShowDirection::show_from_top,ActionHelper::ShowDirection::show_from_top};
                    for(int j=0;j<5;j++){
                        auto btn = _operate->getNodeByName(btnName[j]);
                        if(j == 4){
                            ActionHelper::delayFunc(0.3, this, [=](){
                                ActionHelper::showBackInOut(btn, btn->getPosition(), der[j],[=](){
                                    foods.at(0)->_touchCallBack = [=](){
                                        auto tap_eat = _operate->getNodeByName("tap_eat");
                                        foods.at(0)->_touchCallBack = nullptr;
                                        tap_eat->stopAllActions();
                                        ActionHelper::hide(tap_eat,  ActionHelper::ShowDirection::show_from_left,[tap_eat](){tap_eat->setVisible(false);},false);
                                    };
                                    for(auto f:foods){
                                        f->setEnable(true);
                                        f->CloneSprite();
                                    }
                                            check->setEnabled(true);
                                    if(drink != nullptr){
                                        touchLis = TouchNodeComponent::create();
                                        touchLis->setSwallowTouches(true);
                                        drink->addComponent(touchLis);
                                        touchLis->addListners(ComponentTouchBegan, [this](Node*,Component*, OperateListner* _lis){
                                            drink->resumAction();
                                            if(_loopSound == -1)
                                                _loopSound = SoundPlayer::getInstance()->playEffectLoop("sound/general/drink.mp3");
                                        });
                                        auto endFunc = [this](Node*,Component*, OperateListner* _lis){
                                            drink->pasuAction();
                                            if(_loopSound != -1){
                                                SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
                                                _loopSound = -1;
                                            }
                                        };
                                        touchLis->addListners(ComponentTouchEnd, endFunc);
                                        touchLis->addListners(ComponentTouchCancle, endFunc);
                                        drink->endCallback = [this](KDS::ShadeSprite*){
                                            touchLis->setEnabled(false);
                                            drink->getSrcSprite()->setPositionY(drink->getSrcSprite()->getPositionY()-25);
                                            if(_loopSound != -1){
                                                SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
                                                _loopSound = -1;
                                            }
                                            SoundPlayer::getInstance()->playEffect("sound/Sound/ahaa.mp3");
                                        };
                                    }
                                });
                                btn->setVisible(true);
                            });
                            
                        }else {
                            ActionHelper::showBackInOut(btn, btn->getPosition(), der[j]);
                            btn->setVisible(true);
                        }
                    }
                }
            });
            showNodes.at(i)->setVisible(true);
        });
    }
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
    SoundPlayer::getInstance()->playEffect("sound/general/pop_up_open.mp3");
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
    FilePathUtil::saveToFavorite(_render, nullptr, "dountshopMenu");
}

void EatLayer::homeClick(){
    auto _dialog = MyDialog::create("Do you want to start over with a new game?", MyDialog::twoBtnPath);
    _dialog->dialogBtnClick =[this](MyDialog * dialog,int index){
        if(index == 0) {
            _operate->setIsChange(true);
            auto _endcallBack = [](){
                Director::getInstance()->popScene();
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

void EatLayer::setFoods(const Map<string,Node*>& foodsCache){
    auto keys = foodsCache.keys();
    for(int i=0;i<keys.size();i++){
        auto node = foodsCache.at(keys.at(i));
        auto newNode = Sprite::createWithSpriteFrame(dynamic_cast<Sprite*>(node)->getSpriteFrame());
        if(keys.at(i) == "plate"){
            _operate->changeNode(keys.at(i), newNode->getSpriteFrame());
        }else if(keys.at(i) == "drink"){
            auto cup = _operate->getNodeByName("cup");
            auto src = Sprite::createWithSpriteFrame(newNode->getSpriteFrame());
            src->setAnchorPoint(Vec2(0,0));
            auto mask = Sprite::createWithSpriteFrame(newNode->getSpriteFrame());
            mask->setAnchorPoint(Vec2(0,0));
            drink = ShadeSprite::create(src, mask);
            drink->setDistance(141);
            drink->setPosition(cup->getContentSize()*.5);
            cup->addChild(drink);
            showNodes.pushBack(cup);
            check->addCheckNode(drink);
        } else {
            string plateName;
            if(keys.at(i) == "food")
                plateName = "plate";
            else if(keys.at(i) == "sauce")
                  plateName = "sauce_bowl";
            else
                plateName = "side-dishplate";
            newNode->setPosition(node->getContentSize()*.5);
            auto _eatSprite = EatSprite::create();
            _eatSprite->setName(keys.at(i));
            _eatSprite->setContentSize(newNode->getContentSize());
            _eatSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _eatSprite->addSprite(newNode);
            _eatSprite->setCheckEnable(false);
            _eatSprite->_soundPath = "sound/other/eat.mp3";
            _eatSprite->setEnable(false);
            auto downNoe  = _operate->getNodeByName(plateName);
            _eatSprite->setPosition(node->getPosition());
            downNoe->addChild(_eatSprite);
            check->addCheckNode(_eatSprite);
            foods.pushBack(_eatSprite);
            showNodes.pushBack(downNoe);
        }
    }
    
}
