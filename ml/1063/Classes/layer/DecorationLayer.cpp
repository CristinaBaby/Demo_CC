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
#include "WeddingFoodData.h"
#include "TouchScaleRoteComponent.h"
#include "MoveInVisibleRectComponent.h"
REGIST_SECENE(DecorationLayer);
const string DecorationLayer::_eventName= "Decoration page";
USING_NS_KD;

DecorationLayer::DecorationLayer(){
    categoryNames = WeddingFoodData::getDecorateCategore();
    _lockImage = "ui/dec/dec_bg2_lock.png";
    chooseIndex = -1;
    _icing = nullptr;
    clippingFruit = nullptr;

}

DecorationLayer::~DecorationLayer(){
  
    
}

bool DecorationLayer::init(){
    if(BaseDecorationLayer::init()){
        _operate = StudioLayer::create("DecorationLayer.csb");
        addChild(_operate);
        
    
        
        _operate->changeNode("text", WeddingFoodData::getDecorateTtitle());
        _prompt = _operate->getNodeByName<Sprite*>("finger");
        auto listLayer = StudioLayer::createNoLayout(WeddingFoodData::getListViewPath());
        auto viewBg1 =  _operate->getNodeByName("viewBg");
        viewBg1->addChild(listLayer);
        auto listViewBg = listLayer->getNodeByName<ui::Widget*>("Layer");
        listViewBg->setContentSize(Size(Director::getInstance()->getVisibleSize().width,listViewBg->getContentSize().height));
        ui::Helper::doLayout(listViewBg);
        _chooseBg = Sprite::create("ui/dec/dec_bg2_xuanzhong.png");
        _chooseBg->retain();
        
        auto foodCotainer = _operate->getNodeByName("foodCotainer");
        auto layer = StudioLayer::createNoLayout(WeddingFoodData::getEatLayerPath());
        foodCotainer->addChild(layer);
        auto foodC = layer->getNodeByName("food");
        auto s = Sprite::create(WeddingFoodData::getDecorationFood());
        s->setPosition(foodC->getContentSize()*.5);
        foodC->addChild(s,-2);
        
        clipping = ClippingNode::create();
        auto ss = Sprite::create(WeddingFoodData::getDecorationFood());
        ss->setPosition(_operate->getNodeByName("food")->getContentSize()*.5);
        clipping->setStencil(ss);
        clipping->setAlphaThreshold(0.01f);
        foodC->addChild(clipping,-1);
        
        
        _listView.selectClick = CC_CALLBACK_2(DecorationLayer::touchListView,this);
        _listView.unSelectClick = CC_CALLBACK_2(DecorationLayer::unSelect,this);
        _listView.setUserGary(true);
        string names[] = {"viewBg","btn_next","btn_reset"};
        
        for(int i=0;i<sizeof(names)/sizeof(names[0]);i++)
            _operate->getNodeByName(names[i])->setVisible(false);
        
        auto _decBg = _operate->getNodeByName("icon_bg");
        _decoView = MyScollView::create(148, 500);
        _decoView->setSpace(20);
        _decoView->setDirection(MyScollView::Direction::VERTICAL);
        _decoView->setPosition(_decBg->getContentSize()*.5);
        _decoView->setAnchorPoint(Vec2(0.5,0));
        _decoView->setPositionY(15);
        _decBg->addChild(_decoView);
        
        if(WeddingFoodData::getFoodIndex() == 2){
            clippingFruit = ClippingNode::create();
            foodC->addChild(clippingFruit);
            auto stenci = Sprite::create("png/Fruit Tart/4/send3.png");
            stenci->setPosition(Vec2(303.00,256.65));
            clippingFruit->setStencil(stenci);
            clippingFruit->setAlphaThreshold(0.01f);
            clippingFruit->setInverted(true);
            auto s2 = Sprite::create("png/Fruit Tart/4/send2.png");
            s2->setPosition(foodC->getContentSize()*.5);
            foodC->addChild(s);
        }
        _operate->getNodeByName("foodCotainer")->setVisible(false);
        _operate->getNodeByName("listView")->setVisible(false);
        return true;
    }
    return false;
}

