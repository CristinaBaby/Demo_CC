//
//  HomeCart.cpp
//  Hotdog
//
//  Created by wuguiling on 16/12/2.
//
//

#include "HomeCart.h"

bool HomeCart::init()
{
    if (!Node::init()) {
        return false;
    }
    ArmatureDataManager::getInstance()->addArmatureFileInfo("truck/truck0.png", "truck/truck0.plist", "truck/truck.ExportJson");
    
    Armature* lArmature = Armature::create("truck");
    lArmature->setPosition(Vec2::ZERO);
    this->addChild(lArmature);
    m_pCar = lArmature;
    return true;
}

void HomeCart::stopCartAction()
{
    ArmatureAnimation* lArmation = m_pCar->getAnimation();
    lArmation->stop();
}

void HomeCart::runCar()
{
    ArmatureAnimation* lArmation = m_pCar->getAnimation();
    lArmation->play("run",-1,1);
}

void HomeCart::stopRunCar()
{
    ArmatureAnimation* lArmation = m_pCar->getAnimation();
    lArmation->play("pause",-1,1);
}

