//
//  DecorationLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "DecorationLayer.hpp"
#include "MoveSprite.h"
#include "TouchScaleRoteComponent.h"
#include "MoveInVisibleRectComponent.h"
#include "CategoryReader.h"
#include "LQ_adLoadingLayer.h"
#include "EatLayer.hpp"
#include "FilePathUtil.h"
#include "FlurryEventManager.h"
const string DecorationLayer::_eventName= "Decoration page";
USING_NS_KD;

IMPLEMENT_COCOS2DX_CLASS(DecorationLayer);

DecorationLayer::DecorationLayer(){
       categoryNames.push_back("icing");
    categoryNames.push_back("sauce");
    categoryNames.push_back("candy");
    categoryNames.push_back("nuts");
    categoryNames.push_back("other");
    auto _suaceIonc = CategoryReader::getIconPath("sauce");
    string DecorationLayerSauceNames[] = {
        "apple","blueberries","cherry","chocolate","grapes","Kiwi-fruit"
        ,"oranges","pineapple","strawberry","vanilla"
    };
    int i =0;
    int j = 0;
    for(;i<5;i++){
        _suaceNames.push_back(DecorationLayerSauceNames[i]);
        _suaceDecos.push_back("");
    }
    for(;j<8;j++){
        _suaceNames.push_back(_suaceIonc.at(j));
        _suaceDecos.push_back(CategoryReader::getCategoryPath("sauce", j));
    }
   
    for(;i<sizeof(DecorationLayerSauceNames)/sizeof(DecorationLayerSauceNames[0]);i++){
        _suaceNames.push_back(DecorationLayerSauceNames[i]);
          _suaceDecos.push_back("");
    }
    for(;j<_suaceIonc.size();j++){
        _suaceNames.push_back(_suaceIonc.at(j));
        _suaceDecos.push_back(CategoryReader::getCategoryPath("sauce", j));
    }
    _icing = nullptr;
    _sauce = nullptr;
    _decPath = "";
}

DecorationLayer::~DecorationLayer(){
    
    
    
}

bool DecorationLayer::init(){
    if(BaseStepLayer::init()){
        UserDefault::getInstance()->setIntegerForKey("playIndex",1);
        UserDefault::getInstance()->flush();
        _clipp = nullptr;
        _suaceIs = false;
        _prompt = Sprite::create("ui/pubilc/finger1.png");
        _prompt->setVisible(false);
        addChild(_prompt,300);
        _chooseBg = Sprite::create("ui/pubilc/xuanzhong.png");
        _chooseBg->retain();
        _operate = StudioLayer::create("DecorationLayer.csb");
        addChild(_operate);
        _listView.selectClick = CC_CALLBACK_2(DecorationLayer::touchListView,this);
        _listView.unSelectClick = CC_CALLBACK_2(DecorationLayer::unSelect,this);
        _eatContiner = _operate->getNodeByName("eatContiner");
        auto  thingContainer = _operate->getNodeByName("thingsLayer");
        _thingOperate = TouchNodeComponent::create();
        _thingOperate->addListners(ComponentTouchEnd, [this](Node* _node,Component*, OperateListner* _lis){
            if(_decPath.empty())
                return ;
            this->addTypes();
             SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
            hideFinger();
            auto _pos= _node->convertToNodeSpace(dynAny_cast<Vec2>(_lis->getData()));
            auto _s = Sprite::create(_decPath);
            _s->setRotation(rand()%360);
            float _endScale = 0.7+rand()%4*0.1;
            if(_s->getContentSize().width>130){
                float _p = _s->getContentSize().width/_s->getContentSize().height;
                if(_p>1.4){
                    _endScale = 1;
                    auto _rote = 90- Vec2(1,0).getAngle((_pos- _node->getContentSize()*.5))*(180/3.1415926);
                    log("===%f",_rote);
                    _s->setRotation(_rote);
                }
            }
            
            _node->addChild(_s);
            _s->setPosition(_pos);
            _s->setScale(0);
            _s->runAction(ScaleTo::create(0.2, _endScale));
        });
        thingContainer->addComponent(_thingOperate);
        
        auto _decBg = _operate->getNodeByName("icon_bg");
        _decoView = MyScollView::create(141, 530);
        _decoView->setSpace(20);
        _decoView->setMarginBottom(80);
        _decoView->setDirection(MyScollView::Direction::VERTICAL);
        _decoView->setPosition(_decBg->getContentSize()*.5);
        _decBg->addChild(_decoView);
        dynamic_cast<ui::ScrollView*>(_operate->getNodeByName("iconView"))->setScrollBarOpacity(0);
        
        return true;
    }
    return false;
}

