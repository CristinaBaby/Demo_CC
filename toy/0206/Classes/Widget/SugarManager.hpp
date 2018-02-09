//
//  SugarManager.hpp
//  TOYBM0205
//
//  Created by huangwen on 15/10/23.
//
//

#ifndef SugarManager_hpp
#define SugarManager_hpp

#include "cocos2d.h"

USING_NS_CC;
class SugarManager{
public:
    static SugarManager* getInstance();
    
    void start();
    
    void stop();
    
    void destroy();
    
    void update(float dt);
private:
    static SugarManager* _instance;
    
    float _acc = 0;
    int _count = 0;
    Size _visibleSize;
};

#endif /* SugarManager_hpp */
