//
//  ShadeSprite.h
//  JuiceMaker
//
//  Created by luotianqiang1 on 3/31/15.
//
//

#ifndef __JuiceMaker__ShadeSprite__
#define __JuiceMaker__ShadeSprite__

#include<iostream>
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
#include "KDheader.h"
NS_KD_BEGIN
enum class shde_action_type {
	shade_show,
	shade_hide
};

class ShadeSprite : public Node {
public:
    ShadeSprite();
       static ShadeSprite* create(Sprite* srcS,Sprite* maskS,shde_action_type type = shde_action_type::shade_hide);
    
   virtual ~ShadeSprite();
    static ShadeSprite* create(string fileName,string maskName, Vec2 srcVec = Vec2::ZERO
                               ,Vec2 maskVec = Vec2::ZERO, shde_action_type type = shde_action_type::shade_hide);
   void setSpeed(float speed);
   void setDistance(float distace);
   virtual void resumAction();
   void pasuAction();
   function<void(ShadeSprite*)> endCallback;
    Sprite* getSrcSprite();
    Sprite* getMaskSprite();
    void resetScrSprite();
    void setNewSrc(Sprite* s);
    bool isRunEnd(){return  isEnd;}
protected:
    virtual bool initSprite(string fileName,string maskName, Vec2 srcVec
                            ,Vec2 maskVec,shde_action_type type);

    virtual void initSprite(Sprite* scrSprite,Sprite* maskSprite);
    
    Sprite* srcSprite,*maksSprite;
    ClippingNode* clipNode;
	float speed,distance;
	bool isRunning;
	bool isEnd;
	virtual void endAction();
	shde_action_type curentType;
    void drink(float);
    float startPosition;
    int actionTag;
};
NS_KD_END
#endif /* defined(__JuiceMaker__ShadeSprite__) */
