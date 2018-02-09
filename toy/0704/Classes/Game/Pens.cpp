//
//  Pens.cpp
//  TOYBM0701
//
//  Created by huangwen on 15/11/10.
//
//

#include "Pens.h"
#include "PensManager.h"
#include "GameScene.h"
#include "PensColorManager.h"

#define FILE_PATH_BACK      "game/pens/pen_background.png"
#define FILE_PATH_FRONT     "game/pens/pen_front.png"


Pens* Pens::create(int index, GameScene * game, bool net){
    Pens *widget = new Pens;
    if (widget && widget->init()) {
        widget->autorelease();
        widget->setContentSize(Size(24,200));
        widget->initPens(index, game, net);
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool Pens::initPens(int index, GameScene * game, bool net){
    
    auto back = Sprite::create(FILE_PATH_BACK);
    back->setColor(Color3B(xPcm->getColorForInitPens()));
    back->setPosition(getContentSize()/2);
    addChild(back, 1);
    
    auto front = Sprite::create(FILE_PATH_FRONT);
    addChild(front, 1);
    front->setPosition(getContentSize()/2);
    
    if (PensManager::getInstance()->createBoxBackground(index)) {
        auto background = ImageView::create("game/lock_pack/pencil_box_1.png");
        background->setName("background");
        addChild(background);
        background->setPosition(Vec2(getContentSize()/2) + Vec2(48, 0));
    }
    
    if(PensManager::getInstance()->createBoxFrontground(index))
    {
        auto tag = PensManager::getInstance()->getGroup(index);
        
        auto lock = ImageView::create("game/lock_pack/pencil_box_2.png");
        addChild(lock, 2);
        lock->setName("lock");
        lock->setPosition(Vec2(getContentSize()/2) + Vec2(-50, 0));
        lock->setTag(tag);
        lock->setTouchEnabled(true);
        lock->addTouchEventListener(CC_CALLBACK_2(GameScene::craeteUnlockAds, game));
        
        string file = "game/lock_pack/play_";
        file = net ? (file+  "1.png") : (file+ "2.png");
        auto logo = ImageView::create(file);
        logo->setName("logo");
        lock->addChild(logo);
        logo->setPosition(Vec2(lock->getContentSize()/2) + Vec2(0, 50));
    }
    
    
    return true;
}

void Pens::unLock(){
    
}
