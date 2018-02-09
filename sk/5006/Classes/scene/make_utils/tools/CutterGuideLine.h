//
//  CutterGuideLine.h
//  BHUG1062
//
//  Created by maxiang on 5/16/16.
//
//

#ifndef CutterGuideLine_h
#define CutterGuideLine_h

#include "cocos2d.h"
using namespace cocos2d;

class CutterGuideLine : public Node
{
public:
    CREATE_FUNC(CutterGuideLine);
    virtual bool init() override;
    
    void start();
    void stop();
    
public:
    CutterGuideLine():
    _progressTimer(nullptr),
    _indicator(nullptr)
    {};
    virtual ~CutterGuideLine();
    
protected:
    ProgressTimer *_progressTimer;
    Sprite *_indicator;

};

#endif /* CutterGuideLine_h */
