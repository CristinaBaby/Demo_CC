//
//  SaveLayer.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include <cocos2d.h>
#include "UITools.h"

class GameScene;

class SaveLayer : public Layer
{
public :
    static SaveLayer* create(Image *image, GameScene * game);
    virtual bool init(Image *image, GameScene * game);
    
    void onButton(Ref* pSender, Widget::TouchEventType type);
    
    void createParticle(float dt);
    void stopParticle(float dt);
    virtual void onExit() override;
    
private:
    Image* _pictureImage;
    ParticleSystemQuad * _particle;
    ParticleSystemQuad * _bangbang;
    ParticleSystemQuad * _candy;
    GameScene *          _game;
};