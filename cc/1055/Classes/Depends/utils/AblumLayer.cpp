//
//  AblumLayer.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#include "AblumLayer.h"
#include "FilePathUtil.h"
#include "Dialog.h"
#include "AdapterScreen.h"

AblumLayer::AblumLayer() {
    _image = nullptr;
    _shareFilePath = "";
}

AblumLayer::~AblumLayer(){
    CC_SAFE_RELEASE_NULL(_image);
}

bool AblumLayer::init() {
    if(Layer::init()) {
        _operate = StudioLayer::create("albumLayer.csb");
        addChild(_operate);
        
        _operate->_touchBtnCallback = CC_CALLBACK_1(AblumLayer::touchEnd, this);
        
        Node* node0 = _operate->getNodeByName("btn_ablum");
        Vec2 pos0 = node0->getPosition();
        pos0 = Vec2(pos0.x, pos0.y+20);
        node0->setPosition(pos0);
        
        Node* node1 =_operate->getNodeByName("btn_share");
        Vec2 pos1 = node1->getPosition();
        pos1 = Vec2(pos1.x, pos1.y+20);
        node1->setPosition(pos1);
        
        return true;
    }
    return false;
}

void AblumLayer::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
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
        
#if __cplusplus > 201100L
        RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int           requestcode,bool bgranted){
            onPermissionGrantedResult(requestcode, bgranted);
        };
#else
        RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
#endif
        
        //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
        int requestCode = 1;
        //调用权限申请的方法,根据需要申请敏感权限
        RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kWriteExternalStorage);
        }else if(widget->getName() == "btn_share"){
        if(_shareFilePath.empty()) {
            _shareFilePath = FilePathUtil::newSharePath();
            _image->saveToFile(_shareFilePath);
        }
        string content;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        string appId = "";
        auto map = FileUtils::getInstance()->getValueMapFromFile("AppConfig.plist");
        auto itor = map.find("AppleId");
        if(itor != map.end()){
            auto value = itor->second;
            if(value.getType() == Value::Type::STRING) {
                appId = value.asString();
            }
        }
        
        
        
        content = StringUtils::format("I just made so many yummy cookies with this SUPER FUN app!All kinds of flavors,shapes and topping! Let's Play Together.I can't wait to  see your creations!Download it for FREE now!<p><a href=‘http://itunes.apple.com/app/%s’>http://itunes.apple.com/app/%s</a></p>",appId.c_str(),appId.c_str());
#else
        content = "I just made a delicious Food at this SUPER FUNNN app!!! Download it for FREE now! See if you can make a better one!";
#endif
        
//        CFSystemFunction sys;
        SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Cookie Maker", content.c_str(), _shareFilePath.c_str());
        
    }else if(widget->getName() == "btn_close"){
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
        _operate->playAction("out",[this](){
            this->removeFromParent();
            Director::getInstance()->getEventDispatcher()->setEnabled(true);
        });
    }
}


void AblumLayer::saveImage(){
    
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    //        CFSystemFunction sys;
    SSCInternalLibManager::getInstance()->saveToAlbum(_image, [this](bool _flag){
        if(_flag) {
            Dialog* dialog= Dialog::create((void*)kDialogNotice, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("Image was saved to your Camera Roll!");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            this->getScene()->addChild(dialog, 9999);
            
        } else {
            Dialog* dialog= Dialog::create((void*)kDialogNotice, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("Failed, Please enable photo access in Privacy Setting!");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            this->getScene()->addChild(dialog, 9999);
            
        }
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
    },"Cookies");

}


void AblumLayer::onPermissionGrantedResult(int requestCode,bool bGranted){
    if (requestCode == 1) {
        if (bGranted) {
            this->runAction(Sequence::create(DelayTime::create(0.0f),
                                             CallFunc::create([=] {
                saveImage();
            }),NULL))    ;
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
}