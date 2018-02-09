//
//  TouchGame.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/7.
//
//

#include "TouchGame.h"
#include "FryDount.h"
#include "MoveForeverComponent.h"
#include "ChangeLayer.h"
#include "ParticleLayer.h"
#include "FlurryEventManager.h"
#include "AdLoadingLayerBase.h"
IMPLEMENT_COCOS2DX_CLASS(TouchGame);
const string TouchGame::_eventName = "Donut collect";

TouchGame::TouchGame(){
    numContanier.push_back(0);
}
bool TouchGame::init(){
    if(BaseStepLayer::init()) {
        isEnd = false;
        _operate = StudioLayer::create("cocostudio/TouchGame.csb");
        addChild(_operate);
        _logo = Sprite::create("ui/minigame/awesome.png");
        addChild(_logo,200);
        _logo->setVisible(false);
        
        
        dountContainer = _operate->getNodeByName("dountContainer");
        return true;
    }
    return false;
}
 void TouchGame::onExitTransitionDidStart(){
     _eventDispatcher->removeCustomEventListeners(AdLoadingLayerBase::ADS_SHOW);
      _eventDispatcher->removeCustomEventListeners(AdLoadingLayerBase::ADS_CLOSE);
     FlurryEventManager::getInstance()->logCurrentModuleExitEvent(_eventName);
     BaseStepLayer::onExitTransitionDidStart();
}
void TouchGame::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
      FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(_eventName);
    auto _container = _operate->getNodeByName("text_container");
    _lable = ImageLabel::createWithPlist("nums.plist");
    _lable->setContentSize(_container->getContentSize());
    _lable->setSpace(-7);
    _lable->setPosition(_container->getContentSize()*.5);
    _lable->setDirection(ImageLabel::Direction::right);
    _lable->setText("0");
    _container->addChild(_lable,200);
    
    float _speeds[] = {250*0.8,275*0.8,260*0.8};
    for(int i=0;i<3;i++){
        MoveForeverComponent* _move = MoveForeverComponent::create();
        _move->setSpeed(_speeds[i]);
        _operate->getNodeByName(StringUtils::format("line%d",i))->addComponent(_move);
        _moves.pushBack(_move);
        
        auto _lis = OperateListner::create();
        _lis->setKey(MoveForeverCallBack);
        _lis->setCallBack(CC_CALLBACK_3(TouchGame::moveCallBack, this));
        _dounts.push_back(Vector<ui::Widget*>());
        _move->addListners(_lis);
        
        Vec2 _pos = _operate->getPosinWord(StringUtils::format("Sprite_%d",8+i));
        _pos.x = _pos.x;
        Rect _limit;
        _limit.origin = _pos - Vec2(80, 60);
        _limit.size = Size(130,121);
        limits.push_back(_limit);
        _move->pauseAction();
    }
    auto _title = _operate->getNodeByName("title");
    auto _pos = _title->getPosition();
    _eventDispatcher->addCustomEventListener(AdLoadingLayerBase::ADS_SHOW,[this](EventCustom *){
        for(auto _move:_moves)
            _move->pauseAction();
        _actionManager->pauseTarget(this);
    });
    _eventDispatcher->addCustomEventListener(AdLoadingLayerBase::ADS_CLOSE,[this](EventCustom *){
        for(auto _move:_moves)
            _move->resumeAction();
        _actionManager->resumeTarget(this);
    });
    
    ActionHelper::show(_title, Director::getInstance()->getVisibleSize()*.5, ActionHelper::ShowDirection::show_from_top,[_title,this,_pos](){
        ActionHelper::delayFunc(1.7, _title, [_title,_pos,this](){
            _title->runAction(Sequence::create(EaseBackInOut::create(MoveTo::create(0.8, _pos)),CallFunc::create([_title,this](){
                SoundPlayer::getInstance()->playEffect("sound/Sound/ready.mp3");
                showLogo("ui/minigame/ready.png");
                ActionHelper::delayFunc(1.4, this, [this](){
                    showLogo("ui/minigame/go.png");
                     SoundPlayer::getInstance()->playEffect("sound/Sound/go.mp3");
                    ActionHelper::delayFunc(0.4, this, [this](){
                        for(auto _m:_moves)
                            _m->resumeAction();
                        createSchdue();;
                    });
                });
            }), nullptr));
          
          
        });
        
    });
    _title->setVisible(true);
    
    auto _numBg = _operate->getNodeByName("Sprite_3");
    _faidPos = _operate->getPosinWord("donut");
    ActionHelper::showBackInOut(_numBg, _numBg->getPosition(), ActionHelper::ShowDirection::show_from_top);
    _numBg->setVisible(true);
}