void DecorationLayer::reset(){
    if(_icing != nullptr)
        _icing->removeFromParent();
    _icing = nullptr;
    _operate->getNodeByName("food_0")->removeAllChildren();
    _operate->changeNode("plates", "png/Fruit Tart/4/tray.png");
    _operate->getNodeByName("forks")->setVisible(false);
    SoundPlayer::getInstance()->playEffect("sound/UI/reset.mp3");
    WeddingFoodData::setPlatePath("");
    WeddingFoodData::setForksPath("");
    if(clippingFruit != nullptr)
        clippingFruit->removeAllChildren();
}

void  DecorationLayer::touchListView(ui::CheckBox* _check,int index){
    SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
    showCatagore();
    creatIcon(index);
    chooseIndex = -1;
}

void DecorationLayer::unSelect(ui::CheckBox*_check,int index){
       SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
    hideCatagore();
    chooseIndex = -1;
}

void DecorationLayer::freeClick(KDS::EventSprite*_s,size_t _index,KDS::MyScollView*){
       SoundPlayer::getInstance()->playEffect("sound/general/button_general.mp3");
    if(_icing != nullptr)
        _icing->disable();
    _operate->getNodeByName<ui::Widget*>("food_0")->setEnabled(false);
    for(auto child:_operate->getNodeByName("food_0")->getChildren()){
        auto com = child->getComponent("TouchScaleRoteComponent");
        if(com != nullptr)
            com->setEnabled(false);
        com = child->getComponent("MoveInVisibleRectComponent");
        if(com != nullptr)
            com->setEnabled(false);
    }
    
    vector<string> findNames = {"forks","plates","icing","cookie_fruit"};
    auto itor = std::find(findNames.begin(), findNames.end(), _currentcategory);
    if(itor == findNames.end()) {
        chooseIndex = _index;
        _operate->getNodeByName<ui::Widget*>("food_0")->setEnabled(true);
        _prompt->stopAllActions();
        _prompt->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0, 0),RotateTo::create(0.3, -13.5), DelayTime::create(0.7), nullptr)));
        _prompt->setVisible(true);
    }else if(_currentcategory=="icing") {
        auto  _decPath = WeddingFoodData::getDecIcing(_index);
        if(_icing != nullptr)
            _icing->removeFromParent();
        _icing = nullptr;
        if(_icing == nullptr){
            auto sprite = Sprite::create(_decPath);
            auto food = _operate->getNodeByName("food");
            _icing = ScribbleWithCallBack::create(sprite->getContentSize());
            _icing->setPosition(food->getContentSize()*.5);
            clipping->addChild(_icing);
            _icing->setAntiAliasingBrush(true);
            _icing->useBrush("png/cookies/6/hole_stencilsamll.png",Scribble::eBrush);
            _icing->callBack = [this](cocos2d::Touch *,ScribbleWithCallBack::TouchType _type){
                if(_type == ScribbleWithCallBack::TouchType::eTouchMoved){
                    this->stopPrompt();
                }
            };
        }
        _icing->useTarget(_decPath);
        _icing->enable();
        
        ActionHelper::runMoveHideAction(_prompt, _operate->getPosinWord("food")+Vec2(0,-160), _operate->getPosinWord("food")+Vec2(200,-160));
    } else {
        _eventDispatcher->setEnabled(false);
        string _decPath;
        if(_currentcategory == "plates")
            _decPath = StringUtils::format("png/cake/dec/plates/plates%zu.png",_index);
        else
            _decPath = CategoryReader::getCategoryPath(_currentcategory, _index);
        Node* endNode = nullptr;
        if(_currentcategory != "cookie_fruit")
            endNode = _operate->getNodeByName(_currentcategory);
        Vec2 endpos;
        if(endNode != nullptr)
            endpos = endNode->convertToWorldSpace(endNode->getContentSize()*.5);
        else
            endpos = _operate->getPosinWord("food_0")+Vec2(-60+rand()%60,-60+rand()%60);
        auto starPos = _s->convertToWorldSpace(_s->getContentSize()*.5);
        auto temp = Sprite::createWithSpriteFrame(_s->getSpriteFrame());
        temp->setPosition(starPos);
        addChild(temp,10);
        ccBezierConfig bezier;
        bezier.endPosition = endpos;
        bezier.controlPoint_1 = bezier.controlPoint_2 = Vec2(endpos.x,std::max(starPos.y,endpos.y)+50);
        if(_currentcategory == "plates"){
            WeddingFoodData::setPlatePath(_decPath);
            ActionHelper::delayFunc(0.1, this, [=](){
                temp->setZOrder(-1);
                CocosHelper::reAddNode(temp, _operate->getNodeByName("foodCotainer"));
                _actionManager->resumeTarget(temp);
            });
        }else if(_currentcategory == "forks")
            WeddingFoodData::setForksPath(_decPath);
        
        temp->runAction(Sequence::create(BezierTo::create(0.6, bezier),CallFunc::create([=](){
            SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
            if(endNode != nullptr){
                endNode->setVisible(true);
                endNode->getParent()->setVisible(true);
            }
            auto partilce = ParticleSystemQuad::create("particles/decorateParticle.plist");
            partilce->setPosition(temp->convertToWorldSpace(temp->getContentSize()*.5));
            this->addChild(partilce,30);
            SoundPlayer::getInstance()->playEffect("sound/general/draw_success.mp3");
            auto endfunc = [=](){
                for(auto child:_operate->getNodeByName("food_0")->getChildren()){
                    auto com = child->getComponent("TouchScaleRoteComponent");
                    if(com != nullptr)
                        com->setEnabled(true);
                    com = child->getComponent("MoveInVisibleRectComponent");
                    if(com != nullptr)
                        com->setEnabled(true);
                }
                if(_currentcategory == "cookie_fruit") {
                    auto scale = TouchScaleRoteComponent::create();
                    scale->setCloseButtonPath("ui/dec/zoombox/x.png");
                    scale->setEventButtonPath("ui/dec/zoombox/Y.png");
                    scale->setUiBgPath("ui/dec/zoombox/box.png");
                    auto s = Sprite::create(_decPath);
                    s->setPosition(_operate->getNodeByName("food_0")->convertToNodeSpace(temp->getPosition()));
                    auto limit =MoveInVisibleRectComponent::create();
                    auto rect = LQRect::create(CocosHelper::getNodeRectInWorld(_operate->getNodeByName("food_0")));
                    rect->origin = rect->origin+Vec2(0,25);
                     rect->size = rect->size-Size(0,100);
                    limit->setTarget(rect);
                    s->addComponent(limit);
                    s->addComponent(scale);
                    
                    _operate->getNodeByName("food_0")->addChild(s);
                }else
                    dynamic_cast<Sprite*>(endNode)->setTexture(_decPath);
                temp->removeFromParent();
                _eventDispatcher->setEnabled(true);
            };
            if(_currentcategory == "plates"){
                temp->setTexture(_decPath);
                temp->setScale(0.5);
                temp->runAction(Sequence::create(ScaleTo::create(0.4, 1),CallFunc::create(endfunc), nullptr));
            }else{
                endfunc();
            }
        }), nullptr));
    }
}

