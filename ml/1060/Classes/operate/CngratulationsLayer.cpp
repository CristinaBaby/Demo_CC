//
//  CngratulationsLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#include "CngratulationsLayer.h"
#include "LQ_adLoadingLayer.h"
#include "MyMenu.h"
#include "MakeSignature.h"
#include "MakeStep1.h"
#include "EventPromotLayer.h"
#include "ChangeLayer.h"
#include "AblumLayer.h"
#include "HomeLayer.hpp"
#include "ParticleLayer.h"
#include "MyDialog.h"
IMPLEMENT_COCOS2DX_CLASS(CngratulationsLayer);
CngratulationsLayer::CngratulationsLayer(){
}
bool CngratulationsLayer::init(){
    BaseStepLayer::init();
    _operate = StudioLayer::create("CngratulationsLayer.csb");
    addChild(_operate);
    if(Director::getInstance()->getOpenGLView()->getFrameSize().width>=2048){
        _operate->getNodeByName("highNode")->addChild(Sprite::create("png/1/shop4.jpg"));
    }
    _operate->getNodeByName("shed1_1")->setVisible(false);
    for(int i=0;i<=7;i++){
        auto _node = _operate->getNodeByName(StringUtils::format("limit_%d",i));
        _node->getChildByName("price")->setVisible(false);
    }
    if(Director::getInstance()->getVisibleSize().height<670)
        _operate->getNodeByName("shed")->setVisible(false);
    _operate->getNodeByName("balloon")->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.8, -15), RotateBy::create(1.6, 30), RotateBy::create(0.8, -15), nullptr)));
    auto _path = FileUtils::getInstance()->getWritablePath() + "MakeSignature.png";
    if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->fullPathForFilename(_path)))
        _operate->changeNode("title", _path);
    auto _string = UserDefault::getInstance()->getStringForKey("bgSprite");
    if(!_string.empty())
        _operate->changeNode("title_bg", _string);
   // _operate->getNodeByName("title")->setVisible(false);
    
    
    
    return true;
}

void  CngratulationsLayer::onExitTransitionDidStart(){
    BaseStepLayer::onExitTransitionDidStart();
}

