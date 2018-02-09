//
//  MyMenu.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/16.
//
//

#include "MyMenu.h"
#include "WriteLayer.h"
#include "EventLayer.h"
#include "MyMenu.h"
#include "EventPromotLayer.h"
#include "FilePathUtil.h"
class Menu_adapter : public ViewAdapter{
public:
    CREATE_ViewAdapter(Menu_adapter);
    Node * getItem(size_t position){
        auto _node = ViewAdapter::getItem(position);
        
        
        auto  str = string(file_path.at(position));
         string _newString(str.c_str());
        string _fileName;
        auto token = std::strtok((char*)_newString.c_str(),"/");
        while(token != nullptr){
            _fileName = token;
            token = strtok(nullptr, "/");
        }
        log("_fileName====%s",_fileName.c_str());
        string name = "DonutShop";
        auto _labelPath = FileUtility::getStoragePath()+ "/"+name +"/"+ "lablename/" + _fileName;
        log("path==%s",_labelPath.c_str());
        auto _s = Sprite::create("ui/menu/name.png");
        _s->setPosition(Vec2(_node->getContentSize().width*.5,_node->getContentSize().height));
        _node->addChild(_s);
        KDS::EventSprite* _lable;
        if(FileUtils::getInstance()->isFileExist(_labelPath)){
              log("hasFile==============");
            _lable = KDS::EventSprite::createWithNoCache (_labelPath);
        }
        else
            _lable =  KDS::EventSprite::create ("ui/menu/name1.png");
        _lable->setName(_fileName);
        _lable->setSwallowTouches(true);
        _lable->setPosition(Vec2(_s->getContentSize()*.5)+Vec2(-3,4));
        _lable->touchSpriteEnded =touchLabelCallBack;
        _s->addChild(_lable);
        _s->setName("_lable");
        return _node;
    }
    std::function<void(KDS::EventSprite* ,size_t,Touch*)> touchLabelCallBack;
    
    void recycle(extension::TableView* table, extension::TableViewCell* cell){
        auto _child = dynamic_cast<Sprite*>(cell->getChildByTag(MyTableView::MyTableViewItemTag));
        if(_child != nullptr){
            auto _textCache = Director::getInstance()->getTextureCache();
            _textCache->removeTexture(_child->getTexture());
//            auto node = _child->getChildByName("_lable");
//            if(node != nullptr){
//                auto childrends = node->getChildren();
//                if(childrends.size()>0){
//                    auto spirte = dynamic_cast<Sprite*>(childrends.at(0));
//                    if(spirte != nullptr&& spirte->getTexture() != _textCache->getTextureForKey("ui/menu/name1.png"))
//                        _textCache->removeTexture(spirte->getTexture());
//                }
//            }
        }
    }
};

IMPLEMENT_COCOS2DX_CLASS(MyMenu);

MyMenu::~MyMenu(){
    for(auto _path:_filsePath)
        Director::getInstance()->getTextureCache()->removeTextureForKey(_path);
}

bool MyMenu::init(){
    if(BaseStepLayer::init()) {
        auto _EventPromotLayer = EventPromotLayer::create();
        addChild(_EventPromotLayer,50);
        _EventPromotLayer->setCallbakc([this](){
            Node*  _cell = nullptr;
            auto _used = _listView->getCellsUsed();
             if(_used.size()>0)
                 _cell = _used.at(rand()%_used.size());
            if(_cell != nullptr){
                auto _child = _cell->getChildByTag(MyTableView::MyTableViewItemTag);
                if(_child != nullptr){
                    auto _lable = _child->getChildByName("_lable");
                    if(_lable != nullptr){
                        _lable->stopAllActions();
                        _lable->runAction(Repeat::create(Sequence::create(ScaleTo::create(0.6, 1.05),ScaleTo::create(0.6, 1), nullptr),2));
                    }
                }
            }
            
        });
        _operate = StudioLayer::create("MyMenu.csb");
        addChild(_operate);
        
        _bg = _operate->getNodeByName("bg");
        
        _listView =  MyTableView::create(Size(_bg->getContentSize().width-30, 380));
        _listView->setPosition(_bg->getContentSize().width*.5,_bg->getContentSize().height*.5);
        auto _lock = EventLayer::create();
        _lock->setOpacity(0);
        _lock->setContentSize(Size(_bg->getContentSize().width,284));
        _lock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _lock->ignoreAnchorPointForPosition(false);
        _lock->setPosition(_listView->getPosition());
        _bg->addChild(_lock);
        _bg->addChild(_listView);
        _bg->setVisible(false);
        
        FilePathUtil::getFilesPathByDir("DonutShopMenu", _filsePath);
        
        auto _adapter = Menu_adapter::create(_filsePath);
        _adapter->touchLabelCallBack = CC_CALLBACK_3(MyMenu::touchLabel, this);
        _adapter->setItemSize(Size(284+20,302));
        _listView->setSwallowsTouches(true);
        _listView->setAdapter(_adapter);
        
    }
    return true;
}


