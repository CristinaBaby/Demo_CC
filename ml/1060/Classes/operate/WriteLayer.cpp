//
//  WriteLayer.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/4.
//
//

#include "WriteLayer.h"
#include "CategoryReader.h"
#include "MakeSignatureReader.h"
#include "TouchScaleRoteComponent.h"
#include "MoveInVisibleRectComponent.h"

IMPLEMENT_COCOS2DX_CLASS(WriteLayer);
WriteLayer::WriteLayer(){
    _drawNode = nullptr;
    _bgSprite = nullptr;
    _paper = nullptr;
    writeEnd = nullptr;
    _pen = nullptr;
    isDecs = false;
    resetName = "btn_delete";
    bgPath = "png/pattern.png";
    
    categoryNames.push_back("color");
    categoryNames.push_back("pattern");
     categoryNames.push_back("pencil");
    categoryNames.push_back("stickers");
   
}

void WriteLayer::showOperate(Vec2 pos){
    SoundPlayer::getInstance()->playEffect("sound/general/pop_up_open.mp3");
    _eventDispatcher->setEnabled(false);
    auto container = _operate->getNodeByName("container");
    _endPos = container->getPosition();
    _startPos = container->getParent()->convertToNodeSpace(pos);
    container->setScale(0);
    container->setPosition(_startPos);
    container->runAction(Sequence::create(EaseBackOut::create(Spawn::create(ScaleTo::create(0.7, 1), MoveTo::create(0.7, _endPos), nullptr)),CallFunc::create([this](){
        string names[] = {"btn_sure","btn_delete","chooseBg","iconBg"};
        for(int i=0;i<sizeof(names)/sizeof(names[0]);i++){
            auto _node =  _operate->getNodeByName(names[i]);
            _node->setScale(0);
            _node->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.3, 1)),CallFunc::create([_node](){
            }) ,nullptr));
        }
        _operate->getNodeByName("wirteUI")->setVisible(true);
        _eventDispatcher->setEnabled(true);
    }), nullptr));
}

void WriteLayer::reset(){
    hideFinger();
    isDecs = false;
        bgPath = "png/pattern.png";
    _drawNode = nullptr;
    //_bgSprite = nullptr;
    ActionHelper::changeFade(_bgSprite, Sprite::create("png/pattern.png"));
    _paper->clicke = nullptr;
    _paper->removeAllChildren();
    bgPath = "";
    auto _chilsds = _rendreNode->getChildren();
    for(auto _child:_chilsds)
        if(_child != _paper&&_child != _bgSprite)
            _child->removeFromParent();
    
    if(_pen != nullptr)
        _pen->removeFromParent();
    _pen =nullptr;
    if(_chooseBg != nullptr)
        _chooseBg->removeFromParent();
      SoundPlayer::getInstance()->playEffect("sound/Sound/reset.mp3");
        hideUi();
}

bool WriteLayer::init(){
    BaseDecorationLayer::init();
    _operate = StudioLayer::create("writeLayer.csb");
    addChild(_operate);
    _bgSprite = _operate->getNodeByname<Sprite*>("bgSprite");
    _bgSprite->setCascadeOpacityEnabled(false);
    _chooseBg = Sprite::create("ui/pubilc/draw-xuanzhong.png");
    _chooseBg->retain();
    _rendreNode = _operate->getNodeByName("renderLayer");
    _rendreNode->setCascadeOpacityEnabled(false);
    _rendreNode->setCascadeColorEnabled(false);
    
    
    auto _node = _operate->getNodeByName("iconBg");
    _decoView = KDS::MyScollView::create(620, 90);
    _decoView->setSpace(20);
    _decoView->setPosition(_node->getContentSize()*.5);
    _decoView->setDirection(KDS::MyScollView::Direction::HORIZONTAL);
    _node->addChild(_decoView);
    
    _checkList.selectClick = CC_CALLBACK_2(WriteLayer::choose,this);
    _checkList.unSelectClick = CC_CALLBACK_2(WriteLayer::hide, this);
    
    _paper = EventLayer::create();
    _paper->setContentSize(_rendreNode->getContentSize());
    _paper->setOpacity(0);
    _paper->setSwallowTouche(false);
    _rendreNode->addChild(_paper,3);
    
    return true;
}

