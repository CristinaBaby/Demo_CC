//
//  WheelView.cpp
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/18.
//
//

#include "WheelView.h"
USING_NS_KD;
#include "SoundPlayer.h"

WheelView::WheelView(){
    _roteDistance = 0;
    _itemClick = nullptr;
    _evnetTyp = WheelEventType::type_null;
    curentIndex = -1;
    isDrag = false;
    isChange = false;
    _randitem = nullptr;
}

void WheelView::setPositionR(float _r){
    _positionR = _r;
}

void WheelView::addItems(vector<string> _paths,vector<string> _selectPath){
    Vector<CheckBox*> _sprites;
    for(int i=0;i<_paths.size();i++)
        _sprites.pushBack(CheckBox::create(_paths.at(i),_selectPath.at(i)));
    addItems(_sprites);
   
}

void WheelView::addItems(Vector<KDS::CheckBox*> _items){
    this->_items = _items;
    Vec2 _fisrtVec = Vec2(0,_positionR);//起始向量
    for(int i=0;i<_items.size();i++){
        auto _node = _items.at(i);
        addChild(_node);
        _node->selectCallBack = CC_CALLBACK_2(WheelView::click,this);
        
       float _rote = i*360/(_items.size()*1.0);
        float cosRote = cos(-3.1415926*_rote/180.0);//数学计算中逆时针为正方向，所以要取反
        float sinRote = sin(-3.1415926*_rote/180.0);
        Vec2 _roteVec = Vec2(_fisrtVec.x*cosRote-_fisrtVec.y*sinRote, _fisrtVec.x*sinRote+_fisrtVec.y*cosRote);//旋转后的向量
        auto _pos = Vec2(getContentSize()*.5) + _roteVec;
        _node->setRotation(_rote);
        _node->setPosition(_pos);
    }
}

void WheelView::click(CheckBox* _item,CHECK_TYPE _type){
    if(_evnetTyp == WheelEventType::type_move || isChange) {
        _item->changeState();
        return;
    }
       int clickIndex = -1;
    for (int i=0;i<_items.size();i++)
        if(_items.at(i) == _item) {
            clickIndex = i;
            break;
        }
    if(_type == CHECK_TYPE::CHECK_TYPE_SELECT){
        if(curentIndex != -1)
            _items.at(curentIndex)->setCheckType(CHECK_TYPE::CHECK_TYPE_UNSELECTED);
        curentIndex = clickIndex;
        auto _rote =(360+(int)(getRotation()+_items.at(curentIndex)->getRotation())%360)%360;
        if(_rote>180)
            _rote= 360-_rote;
        else
            _rote = -_rote;
        runAction(RotateBy::create(0.3, _rote));
    } else
        curentIndex = -1;
    if(_itemClick != nullptr)
        _itemClick(_items.at(clickIndex),clickIndex,_type);
    
    
}

bool WheelView::onTouchBegan(Touch* _touch,Event* _event) {
    if(EventSprite::onTouchBegan(_touch, _event) && !isChange) {
        stopAllActions();
        _roteDistance = 0;
         isDrag = true;
        _evnetTyp = WheelEventType::type_null;
        touchRote = getRotation();
        return true;
    }
    return false;
}
void WheelView::onTouchMoved(Touch* _touch,Event* _event){
    EventSprite::onTouchMoved(_touch, _event);
    auto _centerToPre = convertToNodeSpace(_touch->getPreviousLocation())-Vec2(getContentSize()*.5);
    auto _centerToCurrent = convertToNodeSpace(_touch->getLocation())- Vec2(getContentSize()*.5);
    auto _rote = _centerToCurrent.getAngle(_centerToPre)/3.1415926*180;
    
    if(abs(touchRote-getRotation())>60){
        touchRote = getRotation();
        SoundPlayer::getInstance()->playEffect("sound/luckydraw_run.mp3");
    }
    if(_evnetTyp != WheelEventType::type_move){
        _rote = _centerToCurrent.getAngle(convertToNodeSpace(_touch->getStartLocation())-Vec2(getContentSize()*.5))/3.1415926*180;
        if(abs(_rote)>5)
            _evnetTyp = WheelEventType::type_move;
        else
            _evnetTyp = WheelEventType::type_click;
    }
    if(_evnetTyp == WheelEventType::type_move) {
        _roteDistance = _rote;
        setRotation(getRotation()+_rote);
    
    }
}

