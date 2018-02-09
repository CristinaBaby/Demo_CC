//
//  CBUtils.cpp
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#include "CBUtils.h"

USING_NS_CC;

static Utils *singletonInstance = nullptr;
float Utils::SCREEN_WIDTH  = 0.0;
float Utils::SCREEN_HEIGHT = 0.0;

Utils* Utils::getInstance()
{
    if (!singletonInstance)
    {
        singletonInstance = new (std::nothrow)Utils();
        CCASSERT(singletonInstance, "ABORT: Fail to create Utils!");
        
        bool code = singletonInstance->init();
        CCASSERT(code, "ABORT: Fail to init Utils!");
    }
    
    return singletonInstance;
}

bool Utils::init()
{
    bool rcode = false;
    
    do {
        this->setupScreenCoordinate();
        
        rcode = true;
    } while (0);
    
    return rcode;
}


Utils::~Utils(void)
{
    singletonInstance = nullptr;
}

#pragma mark- Screen coordinate calculate

void Utils::setupScreenCoordinate()
{
    Utils::SCREEN_WIDTH  = Director::getInstance()->getWinSize().width;
    Utils::SCREEN_HEIGHT = Director::getInstance()->getWinSize().height;
    
    this->_screenCenter = Vec2(Utils::SCREEN_WIDTH/2, Utils::SCREEN_HEIGHT/2);
}