void WriteLayer::touchEnd(ui::Widget* widget){
    if(widget->getName().compare("btn_sure") == 0){
        hideUi();
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
        _operate->getNodeByName("wirteUI")->setVisible(false);
        auto container = _operate->getNodeByName("container");
        container->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(ScaleTo::create(0.8, 0.3), MoveTo::create(0.8, _startPos), nullptr)),CallFunc::create([this](){
            if(writeEnd != nullptr)
                writeEnd(_rendreNode,isDecs,bgPath);
            this->removeFromParent();
            Director::getInstance()->getEventDispatcher()->setEnabled(true);
        }), nullptr));
    }
    BaseDecorationLayer::touchEnd(widget);
}

void WriteLayer::choose(ui::CheckBox*,int index){
    hideUi();
    SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
    hideFinger();
    if(_drawNode != nullptr)
        _drawNode->setDrawing(false);
    if(_pen != nullptr){
        _pen->removeFromParent();
        _pen = nullptr;
    }
    if(_paper != nullptr)
        _paper->clicke = nullptr;
    creatIcon(index);
    showCatagore();
}

void WriteLayer::hide(ui::CheckBox*,int index){
       endbleUi();
     SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
    if(_drawNode != nullptr)
        _drawNode->setDrawing(false);
    if(_pen != nullptr){
        _pen->removeFromParent();
        _pen = nullptr;
    }
    if(_paper != nullptr)
        _paper->clicke = nullptr;
    _currentcategory = "";
    hideCatagore();
}

void WriteLayer::hideUi(){
    if(_paper != nullptr){
        for(auto _child:_paper->getChildren()){
            auto  _move = _child->getComponent("MoveInVisibleRectComponent");
            if(_move != nullptr)
                _move->setEnabled(false);
            auto _touchCom = dynamic_cast<TouchScaleRoteComponent*>(_child->getComponent("TouchScaleRoteComponent"));
            if(_touchCom != nullptr){
                _touchCom->hideUI();
                _touchCom->setEnabled(false);
            }
        }
    }
}

void WriteLayer::endbleUi(){
    if(_paper != nullptr){
        for(auto _child:_paper->getChildren()){
            auto  _move = _child->getComponent("MoveInVisibleRectComponent");
            if(_move != nullptr)
                _move->setEnabled(true);
            auto _touchCom = dynamic_cast<TouchScaleRoteComponent*>(_child->getComponent("TouchScaleRoteComponent"));
            if(_touchCom != nullptr){
                _touchCom->setEnabled(true);
            }
        }
    }
}

void WriteLayer::freeClick(KDS::EventSprite* s,int index,KDS::MyScollView*){
    if(_currentcategory == "pencil"){
        if(_pen == nullptr){
            _pen = Sprite::createWithSpriteFrame(s->getSpriteFrame());
            _pen->setAnchorPoint(Vec2::ZERO);
            _pen->setRotation(104);
            addChild(_pen,20);
            ActionHelper::show(_pen, getContentSize()*.5+Size(0,50), ActionHelper::ShowDirection::show_from_right);
        }else
            _pen->setSpriteFrame(s->getSpriteFrame());
        
    }else if(_currentcategory != "stickers")
        showFinger();
    changeDec(s,_currentcategory,index);
}

