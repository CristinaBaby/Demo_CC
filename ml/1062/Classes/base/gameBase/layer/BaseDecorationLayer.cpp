//
//  BaseDecorationLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#include "BaseDecorationLayer.h"
#include "MyDialog.h"
#include "TouchScaleRoteComponent.h"
#include "CategoryReader.h"
#include "BaseShopLayer.h"
USING_NS_KD;

const string BaseDecorationLayer:: _lockName = "_lockName";

BaseDecorationLayer::BaseDecorationLayer():
_showIconAciton("showIcon")
,_hideIconAction("hideIcon")
,resetName("btn_reset")
,resetMessage("Are you sure you want to reset?")
,_decoView(nullptr)
,_lockImage("lock.png")
,_currentcategory("")
,shopClassName("ShopLayer")
,_chooseBg(nullptr){
    
}

 BaseDecorationLayer::~BaseDecorationLayer(){
    CC_SAFE_RELEASE_NULL(_chooseBg);
}

void BaseDecorationLayer::touchEnd(ui::Widget* widget){
    if(resetName.compare(widget->getName()) == 0){
        auto _dailog = MyDialog::create(resetMessage, MyDialog::twoBtnPath);
        _dailog->dialogBtnClick = [&](MyDialog * dialog,int index){
            if(index == 0)
                reset();
        };
        getScene()->addChild(_dailog);
    }
}

void BaseDecorationLayer::creatIcon(int _index){
    if(_chooseBg != nullptr)
        _chooseBg->removeFromParent();
    _currentcategory = categoryNames.at(_index);
    if(_decoView != nullptr){
        auto _adapter = MyBaseAdapter::create(CategoryReader::getIconPath(_currentcategory));
        _decoView->changeAdapter(_adapter);
        
        for(int i=0;i<_decoView->nodeCache.size();i++){
            if(!IAPManager::getInstance()->isFree(_currentcategory,i+1)){
                auto _node = _decoView->nodeCache.at(i);
                auto _lock = Sprite::create(_lockImage);
                _lock->setName(_lockName);
                _lock->setAnchorPoint(Vec2(1,0));
                _lock->setPosition(_node->getContentSize().width, 0);
                _node->addChild(_lock);
            }
        }
        if(_decoView->getDirection() ==MyScollView::Direction::VERTICAL)
            _decoView->getContainer()->setPosition(  _decoView->getContainer()->getPosition()+Vec2(0,400));
        else
            _decoView->getContainer()->setPosition(  _decoView->getContainer()->getPosition()+Vec2(-400,0));
        _adapter->setItemClick(CC_CALLBACK_3(BaseDecorationLayer::catagoreClick,this));
    }
    
}

void BaseDecorationLayer::showCatagore(){
    if(_operate != nullptr)
        _operate->playAction(_showIconAciton,[&](){
            FiniteTimeAction* moveAction;
            if(_decoView->getDirection() ==MyScollView::Direction::VERTICAL)
                moveAction = MoveBy::create(0.5, Vec2(0,-400));
            else
                moveAction = MoveBy::create(0.5, Vec2(400,0));
            _decoView->getContainer()->runAction(Sequence::create(moveAction, CallFunc::create([&](){
                for(int i=0;i<_decoView->nodeCache.size();i++){
                    auto _node = _decoView->nodeCache.at(i);
                    _node->runAction(ActionHelper::getJellyAction());
                }
            }), nullptr));
            this->showEnd();
        });
}

void BaseDecorationLayer::hideCatagore(){
    if(_operate != nullptr)
        _operate->playAction(_hideIconAction,CC_CALLBACK_0(BaseDecorationLayer::hideEnd, this));
}

void BaseDecorationLayer::catagoreClick(EventSprite* s,int _index,MyScollView* scol) {
    if(IAPManager::getInstance()->isFree(_currentcategory,_index+1)){
        if(_chooseBg != nullptr){
            _chooseBg->removeFromParent();
            _chooseBg->setPosition(s->getContentSize()*.5);
            s->addChild(_chooseBg,-1);
        }
        SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
        freeClick(s,_index,scol);
    }
    else
        gotoShop();
}

void  BaseDecorationLayer::freeClick(KDS::EventSprite*,int _index,KDS::MyScollView*){
    
}

void BaseDecorationLayer::gotoShop(){
    auto _shopLayer = dynamic_cast<BaseShopLayer*>(DynObject::createDynObject(shopClassName));
    if(_shopLayer != nullptr){
        
        _shopLayer->_callBack = [&](){
            for(int i=0;i<_decoView->nodeCache.size();i++)
                if(IAPManager::getInstance()->isFree(_currentcategory,i+1)){
                    _decoView->nodeCache.at(i)->removeChildByName(_lockName);
                }
        };
        getScene()->addChild(_shopLayer);
    }
}

void BaseDecorationLayer::hideUi(Node* _root){
    if(_root != nullptr){
        auto _com = dynamic_cast<TouchScaleRoteComponent*>(_root->getComponent("TouchScaleRoteComponent"));
        if(_com != nullptr)
            _com->hideUI();
        for(auto _child:_root->getChildren())
            hideUi(_child);
    }
}

void BaseDecorationLayer::saveImage(Node* _node,Size _size,string _name){
    if(_node != nullptr) {
        hideUi(_node);
        auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        _render->begin();
        _node->visit();
        _render->end();
        Director::getInstance()->getRenderer()->render();
        _render->saveToFile(_name);
        Director::getInstance()->getRenderer()->render();
    }
}

void BaseDecorationLayer::onEnterTransitionDidFinish() {
    BaseStepLayer::onEnterTransitionDidFinish();
    if(_operate != nullptr){
        auto _next = _operate->getNodeByName("btn_next");
        if(_next != nullptr)
            _next->stopAllActions();
           
    }
}
