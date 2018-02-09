//
//  ModelLayer.h
//  PaintDress
//
//  Created by maxiang on 6/29/15.
//
//

#ifndef __PaintDress__ModelLayer__
#define __PaintDress__ModelLayer__

#include "cocos2d.h"
#include "cocosGUI.h"
#include "../Model/ResManager.h"

class ModelLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(ModelLayer);
    virtual bool init() override;
    ModelLayer();
    virtual ~ModelLayer();

    void setRes(Res* res);
    
    void setHair(const std::string& hair, const cocos2d::Vec2& position);
    void setDress(const std::string& dress, const cocos2d::Vec2& position);
    void setBottom(const std::string& bottom, const cocos2d::Vec2& position);
    void setTop(const std::string& top, const cocos2d::Vec2& position);
    void setAccessory(const std::string& accessory, const cocos2d::Vec2& position);
    void setShoes(const std::string& shoes, const cocos2d::Vec2& position);

    Model& getModel(){return _model;};
    
protected:
    Model _model;
    
protected:
    cocos2d::Sprite *_modelBodySprite;
    cocos2d::Sprite *_hair;
    cocos2d::Sprite *_dress;
    cocos2d::Sprite *_bottom;
    cocos2d::Sprite *_top;
    cocos2d::Sprite *_shoes;
    cocos2d::Sprite *_accessory;
    
    float _w;
    float _h;
};

#endif /* defined(__PaintDress__ModelLayer__) */