void  DecorationLayer::touchEnd(ui::Widget* widge){
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
        WeddingFoodData::saveFood(_render);
        auto  _endFunc = [this](){
            LQ_adLoadingLayer::loadingDoneCallback = nullptr;
            Director::getInstance()->pushScene(TransitionFade::create(0.8,EatLayer::createScene(),Color3B::WHITE));
            _listView.resetState();
            SoundPlayer::getInstance()->playEffect("sound/next.mp3");
        };
        if(IAPManager::getInstance()->isShowAds()){
            LQ_adLoadingLayer::loadingDoneCallback = _endFunc;
            LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
        }
        else
            _endFunc();
        hideCatagore();
        _listView.resetState();
        
    }else if(widge->getName() == "food_0"){
        if(chooseIndex != -1){
            stopPrompt();
            SoundPlayer::getInstance()->playEffect("sound/category_selection_1.mp3");
            auto _s = Sprite::create(CategoryReader::getCategoryPath(_currentcategory, chooseIndex));
            if(_s != nullptr){
                auto _pos= widge->convertToNodeSpace(widge->getTouchEndPosition());
                if(_currentcategory.find("cream") == string::npos)
                    _s->setRotation(rand()%360);
                float _endScale = 0.7+rand()%4*0.1;
                if(clippingFruit != nullptr){
                       _pos= _operate->getNodeByName("food")->convertToNodeSpace(widge->getTouchEndPosition());
                    clippingFruit->addChild(_s);
                }
                else
                    widge->addChild(_s);
                _s->setPosition(_pos);
                _s->setScale(0);
                _s->runAction(ScaleTo::create(0.2, _endScale));
            }
        }
    }
    
}

