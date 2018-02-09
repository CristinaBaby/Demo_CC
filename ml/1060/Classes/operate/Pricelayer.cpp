//
//  Pricelayer.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#include "Pricelayer.h"
#include "VoteOperate.h"
#include "ParticleLayer.h"
#include "MyMenu.h"
#include "AblumLayer.h"
#include "LQ_adLoadingLayer.h"
#include "MyDialog.h"
#include "HomeLayer.hpp"
#include "AblumLayer.h"
#include "Pricelayer.h"
#include "ChangeLayer.h"
#include "FilePathUtil.h"
#include "EventPromotLayer.h"
#include "HomeLayer.hpp"
#include "CngratulationsLayer.h"
#include "FlurryEventManager.h"
using namespace std;
const string Pricelayer::_eventName = "Price tag";

IMPLEMENT_COCOS2DX_CLASS(Pricelayer);
Pricelayer::Pricelayer() {
    
}
Pricelayer::~Pricelayer(){
    Director::getInstance()->getTextureCache()->removeTextureForKey(gameFoodPath);
}

bool Pricelayer::init() {
    if(BaseStepLayer::init()) {
        gameFoodPath = FileUtils::getInstance()->getWritablePath() + "eatFood.png";
        _currentLayer = nullptr;
        addPos = Vec2::ZERO;
        crruntTarget = 0;
        _operate = StudioLayer::create("cocostudio/PriceLayer.csb");
        addChild(_operate);
        _operate->getNodeByname<ParticleSystemQuad*>("Particle")->stopSystem();
        _prompt = Sprite::create("ui/pubilc/finger1.png");
        _prompt->setAnchorPoint(Vec2(0.2,0.8));
        addChild(_prompt);
        for(int i=0;i<=7;i++){
            dountPath.push_back("");
            dountPrice.push_back("");
        }
        curentIndex = -1;
               saveToMenu();
  
        _operate->getNodeByName("textBg")->setVisible(false);
        vector<string> voteLayers;
        for(int i=0;i<=19;i++)
            voteLayers.push_back(StringUtils::format("png/donuts/donuts%d.png",i));
        vector<int> randIndexs,indexs;
        for(int i=0;i<=7;i++)
            randIndexs.push_back(i);
        for(int i=0;i<3;++i){
            auto _itor = randIndexs.begin()+ rand()%randIndexs.size();
            indexs.push_back(*_itor);
            randIndexs.erase(_itor);
        }
        for(int i=0;i<=7;i++){
            auto _layer = _operate->getNodeByName(StringUtils::format("limit_%d",i));
            dynamic_cast<ui::Layout*>(_layer)->setEnabled(false);
            auto _itor = std::find(indexs.begin(), indexs.end(), i);
            if(_itor != indexs.end()){
                auto _gameDount = Sprite::createWithSpriteFrame(_operate->getNodeByname<Sprite*>("donuts0")->getSpriteFrame());
                _gameDount->setScale(0.25);
                _gameDount->setOpacity(0);
                auto layer_itor = voteLayers.begin()+rand()%voteLayers.size();
                auto _s = Sprite::create(*layer_itor);
                _s->setScale(2.2);
                _s->setPosition(_gameDount->getContentSize()*.5);
                _gameDount->addChild(_s);
                   dountPath[i] = *layer_itor;
                voteLayers.erase(layer_itor);
                auto _inner = Node::create();
                _inner->setPosition(_layer->getContentSize()*.5);
                _inner->setName("_inner");
                _layer->addChild(_inner,-1);
                _inner->addChild(_gameDount,-1);
                _layer->getChildByName("price")->setVisible(true);
                stringstream _strs;
                _strs<<(0.5+rand()%100*0.01f);
                dountPrice[i] = _strs.str();
                dynamic_cast<ui::Text*>(_layer->getChildByName("price")->getChildByName("Text"))->setString(_strs.str());
                indexs.erase(_itor);
                
            }else {
                layers.pushBack(_layer);
                limits.pushBack(LQRect::create(CocosHelper::getNodeRectInWorld(_layer)));
            }
        }
        
        int _gameIndex = rand()%5;
        
        
        
        for(int i=0;i<=4;i++){
            auto _move = TouchMoveComponent::create();
            _move->setMoveType(MoveComponentType::kTypeBack);
            auto _gameDount = _operate->getNodeByname<Sprite*>(StringUtils::format("donuts%d",i));
            _gameDount->setTag(i);
            _gameDount->addComponent(_move);
            if(i == _gameIndex){
                if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->fullPathForFilename(gameFoodPath)))
                    _gameDount->setSpriteFrame(Sprite::create(gameFoodPath)->getSpriteFrame());
                paths.push_back("eatFood.png");
            }else {
                _gameDount->setCascadeOpacityEnabled(false);
                _gameDount->setOpacity(0);
                auto _itor = voteLayers.begin()+rand()%voteLayers.size();
                paths.push_back(*_itor);
                auto _s = Sprite::create(*_itor);
                _s->setScale(2.2);
                _s->setPosition(_gameDount->getContentSize()*.5);
                _gameDount->addChild(_s);
                voteLayers.erase(_itor);
            }
            _move->setTargets(limits);
            _move->addListners(ComponentTouchEnd, CC_CALLBACK_3(Pricelayer::moveEnd,this));
            _dountOperats.pushBack(_move);
        }
        auto _startPos = _operate->getPosinWord("donuts0");
        Vec2 _endPos;
        for(int i=0;i<limits.size();i++){
            _endPos  = dynamic_cast<LQRect*>(limits.at(i))->getMidPos();
            if(std::abs(_endPos.x-_startPos.x)>80)
                break;
        }
        ActionHelper::runBezierMoveHideAction(_prompt, _startPos, _endPos);
        return true;
    }
    return false;
}
 void Pricelayer::onExitTransitionDidStart(){
      FlurryEventManager::getInstance()->logCurrentModuleExitEvent(_eventName);
     BaseStepLayer::onExitTransitionDidStart();
}

void Pricelayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(_eventName);
    if(_operate != nullptr){
        auto _next = _operate->getNodeByName("btn_next");
        if(_next != nullptr)
            _next->stopAllActions();
        
    }
    auto _node = _operate->getNodeByName("textBg");
    auto s = ui::Scale9Sprite::create("png/10/price1.png");
    s->setOpacity(0);
    auto _size = Size(499,100);
    _numBox = NumEditBox::create(_size, s);
    _numBox->setInputMode(cocos2d::ui::EditBox::InputMode::DECIMAL);
    _numBox->setPattern(NumEditBox::pricePattern);
    _numBox->setPositionY(_node->getContentSize().height*.5);
    _numBox->setPositionX(700);
    _numBox->setMaxLength(5);
    _numBox->setFontColor(Color3B::BLACK);
    _node->addChild(_numBox);
    
    auto _EventPromotLayer = EventPromotLayer::create();
    _EventPromotLayer->setTime(3.5);
    addChild(_EventPromotLayer,200);
    _EventPromotLayer->setCallbakc([this](){
        vector<string>_names;
        for(int i=0;i<=7;i++)
            _names.push_back(StringUtils::format("limit_%d",i));
        auto _node = _operate->getNodeByName(_names[rand()%_names.size()])->getChildByName("_inner");
        if(_node != nullptr)
            touchAction(_node);
    });
}

void Pricelayer::touchAction(Node* _node){
    if(_node != nullptr){
        _node->stopActionByTag(1245);
        float timeScale = 1.2;
        ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
        ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
        ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
        ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
        ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
        ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
         ActionInterval * _action = Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
        _action->setTag(1245);
        _node->runAction(_action);
    }
    
}

