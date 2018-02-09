//
//  LoadingLayer.cpp
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/17/15.
//
//

#include "LoadingLayer.h"
#include "CMVisibleRect.h"
#include "HomeLayer.hpp"
#include "LQ_adLoadingLayer.h"
#include "MyDialog.h"
#include "FileUtility.h"

LoadingLayer::~LoadingLayer(){
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("loading/loading.plist");
}

bool LoadingLayer::init() {
    if(BaseStepLayer::init()) {
        string name = "DonutShop";
        
        FileUtility::createDirectory((FileUtility::getStoragePath()+ "/"+name).c_str());
        FileUtility::createDirectory((FileUtility::getStoragePath()+ "/"+name +"/"+ "lablename").c_str());
        auto bg = Sprite::create("loading/loading.png");
        bg-> setPosition(getContentSize()*.5);
        addChild(bg);
        
        cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("loading/loading/loading.ExportJson");
        cocostudio::Armature *armature = cocostudio::Armature::create("loading");
        armature->setPosition(CMVisibleRect::getPosition(480, 200));
        armature->getAnimation()->playWithIndex(0);
        this->addChild(armature);
        
        this -> runAction(Sequence::create(DelayTime::create(0.5)
                                           ,CallFunc::create([=] {
            SoundPlayer::getInstance() ->playEffect("loading/loading.mp3");
        }),DelayTime::create(4), CallFunc::create(CC_CALLBACK_0(LoadingLayer::enCallBack, this)), nullptr));
        
        return true;
    }
    return false;
}

void LoadingLayer::enCallBack(){
    auto _sc = Scene::create();
    _sc->addChild(HomeLayer::create());
    Director::getInstance()->replaceScene(_sc);
    STSystemFunction _cfs;
    _cfs.showNewsBlast(NewsModeLaunch);
    SoundPlayer::getInstance()->playBackGroundMusic("sound/general/background.mp3");
    BaseStepLayer::setKeyBack([](){
        auto _dialog = MyDialog::create("Do you want to start over with a new game?", MyDialog::twoBtnPath);
        _dialog->dialogBtnClick =[](MyDialog * dialog,int index){
            if(index == 0) {
                auto _endcallBack = [](){
                    Director::getInstance()->popToRootScene();
                    LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                    auto _scne = Scene::create();
                    _scne->addChild(HomeLayer::create());
                    Director::getInstance()->replaceScene(TransitionFade::create(0.8,_scne,Color3B::WHITE));
                };
                if(IAPManager::getInstance()->isShowAds()){
                    LQ_adLoadingLayer::loadingDoneCallback = _endcallBack;
                    LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
                } else
                    _endcallBack();
                
            }
        };
        
        Director::getInstance()->getRunningScene()->addChild(_dialog);
        return true;
    });
}

void LoadingLayer::onEnterTransitionDidFinish() {
    BaseStepLayer::onEnterTransitionDidFinish();
}