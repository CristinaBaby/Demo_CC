//
//  SceneManager.cpp
//  SingleEye
//
//  Created by gwsoft on 1/21/14.
//
//
#include "SceneManager.h"
#include "../scenes/HomeScene.h"
#include "../scenes/LoadingScene.h"
#include "../scenes/RecordScene.h"
#include "../scenes/TransitionFadeEx.h"
#include "../scenes/TransitionScaleFade.h"
#include "../scenes/PrefaceScene.h"
#include "../layer/PolicyLayer.h"
#include "../Jigsaw/JPDifficultyScene.h"
#include "../Jigsaw/PuzzleScene.h"
#include "../Spot/SpotScene.h"
#include "../ColorBook/ColorBookScene.h"


SceneManager* SceneManager::getInstance()
{
    static SceneManager instance;
    return &instance;
}


SceneManager::SceneManager()
{
    m_pOldScene = NULL;
}


SceneManager::~SceneManager()
{
    m_vData.clear();
}

void SceneManager::replaceScene(SceneData &data)
{
    bool bDirectReturn = false;
    
    CCScene *pScene = NULL;
    
    //这里将记录当前的Tag
    m_eCurScene = data.aTag;
    
    switch (data.aTag) {
        case S_LOADING:
        {
            pScene = LoadingScene::createScene();
        }
            break;
        case S_HOME:
        {
            pScene = HomeScene::createScene();
        }
            break;
        case S_PUZZLE_DIFF:
        {
            pScene = JPDifficultyScene::createScene();
        }
            break;
        case S_PUZZLE:
        {
            pScene = PuzzleScene::scene();
        }
            break;
        case S_SPOT:
        {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game.plist", "game.pvr.ccz");
            //创建modeinfo
            auto levelInfo = LevelInfo::create();
            levelInfo->m_GameMode = AppGlobal::GameMode::ZEN;
            pScene = SpotScene::createScene(levelInfo);
        }
            break;
        case S_COLOR_BOOK:
        {
            pScene = ColorBookScene::createScene();
        }
            break;
        case S_RECORD:
        {
            if (data.isForward) {
                pScene = RecordScene::createScene();
            }
            else
            {
                pScene = RecordScene::scene();
            }
        }
            break;
        case S_POLICY:
        {
            pScene = (Scene*)PolicyLayer::create();
        }
            break;
        case S_PREFACE:
        {
            pScene = (Scene*)PrefaceScene::create();
        }
            break;
        default:
            break;
    }
            
    
    if (bDirectReturn == false) {
        if (m_vData.size() != 0) {
            Director::getInstance()->replaceScene(pScene);
        } else {
            //第1个场景
            Director::getInstance()->runWithScene(pScene);
        }
    }    
}


void SceneManager::back()
{    
    if (isOldScene()) {
        return;
    }
    
    log("back");
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        runningScene->unscheduleAllSelectors();
    }
    
    //删除当前场景记录            
    m_vData.pop_back();
    CCAssert(m_vData.size() != 0, "error");

    SceneData data = m_vData.at(m_vData.size() - 1);
    data.isForward = false;
    replaceScene(data);
}


void SceneManager::back(SceneTag aTag)
{
    if (isOldScene()) {
        return;
    }
    
    CCAssert(m_vData.size() != 0, "error");
    
    int index = m_vData.size();

    while (index > 0) {
        SceneData data = m_vData.at(index - 1);
        data.isForward = false;
        
        if (data.aTag == aTag) {
            replaceScene(data);
            break;
        }
        else
        {
            //删除该记录
            index--;
            m_vData.pop_back();
            CCAssert(index != 0, "no this scene.");
        }
    }
}


void SceneManager::forward(SceneTag aTag)
{   
    forward(aTag, 0, NULL);
}


void SceneManager::forward(SceneTag aTag, int iData, void* pData)
{
    if (isOldScene()) {
        return;
    }
    
    //记录进入的Scene
    SceneData data;
    
    data.aTag = aTag;
    data.iData = iData;
    data.pData = pData;
    data.isForward = true;
        
    replaceScene(data);

    m_vData.push_back(data);
}


SceneTag SceneManager::getLastSceneTag()
{
    if (m_vData.size() > 1) {
        SceneData data = m_vData.at(m_vData.size() - 2);        
        return data.aTag;
    }
    else
    {
        return ST_None;
    }
}


SceneTag SceneManager::getRunningSceneTag()
{      
    return m_eCurScene;
//    if (m_vData.size() != 0) {
//        SceneData data = m_vData.at(m_vData.size() - 1);        
//        return data.aTag;
//    }
//    else
//    {
//        return ST_None;
//    }
}

bool SceneManager::isOldScene()
{
    bool bRet = false;
    
    CCScene *pRunningScene = CCDirector::sharedDirector()->getRunningScene();
    
    if (pRunningScene != NULL) {
        if (m_pOldScene == pRunningScene) {
            bRet = true;
        }
        else
        {
            m_pOldScene = pRunningScene;
        }
    }
        
    return bRet;
}

