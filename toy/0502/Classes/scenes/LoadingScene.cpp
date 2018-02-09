//
//  LoadingScene.cpp
//  SpotDifference
//
//  Created by Steven.Tian on 14-10-27.
//
//

#include "LoadingScene.h"
#include "../utilities/STMultiScreenHelper.h"
#include "../utilities/CSVParse.h"
#include "../widgets/STUILayer.h"
#include "HomeScene.h"
#include "../AppGlobal.h"
#include "../helpers/PurchaseHelper.h"
#include "../helpers/EncryptDataHelper.h"
#include "../helpers/AudioHelper.h"
#include "../utilities/UICpp.h"
#include "../utilities/STUserDefault.h"
#include "../gameconfig/GameConfig.h"

#include <algorithm>

USING_NS_CC;
using namespace std;

float endPercent;

Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LoadingScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool LoadingScene::init()
{
    if (! Layer::init()) {
        return false;
    }
    
    
    auto root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui00_loading.csb"));
    addChild(root);
    
    _loading = (LoadingBar*)Helper::seekWidgetByName(root, "ProgressBar_7");
    _loading->setPercent(5);
    
    endPercent = 60;
    
    loadStrings();
    scheduleUpdate();
    
    return true;
}


void LoadingScene::update(float delta)
{
    _acc += delta;
    if (_acc > 0.15) {
        _loading->setPercent(_loading->getPercent() + 10);

        if (_loading->getPercent() >=  endPercent) {
            _loading->setPercent(endPercent);
            
            if (_loading->getPercent() == 70) {
                loadMapConfig();
            }
            else if (_loading->getPercent() == 100)
            {
                loadLevelsInfo();
            }
        }
        
        _acc = 0;
    }
}

void LoadingScene:: loadStrings()
{
    //
    AudioHelper::getInstance()->playMusic("bg_start.mp3");
    
    //一来就跑到60%, 然后再换
    _loading->setPercent(endPercent);
    endPercent += 10;
}

void LoadingScene:: loadMapConfig()
{
    // stop the action run before
    ArmatureDataManager::getInstance()->addArmatureFileInfo("fish1.ExportJson");
    
    _loading->stopActionByTag(101);
    
    _loading->setPercent(endPercent);
    endPercent = 100;
}

void LoadingScene:: loadLevelsInfo()
{
    // stop the action run before
    loadComplete();
}

void LoadingScene:: loadComplete()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool policy = STLoadBooleanFromUD("mypolicy", false);
    if (!policy) {
        xScene->forward(S_POLICY);
    }
    else
#endif
    {
        xScene->forward(S_HOME);
    }
}








