//
//  HomeLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#include "HomeLayer.h"
#include "LQ_adLoadingLayer.h"
#include "EventPromotLayer.h"
#include "ChangeLayer.h"
#include "MyDialog.h"
#include "ChooseLayer.h"
IMPLEMENT_COCOS2DX_CLASS(HomeLayer);

bool HomeLayer::init(){
    BaseHomeLayer::init();
    startClass = "ChooseLayer";
    _operate = StudioLayer::create("HomLayer.csb");
    addChild(_operate);
    if(Director::getInstance()->getOpenGLView()->getFrameSize().width>=2048){
        _operate->getNodeByName("bg_1")->getChildByName("highNode")->addChild(Sprite::create("png/0/bg_h.jpg"));
        _operate->getNodeByName("lamp0")->getChildByName("highNode")->addChild(Sprite::create("png/0/lamp0_h.png"));
        _operate->getNodeByName("lamp1")->getChildByName("highNode")->addChild(Sprite::create("png/0/lamp1_h.png"));
    }
   
    
    this->runAction(RepeatForever::create(Sequence::create(CallFunc::create([this](){
        _operate->getNodeByName("lamp0")->setVisible(false);
        _operate->getNodeByName("lamp1")->setVisible(true);
    }),DelayTime::create(0.3),CallFunc::create([this](){
        _operate->getNodeByName("lamp0")->setVisible(true);
        _operate->getNodeByName("lamp1")->setVisible(false);
    }), DelayTime::create(0.3),nullptr)));
    return true;
}

void HomeLayer::playClick(){
    auto widget = _operate->getNodeByName("btn_play");
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    widget->stopAllActions();
    widget->setScale(1);
    SoundPlayer::getInstance()->playEffect("/sound/Sound/play button_jump.mp3", 0.5);
    widget->runAction(Sequence::create(JumpBy::create(1, Vec2::ZERO, 20, 2),CallFunc::create([](){
        SoundPlayer::getInstance()->playEffect("sound/Sound/play button_spin magnify.mp3");
    }), Spawn::create(ScaleTo::create(1, 0), RotateBy::create(1, 720), nullptr),CallFunc::create([=](){
        SoundPlayer::getInstance()->playEffect("sound/general/ingredients_out.mp3");
        _operate->getNodeByName("btn_shop")->runAction(EaseBackIn::create(MoveBy::create(0.6, Vec2(-500,0))));
        _operate->getNodeByName("btn_more")->runAction(EaseBackIn::create(MoveBy::create(0.6, Vec2(500,0))));
    }),DelayTime::create(0.6), CallFunc::create([=](){
        _operate->getNodeByName("uiLayer")->setVisible(false);
        auto _callfunc = [=](){
            AdLoadingLayerBase::loadingDoneCallback = nullptr;
            SoundPlayer::getInstance()->playEffect(startEffect);
              Director::getInstance()->replaceScene(TransitionFade::create(0.8, ChooseLayer::createScene(),Color3B::WHITE));
            Director::getInstance()->getEventDispatcher()->setEnabled(true);
        };
        if(IAPManager::getInstance()->isShowAds()){
            AdLoadingLayerBase::loadingDoneCallback = _callfunc;
            LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(false);
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
        } else
            _callfunc();
    }), nullptr));
}

void HomeLayer::showAdsCallBack(){
    LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
    AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
}
void HomeLayer::touchEnd(ui::Widget* widget){
    BaseHomeLayer::touchEnd(widget);
    if(widget->getName().find("food") != string::npos)
        widget->runAction(JumpBy::create(1, Vec2::ZERO, 15, 2));
    
    
}
void  HomeLayer::onEnterTransitionDidFinish(){
    BaseHomeLayer::onEnterTransitionDidFinish();
    Vec2 starpos[] = {Vec2(41.56,215.71),Vec2(458.43,202.31),Vec2(689.91,213.43)};
    for(int i=0;i<3;i++){
        auto _food = _operate->getNodeByName(StringUtils::format("food%d",i));
        _food->setScale(0.5);
        ActionHelper::delayFunc(0.5*i, _food, [=](){
                SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
            ccBezierConfig config;
            config.endPosition = _food->getPosition();
            config.controlPoint_1 = config.controlPoint_2 = Vec2((starpos[i].x+_food->getPosition().x)*.5,std::max(_food->getPosition().y,starpos[i].y)+100);
            _food->setPosition(starpos[i]);
            _food->runAction(ScaleTo::create(0.7, 1));
            _food->runAction(Sequence::create(BezierTo::create(0.7, config),CallFunc::create([=](){
                if(i==2) {
                    auto logo = _operate->getNodeByName("logo");
                    logo->setVisible(true);
                        SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                    ActionHelper::showBackInOut(logo, logo->getPosition(), ActionHelper::ShowDirection::show_from_top,[this](){
                            SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                        auto _btnShop= _operate->getNodeByName("btn_shop");
                        auto _pos = _btnShop->getPosition();
                        ActionHelper::setStartPostion(_btnShop, _pos, ActionHelper::ShowDirection::show_from_bottom);
                        _btnShop->runAction(JumpTo::create(0.5, _pos,300,1));
                        auto _btnMore= _operate->getNodeByName("btn_more");
                        _pos = _btnMore->getPosition();
                        ActionHelper::setStartPostion(_btnMore, _pos, ActionHelper::ShowDirection::show_from_bottom);
                        _btnMore->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([_btnMore](){_btnMore->setVisible(true);}), JumpTo::create(0.5, _pos, 300, 1),nullptr));
                        auto _btnPlay= _operate->getNodeByName("btn_play");
                        _pos = _btnPlay->getPosition();
                        ActionHelper::setStartPostion(_btnPlay, _pos, ActionHelper::ShowDirection::show_from_bottom);
                        _btnPlay->runAction(Sequence::create(DelayTime::create(0.2),  CallFunc::create([_btnPlay](){_btnPlay->setVisible(true);}),JumpTo::create(0.5, _pos, 400, 1),CallFunc::create([this](){
                            auto _EventPromotLayer = EventPromotLayer::create();
                            _EventPromotLayer->setTime(4);
                            addChild(_EventPromotLayer,50);
                            _EventPromotLayer->setCallbakc([this](){
                                string _names[] = {"food0","food1","food2"};
                                auto _sinze = sizeof(_names)/sizeof(_names[0]);
                                auto _node = _operate->getNodeByname<ui::Widget*>(_names[rand()%_sinze]);
                                if(_node != nullptr)
                                    touchEnd(_node);
                            });
                        }) ,nullptr));
                        _btnShop->setVisible(true);
                    });
                }
            }), nullptr));
            ActionHelper::delayFunc(0.4, this, [=](){_food->getParent()->reorderChild(_food, 4-i);});
            _food->setVisible(true);
        });
    }
  

   
}