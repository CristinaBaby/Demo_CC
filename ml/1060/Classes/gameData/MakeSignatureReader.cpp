//
//  MakeSignatureReader.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/7.
//
//

#include "MakeSignatureReader.h"
#include "CocosHelper.h"

MakeSignatureReader::MakeSignatureReader(){
    _cache = FileUtils::getInstance()->getValueMapFromFile("plist/MakeSignature.plist");
}

Color3B MakeSignatureReader::getPenColor(int index){
    Color3B _color;
    auto _colors = _cache.find("penColor"); 
    if(_colors != _cache.end() && _colors->second.getType() == Value::Type::VECTOR){
        auto _arra = _colors->second.asValueVector();
        if(index>=0&&index<_arra.size()){
            auto _ints = CocosHelper::stringToInt(_arra.at(index).asString());
            _color = Color3B(_ints.at(0),_ints.at(1),_ints.at(2));
        }
    }
    return _color;
}

Color3B MakeSignatureReader::getBgColor(int index){
    Color3B _color;
    auto _colors = _cache.find("bgColor");
    if(_colors != _cache.end() && _colors->second.getType() == Value::Type::VECTOR){
        auto _arra = _colors->second.asValueVector();
        if(index>=0&&index<_arra.size()){
            auto _ints = CocosHelper::stringToInt(_arra.at(index).asString());
            _color = Color3B(_ints.at(0),_ints.at(1),_ints.at(2));
        }
    }
    return _color;
}