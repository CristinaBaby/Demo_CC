//
//  CheckBox.cpp
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/19.
//
//

#include "CheckBox.h"
using namespace std;
USING_NS_KD;


 CheckBox::~CheckBox(){
     CC_SAFE_RELEASE_NULL(nomalTex);
     CC_SAFE_RELEASE_NULL(selectTex);
     CC_SAFE_RELEASE_NULL(_polygon);
}

CheckBox::CheckBox(){
    nomalTex = nullptr;
    selectTex = nullptr;
    selectCallBack = nullptr;
    _polygon = nullptr;
    _check_type = CHECK_TYPE::CHECK_TYPE_UNSELECTED;
}

CheckBox* CheckBox::create(std::string _nomalPath, std::string  _selectPath){
    auto _ps = new CheckBox();
    if(_ps &&_ps->initWithFile(_nomalPath)) {
        _ps->loadTexture(_nomalPath, _selectPath);
        _ps->autorelease();
        return _ps;
    }
    CC_SAFE_RELEASE_NULL(_ps);
    return nullptr;
}

void CheckBox::setCheckType(CHECK_TYPE _type){
    _check_type = _type;
    switch (_check_type) {
        case CHECK_TYPE::CHECK_TYPE_UNSELECTED:
            setTexture(nomalTex);
            break;
        case CHECK_TYPE::CHECK_TYPE_SELECT:
            setTexture(selectTex);
            break;
        default:
            break;
    }
}



bool CheckBox::isTouchTex(Touch *pTouch){
    auto _flag = isContain(pTouch);
    if(_flag) {
        auto _p = convertToNodeSpace(pTouch->getLocation());
        if(nullptr != _polygon)
            _flag = _polygon->containsPoint(_p);
    }
    return _flag;
}

void CheckBox::changeState(){
    switch (_check_type) {
        case CHECK_TYPE::CHECK_TYPE_SELECT:
            setCheckType(CHECK_TYPE::CHECK_TYPE_UNSELECTED);
            break;
        case CHECK_TYPE::CHECK_TYPE_UNSELECTED:
            setCheckType(CHECK_TYPE::CHECK_TYPE_SELECT);
            break;
        default:
            break;
    }
}

bool CheckBox::onTouchBegan(Touch *pTouch, Event *pEvent){
    if(!isContain(pTouch)||!isVisible())
        return false;
    if(!isTouchTex(pTouch))
        return false;
    if(touchSpriteBegan != nullptr){
        bool b =  touchSpriteBegan(this,_tag,pTouch);
        return b;
    }
    return true;

}

void CheckBox::onTouchEnded(Touch *pTouch, Event *pEvent){
    if(isTouchTex(pTouch)) {
        changeState();
        if(nullptr != selectCallBack)
            selectCallBack(this,_check_type);
    }
}

bool CheckBox::init(){
    return EventSprite::init();
}

void CheckBox::loadTexture(std::string _nomalPath, std::string  _selectPath) {
    setNomalTex(Director::getInstance()->getTextureCache()->addImage(_nomalPath));
    setSelectTex(Director::getInstance()->getTextureCache()->addImage(_selectPath));
}

void CheckBox::setNomalTex(Texture2D* _tex){
    _tex->retain();
    CC_SAFE_RELEASE_NULL(nomalTex);
    nomalTex = _tex;
}
void CheckBox::setSelectTex(Texture2D* _tex){
    _tex->retain();
    CC_SAFE_RELEASE_NULL(selectTex);
    selectTex = _tex;
}