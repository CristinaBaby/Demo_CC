//
//  BaseScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "BaseScene.h"

USING_NS_CC;
using namespace ui;

BaseScene::BaseScene():
_background(nullptr),
_rootLayout(nullptr)
{
    
}

BaseScene::~BaseScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool BaseScene::init()
{
    if (!Layer::init()) {
        return false;
    }

    return true;
}
