//
//  HomeLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#include "HomeLayer.hpp"
#include "LQ_adLoadingLayer.h"
#include "MyMenu.h"
#include "MakeSignature.h"
#include "MakeStep1.h"
#include "EventPromotLayer.h"
#include "ChangeLayer.h"
#include "AblumLayer.h"
#include "MyDialog.h"
IMPLEMENT_COCOS2DX_CLASS(HomeLayer);
HomeLayer::HomeLayer(){
    autoWriteLogo = false;
}
bool HomeLayer::init(){
    BaseHomeLayer::init();
    _operate = StudioLayer::create("HomeLayer.csb");
    addChild(_operate);
    if(Director::getInstance()->getOpenGLView()->getFrameSize().width>=2048)
        _operate->getNodeByName("highNode")->addChild(Sprite::create("png/1/shop4.jpg"));
    
    
    if(Director::getInstance()->getVisibleSize().height<670)
        _operate->getNodeByName("shed")->setVisible(false);
    _operate->getNodeByName("balloon")->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.8, -15), RotateBy::create(1.6, 30), RotateBy::create(0.8, -15), nullptr)));
    auto _path = FileUtils::getInstance()->getWritablePath() + "MakeSignature.png";
    if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->fullPathForFilename(_path)))
        _operate->changeNode("title", _path);
    auto _string = UserDefault::getInstance()->getStringForKey("bgSprite");
    if(!_string.empty())
        _operate->changeNode("title_bg", _string);
    _operate->getNodeByName("title")->setVisible(false);
    
    
    
    return true;
}

void  HomeLayer::onExitTransitionDidStart(){
    BaseHomeLayer::onExitTransitionDidStart();
}