void TouchGame::touchEnd(ui::Widget* widget){
    if(widget->getName() == "_donut"){
        if(limits.at(widget->getTag()).containsPoint(widget->getPosition())){
            SoundPlayer::getInstance()->playEffect("sound/other/yay.mp3");
           // _operate->getNodeByName("title")->setVisible(false);
            _dounts.at(widget->getTag()).eraseObject( widget);
            widget->setTouchEnabled(false);
            widget->getParent()->reorderChild(widget, 60);
            showLogo();
            auto _currentIndex = std::min((*(numContanier.end()-1))+1,99);
            numContanier.push_back(_currentIndex);
            ActionHelper::delayFunc(0.18, this, [this,widget](){
                CallFunc *_endF = CallFunc::create([this,widget](){
                    _operate->getNodeByname<ParticleSystemQuad*>("particle")->setLifeVar(0.25);
                    _operate->getNodeByname<ParticleSystemQuad*>("particle")->setTotalParticles(500);
                    _operate->getNodeByname<ParticleSystemQuad*>("particle")->resetSystem();
                    widget->removeFromParent();
                    auto _itor = numContanier.begin()+1;
                    _lable->setText(StringUtils::format("%d",*_itor));
                    numContanier.erase(numContanier.begin());
                    auto _dount = _operate->getNodeByName("donut");
                    _dount->stopAllActions();
                    _dount->setScale(1,1);
                    _dount->runAction(ActionHelper::getJellyAction());
                    if(*_itor >= 20&& !isEnd){
                        isEnd = true;
//                        for(auto _m:_moves)
//                            _m->pauseAction();
                          SoundPlayer::getInstance()->playEffect("sound/other/success.mp3");
                        for(int i=0;i<7;i++){
                            this->runAction(Sequence::create(DelayTime::create(0.4*i),CallFunc::create([this,i](){
                                if(i%3==0)
                                    SoundPlayer::getInstance()->playEffect("sound/general/fireworks.mp3");
                                auto _particle =  ParticleSystemQuad::create("particle/Flow.plist");
                                _particle->setDuration(0.4);
                                _particle->setPosition(Vec2(60+rand()%820,200+rand()%485));
                                addChild(_particle,300);
                            }), nullptr));
                        }
                       // this->addChild(ParticleLayer::create());
                        auto _next = _operate->getNodeByName("btn_next");
                        ActionHelper::showBackInOut(_next, _next->getPosition(), ActionHelper::ShowDirection::show_from_top);
                        _next->setVisible(true);
                        auto reset = _operate->getNodeByName("btn_reset");
                        ActionHelper::showBackInOut(reset, reset->getPosition(), ActionHelper::ShowDirection::show_from_right);
                        reset->setVisible(true);
                    }
                      SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
                });
                ccBezierConfig _config;
                _config.endPosition = _operate->getPosinWord("Sprite_3");
                _config.controlPoint_1 = _config.controlPoint_2 = Vec2(widget->getPositionX()-150, widget->getPosition().y-100);
                widget->runAction(Sequence::create(BezierTo::create(0.8, _config),_endF, nullptr));
                widget->runAction(RotateBy::create(0.8, 360));
                widget->runAction(EaseExponentialIn::create(ScaleTo::create(0.8, 0.4)));
            });
            
            widget->runAction(ScaleTo::create(0.15, 1.15));
            widget->runAction(RotateBy::create(0.15, 720));
        }
    }else if(widget->getName() == "btn_next"){
        ChangeLayer::_changeLayer = [](){
            return FryDount::create();
        };
        ChangeLayer::loading();
        
    }else if(widget->getName() == "btn_reset"){
        auto _scene = Scene::create();
        _scene->addChild(TouchGame::create());
        Director::getInstance()->replaceScene(_scene);
    }
}

