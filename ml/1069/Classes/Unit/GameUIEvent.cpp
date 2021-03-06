
#include "GameUIEvent.h"
#include "GameUILayoutLayer.h"
#include "SceneManager.h"
#include "AudioHelp.h"

GameUIEvent::GameUIEvent()
{
    
}

GameUIEvent::~GameUIEvent()
{
    
}


void GameUIEvent::onButtonCallback(int tag )
{
    if (GameUILayoutLayer::eUIButtonTagBack==tag){
        AudioHelp::getInstance()->playPreviousEffect();
        SceneManager::popScene();
    }else if (GameUILayoutLayer::eUIButtonTagHomeRate==tag){
        STSystemFunction sf;
        sf.rateUs();
    }else if (GameUILayoutLayer::eUIButtonTagMoreGame==tag){
        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        SceneManager::replaceTheScene<HomeScene>();
    }else if (GameUILayoutLayer::eUIButtonTagFav==tag){
        
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag) {
        AudioHelp::getInstance()->stopLoopEffect();
        
        AudioHelp::getInstance()->stopFryEffect();
        AudioHelp::getInstance()->playNextEffect();
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
        switch (nextSceneTag) {
            case GameUIEvent::eSceneTagDecorate:
                SceneManager::replaceTheScene<DecorateScene>();
                break;
            case GameUIEvent::eSceneTagShare:
            {
                if (!UserDefault::getInstance() -> getBoolForKey("removeAds")){
                    AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
                    AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
                        SceneManager::pushTheScene<ShareScene>();
                    };
                }else{
                    SceneManager::pushTheScene<ShareScene>();
                }
            }
                break;
            case GameUIEvent::eSceneTagChoosePackage:
                SceneManager::replaceTheScene<ChoosePackageScene>();
                break;
            default:
                break;
        }
    }
}

