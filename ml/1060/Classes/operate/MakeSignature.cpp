//
//  MakeSignature.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/4.
//
//

#include "MakeSignature.h"
#include "WriteLayer.h"
#include "KeyBackEvent.h"
#include "MakeStep1.h"
#include "ChangeLayer.h"
#include "TouchNodeComponent.h"
#include "ParticleLayer.h"
#include "FlurryEventManager.h"
#include "IAPManager.h"
#include "AdsManager.h"
#include "MyDialog.h"
#include "HomeLayer.hpp"
#include "LQ_adLoadingLayer.h"
const string MakeSignature::_eventName = "Design logo";

IMPLEMENT_COCOS2DX_CLASS(MakeSignature);
MakeSignature::MakeSignature():
_operateLayer(nullptr)
,_prompt(nullptr)
,_drawNode(nullptr)
,_finger(nullptr){
    
}
void MakeSignature::onAdd(){
    LQComponent::onAdd();
    if(IAPManager::getInstance()->isShowAds())
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
      FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(_eventName);
    _operateLayer = dynamic_cast<StudioLayer*>(_owner);
    _operateLayer->getNodeByName("btn_rate")->setVisible(false);
    
    
    
    for(int i=0;i<3;i++)
        ActionHelper::delayFunc(0.25*i,_operateLayer,[this](){
            auto _node = _operateLayer->getNodeByname<ui::Widget*>("showButterfly");
            if(_node != nullptr&&_operateLayer->_touchBtnCallback != nullptr)
                _operateLayer->_touchBtnCallback(_node);
            
        });
    
    
    auto _touch = TouchNodeComponent::create();
    _touch->addListners(ComponentTouchEnd, CC_CALLBACK_3(MakeSignature::touchEnd, this));
    _operateLayer->getNodeByName("title_bg")->addComponent(_touch);
    auto _path = FileUtils::getInstance()->getWritablePath() + "MakeSignature.png";
    if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->fullPathForFilename(_path)))
        _operateLayer->changeNode("title", _path);
    auto _string = UserDefault::getInstance()->getStringForKey("bgSprite");
    if(!_string.empty())
        _operateLayer->changeNode("title_bg", _string);
    KeyBackEvent::getInstance()->addBackEvent(CC_CALLBACK_0(MakeSignature::onBack,this), _owner);
    
    _finger = _operateLayer->getNodeByName("finger");
    if(_finger != nullptr){
        _finger->setVisible(true);
        _prePos = Vec2::ZERO;
        ccBezierConfig _config;
        _config.endPosition = _finger->getPosition()+Vec2(150,0);
        _config.controlPoint_1 = _finger->getPosition()+Vec2(50,50);
        _config.controlPoint_2 = _finger->getPosition()+Vec2(100,-50);
        ccBezierConfig _config2;
        _config2.endPosition = _finger->getPosition()+Vec2(300,0);
        _config2.controlPoint_1 = _finger->getPosition()+Vec2(200,50);
        _config2.controlPoint_2 = _finger->getPosition()+Vec2(250,-50);
        _finger->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0, _finger->getPosition()),FadeTo::create(0, 255),BezierTo::create(0.8, _config),BezierTo::create(0.8, _config2),FadeTo::create(0, 0),DelayTime::create(0.5), nullptr)));
        schedule(schedule_selector(MakeSignature::updatePos), 0);
    }else
        _operateLayer->getNodeByName("btn_next")->setVisible(true);
    auto _showLogo = _operateLayer->getNodeByName("showLogo");
    if(_showLogo != nullptr)
        ActionHelper::showTitle(_operateLayer->getNodeByName("showLogo"));
    auto _title = _operateLayer->getNodeByName("title");
    _title->stopAllActions();
    _title->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.9, 1.1),ScaleTo::create(0.9, 1), nullptr)));
    
}

void MakeSignature::onRemove(){
        FlurryEventManager::getInstance()->logCurrentModuleExitEvent(_eventName);
    unSchedule(schedule_selector(MakeSignature::updatePos));
    KeyBackEvent::getInstance()->removeEvent(_owner);
    if(_prompt != nullptr){
        _prompt->removeFromParent();
        _prompt = nullptr;
    }
    _operateLayer->getAction()->clearLastFrameCallFunc();
    if(_finger != nullptr){
        _finger->setVisible(false);
        _finger->setOpacity(0);
    }
    LQComponent::onRemove();
}