void DecorationLayer::reset(){
    _suaceIs = false;
    _chooseTyps.clear();
    UserDefault::getInstance()->setIntegerForKey("playIndex",1);
    UserDefault::getInstance()->flush();
    hideFinger();
    this->stopPrompt();
    if(_icing != nullptr)
        _icing->resetCanvas();
    if(_sauce != nullptr){
        _sauce->removeFromParent();
        _sauce = nullptr;
    }
    auto _lisNode = _thingOperate->getLisNode();
    auto _vech = _thingOperate->getOwner()->getChildren();
    removeChildByName("drawSauce");
    for(auto _node:_vech)
        if(_node != _lisNode)
            _node->removeFromParent();
    
    SoundPlayer::getInstance()->playEffect("sound/Sound/reset.mp3");
    
}

void  DecorationLayer::touchListView(ui::CheckBox* _check,int index){
    SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
    hideFinger();
    this->stopPrompt();
    removeChildByName("drawSauce");
    for(auto _child:_listView.getContainer()->getChildren()){
        if(_child->getName() != "bg"){
            _child->stopAllActions();
            _child->setScale(1);
        }
    }
    _check->stopAllActions();
    float timeScale = 1.2;
    ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0*0.95, 0.80*0.95);
    ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82*0.95, 1.0*0.95);
    ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0*0.9, 0.86*0.9);
    ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88*0.9, 1.0*0.9);
    ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0*0.85, 0.89*0.85);
    ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 0.85, 0.85);
    auto ret = Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
    _check->runAction(ret);
    //_check->setScale(0.85);
    
    if(_icing != nullptr)
        _icing->disable();
    if(_sauce != nullptr)
        _sauce->setDrawing(false);
    _thingOperate->setEnabled(false);
    showCatagore();
    creatIcon(index-2);
}

void DecorationLayer::unSelect(ui::CheckBox*_check,int index){
     SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
    hideFinger();
    this->stopPrompt();
    removeChildByName("drawSauce");
    _check->stopAllActions();
    float timeScale = 1.2;
    ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
    ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
    ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
    ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1, 1);
    auto ret = Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
    _check->runAction(ret);
    //_check->setScale(1);
    if(_icing != nullptr)
        _icing->disable();
    if(_sauce != nullptr)
        _sauce->setDrawing(false);
    _thingOperate->setEnabled(false);
    hideCatagore();
}

void DecorationLayer::addTypes(){
    if(!_currentcategory.empty()){
        if(std::find(_chooseTyps.begin(), _chooseTyps.end(), _currentcategory) == _chooseTyps.end())
            _chooseTyps.push_back(_currentcategory);
        if(_chooseTyps.size()>=3){
            UserDefault::getInstance()->setIntegerForKey("playIndex",-1);
            UserDefault::getInstance()->flush();
        }
            
    }
}

