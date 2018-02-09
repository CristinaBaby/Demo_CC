//
//  PercentChangeComponent.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/18.
//
//

#ifndef PercentChangeComponent_hpp
#define PercentChangeComponent_hpp

#include "LQComponent.h"

/**
 根据百分比不断切换ower的图片
 */
class PercentChangeComponent: public LQComponent {
    DECLARE_COMPONENT_DYNCLASS(PercentChangeComponent);
public:
    PercentChangeComponent();
    void setFrameFile(const vector<string>&_frameFile);
    void invoking(float percent);
    void stop();
    virtual void onAdd();
    float getCurrentPercent();
protected:
    void initNode();
protected:
    Vector<Sprite*> _changeNodes;
    vector<string> _frameFile;
    int _count;
    float _currentPercent;
};

#endif /* PercentChangeComponent_hpp */
