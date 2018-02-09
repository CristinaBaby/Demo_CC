//
//  STCoverLayer.h
//  SpotDifference
//
//  Created by Steven.Xc.Tian.
//
//

#ifndef __SpotDifference__STCoverLayer__
#define __SpotDifference__STCoverLayer__

#include "cocos2d.h"

#define kCoverLayerContentTag				100000000
#define kCoverLayerTag						0xceaea999
#define kCoverLayerAnimationTime            0.4f
#define DEFAULT_BACKGROUND_COLOR            Color4B(0, 0, 0, 0)

class CoverLayer;

class BackStack
{
    std::vector<CoverLayer*> m_CoverLayerStack;

public:
	~BackStack()
    {
        m_CoverLayerStack.clear();
    }
    
	static BackStack* getInstance();
	static void destroy();
    
	void pushBackLayer(CoverLayer* node);
	CoverLayer* popBackLayer();
	CoverLayer* getTopBackLayer();
    
private:
	BackStack(){};
};


typedef std::function<void(cocos2d::Ref*)> LayerClosedCallback;


class CoverLayer : public cocos2d::LayerColor
{
    static const cocos2d::Color4B DEFAULT_COLOR;
    
public:
    static CoverLayer * create();
    static CoverLayer * create(const cocos2d::Color4B& color);
    
    /** set the target/selector of close button, when layer closed call this selector */
    inline void setCloseCallback(const LayerClosedCallback& callback)
    {
        _closedcallback = callback;
    }
    
    void closeCoverLayer(cocos2d::Ref*);
    
    cocos2d::Sprite* generateMapFrame();
    
    cocos2d::Sprite* generatePauseMapFrame();
    
protected:
    bool init();
    
    bool initWithColor(const cocos2d::Color4B& color);
    
    CoverLayer(): _closedcallback(nullptr){}
    
    LayerClosedCallback _closedcallback;
    
    
};

#endif /* defined(__SpotDifference__STCoverLayer__) */
