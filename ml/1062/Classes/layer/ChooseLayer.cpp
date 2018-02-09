//
//  ChooseLayer.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/18.
//
//

#include "ChooseLayer.h"
#include "GameFood.h"
#include "MakeStep1.h"
#include "ShopLayer.hpp"
#include "LQ_adLoadingLayer.h"
#include "HomeLayer.h"
#include "MyDialog.h"
#include "TouchNodeComponent.h"
class ChooseAdapter: public ViewAdapter{
public:
    CREATE_ViewAdapter(ChooseAdapter);
    Node * getItem(int position){
        auto node = ViewAdapter::getItem(position);
        if(!IAPManager::getInstance()->isFree("foods",position+1)){
            auto lock = Sprite::create("lock.png");
            //lock->setAnchorPoint(Vec2(1,0));
            lock->setName("lock");
            lock->setPosition(node->getContentSize()*.5);
            lock->setPositionY(lock->getPositionY()-30);
            node->addChild(lock);
            lock->setScale(1.6);
        }
        return node;
    }
};


IMPLEMENT_COCOS2DX_CLASS(ChooseLayer);
bool ChooseLayer::init(){
    if(BaseStepLayer::init()){
        currentPage = 4;
        bookView = nullptr;
        _operate = StudioLayer::create("ChooseLayer.csb");
        addChild(_operate);
        _operate->getNodeByName("menu0")->setScale(0.7);
        auto bookNode = _operate->getNodeByName("book");
        auto filePath = GameFood::getInstance()->getChoosePath();
        currentPage = filePath.size();
        for(int i=0;i<filePath.size();i++){
            auto page = Sprite::create(filePath.at(i));
            if(i%2==0){
                page->setAnchorPoint(Vec2(0,0.5));
                page->setVisible(false);
                page->setZOrder(-i);
            }else {
                page->setAnchorPoint(Vec2(1,0.5));
                page->setZOrder(1);
            }
            page->setPosition(bookNode->getContentSize()*.5);
            page->setName(StringUtils::format("%d",i));
            bookNode->addChild(page);
        }
        _operate->getNodeByname<ParticleSystemQuad*>("particle")->stopSystem();
        return true;
    }
    return false;
}

void ChooseLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    SoundPlayer::getInstance()->playEffect("sound/menu_magnify.mp3");
    auto menu = _operate->getNodeByName("menu0");
    menu->runAction(Sequence::create(ScaleTo::create(0.5, 1),DelayTime::create(0.7), CallFunc::create([=]{
        menu->setVisible(false);
        _operate->getNodeByName("menu")->setVisible(true);
        pageControl();
        scheduleUpdate();
    }), nullptr));
    
}

void ChooseLayer::pageControl(){
    currentPage--;
    if(currentPage>=0){
        auto page = _operate->getNodeByName(StringUtils::format("%d",currentPage));
        float endScale;
        if(currentPage%2==0){
            page->setScaleX(0);
            page->setSkewY(10);
            endScale = 1;
        } else
            endScale = 0;
        page->setVisible(true);
        page->runAction(Sequence::create(ScaleTo::create(0.15, endScale,1),CallFunc::create(CC_CALLBACK_0(ChooseLayer::pageControl, this)), nullptr));
    }else {
        auto touch = TouchNodeComponent::create();
        touch->addListners(ComponentTouchEnd, [this](Node* node,Component*, OperateListner* _lis){
            auto point = node->convertToNodeSpace(dynAny_cast<Vec2>(_lis->getData()));
            Rect limit;
            limit.origin  = Vec2(20,20);
            limit.size = node->getContentSize() - Size(40,40);
            if(limit.containsPoint(point))
                _cfsys.showMoreGame();
        });
        _operate->getNodeByName("btn_more")->addComponent(touch);
        
        auto title = _operate->getNodeByName("tittle");
        ActionHelper::showTitle(title);
        SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
        unscheduleUpdate();
        auto bookNode = _operate->getNodeByName("book");
        bookNode->removeAllChildren();
        bookView = BookView::create(bookNode->getContentSize());
        bookView->setPosition(bookNode->getContentSize()*.5);
        bookNode->addChild(bookView);
        bookView->pageMove = [=](){
            this->stopPrompt();
            title->stopActionByTag(ActionHelper::titleActionTag);
        };
        _prompt = _operate->getNodeByname<Sprite*>("finger");
        ActionHelper::runMoveHideAction(_prompt, _prompt->getPosition(), _prompt->getPosition()+Vec2(-150,0));
        auto _adapter = ChooseAdapter::create(GameFood::getInstance()->getChoosePath());
        _adapter->setItemClick([=](Node* node,int index){
            if(!IAPManager::getInstance()->isFree("foods",index+1)){
                auto shop = ShopLayer::create();
                shop->_callBack = [this](){
                    if(IAPManager::getInstance()->isFree("foods",7)){
                        CocosHelper::visitAllNode(bookView, [](Node* node){
                            if(node->getName().compare("lock") == 0)
                                node->removeFromParent();
                        });
                    }
                };
                this->addChild(shop,200);
                return ;
            }
            
            _eventDispatcher->setEnabled(false);
            GameFood::getInstance()->setFoodIndex(index);
            auto particle = ParticleSystemQuad::create("particles/goodParticle.plist");
            particle->setPosition(node->getContentSize()*.5);
            node->addChild(particle);
            SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
            SoundPlayer::getInstance()->playEffect("sound/menu_magnify.mp3");
            _operate->getNodeByName("menu")->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 1.1),MoveBy::create(0.5, Vec2(0,-50)), nullptr),DelayTime::create(0.6),CallFunc::create([this](){
                Director::getInstance()->replaceScene(TransitionFade::create(0.8,MakeStep1::createScene(),Color3B::WHITE));
                _eventDispatcher->setEnabled(true);
            }), nullptr));
        });
        bookView->setAdapter(_adapter);
    }
}
void ChooseLayer::update(float){
    auto page = _operate->getNodeByName(StringUtils::format("%d",currentPage));
    if(page != nullptr){
        auto scaleX = page->getScaleX();
        float distance = 10;
        if(currentPage%2 == 1)
            distance = -10;
        if(scaleX>0.5)
            page->setSkewY((1-scaleX)/0.5*distance);
        else
            page->setSkewY(distance);
    }
}

void ChooseLayer::touchEnd(ui::Widget* widget){
    if(widget->getName() == "btn_more"){
        //_cfsys.showMoreGame();
    }else if(widget->getName() == "btn_home"){
        auto _dialog = MyDialog::create("Do you want to start over with a new game?", MyDialog::twoBtnPath);
        _dialog->dialogBtnClick =[this](MyDialog * dialog,int index){
            if(index == 0) {
                _operate->setIsChange(true);
                auto _endcallBack = [](){
                    //Director::getInstance()->popScene();
                    LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                    //Director::getInstance()->popToRootScene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.8,HomeLayer::createScene(),Color3B::WHITE));
                };
                if(IAPManager::getInstance()->isShowAds()){
                    LQ_adLoadingLayer::loadingDoneCallback = _endcallBack;
                    LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
                } else
                    _endcallBack();
                
            }
        };
        
        getScene()->addChild(_dialog);

    }
}