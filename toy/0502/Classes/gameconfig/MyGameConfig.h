#pragma once

//游戏中全局的一些配置存储区
#include "cocos2d.h"

using std::string;

#define xGame              (MyGameConfig::getInstance())

class MyGameConfig {
    
public:
    static MyGameConfig* getInstance();
    void destroyInstance();
    
    CC_SYNTHESIZE(string, _filename, FileName);                 //不带后缀的, 如"L01"
    CC_SYNTHESIZE(int, _sideLen, SideLen);
    
    string getFullFileName();
    
    void setPictureIndex(int pictureIndex);
    int getPictureIndex();
    
private:
    MyGameConfig();
    int _pictureIndex;
};