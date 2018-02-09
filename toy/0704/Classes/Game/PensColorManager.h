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
#define xPcm    PensColorManager::getColorManager()

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
    PensColorManager();
    
    static PensColorManager* getColorManager();
    
    bool initColor();
    
    Color4F getColor();
    
    Color4F getUseColor();
    
    Color4F getSugarColor();
    
    Color4F getColorForFillMode();
    
    Color4F getColorForInitPens();
    
    void clear();
    
    ValueVector  _colorVector;
    
    bool  getRainbowPenTag(){
        return _rainbowPen;
    }
    
    void setRainbowPenTag(bool state){
        _rainbowPen = state;
    }
    
    CC_SYNTHESIZE(bool, drawType, DrawType);
private:
    
    std::vector<PensColor> _pensColor;
    
    std::vector<Color4F> _pensColorForFill;
    
    std::vector<int> _unLockPens;
    
    static PensColorManager* msg;
    
    Color4F _color;
    
    int     _index = 0;
    
    int     _tag = 0;
    
    int     _acc = 0;
    
    int     _step = 0;
    
    bool    _rainbowPen = false;
};

#endif /* defined(__ColorBook0818__PensColorManager__) */
