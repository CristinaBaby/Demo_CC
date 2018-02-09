//
//  LoadingPage.cpp
//
//

#include "LoadingPage.h"
#include "CMVisibleRect.h"
#include "SceneHeader.h"
#include "SSCIAPManager.h"
#include "IAPAapter.h"

LoadingPage::LoadingPage()
{
    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("ccats/ccats.ExportJson");

}
LoadingPage::~LoadingPage()
{
    cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("ccats/ccats.ExportJson");

}


Scene* LoadingPage::scene()
{
    Scene *pScene = Scene::create();
    pScene->addChild(LoadingPage::create());
    return pScene;
    
}

bool LoadingPage::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto bg = Sprite::create("loading/768x1136.jpg");
    this->addChild(bg);
    bg->setPosition(CMVisibleRect::getCenterOfScene());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    IAPAapter a;
//    int index = a.getAndroidPlatformCode();
//    if (index == 32)
    {
        bg->setTexture("loading/kloadingp.jpg");
        cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("loading/NewAnimation.ExportJson");
        cocostudio::Armature *armature = cocostudio::Armature::create("NewAnimation");
        CMVisibleRect::setPosition(armature, 320, 312);
        armature->setScale(0.7f);
        armature->getAnimation()->playWithIndex(0);
        this->addChild(armature);
        
        armature->getAnimation()->setMovementEventCallFunc([=]
                                                           (cocostudio::Armature *,
                                                            cocostudio::MovementEventType type,
                                                            const std::string&)
                                                           {
                                                               if (type == cocostudio::MovementEventType::LOOP_COMPLETE)
                                                               {
                                                                   
                                                                   this->runAction(Sequence::create(DelayTime::create(0.2),
                                                                                                    CallFunc::create(CC_CALLBACK_0(LoadingPage::goToNextPage, this)),
                                                                                                    nullptr));
                                                               }
                                                           });
    }
#else
    this->runAction(Sequence::create(DelayTime::create(1.5f),
                                     CallFunc::create(CC_CALLBACK_0(LoadingPage::goToNextPage, this)),
                                     nullptr));

#endif

    

    return true;
}

void LoadingPage::goToNextPage()
{
   

    SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}
