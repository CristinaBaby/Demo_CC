
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
        RateUsManager::getInstance()->showRateUs(false);
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
            AudioHelp::getInstance()->stopLoopEffect();
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
                SceneManager::pushTheScene<ShareScene>();
            }
                break;
            case GameUIEvent::eSceneTagSnowconeCrushIce:{
                SceneManager::pushTheScene<SnowconeCrushIceScene>();
            }
                break;
            case GameUIEvent::eSceneTagCottonChooseShape:{
                SceneManager::pushTheScene<CottonChooseShapeScene>();
            }
                break;
            case GameUIEvent::eSceneTagFunnelMix:{
                SceneManager::pushTheScene<FunnelMixSecne>();
            }
                break;
            case GameUIEvent::eSceneTagCottonMake:{
                SceneManager::pushTheScene<CottonMakeScene>();
            }
                break;
            case GameUIEvent::eSceneTagCottonChooseFlavor:{
                SceneManager::pushTheScene<CottonChooseFlavorScene>();
            }
                break;
//            case GameUIEvent::eSceneTagPizzaDisplay:{
//                SceneManager::pushTheScene<PizzaDisplayIngredientScene>();
//            }
//                break;
//            case GameUIEvent::eSceneTagFriesDisplay:{
//                SceneManager::pushTheScene<FriesDisplayIngredientScene>();
//            }
//                break;
//            case GameUIEvent::eSceneTagNachoDisplay:{
//                SceneManager::pushTheScene<NachoDisplayIngredientScene>();
//            }
//                break;
//            case GameUIEvent::eSceneTagBurgerAddIngredient:{
//                SceneManager::pushTheScene<BurgerAddIngredientScene>();
//            }
//                break;
//            case GameUIEvent::eSceneTagHotdogSauteOnion:{
//                SceneManager::pushTheScene<HotdogSauteOnionScene>();
//            }
//                break;
//            case GameUIEvent::eSceneTagPizzaMix:{
//                SceneManager::pushTheScene<PizzaMixScene>();
//            }
//                break;
//            case GameUIEvent::eSceneTagFriesCutPotato:{
//                SceneManager::pushTheScene<FriesCutPotatoScene>();
//            }
//                break;
//            case GameUIEvent::eSceneTagNachoAddIngredient:{
//                SceneManager::pushTheScene<NachoAddIngredientScene>();
//            }
            default:
                break;
        }
    }
}

