//
//  BaseScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__BaseScene__
#define __BedtimeStory__BaseScene__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocosGUI.h"
#include "cocos-ext.h"
#include "../Model/AppConfigs.h"
#include "../Model/STVisibleRect.h"
#include "../Model/MiniGameManager.h"
#include "../Model/Utils.h"
#include "MiniGameManager.h"

class BaseScene : public cocos2d::Layer
{
public:
    template <typename T>
    static T* create()
    {
        T *pRet = new T();
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    };
    
    template <typename T>
    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = BaseScene::create<T>();
        scene->addChild(layer);
        return scene;
    };
    
    BaseScene();
    virtual ~BaseScene();

protected:
    virtual bool init() override;

protected:
    cocos2d::ui::ImageView      *_background;
    cocos2d::ui::Widget         *_rootLayout;

};


#endif /* defined(__BedtimeStory__BaseScene__) */