void HomeLayer::playClick(){
    _operate->getNodeByName("btn_rate")->setVisible(false);
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
            _operate->addComponent(MakeSignature::create());
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
    //    LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
    //    AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
}
void HomeLayer::touchEnd(ui::Widget* widget){
    BaseHomeLayer::touchEnd(widget);
    
    if(widget->getName() == "btn_reset"){
        auto _dialgo = MyDialog::create("Do you want to reset your Donut Shop", MyDialog::twoBtnPath);
        _dialgo->dialogBtnClick = [this](MyDialog * dialog,int _touchInd){
            if(_touchInd == 0){
                auto _path = FileUtils::getInstance()->getWritablePath() + "MakeSignature.png";
                _path = FileUtils::getInstance()->fullPathForFilename(_path);
                std::remove(_path.c_str());
                UserDefault::getInstance()->deleteValueForKey("bgSprite");
                
                for(int i=0;i<=7;i++){
                    UserDefault::getInstance()->deleteValueForKey(StringUtils::format("price%d",i).c_str());
                    UserDefault::getInstance()->deleteValueForKey(StringUtils::format("path%d",i).c_str());
                    UserDefault::getInstance()->flush();
                }
                auto _homeLayer = HomeLayer::create();
                auto _scene = Scene::create();
                _homeLayer->getOperateLayer()->getNodeByName("btn_rate")->setVisible(false);
                _homeLayer->getOperateLayer()->getNodeByName("btn_reset")->setVisible(true);
                _homeLayer->getOperateLayer()->getNodeByName("btn_next")->setVisible(true);
                _homeLayer->setAutoWriteLogo(true);
                _scene->addChild(_homeLayer);
                Director::getInstance()->replaceScene(_scene);
            }
        };
        getScene()->addChild(_dialgo);
        
        
    }else if(widget->getName() == "myMenu"){
        SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
        auto _myMenu = MyMenu::create();
        getScene()->addChild(_myMenu);
        _myMenu->show(widget->convertToWorldSpace(widget->getContentSize()*.5));
    }else if(widget->getName() == "btn_play"){
        
        
    }else if(widget->getName().find("rote") != string::npos){
        widget->stopAllActions();
        
        widget->runAction(ActionHelper::createRoteAction());
    }else if(widget->getName().find("donut") != string::npos){
        widget->stopAllActions();
        ActionInterval *ret = NULL;
        float timeScale = 1.2;
        ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
        ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
        ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
        ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
        ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
        ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
        ret = Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
        widget->runAction(ret);
    }else if(widget->getName() == "balloon"){
        // SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
        if(widget->getOpacity() == 0)
            return;
        widget->setEnabled(false);
        widget->stopAllActions();
        widget->setRotation(0);
        widget->setOpacity(0);
        auto _pos = widget->getPosition();
        widget->runAction(Sequence::create(DelayTime::create(6),MoveBy::create(0, Vec2(0,-1300)),FadeTo::create(0, 255),CallFunc::create([widget](){
            RotateBy *lRotate1 = RotateBy::create(0.5, 10);
            RotateBy *lRotate2 = RotateBy::create(0.5, -10);
            Sequence *lSeq1 = Sequence::create(lRotate1, lRotate2, nullptr);
            Repeat *lRepeat = Repeat::create(lSeq1, 20);
            DelayTime *lDelay1 = DelayTime::create(0.1);
            Sequence *lSeq2 = Sequence::create(lDelay1, lRepeat, nullptr);
            widget->runAction(RepeatForever::create(lSeq2));
        }), MoveTo::create(2, _pos),CallFunc::create([widget](){
            widget->stopAllActions();
            widget->setRotation(0);
            widget->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.8, -15), RotateBy::create(1.6, 30), RotateBy::create(0.8, -15), nullptr)));
            widget->setEnabled(true);
        }), nullptr));
        RotateBy *lRotate1 = RotateBy::create(0.5, 10);
        RotateBy *lRotate2 = RotateBy::create(0.5, -10);
        Sequence *lSeq1 = Sequence::create(lRotate1, lRotate2, nullptr);
        Repeat *lRepeat = Repeat::create(lSeq1, 20);
        DelayTime *lDelay1 = DelayTime::create(0.1);
        Sequence *lSeq2 = Sequence::create(lDelay1, lRepeat, nullptr);
        for(int i=0;i<=2;i++){
            auto _b = _operate->getNodeByname<Sprite*>(StringUtils::format("balloon%d",i));
            auto _s = Sprite::createWithSpriteFrame(_b->getSpriteFrame());
            _s->setPosition(_b->convertToWorldSpace(_b->getContentSize()*.5));
            addChild(_s,25);
            _s->runAction(RepeatForever::create(lSeq2->clone()));
            Vec2 moveByPos = Vec2(0, 800);
            if(i == 0)
                moveByPos = Vec2(300,800);
            else if(i==2)
                moveByPos = Vec2(-300,800);
            _s->runAction(Sequence::create(MoveBy::create(4+rand()%1, moveByPos),CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, _s)), nullptr));
        }
    } else if(widget->getName() == "showButterfly"){
        SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
        auto _upNode = _operate->getNodeByName("Sprite_11");
        _upNode->stopAllActions();
        widget->stopAllActions();
        auto _action = Sequence::create(ScaleTo::create(0.15, 1,1.05),ScaleTo::create(0.3, 1,0.95),ScaleTo::create(0.15, 1,1), nullptr);
        widget->runAction(_action);
        _upNode->runAction(_action->clone());
        auto _bf = _operate->getNodeByName("butterfly");
        int _index = 1+ rand()%4;
        auto _s = Sprite::create(StringUtils::format("png/1/extras%d_1.png",_index));
        _s->setFlippedX(true);
        _s->setScale(_bf->getScale());
        _s->setPosition(_bf->getPosition());
        _bf->getParent()->addChild(_s);
        float speed = 300.0f;
        Vector<cocos2d::FiniteTimeAction *> arrayOfActions;
        Vec2 _prePos = _bf->getPosition()+Vec2(0,100);
        arrayOfActions.pushBack(MoveBy::create(100/speed, Vec2(0,100)));
        for(int i=0;i<6;i++){
            auto _pos = Vec2(_prePos.x-(150+rand()%80),rand()%640);
            arrayOfActions.pushBack(MoveTo::create(_pos.distance(_prePos)/speed, _pos));
            _prePos = _pos;
        }
        arrayOfActions.pushBack(CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, _s)));
        _s->runAction(Sequence::create(arrayOfActions));
        auto  _animation =Animation::create();
        _animation->setLoops(-1);
        _animation->setDelayPerUnit(0.15);
        _animation->addSpriteFrame(_s->getSpriteFrame());
        _animation->addSpriteFrameWithFile(StringUtils::format("png/1/extras%d.png",_index));
        _s->runAction(Animate::create(_animation));
    }else if(widget->getName() == "btn_next"){
        ChangeLayer::_changeLayer = [](){
            return MakeStep1::create();
        };
        ChangeLayer::loading();
    }
}
void  HomeLayer::onEnterTransitionDidFinish(){
    BaseHomeLayer::onEnterTransitionDidFinish();
    if(_operate != nullptr){
        auto _next = _operate->getNodeByName("btn_next");
        if(_next != nullptr&&_next->isVisible()){
            _next->stopAllActions();
        }
    }
    SoundPlayer::getInstance()->playBackGroundMusic("sound/general/background.mp3");
    
    auto _EventPromotLayer = EventPromotLayer::create();
    _EventPromotLayer->setTime(2.5);
    addChild(_EventPromotLayer,50);
    _EventPromotLayer->setCallbakc([this](){
        string _names[] = {"rote0","rote1","rote2","donut","showButterfly","donut_0","donut_1","donut_2","showButterfly","donut_3","donut_4","donut_5","showButterfly"};
        auto _sinze = sizeof(_names)/sizeof(_names[0]);
        auto _node = _operate->getNodeByname<ui::Widget*>(_names[rand()%_sinze]);
        if(_node != nullptr)
            touchEnd(_node);
    });
    
    
    auto _dountsString = UserDefault::getInstance()->getStringForKey("path0","");
    if(!_dountsString.empty()){
        for(int i=0;i<=7;i++){
            auto _limit = _operate->getNodeByName(StringUtils::format("limit_%d",i));
            auto _path = UserDefault::getInstance()->getStringForKey(StringUtils::format("path%d",i).c_str(), "");
            if(_path.find("/") == string::npos)
                _path = FileUtils::getInstance()->getWritablePath() + _path;
            auto _node = Sprite::create(_path);
            if(_node == nullptr)
                _node = Sprite::create(StringUtils::format("png/donuts/donuts%d.png",rand()%20));
            _node->setScale(_limit->getContentSize().width/_node->getContentSize().width);
            _node->setPosition(_limit->getContentSize()*.5);
            _limit->addChild(_node,-1);
            dynamic_cast<ui::Text*>(ui::Helper::seekNodeByName(_limit, "Text"))->setString(UserDefault::getInstance()->getStringForKey(StringUtils::format("price%d",i).c_str(), ""));
            _limit->runAction(Sequence::create(DelayTime::create(0.15*i),CallFunc::create([_limit,i](){
                //if(i%2==0)
                SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                _limit->setVisible(true);
            }), MoveTo::create(0, Vec2(_limit->getPosition().x,486.31)),EaseBackOut::create(MoveTo::create(0.4, _limit->getPosition())),CallFunc::create([_limit,i,this](){
                auto _price = _limit->getChildByName("price");
                _price->setVisible(true);
                
                if(i == 7){
                    auto _title = _operate->getNodeByName("title");
                    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                    ActionHelper::showBackInOut(_title, _title->getPosition(), ActionHelper::ShowDirection::show_from_top,[this,_title](){
                        
                        _title->runAction(ActionHelper::getJellyAction());
                        if(autoWriteLogo){
                            _operate->addComponent(MakeSignature::create());
                        }else {
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
                                _operate->getNodeByName("btn_rate")->setVisible(true);
                                auto _btnMenu = _operate->getNodeByName("myMenu");
                                _btnMenu->setVisible(true);
                                _btnMenu->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.5, -15), RotateBy::create(1, 30), RotateBy::create(0.5, -15), nullptr)));
                            }) ,nullptr));
                            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                            _btnShop->setVisible(true);
#endif
                        }
                        
                    });
                    _title->setVisible(true);
                }
            }),nullptr));
        }
        
    }else
        for(int i=0;i<6;i++){
            auto _node = _operate->getNodeByName(StringUtils::format("donut_%d",i));
            _node->runAction(Sequence::create(DelayTime::create(0.15*i),CallFunc::create([_node,i](){
                //if(i%2==0)
                SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                _node->setVisible(true);
            }), MoveTo::create(0, Vec2(_node->getPosition().x,486.31)),EaseBackOut::create(MoveTo::create(0.4, _node->getPosition())),CallFunc::create([i,this](){
                if(i == 5){
                    auto _title = _operate->getNodeByName("title");
                    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                    ActionHelper::showBackInOut(_title, _title->getPosition(), ActionHelper::ShowDirection::show_from_top,[this,_title](){
                        if(autoWriteLogo){
                            _operate->addComponent(MakeSignature::create());
                        }
                        else{
                            SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                            _title->runAction(ActionHelper::getJellyAction());
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
                                _operate->getNodeByName("btn_rate")->setVisible(true);
                                auto _btnMenu = _operate->getNodeByName("myMenu");
                                _btnMenu->setVisible(true);
                                _btnMenu->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.5, -15), RotateBy::create(1, 30), RotateBy::create(0.5, -15), nullptr)));
                            }) ,nullptr));
                                #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                            _btnShop->setVisible(true);
#endif
                            
                        }
                    });
                    _title->setVisible(true);
                    
                }
            }), nullptr));
            _node->runAction(Sequence::create(DelayTime::create(0.15*i+0.4),ActionHelper::getJellyAction(), nullptr));
        }
    
    
}