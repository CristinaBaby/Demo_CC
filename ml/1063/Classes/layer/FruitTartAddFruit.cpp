//
//  DecorationLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "FruitTartAddFruit.h"
#include "MoveSprite.h"
#include "TouchScaleRoteComponent.h"
#include "MoveInVisibleRectComponent.h"
#include "CategoryReader.h"
#include "LQ_adLoadingLayer.h"
#include "DecorationLayer.h"
#include "FilePathUtil.h"
#include "WeddingFoodData.h"
#include "ChangeLoading.h"
REGIST_SECENE(FruitTartAddFruit);
const string FruitTartAddFruit::_eventName= "Decoration page";
USING_NS_KD;

FruitTartAddFruit::FruitTartAddFruit(){
    categoryNames.push_back("fruit");
     _lockImage = "ui/dec/dec_bg2_lock.png";
    chooseIndex = -1;
}

FruitTartAddFruit::~FruitTartAddFruit(){
    
}

bool FruitTartAddFruit::init(){
    if(BaseDecorationLayer::init()){
        _operate = StudioLayer::create("FruitTartAddFruit.csb");
        addChild(_operate);
        _prompt = _operate->getNodeByName<Sprite*>("finger");
        _chooseBg = Sprite::create("ui/dec/dec_bg2_xuanzhong.png");
        _chooseBg->retain();
        
        auto _decBg = _operate->getNodeByName("icon_bg");
        _decoView = MyScollView::create(148, 500);
        _decoView->setSpace(20);
        _decoView->setDirection(MyScollView::Direction::VERTICAL);
        _decoView->setPosition(_decBg->getContentSize()*.5);
        _decoView->setAnchorPoint(Vec2(0.5,0));
        _decoView->setPositionY(15);
        _decBg->addChild(_decoView);
        
        auto food = _operate->getNodeByName("food");
        clipping = ClippingNode::create();
        food->addChild(clipping);
        auto stenci = Sprite::create("png/Fruit Tart/4/send3.png");
        stenci->setPosition(Vec2(222.00,241.20));
        clipping->setStencil(stenci);
        clipping->setAlphaThreshold(0.01f);
        clipping->setInverted(true);
        auto s = Sprite::create("png/Fruit Tart/4/send2.png");
        s->setPosition(food->getContentSize()*.5);
        food->addChild(s);
        return true;
    }
    return false;
}

void FruitTartAddFruit::reset(){
    SoundPlayer::getInstance()->playEffect("sound/Sound/reset.mp3");
}

void FruitTartAddFruit::freeClick(KDS::EventSprite*_s,size_t _index,KDS::MyScollView*){
           SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
    _prompt->stopAllActions();
    _prompt->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0, 0),RotateTo::create(0.3, -13.5), DelayTime::create(0.7), nullptr)));
    _prompt->setVisible(true);
    chooseIndex = _index;
}

void  FruitTartAddFruit::touchEnd(ui::Widget* widge){
    BaseDecorationLayer::touchEnd(widge);
    if(widge->getName().compare("btn_next") == 0){
        auto food = _operate->getNodeByName("food");
        auto _size = food->getContentSize();
        auto pos = food->getPosition();
        auto ar = food->getAnchorPoint();
        hideUi(food);
        food->setAnchorPoint(Vec2::ZERO);
        food->setPosition(Vec2::ZERO);
        auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        _render->begin();
        food->visit();
        _render->end();
        Director::getInstance()->getRenderer()->render();
        food->setAnchorPoint(ar);
        food->setPosition(pos);
        WeddingFoodData::saveAddFruitFritTart(_render);
        SoundPlayer::getInstance()->playEffect("sound/next.mp3");
        ChangeLoading::loading([](){
            Director::getInstance()->replaceScene(TransitionFade::create(0.8,DecorationLayer::createScene(),Color3B::WHITE));
        });
    }else if(widge->getName() == "fruitLayer"){
        if(chooseIndex != -1){
            stopPrompt();
             SoundPlayer::getInstance()->playEffect("sound/category_selection_1.mp3");
            auto _s = Sprite::create(CategoryReader::getCategoryPath(_currentcategory, chooseIndex));
            if(_s != nullptr){
                auto _pos= _operate->getNodeByName("food")->convertToNodeSpace(widge->getTouchEndPosition());
                 float _endScale = 0.7+rand()%4*0.1;
                clipping->addChild(_s);
                _s->setPosition(_pos);
                _s->setScale(0);
                _s->runAction(ScaleTo::create(0.2, _endScale));
            }
        }
    }
}

void FruitTartAddFruit::onExitTransitionDidStart(){
    BaseDecorationLayer::onExitTransitionDidStart();
}

void FruitTartAddFruit::onEnterTransitionDidFinish(){
    BaseDecorationLayer::onEnterTransitionDidFinish();
    auto title = _operate->getNodeByName("text");
    ActionHelper::showBackInOut(title, title->getPosition(), ActionHelper::ShowDirection::show_from_top);
    title->setVisible(true);
    auto foodCotainer = _operate->getNodeByName("foodCotainer");
    SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
    SoundPlayer::getInstance()->playEffect("sound/note_swish.mp3");
    SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
    ActionHelper::showBezier(foodCotainer, foodCotainer->getPosition(), ActionHelper::ShowDirection::show_from_left,[this](){
        creatIcon(0);
        showCatagore();
        SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
        auto btn_reset = _operate->getNodeByName("btn_reset");
        if(btn_reset != nullptr) {
            ActionHelper::showBackInOut(btn_reset, btn_reset->getPosition(), ActionHelper::ShowDirection::show_from_left);
            btn_reset->setVisible(true);
        }
        
        auto btn_next = _operate->getNodeByName("btn_next");
        ActionHelper::showBackInOut(btn_next, btn_next->getPosition(), ActionHelper::ShowDirection::show_from_right);
        btn_next->setVisible(true);
    });
    foodCotainer->setVisible(true);
}