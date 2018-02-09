//
//  ChooseScene.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/1.
//
//

#include "ChooseScene.h"
#include "AsyncManager.h"
#include "CBPackManager.h"
#include "CBAppManager.h"
#include "UICpp.h"
#include "AsyncManager.h"

#define CHOOSE_SCENE_NUM "123456"
#define PARTICLE_BUBULE_LAYER  55
#define EFFECT_LAYER           54

ChooseScene* ChooseScene::create(bool forward,int packIndex, int count)
{
    ChooseScene *pRet = new ChooseScene();
    if (pRet && pRet->init(forward, packIndex, count))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ChooseScene::init(bool forward,int packIndex, int count){
    if(!Scene::init())
    {
        return false;
    }
    
    if(forward)
    {
        //预加载
        xAsync->add("choose-2.json");
       //(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("choose-2.json"));
        _packLayer = PackLayer::create();
        addChild(_packLayer);
    }
    else
    {
        xAds->showAds(kTypeBannerAds);
        _pictureLayer = PictureLayer::create(packIndex, count);
        addChild(_pictureLayer);
    }
    
    _layer = LayerColor::create();                  //effect layer
    _layer->setColor(cocos2d::Color3B::WHITE);
    _layer->setVisible(false);
    addChild(_layer,EFFECT_LAYER);
    
    auto loadListener = EventListenerCustom::create("event_start_turning", CC_CALLBACK_1(ChooseScene::onEventStartTurning, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(loadListener, this);
    
    auto effectListener = EventListenerCustom::create("event_end_turning", CC_CALLBACK_1(ChooseScene::onEventEndTurning, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(effectListener, this);
    
    auto packListener = EventListenerCustom::create("event_from_picture_to_pack", CC_CALLBACK_1(ChooseScene::onEventFromPictureToPack, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(packListener, this);
    
    xAsync->add("Book1.ExportJson");
    return true;
}

ChooseScene::~ChooseScene()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

void ChooseScene::onEnterTransitionDidFinish()
{
    if(!_beBack)
    {
        if (!xApp->isFirstLaunchApp())
        {
            xApp->requestCrossAd();
        }
        else
        {
            xApp->setIsFirstLaunchApp(false);
            xAds->showAds(kTypeBannerAds);
        }
    }
    else
    {
        xApp->requestFullScreenAd();
    }
}

void ChooseScene::onEventStartTurning(EventCustom* event){
    
    string name = (char*)(event->getUserData());
    
    vector<string> paths;
    int _packIndex = name.find_first_of(CHOOSE_SCENE_NUM);
    if(_packIndex != string::npos)
    {
        auto index = std::stod(&name[_packIndex]);
        xPack->getLoadPath(index, 1, paths);
        xAsync->addImages(paths);
    }
    else
    {
        CCASSERT(false, "error");
    }
    
    Vec2 pos = Director::getInstance()->getWinSize()/2;
    _particle = ParticleSystemQuad::create("particle/book.plist");
    _particle->setPosVar(Vec2(Director::getInstance()->getWinSize().width / 2, 50));
    _particle->setPosition(pos);
    addChild(_particle, PARTICLE_BUBULE_LAYER);
    
    _particle->runAction(Sequence::create(DelayTime::create(2.f), CallFunc::create([=](){
        _particle->removeFromParent();
        _particle = nullptr;
    }),NULL));
    
}

void ChooseScene::onEventEndTurning(EventCustom* event){
    
    int * data = (int *)event->getUserData();
    _data.clear();
    _data.push_back(data[0]);
    _data.push_back(data[1]);

    _layer->runAction(Sequence::create(DelayTime::create(0.4f),
                                       CallFunc::create([=]{
                                            _layer->setVisible(true);
                                       }),
                                       FadeIn::create(0.5f),
                                       CallFunc::create([=]{
                                            log("%d, %d", _data[0], _data[1]);
                                            _pictureLayer = PictureLayer::create(_data[0], _data[1]);
                                            addChild(_pictureLayer);
                                            _packLayer->removeFromParent();
                                       }),
                                       FadeOut::create(0.5f),
                                       DelayTime::create(0.1f),
                                       NULL));
}

void ChooseScene::onEventFromPictureToPack(EventCustom* event)
{
    scheduleOnce(schedule_selector(ChooseScene::onScheduleFromPictureToPack), 0);
}

void ChooseScene::onScheduleFromPictureToPack(float dt)
{
    if (_particle) {
        _particle->removeFromParent();
    }
    _pictureLayer->removeFromParent();
    _packLayer = PackLayer::create();
    addChild(_packLayer);
}