void  DecorationLayer::showEnd(){
    
}

void DecorationLayer::onExitTransitionDidStart(){
    BaseDecorationLayer::onExitTransitionDidStart();
}

void DecorationLayer::onEnterTransitionDidFinish(){
    BaseDecorationLayer::onEnterTransitionDidFinish();
       auto foodCotainer = _operate->getNodeByName("foodCotainer");
    if(foodCotainer->isVisible())
        return;
    auto listView = _operate->getNodeByName("listView");

    _listView.setContainer(listView);
    _listView.getListView()->setScrollBarOpacity(0);
    
    SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
    ActionHelper::showBezier(foodCotainer, foodCotainer->getPosition(), ActionHelper::ShowDirection::show_from_right,[this](){
        auto title = _operate->getNodeByName("text");
         SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3");
        ActionHelper::showBackInOut(title, title->getPosition(), ActionHelper::ShowDirection::show_from_top,[this,title](){
            auto node = Node::create();
            this->addChild(node,99999);
            auto _lis = EventListenerTouchOneByOne::create();
            _lis->onTouchBegan = [_lis,title](Touch*,Event*){
                _lis->setEnabled(false);
                ActionHelper::hide(title, ActionHelper::ShowDirection::show_from_top);
                return false;
            };
            _lis->setSwallowTouches(false);
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_lis, node);
            
        });
        title->setVisible(true);
        
        auto viewBg = _operate->getNodeByName("viewBg");
        ActionHelper::show(viewBg, viewBg->getPosition(), ActionHelper::ShowDirection::show_from_bottom,[this](){
                auto listView = _operate->getNodeByName("listView");
            ActionHelper::showBackInOut(listView,listView->getPosition(), ActionHelper::ShowDirection::show_from_left);
            listView->setVisible(true);
        });
        viewBg->setVisible(true);
        
        auto btn_reset = _operate->getNodeByName("btn_reset");
        ActionHelper::showBackInOut(btn_reset, btn_reset->getPosition(), ActionHelper::ShowDirection::show_from_left);
        btn_reset->setVisible(true);
        
        auto btn_next = _operate->getNodeByName("btn_next");
        ActionHelper::showBackInOut(btn_next, btn_next->getPosition(), ActionHelper::ShowDirection::show_from_right);
        btn_next->setVisible(true);
    });
    foodCotainer->setVisible(true);
}
void DecorationLayer::creatIcon(size_t _index){
    if(_chooseBg != nullptr)
        _chooseBg->removeFromParent();
    _currentcategory = categoryNames.at(_index);
    if(_decoView != nullptr){
        vector<string> filePath;
        if(_currentcategory == "icing"){
            filePath = WeddingFoodData::getIcingIcon();
        }else if(_currentcategory == "plates"){
            int num = FilePathUtil::getFileNum("png/cake/dec/icon/icon_plates");
            for(int i=0;i<num;++i)
                filePath.push_back(StringUtils::format("png/cake/dec/icon/icon_plates/plates%d.png",i));
        }else {
            filePath = CategoryReader::getIconPath(_currentcategory);
        }
        auto _adapter = MyBaseAdapter::create(filePath);
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
        _adapter->setItemClick(CC_CALLBACK_3(DecorationLayer::catagoreClick,this));
    }
    
}