
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
//        STSystemFunction sf;
        //        sf.rateUs();
        RateUsManager::getInstance()->showRateUs();
    }else if (GameUILayoutLayer::eUIButtonTagMoreGame==tag){
        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to start over with a new food?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            AudioHelp::getInstance()->resetEffect();
//            AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
//            AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
//                SceneManager::popToRootScene();
//                SceneManager::replaceTheScene<HomeScene>();
//            };
            
            AdsManager::getInstance()->showAds(kTypeInterstitialAds);
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
//                    AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
//                    AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
//                        SceneManager::pushTheScene<ShareScene>();
//                    };
                    
                    AdsManager::getInstance()->showAds(kTypeInterstitialAds);
                    SceneManager::pushTheScene<ShareScene>();
                }else{
                    SceneManager::pushTheScene<ShareScene>();
                }
            }
                break;
            case GameUIEvent::eSceneTagMixFlour:
                SceneManager::replaceTheScene<MixFlourScene>();
                break;
            case GameUIEvent::eSceneTagCutDonut:
                SceneManager::replaceTheScene<CutDonutScene>();
                break;
            case GameUIEvent::eSceneTagFryDonut:
                SceneManager::replaceTheScene<FryDonutScene>();
                break;
            case GameUIEvent::eSceneTagRiseDough:
                SceneManager::replaceTheScene<TwistRiseDoughScene>();
                break;
            case GameUIEvent::eSceneTagRubTwist:
                SceneManager::replaceTheScene<TwistRubDoughScene>();
                break;
            case GameUIEvent::eSceneTagFryTwist:
                SceneManager::replaceTheScene<FryTwistScene>();
                break;
            default:
                break;
        }
        
        
        
    }
}

