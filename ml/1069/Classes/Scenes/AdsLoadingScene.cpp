//


#include "AdsLoadingScene.h"
#include "SceneManager.h"
#include "AnimationHelp.h"


static std::string gFontLetter[7] = {"l","o","a","d","i","n","g"};

bool AdsLoadingScene::init()
{
    if ( !AdLoadingLayerBase::init() ) {
        return false;
    }
    loading();
    
    return true;
}

void AdsLoadingScene::onEnter()
{
    AdLoadingLayerBase::onEnter();
    
    AudioHelp::getInstance()->pauseBackGroundMusic();
    AdsManager::getInstance()->setVisiable(kTypeBannerAds,false);
}
void AdsLoadingScene::onExit()
{
    AudioHelp::getInstance()->resumeBackGroundMusic();
    AdsManager::getInstance()->setVisiable(kTypeBannerAds,true);
    AdLoadingLayerBase::onExit();
}
void AdsLoadingScene::loading()
{
    Size size = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create("loading/loading.jpg");
    bg->setPosition(Point(size.width/2, size.height/2));
    this->addChild(bg);
    
//    Sprite* content = Sprite::create("loading/loading1.png");
//    content->setPosition(Point(size.width/2+50, size.height/2));
//    this->addChild(content);
    {
        
        ArmatureDataManager::getInstance()->addArmatureFileInfo("loading/loading0.png", "loading/loading0.plist", "loading/loading.ExportJson");
        ArmatureDataManager::getInstance()->addArmatureFileInfo("loading/loading1.png", "loading/loading1.plist", "loading/loading.ExportJson");
        Armature* lArmature = Armature::create("loading");
        lArmature->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2-200));
        this->addChild(lArmature);
        ArmatureAnimation* lArmation = lArmature->getAnimation();
        lArmation->playWithIndex(0,-1,0);
    }
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("loading/loadingletter0.png", "loading/loadingletter0.plist", "loading/loadingletter.ExportJson");
    Armature* lArmature = Armature::create("loadingletter");
    lArmature->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2-200));
    this->addChild(lArmature);
    ArmatureAnimation* lArmation = lArmature->getAnimation();
    lArmation->playWithIndex(0,-1,1);

}
