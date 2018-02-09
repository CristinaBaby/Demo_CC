#pragma once

#include "cocos2d.h"

using namespace cocos2d;
using std::string;

class Balloon : public Sprite
{
public:
    static Balloon* create(const std::string& filename);
private:
    bool onTouchBegan(Touch* touch, Event* event);
    void disappear(float delta);
    void setFileName(const string& fileName);
    
    
    std::string _filename;
};

