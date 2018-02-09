//
//  CloudSprite.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/11.
//
//

#include "CloudSprite.h"
#include "UITools.h"

CloudSprite* CloudSprite::create(const std::string& filename, CloudType type){
    
    
    CloudSprite *cloud = new (std::nothrow) CloudSprite();
    if (cloud && cloud->initWithFile(filename) && cloud->initAnimation())
    {
        cloud->autorelease();
        cloud->setTag((int)type);
        return cloud;
    }
    CC_SAFE_DELETE(cloud);
    return nullptr;
}

bool CloudSprite::initAnimation(){
    
    scheduleOnce(schedule_selector(CloudSprite::changeDirection), 0.2f);
    
    return true;
}

void CloudSprite::changeDirection(float dt){
//    auto variable_x = quickRandom(-2, 2);
//    auto variable_y = quickRandom(-2, 2);
//    auto prePostion = getPosition();
//    auto position = prePostion + Vec2(variable_x,variable_y);
//    
//    if (limitBox((CloudType)getTag(), position)) {
//        setPosition(position);
//    }
//    else
//    {
//        log("limit boox return false, cloud position over the limitBox");
//        setPosition(prePostion);
//    }
    
    CloudType type = (CloudType)getTag();
    switch (type) {
        case CloudType::Animation_1:
            runAction(RepeatForever::create(Sequence::create(MoveBy::create(6.0, Vec2(275, - 15)), DelayTime::create(4.0) ,
                                                             MoveBy::create(7.0, Vec2(-275,15)), DelayTime::create(3.0), NULL)));
            break;
        case CloudType::Animation_2:
            runAction(RepeatForever::create(Sequence::create(MoveBy::create(7.0, Vec2(180, - 20)), DelayTime::create(3.0) ,
                                                             MoveBy::create(8.0, Vec2(-180,20)), DelayTime::create(3.0), NULL)));
            break;
        default:
            break;
    }
}

bool CloudSprite::limitBox(CloudType type, Vec2 pos){
    auto size = Director::getInstance()->getVisibleSize();
    if (CloudType::Animation_1 == type) {
        if (pos.x < -10 || pos.x >200 || pos.y > (size.height - 20) || pos.y < (size.height - 35)) {
            return false;
        }
        return true;
    }
    else if(CloudType::Animation_2 == type)
    {
        if (pos.x < -10 || pos.x >235 || pos.y > (size.height - 60) || pos.y < (size.height - 100)) {
            return false;
        }
        return  true;
    }
    log("limit box error");
    return false;
}

