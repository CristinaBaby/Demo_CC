#include "Balloon.h"
#include "../helpers/AudioHelper.h"

USING_NS_CC;


Balloon* Balloon::create(const std::string& filename)
{
    Balloon *sprite = new (std::nothrow) Balloon();
    if (sprite && sprite->initWithFile(filename))
    {
        //加入点击事件
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(Balloon::onTouchBegan, sprite);
        sprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);
        sprite->setFileName(filename);
        
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool Balloon::onTouchBegan(Touch* touch, Event* event)
{
    auto target = (Sprite*)(event->getCurrentTarget());
    
    // 获取当前点击点所在相对按钮的位置坐标
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Rect rect = target->getBoundingBox();
    rect.origin = Vec2::ZERO;
    
    // 点击范围判断检测
    if (rect.containsPoint(locationInNode))
    {
        //停止, 0.5秒后移除
        int pos = _filename.find(".");
        string file = _filename.substr(0, pos) + "_p.png";
        setTexture(file);
        stopAllActions();
        scheduleOnce(schedule_selector(Balloon::disappear), 0.2);
        xAudio->playSound("sfx_balloon_pop.mp3");
        return true;
    }
    
    return false;
}

void Balloon::disappear(float delta)
{
    removeFromParent();
}


void Balloon::setFileName(const string& fileName)
{
    _filename = fileName;
}
