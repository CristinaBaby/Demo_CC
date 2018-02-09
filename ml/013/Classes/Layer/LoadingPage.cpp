//
//  LoadingPage.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-26.
//
//

#include "LoadingPage.h"
#include "KSVisibleRect.h"
#include "GameMaster.h"
#include "AudioController.h"
#include "STSystemFunction.h"
Scene* LoadingPage::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(LoadingPage::create());
    return pScene;
    
}

bool LoadingPage::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto bg = Sprite::create("loading/loading.jpg");
    bg->setPosition(KSVisibleRect::getPosition(320, 480));
    addChild(bg);
    
    cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("loading/loading.ExportJson");
    cocostudio::Armature *armature = cocostudio::Armature::create("loading");
    KSVisibleRect::setPosition(armature, 320, 280);
    armature->getAnimation()->playWithIndex(0);
    this->addChild(armature);
    
    
    auto call = CallFunc::create([=]
                                 {
                                     CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("loading/loading.mp3");
                                 });
    
    this -> runAction(Sequence::create(DelayTime::create(0.5),call, NULL));

    
    armature->getAnimation()->setMovementEventCallFunc([=](cocostudio::Armature *, cocostudio::MovementEventType type, const std::string&)
                                                       {
                                                           if (type == cocostudio::MovementEventType::LOOP_COMPLETE)
                                                           {
                                                               this -> goToNextPage();
                                                           }
                                                       });

    return true;
}

void LoadingPage::goToNextPage()
{
    AudioController::getInstance() -> playBackGroundMusic();
    STSystemFunction st;
    st.showNewsBlast(NewsModeLaunch);
    GameMaster::getInstance() -> goToHomeLayer();
    Director::getInstance()->getTextureCache() -> removeUnusedTextures();
}
