
#include "MixManager.h"
#include "cocos2d.h"
#include "GameDataAdapter.h"
#include "Global.h"
#include "extensions/cocos-ext.h"
#include "SSCInternalLibManager.h"

USING_NS_CC;
using namespace cocos2d::ui;


void MixManager::addPoint(Vec2 pos)
{
    _pointVector.push_back(pos);
}