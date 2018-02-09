//
//  LoadingPage.cpp
//
//

#include "LoadingPage.h"
#include "CMVisibleRect.h"
#include "SceneHeader.h"
#include "SSCIAPManager.h"
LoadingPage::LoadingPage()
{
    //cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("ccats/ccats.ExportJson");

}
LoadingPage::~LoadingPage()
{
   // cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("ccats/ccats.ExportJson");

}


Scene* LoadingPage::scene()
{
    Scene *pScene = Scene::create();
    pScene->addChild(LoadingPage::create());
    return pScene;
    
}

bool LoadingPage::init()
{

  CCLOG("loading page ******************");

    if (!Layer::init())
    {
        return false;
    }
//    auto color = LayerColor::create(Color4B(255, 125, 1, 255), 9999, 9999);
//    this->addChild(color);
    
    auto bg = Sprite::create("loading/bg.png");
    this->addChild(bg,-1);
    bg->setPosition(CMVisibleRect::getCenterOfScene());
    
    this->runAction(Sequence::create(
                                     DelayTime::create(0.2f),
                                     CallFunc::create([]
                                                      {
                                                          kAudioUtil->playEffect("loading/loading_1.mp3");
                                                      }),
                                     DelayTime::create(0.6f),
                                     CallFunc::create([]
                                                      {
                                                          kAudioUtil->playEffect("loading/loading_2.mp3");
                                                      }),
                                     DelayTime::create(0.5f),
                                     CallFunc::create([]
                                                      {
                                                          kAudioUtil->playEffect("loading/loading_3.mp3");
                                                      }),
                                     DelayTime::create(0.44f),
                                     CallFunc::create([]
                                                      {
                                                          kAudioUtil->playEffect("loading/loading_decoration out.mp3",true);
                                                      }),
                                     DelayTime::create(3.f),
                                     CallFunc::create([]
                                                      {
                                                          kAudioUtil->stopAllEffect();
                                                      }),

                                     NULL));
    
    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("loading/loading_shake.ExportJson");
    cocostudio::Armature *armature = cocostudio::Armature::create("loading_shake");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    armature->getAnimation()->play("shake_it");
#else
    armature->getAnimation()->play("kids_food_game");
    log("===1======");
#endif
//    armature->setScale(0.5);
    CMVisibleRect::setPosition(armature, 320,480);
    this->addChild(armature);
    
//    kAudioUtil->playEffect("ccats/loading.mp3");
    
    armature->getAnimation()->setMovementEventCallFunc([=]
                                                       (cocostudio::Armature *,
                                                        cocostudio::MovementEventType type,
                                                        const std::string&)
                                                       {
                                                           if (type == cocostudio::MovementEventType::COMPLETE)
                                                           {
                                                               kAudioUtil->stopAllEffect();
                                                               this->runAction(Sequence::create(DelayTime::create(0.2),
                                                                                                CallFunc::create(CC_CALLBACK_0(LoadingPage::goToNextPage, this)),
                                                                                                nullptr));
                                                           }
                                                       });

    return true;
}

void LoadingPage::goToNextPage()
{
   

    SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}
