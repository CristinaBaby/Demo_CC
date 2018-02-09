//
//  PensColorManager.h
//  ColorBook0818
//
//  Created by huangwen on 15/9/17.
//
//

#ifndef __ColorBook0818__PensColorManager__
#define __ColorBook0818__PensColorManager__

#include "cocos2d.h"


USING_NS_CC;
struct Color{
    float red;
    float green;
    float blue;
};

struct PensColor
{
    Color4F original;
    Color4F offset;
};

class PensColorManager{
public:
    
    static PensColorManager* getColorManager();
    
    bool initColor();
    
    Color4F getColor();
    
    Color4F getUseColor();
    
private:
    
    std::vector<PensColor> _pensColor;
    
    static PensColorManager* msg;
    
    Color4F _color;
    
    int _index = 0;
    
    int _acc = 0;
};

#endif /* defined(__ColorBook0818__PensColorManager__) */
