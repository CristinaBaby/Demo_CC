//
//  ShadeSprite.cpp
//  JuiceMaker
//
//  Created by luotianqiang1 on 3/31/15.
//
//

#include "ShadeSprite.h"
USING_NS_KD;

ShadeSprite* ShadeSprite::create(string fileName,string maskName, Vec2 srcVec
                                 ,Vec2 maskVec,shde_action_type type) {
    auto sha = new ShadeSprite();
    if(sha&&sha->initSprite(fileName, maskName,srcVec,maskVec,type)) {
        sha->autorelease();
        return sha;
    }
    return nullptr;
}


ShadeSprite* ShadeSprite::create(Sprite* srcS,Sprite* maskS, shde_action_type type) {
    auto sha = new ShadeSprite();
    sha->autorelease();
    sha->curentType = type;
    sha->initSprite(srcS, maskS);
    return sha;
}


 void ShadeSprite::initSprite(Sprite* srcS,Sprite* maskS) {
     srcSprite = srcS;
     maksSprite = maskS;
     srcSprite->retain();
     maksSprite->retain();
     setContentSize(maksSprite->getContentSize());
     setAnchorPoint(Vec2(0.5f,0.5f));
     clipNode = ClippingNode::create();
     clipNode->setAlphaThreshold(0.05);
     clipNode->setStencil(maksSprite);
     addChild(clipNode);
     clipNode->addChild(srcSprite);
     distance = srcSprite->getContentSize().height;
     startPosition = srcSprite->getPosition().y;
}

bool ShadeSprite::initSprite(string fileName,string maskName, Vec2 srcVec
                             ,Vec2 maskVec,shde_action_type type) {
    if((srcSprite = Sprite::create(fileName))== nullptr || (maksSprite = Sprite::create(maskName))== nullptr)
        return false;
	srcSprite->retain();
	maksSprite->retain();

	curentType = type;

    setContentSize(maksSprite->getContentSize());
	setAnchorPoint(Vec2(0.5f,0.5f));
    clipNode = ClippingNode::create();
    clipNode->setAlphaThreshold(0.05);
    maksSprite->setAnchorPoint(Vec2(0,0));
    maksSprite->setPosition(maskVec);
    clipNode->setStencil(maksSprite);
    addChild(clipNode);
	switch (curentType)
	{
	case shde_action_type::shade_show:
		srcSprite->setAnchorPoint(Vec2(0,1));
		break;
	case shde_action_type::shade_hide:
		srcSprite->setAnchorPoint(Vec2(0,0));
		break;
	default:
		break;
	}
    srcSprite->setPosition(srcVec);
    clipNode->addChild(srcSprite);
	distance = srcSprite->getContentSize().height;
    startPosition = srcSprite->getPosition().y;
    return true;
}

Sprite* ShadeSprite::getSrcSprite() {
    return srcSprite;
}

Sprite* ShadeSprite::getMaskSprite() {
    return maksSprite;
}

 void ShadeSprite::resumAction() {
	 if(isRunning||isEnd)
		 return;
     srcSprite->stopActionByTag(actionTag);
	 isRunning = true;
     float dis = distance;
     switch (curentType)
     {
         case shde_action_type::shade_show:
			 dis = distance;
			 break;
		 case shde_action_type::shade_hide:
			 dis = -distance;
			 break;
		 default:
			 break;
     }
     auto time = abs((abs(startPosition-srcSprite->getPosition().y)-abs(distance)))/speed;
     auto moveBy = startPosition-srcSprite->getPosition().y + dis;
     auto thisAction  = Sequence::create(MoveBy::create(time,Vec2(0,moveBy)), CallFuncN::create(CC_CALLBACK_0(ShadeSprite::endAction,this)), nullptr);
     thisAction->setTag(actionTag);
     srcSprite->runAction(thisAction);

 }

void ShadeSprite::resetScrSprite() {
    srcSprite->stopActionByTag(actionTag);
    srcSprite->setPositionY(startPosition);
    isEnd = false;
    isRunning = false;
}

 void ShadeSprite::pasuAction() {
    srcSprite->stopActionByTag(actionTag);
    isRunning = false;
 }

  void ShadeSprite::endAction() {
	  isEnd = true;
	  if(nullptr != endCallback)
		  endCallback(this);
 }

void ShadeSprite::setSpeed(float speed) {
	this->speed = speed;
 }

void ShadeSprite::setDistance(float distace) {
	this->distance = distace;
}

ShadeSprite::ShadeSprite() {
    srcSprite = nullptr;
    maksSprite = nullptr;
    clipNode = nullptr;
	speed = 30;
	distance = 0;
	endCallback = nullptr;
	isRunning = false;
	isEnd = false;
    actionTag = 9876;
}

void ShadeSprite::setNewSrc(Sprite* s){
    s->retain();
    CC_SAFE_RELEASE_NULL(srcSprite);
    srcSprite = s;
    clipNode->addChild(srcSprite);
}

ShadeSprite::~ShadeSprite() {
    CC_SAFE_RELEASE_NULL(srcSprite);
    CC_SAFE_RELEASE_NULL(maksSprite);
}

void ShadeSprite::drink(float) {
    
}
