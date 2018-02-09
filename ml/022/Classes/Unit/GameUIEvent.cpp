
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
    
        SSCInternalLibManager::getInstance()->rateUs();
    }else if (GameUILayoutLayer::eUIButtonTagMoreGame==tag){
        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to start over with a new food?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            AudioHelp::getInstance()->resetEffect();
            SceneManager::popToRootScene();
            SceneManager::replaceTheScene<HomeScene>();
            
        };
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
            case GameUIEvent::eSceneTagChooseIceShape:
                SceneManager::replaceTheScene<ChooseIceShapeScene>();
                break;
            case GameUIEvent::eSceneTagFreeze:
                SceneManager::replaceTheScene<FreezeScene>();
                break;
            case GameUIEvent::eSceneTagPourIce:
                SceneManager::replaceTheScene<PourIceScene>();
                break;
            case GameUIEvent::eSceneTagAddJuice:
                SceneManager::replaceTheScene<AddJuiceScene>();
                break;
            case GameUIEvent::eSceneTagCrushIce:
                SceneManager::replaceTheScene<MakeCreamScene>();
                break;
            case GameUIEvent::eSceneTagChooseCup:
                SceneManager::replaceTheScene<ChooseCupScene>();
                break;
            case GameUIEvent::eSceneTagPourJuice:
                SceneManager::replaceTheScene<PourJuiceScene>();
                break;
            default:
                break;
        }
        
        
    }
}