void MyMenu::touchLabel(KDS::EventSprite* psprite,int,Touch* t){
    Rect _rect;
    _rect.origin = _listView->convertToWorldSpace(Vec2::ZERO);
    _rect.size = _listView->getViewSize();
    if(_rect.containsPoint(t->getLocation())) {
        auto writeLayer = WriteLayer::create();
        addChild(writeLayer,300);
        writeLayer->getOperateLayer()->changeNode("Image_1", "ui/draw/bg-name.png");
        writeLayer->showOperate(t->getLocation());
        writeLayer->writeEnd = [this,psprite](Node* _rendreNode,bool isChange,string){
            if(isChange) {
                auto _size = _rendreNode->getContentSize();
                auto _crentSize = psprite->getContentSize();
                auto _scale = max(_crentSize.width/_size.width,_crentSize.height/_size.height);
                _rendreNode->setScale(_scale);
                _rendreNode->ignoreAnchorPointForPosition(false);
                _rendreNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                _rendreNode->setPosition(_crentSize*.5);
                _rendreNode->retain();
                _rendreNode->removeFromParent();
                auto _clppin = ClippingNode::create();
                _clppin->setAlphaThreshold(0.0);
                auto _sten = Sprite::create("ui/menu/name1.png");
                _sten->setPosition(psprite->getContentSize()*.5);
                _clppin->setStencil(_sten);
                _clppin->addChild(_rendreNode);
                _rendreNode->release();
                auto _render = RenderTexture::create(_crentSize.width, _crentSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
                _render->begin();
                _clppin->cocos2d::Node::visit();
                _render->end();
                Director::getInstance()->getRenderer()->render();
                //_render->getSprite()->getTexture()->setAliasTexParameters();
                
            
                
                auto _path = FileUtility::getStoragePath()+ "/"+"DonutShop" +"/"+ "lablename/" + psprite->getName();
                auto image = _render->newImage();
                image->saveToFile(_path,false);
                Director::getInstance()->getRenderer()->render();
                _cfsys.refreshDCIM(_path);
                image->release();
                psprite->setSpriteFrame(CocosHelper::spriteCreateWithNoCache(_path.c_str())->getSpriteFrame());
            }
            auto _pari = ParticleSystemQuad::create("particle/decorateParticle.plist");
            _pari->setPosition(psprite->getContentSize()*.5);
            psprite->addChild(_pari,50);
            SoundPlayer::getInstance()->playEffect("sound/other/draw_success.mp3");
        };
        
    }
}

void MyMenu::touchEnd(ui::Widget* widget){
    if(widget->getName() == "bg") {
        
    }else if(widget->getName().compare("btn_close")==0)
        close();
}

void MyMenu::show(Vec2 _pos){
    SoundPlayer::getInstance()->playEffect("sound/general/pop_up_open.mp3");
    _eventDispatcher->setEnabled(false);
    _starPos = _bg->getParent()->convertToNodeSpace(_pos);
    _bg->setPosition(_starPos);
    _bg->setScale(0);
    _bg->setVisible(true);
    _bg->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5, getContentSize()*.5)), CallFunc::create([this](){
        Node*  _cell = nullptr;
        auto _used = _listView->getCellsUsed();
        if(_used.size()>0)
            _cell = _used.at(rand()%_used.size());
        if(_cell != nullptr){
            auto _child = _cell->getChildByTag(MyTableView::MyTableViewItemTag);
            if(_child != nullptr){
                auto _lable = _child->getChildByName("_lable");
                if(_lable != nullptr){
                    _lable->stopAllActions();
                    _lable->runAction(Repeat::create(Sequence::create(ScaleTo::create(0.6, 1.05),ScaleTo::create(0.6, 1), nullptr),2));
                }
            }
        }
        _eventDispatcher->setEnabled(true);
    }), nullptr));
    _bg->runAction(EaseBackOut::create(ScaleTo::create(0.5, 1)));
}
void MyMenu::close(){
    _eventDispatcher->setEnabled(false);
    _bg->runAction(Sequence::create(EaseExponentialIn::create(MoveTo::create(0.8, _starPos)), CallFunc::create([this](){
        _eventDispatcher->setEnabled(true);
        this->removeFromParent();
    }), nullptr));
    _bg->runAction(EaseExponentialIn::create(ScaleTo::create(0.8, 0)));
}

bool MyMenu::onBack(){
    close();
    return true;
}