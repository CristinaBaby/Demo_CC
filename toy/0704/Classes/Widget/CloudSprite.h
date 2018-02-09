//
//  CloudSprite.h
//  ColorBook0818
//
//  Created by huangwen on 15/9/11.
//
//

#ifndef __ColorBook0818__CloudSprite__
#define __ColorBook0818__CloudSprite__

#include "cocos2d.h"

USING_NS_CC;

class CloudSprite : public Sprite{
public:
    enum class CloudType{
        Animation_1 = 99,
        Animation_2
    };
    
    static CloudSprite* create(const std::string& filename, CloudType type);
    
    bool initAnimation();

    //virtual void update(float dt) override;

    void changeDirection(float dt);

    bool limitBox(CloudType type, Vec2 pos);
private:
//    static CloudSprite* m_cloud;
};

#endif /* defined(__ColorBook0818__CloudSprite__) */
