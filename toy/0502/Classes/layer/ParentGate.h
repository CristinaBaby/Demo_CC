//
//  ParentGate.h
//  KidsBook
//
//  Created by lidong on 14-10-27.
//
//

#ifndef __KidsBook__ParentGate__
#define __KidsBook__ParentGate__

#include <iostream>
#include "cocos2d.h"
#include "BaseLayerColor.h"
USING_NS_CC;

#define ST_PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void)\
{\
return varName; \
}\
public: virtual void set##funName(varType var) \
{\
varName=var; \
}\

typedef enum
{
    Answer_UnknowValue,
    Answer_ShipValue=1,
    Answer_AirplayValue,
    Answer_BallonValue
}AnswerValue;


class ParentGateDelegate
{
public:
    virtual void parentGateBack(std::string &key_word)=0;
};

class ParentGate : public BaseLayerColor {
    
    AnswerValue _value;
    int _number;
    Menu* _menu;
    
    int _counter;
    std::string _linkStr;
public:
    bool init();
    CREATE_FUNC(ParentGate);
    ST_PROPERTY(ParentGateDelegate*, _delegate, Delegate);
    void setLinkString(const char* info);
    
    void registerWithMyTouchDispatcher();
//    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    
public :
    std::string getStr(AnswerValue pValue);
    const char* getBlankStr(AnswerValue value);
    void doAction(cocos2d::Ref* pSender);
    void gotoNext(float dt);
    void gotoBack();
    
    cocos2d::Label* getMyLabel(std::string label_font , const char *label_name , int size);
    
    
};

#endif /* defined(__KidsBook__ParentGate__) */
