

#ifndef __KidsFrameWorkTest__KSAdapter__
#define __KidsFrameWorkTest__KSAdapter__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class KDAdapter : public Ref{
    
public:
    /**
     * @brief
     */
    virtual int getCount() = 0;
    
    /**
     * @brief
     */
    virtual bool isEmpty() = 0;
    
    /**
     * @brief
     */
    virtual float getItemWidth() = 0;
    
    /**
     * @brief
     */
    virtual float getItemHeight() = 0;
    
    /**
     * @brief
     */
    virtual MenuItem* getView(int position,MenuItem* cacheView,Node* parent) = 0;
};
#endif /* defined(__KidsFrameWorkTest__KSAdapter__) */
