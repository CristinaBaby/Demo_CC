//
//  Sugar.hpp
//  TOYBM0205
//
//  Created by huangwen on 15/10/23.
//
//

#ifndef Sugar_hpp
#define Sugar_hpp

#include "cocos2d.h"

USING_NS_CC;

class Sugar : public Sprite{
public:
    static Sugar* create(const std::string filename);
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    
    void destroy(float dt);
private:
    
};

#endif /* Sugar_hpp */
