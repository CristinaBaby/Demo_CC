//
//  JuiceMachine.h
//  BHUG1066
//
//  Created by maxiang on 6/28/16.
//
//

#ifndef JuiceMachine_h
#define JuiceMachine_h

#include "MakeBaseNode.h"

class JuiceMachine : public MakeBaseNode
{
public:
    static JuiceMachine* create(int index);
    virtual bool init(int index);
    
    //是否当前用户选中的机器
    void setMachineSelect(bool isSelect);
    bool isMachineSelect() const {return _isSelect;};
    
    void onOffAction(bool isOn);
    
    //淋果汁回调
    std::function<void()> pourJuiceCallback = nullptr;
    
public:
    JuiceMachine():
    _isSelect(false),
    _isOn(false),
    _onOffButton(nullptr),
    _juiceFlow(nullptr)
    {};
    virtual ~JuiceMachine();
protected:
    
protected:
    bool _isOn;
    Button *_onOffButton;
    int _juiceIndex;
    bool _isSelect;
    Sprite *_juiceFlow;
};


#endif /* JuiceMachine_h */
