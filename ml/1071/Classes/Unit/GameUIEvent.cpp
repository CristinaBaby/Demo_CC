
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
        RateUsManager::getInstance()->showRateUs();
    }else if (GameUILayoutLayer::eUIButtonTagPolicy==tag){
        
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
                    AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
                    AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
                        SceneManager::pushTheScene<ShareScene>();
                    };
                }else{
                    SceneManager::pushTheScene<ShareScene>();
                }
            }
                break;
            case GameUIEvent::eSceneTagGrillSausage:{
                SceneManager::pushTheScene<GrillSausageScene>();
            }
                break;
            case GameUIEvent::eSceneTagCutBread:{
                SceneManager::pushTheScene<CutBreadScene>();
            }
                break;
            case GameUIEvent::eSceneTagGrillBread:{
                SceneManager::pushTheScene<GrillBreadScene>();
            }
                break;
            case GameUIEvent::eSceneTagPutinSausage:{
                SceneManager::pushTheScene<PutinSausageScene>();
            }
                break;
            case GameUIEvent::eSceneTagMixFlour:{
                SceneManager::pushTheScene<MixFlourScene>();
            }
                break;
            case GameUIEvent::eSceneTagFryPancake:{
                SceneManager::pushTheScene<FryPancakeScene>();
            }
                break;
            case GameUIEvent::eSceneTagFrySausage:{
                SceneManager::pushTheScene<FrySausageScene>();
            }
                break;
            case GameUIEvent::eSceneTagRollSausage:{
                SceneManager::pushTheScene<RollSausageScene>();
            }
                break;
            case GameUIEvent::eSceneTagCookHotdog:{
                SceneManager::pushTheScene<CookHotdogScene>();
            }
                break;
            case GameUIEvent::eSceneTagDecHotdog:{
                SceneManager::pushTheScene<DecHotdogScene>();
            }
                break;
            default:
                break;
        }
        
    }
}

