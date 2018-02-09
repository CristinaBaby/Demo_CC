
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
                    AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
                }
                SceneManager::pushTheScene<ShareScene>();
            }
                break;
            case GameUIEvent::eSceneTagChooseBg:
            {
                SceneManager::replaceTheScene<ChooseBgScene>();
            }
                break;
            case GameUIEvent::eSceneTagCrushIce:
            {
                SceneManager::replaceTheScene<CrushIceScene>();
            }
                break;
            case GameUIEvent::eSceneTagShapePuzzle:
            {
                SceneManager::replaceTheScene<PuzzleGameScene>();
            }
                break;
            case GameUIEvent::eSceneTagColorIce:
            {
                SceneManager::replaceTheScene<ColorIceScene>();
            }
                break;
            case GameUIEvent::eSceneTagMap:
            {
                SceneManager::replaceTheScene<MapScene>();
            }
                break;
            default:
                break;
        }
    }
}