void WriteLayer::changeDec(KDS::EventSprite* s,string _name,const int index){
    if(_paper != nullptr)
        _paper->clicke = nullptr;
    if(_name == "pencil") {
        if(_drawNode == nullptr){
            _drawNode = ScribbleSauce::create(_rendreNode->getContentSize());
            _drawNode->setTouchBeginEnable(true);
            //_drawNode->setDrawRadius(5);
            _drawNode->setBrush(Sprite::create("hole_stencilsamllpen.png"));
            _rendreNode->addChild(_drawNode,2);
            _drawNode->callBack = [this](Touch* pTouch,ScribbleSauce::touchType _type){
                if(_type == ScribbleSauce::touchType::touchMove){
                    Rect _limit;
                    _limit.size = _rendreNode->getContentSize();
                    if(_limit.containsPoint(_rendreNode->convertToNodeSpace(pTouch->getLocation()))){
                        if(_loopSound == -1)
                            _loopSound = SoundPlayer::getInstance()->playEffectLoop("sound/other/pen_drawing.mp3");
                        isDecs = true;
                        if(_pen != nullptr){
                            _pen->stopAllActions();
                            _pen->setPosition(pTouch->getLocation());
                        }
                    }else {
                        if(_loopSound != -1){
                            SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
                            _loopSound = -1;
                        }
                    }
                }else if(_type == ScribbleSauce::touchType::touchCancle || _type == ScribbleSauce::touchType::touchEnd){
                    
                    Rect _limit;
                    _limit.size = _rendreNode->getContentSize();
                    if(_limit.containsPoint(_rendreNode->convertToNodeSpace(pTouch->getLocation()))){
                        isDecs = true;
                        if(_pen != nullptr){
                            _pen->stopAllActions();
                            _pen->setPosition(pTouch->getLocation());
                        }
                    }
                    
                    if(_loopSound != -1){
                        SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
                        _loopSound = -1;
                    }
                }
                };
            
        }
        //_drawNode->setBrushScale(0.1);
        _drawNode->setDrawing(true);
        _drawNode->setDrawColor(MakeSignatureReader::getInstance()->getPenColor(index));
    } else if(_name == "pattern"|| _name=="color"){
        string _path = CategoryReader::getCategoryPath(_name, index);
        _paper->clicke = [this,_path](EventLayer*, int,Touch* _touch){
             SoundPlayer::getInstance()->playEffect("sound/other/category_selection_1.mp3");
            isDecs = true;
            hideFinger();
            bgPath = _path;
            if(_bgSprite == nullptr){
                _bgSprite = Sprite::create(bgPath);
                _bgSprite->setName("bgSprite");
                _bgSprite->setPosition(_rendreNode->getContentSize()*.5);
                _rendreNode->addChild(_bgSprite);
            }else {
                auto _s = Sprite::create(bgPath);
                ActionHelper::changeFade(_bgSprite, _s);
            }
        };
    } else if(_name == "stickers") {
        _eventDispatcher->setEnabled(false);
        string _path = CategoryReader::getCategoryPath(_name, index);
        isDecs = true;
        auto  _item = Sprite::create(_path);
        _item->setPosition(s->convertToWorldSpace(s->getContentSize()*.5));
        this->addChild(_item,20);
        ccBezierConfig _bezierConfig;
        _bezierConfig.controlPoint_1 = _bezierConfig.controlPoint_2 = _item->getPosition() +Vec2(50,50);
        _bezierConfig.endPosition = _paper->convertToWorldSpace(_paper->getContentSize()*.5)- Vec2(200,20) +Vec2(rand()%400,rand()%40);
        _item->runAction(Sequence::create(BezierTo::create(0.5, _bezierConfig),CallFunc::create([_item,this](){
            endbleUi();
            CocosHelper::reAddNode(_item, _paper);
        
            auto _moveCom = MoveInVisibleRectComponent::create();
            auto _rect = CocosHelper::getNodeRectInWorld(_paper);
            _rect.origin = _rect.origin + Vec2(40,40);
            _rect.size = _rect.size - Size(80,80);
            _moveCom->setTarget(LQRect::create(_rect));
            _item->addComponent(_moveCom);
            auto _touchScale = TouchScaleRoteComponent::create();
            _touchScale->setMaxScale(1.0f);
            _item->addComponent(_touchScale);
                _item->setScale(0.7);
            SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
            auto _particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
            _particle->setPosition(_item->convertToWorldSpace(_item->getContentSize()*.5));
            this->addChild(_particle,20);
              _eventDispatcher->setEnabled(true);
        }), nullptr));
    }
}

void  WriteLayer::onEnterTransitionDidFinish(){
    BaseDecorationLayer::onEnterTransitionDidFinish();
    _checkList.setContainer(_operate->getNodeByName("chooseBg"));
}

void WriteLayer::showFinger(){
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
void WriteLayer::hideFinger(){
    _operate->getNodeByName("finger")->setVisible(false);
    _operate->getNodeByName("circle")->setVisible(false);
}