#include "BalloonManager.h"
#include "Balloon.h"
#include "../utilities/UICpp.h"
#include "../utilities/STUtility.h"
#include "../utilities/STVisibleRect.h"

static BalloonManager *_sharedBalloon = nullptr;

BalloonManager* BalloonManager::getInstance()
{
    if (! _sharedBalloon)
    {
        _sharedBalloon = new BalloonManager();
    }
    
    return _sharedBalloon;
}

void BalloonManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedBalloon);
}

BalloonManager::BalloonManager()
{
    _visibleSize = V::getRealDesignSize();
}

void BalloonManager::start()
{
    _count = 0;
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

void BalloonManager::stop()
{
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

void BalloonManager::update(float dt)
{
    _acc += dt;
    
    float limit = 0.5 + namespaceST::STUtility::getRandom(1.5);
    
    if (_acc > limit) {
        _acc = 0;
        
        string filename = "balloon" + std::to_string(namespaceST::STUtility::getRandom(4) + 1) + ".png";
        auto balloon = Balloon::create(filename);
        Size balloonSize = balloon->getContentSize();
        
        auto pos = Vec2(namespaceST::STUtility::getRandom(_visibleSize.width * 0.7) + _visibleSize.width * 0.15, -balloonSize.height/2);
        balloon->setPosition(pos);
        
        Scene *pRunScene = Director::getInstance()->getRunningScene();
        if (pRunScene) {
            pRunScene->addChild(balloon, 200);
        }
        
        //向顶部移动
        float speed = 2.5 + namespaceST::STUtility::getRandom(1);
        auto sequense = Sequence::create(MoveBy::create(speed, Vec2(0, _visibleSize.height + balloonSize.height)),
                                         CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, balloon)), NULL);
        balloon->runAction(sequense);
        
        _count++;
        
        if (_count > 20) {
            stop();
        }
    }
}