void DecorationLayer::freeClick(KDS::EventSprite* _s,int _index,KDS::MyScollView*){
    _decPath = CategoryReader::getCategoryPath(_currentcategory, _index);
    auto sprite =  Sprite::create(_decPath);
    removeChildByName("drawSauce");
    if(_currentcategory == "icing"){
        if(_icing == nullptr){
            auto _dount = _operate->getNodeByName("dount");
            _icing = ScribbleWithCallBack::create(sprite->getContentSize());
            _icing->setPosition(_dount->getContentSize()*.5);
            _dount->addChild(_icing,1);
            _icing->setAntiAliasingBrush(true);
            _icing->useBrush("hole_stencilsamll.png",Scribble::eBrush);
            _icing->callBack = [this](cocos2d::Touch *,ScribbleWithCallBack::TouchType _type){
                if(_type == ScribbleWithCallBack::TouchType::eTouchMoved){
                    this->stopPrompt();
                    addTypes();
                }
            };
        }
        _icing->useTarget(_decPath);
        _icing->enable();
        
        ActionHelper::runMoveHideAction(_prompt, _operate->getPosinWord("dount"), _operate->getPosinWord("dount")+Vec2(200,0));
        
    } else if(_currentcategory == "sauce"){
        
        _decPath = _suaceDecos.at(_index);
      
        //if(_sauce != nullptr)
          //  _sauce->removeFromParent();
                auto _dount = _operate->getNodeByName("dount");
        if(_clipp == nullptr){
            _clipp = ClippingNode::create();
            auto _mask = Sprite::create("png/6/donuts.png");
            _mask->setPosition(_dount->getContentSize()*.5);
            _clipp->setStencil(_mask);
            _clipp->setAlphaThreshold(0.8f);
            _dount->addChild(_clipp,2);
        }
       // _sauce = nullptr;
        if(_sauce == nullptr){
            _sauce = ScribbleSauce::create(_dount->getContentSize());
            _clipp->addChild(_sauce);
        }
        _eventDispatcher->setEnabled(false);
        auto temp  = Sprite::createWithSpriteFrame(_s->getSpriteFrame());
        temp->setPosition(_s->convertToWorldSpace(_s->getContentSize()*.5));
        addChild(temp,30);
        temp->setName("drawSauce");
        temp->runAction(Sequence::create(EaseSineInOut::create(MoveTo::create(0.5, _operate->getPosinWord("dount"))), CallFunc::create([this,_index,temp](){
            ActionHelper::runMoveHideAction(_prompt, _operate->getPosinWord("dount"), _operate->getPosinWord("dount")+Vec2(200,0));
            auto _str = _suaceNames.at(_index);
            if(_str.find(".png") == string::npos){
                if(_suaceIs){
                    _sauce->reset();
                    _suaceIs = false;
                }
                _sauce->setBrush(Sprite::create("png/sauce/"+_str+"_1.png"));
                _sauce->setDrawing(true);
                temp->setAnchorPoint(Vec2(0.5, 1));
                temp->runAction(RotateBy::create(0.3, -45));
                _sauce->callBack = [this,_str](Touch* pTouch,ScribbleSauce::touchType _type){
                    if(_type == ScribbleSauce::touchType::touchMove){
                        this->stopPrompt();
                        this->addTypes();
                        auto _draw = getChildByName("drawSauce");
                        if(_draw != nullptr){
                            auto _dount = _operate->getNodeByName("dount");
                            Rect _limit;
                            _limit.size = _dount->getContentSize();
                            
                            
                            auto _pos = _dount->convertToNodeSpace(pTouch->getLocation());
                            if(_limit.containsPoint(_pos)) {
                            _draw->setPosition(pTouch->getLocation());
                            dynamic_cast<Sprite*>(_draw)->setSpriteFrame(Sprite::create("png/sauce/"+_str+"_0.png")->getSpriteFrame());
                            }
                        }
                    }
                };
            } else {
                _suaceIs = true;
                this->stopPrompt();
                temp->removeFromParent();
                _sauce->reset();
                _sauce->setBrush(Sprite::create(_decPath));
                _sauce->paint(_operate->getPosinWord("dount"));
                _sauce->setDrawing(false);
                auto _pari = ParticleSystemQuad::create("particle/decorateParticle.plist");
                _pari->setPosition(_operate->getPosinWord("dount"));
                this->addChild(_pari,50);
                SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
            }
            _eventDispatcher->setEnabled(true);
        }),nullptr));
        
    } else {
        showFinger();
        _thingOperate->setEnabled(true);
    }
    
}

