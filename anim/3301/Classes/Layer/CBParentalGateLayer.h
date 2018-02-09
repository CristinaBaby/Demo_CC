//
//  CBParentalGateLayer.h
//  ColorBook
//
//  Created by maxiang on 4/30/15.
//
//

#ifndef __ColorBook__CBParentalGateLayer__
#define __ColorBook__CBParentalGateLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class ParentalGateProtocol
{
public:
    
    virtual void parentalGatePass(int tag) = 0;
};

typedef enum
{
    ItemType_Ship = 0,
    ItemType_AirPlane,
    ItemType_AirBall,
}ItemType;

typedef std::function<void()> parentalGateCallback;

class ParentalGateLayer : public cocos2d::LayerColor
{
public:
    static ParentalGateLayer* create();
    
    //set parental gate delegate
    void setDelegate(ParentalGateProtocol *delegate, int tag);
    void setCallback(const parentalGateCallback& callback);

    //init methods
    virtual bool init();
    
    //show parent gate in layer
    void showInLayer(cocos2d::Layer *layer);
    
    //close parent gate layer
    void close();
    
private:
    void generateRandomValue(char& valueString);
    void closeAction();
    void item1Action();
    void item2Action();
    void item3Action();
    
    //called by menu item action
    void chooseItem(ItemType type);
    
private:
    cocos2d::Sprite *_bgSprite;

    ParentalGateProtocol *_delegate;
    ItemType _itemType;
    int _itemTapTimes;
    int _tapCount;
    int _tag;
    parentalGateCallback _callback;
};


CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBParentalGateLayer__) */
