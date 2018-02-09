//
//  GameControl.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/3.
//
//

#include "GameControl.h"

void GameControl::changeLayer(Layer* _layer) {
    auto _scne = Director::getInstance()->getRunningScene();
    if(_scne == nullptr){
        _scne = Scene::create();
        Director::getInstance()->runWithScene(_scne);
    } else {
        Director::getInstance()->popToRootScene();
        _scne = Scene::create();
        Director::getInstance()->replaceScene(_scne);
    }
    _scne->addChild(_layer);
}
void GameControl::pushBackLayer(Layer* _layer){
    Director::getInstance()->getRunningScene()->addChild(_layer);
}
void GameControl::changeScene(Scene* _scene){
    Director::getInstance()->popToRootScene();
    Director::getInstance()->replaceScene(_scene);
}
void GameControl::pushBackScene(Scene* _scene){
    Director::getInstance()->pushScene(_scene);
}
void GameControl::goToPreLayer(Layer* _layer){
    auto _runningScene = Director::getInstance()->getRunningScene();
    if(_layer != nullptr)
        _runningScene->removeChild(_layer);
}
void GameControl::goToPreScene(){
    Director::getInstance()->popScene();
}