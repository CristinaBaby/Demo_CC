//
//  TypeSprite.cpp
//  Make
//
//  Created by QAMAC01 on 15-2-11.
//
//

#include "TypeSprite.h"

TypeSprite* TypeSprite::create(const char* pzFileName)
{
    TypeSprite* pRet = new TypeSprite();
    if (pRet && pRet -> initWithFile(pzFileName))
    {
        pRet -> pzFileName_ = pzFileName;
        pRet -> selected_ = false;
        pRet -> autorelease();
        return pRet;
    }
    else
        delete pRet;
    return nullptr;
}


void TypeSprite::setHighlighted(bool isHighLighted)
{
    if (isHighLighted)
    {
        if(this -> getSelected())
        {
            this -> setTexture(pzFileName_);
            this -> setSelected(false);
        }
        else
        {
            this -> setTexture(this -> getHighLightedFileName().c_str());
            this -> setSelected(true);
        }
    }
    else
    {
        this -> setTexture(pzFileName_);
        this -> setSelected(false);
    }
}

void TypeSprite::setSpriteSelected(bool isHighLighted)
{

    this -> setTexture(this -> getHighLightedFileName().c_str());

}



std::string TypeSprite::getHighLightedFileName()
{
    std::string highlightedStr = pzFileName_;
    int a =  (int)highlightedStr.find(".");
    highlightedStr.replace(a, 20, "_h.png");
    return highlightedStr;
}


