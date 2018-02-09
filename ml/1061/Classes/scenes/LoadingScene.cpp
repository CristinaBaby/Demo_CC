//
//  LoadingScene.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-18.
//
//

#include "LoadingScene.h"
#include "../Depends/utils/AudioHelp.h"
#include "../Depends/managers/SceneManager.h"
#include "../Depends/utils/AdapterScreen.h"
#include "../Depends/utils/Global.h"
#include "CFSystemFunction.h"
#include "ActionUtils.h"
#include "cocostudio/CocoStudio.h"
#include "HomeScene.h"
using namespace cocostudio;
LoadingScene::LoadingScene()
{
    time = 0;
}
LoadingScene::~LoadingScene()
{
    ArmatureDataManager::getInstance()->removeArmatureFileInfo("images/loading/NewAnimation.ExportJson");
}
bool LoadingScene::init()
{
    if ( !BaseScene::init() ) {
        return false;
    }
    this->loading();
    
    return true;
}

void LoadingScene::loading()
{
    auto bg = Sprite::create("loading/loading.png");
    
    kAdapterScreen->setExactPosition(bg, 480, 320);
    addChild(bg);
    
    cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("loading/loading.ExportJson");
    cocostudio::Armature *armature = cocostudio::Armature::create("loading");
    
    kAdapterScreen->setExactPosition(armature, 480, 320 + 50);
    armature->getAnimation()->playWithIndex(0);
    this->addChild(armature);
    
    auto call = CallFunc::create([=]
                                 {
                                     AudioHelp::getInstance()->playEffect("loading.mp3");
                                 });
    
    this -> runAction(Sequence::create(DelayTime::create(0.5),call, NULL));
    
    armature->getAnimation()->setMovementEventCallFunc([](cocostudio::Armature *, cocostudio::MovementEventType type, const std::string&)
                                                       {
                                                           if (type == cocostudio::MovementEventType::LOOP_COMPLETE)
                                                           {
                                                               
                                                               SceneManager::getInstance()->enterHomeScene();
                                                               CFSystemFunction cfs;
                                                               cfs.showNewsBlast(NewsModeLaunch);
                                                               
                                                           }
                                                       });

}

void LoadingScene::preLoad()
{
    
    AudioHelp::getInstance()->setAudioVolumeMode(true);
    
}
void LoadingScene::callBack()
{
    SceneManager::getInstance()->enterHomeScene();
    CFSystemFunction sys;
    sys.showNewsBlast(NewsModeLaunch);
}
