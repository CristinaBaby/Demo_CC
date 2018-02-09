//
//  TypeSprite.h
//  Make
//
//  Created by QAMAC01 on 15-2-11.
//
//

#ifndef __Make__TypeSprite__
#define __Make__TypeSprite__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;


class TypeSprite : public Sprite
{

public:
    static TypeSprite* create(const char* pzFileName);
    void setHighlighted(bool isHighLighted);
    void setSpriteSelected(bool isHighLighted);
    std::string pzFileName_;

private:
    std::string getHighLightedFileName();
    CC_SYNTHESIZE(bool, selected_, Selected);
};

#endif /* defined(__Make__TypeSprite__) */
