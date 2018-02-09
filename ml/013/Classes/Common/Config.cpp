//
//  Config.cpp
//  Make
//
//  Created by QAMAC01 on 15-2-9.
//
//

#include "Config.h"
#include "GameMaster.h"
#include "CCImageColorSpace.h"
#include "MyMakeConfig.h"
#include "MathUtil.h"
#include "GameMaster.h"

int Config::getShape()
{
    
    log("current shapeIdx is %d" , GameMaster::getInstance() -> getShapeIdx());
    return GameMaster::getInstance() -> getShapeIdx();
}

const char* Config::getShapeScribbleName()
{
    return __String::createWithFormat("decoration/syrup_shape/syrup_shape%d.png",getShape()) -> getCString();
}


Vec2 Config::getSnowconePos()
{
    return Vec2(316, 500);
}


std::string Config::getShapeName()
{
    
    MyMakeConfig::getInstance()->setTypeName("shape");
    
    log("current shapename is %s" , MyMakeConfig::getInstance() -> vec_UnitName.at(getShape()) ->getCString());

    return MyMakeConfig::getInstance() -> vec_UnitName.at(getShape()) ->getCString() ;
}

const char* Config::getChoosenName(const char* pName)
{

}


Texture2D* Config::getShapeScribbleTexture(int h, int s, int v)
{
    auto orImg = new Image();
    orImg -> initWithImageFile(getShapeScribbleName());
    
    auto img = CCImageColorSpace::imageWithHSB(orImg, h, s, v);
    Texture2D * tex = new Texture2D();
    tex->initWithImage(img);
    
    
    
    return tex;
}

Texture2D* Config::getShapeScribbleTexture(int h, int s, int v, int idx)
{
    auto orImg = new Image();
    orImg -> initWithImageFile(MyMakeConfig::getInstance() -> vec_UnitName.at(getShape()) -> getCString());
    orImg -> autorelease();
    auto img = CCImageColorSpace::imageWithHSB(orImg, h, s, v);
    Texture2D * tex = new Texture2D();
    tex->initWithImage(img);
    tex -> autorelease();
    return tex;
}




void Config::flashAction(Sprite* sp, bool isRepeat, int times)
{
    auto flash = Sequence::create(Hide::create(),
                                  DelayTime::create(0.5),
                                  Show::create(),
                                  DelayTime::create(0.5),
                                  NULL);
    if (isRepeat)
    {
        sp -> runAction(RepeatForever::create(flash));
    }
    else
    {
        sp -> runAction(Sequence::create(
                                         Repeat::create(flash, times),
                                         Hide::create()
                                         , NULL));
    }

}

void Config::zoomAction(Sprite* sp, bool isRepeat, int times)
{
    auto zoom = Sequence::create(ScaleTo::create(0.5, 0.8),
                                 ScaleTo::create(0.5, 1),
                                  NULL);
    if (isRepeat)
    {
        sp -> runAction(RepeatForever::create(zoom));
    }
    else
    {
        sp -> runAction(Sequence::create(
                                         Repeat::create(zoom, times),
                                         Hide::create()
                                         , NULL));
    }
    
}


