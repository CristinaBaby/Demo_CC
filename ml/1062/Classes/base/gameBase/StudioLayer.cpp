//
//  StudioLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/13.
//
//

#include "StudioLayer.h"
using namespace ui;
#include "SoundPlayer.h"
IMPLEMENT_COCOS2DX_CLASS(StudioLayer);


StudioLayer::StudioLayer():
_fileName("")
,_root(nullptr)
,uiAction(nullptr)
,isChange(true)
,_touchBtnCallback(nullptr)
,actionEndFuc(nullptr){
    
}

StudioLayer::~StudioLayer(){
}

StudioLayer* StudioLayer::create(string _filePath){
    auto _layer = new StudioLayer();
    if(_layer != nullptr ){
        _layer->_fileName = _filePath;
        if(_layer->init()){
            _layer->autorelease();
            return _layer;
        }
    }
    CC_SAFE_RELEASE_NULL(_layer);
    return nullptr;
}

 StudioLayer* StudioLayer::createNoLayout(string _filePath){
      auto _layer = new StudioLayer();
     if(_layer != nullptr){
           _layer->_fileName = _filePath;
         if(_layer->init()){
             _layer->initWithFileNoLayout(_filePath);
             _layer->autorelease();
             return _layer;
         }
     }
     CC_SAFE_RELEASE_NULL(_layer);
     return nullptr;
}

void StudioLayer::playAction(string name,std::function<void()> _endFun,bool _loop){
    if(uiAction != nullptr){
        if(_endFun != nullptr){
            actionEndFuc = _endFun;
            uiAction->setLastFrameCallFunc([this](){
                uiAction->setLastFrameCallFunc(nullptr);
                if(actionEndFuc != nullptr){
                    
                    auto _temp = actionEndFuc;
                    actionEndFuc = nullptr;
                    _temp();
                }
            });
        }
        uiAction->play(name, _loop);
    }
}

timeline::ActionTimeline* StudioLayer::getAction(){
    return uiAction;
}


Node* StudioLayer::getNodeByName(string _name){
    return Helper::seekNodeByName(_root, _name);
}


void StudioLayer::changeNode(string name,string path) {
    changeNode(name,Sprite::create(path)->getSpriteFrame());
}

void StudioLayer::changeNode(string name,Texture2D* _tex){
    auto _node = getNodeByName(name);
    auto _sprite = dynamic_cast<Sprite*>(_node);
    if(_sprite != nullptr)
        _sprite->setTexture(_tex);
    else{
        auto _image = dynamic_cast<ImageView*>(_node);
        if(_image != nullptr)
            dynamic_cast<Scale9Sprite*>(_image->getVirtualRenderer())->init(Sprite::createWithTexture(_tex), Rect::ZERO,Rect::ZERO);
    }
}
void StudioLayer::changeNode(string name,SpriteFrame* _frame){
    auto _node = getNodeByName(name);
    auto _sprite = dynamic_cast<Sprite*>(_node);
    if(_sprite != nullptr)
        _sprite->setSpriteFrame(_frame);
    else{
        auto _image = dynamic_cast<ImageView*>(_node);
        if(_image != nullptr)
            dynamic_cast<Scale9Sprite*>(_image->getVirtualRenderer())->setSpriteFrame(_frame);
    }
    
}
void StudioLayer::changeNode(string name,Sprite* _sprite){
    changeNode(name,_sprite->getSpriteFrame());
}

Vec2 StudioLayer::getPosinWord(string name){
    Vec2 _pos;
    auto _node = ui::Helper::seekNodeByName(_root, name);
    if(_node != nullptr)
        _pos= _node->convertToWorldSpace( _node->getAnchorPointInPoints());
    return _pos;
}
bool StudioLayer::init(){
    if(Layer::init()){
        initWithFile(_fileName);
        return true;
    }
    return false;
}

void StudioLayer::initWithFile(string _filePath){
    _fileName = _filePath;
    uiAction = nullptr;
    if(_root != nullptr)
        _root->removeFromParent();
    _root = CSLoader::createNode(_fileName);
    if(_root != nullptr){
        _root->setContentSize(Director::getInstance()->getVisibleSize());
        Helper::doLayout(_root);
        addChild(_root);
        uiAction = CSLoader::createTimeline(_fileName, _root);
        if(uiAction != nullptr){
            _root->runAction(uiAction);
            uiAction->gotoFrameAndPause(0);
        }
    }
    
}

 void StudioLayer::initWithFileNoLayout(string _filePath){
     _fileName = _filePath;
     uiAction = nullptr;
     if(_root != nullptr)
         _root->removeFromParent();
     _root = CSLoader::createNode(_fileName);
     if(_root != nullptr){
         this->setContentSize(_root->getContentSize());
         addChild(_root);
         uiAction = CSLoader::createTimeline(_fileName, _root);
         if(uiAction != nullptr){
             _root->runAction(uiAction);
             uiAction->gotoFrameAndPause(0);
         }
     }

}

void StudioLayer::touchBtn(Ref* _ref,ui::Widget::TouchEventType _type){
    if(!isChange ){
        if(_type == Widget::TouchEventType::BEGAN){
            auto _btn = dynamic_cast<ui::Button*>(_ref);
            auto _check  = dynamic_cast<ui::CheckBox*>(_ref);
            if(_btn != nullptr || _check != nullptr){
                
                 dynamic_cast<Node*>(_ref)->stopActionByTag(521);
                ActionInterval *ret = NULL;
                float timeScale = 1.2;
                ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
                ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
                ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
                ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
                ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
                ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
                ret = Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
                ret->setTag(521);
                if(_btn != nullptr){
                
                    _btn->runAction(ret);
                }
                else if(_check != nullptr){
                     _check->runAction(ret);
                }
            }
        }
        if(_type ==Widget::TouchEventType::ENDED){
              clickBtn(dynamic_cast<Widget*>(_ref));
         
            auto _btn = dynamic_cast<Button*>(_ref);
            if(_btn != nullptr){
            auto _soundNode = ui::Helper::seekNodeByName(_btn, "Audio");
                if(_soundNode != nullptr){
                auto _soundPlayer = dynamic_cast<ComAudio*>(_soundNode->getComponent("Audio"));
                if(_soundPlayer != nullptr)
                    _soundPlayer->playEffect();
                    else
                        SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
                }else
               SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
            }
            if(_touchBtnCallback != nullptr)
                _touchBtnCallback(dynamic_cast<Widget*>(_ref));
        }
    }
}

void StudioLayer::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
    registLisnter(_root);
    isChange = false;
}
void StudioLayer::onExitTransitionDidStart(){
    isChange = true;
    Layer::onExitTransitionDidStart();
    removeLesner(_root);
}

void StudioLayer::registLisnter(Node* _node){
    if(_node != nullptr) {
        auto _widget = dynamic_cast<ui::Widget*>(_node);
        if(_widget != nullptr)
            _widget->addTouchEventListener(CC_CALLBACK_2(StudioLayer::touchBtn,this));
        for(auto _child:_node->getChildren())
            registLisnter(_child);
    }
}

void StudioLayer::removeLesner(Node* _node){
    if(_node != nullptr) {
        auto _btn = dynamic_cast<Widget*>(_node);
        if(_btn != nullptr)
            _btn->addTouchEventListener(nullptr);
        for(auto _child:_node->getChildren())
            registLisnter(_child);
    }
}

