
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
            case GameUIEvent::eSceneTagMixFlour:
            {
                SceneManager::replaceTheScene<MixFlourScene>();
            }
                break;
            case GameUIEvent::eSceneTagBake:
            {
                SceneManager::replaceTheScene<BakeScene>();
            }
                break;
            case GameUIEvent::eSceneTagFrosting:
            {
                SceneManager::replaceTheScene<FrostingScene>();
            }
                break;
            case GameUIEvent::eSceneTagAddIcing:
            {
                SceneManager::replaceTheScene<AddIcingScene>();
            }
                break;
            case GameUIEvent::eSceneTagAddCandle:
            {
                SceneManager::pushTheScene<AddCandleScene>();
            }
                break;
            case GameUIEvent::eSceneTagChooseBg:
            {
                SceneManager::pushTheScene<ChooseBgScene>();
            }
                break;
            case GameUIEvent::eSceneTagDecGift:
            {
                SceneManager::pushTheScene<DecorateGiftScene>();
            }
                break;
            case GameUIEvent::eSceneTagGetGift:
            {
                SceneManager::pushTheScene<GetGiftScene>();
            }
                break;
            default:
                break;
        }
    }
}

