//
//  Utils.h
//  BHUG1064
//
//  Created by maxiang on 5/25/16.
//
//

#ifndef Utils_h
#define Utils_h

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class Utils
{
public:
    template <typename T>
    static Scene* createScene(bool physics = false)
    {
        Scene *scene = nullptr;
        if (physics)
            scene = Scene::createWithPhysics();
        else
            scene = Scene::create();
        auto layer = T::create();
        scene->addChild(layer);
        return scene;
    };
    
    
public:
    Utils();
    virtual ~Utils();
    
protected:
    

};

#endif /* Utils_h */
