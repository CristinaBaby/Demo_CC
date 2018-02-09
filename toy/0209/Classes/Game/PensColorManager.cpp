//
//  PensColorManager.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/17.
//
//

#include "PensColorManager.h"

#define COLOR_COUNT             (10)

PensColorManager* PensColorManager::msg = nullptr;

PensColorManager* PensColorManager::getColorManager(){
    if (!msg) {
        msg = new (std::nothrow)PensColorManager();
        msg->initColor();
    }
    return msg;
}

bool PensColorManager::initColor(){
    
    Color color[COLOR_COUNT] = {
        Color{230,0,18},
        Color{234,85,32},
        Color{242,142,0},
        Color{254,209,0},
        Color{195,215,0},
        Color{21,173,103},
        Color{35,173,229},
        Color{19,64,152},
        Color{115,91,158},
        Color{111,24,110}};
    Color offset[COLOR_COUNT] = {
        Color{4 ,85, 14},
        Color{8, 57, -32},
        Color{12 ,67, 0},
        Color{-59 ,6, 0},
        Color{-174, -42, 0},
        Color{14, 0, 126},
        Color{-16, -107, -77},
        Color{96 ,27, 6},
        Color{-4 ,-67 ,-48},
        Color{119 ,-24, -92}};
    
    for (int i = 0; i<COLOR_COUNT; i++) {
        PensColor penscolor;
        Color4F offset4f = Color4F(offset[i].red / 255, offset[i].green / 255, offset[i].blue / 255, 1);
        Color4F color4f = Color4F(color[i].red / 255, color[i].green / 255, color[i].blue / 255, 1);
        penscolor.original = color4f;
        penscolor.offset = offset4f;
        _pensColor.push_back(penscolor);
    }
    
    _color = Color4F(color[0].red / 255, color[0].green / 255, color[0].blue / 255, 1);
    
    return true;
}

Color4F PensColorManager::getUseColor(){
    return _color;
}

Color4F PensColorManager::getColor()
{
    _acc++;
    
    if (_acc >= 5) {
        _index++;
        _acc = 0;
        if (_index == COLOR_COUNT) {
            _index = 0;
        }
        // 取出 纯色
        _color = _pensColor.at(_index).original;
        return _color;
    }
    _color.r = _color.r + _pensColor.at(_index).offset.r / 5.0;
    _color.g = _color.g + _pensColor.at(_index).offset.g / 5.0;
    _color.b = _color.b + _pensColor.at(_index).offset.b / 5.0;
    return _color;
    // 取出 渐变色
}