void CngratulationsLayer::touchEnd(ui::Widget* widget){
    BaseStepLayer::touchEnd(widget);
    if(widget->getName() == "home_btn"){
        auto _dialog = MyDialog::create("Do you want to start over with a new game?", MyDialog::twoBtnPath);
        _dialog->dialogBtnClick =[this](MyDialog * dialog,int index){
            if(index == 0) {
                _operate->setIsChange(true);
                auto _endcallBack = [](){
                    LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                    auto _scne = Scene::create();
                    _scne->addChild(HomeLayer::create());
                    Director::getInstance()->replaceScene(TransitionFade::create(0.8,_scne,Color3B::WHITE));
                };
                if(IAPManager::getInstance()->isShowAds()){
                    LQ_adLoadingLayer::loadingDoneCallback = _endcallBack;
                    LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
                } else
                    _endcallBack();
                
            }
        };
        
        getScene()->addChild(_dialog);
    }else if(widget->getName() == "congratulations"){
        widget->setEnabled(false);
        auto _container = _operate->getNodeByName("conTainer");
        _container->stopAllActions();
        auto _particle =   _operate->getNodeByname<ParticleSystemQuad*>("Particle");
        _particle->stopSystem();
        _container->runAction(Sequence::create(EaseExponentialIn::create(ScaleTo::create(0.6, 0)),CallFunc::create([this](){
            auto btn_camer = _operate->getNodeByName("btn_camer");
            ActionHelper::showBackInOut(btn_camer, btn_camer->getPosition(), ActionHelper::ShowDirection::show_from_top);
            btn_camer->setVisible(true);
            auto _btnNext = _operate->getNodeByName("btn_next");
            ActionHelper::showBackInOut(_btnNext, _btnNext->getPosition(), ActionHelper::ShowDirection::show_from_top);
            _btnNext->setVisible(true);
            
            auto _btnhome = _operate->getNodeByName("home_btn");
            ActionHelper::showBackInOut(_btnhome, _btnhome->getPosition(), ActionHelper::ShowDirection::show_from_left);
            _btnhome->setVisible(true);
            _operate->getNodeByname<ui::Widget*>("congratulations")->setVisible(false);
        }), nullptr));
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
        auto _layer = HomeLayer::create();
         _layer->getOperateLayer()->getNodeByName("btn_rate")->setVisible(false);
         _layer->getOperateLayer()->getNodeByName("btn_next")->setVisible(true);
         _layer->getOperateLayer()->getNodeByName("btn_reset")->setVisible(true);
        _layer->setAutoWriteLogo(true);
        auto _scene = Scene::create();
        _scene->addChild(_layer);
        Director::getInstance()->replaceScene(_scene);
    }else if(widget->getName().compare("btn_camer") == 0){
        _operate->getNodeByName("btn_camer")->setVisible(false);
         _operate->getNodeByName("home_btn")->setVisible(false);
        _operate->getNodeByName("btn_next")->setVisible(false);
        auto _size = Director::getInstance()->getVisibleSize();
        auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        _render->begin();
        visit();
        _render->end();
        Director::getInstance()->getRenderer()->render();
        _operate->getNodeByName("btn_camer")->setVisible(true);
         _operate->getNodeByName("home_btn")->setVisible(true);
        _operate->getNodeByName("btn_next")->setVisible(true);
        auto _layer = AblumLayer::create();
        addChild(_layer,30);
        _layer->setRender(_render);
        SoundPlayer::getInstance()->playEffect("sound/camera.mp3");
    }
}
void  CngratulationsLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    SoundPlayer::getInstance()->playBackGroundMusic("sound/general/background.mp3");
    
    auto _EventPromotLayer = EventPromotLayer::create();
    _EventPromotLayer->setTime(2.5);
    addChild(_EventPromotLayer,50);
    _EventPromotLayer->setCallbakc([this](){
  
            string _names[] = {"rote0","showButterfly","rote1","rote2","donut","showButterfly"};
            auto _sinze = sizeof(_names)/sizeof(_names[0]);
            auto _node = _operate->getNodeByname<ui::Widget*>(_names[rand()%_sinze]);
            if(_node != nullptr)
                touchEnd(_node);
        
    });
    
    
        for(int i=0;i<=7;i++){
            auto _node = _operate->getNodeByName(StringUtils::format("limit_%d",i));
             dynamic_cast<ui::Text*>(ui::Helper::seekNodeByName(_node, "Text"))->setString(UserDefault::getInstance()->getStringForKey(StringUtils::format("price%d",i).c_str(), ""));
            _node->runAction(Sequence::create(DelayTime::create(0.15*i),CallFunc::create([_node,i](){
                    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                auto _path = UserDefault::getInstance()->getStringForKey(StringUtils::format("path%d",i).c_str(), "");
                if(_path.find("/") == string::npos)
                    _path = FileUtils::getInstance()->getWritablePath() + _path;
              auto _limitDount = Sprite::create(_path);
                
                _limitDount->setScale(_node->getContentSize().width/_limitDount->getContentSize().width);
                _limitDount->setPosition(_node->getContentSize()*.5);
                _node->addChild(_limitDount,-1);
                _node->setVisible(true);
            }), MoveTo::create(0, Vec2(_node->getPosition().x,486.31)),EaseBackOut::create(MoveTo::create(0.4, _node->getPosition())),CallFunc::create([_node,i,this](){
              
                auto _price = _node->getChildByName("price");
                _price->setVisible(true);
                if(i==7){
                     addChild(ParticleLayer::create(),30);
                    auto shed1_1 = _operate->getNodeByName("shed1_1");
                    ActionHelper::showBackInOut(shed1_1, shed1_1->getPosition(), ActionHelper::ShowDirection::show_from_top,[this](){
                        ActionHelper::delayFunc(1.5, this, [this](){
                            _operate->getNodeByname<ui::Widget*>("congratulations")->setEnabled(false);
                            auto _container = _operate->getNodeByName("conTainer");
                            _container->setScale(0);
                            SoundPlayer::getInstance()->playEffect("sound/general/pop_up_open.mp3");
                            _container->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.8, 1)),CallFunc::create([this](){
                                for(int i=0;i<=3;i++){
                                    auto _action = EaseBackOut::create(ScaleTo::create(0.7, 1));
                                    auto _start = _operate->getNodeByName(StringUtils::format("star%d",i));
                                    _start->setScale(0);
                                    _start->setVisible(true);
                                    _start->runAction(Sequence::create(DelayTime::create(0.5*i), _action->clone(), nullptr));
                                    auto _start2 = _operate->getNodeByName(StringUtils::format("star2_%d",i));
                                    _start2->setScale(0);
                                    _start2->setVisible(true);
                                    _start2->runAction(Sequence::create(DelayTime::create(0.2*i), _action->clone(), nullptr));
                                }
                                SoundPlayer::getInstance()->playEffect("sound/other/success.mp3");
                                for(int i=0;i<7;i++){
                                    this->runAction(Sequence::create(DelayTime::create(0.4*i),CallFunc::create([this,i](){
                                        if(i%3==0)
                                            SoundPlayer::getInstance()->playEffect("sound/general/fireworks.mp3");
                                        auto _particle =  ParticleSystemQuad::create("particle/Flow.plist");
                                        _particle->setDuration(0.4);
                                        _particle->setPosition(Vec2(60+rand()%820,200+rand()%485));
                                        _operate->getNodeByName("particleContiner")->addChild(_particle,300);
                                    }), nullptr));
                                }
                                _operate->getNodeByname<ui::Widget*>("congratulations")->setEnabled(true);
                            }),DelayTime::create(5),CallFunc::create([this](){
                                auto _particle =   _operate->getNodeByname<ParticleSystemQuad*>("Particle");
                                _particle->stopSystem();
                                _operate->getNodeByname<ui::Widget*>("congratulations")->setEnabled(false);
                            }),EaseExponentialIn::create(ScaleTo::create(0.6, 0)),CallFunc::create([this](){
                                auto btn_camer = _operate->getNodeByName("btn_camer");
                                ActionHelper::showBackInOut(btn_camer, btn_camer->getPosition(), ActionHelper::ShowDirection::show_from_top);
                                btn_camer->setVisible(true);
                                auto _btnNext = _operate->getNodeByName("btn_next");
                                ActionHelper::showBackInOut(_btnNext, _btnNext->getPosition(), ActionHelper::ShowDirection::show_from_top);
                                _btnNext->setVisible(true);
                                
                                auto _btnhome = _operate->getNodeByName("home_btn");
                                ActionHelper::showBackInOut(_btnhome, _btnhome->getPosition(), ActionHelper::ShowDirection::show_from_left);
                                _btnhome->setVisible(true);
                                
                                _operate->getNodeByname<ui::Widget*>("congratulations")->setVisible(false);
                            }), nullptr));
                            _operate->getNodeByName("congratulations")->setVisible(true);
                        
                        });
                       
                    });
                    shed1_1->setVisible(true);
                  
                }
            }), nullptr));
        }
   
}