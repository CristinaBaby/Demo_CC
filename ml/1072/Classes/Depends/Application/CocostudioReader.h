
#ifndef CocostudioReader_h
#define CocostudioReader_h

#include "SingleTon.h"
#include "cocos2d.h"
#include <stdio.h>
#include <string.h>
#include <iostream>


USING_NS_CC;
class CocostudioReader:public SingleTon<CocostudioReader>
{
public:
    
    void loadScene(std::string path);
    cocos2d::Vec2 getPosition(std::string name);
    static cocos2d::Vec2 pos(std::string name){
        return CocostudioReader::getInstance()->getPosition(name);
    }
    
    cocos2d::Node* pScene;
};
#endif /* CocostudioReader_h */