void Pricelayer::showText(Vec2 pos) {
    SoundPlayer::getInstance()->playEffect("sound/general/pop_up_open.mp3");
    auto str = string(dynamic_cast<ui::Text*>(_currentLayer->getChildByName("price")->getChildByName("Text"))->getString());
    
    _numBox->setText(str.c_str());
    _eventDispatcher->setEnabled(false);
    auto _layer = _operate->getNodeByName("lockLayer");
    _layer->setVisible(true);
    auto _bg = _operate->getNodeByName("textBg");
    _bg->setScale(1);
    _bg->setVisible(false);
    _bg->setPosition(_bg->getParent()->convertToNodeSpace(pos));
    _numBox->setInputMode(cocos2d::ui::EditBox::InputMode::DECIMAL);
    _numBox->openKeyboard();
  
   // _bg->setScale(0);
    //_bg->setVisible(true);
    _bg->runAction(Sequence::create(ScaleTo::create(0, 1),Show::create(), EaseBackOut::create(Spawn::create(ScaleTo::create(0.7, 1), MoveTo::create(0.7, Vec2(400,320+80)), nullptr)),CallFunc::create([this](){
        _eventDispatcher->setEnabled(true);
    }), nullptr));
}
void Pricelayer::hideText(Vec2 pos){
    _eventDispatcher->setEnabled(false);
    auto _bg = _operate->getNodeByName("textBg");
    _bg->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.4, 180/890.0f), MoveTo::create(0.4, pos), nullptr),Spawn::create(ScaleTo::create(0.4, 85/890.0f), RotateBy::create(0.4, 720), nullptr), CallFunc::create([this,_bg](){
        string _text = _numBox->getText();
        auto _pari = ParticleSystemQuad::create("particle/decorateParticle.plist");
        _pari->setPosition(_bg->getParent()->convertToWorldSpace(_bg->getPosition()));
        this->addChild(_pari,50);
        SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
        dynamic_cast<ui::Text*>(_currentLayer->getChildByName("price")-> getChildByName("Text"))->setString(_text);
        dountPrice[curentIndex] = _text;
        _currentLayer->getChildByName("price")->setVisible(true);
        _numBox->closeKeyboard();
        auto _layer = _operate->getNodeByName("lockLayer");
        _layer->setVisible(false);
        
        dynamic_cast<ui::Layout*>(_currentLayer)->setEnabled(true);
        _currentLayer = nullptr;
        if(addPos != Vec2::ZERO&&crruntTarget<5){
            
            addPos = Vec2::ZERO;
            crruntTarget++;
            if(crruntTarget == 5){
                auto _talbe = _operate->getNodeByName("table");
                ActionHelper::hide(_talbe, ActionHelper::ShowDirection::show_from_left,[this](){
                    auto _operationLayer = _operate->getNodeByName("optionLayer");
                    _operationLayer->runAction(MoveBy::create(0.8,Vec2(0,-100)));
                    _operationLayer->runAction(Sequence::create(ScaleTo::create(0.8, 1.1),CallFunc::create([this](){
                        this->addChild(ParticleLayer::create());
                        auto _s = Sprite::create(gameFoodPath);
                        _s->setScale(0.35);
                        _s->setPosition(getContentSize()*.5);
                        addChild(_s,10);
                        
                        ActionHelper::delayFunc(0.4, this, [this,_s](){
                            ccBezierConfig _config;
                            _config.endPosition = _operate->getPosinWord("myMenu");
                            _config.controlPoint_1 = _config.controlPoint_2 = Vec2(_s->getPositionX()+150, _s->getPosition().y-100);
                            _s->runAction(Sequence::create(BezierTo::create(1.4, _config),CallFunc::create([=](){
                                auto _pari = ParticleSystemQuad::create("particle/decorateParticle.plist");
                                _pari->setPosition(_s->getParent()->convertToWorldSpace(_s->getPosition()));
                                this->addChild(_pari,50);
                                SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
                                _s->removeFromParent();
                                //ActionHelper::delayFunc(1.5, this, CC_CALLBACK_0(Pricelayer::priceEndDialog, this));
                
                            }), nullptr));
                            _s->runAction(RotateBy::create(1.4, -360));
                            _s->runAction(EaseExponentialIn::create(ScaleTo::create(1.4, 0.1)));
                        });
                        auto _btnMenu = _operate->getNodeByName("myMenu");
                        _btnMenu->setVisible(true);
                        _btnMenu->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.5, -15), RotateBy::create(1, 30), RotateBy::create(0.5, -15), nullptr)));
                        auto _btnHome = _operate->getNodeByName("btn_home");
                        if(_btnHome != nullptr){
                        ActionHelper::showBackInOut(_btnHome, _btnHome->getPosition(), ActionHelper::ShowDirection::show_from_top);
                        _btnHome->setVisible(true);
                        }
                        auto btn_camer = _operate->getNodeByName("btn_camer");
                        ActionHelper::showBackInOut(btn_camer, btn_camer->getPosition(), ActionHelper::ShowDirection::show_from_left);
                        btn_camer->setVisible(true);
                        auto _btnNext = _operate->getNodeByName("btn_next");
                        ActionHelper::showBackInOut(_btnNext, _btnNext->getPosition(), ActionHelper::ShowDirection::show_from_right);
                        _btnNext->setVisible(true);
                    }), nullptr));
                });
                
            }
        }
        _eventDispatcher->setEnabled(true);
    }),ScaleTo::create(0, 0), nullptr));
}