void TouchGame::createSchdue(){
    vector<int > indexs;
    for(int i=0;i<3;i++)
        indexs.push_back(i);
    float _time = 0.0f;
    for (int i=0;i<3;i++){
        auto _itor = indexs.begin()+rand()%indexs.size();
        int index = *_itor;
        this->runAction(Sequence::create(DelayTime::create(_time), CallFunc::create([index,this,i](){ createDonut(index);
            if(i==2)
               this->runAction(Sequence::create(DelayTime::create(1+0.1*(rand()%8)), CallFunc::create(CC_CALLBACK_0(TouchGame::createSchdue, this)), nullptr));
        }), nullptr));
         _time+= 0.8+0.1*(rand()%12);
        indexs.erase(_itor);
    }
//    
//    this->runAction(Sequence::create(DelayTime::create(3+0.1*(rand()%8)), CallFunc::create(CC_CALLBACK_0(TouchGame::createSchdue, this)), nullptr));
}

void TouchGame::createDonut(int _index){
    auto _move = _moves.at(_index);
    string _filePath = "png/4/donuts.png";
    auto _donut = ui::ImageView::create(_filePath);
    _donut->setTouchEnabled(true);
    _donut->addTouchEventListener(CC_CALLBACK_2(StudioLayer::touchBtn, _operate));
    _donut->setName("_donut");
    _donut->setTag(_index);
    Vec2 _pos;
    _pos.y = _move->getOwner()->convertToWorldSpace(_move->getOwner()->getContentSize()*.5).y;
    _pos.x = -_donut->getContentSize().width*.5 -100;
    _donut->setPosition(_pos);
    dountContainer->addChild(_donut,3);
    _dounts.at(_index).pushBack(_donut);
    
}

void TouchGame::moveCallBack(Node*,Component* _coplent,OperateListner* _oper){
    auto _index = std::find(_moves.begin(), _moves.end(), _coplent) - _moves.begin();
    const auto& _deta = dynAny_cast<Vec2>(_oper->getData());
    auto& _vector = _dounts.at(_index);
    auto _itor = _vector.begin();
    while (_itor != _vector.end()) {
        (*_itor)->setPosition((*_itor)->getPosition() + _deta);
        if(limits.at(_index).getMaxX()<(*_itor)->getPositionX()&&(*_itor)->isEnabled()){
            (*_itor)->setEnabled(false);
            SoundPlayer::getInstance()->playEffect("sound/other/mini game_miss.mp3");
            
            if(*(numContanier.end() -1)>0){
                if(numContanier.size() == 1){
                    numContanier[0] = numContanier[0]-1;;
                    _lable->setText(StringUtils::format("%d",numContanier[0]));
                }else {
                    numContanier[numContanier.size()-1]= numContanier[numContanier.size()-1]-1;
                }
                
                auto _dount = Sprite::create("ui/minigame/donut.png");
                _dount->setPosition(_faidPos+Vec2(-10,10)+Vec2(rand()%40,rand()%20));
                addChild(_dount,30);
                
                _dount->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(1.4, Vec2(0,-800))),CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, _dount)), nullptr));
            }
        }
        
        if( (*_itor)->getPositionX()>Director::getInstance()->getVisibleSize().width +  (*_itor)->getContentSize().width){//超出屏幕，就移除
            (*_itor)->removeFromParent();
            _vector.erase(_itor);
           
        } else
            _itor++;
    }
}

void TouchGame::showLogo(string filePath){
    string paths[] = {"ui/minigame/cool.png","ui/minigame/amazing.png","ui/minigame/awesome.png"};
   
    _logo->stopAllActions();
    _logo->setScale(0);
    _logo->setPosition(Vec2(getContentSize())*.5+Vec2(0,20));
    float _delayTime = 0.2;
    int _index = -1;
    if(filePath.empty()){
         _index =rand()%3;
        _logo->setTexture(paths[_index]);
        auto _particle = ParticleSystemQuad::create("particle/goodParticle.plist");
        _particle->setPosition(_logo->getContentSize()*.5);
        _logo->addChild(_particle,0);
    }else{
        _logo->setTexture(filePath);
        _delayTime = 0.8;
    }
         _logo->setVisible(true);
    if(_index != -1){
        string soundPath[] = {"sound/Sound/cool.mp3","sound/Sound/amazing.mp3","sound/Sound/awesome.mp3"};
        SoundPlayer::getInstance()->playEffect(soundPath[_index]);
    }
    _logo->runAction(Sequence::create(ScaleTo::create(0.2f, 1), EaseBounceOut::create(MoveBy::create(0.2, Vec2(0,-20))), DelayTime::create(_delayTime),CallFunc::create([this](){_logo->setVisible(false);}), nullptr));
}