//
//  DecorationLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "DecorationLayer.h"
#include "MoveSprite.h"
#include "TouchScaleRoteComponent.h"
#include "MoveInVisibleRectComponent.h"
#include "CategoryReader.h"
#include "LQ_adLoadingLayer.h"
#include "EatLayer.h"
#include "FilePathUtil.h"
#include "GameFood.h"
const string DecorationLayer::_eventName= "Decoration page";
USING_NS_KD;

IMPLEMENT_COCOS2DX_CLASS(DecorationLayer);

DecorationLayer::DecorationLayer(){

    categoryNames.push_back("sauce");
    categoryNames.push_back("drink");
    categoryNames.push_back("side-dish");
    categoryNames.push_back("plate");
    _lockImage = "icon_lock.png";
}

DecorationLayer::~DecorationLayer(){
    
    
    
}

bool DecorationLayer::init(){
    if(BaseStepLayer::init()){
        _operate = StudioLayer::create("DecorationLayer.csb");
        addChild(_operate);
        _operate->changeNode("food", GameFood::getInstance()->getDecorationPath());
        _listView.selectClick = CC_CALLBACK_2(DecorationLayer::touchListView,this);
        _listView.unSelectClick = CC_CALLBACK_2(DecorationLayer::unSelect,this);
        
        string names[] = {"btn_reset","btn_next","viewBg","plate","cup","sauce_bowl","side-dishplate"};
        
        for(int i=0;i<sizeof(names)/sizeof(names[0]);i++)
            _operate->getNodeByName(names[i])->setVisible(false);
        
        auto _decBg = _operate->getNodeByName("icon_bg");
        _decoView = MyScollView::create(141, 480);
        _decoView->setSpace(20);
        _decoView->setMarginBottom(80);
        _decoView->setDirection(MyScollView::Direction::VERTICAL);
        _decoView->setPosition(_decBg->getContentSize()*.5);
        _decBg->addChild(_decoView);
        
        decoration.insert("food", _operate->getNodeByName("food"));
        return true;
    }
    return false;
}

void DecorationLayer::reset(){
    string names[] = {"cup","sauce_bowl","side-dishplate"};
    for(int i=0;i<sizeof(names)/sizeof(names[0]);i++)
        _operate->getNodeByName(names[i])->setVisible(false);
    decoration.clear();
    decoration.insert("food", _operate->getNodeByName("food"));
    _operate->changeNode("plate", "png/5/plate0.png");
    SoundPlayer::getInstance()->playEffect("sound/Sound/reset.mp3");
    
}

void  DecorationLayer::touchListView(ui::CheckBox* _check,int index){
    SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
    showCatagore();
    creatIcon(index);
}

void DecorationLayer::unSelect(ui::CheckBox*_check,int index){
    SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
    hideCatagore();
}



void DecorationLayer::freeClick(KDS::EventSprite* _s,int _index,KDS::MyScollView*){
    _eventDispatcher->setEnabled(false);
    auto  _decPath = CategoryReader::getCategoryPath(_currentcategory, _index);
    auto endNode = _operate->getNodeByName(_currentcategory);
    auto endpos = endNode->convertToWorldSpace(endNode->getContentSize()*.5);
    auto starPos = _s->convertToWorldSpace(_s->getContentSize()*.5);
    auto temp = Sprite::createWithSpriteFrame(_s->getSpriteFrame());
    temp->setPosition(starPos);
    addChild(temp,10);
    ccBezierConfig bezier;
    bezier.endPosition = endpos;
    bezier.controlPoint_1 = bezier.controlPoint_2 = Vec2(starPos.x,std::max(starPos.y,endpos.y)+50);
    if(_currentcategory == "plate"){
        ActionHelper::delayFunc(0.3, this, [=](){
            temp->setZOrder(-1);
            CocosHelper::reAddNode(temp, _operate->getNodeByName("food"));
            _actionManager->resumeTarget(temp);
        });
    }
    temp->runAction(Sequence::create(BezierTo::create(0.6, bezier),CallFunc::create([=](){
          SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
        auto itor = decoration.find(endNode->getName());
        if(itor != decoration.end())
            decoration.erase(itor);
        decoration.insert(endNode->getName(), dynamic_cast<Sprite*>(endNode));
       
        endNode->setVisible(true);
        endNode->getParent()->setVisible(true);
        auto partilce = ParticleSystemQuad::create("particles/decorateParticle.plist");
        partilce->setPosition(temp->convertToWorldSpace(temp->getContentSize()*.5));
        this->addChild(partilce,30);
        auto endfunc = [=](){
            temp->removeFromParent();
            dynamic_cast<Sprite*>(endNode)->setTexture(_decPath);
            _eventDispatcher->setEnabled(true);
        };
        if(_currentcategory == "plate"){
            temp->setTexture(_decPath);
            temp->setScale(0.5);
            temp->runAction(Sequence::create(ScaleTo::create(0.4, 1),CallFunc::create(endfunc), nullptr));
        }else{
            endfunc();
        }
    }), nullptr));
}

void  DecorationLayer::touchEnd(ui::Widget* widge){
    BaseDecorationLayer::touchEnd(widge);
    if(widge->getName().compare("btn_next") == 0){
        
        auto  _endFunc = [this](){
            LQ_adLoadingLayer::loadingDoneCallback = nullptr;
            auto _step = EatLayer::create();
            _step->setFoods(decoration);
            auto _scen = Scene::create();
            _scen->addChild(_step);
            Director::getInstance()->pushScene(TransitionFade::create(0.8,_scen,Color3B::WHITE));
            _listView.resetState();
            SoundPlayer::getInstance()->playEffect("sound/next.mp3");
        };
        if(IAPManager::getInstance()->isShowAds()){
            LQ_adLoadingLayer::loadingDoneCallback = _endFunc;
            LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
        }
        else
            _endFunc();
        
    }
}

void  DecorationLayer::showEnd(){
    
}

void DecorationLayer::onExitTransitionDidStart(){
    BaseDecorationLayer::onExitTransitionDidStart();
}

void DecorationLayer::onEnterTransitionDidFinish(){
    BaseDecorationLayer::onEnterTransitionDidFinish();

    _listView.setContainer(_operate->getNodeByName("listView"));
    _listView.getListView()->setScrollBarOpacity(0);
    
    auto plate = _operate->getNodeByName("plate");
    if(plate->isVisible())
        return;
     SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    SoundPlayer::getInstance()->playEffect("sound/note_swish.mp3");
    ActionHelper::showBezier(plate, plate->getPosition(), ActionHelper::ShowDirection::show_from_right,[this](){
        auto viewBg = _operate->getNodeByName("viewBg");
        ActionHelper::showBackInOut(viewBg, viewBg->getPosition(), ActionHelper::ShowDirection::show_from_left);
        viewBg->setVisible(true);
        
        auto btn_reset = _operate->getNodeByName("btn_reset");
        ActionHelper::showBackInOut(btn_reset, btn_reset->getPosition(), ActionHelper::ShowDirection::show_from_right);
        btn_reset->setVisible(true);
        
        auto btn_next = _operate->getNodeByName("btn_next");
        ActionHelper::showBackInOut(btn_next, btn_next->getPosition(), ActionHelper::ShowDirection::show_from_top);
        btn_next->setVisible(true);
    });
    plate->setVisible(true);
}