void Pricelayer::touchEnd(ui::Widget* widget){
  if(widget->getName() == "btn_delete") {
        _numBox->setText("");
    } else if(widget->getName() == "btn_sure"){
        auto _price = _currentLayer->getChildByName("price");
        _price->setVisible(false);
        auto _pos = _operate->getNodeByName("Panel_2")->convertToNodeSpace(_price->convertToWorldSpace(_price->getContentSize()*.5));  //_currentLayer->getParent()->convertToNodeSpace(_currentLayer->convertToWorldSpace(_price->getPosition()));
        hideText(_pos+Vec2(10,-10));
    } else if(widget->getName() == "myMenu"){
        auto _myMenu = MyMenu::create();
        getScene()->addChild(_myMenu);
        _myMenu->show(widget->convertToWorldSpace(widget->getContentSize()*.5));
    }else if(widget->getName() == "btn_home"){
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
    }else if(widget->getName().compare("btn_camer") == 0){
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
        _operate->getNodeByName("btn_camer")->setVisible(false);
       // _operate->getNodeByName("btn_home")->setVisible(false);
        _operate->getNodeByName("myMenu")->setVisible(false);
        _operate->getNodeByName("btn_next")->setVisible(false);
        auto _size = Director::getInstance()->getVisibleSize();
        auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        _render->begin();
        visit();
        _render->end();
        Director::getInstance()->getRenderer()->render();
        _operate->getNodeByName("btn_camer")->setVisible(true);
        //_operate->getNodeByName("btn_home")->setVisible(true);
        _operate->getNodeByName("myMenu")->setVisible(true);
        _operate->getNodeByName("btn_next")->setVisible(true);
        auto _layer = AblumLayer::create();
        addChild(_layer,30);
        _layer->setRender(_render);
        SoundPlayer::getInstance()->playEffect("sound/camera.mp3");
    }else if(widget->getName() == "btn_next"){
        for(int i=0;i<dountPrice.size();i++)
            UserDefault::getInstance()->setStringForKey(StringUtils::format("price%d",i).c_str(),dountPrice.at(i));
        
        for(int i=0;i<dountPath.size();i++)
            UserDefault::getInstance()->setStringForKey(StringUtils::format("path%d",i).c_str(),dountPath.at(i));
        UserDefault::getInstance()->flush();
        
        auto _layer = CngratulationsLayer::create();
//        for(int i=0;i<=7;i++){
//            auto _limit = _operate->getNodeByName(StringUtils::format("limit_%d",i));
//            auto _inner = _limit->getChildByName("_inner");
//            if(_inner != nullptr){
//                _inner->retain();
//                auto _homeLimit = _layer->getOperateLayer()->getNodeByName(StringUtils::format("limit_%d",i));
//                auto _news = Node::create();
//                _news->setName("_inner");
//                _news->setVisible(false);
//                _news->addChild(_inner);
//                _inner->setPosition(Vec2::ZERO);
//                _inner->release();
//                _inner->setScale((_homeLimit->getContentSize().width+10)/_limit->getContentSize().width);
//                _news->setPosition(_homeLimit->getContentSize()*.5);
//                _homeLimit->addChild(_news,-1);
//                dynamic_cast<ui::Text*>(ui::Helper::seekNodeByName(_homeLimit, "Text"))->setString( dynamic_cast<ui::Text*>(ui::Helper::seekNodeByName(_limit, "Text"))->getString());
//                _homeLimit->setVisible(true);
//            }
//            
//        }
      
        auto _sene = Scene::create();
        _sene->addChild(_layer);
        Director::getInstance()->replaceScene(_sene);
       
    }else {
        for(int i=0;i<=7;i++){
            if(widget->getName().compare(StringUtils::format("limit_%d",i))==0){
                _currentLayer = widget;
                curentIndex = i;
                showText(_currentLayer->getParent()->convertToWorldSpace(_currentLayer->getPosition()));
                touchAction(_currentLayer);
                break;
            }
        }
    }
}

void Pricelayer::moveEnd(Node* _node,Component* com,OperateListner* _lis){
    stopPrompt();
    addPos = dynamic_cast<TouchMoveComponent*>(com)->getOrnagnalPos();
    int _index = dynAny_cast<int>(_lis->getData());
    
    _currentLayer = layers.at(_index);
    for(int i=0;i<=7;i++){
        if(_currentLayer->getName().compare(StringUtils::format("limit_%d",i))==0){
            curentIndex = i;
            dountPath[i] = paths[_node->getTag()];
            break;
        }
    }
    auto _inner = Node::create();
    _inner->setPosition(_currentLayer->getContentSize()*.5);
    _inner->setName("_inner");
    _currentLayer->addChild(_inner,-1);
    //auto _rect = dynamic_cast<LQRect*>(limits.at(_index));
    auto _pos = Vec2(_currentLayer->getContentSize()*.5) +Vec2(0,10);
    CocosHelper::reAddNode(_node, _inner);
   // _node->getParent()->reorderChild(_node, -1);
    _node->runAction(MoveTo::create(0.2, Vec2::ZERO));
    
    showText(_currentLayer->convertToWorldSpace(_pos));
    limits.erase(limits.begin()+_index);
    layers.erase(_index);
    _node->removeAllComponents();
    for(auto _com:_dountOperats)
        _com->setTargets(limits);
    
}

void  Pricelayer::saveToMenu(){
    auto _eatContiner = Sprite::create(gameFoodPath);
    auto _bg = Sprite::create("ui/menu/menu-donutbg.png");
    _bg->setAnchorPoint(Vec2::ZERO);
    auto _size = _bg->getContentSize();
    _eatContiner->setPosition(_size*.5);
    _eatContiner->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _eatContiner->setScale(0.5);
    auto _render = RenderTexture::create(_size.width, _size.height);
    _render->begin();
    _bg->visit();
    _eatContiner->visit();
    _render->end();
     FilePathUtil::saveToFavorite(_render, nullptr, "DonutShopMenu");
    Director::getInstance()->getRenderer()->render();

}