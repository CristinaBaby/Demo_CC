//
//  AblumLayer.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#include "AblumLayer.h"
#include "FilePathUtil.h"
#include "MyDialog.h"
IMPLEMENT_COCOS2DX_CLASS(AblumLayer);
AblumLayer::AblumLayer() {
    _image = nullptr;
}

AblumLayer::~AblumLayer(){
    CC_SAFE_RELEASE_NULL(_image);
}

bool AblumLayer::init() {
    if(BaseStepLayer::init()) {
        _operate = StudioLayer::create("albumLayer.csb");
        addChild(_operate);
        return true;
    }
    return false;
}

void AblumLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    ActionHelper::delayFunc(0.01, this, [this](){
        _operate->playAction("in",[this](){
            Director::getInstance()->getEventDispatcher()->setEnabled(true);
        });
    
    });
}

void AblumLayer::setRender(RenderTexture* _render){
    CC_SAFE_RELEASE_NULL(_image);
    _image = _render->newImage();
    _shareFilePath = FilePathUtil::newSharePath();
    _image->saveToFile(_shareFilePath);
    auto _contanier = _operate->getNodeByName("contanier");
    auto _s = Sprite::createWithSpriteFrame(_render->getSprite()->getSpriteFrame());
    auto spSize = _s->getContentSize();
    auto _size = _contanier->getContentSize();
    float scaleX = _size.width/spSize.width;
    float scaleY = _size.height/spSize.height;
    auto _scale = max(scaleX, scaleY);
    _s->setScale(_scale);
    _s->setFlippedY(true);
    _s->setPosition(_contanier->getContentSize()*.5);
    _contanier->addChild(_s);
  
}

void AblumLayer::touchEnd(ui::Widget* widget){
    if(widget->getName() == "btn_ablum"){
         Director::getInstance()->getEventDispatcher()->setEnabled(false);
            _cfsys.saveToAlbum(_image, [this](bool _flag){
                if(_flag) {
                    auto dialog = MyDialog::create("Image was saved to your Camera Roll!", MyDialog::oneBtnPath);
                    this->getScene()->addChild(dialog);
                } else {
                    auto dialog = MyDialog::create("Failed, Please enable photo access in Privacy Setting!", MyDialog::oneBtnPath);
                     this->getScene()->addChild(dialog);
                }
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
            },"DeepFryAnything");
    }else if(widget->getName() == "btn_share"){
        _cfsys.sendEmailAndFilePic("See my picture for you!", "My friend, I just made a delicious Food at this SUPER FUNNN app!!! Download it for FREE now! See if you can make a better one!<p><a href=‘http://itunes.apple.com/app/id1071491226’>http://itunes.apple.com/app/id1071491226</a></p>", _shareFilePath.c_str());
    }else if(widget->getName() == "btn_close"){
         Director::getInstance()->getEventDispatcher()->setEnabled(false);
        _operate->playAction("out",[this](){
            this->removeFromParent();
              Director::getInstance()->getEventDispatcher()->setEnabled(true);
        });
    }
}