void WheelView::onTouchEnded(Touch* _touch,Event* _event){
    EventSprite::onTouchEnded(_touch, _event);
    isDrag = false;
    if(_evnetTyp == WheelEventType::type_move)
        schedule(schedule_selector(WheelView::deaccelerateRoteing));
}

void WheelView::onTouchCancle(Touch* _touch,Event* _event) {
    isDrag = false;
}

void WheelView::deaccelerateRoteing(float){
    if(isDrag){
        unschedule(schedule_selector(WheelView::deaccelerateRoteing));
        return;
    }
    setRotation(getRotation()+_roteDistance);
    _roteDistance = _roteDistance*0.85;
    if(abs(_roteDistance)<=1){
        SoundPlayer::getInstance()->playEffect("sound/luckydraw_stop.mp3");
        unschedule(schedule_selector(WheelView::deaccelerateRoteing));
    }
}

void WheelView::randIndex(){
    if(_items.size() == 0 || isChange)
        return;
    isChange = true;
    changeTime = 0;
    endIndex = rand()%_items.size();
    runAction(Sequence::create(DelayTime::create(changeTime), CallFunc::create(CC_CALLBACK_0(WheelView::changeTex, this)),nullptr));
}

void WheelView::changeTex(){
    if(curentIndex != -1)
        _items.at(curentIndex)->setCheckType(CHECK_TYPE::CHECK_TYPE_UNSELECTED);
    curentIndex = (curentIndex+1)%_items.size();
    _items.at(curentIndex)->setCheckType(CHECK_TYPE::CHECK_TYPE_SELECT);
    changeTime++;

    if(changeTime>=57+endIndex){
        auto _rote =(360+(int)(getRotation()+_items.at(curentIndex)->getRotation())%360)%360;
        if(_rote>180)
            _rote= 360-_rote;
        else
            _rote = -_rote;
        runAction(Sequence::create(RotateBy::create(0.3, _rote), CallFunc::create([&](){
             SoundPlayer::getInstance()->playEffect("sound/luckydraw_stop.mp3");
            if(_randitem != nullptr)
                _randitem(_items.at(curentIndex),curentIndex,CHECK_TYPE::CHECK_TYPE_SELECT);
              isChange = false;
        }), nullptr));
        
    } else if(changeTime>50+endIndex){
        musicJudge =0;
            SoundPlayer::getInstance()->playEffect("sound/luckydraw_run.mp3");
        runAction(Sequence::create(DelayTime::create(0.1+0.45/7.0*(changeTime-50-endIndex)), CallFunc::create(CC_CALLBACK_0(WheelView::changeTex, this)),nullptr));
    } else if(changeTime>5){
        musicJudge++;
        if(musicJudge%2==1)
            SoundPlayer::getInstance()->playEffect("sound/luckydraw_run.mp3");
        runAction(Sequence::create(DelayTime::create(0.05), CallFunc::create(CC_CALLBACK_0(WheelView::changeTex, this)),nullptr));
    } else if(changeTime<=5){
        musicJudge=0;
            SoundPlayer::getInstance()->playEffect("sound/luckydraw_run.mp3");
         runAction(Sequence::create(DelayTime::create(0.5-(0.45/5.0)*changeTime), CallFunc::create(CC_CALLBACK_0(WheelView::changeTex, this)),nullptr));
    }
}

void WheelView::randRote(){
    if(_items.size() == 0)
        return;
    stopAllActions();
    auto _index = rand()%_items.size();
    auto _curentRote = (int)getRotation()%360;
    auto _nodeRote = _items.at(_index)->getRotation();
    _roteDistance = 720*5 - (int)(_nodeRote+_curentRote)%360;
    runAction(EaseExponentialInOut::create(RotateBy::create(2, _roteDistance)));
}

void WheelView::setRote(float _degree,float _time){
    FiniteTimeAction *roteAciton,*endAction;
    roteAciton = RotateTo::create(_time, _degree);
    endAction = CallFunc::create(CC_CALLBACK_0(WheelView::stopRoteAnimal, this));
    this->runAction(Sequence::create(roteAciton,endAction ,nullptr));
    this->schedule(schedule_selector(WheelView::Roteing));
}

void WheelView::Roteing(float){
    
}

void WheelView::stopRoteAnimal() {
    unschedule(schedule_selector(WheelView::Roteing));
}

WheelView* WheelView::create(const std::string& filename){
    auto *sprite = new (std::nothrow) WheelView();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->_positionR = sprite->getContentSize().width*.5;
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
    
}