bool MakeSignature::onBack(){
    auto _dialog = MyDialog::create("Do you want to start over with a new game?", MyDialog::twoBtnPath);
    _dialog->dialogBtnClick =[](MyDialog * dialog,int index){
        if(index == 0) {
            auto _endcallBack = [](){
                Director::getInstance()->popToRootScene();
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
    
    Director::getInstance()->getRunningScene()->addChild(_dialog);
    return true;

}

bool MakeSignature::init(){
    LQComponent::init();
    return true;
}

void MakeSignature::touchEnd(Node* widget,Component*, OperateListner* _lis){
    if(widget->getName() == "title_bg"){
        auto _title = _operateLayer->getNodeByName("title");
        unSchedule(schedule_selector(MakeSignature::updatePos));
        if(_drawNode != nullptr)
            _drawNode->clear();
        
        _title->stopAllActions();
        _title->setScale(1);
        if(_finger != nullptr)
            _finger->setVisible(false);
        auto writeLayer = WriteLayer::create();
        _operateLayer ->addChild(writeLayer,300);
        writeLayer->showOperate(widget->getParent()->convertToWorldSpace(widget->getPosition()));
        writeLayer->writeEnd = [this](Node* _rendreNode,bool isChange,string bgPath){
            if(isChange&&!bgPath.empty()){
                _operateLayer->changeNode("title_bg", bgPath);
                UserDefault::getInstance()->setStringForKey("bgSprite", bgPath);
                UserDefault::getInstance()->flush();
            }
            _rendreNode->removeChildByName("bgSprite");
            if(isChange&&_rendreNode->getChildren().size()>0) {
                auto _size = _rendreNode->getContentSize();
                auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
                _render->begin();
                _rendreNode->visit();
                _render->end();
                Director::getInstance()->getRenderer()->render();
                auto _path = FileUtils::getInstance()->getWritablePath() + "MakeSignature.png";
                Director::getInstance()->getTextureCache()->removeTextureForKey(_path);
                SpriteFrameCache::getInstance()->removeSpriteFrameByName(_path);
                _render->saveToFile("MakeSignature.png");
                Director::getInstance()->getRenderer()->render();
                _operateLayer->changeNode("title", _path);
            }
            _operateLayer->getAction()->clearLastFrameCallFunc();
            if(_finger != nullptr){
                _finger->setVisible(false);
                _finger->setOpacity(0);
            }
            auto _showLogo =   _operateLayer->getNodeByName("showLogo");
            if(_showLogo != nullptr)
                _showLogo->setVisible(false);
            SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
            auto _title = _operateLayer->getNodeByName("title");
            if(_title->getChildByName("paritcle") == nullptr){
                auto _paritlc = ParticleSystemQuad::create("particle/characterSelection_selectGirl1.plist");
                if(_paritlc != nullptr){
                    _paritlc->setPosition(_title->getContentSize()*.5);
                    _title->addChild(_paritlc);
                }
            }
            ActionHelper::delayFunc(1.8, _operateLayer, [this](){
                _operateLayer->addChild(ParticleLayer::create());
                
                string _names[] = {"rote2","donut","donut_0","donut_5","showButterfly","rote0","showButterfly","showButterfly","showButterfly","showButterfly","balloon"};
                
                auto _sinze = sizeof(_names)/sizeof(_names[0]);
                for(int i=0;i<_sinze;i++){
                    auto _node = _operateLayer->getNodeByname<ui::Widget*>(_names[i]);
                    
                    if(_node != nullptr&&_operateLayer->_touchBtnCallback != nullptr)
                        _operateLayer->_touchBtnCallback(_node);
                    
                }
            });
            
            
            ActionHelper::delayFunc(2.5, _operateLayer, [this](){
                auto _next = _operateLayer->getNodeByName("btn_next");
                if(!_next->isVisible()){
                    ActionHelper::showBackInOut(_next, _next->getPosition(), ActionHelper::ShowDirection::show_from_top);
                    _next->setVisible(true);
                }
//                
//                auto doutKeys = UserDefault::getInstance()->getStringForKey("path0","");
//                if(!doutKeys.empty()){
                    auto _btnReset = _operateLayer->getNodeByName("btn_reset");
                    if(!_btnReset->isVisible()){
                        ActionHelper::showBackInOut(_btnReset, _btnReset->getPosition(), ActionHelper::ShowDirection::show_from_top);
                        _btnReset->setVisible(true);
                    }
               // }
            });
            
        };
    }
}

void MakeSignature::updatePos(float){
    if(_drawNode == nullptr){
        auto _titlBg = _operateLayer->getNodeByName("title_bg");
        auto title = _operateLayer->getNodeByName("title");
        _drawNode = CaculateScribbleNode::create();
        _drawNode->setPosition(title->getContentSize()*.5);
        _drawNode->setContentSize(_titlBg->getContentSize());
        _drawNode->setDrawRadius(2.5f);
        title->addChild(_drawNode);
        _drawNode->setEnable(false);
    }
    auto _crrent = _finger->convertToWorldSpace(Vec2(5,5));
    if(_crrent.x<_prePos.x){
        _drawNode->clear();
        _prePos = Vec2::ZERO;
    }
    if(_prePos == Vec2::ZERO)
        _prePos = _crrent;
    
    _drawNode->paint(_prePos, _crrent);
    _prePos = _crrent;
}
