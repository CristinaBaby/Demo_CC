//
//  CheckEatPercent.hpp
//  italianchef
//
//  Created by luotianqiang1 on 15/10/29.
//
//

#ifndef CheckEatPercent_hpp
#define CheckEatPercent_hpp

#include "cocos2d.h"
USING_NS_CC;
class CheckEatPercent:public Component{
public:
    CheckEatPercent();
    CREATE_FUNC(CheckEatPercent);
    void setEnabled(bool b);
    void reset();
    void addCheckNode(Node* _node);
protected:
   virtual bool init();
    Vector<Node*> _checkNodes;
   virtual void onEnter();
    virtual void onExit();
    virtual void checkSchedle(float);
    Scheduler* _scheduler;
    size_t calcute(Image *pImage);
    size_t _pixcelBegin;
    size_t _pixcelEnd;
    Image *_image;
public:
    std::function<void(float)> eatPercentageCallback;
};

#endif /* CheckEatPercent_hpp */
