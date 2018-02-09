//
//  STSlider.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#ifndef __LVUP002__STSlider__
#define __LVUP002__STSlider__

#include "cocos2d.h"
#include <GUI/CCControlExtension/CCControlSlider.h>
//#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class STSlider : public ControlSlider
{
public:
    /**
     * Creates a slider with a given background sprite and a progress bar and a
     * thumb item.
     *
     * @see initWithBackgroundSprite:progressSprite:thumbMenuItem:
     */
    static STSlider* create(cocos2d::Sprite * backgroundSprite, cocos2d::Sprite* pogressSprite, cocos2d::Sprite* thumbSprite);
    
    static STSlider* create(cocos2d::Sprite * backgroundSprite, cocos2d::Sprite* pogressSprite, cocos2d::Sprite* thumbSprite,
                            cocos2d::Ref* target, cocos2d::SEL_CallFuncN selector);
    
    STSlider();
    virtual ~STSlider();
    
    //    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    //    virtual void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    /** set the target/selector of the slider*/
    void setTarget(Ref *target, cocos2d::SEL_CallFuncN selector);
    int  square(int n);
protected:
    cocos2d::Ref*          m_pListener;
    cocos2d::SEL_CallFuncN      m_pfnSelector;
    
    
};

#endif /* defined(__LVUP002__STSlider__) */