void  DecorationLayer::touchEnd(ui::Widget* widge){
    BaseDecorationLayer::touchEnd(widge);
    if(widge->getName().compare("btn_next") == 0){
        std::remove(FileUtils::getInstance()->fullPathForFilename(FileUtils::getInstance()->getWritablePath()+"eatFood.png").c_str());
        Director::getInstance()->getTextureCache()->removeTextureForKey(FileUtils::getInstance()->fullPathForFilename(FileUtils::getInstance()->getWritablePath()+"eatFood.png"));
            Director::getInstance()->getTextureCache()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"eatFood.png");
        auto _pos = _eatContiner->getPosition();
        _eatContiner->setPosition(Vec2::ZERO);
        auto _size = _eatContiner->getContentSize();
        auto _render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        _render->begin();
        _eatContiner->visit();
        _render->end();
        Director::getInstance()->getRenderer()->render();
     
        _render->saveToFile("eatFood.png");
        Director::getInstance()->getRenderer()->render();
        _eatContiner->setPosition(_pos);
        _operate->playAction("out",[this](){
            auto  _endFunc = [this](){
                LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                auto _step = EatLayer::create();
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
        });
    };
    
    
    
}

void  DecorationLayer::showEnd(){
    
}

void DecorationLayer::onExitTransitionDidStart(){
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(_eventName);
    BaseDecorationLayer::onExitTransitionDidStart();
}

void DecorationLayer::onEnterTransitionDidFinish(){
    BaseDecorationLayer::onEnterTransitionDidFinish();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(_eventName);
     SoundPlayer::getInstance()->playBackGroundMusic("sound/general/background.mp3");
    _operate->playAction("show",CC_CALLBACK_0(DecorationLayer::showEnd, this));
    _listView.setContainer(_operate->getNodeByName("iconView"));
    SoundPlayer::getInstance()->playEffect("sound/note_swish.mp3");
}

void DecorationLayer::showFinger(){
    auto _finger = _operate->getNodeByName("finger");
    auto _cicle = _operate->getNodeByName("circle");
    _cicle->stopAllActions();
    _cicle->setScale(0);
    _cicle->setVisible(true);
    _finger->stopAllActions();
    _finger->setRotation(0);
    _finger->runAction(RepeatForever::create(Sequence::create(CallFunc::create([_cicle](){_cicle->runAction(ScaleTo::create(0, 0));}),RotateTo::create(0, 0),RotateTo::create(0.3, -13.5),DelayTime::create(0.1),CallFunc::create([_cicle](){_cicle->runAction(Repeat::create(Sequence::create(ScaleTo::create(0.4, 1),DelayTime::create(0.2),ScaleTo::create(0, 0), nullptr),1));}), DelayTime::create(2), nullptr)));
    _finger->setVisible(true);
}
void DecorationLayer::hideFinger(){
    _operate->getNodeByName("finger")->setVisible(false);
    _operate->getNodeByName("circle")->setVisible(false);
}

void DecorationLayer::creatIcon(int _index){
    if(_chooseBg != nullptr)
        _chooseBg->removeFromParent();
    _currentcategory = categoryNames.at(_index);
    if(_decoView != nullptr){
        vector<string>  iconPath;
        auto icons =CategoryReader::getIconPath(_currentcategory);
        if(_currentcategory=="sauce"){
            for(auto _s:_suaceNames)
                if(_s.find("png") != string::npos)
                    iconPath.push_back(_s);
            else
                iconPath.push_back("png/sauce/"+_s+".png");
        }
        else
            iconPath = icons;
        auto _adapter = MyBaseAdapter::create(iconPath);
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